# from scipy.optimize import curve_fit
import numpy as np
from leastsqbound import leastsqbound
import inspect
from traits.api import HasTraits, Array, Float

class Fit(HasTraits):
    """ scipy's curve_fit does not give any other statistical information.
    That's where this object comes in.
    """
    residues = Array
    func_i = Array
    r_squared = Float
    
    def __init__(self, Func, x, y, p0=None, sigma=None, **kw):
        # An AbstractFunction object
        self.Func = Func
        # A Pythonic function for the actual mathematical expression
        self.func = Func.func
        self.x = x
        self.y = y
        self.p0 = p0
        self.sigma = sigma
        self.kw = kw
        print unicode(Func)

    def execute(self,):
        print 'Executing FIT'
        self.popt, self.cov_x, self.infodic, self.mesg, self.ier = leastsqbound(self.error, self.p0, args=(self.func, self.x, self.y), bounds=self.Func.get_bounds(), full_output=True)
        self.Func.updated = True
#         self.Func.set_params(*self.get_popt_tuple())
#         self.popt, self.pcov = curve_fit(func, x, y, p0, sigma, **kw)
        
    @staticmethod
    def error(p, func, x, y):
        # Note that we do not unpack p using *p. This is important, the func objects depend on it.
        return y - func(x, p)
    
    def get_func_i(self):
        """ Points as predicted by the model for the points self.x """
        if self.func_i is not None:
            return self.func_i
        else:
            self.func_i = self.func(self.x, self.popt)
            return self.func_i
        
    def get_residues(self):
        if self.residues:
            return self.residues
        else:
            self.residues = self.get_func_i() - self.y
            return self.residues
    
    def get_popt_tuple(self):
        """ Returns the popt with their names """
#         return dict(zip(self.Func.get_param_names(), self.popt))
        print self.Func.get_param_names()
        print self.popt
        dic = zip(self.Func.get_param_names(), self.popt)
        print dic
        return dic
        
    def get_r_squared(self):
        if self.r_squared:
            return self.r_squared
        else:
            self.r_squared = 1 - np.sum(self.get_residues()**2)/np.sum((self.get_func_i() - np.mean(self.y))**2)
            return self.r_squared
        
#     def __str__(self):
#         return ''.join(['{}, '.format(opt) for opt in self.popt])
    
    def __unicode__(self):
        # return u'{}{}'.format(self.Func, self.get_popt_tuple())
        return u'{}'.format(self.Func)