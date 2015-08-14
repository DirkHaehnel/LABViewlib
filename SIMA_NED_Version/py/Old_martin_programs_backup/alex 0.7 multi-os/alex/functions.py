import numpy as np
from traits.api import HasTraits, Float, List, Enum, Instance, Bool, on_trait_change, Unicode, Property, Event
import fit
from models import EvsS

class AbstractFunction(HasTraits):
    # Fired incase the params are updated and we want to display this
    update = Event
    
    def get_param_names(self):
        """ Return a list of string representations of the parameters. 
        If not defined, the list of traits is returned.
        """
#         return self.all_trait_names()[:-2]
        raise NotImplementedError('')
    
    def get_params(self):
        raise NotImplementedError('')
    
    def get_param_dict(self):
        return dict(zip(self.get_param_names(), self.get_params()))
    
    def set_params(self, **kwargs):
        """ Naively assumes no error's can occur. """
        for name, value in kwargs.iteritems():
            setattr(self, name, value)

    def get_area(self):
        raise NotImplementedError('')

    def get_areas(self):
        return [self.get_area()]

    def get_bounds(self):
        raise NotImplementedError('')

#     def set_params(self, *args):
#         """ General setter for traits, assumes the values to set are in the 
#         same order as their definition.
#         """
#         for name, value in args:
#             setattr(self, name, value)
# #         for key, trait in enumerate(self.all_trait_names()[:-2]):
# #             setattr(self, trait, args[key])

    def __add__(self, other):
        """ Adding children of this object should result in a new object with 
        the combined functionality.
        Adding None is allowed and results in 'self'.
        """
        if isinstance(other, Zero):
            return self
        else:
            _self = self # Needed so we can use the name self in the new class definition
            class Function(_self.__class__):
    #             @staticmethod
                def func(self, x, p):
                    arg_self = p[:len(_self.get_params())] 
                    arg_other = p[len(_self.get_params()):]
                    return _self.func(x, arg_self) + other.func(x, arg_other)
                
                def get_params(self):
                    return _self.get_params() + other.get_params()
                
                def get_param_names(self):
                    return _self.get_param_names() + other.get_param_names()

                def get_area(self):
                    return _self.get_area() + other.get_area()

                def get_areas(self):
                    return _self.get_areas() + other.get_areas()

                def get_bounds(self):
                    return _self.get_bounds() + other.get_bounds()
                
    #             def set_params(self, p):
    #                 arg_self = p[:len(_self.get_params())] 
    #                 arg_other = p[len(_self.get_params()):]
    #                 return _self.set_params(arg_self) + other.set_params(arg_other)
                
                def __unicode__(self):
                    return u'{} + {}'.format(_self, other)
            
            return Function()
        
class Gaussian(AbstractFunction):
    x0 = Float(0.5)
    sig = Float(0.05)
    A = Float(300.0)
    x0_fixed = Bool(False)
    sig_fixed = Bool(False)
    A_fixed = Bool(False)
    
#     @staticmethod
    def func(self, x, p):
        self.x0, self.sig, self.A = p
#         x0, sig, A = p
        return self.A*np.exp( -(x - self.x0)**2 / (2*self.sig)**2 )
#         return A*np.exp( -(x - x0)**2 / (2*sig)**2 )
    
    def get_params(self):
        return [self.x0, self.sig, self.A]
    
    def get_param_names(self):
        return ['x0', 'sig', 'A']
    
    def get_param_names_unicode(self):    
        return [u'x0', u'\u03c3', u'A']

    def get_bounds(self):
        """This function returns the bounds for the given function.
        At the moment, it is only used to fix the value of a parameter in fitting.
        """
        bounds = []
        bounds.append((self.x0, np.nextafter(self.x0, 1), ) if self.x0_fixed else (None, None, ))
        bounds.append((self.sig, np.nextafter(self.sig, 1), ) if self.sig_fixed else (None, None, ))
        bounds.append((self.A, np.nextafter(self.A, 1), ) if self.A_fixed else (None, None, ))
        print bounds
        return bounds

    def get_area(self):
        """ Return the area under a Gaussian, which is sqrt(pi/a) """
        return self.A*np.sqrt(np.pi*(2*self.sig)**2)
    
#     def get_param_names_save(self):
#         """ Real names, that can be used by """
    
    def __unicode__(self):
        return u'Gaussian({3} = {0:.3f}, {4} = {1:.3f}, {5} = {2:.3f})'.format(*(self.get_params() + self.get_param_names()))

class Constant(AbstractFunction):
    """ A constant that can be added to other functions """
    A0 = Float(1.0)
    A0_fixed = Bool(False)

#     @staticmethod
    def func(self, x, p):
        self.A0, = p
        # Return an array of the same size as x, but filled with A0
        ans = np.empty(x.shape)
        ans.fill(self.A0)
        return ans
    
    def get_params(self):
        return [self.A0]

    def get_bounds(self):
        """This function returns the bounds for the given function.
        At the moment, it is only used to fix the value of a parameter in fitting.
        """
        bounds = []
        bounds.append((self.A0, self.A0 + float('1E-20'), ) if self.A0_fixed else (None, None, ))
        return bounds
    
    def __unicode__(self):
        return u'Constant(A0 = {0:.3f})'.format(*self.get_params())
    
class Zero(AbstractFunction):
    """ Dummy object when no function is needed. """
    def func(self, x, p):
        return 0.0*x
     
    def get_params(self):
        return []

    def get_bounds(self):
        """This function returns the bounds for the given function.
        At the moment, it is only used to fix the value of a parameter in fitting.
        """
        return [(None, None, )]
     
    def __unicode__(self):
        return u'Zero()'
    
class Guess(HasTraits):
    # Only subclasses of AbstractFunction are allowed types
    type_E = Instance(AbstractFunction, Gaussian)
    type_S = Instance(AbstractFunction, Gaussian)
    name = Unicode
    
    def __init__(self, *args, **kwargs):
        super(Guess, self).__init__(*args, **kwargs)
        self.update_name()
        
    def get_params_E(self):
        return self.type_E.get_params()
    
    def get_params_S(self):
        return self.type_S.get_params()
        
    def __unicode__(self):
        return self.name
    
    def __str__(self):
        return unicode(self).encode('utf-8')
    
    @on_trait_change('type_E.+, type_S.+')
    def update_name(self):
        self.name = u'E: {}\nS: {}'.format(self.type_E, self.type_S)
#         """ Needed """
#         self.name_E = unicode(self.type_E)
#         self.name_S = unicode(self.type_S)
#         print(unicode(self))

# class AbstractGuessesToFit(HasTraits):
class GuessesToFit(HasTraits):
    """ Class that takes a number of Guess objects and turns them into a valid
    function for both E and S, that will than be used for fitting.
    """
    guesses = List(Guess)#, [Guess(type_E = Constant(), type_S = Constant())])
    evss = Instance(EvsS)
    fit_E = Instance(fit.Fit)
    fit_S = Instance(fit.Fit)
    fits = List(Instance(fit.Fit), [None, None], minlen = 2, maxlen = 2)
#     fits = Property()
    # This event is used to signal that fitting should start.
    run_fitting = Event
    show_E = Bool(True)
    show_S = Bool(True)
    show_guesses = Bool(True)
    
    def execute(self):
        """ Run the valid fit functions. """
        Func_E, Func_S = self._get_prototypes()
        print Func_E
        # Fit E to this function
        self.fit_E = fit.Fit(Func_E, self.evss.Ecentres, self.evss.H_Epr, 
                             p0 = self._get_fit_params('E'),
#                              bounds = self._get_fit_bounds('E'),
                             )
        self.fit_S = fit.Fit(Func_S, self.evss.Scentres, self.evss.H_Sraw, 
                             p0 = self._get_fit_params('S'),
#                              bounds = self._get_fit_bounds('S'),
                             )
        self.fit_E.execute()
        self.fit_S.execute()
        self.fits = [self.fit_E, self.fit_S]
        print self.fits
#         print '\nE:', self.fit_E
#         print 'S:', self.fit_S 
    #--------------------------------------------------------------------------
    # Private API
    #--------------------------------------------------------------------------
    
#     def _get_fits(self):
#         return self.fit_E, self.fit_S
        
    def _get_prototypes(self):
        """ This function determines the function prototype to use on the basis
        of the number of guesses and their type.
        """
        f_E = f_S = None
        for key, f_obj in enumerate([guess.type_E for guess in self.guesses]):
            if key == 0:
                f_E = f_obj
            else:
                f_E += f_obj
        for key, f_obj in enumerate([guess.type_S for guess in self.guesses]):
            if key == 0:
                f_S = f_obj
            else:
                f_S += f_obj
#         func = sum([guess.type_E for guess in self.guesses] + [Constant()])
        return f_E, f_S
        
    def _get_fit_params(self, axis):
        """ Retrieve the guessed params from every object. Add 1.0 for A0 to 
        the list. 
        """
        params = []
        if axis == 'E':
            for guess in self.guesses:
                params += guess.get_params_E()
#             params += Constant().get_params()
        elif axis == 'S':
            for guess in self.guesses:
                params += guess.get_params_S()
#             params += Constant().get_params()
        return np.array(params)
    
    def _get_fit_bounds(self, axis):
        """ The parameters need to be restricted as much as possible for fitting
        to stand a chance, In the future these values should be loaded from an
        xml file for semi-easy adaptation. For now hard coded will do.
        """
        for guess in self.guesses:
            guess.get_bounds()

# Dictionary exposing all allowed function types to the outside world.
func_dict = {'Gaussian': Gaussian, 'Zero': Zero, 'Constant': Constant}