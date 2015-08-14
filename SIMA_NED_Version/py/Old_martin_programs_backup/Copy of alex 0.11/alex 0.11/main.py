""" Main program. """
import enaml
from enaml.stdlib.sessions import show_simple_view
from alex.models import ReducParams
from alex.merge_bursts import DataManager
from alex.controller import Controller

if __name__ == '__main__':
    with enaml.imports():
        from alex.gui.main import Main

    # Burst search setting
    params = ReducParams()
    # Initiate a data_manager mass-converter, for reading .sm files in bulk
    data_manager = DataManager(setting = params)

    # # Start the sessionmanager, used for storing sessions to HDD
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