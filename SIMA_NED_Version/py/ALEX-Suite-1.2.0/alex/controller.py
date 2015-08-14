import enaml
from enaml.stdlib.sessions import simple_session, show_simple_view
from enaml.qt.qt_application import QtApplication
from alex.models import ReducParams, EvsS, BVA, Thresholds, PlotOptions, Metadata, FretCorrections, Dataset, \
    AxisManager, PlotFormatOptions, BGData, DataContainer, TitrationContainer, \
    TitrationPresets, BurstProperties, BurstPropertiesOptions
from alex.views import EvsSPlot, BVAPlot, DatasetPlot, MPLBasePlot, TitrationPlot, BurstPropertiesPlot
from alex.merge_bursts import DataManager
from alex.functions import GuessesToFit, GuessesToFit1D, Guess, Gaussian, GuessesToFitTitration
from alex.sessions import SessionManager, TitrationWriter, BurstPropertiesWriter
from alex.mixins import *
from traits.api import HasTraits, HasStrictTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Enum, Dict
import numpy as np
# If ENAML has a threading API, switch to that in the future
import threading
from collections import OrderedDict # Used to create a list of unique items whilst preserving order.


class WindowController(HasTraits):
    """ Base window controller. Base-type for all window controllers such as the 
    Proximity Ratio Histogram and burstviewer.
    """
    # Enaml views
    window = Any
    data_manager = Instance(DataManager)
    data_container = Instance(DataContainer)
    plot = Instance(MPLBasePlot)

    def __init__(self, file_window, *args, **kwargs):
        # self.data_manager = data_manager
        super(WindowController, self).__init__(*args, **kwargs)


        # Get the class that data_container and plot should be an instance of.
        data_container_cls = self.traits()['data_container'].trait_type.klass
        plot_cls = self.traits()['plot'].trait_type.klass

        data_container_kwargs = self.prepare_kwargs(data_container_cls)
        # Intiate a DataContainer
        self.data_container = data_container_cls(
            data = file_window.data_manager.merged_data,
            **data_container_kwargs
        )

        # Initiate a fitting class
        if hasattr(self, 'fitting'):
            # Get the fitting traits default value (A GuessesToFit class)
            # and initiate it.
            # This should be cleaner if in the future we build a plotting API.
            fitting_cls = self.traits()['fitting'].trait_type.klass
            if fitting_cls is GuessesToFit:
                self.fitting = fitting_cls(evss=self.data_container, guesses=[])
            elif fitting_cls is GuessesToFitTitration:
                self.fitting = fitting_cls(data_container=self.data_container, guesses=[])
            else:
                self.fitting = fitting_cls(guesses=[])

        # Initiate a plot instance
        plot_kwargs = self.prepare_kwargs(plot_cls)
        self.plot = plot_cls(
            data_container = self.data_container, # Every plot needs data
            **plot_kwargs
        )

        # Launch the generic view of the data.
        with enaml.imports():
            from alex.gui.generic import GenericView

        self.window = GenericView(
            file_window,
            plot=self.plot,
            option_panels = self.get_options_panel_list(),
            fitting_panels = self.get_fitting_panel_list(),
            toolbar = self.get_toolbar(),
            title=self.get_window_title()
        )

    def prepare_kwargs(self, cls):
        """ Accepts a random class as input.
        We then look at the traits of this class to determine which plotoptions 
        should be provided.
        Example:
        class MyController(WindowController, ThresholdsMixin):
            data_container = Instance(MyDataContainer)
        Specifies a controller with thresholds support. Corresponding, we have:

        class MyDataContainer(DataContainer):
            thresholds = Instance(Thresholds)

        This function lives on MyController and therefore can be called as
        kwargs = self.prepare_kwargs(MyDataContainer)
        It should return:
        {'thresholds': self.thresholds}

        This way, we can dynamically initiate the data_container trait by doing:
        self.data_container = MyDataContainer(**kwargs)
        """
        # Get the GUIOptions from this controller and see which are available on
        # cls.
        gui_options = set(self.__class__.get_gui_options())
        # Fitting is not a gui_option but some objects may need it.
        # FittingOptions have been introduced but are not working yet.
        if hasattr(self, 'fitting'):
            gui_options.add('fitting')
        if hasattr(self, 'titration_presets'):
            gui_options.add('titration_presets')

        names = set(cls.class_editable_traits()) & gui_options
        return dict([(name, getattr(self, name)) for name in names])

    @on_trait_change('window.close')
    def delete_after_close(self):
        """ When the user gives a close event on the window, we should also kill this object """
        # raise NotImplementedError('Please delete me')
        pass

    # @classmethod
    # def get_gui_options(cls):
    #     """
    #     Subclasses of this object can have ascociated GUIOptions through Mixin classes.
    #     To get the allowed options in a generic way, this function returns a list of
    #     names of the attributes.
    #
    #     To allow WindowController's to inherit from one-another,
    #     we recursevely check for this method on the superclasses until it no
    #     longer exists. We the know we are dealing with the top-level object
    #     which inherits from mixins.
    #
    #     """
    #     def find_mixins_and_bases(bases):
    #         """
    #         This function seperates out GUIOptionsMixin's in a list of
    #         baseclasses. It returns both, so the non-mixins found this way
    #         can be subjected to the same process again.
    #         """
    #         mixins = []
    #         new_bases = []
    #         for base in bases:
    #             if GUIOptionsMixin in base.__bases__:
    #                 mixins += [base]
    #             else:
    #                 new_bases += [base]
    #         return mixins, new_bases
    #
    #     # First find all mixins in this WindowController.
    #     mixins = []
    #     bases = cls.__bases__
    #     while not WindowController in bases:
    #         new_mixins, new_bases = find_mixins_and_bases(bases)
    #         mixins += new_mixins
    #         bases = []
    #         for base in new_bases:
    #             bases += base.__bases__
    #     else: # We have arived at the top level. Find the last of them.
    #         new_mixins, new_bases = find_mixins_and_bases(bases)
    #         mixins += new_mixins
    #
    #     traits = []
    #     for base in mixins:
    #         traits += base.class_editable_traits()
    #     # Return all of them except for the ones which are in the WindowController baseclass itself.
    #     traits = list(OrderedDict.fromkeys(traits)) # This makes it unique
    #     return [trait for trait in traits if trait not in WindowController.class_editable_traits()]

    @classmethod
    def get_gui_options(cls, option_mixin=GUIOptionsMixin):
        """
        Subclasses of this object can have ascociated GUIOptions through Mixin classes.
        To get the allowed options in a generic way, this function returns a list of
        names of the attributes.

        To allow WindowController's to inherit from one-another,
        we recursevely check for this method on the superclasses until it no
        longer exists. We the know we are dealing with the top-level object
        which inherits from mixins.

        """
        def find_mixins_and_bases(bases):
            """
            This function seperates out option_mixin's in a list of
            baseclasses. It returns both, so the non-mixins found this way
            can be subjected to the same process again.
            """
            mixins = []
            new_bases = []
            for base in bases:
                if option_mixin in base.__bases__:
                    mixins += [base]
                else:
                    new_bases += [base]
            return mixins, new_bases

        # First find all mixins in this WindowController.
        mixins = []
        bases = cls.__bases__
        while not WindowController in bases:
            new_mixins, new_bases = find_mixins_and_bases(bases)
            mixins += new_mixins
            bases = []
            for base in new_bases:
                bases += base.__bases__
        else: # We have arived at the top level. Find the last of them.
            new_mixins, new_bases = find_mixins_and_bases(bases)
            mixins += new_mixins

        traits = []
        for base in mixins:
            traits += base.class_editable_traits()
        # Return all of them except for the ones which are in the WindowController baseclass itself.
        traits = list(OrderedDict.fromkeys(traits)) # This makes it unique
        return [trait for trait in traits if trait not in WindowController.class_editable_traits()]

    def get_options_panel_list(self):
        """ Return a list of the following format:
        [{'title': 'Some Title', 'options_panel': Panel}, ...]
        This can be used for a GenericView.
        The order of panels is determined from the mixin order in the class definition.
        """
        panel_dict = self._get_options_panel_dict()
        panel_list = []
        for option in self.__class__.get_gui_options():
            Panel = panel_dict[option]['options_panel']
            panel_dict[option]['options_panel'] = Panel(gui_option = getattr(self, option))
            panel_list.append(panel_dict[option])
        return panel_list

    def _get_options_panel_dict(self):
        """ Returns a dict of dicts containing the title for the panel and the class.
        It is initiated in get_panel_dict.
        """
        with enaml.imports():
            from alex.gui.panels.corr import BkgCorrOptionsPanel
            from alex.gui.panels.format_options import FormatOptionsPanel
            from alex.gui.panels.metadata import MetadataPanel
            from alex.gui.panels.plot_options import PlotOptionsPanel
            from alex.gui.panels.thresholds import ThresholdsPanel
            from alex.gui.panels.axis import AxisPanel
            from alex.gui.panels.evsslayers import EvsSLayersPanel
            from alex.gui.panels.titration import TitrationDirectoriesPanel
            from alex.gui.panels.burst_properties import BurstPropertiesPanel
            from alex.gui.panels.burst_thresholds import BurstThresholdsPanel
        return {
            'plot_options': {'title': 'Plot Options', 
                             'options_panel': PlotOptionsPanel
                             },
            'thresholds': {'title': 'Thresholds', 
                           'options_panel': ThresholdsPanel
                            },
            'burst_thresholds': {'title': 'Thresholds',
                               'options_panel': BurstThresholdsPanel
                                },
            'metadata': {'title': 'Metadata', 
                         'options_panel': MetadataPanel
                         },
            'format_options': {'title': 'Format Options', 
                               'options_panel': FormatOptionsPanel
                               },
            'corr': {'title': 'Accurate FRET', 
                     'options_panel': BkgCorrOptionsPanel
                     },
            'axis_manager': {'title': 'Axis Settings', 
                             'options_panel': AxisPanel
                              },
            'evss_layers': {'title': 'Layers', 
                            'options_panel': EvsSLayersPanel
                            },
            'titration_directories': {'title': 'Directories',
                            'options_panel': TitrationDirectoriesPanel
                            },
            'burst_properties_options': {'title': 'Settings',
                            'options_panel': BurstPropertiesPanel
                            },

        }

    def get_fitting_panel_list(self):
        """ Returns a dict of dicts containing the title for the panel and the class.
        It is initiated in get_panel_dict.
        """
        if hasattr(self, 'fitting'):
            # The following is needed because of the distinction between the 2D and 1D fitting.
            # This should be changed in the future: who cares how many variables you use,
            # the API should be the same.
            if isinstance(self.fitting, GuessesToFit):
                with enaml.imports():
                    from alex.gui.panels.fitting import GuessPanel, FitPanel
                return [
                    {'title': 'Guesses',
                     'fitting_panel': GuessPanel(guesses=self.fitting.guesses)
                      },
                    {'title': 'Fit Results',
                     'fitting_panel': FitPanel(fitting=self.fitting)
                     },
                ]
            elif isinstance(self.fitting, GuessesToFitTitration):
                # Titration has this.
                with enaml.imports():
                    from alex.gui.panels.titration_fitting import GuessTitrationPanel, FitTitrationPanel
                    from alex.gui.panels.titration_presets import TitrationPresetsPanel
                return [
                    {'title': 'Presets',
                     'fitting_panel': TitrationPresetsPanel(fitting_options=self.titration_presets)
                     },
                    # Freeform guessing is temporarily disabled as it does not work properly
                    # {'title': 'Guesses',
                    #  'fitting_panel': GuessTitrationPanel(fitting=self.fitting)
                    #   },
                    {'title': 'Fit Results',
                     'fitting_panel': FitTitrationPanel(fitting=self.fitting,)
                     },
                ]
            else:
                with enaml.imports():
                    from alex.gui.panels.fitting1D import Guess1DPanel, Fit1DPanel
                return [
                    {'title': 'Guesses',
                     'fitting_panel': Guess1DPanel(guesses=self.fitting.guesses)
                      },
                    {'title': 'Fit Results',
                     'fitting_panel': Fit1DPanel(fitting=self.fitting)
                     },
                ]
        else:
            return []

    def get_window_title(self):
        """ The window title for the enamlview. """
        raise NotImplemented()

    def get_toolbar(self):
        """ Toolbar for the enamlview. None is the default. """
        return None


class DatasetViewController(WindowController,
                            AxisManagerMixin,
                            PlotOptionsMixin,
                            ThresholdsMixin,
                            FretCorrectionsMixin,
                            PlotFormatOptionsMixin,
                            FittingDataPropertiesMixin,
                            ):
    """ Controller for the DatasetPlot window """
    # The resulting data will later be given to the EvsS object
    data_container = Instance(Dataset)
    # Start a plot instance and options
    plot = Instance(DatasetPlot)

    def get_window_title(self):
        return 'DatasetPloter: Plot any two properties of a dataset against one another.'




class BVAViewController(WindowController, 
                        ThresholdsMixin,
                        PlotOptionsMixin, 
                        PlotFormatOptionsMixin,
                        FretCorrectionsMixin,
                        ):
    """ Controller for the BVAPloter window """
    # The resulting data will later be given to the EvsS object
    data_container = Instance(BVA)
    # Start a plot instance and options
    plot = Instance(BVAPlot)

    def get_window_title(self):
        return 'BVA View'


class EvsSViewController(WindowController,
                         ThresholdsMixin, 
                         PlotOptionsMixin, 
                         EvsSLayersMixin,
                         MetadataMixin,
                         FretCorrectionsMixin,
                         PlotFormatOptionsMixin,
                         FittingEvsSMixin,
                         ):
    """ Controller for the proximity ratio histogram part of the ALEX suite. """

    # The resulting data will later be given to the EvsS object
    data_container = Instance(EvsS)
    # Start a plot instance and options
    plot = Instance(EvsSPlot)
    # Storing and retrieving sessions is done by this object
    manager = Instance(SessionManager)

    settings = Instance(ReducParams)

    def __init__(self, file_window, *args, **kwargs):
        """ The PRH is initiated from a large dataset of bursts. 
        Everything else is handled from here.
        """
        self.settings = file_window.params
        super(EvsSViewController, self).__init__(file_window, *args, **kwargs)


    def get_toolbar(self):
        # Start the sessionmanager
        self.manager = SessionManager(params=self.settings,
                                      thresholds=self.thresholds,
                                      evss=self.data_container,
                                      fitting=self.fitting,
                                      metadata=self.metadata,
                                      corr=self.corr,
                                      plot_options=self.plot_options,
                                      )

        with enaml.imports():
            from alex.gui.toolbars.evss import EvsSToolBar

        return EvsSToolBar(fitting = self.fitting,
                           plot = self.plot,
                           manager = self.manager)

    def get_window_title(self):
        return 'ALEX Suite'

    @on_trait_change('plot.rectangle')
    def add_guess_rect(self, rect):
        """ Using the drawn rectangle, we can add a new guess to the list.
        Format of rect:
        ((eclick.xdata, eclick.ydata), (erelease.xdata, erelease.ydata))
        """
        print 'rectangle event fired!'
        x1 = rect[0][0]
        x2 = rect[1][0]
        y1 = rect[0][1]
        y2 = rect[1][1]
        x0_E = (x1 + x2) / 2.0
        x0_S = (y1 + y2) / 2.0
        sig_E = np.abs(x1 - x2) / 4.0 if x1 != x2 else 0.05
        sig_S = np.abs(y1 - y2) / 4.0 if y1 != y2 else 0.05
        # Find indices in the array corrosponding to these values
        indmin_E, indmax_E = np.searchsorted(self.data_container.Eedges, (x1, x2)) if x1 <= x2 else np.searchsorted(
            self.data_container.Eedges, (x2, x1))
        indmax_E = min(len(self.data_container.Eedges) - 1, indmax_E)
        A_E = np.max(self.data_container.H_Epr[indmin_E:indmax_E + 1])
        indmin_S, indmax_S = np.searchsorted(self.data_container.Sedges, (y1, y2)) if y1 <= y2 else np.searchsorted(
            self.data_container.Sedges, (y2, y1))
        indmax_S = min(len(self.data_container.Sedges) - 1, indmax_S)
        A_S = np.max(self.data_container.H_Sraw[indmin_S:indmax_S + 1])
        # Get the maximum E and S in this area 
        guess = Guess(type_E=Gaussian(x0=x0_E, sig=sig_E, A=A_E),
                      type_S=Gaussian(x0=x0_S, sig=sig_S, A=A_S),
                      )
        self.fitting.guesses.append(guess)


class TitrationViewController(WindowController,
                              TitrationDirectoriesMixin,
                              ThresholdsMixin,
                              PlotOptionsMixin,
                              # FretCorrectionsMixin,
                              PlotFormatOptionsMixin,
                              FittingTitrationMixin,
                              ):
    """ Titrations are just the combination of many 1D FRET histograms.
    """
    data_container = Instance(TitrationContainer)
    plot = Instance(TitrationPlot)
    settings = Instance(ReducParams)
    manager = Instance(TitrationWriter)

    def __init__(self, file_window, *args, **kwargs):
        self.settings = file_window.params
        kwargs['titration_presets'] = TitrationPresets() # Remove this once FittingOptionsMixin also work!
        super(TitrationViewController, self).__init__(file_window, *args, **kwargs)
        # Initiate the TitrationDirectories to the values now in data_manager.
        # self.titration_directories.conditions = [[0.0, kwargs['data_manager'].sm_path]]
        self.titration_directories.concentrations = [0.0]
        self.titration_directories.paths = [kwargs['data_manager'].sm_path]
        self.fitting.fittings.append(GuessesToFit1D())

    def get_window_title(self):
        return 'Titration'

    def get_toolbar(self):
        # # Start the sessionmanager
        # self.manager = SessionManager(params=self.settings,
        #                               thresholds=self.thresholds,
        #                               evss=self.data_container,
        #                               fitting=self.fitting,
        #                               metadata=self.metadata,
        #                               corr=self.corr,
        #                               plot_options=self.plot_options,
        #                               )
        self.manager = TitrationWriter(data_container=self.data_container)

        with enaml.imports():
            from alex.gui.toolbars.titration import TitrationToolBar

        return TitrationToolBar(manager = self.manager)


class BurstPropertiesViewController(WindowController,
                                    BurstPropertiesOptionsMixin,
                                    BurstThresholdsMixin,
                                    # PlotOptionsMixin,
                                    # EvsSLayersMixin,
                                    # MetadataMixin,
                                    # FretCorrectionsMixin,
                                    PlotFormatOptionsMixin,
                                    ):
    data_container = Instance(BurstProperties)
    plot = Instance(BurstPropertiesPlot)

    def __init__(self, *args, **kwargs):
        super(BurstPropertiesViewController, self).__init__(*args, **kwargs)
        # Force the mode and no_bins to be configured properly.
        self.update_mode()

    def get_window_title(self):
        return 'Burst Properties'

    def get_toolbar(self):
        self.manager = BurstPropertiesWriter(data_container=self.data_container)

        with enaml.imports():
            from alex.gui.toolbars.burst_properties import BurstPropertiesToolBar

        return BurstPropertiesToolBar(manager = self.manager)


class Controller(HasTraits):
    """ Create a controller instance for linking all the objects and event 
    handling. Needed for propper mvc.
    """
    # Initiate an DataManager mass-converter
    data_manager = Instance(DataManager)
    # Global options
    thresholds = Instance(Thresholds)
    plot_options = Instance(PlotOptions)
    corr = Instance(FretCorrections)
    metadata = Instance(Metadata)
    format_options = Instance(PlotFormatOptions)
    params = Instance(ReducParams)
    burst_thresholds = Instance(BurstThresholds)
    # File window start imidiately with the program
    file_window = Any
    load_window = Any
    # Controllers that can be started based on options clicked. Should be generalised!!!!!!!!!!
    window_controller = Instance(WindowController)
    # Contains the analysis type to perform with the result from DataManager and is set by an DataManager.load event
    analysis_type = Str
    window_controllers = Dict({'evss': EvsSViewController, 
                               # 'burst_viewer': BurstViewController, 
                               'bva_viewer': BVAViewController, 
                               'dataset': DatasetViewController,
                               'titration': TitrationViewController,
                               'burst_properties': BurstPropertiesViewController,
                               })

    def __init__(self, *args, **kwargs):
        super(Controller, self).__init__(*args, **kwargs)
        # Set certain global options so they are the same for all views, making analysis a smoother experience.
        self.thresholds = Thresholds()
        self.plot_options = PlotOptions()
        self.corr = FretCorrections()
        self.metadata = Metadata()
        self.format_options = PlotFormatOptions()
        self.axis_manager = AxisManager()
        self.evss_layers = EvsSLayers()
        self.titration_directories = TitrationDirectories()
        self.burst_properties_options = BurstPropertiesOptions()
        self.burst_thresholds = BurstThresholds()
        # self.params = ReducParams()

    def set_window_controller(self, result):
        """ Remember that we have made a Window of a certain type so we can poll it's properties if needed. """
        self.window_controller = result
        if self.analysis_type in ['evss',]:
            # nasty bugfix to make sure the plot is interactive.
            t = threading.Thread(target=self._update_plot)
            t.start()

    @on_trait_change('data_manager.load')
    def show_load_window(self, new_value):
        print 'titration'
        if new_value == 'STOP':
            # Probably invalid files where supplied.
            # This will only happen when we are already showing a load window, so let's kill it. Destroy seems a bit harsh, but use whatever works.
            task = enaml.application.schedule(self.load_window.destroy)
        # elif new_value == 'titration':
        #     self._launch_interface(self.window_controllers[new_value])
        else:
            if self.data_manager.files:  # Will be false upon decorating
                with enaml.imports():
                    from alex.gui.files.files import LoadWindow
                self.load_window = LoadWindow(self.file_window, data_manager=self.data_manager,
                                              title='Running burst search')
                t = threading.Thread(target=self.data_manager.execute)
                t.start()
                self.analysis_type = new_value

    @on_trait_change('data_manager.merged_data')
    def done_loading(self):
        """ When merged_data is updated, loading must be done. Start the desired interface!
        When adding a new controller, do not forget to update the analysis_type Enum.
        """
        if self.load_window is not None:
            self.load_window.close()
            self._launch_interface(self.window_controllers[self.analysis_type])

    def _update_plot(self):
        """ Nasty hack to fix a bug where the plot is irresponcive until the first time it get's updated
        by manually updating the plot.
        Must be run from a seperate thread, otherwise Enaml will just delay popping up the window.
        """
        import time

        time.sleep(2)
        self.window_controller.data_container.update()

    def _launch_interface(self, controller):
        """ Private function that puts an Enaml view on the schedule to be launched. 
        controller : Any subclass of WindowController.
        """
        # Retrieve the relevan GUIOptions from this controller.
        kwargs = dict([(gui_option, getattr(self, gui_option)) for gui_option in controller.get_gui_options()])
        # print controller.get_gui_options()
        kwargs['data_manager'] = self.data_manager

        # Put the controller on the heap for execution by the main GUI thread.
        task = enaml.application.schedule(controller,
                                          args=[self.file_window,],
                                          kwargs=kwargs,
                                          )
        # When the task is done and the window is created, notify self.set_window_controller so we can keep track of this view.
        task.notify(self.set_window_controller)


