from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event
from .models import ReducParams, Thresholds, EvsS
from .functions import GuessesToFit
from .xml_converters import *
import xml.etree.ElementTree as ET
import os
from zipfile import ZipFile
import csv

class SessionManager(HasTraits):
    """ Object responsible for saving and retrieving the session data to the 
    harddrive.
    """
    params = Instance(ReducParams)
    thresholds = Instance(Thresholds)
    evss = Instance(EvsS)
    fitting = Instance(GuessesToFit)
    # save = Event
    # load = Event
    export = Event

    def encode(self, name):
        top = ET.Element('session')
        experiment = ET.SubElement(top, 'experiment', )#{'name': experiment_name})
        # Convert all the different objects to XML
        fits = FitsXML(parent = experiment, fitting = self.fitting).encode()
        guesses = GuessesXML(parent = experiment, fitting = self.fitting).encode()
        metadata = MetadataXML(parent = experiment, params = self.params).encode()
        data = DataXML(parent = experiment, evss = self.evss, name = name).encode()
        # data = DataXML(parent = experiment, evss = self.evss, directory = directory, name = name).encode()
        thresholds = ThresholdsXML(parent = experiment, thresholds = self.thresholds).encode()
        return top

    def decode(self, xml_file, zip_file):
        """ Reads an xml and turns it into the relevant objects."""
        tree = ET.parse(xml_file)
        root = tree.getroot()
        experiment = root.find('experiment')
        # Read data from XML
        # fits = FitsXML(parent = experiment, fitting = self.fitting).decode()
        # guesses = GuessesXML(parent = experiment, fitting = self.fitting).decode()
        metadata = MetadataXML(parent = experiment, params = self.params).decode()
        data = DataXML(parent = experiment, evss = self.evss).decode(zip_file)
        # thresholds = ThresholdsXML(parent = experiment, thresholds = self.thresholds).decode()
    
#     @on_trait_change('save')
#     def save_session(self, path):
#         """ path containes the user specified file name + dir to store in. """
# #         raise NotImplementedError('')
#         directory, name = os.path.split(path)
#         top = self.encode(name)
# #         tree = ET.ElementTree(top)
#         xml_str = ET.tostring(top)
#         with ZipFile('{}.zip'.format(path), 'w') as myzip:
#             myzip.writestr('{}.xml'.format(name), xml_str)
#             # Write the data onto the hdd, then into the zip
#             with open('{}.npz'.format(path), 'wb') as f:
#                 np.savez_compressed(f, self.evss.data_orig)
#             # Place it in the archive and delete the temporary file
#             myzip.write('{}.npz'.format(path),'{}.npz'.format(name))
#             os.remove('{}.npz'.format(path))
#         #Write information on this seeion in the session log
            
# #         tree.write('{}.xml'.format(dir, name))
     
#     @on_trait_change('load')
#     def load_session(self, path):
#         """ path containes the user specified file name + dir to load from. """
#         root, ext = os.path.splitext(path)
#         print root, ext
#         if ext != '.zip':
#             raise IOError('Invalid filetype. Please select a .zip file.')
#         with ZipFile('{}'.format(path), 'r') as myzip:
#             for filename in myzip.namelist():
#                 root, ext = os.path.splitext(filename)
#                 if ext == '.xml':
#                     # Retrieve all session information from the xml.
#                     with myzip.open(filename) as xml_file:
#                         self.decode(xml_file, myzip)
#                 elif ext == '.npz':
#                     pass    # This type of file is allowed, but I only want to load the file referenced in the .xml
#                 else:
#                     raise IOError('The specified archive contains files I do not recognise.')

    @on_trait_change('export')
    def export_session(self, path):
        """ path containes the user specified file name + dir to load from. """
        root, ext = os.path.splitext(path)
        if not ext:
            ext = '.csv'
        print root, ext
        with open('{}{}'.format(root, ext), 'w') as f:
            csv_writer = csv.writer(f)
            # Write the metadata about the used burstsearch first
            csv_writer.writerow(['METADATA'])
            csv_writer.writerow(['BURST SEARCH PARAMETERS'])
            for param_tuple in self.params.get_param_tuple():
                csv_writer.writerow(param_tuple)
            # Applied Thresholds
            csv_writer.writerow(['THRESHOLDS'])
            for param_tuple in self.params.get_param_tuple():
                csv_writer.writerow(param_tuple)

            csv_writer.writerow(['DATA'])
            csv_writer.writerow(['Bin Centres', 'E Histogram', 'S Histogram'])
            for x, E, S in zip(self.evss.Ecentres, self.evss.H_Epr, self.evss.H_Sraw):
                csv_writer.writerow([x, E, S])

            csv_writer.writerow([''])
            csv_writer.writerow(['2D Histogram'])
            for row in self.evss.H_EvsS:
                csv_writer.writerow(row)