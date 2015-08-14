from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event, Bool, List, Unicode
import subprocess
import csv
import os
import numpy as np
import struct
from .models import ReducParams, Thresholds, EvsS, FretCorrections, Metadata, PlotOptions
from .functions import GuessesToFit
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

    def _read_to_np(self):
        raise NotImplementedError('')

class CacheFile(HasTraits):
    """ Any file that shares resamblence to a burstsearch cache needs to know what columns are present in such a cache.
    Thats where this Object comes in.
    """
    def _get_dtype(self):
        # return [(n, 'float64') for n in ['Epr', 'Sraw', 'Len', 'F_Aexc_Aem', 'F_Dexc_Dem', 'F_Dexc_Aem', 'F_Aexc_Dem', 'Tau', 'BurstStart', 'BurstEnd', 'bvaSigEpr', 'sigEpr', 'nWindows']]
        return [('Epr', 'float64'), 
                ('Sraw', 'float64'), 
                ('Len', 'int64'), 
                ('F_Aexc_Aem', 'int64'), 
                ('F_Dexc_Dem', 'int64'), 
                ('F_Dexc_Aem', 'int64'), 
                ('F_Aexc_Dem', 'int64'), 
                ('Tau', 'float64'), 
                ('BurstStart', 'float64'), 
                ('BurstEnd', 'float64'), 
                ('bvaSigEpr', 'float64'), 
                ('sigEpr', 'float64'), 
                ('nWindows', 'int64'),
                ]

class SessionFile(CacheFile):
    """ The active session can be exported by the user to a csv file. This same type of file is also used by the 
    Titration view to import procesed data from several runs and turn it all into a stack plot.

    The data that is reconstructed from the csv file is:
    ReducParams -> Burst Search Parameters.
    Thresholds -> Thresholds used at the time.
    EvsS -> Contains the info about the histograms that is also placed in the csv file.
    """
    params = Instance(ReducParams)
    thresholds = Instance(Thresholds)
    evss = Instance(EvsS)
    fitting = Instance(GuessesToFit)
    metadata = Instance(Metadata)
    corr = Instance(FretCorrections)
    plot_options = Instance(PlotOptions)
    file_path = Unicode

    def read(self):
        with open('{}'.format(self.file_path), 'rU') as f:
            csv_reader = csv.reader(f)
            # Do not change this order!
            titles = ['BURST SEARCH PARAMETERS', 'THRESHOLDS', 'EXPERIMENTAL', 'ACCURATE FRET', 'PLOT OPTIONS', 'BIN CENTERS', '2D HISTOGRAM', 'DATA']
            sections = [[] for i in range(len(titles))] # Create an empty list of lists to hold all data after extraction from file. +1 because there is some bull before the valuable info starts.
            key = 0
            for row in csv_reader:
                if row[0] in titles:
                    # Change the key to start appending to the next part of the list.
                    key = titles.index(row[0])
                else:
                    sections[key].append(row)

            # Next phase is to initiate the different GUIOptions from this data.
            self.params = ReducParams()
            self.params.set_param_tuples(sections[0])
            self.thresholds = Thresholds()
            self.thresholds.set_param_tuples(sections[1])
            self.metadata = Metadata()
            self.metadata.set_param_tuples(sections[2])
            self.corr = FretCorrections()
            self.corr.set_param_tuples(sections[3])
            self.plot_options = PlotOptions()
            self.plot_options.set_param_tuples(sections[4])
            # Initiate the EvsS object.
            data = np.array(sections[7][1:], dtype = self._get_dtype())
            self.evss = EvsS(data = data, options = self.plot_options, thresholds = self.thresholds, corr = self.corr)

    def write(self):
        root, ext = os.path.splitext(self.file_path)
        if not ext:
            ext = '.csv'

        with open('{}{}'.format(root, ext), 'w') as f:
            csv_writer = csv.writer(f)
            # Write the metadata about the used burstsearch first
            # csv_writer.writerow(['METADATA'])
            # csv_writer.writerow(['BURST SEARCH PARAMETERS'])
            # for param_tuple in self.params.get_param_tuple():
            #     csv_writer.writerow(param_tuple)
            # # Applied Thresholds
            # csv_writer.writerow(['THRESHOLDS'])
            # for param_tuple in self.thresholds.get_param_tuple():
            #     csv_writer.writerow(param_tuple)

            items = [('BURST SEARCH PARAMETERS', self.params),
                     ('THRESHOLDS', self.thresholds),
                     ('EXPERIMENTAL', self.metadata),
                     ('ACCURATE FRET', self.corr),
                     ('PLOT OPTIONS', self.plot_options),
                     ]

            for title, obj in items:
                csv_writer.writerow([title])
                for param_tuple in obj.get_param_tuple():
                    csv_writer.writerow(param_tuple)

            # csv_writer.writerow(['DATA'])
            csv_writer.writerow(['BIN CENTERS', 'E HISTOGRAM', 'S HISTOGRAM'])
            for x, E, S in zip(self.evss.Ecentres, self.evss.H_Epr, self.evss.H_Sraw):
                csv_writer.writerow([x, E, S])

            # csv_writer.writerow([''])
            csv_writer.writerow(['2D HISTOGRAM'])
            for row in self.evss.H_EvsS:
                csv_writer.writerow(row)

            csv_writer.writerow(['DATA'])
            for row in self.evss.data_orig:
                csv_writer.writerow(row)

class BurstFile(LazyFile, CacheFile):
    """ Manager for a .csv file containing Bursts.
    """
    def _get_header_size(self):
        """ The header of the csv file contains metadata. The length of this
        section will be 3 + the number of setting fed to alex_tools.
        (3 because we want to skip the words METADATA, DATA and the header as well.)
        """
        return 3 + len(self.setting.str_list())

    def _read_to_np(self):
        """ Opens a file and turns the result into a structured array """
        return np.genfromtxt(self.file_name, delimiter = ",", skip_header=self._get_header_size(), dtype = self._get_dtype(), invalid_raise = False, comments = None)

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
            file_name = string.join(file_name.split('\\'), '\\\\')
        subprocess_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "alex_tools.exe" if _platform == 'win32' else "alex_tools")

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
            with open(wrapped_file_name, 'rU') as file_handle:
                # The csv sniffer caused a lot of problems. So this hack works quite nice to determine the seperator to use.
                header = file_handle.readline()
                try:
                    delimiter = header[8]   # This is ; or whatever excel makes of it, or nothing.
                except IndexError:
                    delimiter = ','
                else:
                    if not delimiter.strip():
                        delimiter = ','

                file_handle.seek(0)
                csvreader = csv.reader(file_handle, delimiter = delimiter)
                raw_data = list(csvreader)
                metadata = []
                for key, row in enumerate(raw_data):
                    if 'METADATA' in row:
                        continue
                    elif 'DATA' in row: # End of metadata
                        break
                    else:   # we are still in the metadata section, so store the items in a set.
                        # Cast the second column to str and add to set
                        metadata.append(str(row[1]))
                else:   # This point should never be reached. If we did, throw error
                    raise EOFError('End of file reached without finding \'DATA\'. This indicates an impropper wrapped burst file.')
                # Naively assumes the chance of two set's checking out the same while they are not is so slim, this assumption is save.
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
    Then, if the user decides on a Burst Search option, this object will run the desired burstsearch on the .sm files.
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
        if len(self._file_types()) >= 2: # Mixed files are not allowed!
            self.load = 'STOP'
        # Nothing special, most of the times users will be opening .sm files.
        elif u'sm' in self._file_types():
            self.datasets = []
            self.number_of_dataset = len(self.files)
            data_to_merge = []
            for key, file_name in enumerate(self.files):
                self.datasets.append(Dataset(file_name, self.setting))
                data_to_merge.append(self.datasets[-1].get_bursts())
                self.number_loaded = key
            self.merged_data = np.concatenate(data_to_merge)
        # CSV files are exports from a previous experiment.
        elif u'csv' in self._file_types():
            pass


    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    @on_trait_change('sm_path')
    def set_files(self):
        """ Responsible for finding .sm files in the given dir """
        # Make sm_path correct for this os, just in case. Dirty, I know. Let's hope enaml fixes this in the file dlg
        self.sm_path = string.join(self.sm_path.split('/'), os.sep)
        # self.files = [os.path.join(self.sm_path, item) for item in os.listdir(self.sm_path) if item.split('.')[-1] == 'sm'] if self.sm_path else []
        self.files = [os.path.join(self.sm_path, item) for item in os.listdir(self.sm_path) if self.valid_file(item)] if self.sm_path else []

    def valid_file(self, file_name):
        """ Upon selection of a folder, we need to index all the valid files in this directory.
        Currently, this means two types: .sm files and .csv files created by the export option in the EvsSView.
        Unfortinately, there is no way to distinguish these from other csv's until they are opened so 
        users must make sure to only point to proper files.
        """
        parts = file_name.split('.')
        if parts[-1] != 'sm' and parts[-1] != 'csv': # Not a csv or sm, so skip
            return False
        elif parts.count('sm') >= 2:                # Corrupt sm files from the Oxford software have .sm occuring twice in their name.
            return False
        elif '.sm.wrapped_new.csv' in file_name:    # This is a cache csv from Burst Searching. These are not of interest either.
            return False
        else:                                       # What we are left with are proper .sm files and any other .csv file 
            return True

    def _file_types(self):
        """ Before loading the files, we must make sure we are only dealing with a single file type. 
        For example, if a folder contains both proper csv files and sm files, we cant just merge 
        those because they represent different information. (Photon arrival times and procesed histograms.)
        """
        parts = set([item for file_name in self.files for item in file_name.split('.')])
        if u'sm' in parts and u'csv' in parts:
            return [u'sm', u'csv']
        elif u'sm' in parts:
            return [u'sm']
        elif u'csv' in parts:
            return [u'csv']
        else:
            return []
        # return not(u'sm' in parts and u'csv' in parts)
