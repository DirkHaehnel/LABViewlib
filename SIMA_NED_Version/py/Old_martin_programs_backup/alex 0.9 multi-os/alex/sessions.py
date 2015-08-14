from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event
from .models import ReducParams, Thresholds, EvsS, Metadata
from .functions import GuessesToFit
from .xml_converters import *
import xml.etree.ElementTree as ET
import os
from zipfile import ZipFile
import csv
import matplotlib.pyplot as plt

class SessionManager(HasTraits):
    """ Object responsible for saving and retrieving the session data to the 
    harddrive.
    """
    params = Instance(ReducParams)
    thresholds = Instance(Thresholds)
    evss = Instance(EvsS)
    fitting = Instance(GuessesToFit)
    metadata = Instance(Metadata)
    # save = Event
    # load = Event
    export = Event
    pdf = Event

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

    @on_trait_change('pdf')
    def create_pdf(self, path):
        """ Save the plot and metadata to a nicely typeset A4 sized pdf which contains all relevant data.
        This was a request from Thorben, so we have a single house style for the presentation of data
        on group meetings.
        path - valid file path.
        v_offset - vertical offset between the various tables and images.
        """
        v_offset = 30
        h_offset = 30
        root, ext = os.path.splitext(path)
        if not ext:
            ext = '.pdf'

        from reportlab.lib import colors
        from reportlab.lib.pagesizes import A4, cm
        from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Image, Paragraph
        from reportlab.pdfgen.canvas import Canvas
        from reportlab.lib.styles import getSampleStyleSheet

        stylesheet=getSampleStyleSheet()

        doc = Canvas('{}{}'.format(root, ext))
        doc.setFont("Helvetica", 12)    # For title text
        page_width, page_height = A4

        # Store the page width we have left so we can calculate whether the next table item would fit or if we need to cut off early and go to the next line.
        width_remainder = page_width
        height_remainder = page_height
        max_height = 0  # Max height in the current line. Must be reset at the beginning of a new line.
        column = 1         # Counter to monitor the current column. Used to autoscale the margins.
        for title, obj in [('Metadata', self.metadata), ('Burst Search Parameters', self.params), ('Thresholds', self.thresholds)]:
            table = Table([(label, unicode(value)) for label, value in obj.get_param_tuple()])
            # table = Table([('{}: {}'.format(label, value),) for label, value in obj.get_param_tuple()])
            # table = Table([(Paragraph(label, stylesheet['BodyText']), Paragraph(unicode(value), stylesheet['BodyText'])) for label, value in obj.get_param_tuple()])#, colWidths = [2.0 * cm, 5 * cm,])
            table.setStyle(TableStyle([ ('TEXTCOLOR',(0,0),(1,-1),colors.black),
                                        ('BOX', (0,0), (-1,-1), 0.25, colors.black)]))
            #('BACKGROUND',(0,0),(0,-1),colors.palegreen),
            table.wrapOn(doc, width_remainder, height_remainder)
            # Dimension of the table in the pdf
            w, h = table.wrap(0, 0)
            # Draw it on the canvas 

            # Draw the current table only if we have enough space remaining.
            if (h + v_offset < height_remainder) and (w + h_offset < width_remainder):
                # Title
                doc.drawString(page_width - width_remainder + h_offset*column, height_remainder - v_offset, title)
                # Draw on this position
                table.drawOn(doc, page_width - width_remainder + h_offset*column, height_remainder - h - v_offset)
                # Update the various measurements
                max_height = h if h > max_height else max_height
                width_remainder -= w
                column += 1
                print 'same line'

            # Apperantly we did not, so check if we still have enough height. If so, draw on the next line.
            elif h + v_offset < height_remainder:
                print ' new line'
                # Title
                doc.drawString(page_width - width_remainder + h_offset*column, height_remainder - v_offset, title)

                width_remainder = page_width        # New line
                height_remainder -= max_height    # Subtract the largest table to prevent overlapping tables.
                max_height = 0
                column = 1
                table.drawOn(doc, h_offset, height_remainder - h - v_offset)

        # Save the histogram to a temporary file on the disk, which is then placed in the pdf and removed immidiately after.
        temp_img = '{}{}.jpg'.format(root, ext)
        # plt.savefig(temp_img, bbox_inches='tight')

        I = Image(temp_img)
        I.drawHeight = (page_width - 2*h_offset)*I.drawHeight / I.drawWidth
        I.drawWidth = page_width - 2*h_offset
        w, h = table.wrap(0, 0)

        # Title
        doc.drawString(h_offset, I.drawHeight + 2*v_offset, 'Histogram')

        I.drawOn(doc, h_offset, v_offset)
        doc.save()
