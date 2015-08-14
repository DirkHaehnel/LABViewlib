""" Main program. """
import enaml
from enaml.stdlib.sessions import show_simple_view
from alex.models import ReducParams
from alex.merge_bursts import DataManager
from alex.controller import Controller
# We are going to log all uncaught exceptions. Sweet!
import logging
import sys
import time
import os

def exception_handler(type, value, tb):
    logger.exception("Uncaught exception: {0}".format(str(value)))

def generate_log_filename(base = 'Embarrassing Moments'):
    """ 
    A filename for this logger must always contain the base string plus
    the time at which the program was started in seconds.
    """
    t = time.strftime('%Y-%m-%d %H-%M-%S', time.localtime())
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), 'log', '{} {}.log'.format(base, t))

# Configure logger to write to a file
logging.basicConfig(filename = generate_log_filename(), level = logging.DEBUG)
logger = logging.getLogger(__name__)
# Install exception handler
sys.excepthook = exception_handler

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