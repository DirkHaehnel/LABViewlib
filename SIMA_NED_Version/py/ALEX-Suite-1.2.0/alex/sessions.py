from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event, Unicode, HasStrictTraits
from .models import ReducParams, Thresholds, EvsS, Metadata, FretCorrections, PlotOptions, TitrationContainer, BurstProperties
from .functions import GuessesToFit
from .xml_converters import *
from .merge_bursts import SessionFile
import xml.etree.ElementTree as ET
import os
from zipfile import ZipFile
import csv
import matplotlib.pyplot as plt


class TitrationWriter(HasStrictTraits):
    export = Event
    export_path = Unicode
    data_container = Instance(TitrationContainer)

    @on_trait_change('export')
    def export_session(self, path):
        """ path containes the user specified file name + dir to write to. """
        root, ext = os.path.splitext(path)
        if not ext:
            ext = '.csv'

        with open('{}{}'.format(root, ext), 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['concentration', 'ratio', 'std dev ratio'])
            for row in zip(self.data_container.concentrations, self.data_container.ratio, ):
                print row
                writer.writerow(row)


class BurstPropertiesWriter(HasStrictTraits):
    export = Event
    export_path = Unicode
    data_container = Instance(BurstProperties)

    @on_trait_change('export')
    def export_session(self, path):
        """ path containes the user specified file name + dir to write to. """
        root, ext = os.path.splitext(path)
        if not ext:
            ext = '.csv'

        with open('{}{}'.format(root, ext), 'w') as f:
            writer = csv.writer(f)
            # Metadata
            writer.writerow(['METADATA'])
            for row in self.data_container.burst_properties_options.get_param_tuple():
                writer.writerow(row)
            # Data
            writer.writerow(['DATA'])
            if self.data_container.burst_properties_options.mode == 0: # 0 -> tau, 1-> events, 2 -> freq
                x_title = 'Tau'
            elif self.data_container.burst_properties_options.mode == 1:
                x_title = 'Counts'
            else:
                x_title = 'Frequency'
            writer.writerow([x_title, 'Number of Bursts'])
            for row in zip(self.data_container.bin_edges, self.data_container.hist, ):
                writer.writerow(row)


class SessionManager(HasTraits):
    """ Object responsible for saving and retrieving the session data to the 
    harddrive.
    """
    params = Instance(ReducParams)
    thresholds = Instance(Thresholds)
    evss = Any
    fitting = Instance(GuessesToFit)
    metadata = Instance(Metadata)
    corr = Instance(FretCorrections)
    plot_options = Instance(PlotOptions)
    # save = Event
    # load = Event
    export = Event
    pdf = Event
    export_path = Unicode

    def encode(self, name):
        top = ET.Element('session')
        experiment = ET.SubElement(top, 'experiment', )  # {'name': experiment_name})
        # Convert all the different objects to XML
        fits = FitsXML(parent=experiment, fitting=self.fitting).encode()
        guesses = GuessesXML(parent=experiment, fitting=self.fitting).encode()
        metadata = MetadataXML(parent=experiment, params=self.params).encode()
        data = DataXML(parent=experiment, evss=self.evss, name=name).encode()
        # data = DataXML(parent = experiment, evss = self.evss, directory = directory, name = name).encode()
        thresholds = ThresholdsXML(parent=experiment, thresholds=self.thresholds).encode()
        return top

    def decode(self, xml_file, zip_file):
        """ Reads an xml and turns it into the relevant objects."""
        tree = ET.parse(xml_file)
        root = tree.getroot()
        experiment = root.find('experiment')
        # Read data from XML
        # fits = FitsXML(parent = experiment, fitting = self.fitting).decode()
        # guesses = GuessesXML(parent = experiment, fitting = self.fitting).decode()
        metadata = MetadataXML(parent=experiment, params=self.params).decode()
        data = DataXML(parent=experiment, evss=self.evss).decode(zip_file)
        # thresholds = ThresholdsXML(parent = experiment, thresholds = self.thresholds).decode()

    # @on_trait_change('save')
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
        # root, ext = os.path.splitext(path)
        # if not ext:
        #     ext = '.csv'
        # print root, ext
        # with open('{}{}'.format(root, ext), 'w') as f:
        #     csv_writer = csv.writer(f)
        #     # Write the metadata about the used burstsearch first
        #     csv_writer.writerow(['METADATA'])
        #     csv_writer.writerow(['BURST SEARCH PARAMETERS'])
        #     for param_tuple in self.params.get_param_tuple():
        #         csv_writer.writerow(param_tuple)
        #     # Applied Thresholds
        #     csv_writer.writerow(['THRESHOLDS'])
        #     for param_tuple in self.thresholds.get_param_tuple():
        #         csv_writer.writerow(param_tuple)

        #     csv_writer.writerow(['DATA'])
        #     csv_writer.writerow(['Bin Centres', 'E Histogram', 'S Histogram'])
        #     for x, E, S in zip(self.evss.Ecentres, self.evss.H_Epr, self.evss.H_Sraw):
        #         csv_writer.writerow([x, E, S])

        #     csv_writer.writerow([''])
        #     csv_writer.writerow(['2D Histogram'])
        #     for row in self.evss.H_EvsS:
        #         csv_writer.writerow(row)
        session_file = SessionFile(file_path=path,
                                   params=self.params,
                                   thresholds=self.thresholds,
                                   evss=self.evss,
                                   fitting=self.fitting,
                                   metadata=self.metadata,
                                   corr=self.corr,
                                   plot_options=self.plot_options)
        session_file.write()

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
        from reportlab.lib.units import inch
        from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Image, Paragraph, Frame
        from reportlab.pdfgen.canvas import Canvas
        from reportlab.lib.styles import getSampleStyleSheet

        stylesheet = getSampleStyleSheet()

        doc = SimpleDocTemplate('{}{}'.format(root, ext), pagesize=A4)
        c = Canvas('{}{}'.format(root, ext))
        elements = []  # The order of items in elements is important, as it is assumed in the placement in the pdf.
        page_width, page_height = A4

        # Store the page width we have left so we can calculate whether the next table item would fit or if we need to cut off early and go to the next line.
        width_remainder = page_width
        height_remainder = page_height

        # Save the histogram to a temporary file on the disk, which is then placed in the pdf and removed immidiately after.
        temp_img = '{}{}.png'.format(root, ext)
        plt.savefig(temp_img, bbox_inches='tight')
        pdf_img = '{}{}.pdf'.format(root, ext)
        plt.savefig(pdf_img, bbox_inches='tight')

        # The picture should take up 3/4 of the width
        I = Image(temp_img)
        I.drawHeight = (3. / 4) * (page_width - 2 * h_offset) * I.drawHeight / I.drawWidth
        I.drawWidth = (3. / 4) * (page_width - 2 * h_offset)
        # Place it on the page
        I.drawOn(c, h_offset, page_height - I.drawHeight - v_offset)

        # Fitting Reuslts
        title_flowable = Paragraph('<b>Fit Results</b>', stylesheet['BodyText'])
        elements.append(title_flowable)

        if self.fitting.fits[0] is None:
            table = Table([(Paragraph('E', stylesheet['BodyText']), Paragraph('None', stylesheet['BodyText'])),
                           (Paragraph('S', stylesheet['BodyText']), Paragraph('None', stylesheet['BodyText'])),
                           ])
        else:
            table = Table([(Paragraph(u'E: <br/>{}'.format(self.fitting.fits[0]), stylesheet['BodyText']),),
                           (Paragraph(u'S: <br/>{}'.format(self.fitting.fits[1]), stylesheet['BodyText']),),
                           ])
        table.setStyle(TableStyle([('TEXTCOLOR', (0, 0), (-1, -1), colors.black),
                                   ('FONTSIZE', (0, 0), (-1, -1), 10),
                                   ('BOTTOMPADDING', (0, 0), (-1, -1), 1),
                                   ('TOPPADDING', (0, 0), (-1, -1), 1),
                                   ]))
                                   # ('BOX', (0,0), (-1,-1), 0.25, colors.black)

        elements.append(table)

        for title, obj in [('<b>Thresholds</b>', self.thresholds), ('<b>Accurate FRET</b>', self.corr),
                           ('<b>Metadata</b>', self.metadata), ('<b>Burst Search Parameters</b>', self.params),
                           ('<b>Plot Options</b>', self.plot_options), ]:
            # table = Table([(label, unicode(value)) for label, value in obj.get_param_tuple()])
            # table = Table([('{}: {}'.format(label, value),) for label, value in obj.get_param_tuple()])
            title_flowable = Paragraph(title, stylesheet['BodyText'])
            elements.append(title_flowable)

            table = Table(
                [(Paragraph(label, stylesheet['BodyText']), Paragraph(unicode(value), stylesheet['BodyText'])) for
                 label, value in obj.get_param_tuple()])  #, colWidths = [2.0 * cm, 5 * cm,])
            table.setStyle(TableStyle([('TEXTCOLOR', (0, 0), (-1, -1), colors.black),
                                       ('FONTSIZE', (0, 0), (-1, -1), 10),
                                       ('BOTTOMPADDING', (0, 0), (-1, -1), 1),
                                       ('TOPPADDING', (0, 0), (-1, -1), 1),
                                       ]))
                                       # ('BOX', (0,0), (-1,-1), 0.25, colors.black)

            elements.append(table)

        # raise Exception(elements)

        # Column to the right of the image
        f = Frame(h_offset + I.drawWidth, page_height - I.drawHeight - v_offset, (1. / 4) * (page_width - 2 * h_offset),
                  I.drawHeight, showBoundary=0)
        f.addFromList(elements[2:4], c)

        # Bottom-left column
        f = Frame(h_offset, v_offset, .5 * page_width - h_offset, page_height - I.drawHeight - 2 * v_offset,
                  showBoundary=0)
        f.addFromList(elements[:2] + elements[4:8], c)

        # Bottom-right column
        f = Frame(.5 * page_width, v_offset, .5 * page_width - h_offset, page_height - I.drawHeight - 2 * v_offset,
                  showBoundary=0)
        f.addFromList(elements[8:], c)

        c.save()