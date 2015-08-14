from traits.api import HasTraits, Array, ListInt, ListFloat, Float, Int, Bool, Instance, on_trait_change, Event, Str, Date
# import merge_bursts
import numpy as np
# from uncertainties import unumpy
import scipy.stats as ss
import datetime

class Metadata(HasTraits):
    """docstring for Metadata"""
    sample_name = Str
    date = Date(datetime.date.today())
    buffer_used = Str # buffer is a protected keyword
    exc_green = Str
    exc_red = Str       
    description = Str

    def get_params(self):
        return [('Sample Name', self.sample_name),
                ('Date', self.date),
                ('Buffer', self.buffer_used),
                ('Excitation Power Green', self.exc_green),
                ('Excitation Power Red', self.exc_red),
                ('Description', self.description),
                ]

class PlotOptions(HasTraits):
    """ Certain parameters that pertain to the image quality """
    bins = ListInt([100, 100], minlen = 2, maxlen = 2) #S, E!!!
    E_min = Float(0.0)
    E_max = Float(1.0)
    S_min = Float(0.0)
    S_max = Float(1.0)
    update = Event

class Thresholds(HasTraits):
    """ Thresholds to apply to the dataset. Thresholding reduces the number 
    of points in our dataset and helps remove noise. 
    """
    weighted = Bool(True)
    AexAem_min = Float(0.0)
    AexAem_max = Float(float("inf"))
    Dex_min = Float(0.0)
    Dex_max = Float(float("inf"))
    total_min = Float(0.0)
    total_max = Float(float("inf"))
    update = Event
    
    def get_names(self):
        return ['weighted', 'AexAem_min', 'AexAem_max', 'Dex_min', 'Dex_max', 'total_min', 'total_max']

    def get_param_tuple(self):
        return zip(self.get_names(), self.str_list())

    def str_list(self):
        return str(self).split()
    
    def __str__(self):
        return "{} {} {} {} {} {} {}".format(self.thresholds.weighted,
                                             self.AexAem_min,
                                             self.AexAem_max,
                                             self.Dex_min,
                                             self.Dex_max,
                                             self.total_min,
                                             self.total_max,
                                             )

class ReducParams(HasTraits):
    """ In the future, these sm2burst parameters should be loaded from a simple 
    xml file so researchers can share setting more easely
    """
    time_resolution = Float(float("12.5E-09"))
    arrival_threshold = Float(0.0005)
    min_neighbors = Int(15)
    min_photons_per_burst = Int(50)
    reduction_mode = Int(0) # An arbitrary 1 is added to later to match sm2burst.cpp
    burst_arriv_mat = Int(0)
    binning_time = Float(0.001)
    
    alex_period = Float(float("1E-04"))
    first_half = Float(float("5E-05"))
    green_laser_on = Float(0.0)
    green_laser_off = Float(float("4.7E-05"))
    red_laser_on = Float(float("5.2E-05"))
    red_laser_off = Float(float("1E-04"))
    
    def str_list(self):
        return str(self).split()
    
    def get_names(self):
        if self.reduction_mode == 4:
            return ['reduction_mode', 'binning_time', 'time_resolution', 
                    'alex_period', 'first_half', 'green_laser_on',
                    'green_laser_off', 'red_laser_on', 'red_laser_off',
                    ]
        else:
            return ['reduction_mode', 'arrival_threshold', 'min_neighbors', 
                    'min_photons_per_burst', 'time_resolution', 
                    'alex_period', 'first_half', 'green_laser_on',
                    'green_laser_off', 'red_laser_on', 'red_laser_off',
                    ]
        
    def get_param_tuple(self):
        return zip(self.get_names(), self.str_list())

    def set_params(self, **kwargs):
        for name, value in kwargs.iteritems():
            setattr(self, name, self._sanitize(value))

    @staticmethod
    def _sanitize(value):
        if value.isdigit():
            return int(value)
        else:
            return float(value)
    
    def __str__(self):
        """ This object can be used to call the command-line alex_tools thanks 
        to this overloading of __str__.
        Usage of commandline utility:
        alex_tools filename reductionMode binningTime timeResolution alexPeriod firstHalf greenLaserOn greenLaserOff redLaserOn redLaserOff
        alex_tools filename reductionMode arrivalThreshold minNeighbors minPhotPerBurst timeResolution alexPeriod firstHalf greenLaserOn greenLaserOff redLaserOn redLaserOff
        """
        if self.reduction_mode == 4:
            return "{} {} {} {} {} {} {} {} {}".format(self.reduction_mode,
                                                      self.binning_time,
                                                      self.time_resolution,
                                                      self.alex_period,
                                                      self.first_half,
                                                      self.green_laser_on,
                                                      self.green_laser_off,
                                                      self.red_laser_on,
                                                      self.red_laser_off,
                                                      )
        else:
            return "{} {} {} {} {} {} {} {} {} {} {}".format(self.reduction_mode,
                                                      self.arrival_threshold,
                                                      self.min_neighbors,
                                                      self.min_photons_per_burst,
                                                      self.time_resolution,
                                                      self.alex_period,
                                                      self.first_half,
                                                      self.green_laser_on,
                                                      self.green_laser_off,
                                                      self.red_laser_on,
                                                      self.red_laser_off,
                                                      )

class DataContainer(HasTraits):
    """ All common basic functionality we want for our datasets.
    """
    data = Array        # The burstdata as obtained from the wrapped output of sm2burst
    data_orig = Array   # Store a copy of the original in case a user want to apply new/different thresholds
    H_Epr = Array
    Eedges = Array
    Ecentres = Array
    # Thresholds such as minimal number of photons.
    thresholds = Instance(Thresholds)
    # Stuff like how many bins and what E and S range to use
    options = Instance(PlotOptions)
    # Event fired to demonstrate that we have just updated the object so other can catch this
    updated = Event

    def __init__(self, data, *args, **kwargs):
        self.data = data
        self.data_orig = np.copy(data)
        super(DataContainer, self).__init__(*args, **kwargs)
        # Initiate secundairy information such as histograms
        self.update()

    def update(self):
        raise NotImplementedError('')

    def set_histE(self): # , Smin = 0, Smax = 1
        """ Calculate E histogram for selected area """
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.thresholds.weighted else None
        self.H_Epr, self.Eedges = np.histogram(self.data['Epr'], 
                                            self.options.bins[1], 
                                            # density = self.thresholds.weighted,
                                            weights = weights,
                                            range = (0.0, 1.0)
                                            )

    # The following function can be optimised by making it three or even 5 functions
    @on_trait_change('thresholds.update')
    def apply_thresholds(self):
        print 'thresholds changed'
        self._apply_thresholds()
        self.update()

    @on_trait_change('options.[S_min,S_max]')
    def discard_data_on_S(self):
        print 'discarting data'
        Smin = self.options.S_min
        Smax = self.options.S_max
        self.data = self.data[(Smin <= self.data['Sraw']) & (self.data['Sraw'] <= Smax)]
        self.update()

    @on_trait_change('options.[E_min, E_max]')
    def discard_data_on_E(self):
        print 'discarting data'
        Emin = self.options.E_min
        Emax = self.options.E_max
        self.data = self.data[(Emin <= self.data['Epr']) & (self.data['Epr'] <= Emax)]
        self.update()

    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _apply_thresholds(self):
        """ Do masking. This is done in such a way that all data that does not meet the 
        thresholds is thrown out, not just put to zero. 
        """
        print 'thresholds'
        data = np.copy(self.data_orig)
        # More than a certain amount of photons per burst is required
        self.data = self._apply_thresholds_to_data(data)
    
    def _apply_thresholds_to_data(self, data):
        # E and S values of 0 and 1 are not allowed for healthy species
        data = data[data['Epr'] > 0.0]
        data = data[data['Epr'] < 1.0]
        data = data[data['Sraw'] > 0.0]
        data = data[data['Sraw'] < 1.0]

        data = data[data['Len'] > self.thresholds.total_min]
        data = data[data['Len'] < self.thresholds.total_max]
        # Require a certain amount of red photons
        data = data[data['F_Aexc_Aem'] > self.thresholds.AexAem_min]
        data = data[data['F_Aexc_Aem'] < self.thresholds.AexAem_max]
        # Same for green photons
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] > self.thresholds.Dex_min]
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] < self.thresholds.Dex_max]
        return data


class EvsS(DataContainer):
    """ This class allows manipulation of the EvsS diagram in a lot of ways. 
    """
    # Aliases for certain columns in data for ease of use
    Epr = Array
    Sraw = Array
    # Histograms
    H_EvsS = Array
    H_Sraw = Array
    # Range's
    Sedges = Array
    Scentres = Array

    def set_hist2D(self):
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.thresholds.weighted else None
        self.H_EvsS, self.Sedges, self.Eedges = np.histogram2d(self.Sraw, 
                                                               self.Epr, 
                                                               bins = self.options.bins, 
                                                               weights = weights, 
                                                               # normed = self.thresholds.weighted,
                                                               range = [(0.0, 1.0), (0.0, 1.0)]
                                                               )
#         self.H_EvsS = self.H_EvsS[::-1, :]    # Because numpy is a bit of a disaster when it comes to x and y order, this makes this array more intuitive
        self.Scentres = (self.Sedges[:-1] + self.Sedges[1:])/2
        self.Ecentres = (self.Eedges[:-1] + self.Eedges[1:])/2
        
    def set_histS(self,):
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.thresholds.weighted else None
        self.H_Sraw, Sedges = np.histogram(self.data['Sraw'], 
                                             self.options.bins[0], 
                                             # density = self.thresholds.weighted, 
                                             weights = weights,
                                             range = (0.0, 1.0)
                                             )

    def update(self):
        """ (re)Calculate all secundairy data such as histograms """
        # Update links
        self.Epr = self.data['Epr']      # Handle for commonly used name
        self.Sraw = self.data['Sraw']    # Handle for commonly used name
        
        self.set_hist2D()
        self.set_histS()
        self.set_histE()
        # self.correct_E() Not implemented

        # fire on event to demonstrate that we have just updated the object so other can catch this
        self.updated = True
        
    def correct_E(self):
        """ Do gamma correction etc. """
        raise NotImplementedError()
    


class BVA(DataContainer):
    """ All BVA functionality is here, e.g. generating the E histogram and the standarddeviation info.
    Plotting this info is done by the view BVAView
    """
    bins = Int(20)
    bin_edges = Array
    bin_centres = Array
    bin_avg = Array
    window_size = Int(5)    # Window size used in BVA analysis

    def update(self):
        """ (re)Calculate all secundairy data such as histograms """
        self._set_bin_avg()
        self.set_histE()
        # fire on event to demonstrate that we have just updated the object so other can catch this
        self.updated = True

    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _set_bin_avg(self,):
        """ To make the BVA significant, we average the stdev for all the bursts within a bin. 
        This is stored in self.bin_avg, and the bin centres in self.bin_centres. 
        self.bins is used to determine the number of bins to use.
        """
        # self.bin_avg, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'], self.data['bvaSigEpr'], statistic='mean', bins = self.bins, range = [(0.0, 1.0)])
        weighted_sig, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'], self.data['bvaSigEpr']*self.data['nWindows'], statistic='sum', bins = self.bins, range = [(0.0, 1.0)])
        total_windows, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'], self.data['nWindows'], statistic='sum', bins = self.bins, range = [(0.0, 1.0)])
        self.bin_avg = weighted_sig/total_windows
        # print self.bin_centres, self.bin_centres.shape
        print self.bin_avg, self.bin_avg.shape

    @on_trait_change('bin_edges')
    def update_bin_centres(self):
        self.bin_centres = (self.bin_edges[1:] + self.bin_edges[:-1])/2