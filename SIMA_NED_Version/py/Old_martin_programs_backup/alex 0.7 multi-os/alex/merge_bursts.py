from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event, Bool, List
import subprocess
import csv
import os
import numpy as np
import struct
from .models import ReducParams
import string
from sys import platform as _platform

class LazyFile(HasTraits):
    """ Wrapper that represents a file lazely as an array. """
    file_name = Str
    setting = Instance(ReducParams)
    # data = Array

    # def __init__(self, file_name, *args, **kwargs):
    #     self.file_name = file_name
    #     self.setting = setting
    def __enter__(self):
        """ Returns the structured data as an array """
        # self.data = self._read_sm_to_np()
        return self._read_to_np()

    def __exit__(self, type, value, traceback):
        return False

    def _read_to_np():
        raise NotImplementedError('')

class BurstFile(LazyFile):
    """ Manager for a .csv file containing Bursts.
    """
    def _get_header_size(self):
        """ The header of the csv file contains metadata. The length of this
        section will be 3 + the number of setting fed to alex_tools.
        (3 because we want to skip the words METADATA, DATA and the header as well.)
        """
        return 3 + len(self.setting.str_list())

    def _get_dtype(self):
        return [(n, 'float64') for n in ['Epr', 'Sraw', 'Len', 'F_Aexc_Aem', 'F_Dexc_Dem', 'F_Dexc_Aem', 'F_Aexc_Dem', 'Tau', 'BurstStart', 'BurstEnd', 'bvaSigEpr', 'sigEpr', 'nWindows']]

    def _read_to_np(self):
        """ Opens a file and turns the result into a structured array """
        return np.genfromtxt(self.file_name, delimiter = ",", skip_header=self._get_header_size(), dtype = self._get_dtype(), invalid_raise = False)

class PhotonArrivalFile(LazyFile):
    """ Manager for a single photon arrival file (currently .sm is ASSUMED).
    The object is initiated with a file_name but does not proceed to opening the file.
    This is done lazely to conserve resources; 'with PhotonArrivalFile('file.sm'):' causes the file to be opened on runtime.
    This is because typically we are not interested in the photons, the C++ core does that.
    Some views however, might want this possibility.
    """
    def __init__(self, *args, **kwargs):
        super(PhotonArrivalFile, self).__init__(*args, **kwargs)

    def _get_number_photons(self, fmt = '>Qxxx?'):
        # After the text 'Arrival Time Counter' is an unsigned long that gives us the number of characters in the run
        with open(self.file_name, 'rb') as file_handle:
            file_handle.seek(0x2e)
            fmt_no_char = '>L'
            size_fmt_no_char = struct.calcsize(fmt_no_char)
            # In the no_char, we have to compensate for the fact that the ulong with the number of char includes part of the header so we substract that.
            no_char = 0x2e + 4 - 0xa5 + struct.unpack(fmt_no_char, file_handle.read(size_fmt_no_char))[0]
        no_photons = no_char//struct.calcsize(fmt)
        return no_photons

    def _read_to_np(self):
        """ Reads an sm file and returns a structured array with all the info. """
        record_dtype = np.dtype([
            ('ArrivalTime', '>u8'),
            ('FinalChannel', '>u2'), # Risky proposition. We assign the paddingbytes to this channel, so we get the right columns in our array. It does not exist in the file itself! See smfile documentation.
            ('Channel', '>u2'),
        ])
        with open(self.file_name, 'rb') as file_handle:
            no_photons = self._get_number_photons()
            file_handle.seek(0xa5)
            # Instead of python struct, read to array using numpy fromfile.
            data = np.fromfile(file_handle, dtype=record_dtype, count=no_photons)

        new_data = np.empty(data.shape, dtype = [('ArrivalTime','float64'), ('FinalChannel', 'int16'), ('Channel', 'bool')])
        new_data['ArrivalTime'] = data['ArrivalTime'].astype('float64', copy = False) * self.setting.time_resolution
        new_data['FinalChannel'] = data['FinalChannel']
        new_data['Channel'] = data['Channel']
        self._identify_channels(new_data)
        return new_data

    def _identify_channels(self, data):
        """ Identifies the real channel for every photon, using knowledge of
        which laser was on, and where the photon was detected.
        Modifies data in place, so no return needed.
        """
        # The arrival time of the photon in the bin
        arrival_time_binned = np.mod(data['ArrivalTime'], self.setting.alex_period)
        active_laser = arrival_time_binned < self.setting.first_half # if true, green laser active, else red
        # Replace FinalChannel by a value from 0-3, where 0 -> DD, 1 -> AA, 2 -> DA, 3 -> AD
        data['FinalChannel'][np.logical_and(data['Channel'], active_laser)] = 0    # Laser green, detection also happend in green
        data['FinalChannel'][np.logical_and(~data['Channel'], ~active_laser)] = 1  # Laser red, detection also happend in red
        data['FinalChannel'][np.logical_and(~data['Channel'], active_laser)] = 2   # Laser green, detection happend in red
        data['FinalChannel'][np.logical_and(data['Channel'], ~active_laser)] = 3   # Laser red, detection happend in green

class Dataset(HasTraits):
    """ Every dataset consists of two parts: a PhotonArrivalFile and a BurstFile.
    The PhotonArrivalFile is mandatory, the BurstFile can always be generated from it.
    The reason for keeping the two files together is so the burst viewer can request a photon subset from the PhotonArrivalFile.
    """
    burst_file = Instance(BurstFile)
    photon_file = Instance(PhotonArrivalFile)

    def __init__(self, file_name, settings, *args, **kwargs):
        super(Dataset, self).__init__(*args, **kwargs)
        self.photon_file = PhotonArrivalFile(file_name = file_name, setting = settings)
        # Check if the corresponding BurstFile exists
        burst_file_name = self._burst_file_name(file_name)
        if self._file_exists(burst_file_name, settings):
            self.burst_file = BurstFile(file_name = burst_file_name, setting = settings)
        else:
            # No mathing burst_file exists yet. Therefore run burst_search again.
            self.run_burst_search_cpp(file_name, settings)
            self.burst_file = BurstFile(file_name = burst_file_name, setting = settings)

    def get_bursts(self):
        """ Retrieves all the bursts from the burst_file."""
        with self.burst_file as bursts:
            return bursts

    def run_burst_search_cpp(self, file_name, settings):
        """ Calls the sm2burst C++ utility on the given .sm file """
        # On windows, \ characters must be doubly escaped for the cpp program to work.
        if _platform == 'win32':
            string.join(file_name.split('\\'), '\\\\')
        subprocess_path = "./alex/alex_tools.exe" if _platform == 'win32' else "./alex/alex_tools"
        try:
            print subprocess.check_call([subprocess_path, file_name] + settings.str_list())
        except subprocess.CalledProcessError:
            # Wrapping sm file failed
            print "Subprocess error"

    @staticmethod
    def _burst_file_name(file_name):
        """ This function is terrible. It assumes so much, bad things are bound
        to happen. We simply append '.wrapped.csv' to the filenames, simply
        because the current version of alex_tools does that.
        """
        return file_name + u'.wrapped_new.csv'

    def _file_exists(self, wrapped_file_name, settings):
        """ Private helper function which checks if a wrapped file by this name
        already exists, and whether or not it's METADATA mathes 'settings'.
        If so, the file is considered existent. It works by adding all metadata
        to a set and comparing that to set obtained from the 'settings' obj.
        If these sets are equal, return True.

        WARNING:
        There is a very small chance for false positives because this version
        does not check on a per parameter basis if they are equal.
        """
        try:
            with open(wrapped_file_name, 'rb') as file_handle:
                csvreader = csv.reader(file_handle, delimiter = ",")
                raw_data = list(csvreader)
                metadata = []
                for key, row in enumerate(raw_data):
                    if row == ['METADATA']:
                        continue
                    elif row == ['DATA']: # End of metadata
                        break
                    else:   # we are still in the metadata section, so store the items in a set.
                        # Cast the second column to str and add to set
                        metadata.append(str(row[1]))
                else:   # This point should never be reached. If we did, throw error
                    raise EOFError('End of file reached without finding \'DATA\'. This indicates an impropper wrapped burst file.')
                # Naively assumes the chance of two set's checking out the same while they are not is so slim, this assumption is save.
                print metadata, set(settings.str_list())
                return set(self._str_list_to_types(metadata)) == set(self._str_list_to_types(settings.str_list()))
        except IOError:
            return False

    @staticmethod
    def _str_list_to_types(str_list):
        """ Converts the items in a list of strings to Floats, Ints or str and returns it. """
        result = []
        for item in str_list:
            try:
                result.append(float(item))
            except TypeError:
                try:
                    result.append(int(item))
                except TypeError:
                    result.append(str(item))
        return result

class DataManager(HasTraits):
    """ Responsible for handling the various files used in analysis.
    Users specify .sm files for this object to use.
    Then, if the user decides on an analysis option, this object will run the desired burstsearch on the .sm files.
    The object than loads the resulting datafiles from hdd and merges them into a single array.
    In the background, the filenames are remembered in the Dataset objects in case they are needed again.
    """
    # Path containing the .sm files
    sm_path = Str
    # List containing all sm files to be converted to wrapped burst files
    files = ListUnicode
    # Setting to provide sm2burst.cpp
    setting = Instance(ReducParams)
    # Read-only property of resulting wrapped file names. If write_wrapped_files == False, this is merely used as a counter.
    datasets = List(Dataset)
    # Combined data extracted from files
    merged_data = Array
    # This is fired when the user orders us to execute sm2burst on files
    load = Event
    # Status of how many datasets have been processed and ready for merger.
    number_of_dataset = Int
    number_loaded = Int

    def execute(self):
        """ If a load event is fired, we (re)initiate the datasets property and (re)initiate merged_data """
        self.datasets = []
        self.number_of_dataset = len(self.files)
        data_to_merge = []
        for key, file_name in enumerate(self.files):
            self.datasets.append(Dataset(file_name, self.setting))
            data_to_merge.append(self.datasets[-1].get_bursts())
            self.number_loaded = key
        self.merged_data = np.concatenate(data_to_merge)

    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    @on_trait_change('sm_path')
    def set_files(self):
        """ Responsible for finding .sm files in the given dir """
        # Make sm_path correct for this os, just in case. Dirty, I know. Let's hope enaml fixes this in the file dlg
        self.sm_path = string.join(self.sm_path.split('/'), os.sep)
        self.files = [os.path.join(self.sm_path, item) for item in os.listdir(self.sm_path) if item.split('.')[-1] == 'sm'] if self.sm_path else []



# class Sm2burst(HasTraits):
#     # Path containing the .sm files
#     sm_path = Str
#     # List containing all sm files to be converted to wrapped burst files
#     files = ListUnicode
#     # Setting to provide sm2burst.cpp
#     setting = Instance(ReducParams)
#     # Read-only property of resulting wrapped file names. If write_wrapped_files == False, this is merely used as a counter.
#     wrapped_files = ListUnicode
#     # Combined data extracted from files
#     merged_data = Array
#     # Used for displaying progress
#     len_files = Int
#     len_wrapped_files = Int
#     # This is fired when the user orders us to execute sm2burst on files
#     load = Event

#     def run_burst_search_cpp(self, file_name):
#         """ Calls the sm2burst C++ utility on the given .sm file """
#         print file_name
#         try:
#             print subprocess.check_call(["./alex/alex_tools", file_name] + self.setting.str_list())
#         except subprocess.CalledProcessError:
#             # Wrapping sm file failed
#             print "Subprocess error"
#             pass

#     def get_number_photons(self, file_name):
#         with open(file_name, 'rb') as f:
#             assert isinstance(f, file)
#             return self._get_number_photons(f)

#     def execute(self):
#         self.wrapped_files = []
#         for file_name in self.files:
#             wrapped_file_name = self._wrapped_file_name(file_name)
#             # Check if a file with the same name and metadata already exists.
#             # If so, skip
#             if not self._file_exists(wrapped_file_name, self.setting):
#                 print 'Does not exist, running again.'
#                 self.run_burst_search_cpp(file_name)
#                 data = self._extract_data(wrapped_file_name)
#             else:
#                 print 'Does exist'
#                 data = self._extract_data(wrapped_file_name)
#                 print "put into data: ", data

#             print self.merged_data.shape
#             if self.merged_data.shape[0]:
#                 print 'merging'
#                 self.merged_data = np.hstack((self.merged_data, data))
#             else:
#                 print 'not merging'
#                 self.merged_data = data
#             print self.merged_data, self.merged_data['Tau']
#             # This funky form is needed so the trait gets changed. Appending does not work.
#             self.wrapped_files = self.wrapped_files + [wrapped_file_name]

#     #--------------------------------------------------------------------------
#     # Private API
#     #--------------------------------------------------------------------------

#     @on_trait_change('sm_path')
#     def set_files(self):
#         """ Responsible for finding .sm files in the given dir """
#         self.files = ['{}/{}'.format(self.sm_path, item) for item in os.listdir(self.sm_path) if item.split('.')[-1] == 'sm'] if self.sm_path else []
#         self.wrapped_files = []

#     @on_trait_change('files')
#     def set_len_files(self):
#         self.len_files = len(self.files)

#     @on_trait_change('wrapped_files')
#     def set_len_wrapped_files(self):
#         self.len_wrapped_files = len(self.wrapped_files)

#     @staticmethod
#     def _wrapped_file_name(file_name):
#         """ This function is terrible. It assumes so much, bad things are bound
#         to happen. We simply append '.wrapped.csv' to the filenames, simply
#         because the current version of sm2burst does that.
#         """
#         return file_name + u'.wrapped_new.csv'

#     def _get_number_photons(self, file_handle, fmt = '>Qxxx?'):
#         # After the text 'Arrival Time Counter' is an unsigned long that gives us the number of characters in the run
#         file_handle.seek(0x2e)
#         fmt_no_char = '>L'
#         size_fmt_no_char = struct.calcsize(fmt_no_char)
#         # In the no_char, we have to compensate for the fact that the ulong with the number of char includes part of the header so we substract that.
#         no_char = 0x2e + 4 - 0xa5 + struct.unpack(fmt_no_char, file_handle.read(size_fmt_no_char))[0]
#         no_photons = no_char//struct.calcsize(fmt)
#         return no_photons

#     def _read_sm(self, file_name):
#         """ Reads an sm file and returns a structured array with all the info. """
# #         print 'read sm'
#         fmt = '>Qxxx?'
#         size = struct.calcsize(fmt)
#         with open(file_name, 'rb') as file_handle:
#             no_photons = self._get_number_photons(file_handle)

#             # Read until no_photons is reached
#             file_handle.seek(0xa5)
#             chunk = file_handle.read(size)
#             data = []
#             cnt = 0
#             while cnt < no_photons:
#                 cnt += 1
#                 try:
#                     data.append(struct.unpack(fmt, chunk) + (0, )) # Returns (Q, ?). Add another dimension to it which we can fill later.
#                 except struct.error:
#                     break
#                 else:
#                     chunk = file_handle.read(size)

#         new_data = np.array(data, dtype = [('ArrivalTime',float), ('Channel', 'bool'), ('FinalChannel', 'int64')])
#         new_data['ArrivalTime'] = new_data['ArrivalTime'] * self.setting.time_resolution
# #         print 'sm read'
#         self._identify_channels(new_data)
#         return new_data

#     def _read_sm_np(self, file_name):
#         """ Reads an sm file and returns a structured array with all the info. """
#         record_dtype = np.dtype([
#             ('ArrivalTime', '>u8'),
#             ('FinalChannel', '>u2'), # Dangerous proposition. We assign the paddingbytes to this channel, so we get the right columns in our array. It does not exist in the file itself!
#             ('Channel', '>u2'),
#         ])
#         with open(file_name, 'rb') as file_handle:
#             no_photons = self._get_number_photons(file_handle)
#             print no_photons
#             file_handle.seek(0xa5)
#             # Instead of python struct, read to array using numpy fromfile.
#             data = np.fromfile(file_handle, dtype=record_dtype, count=no_photons)

#         new_data = np.empty(data.shape, dtype = [('ArrivalTime','float64'), ('FinalChannel', 'int16'), ('Channel', 'bool')])
#         new_data['ArrivalTime'] = data['ArrivalTime'].astype('float64', copy = False) * self.setting.time_resolution
#         new_data['FinalChannel'] = data['FinalChannel']
#         new_data['Channel'] = data['Channel']
#         self._identify_channels(new_data)
#         return new_data

#     def _identify_channels(self, data):
#         """ Identifies the real channel for every photon, using knowledge of
#         which laser was on, and where the photon was detected.
#         Modifies data in place, so no return needed.
#         """
#         # The arrival time of the photon in the bin
#         arrival_time_binned = np.mod(data['ArrivalTime'], self.setting.alex_period)
#         active_laser = arrival_time_binned < self.setting.first_half # if true, green laser active, else red
#         # Replace FinalChannel by a value from 0-3, where 0 -> DD, 1 -> AA, 2 -> DA, 3 -> AD
#         data['FinalChannel'][np.logical_and(data['Channel'], active_laser)] = 0    # Laser green, detection also happend in green
#         data['FinalChannel'][np.logical_and(~data['Channel'], ~active_laser)] = 1  # Laser red, detection also happend in red
#         data['FinalChannel'][np.logical_and(~data['Channel'], active_laser)] = 2   # Laser green, detection happend in red
#         data['FinalChannel'][np.logical_and(data['Channel'], ~active_laser)] = 3   # Laser red, detection happend in green
#         print 'channels identified'

#     def _burst_search_binned(self, data):
#         print 'binned burstsearch mode'
#         # Logic for this sentence taken from sm2burst.cpp
#         number_of_bins = int((data['ArrivalTime'][-1] - data['ArrivalTime'][0])/self.setting.binning_time) + 1
#         print number_of_bins, data['ArrivalTime'][-1], data['ArrivalTime'][0]
#         # First find the borders for the bins
# #         bins = np.linspace(0, number_of_bins*self.setting.binning_time, number_of_bins)
#         bins = np.linspace(data['ArrivalTime'][0], number_of_bins*self.setting.binning_time + data['ArrivalTime'][0], number_of_bins+1)

#         # Calculates the number of photons for every channel on a binned basis.
#         DD = np.histogram(data['ArrivalTime'][data['FinalChannel'] == 0], bins)[0].astype('float64')
#         AA = np.histogram(data['ArrivalTime'][data['FinalChannel'] == 1], bins)[0].astype('float64')
#         DA = np.histogram(data['ArrivalTime'][data['FinalChannel'] == 2], bins)[0].astype('float64')
#         AD = np.histogram(data['ArrivalTime'][data['FinalChannel'] == 3], bins)[0].astype('float64')

#         #Now we can calculate important values for every bin
#         Epr = DA / (DA + DD)
#         Len = AA + DD + DA
#         Sraw = (DD + DA) / Len
#         Tau = np.empty(Epr.shape)
#         Tau.fill(self.setting.binning_time)
#         start = bins[:-1] - data['ArrivalTime'][0]
#         end = bins[1:] - data['ArrivalTime'][0]

#         print Epr.shape, Sraw.shape, Len.shape, AA.shape, DD.shape, DA.shape,
#         print Tau.shape
#         print start.shape, end.shape

#         new_data = np.vstack((Epr, Sraw, Len, AA, DD, DA, AD, Tau, start, end)).T
#         new_data = np.array(map(tuple,new_data), self._get_dtype())
#         return new_data

#     def _get_wrapped_files(self):
#         return [self._wrapped_file_name(file_name) for file_name in self.files]

#     def _file_exists(self, wrapped_file_name, setting):
#         """ Private helper function which checks if a wrapped file by this name
#         already exists, and whether or not it's METADATA mathes 'settings'.
#         If so, the file is considered existent. It works by adding all metadata
#         to a set and comparing that to set obtained from the 'settings' obj.
#         If these sets are equal, return True.

#         WARNING:
#         There is a very small chance for false positives because this version
#         does not check on a per parameter basis if they are equal.
#         """
#         try:
#             with open(wrapped_file_name, 'rb') as file_handle:
#                 csvreader = csv.reader(file_handle, delimiter = ",")
#                 raw_data = list(csvreader)
#                 metadata = []
#                 for key, row in enumerate(raw_data):
#                     if row == ['METADATA']:
#                         continue
#                     elif row == ['DATA']: # End of metadata
#                         break
#                     else:   # we are still in the metadata section, so store the items in a set.
#                         # Cast the second column to str and add to set
#                         metadata.append(str(row[1]))
#                 else:   # This point should never be reached. If we did, throw error
#                     raise EOFError('End of file reached without finding \'DATA\'. This indicates an impropper wrapped burst file.')
#                 # Naively assumes the chance of two set's checking out the same while they are not is so slim, this assumption is save.
#                 print metadata, set(setting.str_list())
#                 return set(self._str_list_to_types(metadata)) == set(self._str_list_to_types(setting.str_list()))
#         except IOError:
#             return False

#     @staticmethod
#     def _str_list_to_types(str_list):
#         """ Converts the items in a list of strings to Floats, Ints or str and returns it. """
#         result = []
#         for item in str_list:
#             try:
#                 result.append(float(item))
#             except TypeError:
#                 try:
#                     result.append(int(item))
#                 except TypeError:
#                     result.append(str(item))
#         return result

#     def _get_header_size(self):
#         """ The header of the csv file contains metadata. The length of this
#         section will be 2 + the number of setting fed to alex_tools.
#         (2 because we want to skip the words METADATA and DATA as well.
#         """
#         return 3 + len(self.setting.str_list())

#     def _get_dtype(self):
#         return [(n, 'float64') for n in ['Epr', 'Sraw', 'Len', 'F_Aexc_Aem', 'F_Dexc_Dem', 'F_Dexc_Aem', 'F_Aexc_Dem', 'Tau', 'BurstStart', 'BurstEnd', 'bvaSigEpr', 'sigEpr', 'nWindows']]

#     def _extract_data(self, file_name):
#         """ Opens a file and turns the result into a structured array """
#         # print "going to open"
#         # with open(file_name, 'rb') as f:
#         # #     print "opened"
#         # print self._get_header_size()
#         # print self._get_dtype()
#         # print file_name
#         return np.genfromtxt(file_name, delimiter = ",", skip_header=self._get_header_size(), dtype = self._get_dtype())
#             # csvreader = csv.reader(f, delimiter = ",")
#             # raw_data = list(csvreader)
#             # # Skip METADATA section in csv file
#             # for key, row in enumerate(raw_data):
#             #     if row == ['DATA']: # Beginning of useful data
#             #         break
#             # else:   # This point should never be reached. If we did, throw error
#             #     raise EOFError('End of file reached without finding \'DATA\'. This indicates an impropper wrapped burst file.')
#             # # Create a named array so columns can be adressed by their name
#             # data = np.array(raw_data[key+2:], dtype = "float64")  # Force float so nan is handled correctly
#             # data = np.array(map(tuple,data), [(n, 'float64') for n in raw_data[key+1]])
#             # return data

#
