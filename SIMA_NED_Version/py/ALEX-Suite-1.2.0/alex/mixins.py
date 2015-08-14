"""
Mixins for different GUIOptions.

This allows for flexible assignment of GUIOption support to a controller.
Example usage:
class MyController(WindowController, PlotOptionsMixin):
    pass

This creates a simple window controller with PlotOptions enabled.

WARNING:
When using Mixin's, always use them after the WindowController! 
Order matters.
"""
from traits.api import HasStrictTraits, Instance, on_trait_change, List
import numpy as np

from alex.models import (
    ReducParams, 
    Thresholds, 
    PlotOptions, 
    Metadata, 
    FretCorrections,
    AxisManager, 
    PlotFormatOptions,
    BGData,
    EvsSLayers,
    TitrationDirectories,
    FRETContainer,
    TitrationPresets,
    BurstPropertiesOptions,
    BurstThresholds,
)
from alex.functions import GuessesToFit, GuessesToFit1D, GuessesToFitTitration, Guess1D, Gaussian
from alex.merge_bursts import DataManager, Dataset

class CommonBase(HasStrictTraits):
    pass


class GUIOptionsMixin(CommonBase):
    pass

class FittingOptionsMixin(CommonBase):
    """ Base class for all fitting related GUIOptions.
    """
    pass


class PlotOptionsMixin(GUIOptionsMixin):
    plot_options = Instance(PlotOptions)

    @on_trait_change('plot_options.[S_min, S_max, E_min, E_max]')
    def update_range(self):
        """
        Update the plot range.
        """
        self.plot.update()
        # self.data_container._apply_thresholds()
        # self.data_container.update()

    @on_trait_change('plot_options.weighted, plot_options.log_scale, plot_options.bins[]')
    def update_binning(self):
        self.data_container.update()


class ThresholdsMixin(GUIOptionsMixin):
    thresholds = Instance(Thresholds)

    # The following function can be optimised by making it three or even 5 functions
    @on_trait_change('thresholds.updated')
    def apply_thresholds(self):
        print 'thresholds changed'
        self.data_container._apply_thresholds()
        self.data_container.update()

class BurstThresholdsMixin(GUIOptionsMixin):
    burst_thresholds = Instance(BurstThresholds)

    # The following function can be optimised by making it three or even 5 functions
    @on_trait_change('burst_thresholds.updated')
    def apply_thresholds(self):
        print 'thresholds changed'
        self.data_container._apply_thresholds()
        self.data_container.update()


class PlotFormatOptionsMixin(GUIOptionsMixin):
    format_options = Instance(PlotFormatOptions)

class ReducParamsMixin(GUIOptionsMixin):
    params = Instance(ReducParams)


class AxisManagerMixin(GUIOptionsMixin):
    axis_manager = Instance(AxisManager)

    @on_trait_change('axis_manager.updated')
    def change_axis(self, ):
        self.data_container.update()


class MetadataMixin(GUIOptionsMixin):
    metadata = Instance(Metadata)

class EvsSLayersMixin(GUIOptionsMixin):
    evss_layers = Instance(EvsSLayers)

    @on_trait_change('evss_layers.[show_E, show_S, show_guesses, show_S_range, show_E_range]')
    def redraw_plot(self):
        """ Redraw the plot with the new options. """
        print 'should update'
        try:
            self.plot.update()
        except IndexError:  # Fails on decoration
            pass


class FretCorrectionsMixin(GUIOptionsMixin):
    corr = Instance(FretCorrections)

    # This is a quick hack to implement automatic background correction 
    # in the otherwise beatiful architecture of this program.
    # It will be redesigned later, but Thorben realy wanted this feature ASAP.
    @on_trait_change('corr.thorben_hack')
    def script_bg(self, threshold):
        photons = self.data_manager.datasets[0].get_photons()

        bg_data = BGData(photons, self.settings)

        keys = ['DD', 'DA', 'AA']
        dic = {}

        for key in keys:
            data = bg_data.time_data[key]
            data_masked = np.ma.masked_greater(data, threshold)
            bg_mean = np.mean(data_masked)
            dic[key] = bg_mean

        # return dic
        # Set the new background values.
        self.corr.bkg_DA = dic['DA']
        self.corr.bkg_DD = dic['DD']
        self.corr.bkg_AA = dic['AA']

    @on_trait_change('corr.updated')
    def apply_corrections(self):
        """ New values for the corrections have been submitted. A new call to update suffieces to redo the calculations,
        since _accurate_E always gets called by update() because this ensures that any changes the user makes to 
        thresholds or plotoptions will not undo the accurate fret.
        """
        self.data_container.update()


class BurstPropertiesOptionsMixin(GUIOptionsMixin):
    burst_properties_options = Instance(BurstPropertiesOptions)

    @on_trait_change('burst_properties_options.updated')
    def apply_burst_options(self):
        self.data_container.update()

    @on_trait_change('burst_properties_options.[bin_size_freq, bin_size_time, bin_size_counts]')
    def update_no_bins(self, value=0.001):
        if self.burst_properties_options.mode == 2:
            lim = np.max(self.data_container.data[self.burst_properties_options.column]/self.data_container.data['Tau'])
        else:
            lim = np.max(self.data_container.data[self.burst_properties_options.column])
        self.burst_properties_options.no_bins = int(np.ceil(lim/value))

    @on_trait_change('burst_properties_options.[index, normalised]')
    def update_mode(self):
        """
        Change mode based on the new settings.
        """
        print 'updated mode'
        self.burst_properties_options.column = self.burst_properties_options.values[self.burst_properties_options.index]
        if self.burst_properties_options.column == 'Tau':
            self.burst_properties_options.mode = 0
            self.update_no_bins(value=self.burst_properties_options.bin_size_time)
        elif self.burst_properties_options.normalised:
            self.burst_properties_options.mode = 2
            self.update_no_bins(value=self.burst_properties_options.bin_size_freq)
        else:
            self.burst_properties_options.mode =1
            self.update_no_bins(value=self.burst_properties_options.bin_size_counts)
        print self.burst_properties_options.mode



class TitrationDirectoriesMixin(GUIOptionsMixin):
    titration_directories = Instance(TitrationDirectories)

    @on_trait_change('titration_directories.updated')
    def load_directories(self):
        """
        The user set all the directories and hopefulle concentrations.
        Lets first validate this and then run a datamanager for every dir.
        """
        # print self.titration_directories
        # # Remove empty rows.
        # try:
        #     while 1:
        #         self.titration_directories.paths.remove('')
        #         # The order is importent, because we dont want to throw away the last 0.0
        #         self.titration_directories.concentrations.remove(0.0)
        # except ValueError:  # No more available.
        #     pass
        #
        # print self.titration_directories

        # Sort by concentration
        conditions = zip(
            self.titration_directories.concentrations,
            self.titration_directories.paths,
        )
        print 'conditions: ', conditions
        try:
            while 1:
                conditions.remove((0.0, ''))
        except ValueError:  # No more available.
            pass

        conditions.sort()

        # Seperate concentration and paths in ordered lists.
        concentrations = []
        paths = []
        for concentration, path in conditions:
            concentrations.append(concentration)
            paths.append(path)

        # Both these lists should be unique
        if not self._unique(concentrations) or not self._unique(paths):
            print 'not all concentrations are unique'
            return False
        else:
            self.titration_directories.concentrations = concentrations
            self.titration_directories.paths = paths

        # Load up the files!
        self.data_container.fret_containers = []
        for path in paths:
            dm = DataManager(setting=self.settings)
            dm.sm_path = path
            dm.selected_files = dm.files
            # # selected_files is determined from whether or not a burst_file already exists.
            # # Thorben requested that we only open burstfiles that already exists.
            # # That way we can force researchers to look at their data first ^^.
            # for sm_file in dm.files:
            #     burst_file_name = Dataset._burst_file_name(sm_file)
            #     if Dataset._file_exists(burst_file_name, setting=self.settings):
            #         dm.selected_files.append(sm_file)
            # Run burst search for the entire dir
            dm.execute()
            # Add this dataset to a FRET container and add to the container.
            self.data_container.fret_containers.append(
                FRETContainer(
                    data=dm.merged_data,
                    corr=self.corr,
                    thresholds=self.thresholds,
                    plot_options=self.plot_options,
                )
            )

        # Empty without changing the actual list used in memory!
        # del self.data_container.concentrations[:]
        # for concentration in concentrations:
        #     self.data_container.concentrations.append(concentration)

        self.data_container.concentrations = concentrations
        self.data_container.update()

    def _unique(self, items):
        return len(set(items)) == len(items)


class GuessesMixin(FittingOptionsMixin):
    """ Guesses panel mixin. """
    pass

class FittingEvsSMixin(FittingOptionsMixin):
    # Runs fitting based on the users parameters.
    fitting = Instance(GuessesToFit)

    @on_trait_change('fitting:run_fitting')
    def execute_fitting(self):
        print 'run fitting'
        # Needed to prevent the code from being executed the first time
        try:
            self.fitting.execute()
        except AttributeError:
            pass
        else:
            # Now that fit data is available, update the plot
            self.plot.update()

    @on_trait_change('fitting:guesses.[type_E, type_S].update')
    def show_guesses(self):
        """ When a guess is added or changed we want to show it's graph.
        The syntax might be confusing, but [type_E, type_S].update is applied
        to every item in guesses.
        """
        print 'added'
        if self.evss_layers.show_guesses:
            self.plot.update()

    @on_trait_change('fitting:guesses[]')
    def remove_fits(self):
        """ If the guesses list is emptied, we should also remove the old fit """
        if not self.fitting.guesses:
            self.fitting.fit_E = None
            self.fitting.fit_S = None
            self.fitting.fits = [None, None]
            self.plot.update()


class FittingDataPropertiesMixin(FittingOptionsMixin):
    # Runs fitting based on the users parameters.
    fitting = Instance(GuessesToFit1D)

    @on_trait_change('fitting.run_fitting')
    def execute_fitting(self):
        # Needed to prevent the code from being executed the first time
        try:
            self.fitting.execute(self.data_container.xaxis, self.data_container.yaxis)
        except AttributeError:
            pass
        else:
            # Now that fit data is available, update the plot
            self.plot.update()

    @on_trait_change('fitting:guesses[]')
    def remove_fits(self):
        """ If the guesses list is emptied, we should also remove the old fit """
        if not self.fitting.guesses:
            self.fitting.fit_instance = None
            self.fitting.fits = [None]
            self.plot.update()


class FittingTitrationMixin(FittingOptionsMixin):
    """ Similar to a 1D histogram fitting, this mixin fits to a stack of
    FRETContainers.
    """
    fitting = Instance(GuessesToFitTitration)
    titration_presets = Instance(TitrationPresets)

    @on_trait_change('fitting.run_fitting')
    def execute_fitting(self):
        # Needed to prevent the code from being executed the first time
        ''
        try:
            # for fitting in self.fittings:
            #     fitting.execute(self.data_container.xaxis, self.data_container.yaxis)
            self.fitting.execute()
        except AttributeError:
            pass
        else:
            # Now that fit data is available, update the plot
            self.plot.update()

    @on_trait_change('data_container.concentrations')
    def initiate_fitting(self):
        """
        data_container.concentrations is updated after the user supplied all
        directories. Therefore, we can initiate as many GuessesToFit1D objects
        as we have concentrations in fitting.fittings.
        """
        print 'concentrations updated!'
        # temp = []
        del self.fitting.fittings[:]
        for concentration in self.data_container.concentrations:
            self.fitting.fittings.append(GuessesToFit1D())
        # self.fitting.fittings = temp
        self.fitting.concentrations = self.data_container.concentrations
        print self.fitting.fittings

    @on_trait_change('titration_presets.presets')
    def assign_presets(self, new_value):
        """
        A new preset has been selected. We must therefore change our
        guesses to match this.
        :new_value: 0 -> Two populations, fixed x0 and sigma.
        """
        # 'Two populations, fixed x0 and sigma.'
        print 'presets'
        del self.fitting.fittings[0].guesses[:]
        del self.fitting.fittings[-1].guesses[:]
        if new_value == 0:
            self.fitting.fittings[0].guesses.append(Guess1D())
            self.fitting.fittings[-1].guesses.append(Guess1D())
            # First fit to the two extremes.
            self.fitting._fit_fret_container(0)
            self.fitting._fit_fret_container(-1)

            # Get the values for the first and last condition
            initial = self.fitting.fittings[0].guesses[0].get_param_dict()
            final = self.fitting.fittings[-1].guesses[0].get_param_dict()
            # Now assing all the other guesses, using the results from the
            # extremes as fixed parameters.
            for key, fitting in enumerate(self.fitting.fittings[1:-1]):
                key += 1
                # Initiate it with fixed Gaussians, using values from the extremes
                del self.fitting.fittings[key].guesses[:]
                fitting.guesses.extend([
                    Guess1D(
                        function_type=Gaussian(
                            x0=initial['x0'],
                            sig=initial['sig'],
                            x0_fixed=True,
                            sig_fixed=True,
                        ),
                    ),
                    Guess1D(
                        function_type=Gaussian(
                            x0=final['x0'],
                            sig=final['sig'],
                            x0_fixed=True,
                            sig_fixed=True,
                        ),
                    )
                ])
                # self.fitting._fit_fret_container(key)
        self.execute_fitting()

    @on_trait_change('titration_presets.mode')
    def assign_mode(self, new_value):
        """
        Change the plotting mode.
        """
        print 'mode changed'
        self.plot.mode = new_value