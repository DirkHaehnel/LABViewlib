""" Main program. """
import enaml
from enaml.stdlib.sessions import simple_session, show_simple_view
from enaml.qt.qt_application import QtApplication
from alex.models import ReducParams, EvsS, BVA, Thresholds, PlotOptions, Metadata
from alex.views import EvsSView, BVAView
from alex.merge_bursts import DataManager
from alex.functions import GuessesToFit, Guess, Gaussian
from alex.sessions import SessionManager
from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Enum
import numpy as np
# If ENAML has a threading API, switch to that in the future
import threading

class WindowController(HasTraits):
    """ Base window controller. Base-type for all window controllers such as the Proximity Ratio Histogram and burstviewer.
    """
    # Enaml views
    window = Any

    @on_trait_change('window.close')
    def delete_after_close(self):
        """ When the user gives a close event on the window, we should also kill this object """
        raise NotImplementedError('Please delete me')

class BVAViewerController(WindowController):
    """ Controller for the BVAViewer window """
    thresholds = Instance(Thresholds)
    plot_options = Instance(PlotOptions)
    # The resulting data will later be given to the EvsS object
    bva = Instance(BVA)
    # Start a plot instance and options
    bva_view = Instance(BVAView)

    def __init__(self, data, file_window, *args, **kwargs):
        """ The PRH is initiated from a large dataset of bursts. 
        Everything else is handled from here.
        """
        self.thresholds = Thresholds()
        self.plot_options = PlotOptions()
        self.bva = BVA(data = data, thresholds = self.thresholds, options = self.plot_options)
        # Start a plot instance
        self.bva_view = BVAView(bva = self.bva)

        super(BVAViewerController, self).__init__(*args, **kwargs)
        # Start the processing window!
        self.window = BVAViewer(file_window, 
                                    bva_view = self.bva_view, 
                                    thresholds = self.thresholds, 
                                    plot_options = self.plot_options,
                                    title = 'BVA View - Detecting Dynamics Made Easy.')

class PRHController(WindowController):
    """ Controller for the proximity ratio histogram part of the ALEX suite. """
    params = Instance(ReducParams)
    thresholds = Instance(Thresholds)
    plot_options = Instance(PlotOptions)

    # The resulting data will later be given to the EvsS object
    evss = Instance(EvsS)
    # Start a plot instance and options
    evss_view = Instance(EvsSView)
    # Runs fitting based on the users parameters.
    fitting = Instance(GuessesToFit)
    # Storing and retrieving sessions is done by this object
    manager = Instance(SessionManager)
    metadata = Instance(Metadata)

    def __init__(self, data, file_window, *args, **kwargs):
        """ The PRH is initiated from a large dataset of bursts. 
        Everything else is handled from here.
        """
        super(PRHController, self).__init__(*args, **kwargs)
        self.params = file_window.params
        self.plot_options = PlotOptions()
        self.thresholds = Thresholds()
        self.metadata = Metadata()
        self.evss = EvsS(data = data, options = self.plot_options, thresholds = self.thresholds)

        print 'Your data sir', self.evss.data
        # Object for fitting based on user specified guesses
        self.fitting = GuessesToFit(evss = self.evss)
        # Start a plot instance
        self.evss_view = EvsSView(evss = self.evss, fitting = self.fitting)
        # Start the sessionmanager
        self.manager = SessionManager(params = self.params, 
                             thresholds = self.thresholds,
                             evss = self.evss,
                             fitting = self.fitting,
                             )

        # Start the processing window!
        self.window = Processing(file_window, 
                                    evss_view = self.evss_view, 
                                    thresholds = self.thresholds, 
                                    plot_options = self.plot_options,
                                    fitting = self.fitting,
                                    metadata = self.metadata,
                                    manager = self.manager,
                                    title = 'ALEX Suite')

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
            self.evss_view.update()

#     @on_trait_change('fitting.guesses.[type_E, type_S].+')
#     @on_trait_change('fitting:guesses.[type_E, type_S].update')
    @on_trait_change('fitting:guesses.[type_E, type_S].update')
    def show_guesses(self):
        """ When a guess is added or changed we want to show it's graph.
        The syntax might be confusing, but [type_E, type_S].update is applied
        to every item in guesses.
        """
        print 'added'
        if self.fitting.show_guesses:
            self.evss_view.update()

    @on_trait_change('fitting:guesses')
    def remove_fits(self):
        """ If the guesses list is emptied, we should also remove the old fit """
        if not self.fitting.guesses:
            self.fitting.fit_E = None
            self.fitting.fit_S = None
            self.fitting.fits = [None, None]
            self.evss_view.update()
            
    @on_trait_change('fitting.[show_E, show_S, show_guesses]')
    def redraw_plot(self):
        """ Redraw the plot with the new options. """
        print 'should update'
        try:
            self.evss_view.update()
        except IndexError: # Fails on decoration
            pass
        
    @on_trait_change('evss_view.rectangle')
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
        x0_E = (x1 + x2)/2.0
        x0_S = (y1 + y2)/2.0
        sig_E = np.abs(x1 - x2)/4.0 if x1 != x2 else 0.05
        sig_S = np.abs(y1 - y2)/4.0 if y1 != y2 else 0.05
        # Find indices in the array corrosponding to these values
        indmin_E, indmax_E = np.searchsorted(self.evss.Eedges, (x1, x2))
        indmax_E = min(len(self.evss.Eedges)-1, indmax_E)
        A_E = np.max(self.evss.H_Epr[indmin_E:indmax_E+1])
        indmin_S, indmax_S = np.searchsorted(self.evss.Sedges, (y1, y2))
        indmax_S = min(len(self.evss.Sedges)-1, indmax_S)
        A_S = np.max(self.evss.H_Sraw[indmin_S:indmax_S+1])
        # Get the maximum E and S in this area 
        guess = Guess(type_E = Gaussian(x0 = x0_E, sig = sig_E, A = A_E),
                      type_S = Gaussian(x0 = x0_S, sig = sig_S, A = A_S),
                      )
        self.fitting.guesses.append(guess)
        # self.fitting.guesses += [guess]

class Controller(HasTraits):
    """ Create a controller instance for linking all the objects and event 
    handling. Needed for propper mvc.
    """
    # Initiate an DataManager mass-converter
    data_manager = Instance(DataManager)
    # File window start imidiately with the program
    file_window = Any
    load_window = Any
    # Controllers that can be started based on options clicked. Should be generalised!!!!!!!!!!
    window_controller = Instance(WindowController)
    # Contains the analysis type to perform with the result from DataManager and is set by an DataManager.load event
    analysis_type = Enum('evss', 'burst_viewer', 'bva_viewer')

    def set_window_controller(self, result):
        self.window_controller = result

    @on_trait_change('data_manager.load')
    def show_load_window(self, new_value):
        if self.data_manager.files: # Will be false upon decorating
            self.load_window = LoadWindow(self.file_window, data_manager = self.data_manager, title = 'Running burst search')
            t = threading.Thread(target = self.data_manager.execute)
            t.start()
            self.analysis_type = new_value

    @on_trait_change('data_manager.merged_data')
    def done_loading(self):
        """ When merged_data is updated, loading must be done. Start the desired interface!
        """
        # if self.data_manager.len_wrapped_files == self.data_manager.len_files and self.data_manager.len_files != 0:
            # self.file_window.close()
        self.load_window.close()
        if self.analysis_type == 'evss':
            # Put the PRHController on the heap for execution by the main GUI thread.
            task = enaml.application.schedule(PRHController, args = [self.data_manager.merged_data, self.file_window])
            task.notify(self.set_window_controller)
        elif self.analysis_type == 'burst_viewer':
            task = enaml.application.schedule(BurstViewerController, args = [self.data_manager.merged_data, self.file_window])
            task.notify(self.set_window_controller)
        elif self.analysis_type == 'bva_viewer':
            task = enaml.application.schedule(BVAViewerController, args = [self.data_manager.merged_data, self.file_window])
            task.notify(self.set_window_controller)
            
     
    
    
# if __name__ == '__main__':
with enaml.imports():
    from alex.gui.main import Main
    from alex.gui.files.files import LoadWindow
    from alex.gui.processing.processing_window import Processing
    from alex.gui.bva.bvaviewer import BVAViewer

# Burst search setting
params = ReducParams()
# Initiate an data_manager mass-converter
data_manager = DataManager(setting = params)

# # Start the sessionmanager
# manager = SessionManager(params = params, 
#                          thresholds = thresholds,
#                          evss = evss,
#                          fitting = fitting,
#                          )
# Start the view instance
file_window = Main(params = params, 
                   data_manager = data_manager,
                   # manager = manager,
                   )
file_window.maximize()


ctrl = Controller(params = params,
                  data_manager = data_manager,
                  file_window = file_window,
                  # manager = manager,
                  )
# Start the app!
app = show_simple_view(file_window)