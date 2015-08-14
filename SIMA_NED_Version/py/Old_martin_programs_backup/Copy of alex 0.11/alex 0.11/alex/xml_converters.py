from traits.api import HasTraits, Instance, ListUnicode, Array, Str, on_trait_change, Int, Any, Event
import xml.etree.ElementTree as ET
from alex.functions import AbstractFunction, Guess, GuessesToFit, func_dict
from alex.fit import Fit
from alex.models import Thresholds, ReducParams, EvsS
import numpy as np

class BaseXML(HasTraits):
    """ Base object to xml converter """
    parent = Instance(ET.Element)
    
    def encode(self):
        raise NotImplementedError('')
    def decode(self):
        raise NotImplementedError('')
    
class FunctionXML(BaseXML):
    func = Instance(AbstractFunction)
    
    def encode(self):
        """While encoding, the parent is the element to which we will add e.g.
        <Gaussian A="3.72861884098" sig="0.0563813583347" x0="0.451774281765"/>
        """
        param_dict = dict([(str(name), str(value)) for name, value in self.func.get_param_dict().items()])
        return ET.SubElement(self.parent, self.func.__class__.__name__, param_dict)

    def decode(self, name):
        """ To decode, the parent should have functions as children, and
        we can retrieve the child with 'name'.
        """
        # Should only be one, but we don't know it's name. Could be 'fit', 'gaussian', etc.
        element = parent.find(name)
        self.func.set_params(**element.attrib)

class GuessXML(BaseXML):
    guess = Instance(Guess)
    
    def encode(self):
        print 'encoding guess'
        guess_xml = ET.SubElement(self.parent, 'guess')
        type_E_xml = ET.SubElement(guess_xml, 'type_E')
        FunctionXML(parent = type_E_xml, func = self.guess.type_E).encode()
        
        type_S_xml = ET.SubElement(guess_xml, 'type_S')
        FunctionXML(parent = type_S_xml, func = self.guess.type_S).encode()
        return guess_xml

    def decode(self):
        type_E_xml = self.parent.find('type_E')
        func_E_name = list(type_E_xml)[0].tag
        self.guess.type_E = func_dict(func_E_name)
        FunctionXML(parent = type_E_xml, 
                    func = self.guess.type_E).decode(name = func_E_name)

        type_S_xml = self.parent.find('type_S')
        func_S_name = list(type_S_xml)[0].tag
        self.guess.type_S = func_dict(func_S_name)
        FunctionXML(parent = type_S_xml, 
                    func = self.guess.type_S).decode(name = func_S_name)
    
# class FitXML(FunctionXML):
#     """ Fits are ultemately just AbstractFunctions """
#     # Name of this node in the xml file
#     name = Str

#     def encode(self):
#         param_dict = dict([(str(name), str(value)) for name, value in self.func.get_param_dict().items()])
#         print self.func.get_param_names()
#         return ET.SubElement(self.parent, self.name, param_dict)

#     def decode(self, name):
#         # The parent will be an instance of Fit
#         FunctionXML(parent = self.parent, 
#                     func = self.guess.type_S).decode(name = name)
        
class ThresholdsXML(BaseXML):
    thresholds = Instance(Thresholds)
    
    def encode(self):
        thresholds_xml = ET.SubElement(self.parent, 'thresholds')
        for name in self.thresholds.get_names():
            element = ET.SubElement(thresholds_xml, name)
            element.text = '{}'.format(getattr(self.thresholds, name))
        return thresholds_xml
    
class GuessesXML(BaseXML):
    fitting = Instance(GuessesToFit)
    
    def encode(self):
        guesses_xml = ET.SubElement(self.parent, 'guesses')
        for guess in self.fitting.guesses:
            print guess
            GuessXML(parent = guesses_xml, guess = guess).encode()
        return guesses_xml

    def decode(self):
        guesses_xml = self.parent.find('guesses')
        for key, guess_xml in enumerate(guesses_xml):
            GuessXML(parent = guess_xml, guess = self.fitting.guesses[key]).decode()
    
# class FitsXML(BaseXML):
#     fitting = Instance(GuessesToFit)
    
#     def encode(self):
#         fits_xml = ET.SubElement(self.parent, 'fits')
#         for name, fit in zip(['fit_E', 'fit_S'], self.fitting.fits):
#             if fit:
#                 print name
#                 FitXML(parent = fits_xml, func = fit.Func, name = name).encode()
#         return fits_xml

#     def decode(self):
#         fits_xml = self.parent.find('fits')
#         for name in ['fit_E', 'fit_S']:
#             setattr(self.fitting, name, Fit())
#             FitXML(parent = fits_xml, func = getattr(self.fitting, name), name = name).decode(name = name)

class MetadataXML(BaseXML):
    params = Instance(ReducParams)
    
    def encode(self):
        meta_xml = ET.SubElement(self.parent, 'metadata')
        for name, value in self.params.get_param_tuple():
            element = ET.SubElement(meta_xml, name)
            element.text = value
        return meta_xml

    def decode(self):
        meta_xml = self.parent.find('metadata')
        metadata = dict([(item.tag, item.text) for item in meta_xml])
        self.params.set_params(**metadata)
    
class DataXML(BaseXML):
    """ Saving the merged data is done in the following way:
    - The xml contains the version of the data array.
    In the future column names etc may be changed, which will be a new version
    of the array spec.
    """ 
    evss = Instance(EvsS)
    directory = Str # Must be a valid path
    name = Str # Session name 
    version = Str('1.0')
    
    def encode(self):
        data_xml = ET.SubElement(self.parent, 'data')
        version = ET.SubElement(data_xml, 'version')
        assert self.version == '1.0'
        file_name = '{}.npz'.format(self.name)
        with open(file_name, 'wb') as f:
            np.savez_compressed(f, self.evss.data_orig)
#             np.savez(f, self.evss.data_orig)
        file_name_xml = ET.SubElement(data_xml, 'file_name')
        file_name_xml.text = file_name
        return data_xml

    def decode(self, zipfile):
        data_xml = self.parent.find('data')
        file_name = data_xml.find('file_name').text
        zipfile.extract(file_name)
        with zipfile.open(file_name, 'r') as data_file:
            self.evss.data_orig = np.load(data_file)