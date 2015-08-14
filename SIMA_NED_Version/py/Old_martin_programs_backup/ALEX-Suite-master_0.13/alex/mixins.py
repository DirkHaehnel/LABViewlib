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
from traits.api import HasStrictTraits, Instance, on_trait_change
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
)
import numpy as np

class CommonBase(HasStrictTraits):
    pass


class GUIOptionsMixin(CommonBase):
    pass
    # def __init__(self, *args, **kwargs):
    #     print self.__class__
    #     super(GUIOptionsMixin, self).__init__(*args, **kwargs)


class PlotOptionsMixin(GUIOptionsMixin):
    plot_options = Instance(PlotOptions)

    # def __init__(self, *args, **kwargs):
    #     super(PlotOptionsMixin, self).__init__(*args, **kwargs)
    #     self.plot_options = PlotOptions()

    @on_trait_change('plot_options.[S_min, S_max, E_min, E_max]')
    def update_range(self):
        self.data_container._apply_thresholds()
        self.data_container.update()

    @on_trait_change('plot_options.bins[]')
    def update_binning(self):
        self.data_container.update()

    @on_trait_change('plot_options.weighted, plot_options.log_scale')
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

        bg_data = BGData(photons, self.params)

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