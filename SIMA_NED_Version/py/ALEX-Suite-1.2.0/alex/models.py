""" I'm a docstring

.. module:: alex.models

"""

from traits.api import HasTraits, Array, ListInt, Float, Int, Bool, Instance, on_trait_change, Event, Str, Date, Enum, \
    Dict, List, HasStrictTraits, Property, ListUnicode, Range
# import merge_bursts
import numpy as np
# from uncertainties import unumpy
import scipy.stats as ss
import datetime
from scipy.stats import gaussian_kde
# from matplotlib.font_manager import FontProperties
import matplotlib.font_manager


class Options(HasTraits):
    """ To decouple our datacontainers from the GUI, users interact with the data via a subclass of this object.
    To recap, any panel in the GUI must have an associated GUIOptions object which stores the state of the options in
    the GUI panel. A DataContainer or subclass thereof will typically have a GUIOptions attribute (or subclass thereof)
    and will modify it's contents based on changes of the GUIOptions. Since every DataContainer has a View which listens
    for changes to the DataContainer, this will in turn cause the plot to update.
    """
    updated = Event

    def get_param_tuple(self):
        return [(name, getattr(self, name)) for name in self.get_names()]

    def set_param_tuples(self, items):
        """ Items is a list of iterables of len 2. These items in each of these rows is used to initiate the object. """
        for name, value in items:
            setattr(self, name, self._sanitize(value))

    def get_names(self):
        return self.class_editable_traits()

    def get_values(self):
        return [getattr(self, name) for name in self.get_names()]

    def __str__(self):
        str_rep = ''
        for name in self.get_names():
            str_rep = '{}{} '.format(str_rep, getattr(self, name))
        return str_rep.strip()

    @staticmethod
    def _sanitize(value):
        if type(value) is str:
            try:
                return int(value)
            except ValueError:
                try:
                    return float(value)
                except ValueError:
                    if value.lower() == 'false':
                        return False
                    elif value.lower == 'true':
                        return True
                    else:
                        try:
                            return datetime.datetime.strptime(value, "%Y-%m-%d").date()
                        except ValueError:
                            return value
        else:
            return value

class GUIOptions(Options):
    pass

class FittingOptions(Options):
    """ Base classes for fitting panel options.
    """
    pass

class TitrationPresets(FittingOptions):
    """ Titration experiments are usualy calculated using typical settings.
    These presets are provided for convinience.
    """
    presets = Event
    # Human readable text for the gui.
    gui_text = ListUnicode([u'Two populations, fixed x0 and sigma.'])
    mode = Int(0)
    mode_text = List(['Stack Plot', 'Titration Curve'])

class PlotFormatOptions(GUIOptions):
    """Formatting of the plot such as fontsize, figure size etc. If the images
    generated by this project are to be used for publication we need these 
    options.
    """
    # family = Enum('serif', 'sans-serif', 'cursive', 'fantasy', 'monospace')
    family = Enum(['serif', 'sans-serif', 'cursive', 'fantasy', 'monospace'] + sorted(
        set([f.name for f in matplotlib.font_manager.fontManager.ttflist])))
    style = Enum('normal', 'italic', 'oblique')
    variant = Enum('normal', 'small-caps')
    weight = Enum('light', 'normal', 'medium', 'semibold', 'bold', 'heavy', 'black')
    # size  = ['xx-small', 'x-small', 'small', 'medium', 'large', 'x-large', 'xx-large']
    size = Int(13)
    # Label postion offset. In case of a bigger font-size, users must tweak this to prevent overlap.
    x_label_offset = Float(-0.1)
    y_label_offset = Float(-0.1)
    # Figure size. Can be used to get an idea off what the image will look like in print.
    width = Float(8)
    height = Float(8)
    dpi = Int(80)

    @staticmethod
    def get_fonts():
        return ['serif', 'sans-serif', 'cursive', 'fantasy', 'monospace'] + sorted(
            set([f.name for f in matplotlib.font_manager.fontManager.ttflist]))


class Metadata(GUIOptions):
    """docstring for Metadata"""
    sample_name = Str
    date = Date(datetime.date.today())
    buffer_used = Str  # buffer is a protected keyword
    exc_green = Str
    exc_red = Str
    description = Str

    # def get_param_tuple(self):
    # return [('Sample Name', self.sample_name),
    #             ('Date', self.date),
    #             ('Buffer', self.buffer_used),
    #             ('Excitation Power Green', self.exc_green),
    #             ('Excitation Power Red', self.exc_red),
    #             ('Description', self.description),
    #             ]

    def get_names(self):
        """ These must be the names of the attributes of htis object.
        To my knowledge, Traits does not have a reliable shorthand for this so we need to do it ourselfs.
        """
        return ['sample_name', 'date', 'buffer_used', 'exc_green', 'exc_red', 'description']


class FretCorrections(GUIOptions):
    """ To go from apperent FRET to accurate fret, certain parameters are needed.
    This object is a container for such values, which can be changed from the GUI and are then used in the calculations.
    Justification for this can be found in the 2005 paper about Accurate FRET.
    """
    bkg_DA = Float(0.0)  # number of counts per ms in the DA channel due to noise.
    bkg_DD = Float(0.0)  # number of counts per ms in the DD channel due to noise.
    bkg_AA = Float(0.0)  # number of counts per ms in the AA channel due to noise.
    E_donly = Float(0.0)  # E postion of the donor only population AFTER background correction.
    S_aonly = Float(0.0)  # S postion of the accepter only population AFTER background correction.
    gamma = Float(1.0)  # Gamma parameter. See paper for justification.
    beta = Float(1.0)  # Beta parameter. See paper for justification.
    PIFE_correct = Bool(False)
    S_ref = Float  # Used for PIFE corrected S. None by default.

    # Temporary (?) variables for the Thorben Hack.
    thorben_hack = Event  # upon clicking the button to calculate the background, this event should be fired.
    # It's payload is the cutoff supplied by the user.


class PlotOptions(GUIOptions):
    """ Certain parameters that pertain to the image quality """
    weighted = Bool(False)
    bins = ListInt([101, 101], minlen=2, maxlen=2)  # S, E!!!
    E_min = Float(0.0)
    E_max = Float(1.0)
    S_min = Float(0.0)
    S_max = Float(1.0)
    log_scale = Bool(False)

    # def get_names(self):
    #     return ['weighted', 'bins E', 'bins S', 'E_min', 'E_max', 'S_min', 'S_max']

class BurstThresholds(GUIOptions):
    """
    Thresholds that have only to do with bursts, not derivative properties.
    """
    # Photon count thresholds
    AexAem_min = Float(0.0)
    AexAem_max = Float(float("inf"))
    Dex_min = Float(0.0)
    Dex_max = Float(float("inf"))
    total_min = Float(0.0)
    total_max = Float(float("inf"))
    # Timing thresholds
    Tau_min = Float(0.0)
    Tau_max = Float(float('inf'))
    # E and S ranges
    S_min = Float(0.0)
    S_max = Float(1.0)
    E_min = Float(0.0)
    E_max = Float(1.0)

class Thresholds(BurstThresholds):
    """ Thresholds to apply to the dataset. Thresholding reduces the number 
    of points in our dataset and helps remove noise. 

    S_min and S_max defines the S range to be used in the construction of the 1D E* histogram.
    """
    total_min = Float(250.0)
    # Only for the 1D histogram
    S_min_1D = Float(0.3)
    S_max_1D = Float(0.8)
    E_min_1D = Float(0.0)
    E_max_1D = Float(1.0)
    


class EvsSLayers(GUIOptions):
    """ Which layers are present in the plot. """
    show_E = Bool(True)
    show_S = Bool(True)
    show_guesses = Bool(True)
    show_S_range = Bool(True)
    show_E_range = Bool(False)


class TitrationDirectories(GUIOptions):
    """ For a titration experiment, we wan't to have multiple directories each
    corresponding to a certain concentration and then combine all of them to a
    stackplot.
    This object holds the concentrations and the paths to use.
    """
    # conditions = List # List of [concentration, path] pairs.
    concentrations = List
    paths = List


class ReducParams(GUIOptions):
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
    green_laser_on = Float(float("0.3E-05"))
    green_laser_off = Float(float("4.7E-05"))
    red_laser_on = Float(float("5.2E-05"))
    red_laser_off = Float(float("9.6E-05"))

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
                'Count(DD)', 'Count(DA)', 'Count(AA)', 'Count(AD)', 'Count(DD + DA)', 'Count(DD + DA + AA)',
                'Unique(DD)', 'Unique(DA)', 'Unique(AA)', 'Unique(AD)', 'Unique(DD + DA)', 'Unique(DD + DA + AA)',
                'Tau', 'Burst Start', 'Burst End', 'Epr', 'Sraw', 'Shot Noise stdev', 'BVA stdev', 'BVA Windows',
                ]
    xaxis = Enum('Unique(DD + DA)', *_options)
    yaxis = Enum('Count(DD + DA)', *_options)
    # Length of x and y vectors
    xlen = Int
    ylen = Int
    xlabel = Str
    ylabel = Str
    # plot_type = Enum('scatter',)
    # xaxis = Enum(*(['Unique(DD + DA)'] + _options))
    # yaxis = Enum(*(['Count(DD + DA)'] + _options))


class BurstPropertiesOptions(GUIOptions):
    bin_size_time = Float(0.001)
    bin_size_counts = Int(10)
    bin_size_freq = Float(10000.0)
    normalised = Bool(False)
    no_bins = Int   # Shows the user how many bins to expect.
    mode = Int(0)   # The type of binning used. 0 -> tau, 1-> events, 2 -> freq
    index = Int(0)  # Index of the column
    values = List([
        'Tau', 'Len', 'F_Aexc_Aem', 'F_Dexc_Dem',
        'F_Dexc_Aem', 'F_Aexc_Dem',
    ])
    column = Enum(
        'Tau', 'Epr', 'Sraw', 'Len', 'F_Aexc_Aem', 'F_Dexc_Dem',
        'F_Dexc_Aem', 'F_Aexc_Dem',
        'bvaSigEpr', 'sigEpr',
    )




class BGData(HasTraits):
    """

    Photon data format, see also merge_bursts PhotonArrivalTime._read_to_np:
    dtype: [('ArrivalTime', '<f8'), ('FinalChannel', '<i2'), ('Channel', '?')] where "?" = bool
    FinalChannel:
    0   Laser green, detection green    DD
    1   Laser red, detection red        AA
    2   Laser green, detection red      DA
    3   Laser red, detection green      AD

    """
    # todo: Make base datacontainter parent for both BGData and DataContainer?
    photon_data = Array
    options = Instance(PlotOptions)
    params = Instance(ReducParams)
    updated = Event
    _channel = {0: 'DD', 1: 'AA', 2: 'DA', 3: 'AD'}
    #holds binned data and corresponding time for all channels
    time_data = Dict
    time = Dict

    def __init__(self, photon_data, params, *args, **kwargs):
        super(BGData, self).__init__(*args, **kwargs)

        self.params = params
        self.photon_data = photon_data

        #Deze regel is als het goed is nu overbodig omdat door de super direct na de init staat maar
        self.get_binned_data()

        self.update()

    def update(self):
        #todo naar parent
        """ Send up a flare indicating that the data contained in this object has changed. """
        self.updated = True

    @on_trait_change('photon_data')
    def get_binned_data(self):
        channel_photons = self._channels_separate()
        binsize = self.params.binning_time

        for key in channel_photons:
            max = np.ceil(channel_photons[key].max() / binsize) * binsize
            time = np.arange(0, max, binsize)
            #NOTE: np.digitize is MUCH slower then np.searchsorted!!
            #todo seems to work this binning but needs checking / confirmation
            inds = np.searchsorted(time, channel_photons[key]) - 1

            #todo fix dis hack where photon arrival time 0.0 ends up in bin -1
            self.time_data[key] = np.bincount(inds[inds >= 0])

            #combine time array to one?
            self.time[key] = time

    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _channels_separate(self):
        channel_photons = {}

        for ch in np.unique(self.photon_data['FinalChannel']):
            val = self.photon_data['ArrivalTime'][self.photon_data['FinalChannel'] == ch]
            key = self._channel[ch]
            channel_photons[key] = val

        return channel_photons


class DataContainer(HasTraits):
    """ All common basic functionality we want for our datasets. All datasets are initiated with bursts obtained after running burstsearch.
    These are then stored in data and data_orig upon initiating a DataContainer. The data is visible to the world in the plots,
    the data_orig is only used if the user suddenly desides he want to use less thresholds than before.

    All usecasses so far also need an E histogram, so that is also included in the datacontainer. (As a vector, not a plot!)
    This can be found in the H_Epr, Eedges and Ecentres members.
    """
    data = Array  # The burstdata as obtained from the wrapped output of sm2burst
    data_orig = Array  # Store a copy of the original in case a user want to apply new/different thresholds

    # Event fired to demonstrate that we have just updated the object so other can catch this
    updated = Event

    def __init__(self, data, *args, **kwargs):
        super(DataContainer, self).__init__(*args, **kwargs)
        self.data = np.copy(data)
        self.data_orig = np.copy(data)
        self.update()

    def update(self):
        """ Send up a flare indicating that the data contained in this object has changed. """
        self.updated = True


class BurstContainer(DataContainer):
    thresholds = Instance(Thresholds)

    def update(self):
        self._apply_thresholds()
        self._apply_range()
        super(BurstContainer, self).update()

    # --------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _apply_thresholds(self):
        """ Do masking. This is done in such a way that all data that does not meet the
        thresholds is thrown out, not just put to zero.
        """
        data = np.copy(self.data_orig)
        # More than a certain amount of photons per burst is required
        self.data = self._apply_thresholds_to_data(data)

    def _apply_thresholds_to_data(self, data):
        # E and S values of 0 and 1 are not allowed for healthy species
        # So these are the only ones that should exclude the borders.
        data = data[data['Len'] >= self.thresholds.total_min]
        data = data[data['Len'] <= self.thresholds.total_max]

        data = data[data['Tau'] >= self.thresholds.Tau_min]
        data = data[data['Tau'] <= self.thresholds.Tau_max]

        # Require a certain amount of red photons
        data = data[data['F_Aexc_Aem'] >= self.thresholds.AexAem_min]
        data = data[data['F_Aexc_Aem'] <= self.thresholds.AexAem_max]
        # Same for green photons
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] >= self.thresholds.Dex_min]
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] <= self.thresholds.Dex_max]
        return data

    def _apply_range(self):
        # More than a certain amount of photons per burst is required
        self.data = self._apply_range_to_data(self.data)

    def _apply_range_to_data(self, data):
        """
        Through out all points outside a certain E/S range.
        """
        data = data[data['Epr'] > self.thresholds.E_min]
        data = data[data['Epr'] < self.thresholds.E_max]
        data = data[data['Sraw'] > self.thresholds.S_min]
        data = data[data['Sraw'] < self.thresholds.S_max]
        return data


class FRETContainer(BurstContainer):
    """ Adds all FRET functionality.
    """
    H_Epr = Array
    Eedges = Array
    Ecentres = Array

    corr = Instance(FretCorrections)
    plot_options = Instance(PlotOptions)

    def update(self):
        self._apply_thresholds()
        self._accurate_E()
        self._apply_range()
        self.set_histE()
        super(FRETContainer, self).update()

    def set_histE(self):  # , Smin = 0, Smax = 1
        """ Calculate E histogram for selected area.
        New since alex 0.9: only values within a certain S range are used to construct this histogram.
        This range is given by the S_min and S_max parameters in self.thresholds.
        """
        Smin = self.thresholds.S_min_1D
        Smax = self.thresholds.S_max_1D
        S_range_mask = (Smin <= self.data['Sraw']) & (self.data['Sraw'] <= Smax)

        weights = (self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'])[S_range_mask] if self.plot_options.weighted else None
        self.H_Epr, self.Eedges = np.histogram(self.data['Epr'][S_range_mask],
                                               self.plot_options.bins[1],
                                               # density = self.plot_options.weighted,
                                               weights=weights,
                                               range=(self.thresholds.E_min, self.thresholds.E_max),
                                               )
        self.Ecentres = (self.Eedges[:-1] + self.Eedges[1:]) / 2

    # --------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _apply_thresholds(self):
        """ Do masking. This is done in such a way that all data that does not meet the
        thresholds is thrown out, not just put to zero.
        """
        data = np.copy(self.data_orig)
        # More than a certain amount of photons per burst is required
        self.data = self._apply_thresholds_to_data(data)

    def _apply_thresholds_to_data(self, data):
        # E and S values of 0 and 1 are not allowed for healthy species
        # So these are the only ones that should exclude the borders.
        data = data[data['Len'] >= self.thresholds.total_min]
        data = data[data['Len'] <= self.thresholds.total_max]

        data = data[data['Tau'] >= self.thresholds.Tau_min]
        data = data[data['Tau'] <= self.thresholds.Tau_max]

        # Require a certain amount of red photons
        data = data[data['F_Aexc_Aem'] >= self.thresholds.AexAem_min]
        data = data[data['F_Aexc_Aem'] <= self.thresholds.AexAem_max]
        # Same for green photons
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] >= self.thresholds.Dex_min]
        data = data[data['F_Dexc_Dem'] + data['F_Dexc_Aem'] <= self.thresholds.Dex_max]
        return data

    def _apply_range(self):
        # More than a certain amount of photons per burst is required
        self.data = self._apply_range_to_data(self.data)

    def _apply_range_to_data(self, data):
        """
        Through out all points outside a certain E/S range.
        """
        data = data[data['Epr'] > self.thresholds.E_min]
        data = data[data['Epr'] < self.thresholds.E_max]
        data = data[data['Sraw'] > self.thresholds.S_min]
        data = data[data['Sraw'] < self.thresholds.S_max]
        return data

    def _accurate_E(self):
        """ If the parameters for the correction where changed by the user in the GUI, we need to recalculate the E and S values.
        This has to be done for every burst, so for every burst we need to calculate the corrected

        """
        if not self.corr:
            return
        print 'Calculating accurate FRET'
        # First we must correct the signal for background. The 1000 is because bkg_XX is in counts /ms
        DA_bkg_corr = self.data['F_Dexc_Aem'] - self.corr.bkg_DA * self.data['Tau'] * 1000
        DD_bkg_corr = self.data['F_Dexc_Dem'] - self.corr.bkg_DD * self.data['Tau'] * 1000
        AA_bkg_corr = self.data['F_Aexc_Aem'] - self.corr.bkg_AA * self.data['Tau'] * 1000
        # Negative values are mathematically possible, but physically impossible. Therefore, set all negative values to zero instead.
        DA_bkg_corr[DA_bkg_corr < 0] = 0
        DD_bkg_corr[DA_bkg_corr < 0] = 0
        DA_bkg_corr[AA_bkg_corr < 0] = 0

        # Calculate the bkg corrected E and S
        self.data['Epr'] = DA_bkg_corr / (DA_bkg_corr + DD_bkg_corr)
        self.data['Sraw'] = (DA_bkg_corr + DD_bkg_corr) / (DA_bkg_corr + DD_bkg_corr + AA_bkg_corr)

        # We can now do all other calculations. If the other parameters are unchanged, all these calculations should have no effect.
        _leakage = np.divide(1., -1 + np.divide(1,
                                                self.corr.E_donly))  # Use numpy because it uses that 1/0 = Inf, whereas python throws an error.
        _dir = np.divide(1., -1 + np.divide(1, self.corr.S_aonly))

        DA_corr = DA_bkg_corr - DD_bkg_corr * _leakage - AA_bkg_corr * _dir
        Ebkg_corr = DA_corr / (DA_corr + DD_bkg_corr)
        Sbkg_corr = (DA_corr + DD_bkg_corr) / (DA_corr + DD_bkg_corr + AA_bkg_corr)
        # We can now calculate all the corrections and change the E and S and substitute the values in self.data with this.
        self.data['Epr'] = DA_corr / (DA_corr + self.corr.gamma * DD_bkg_corr)
        self.data['Sraw'] = (DA_corr + self.corr.gamma * DD_bkg_corr) / (
            DA_corr + self.corr.gamma * DD_bkg_corr + AA_bkg_corr / self.corr.beta)

        # PIFE corrected data
        if self.corr.PIFE_correct:
            # l_vec = (self.corr.S_ref * (1 - self.data['Sraw'])) / (self.data['Sraw'] * (1 - self.corr.S_ref))
            # self.data['Epr'] /= (l_vec * self.data['Epr']) + self.data['Epr']
            l_vec = (self.data['Sraw'] * (1 - self.corr.S_ref)) / (self.corr.S_ref * (1 - self.data['Sraw']))
            self.data['Epr'] /= ((1 / l_vec) * (1 - self.data['Epr'])) + self.data['Epr']


class EvsS(FRETContainer):
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
        weights = self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'] if self.plot_options.weighted else None
        self.H_EvsS, self.Sedges, self.Eedges = np.histogram2d(self.data['Sraw'],
                                                               self.data['Epr'],
                                                               bins=self.plot_options.bins,
                                                               weights=weights,
                                                               # normed = self.plot_options.weighted,
                                                               range=[(self.thresholds.S_min, self.thresholds.S_max), (self.thresholds.E_min, self.thresholds.E_max)],)
                                                               # range=[(0.0, 1.0), (0.0, 1.0)],)
        # self.H_EvsS = self.H_EvsS[::-1, :]    # Because numpy is a bit of a disaster when it comes to x and y order, this makes this array more intuitive
        self.Scentres = (self.Sedges[:-1] + self.Sedges[1:]) / 2
        self.Ecentres = (self.Eedges[:-1] + self.Eedges[1:]) / 2

    def set_histS(self, ):
        Emin = self.thresholds.E_min_1D
        Emax = self.thresholds.E_max_1D
        E_range_mask = (Emin <= self.data['Epr']) & (self.data['Epr'] <= Emax)

        weights = (self.data['F_Dexc_Dem'] + self.data['F_Dexc_Aem'])[E_range_mask] if self.plot_options.weighted else None
        self.H_Sraw, Sedges = np.histogram(self.data['Sraw'][E_range_mask],
                                           self.plot_options.bins[0],
                                           # density = self.plot_options.weighted,
                                           weights=weights,
                                           range=(self.thresholds.S_min, self.thresholds.S_max),
                                           )

    def update(self):
        """ (re)Calculate all secundairy data such as histograms.
        This function is bad in terms of API changes, cause it's almost a copy
        paste of FRETContainer.update. However, this is the only thing I can
        think of that doesn't introduce numerical errors. Any other solution I
        could think of only displayed the correct result after clicking apply
        twice. We are not building LaTeX here.
        """
        self._apply_thresholds()
        self._accurate_E()
        self._apply_range()
        self.set_histE()
        self.set_hist2D()
        self.set_histS()
        DataContainer.update(self)# This is not the nicest way API wise


class BVA(FRETContainer):
    """ All BVA functionality is here, e.g. generating the E histogram and the standarddeviation info.
    Plotting this info is done by the view BVAView
    """
    bins = Int(20)
    bin_edges = Array
    bin_centres = Array
    bin_avg = Array
    window_size = Int(5)  # Window size used in BVA analysis

    def update(self):
        """ (re)Calculate all secundairy data such as histograms """
        super(BVA, self).update()
        self._set_bin_avg()
        self.set_histE()
        # # fire on event to demonstrate that we have just updated the object so other can catch this
        # self.updated = True

    # --------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------

    def _set_bin_avg(self, ):
        """ To make the BVA significant, we average the stdev for all the bursts within a bin. 
        This is stored in self.bin_avg, and the bin centres in self.bin_centres. 
        self.bins is used to determine the number of bins to use.
        """
        # self.bin_avg, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'], self.data['bvaSigEpr'], statistic='mean', bins = self.bins, range = [(0.0, 1.0)])
        weighted_sig, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'],
                                                                      self.data['bvaSigEpr'] * self.data['nWindows'],
                                                                      statistic='sum', bins=self.bins,
                                                                      range=[(0.0, 1.0)])
        total_windows, self.bin_edges, binnumber = ss.binned_statistic(self.data['Epr'], self.data['nWindows'],
                                                                       statistic='sum', bins=self.bins,
                                                                       range=[(0.0, 1.0)])
        self.bin_avg = weighted_sig / total_windows
        # print self.bin_centres, self.bin_centres.shape

    @on_trait_change('bin_edges')
    def update_bin_centres(self):
        self.bin_centres = (self.bin_edges[1:] + self.bin_edges[:-1]) / 2


class Dataset(FRETContainer):
    """ Model for the DatasetViewer. Does not add new functionality other than a 
    variable x and y axis that the DatasetViewer plot's against each other.
    """
    xaxis = Array
    yaxis = Array
    axis_manager = Instance(
        AxisManager)  # Object linked to GUI which indicates which axis the user want's to plot against each other.

    def update(self):
        # Using a dict might not be the most efficient way to do this, but I don't feel like typing a long elif statement.
        # It efficiency is comprimised this should be changed.  
        options = {'DD': self.data['F_Dexc_Dem'],
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

    @staticmethod
    def _count(arr, discard_zeros=True):
        """ Converts an array into a sorted array with the number of times each number occures in the array. 
        Since bincount will also report on number it counted zero times, we choose to discard those by default.

        USE ONLY ON ARRAYS OF INT's or if you don't care about flooring, because np.bincount only works on int's.
        """
        counts = np.bincount(arr.astype(int))
        return counts[counts > 0] if discard_zeros else counts

    @staticmethod
    def _unique(arr):
        """ Turns arr into a sorted array with unique elements. """
        return np.unique(arr)

class TitrationContainer(DataContainer):
    """
    The titration DataContainer is slightly different from the others.
    it depends on a list of concentrations vs. FRETContainers.
    """
    concentrations = List
    fret_containers = List(FRETContainer)
    data = Property
    data_orig = Property
    ratio = Array # Will hold the ratio's after fitting.

    corr = Instance(FretCorrections)
    thresholds = Instance(Thresholds)
    plot_options = Instance(PlotOptions)

    def _set_data(self, value):
        """
        Normal DataContainers currently have a data attribute, but this guy does
        not. However, we can use it to initiate the first experiment in the
        stack plot.
        """
        self.fret_containers = []
        self.fret_containers.append(
            FRETContainer(
                data=value,
                corr=self.corr,
                thresholds=self.thresholds,
                plot_options=self.plot_options,
            )
        )
        self.concentrations = []
        self.concentrations.append(0.0)

    def _set_data_orig(self, value):
        """
        Dummy method. This object does not have data.
        """
        pass

    def _apply_thresholds(self):
        """
        Wrapper for the same method on all the FRETContainers.
        """
        for fret_container in self.fret_containers:
            fret_container._apply_thresholds()

    def update(self):
        """ Order all of the FRETContainers to update. """
        for fret_container in self.fret_containers:
            fret_container.update()
        # fire on event to demonstrate that we have just updated the object so other can catch this
        self.updated = True


class BurstProperties(BurstContainer):
    """
    Look at burstsize distribution.

    We will not use the E histogram on FRETContainer, but we need the
    _accurate_E and _apply_thresholds.
    """
    burst_properties_options = Instance(BurstPropertiesOptions)
    burst_thresholds = Instance(BurstThresholds)
    # bin_centres = Array
    bin_edges = Array
    hist = Array
    # Thresholds must now be an alias for burst_thresholds
    thresholds = Property(depends_on='burst_properties')

    # def __init__(self, *args, **kwargs):
    #     super(BurstProperties, self).__init__(*args, **kwargs)
    #     self.remove_trait('thresholds') # Small hack because this object has slightly changed thresholds, burst_thresholds.


    def update(self):
        self._apply_thresholds()
        self._apply_range()
        xaxis = self.data[self.burst_properties_options.column]

        bin_size = {
            0: self.burst_properties_options.bin_size_time,
            1: self.burst_properties_options.bin_size_counts,
            2: self.burst_properties_options.bin_size_freq,
        }[self.burst_properties_options.mode]

        if self.burst_properties_options.mode == 2:
            xaxis = xaxis/self.data['Tau']
        self.bin_edges = np.arange(0, np.max(xaxis), bin_size)
        # self.bin_centres = np.diff(bin_edges)
        self.hist, edges = np.histogram(xaxis, bins=self.bin_edges)
        self.updated = True

    def _get_thresholds(self):
        return self.burst_thresholds

    # def _set_thresholds(self):
    #