from traits.api import HasTraits, Array, ListInt, ListFloat, Float, Int, Bool, Instance, on_trait_change, Event, Str, Date, List, Enum
# import merge_bursts
import numpy as np
# from uncertainties import unumpy
import scipy.stats as ss
import datetime
from scipy.stats import gaussian_kde

class GUIOptions(HasTraits):
    """ To decouple our datacontainers from the GUI, users interact with the data via a subclass of this object.
    To recap, any panel in the GUI must have an associated GUIOptions object which stores the state of the options in the GUI panel.
    A DataContainer or subclass thereof will typically have a GUIOptions attribute (or subclass thereof) and will modify it's 
    contents based on changes of the GUIOptions. 
    Since every DataContainer has a View which listens for changes to the DataContainer, this will in turn cause the plot to update.
    """
    updated = Event

    def get_param_tuple(self):
        return zip(self.get_names(), self.str_list())

    def str_list(self):
        return str(self).split()

class Metadata(GUIOptions):
    """docstring for Metadata"""
    sample_name = Str
    date = Date(datetime.date.today())
    buffer_used = Str # buffer is a protected keyword
    exc_green = Str
    exc_red = Str       
    description = Str

    def get_param_tuple(self):
        return [('Sample Name', self.sample_name),
                ('Date', self.date),
                ('Buffer', self.buffer_used),
                ('Excitation Power Green', self.exc_green),
                ('Excitation Power Red', self.exc_red),
                ('Description', self.description),
                ]

class FretCorrections(GUIOptions):
    """ To go from apperent FRET to accurate fret, certain parameters are needed.
    This object is a container for such values, which can be changed from the GUI and are then used in the calculations.
    Justification for this can be found in the 2005 paper about Accurate FRET.
    """
    bkg_DA = Float(0.0) # number of counts per ms in the DA channel due to noise.
    bkg_DD = Float(0.0) # number of counts per ms in the DD channel due to noise.
    bkg_AA = Float(0.0) # number of counts per ms in the AA channel due to noise.
    E_donly = Float(0.0)   # E postion of the donor only population AFTER background correction.
    S_aonly = Float(0.0)   # S postion of the accepter only population AFTER background correction.
    gamma = Float(1.0)  # Gamma parameter. See paper for justification.
    beta = Float(1.0)   # Beta parameter. See paper for justification.

    def get_names(self):
        return ['bkg_DA', 'bkg_DD', 'bkg_AA', 'E_donly', 'S_aonly', 'gamma', 'beta',]
    
    def __str__(self):
        return "{} {} {} {} {} {} {}".format(self.bkg_DA,
                                             self.bkg_DD,
                                             self.bkg_AA,
                                             self.E_donly,
                                             self.S_aonly,
                                             self.gamma,
                                             self.beta,
                                             )

class PlotOptions(GUIOptions):
    """ Certain parameters that pertain to the image quality """
    weighted = Bool(False)
    bins = ListInt([101, 101], minlen = 2, maxlen = 2) #S, E!!!
    E_min = Float(0.0)
    E_max = Float(1.0)
    S_min = Float(0.0)
    S_max = Float(1.0)

    def get_names(self):
        return ['weighted', 'bins E', 'bins S', 'E_min', 'E_max', 'S_min', 'S_max']
    
    def __str__(self):
        return "{} {} {} {} {} {} {}".format(self.weighted,
                                             self.bins[0],
                                             self.bins[1],
                                             self.E_min,
                                             self.E_max,
                                             self.S_min,
                                             self.S_max,
                                             )

class Thresholds(GUIOptions):
    """ Thresholds to apply to the dataset. Thresholding reduces the number 
    of points in our dataset and helps remove noise. 

    S_min and S_max defines the S range to be used in the construction of the 1D E* histogram.
    """
    AexAem_min = Float(0.0)
    AexAem_max = Float(float("inf"))
    Dex_min = Float(0.0)
    Dex_max = Float(float("inf"))
    total_min = Float(0.0)
    total_max = Float(float("inf"))
    S_min = Float(0.0)
    S_max = Float(1.0)
    
    def get_names(self):
        return ['AexAem_min', 'AexAem_max', 'Dex_min', 'Dex_max', 'total_min', 'total_max', 'S_min', 'S_max']

    def get_param_tuple(self):
        return zip(self.get_names(), self.str_list())

    def str_list(self):
        return str(self).split()
    
    def __str__(self):
        return "{} {} {} {} {} {} {} {}".format(self.AexAem_min,
                                             self.AexAem_max,
                                             self.Dex_min,
                                             self.Dex_max,
                                             self.total_min,
                                             self.total_max,
                                             self.S_min,
                                             self.S_max,
                                             )

class ReducParams(HasTraits):
    """ In the future, these sm2burst parameters should be loaded from a simple 
    config file so researchers can share setting more easely. For now they are manually set here.

    reduction_mode
    0 APBS
    1 DCBS
    2 Red-only
    3 Green-only
    4 Binning
    """
    time_resolution = Float(float("12.5E-09"))
    arrival_threshold = Float(float("5E-04"))
    min_neighbors = Int(15)
    min_photons_per_burst = Int(50)
    reduction_mode = Int(0)
    burst_arriv_mat = Int(0)
    binning_time = Float(float("1E-03"))
    
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
    
    @on_trait_change('reduction_mode')
    def update_default_values(self, new_value):
        """ Every Burst Search algorithm has it's own preferred default parameters.
        Therefore, if the user changes the reduction_mode from the interface, we 
        need to change the default settings to the ones associated with the selected algorithm.

        These values need to be loaded from a config file in the future, but are for now 
        manually set based on values given by Thorben.
        """
        if new_value == 0:
            self.arrival_threshold = float("5E-04")
            self.min_neighbors = 15
            self.min_photons_per_burst = 50
        elif new_value == 1:
            self.arrival_threshold = float("5E-04")
            self.min_neighbors = 15
            self.min_photons_per_burst = 25
        elif new_value == 2:
            self.arrival_threshold = float("5E-04")
            self.min_neighbors = 7
            self.min_photons_per_burst = 12
        elif new_value == 3:
            self.arrival_threshold = float("5E-04")
            self.min_neighbors = 7
            self.min_photons_per_burst = 12

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

class AxisManager(GUIOptions):
    """ For the DatasetViewer, the user can select which two properties 
    of a dataset he or she want's to plot against each other.
    This object controls those options.
    """
    _options = ['DD', 'DA', 'AA', 'AD', 'DD + DA', 'DD + DA + AA',
                'Count(DD)','Count(DA)','Count(AA)','Count(AD)','Count(DD + DA)','Count(DD + DA + AA)',
                'Unique(DD)','Unique(DA)','Unique(AA)','Unique(AD)','Unique(DD + DA)','Unique(DD + DA + AA)',
                'Tau','Burst Start','Burst End','Epr','Sraw','Shot Noise stdev','BVA stdev','BVA Windows',
                ]
    xaxis = Enum('Unique(DD + DA)', *_options)
    yaxis = Enum('Count(DD + DA)', *_options)
    # Length of x and y vectors
    xlen = Int
    ylen = Int
    xlabel = Str
    ylabel = Str
    # plot_type = Enum('scatter',)
    #     xaxis = Enum(*(['Unique(DD + DA)'] + _options))
    # yaxis = Enum(*(['Count(DD + DA)'] + _options))

class DataContainer(HasTraits):
    """ All common basic functionality we want for our datasets. All datasets are initiated with bursts obtained after running burstsearch.
    These are then stored in data and data_orig upon initiating a DataContainer. The data is visible to the world in the plots, 
    the data_orig is only used if the user suddenly desides he want to use less thresholds than before.

    All usecasses so far also need an E histogram, so that is also included in the datacontainer. (As a vector, not a plot!)
    This can be found in the H_Epr, Eedges and Ecentres members.
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
        """ Send up a flare indicating that the data contained in this object has changed. """
        self.updated = True

    def set_histE(self): # , Smin = 0, Smax = 1
        """ Calculate E histogram for selected area.
        New since alex 0.9: only values within a certain S range are used to construct this histogram.
        This range is given by the S_min and S_max parameters in self.thresholds.
        """
        Smin = self.thresholds.S_min
        Smax = self.thresholds.S_max
        S_range_mask = (Smin <= self.data['Sraw']) & (self.data['Sraw'] <= Smax)

        weights = (self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'])[S_range_mask] if self.options.weighted else None
        self.H_Epr, self.Eedges = np.histogram(self.data['Epr'][S_range_mask], 
                                            self.options.bins[1], 
                                            # density = self.options.weighted,
                                            weights = weights,
                                            range = (0.0, 1.0)
                                            )

    # The following function can be optimised by making it three or even 5 functions
    @on_trait_change('thresholds.updated')
    def apply_thresholds(self):
        print 'thresholds changed'
        self._apply_thresholds()
        self.update()

    @on_trait_change('options.[S_min, S_max]')
    def discard_data_on_S(self):
        print 'discarting data'
        self._apply_thresholds()
        Smin = self.options.S_min
        Smax = self.options.S_max
        self.data = self.data[(Smin <= self.data['Sraw']) & (self.data['Sraw'] <= Smax)]
        self.update()

    @on_trait_change('options.[E_min, E_max]')
    def discard_data_on_E(self):
        print 'discarting data'
        self._apply_thresholds()
        Emin = self.options.E_min
        Emax = self.options.E_max
        self.data = self.data[(Emin <= self.data['Epr']) & (self.data['Epr'] <= Emax)]
        self.update()

    @on_trait_change('options.bins[]')
    def update_binning(self):
        self.update()

    @on_trait_change('options.weighted')
    def update_weighting(self):
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
    # For calculating accurate fret, we need the extra parameters carried by the following object.
    corr = Instance(FretCorrections)

    def set_hist2D(self):
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.options.weighted else None
        self.H_EvsS, self.Sedges, self.Eedges = np.histogram2d(self.Sraw, 
                                                               self.Epr, 
                                                               bins = self.options.bins, 
                                                               weights = weights, 
                                                               # normed = self.options.weighted,
                                                               range = [(0.0, 1.0), (0.0, 1.0)]
                                                               )
#         self.H_EvsS = self.H_EvsS[::-1, :]    # Because numpy is a bit of a disaster when it comes to x and y order, this makes this array more intuitive
        self.Scentres = (self.Sedges[:-1] + self.Sedges[1:])/2
        self.Ecentres = (self.Eedges[:-1] + self.Eedges[1:])/2
        
    def set_histS(self,):
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.options.weighted else None
        self.H_Sraw, Sedges = np.histogram(self.data['Sraw'], 
                                             self.options.bins[0], 
                                             # density = self.options.weighted, 
                                             weights = weights,
                                             range = (0.0, 1.0)
                                             )

    def update(self):
        """ (re)Calculate all secundairy data such as histograms """
        # Calculate accurate E before doing anything else. Otherwise applying background corrections and then changing the thresholds will undo the corrections.
        self._accurate_E()

        # Update links
        self.Epr = self.data['Epr']      # Handle for commonly used name
        self.Sraw = self.data['Sraw']    # Handle for commonly used name
        
        self.set_hist2D()
        self.set_histS()
        self.set_histE()
        
        # fire on event to demonstrate that we have just updated the object so other can catch this
        super(EvsS, self).update()
        # self.updated = True
        
    @on_trait_change('corr.updated')
    def apply_corrections(self):
        """ New values for the corrections have been submitted. A new call to update suffieces to redo the calculations,
        since _accurate_E always gets called by update() because this ensures that any changes the user makes to 
        thresholds or plotoptions will not undo the accurate fret.
        """
        self.update()

    def _accurate_E(self):
        """ If the parameters for the correction where changed by the user in the GUI, we need to recalculate the E and S values. 
        This has to be done for every burst, so for every burst we need to calculate the corrected 
        """
        print 'Calculating accurate FRET'
        # First we must correct the signal for background. The 1000 is because bkg_XX is in counts /ms
        DA_bkg_corr = self.data['F_Dexc_Aem'] - self.corr.bkg_DA*self.data['Tau']*1000
        DD_bkg_corr = self.data['F_Dexc_Dem'] - self.corr.bkg_DD*self.data['Tau']*1000
        AA_bkg_corr = self.data['F_Aexc_Aem'] - self.corr.bkg_AA*self.data['Tau']*1000
        # Negative values are mathematically possible, but physically impossible. Therefore, set all negative values to zero instead.
        DA_bkg_corr[DA_bkg_corr < 0] = 0
        DD_bkg_corr[DA_bkg_corr < 0] = 0
        DA_bkg_corr[AA_bkg_corr < 0] = 0

        # Calculate the bkg corrected E and S
        self.data['Epr'] = DA_bkg_corr/(DA_bkg_corr + DD_bkg_corr)
        self.data['Sraw'] = (DA_bkg_corr + DD_bkg_corr)/(DA_bkg_corr + DD_bkg_corr + AA_bkg_corr)

        # We can now do all other calculations. If the other parameters are unchanged, all these calculations should have no effect.
        _leakage = np.divide(1., 1 + np.divide(1,self.corr.E_donly)) # Use numpy because it uses that 1/0 = Inf, whereas python throws an error.
        _dir = np.divide(1., 1 + np.divide(1, self.corr.S_aonly))

        DA_corr = DA_bkg_corr - DD_bkg_corr*_leakage - AA_bkg_corr*_dir
        Ebkg_corr = DA_corr/(DA_corr + DD_bkg_corr)
        Sbkg_corr = (DA_corr + DD_bkg_corr)/(DA_corr + DD_bkg_corr + AA_bkg_corr)
        # We can now calculate all the corrections and change the E and S and substitute the values in self.data with this.
        self.data['Epr'] = DA_corr/(DA_corr + self.corr.gamma*DD_bkg_corr)
        self.data['Sraw'] = (DA_corr + self.corr.gamma*DD_bkg_corr)/(DA_corr + self.corr.gamma*DD_bkg_corr + AA_bkg_corr/self.corr.beta)

        # Update the plot with the updated data!
        # self.update()

class EvsSKDE(EvsS):
    """Slightly different version of the EvsS datacontainer which uses KDE rather than histograms to estimate the distribution of datapoints."""
    def set_histS(self,):
        density = gaussian_kde(self.Sraw, bw_method = 'silverman')
        self.Scentres = np.linspace(0,1,self.options.bins[1])
        self.H_Sraw = density(self.Scentres)
        # weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.options.weighted else None

    def set_histE(self,):
        Smin = self.thresholds.S_min
        Smax = self.thresholds.S_max
        S_range_mask = (Smin <= self.data['Sraw']) & (self.data['Sraw'] <= Smax)
 
        density = gaussian_kde(self.Epr[S_range_mask], bw_method = 'silverman')
        self.Ecentres = np.linspace(0,1,self.options.bins[0])
        self.H_Epr = density(self.Ecentres)

    def set_hist2D(self):
    #     # weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.options.weighted else None
        self.Eedges = np.linspace(0,1, self.options.bins[0])
        self.Sedges = np.linspace(0,1, self.options.bins[0])
        values = np.vstack([self.Epr, self.Sraw]).T
        print values.shape


        kde = gaussian_kde(values.T) 

        # Regular grid to evaluate kde upon.
        # This part comes staight from the SciPy Mailing list, I have no idea why they make 2D arrays so difficult to evaluate.
        # http://scipy-user.10969.n7.nabble.com/scipy-stats-gaussian-kde-for-2d-kernel-density-estimation-td5841.html
        x_flat = np.r_[values[:,0].min():values[:,0].max():complex(imag=self.options.bins[0])] 
        y_flat = np.r_[values[:,1].min():values[:,1].max():complex(imag=self.options.bins[1])] 
        x,y = np.meshgrid(x_flat,y_flat) 
        grid_coords = np.append(x.reshape(-1,1),y.reshape(-1,1),axis=1) 

        z = kde(grid_coords.T) 
        self.H_EvsS = z.reshape(self.options.bins[0],self.options.bins[1]) 

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

class Dataset(DataContainer):
    """ Model for the DatasetViewer. Does not add new functionality other than a 
    variable x and y axis that the DatasetViewer plot's against each other.
    """
    xaxis = Array
    yaxis = Array
    axis_manager = Instance(AxisManager)    # Object linked to GUI which indicates which axis the user want's to plot against each other.

    @on_trait_change('axis_manager.updated')
    def change_axis(self,):
        self.update()

    def update(self):
        # Using a dict might not be the most efficient way to do this, but I don't feel like typing a long elif statement.
        # It efficiency is comprimised this should be changed.  
        options = { 'DD': self.data['F_Dexc_Dem'],
                    'DA': self.data['F_Dexc_Aem'],
                    'AA': self.data['F_Aexc_Aem'],
                    'AD': self.data['F_Aexc_Dem'],
                    'DD + DA': self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'],
                    'DD + DA + AA': self.data['Len'],

                    'Count(DD)': self._count(self.data['F_Dexc_Dem']),
                    'Count(DA)': self._count(self.data['F_Dexc_Aem']),
                    'Count(AA)': self._count(self.data['F_Aexc_Aem']),
                    'Count(AD)': self._count(self.data['F_Aexc_Dem']),
                    'Count(DD + DA)': self._count(self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem']),
                    'Count(DD + DA + AA)': self._count(self.data['Len']),

                    'Unique(DD)': self._unique(self.data['F_Dexc_Dem']),
                    'Unique(DA)': self._unique(self.data['F_Dexc_Aem']),
                    'Unique(AA)': self._unique(self.data['F_Aexc_Aem']),
                    'Unique(AD)': self._unique(self.data['F_Aexc_Dem']),
                    'Unique(DD + DA)': self._unique(self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem']),
                    'Unique(DD + DA + AA)': self._unique(self.data['Len']),

                    'Tau': self.data['Tau'],
                    'Burst Start': self.data['BurstStart'],
                    'Burst End': self.data['BurstEnd'],
                    'Epr': self.data['Epr'],
                    'Sraw': self.data['Sraw'],
                    'Shot Noise stdev': self.data['sigEpr'],
                    'BVA stdev': self.data['bvaSigEpr'],
                    'BVA Windows': self.data['nWindows'],
                    }
        self.xaxis = options[self.axis_manager.xaxis]
        self.yaxis = options[self.axis_manager.yaxis]
        self.axis_manager.xlen = len(self.xaxis)
        self.axis_manager.ylen = len(self.yaxis)

        # fire on event to demonstrate that we have just updated the object so other can catch this
        super(Dataset, self).update()

    def _count(self, arr, discard_zeros = True):
        """ Converts an array into a sorted array with the number of times each number occures in the array. 
        Since bincount will also report on number it counted zero times, we choose to discard those by default.

        USE ONLY ON ARRAYS OF INT's or if you don't care about flooring, because np.bincount only works on int's.
        """
        counts = np.bincount(arr.astype('int64'))
        return counts[counts > 0] if discard_zeros else counts

    def _unique(self, arr):
        """ Turns arr into a sorted array with unique elements. """
        return np.unique(arr)

