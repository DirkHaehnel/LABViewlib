import matplotlib
matplotlib.use('Qt4Agg') # Qt since enaml uses it too
import matplotlib.pyplot as plt
import numpy as np
# plt.xkcd() # XKCD easteregg
from matplotlib import gridspec
from matplotlib.ticker import MaxNLocator
from matplotlib.widgets import  SpanSelector, RectangleSelector
from traits.api import HasTraits, Instance, ReadOnly, Event, Enum, on_trait_change, undefined
from models import EvsS, BVA
from functions import GuessesToFit

class MPLBaseView(HasTraits):
    """ Base class for all matplotlib based views. """
    fig = Instance(matplotlib.figure.Figure)
    gs = Instance(gridspec.GridSpec)
    updated = Event

    def update(self):
        raise NotImplementedError('')

class BVAView(MPLBaseView):
    """ View for the BVA object. Makes a beatiful plot of the BVA data."""
    bva = Instance(BVA)

    def __init__(self, *args, **kwargs):
        super(BVAView, self).__init__(*args, **kwargs)
        # Initiate a plot in a MPLCanvas compatible way
        self.fig = plt.figure(figsize = (8,8))
        self.gs = gridspec.GridSpec(2, 1,
                       height_ratios=[1,3],
                       hspace=0.,
                       wspace=0.,
                       )
        self.update()

    @on_trait_change('bva:updated')
    def update(self):
        """ Update the plot to reflect changes to the BVA object """
        if self.fig:
            self.fig.clear()
            self.plot()           
            self.fig.canvas.draw()
            # print 'updated'
        else:
            self.plot()
        self.updated = True

    def plot(self):
        """ Plot's all relevant items """
        self.ax_E = self.fig.add_subplot(self.gs[0])
        self.ax_stdev = self.fig.add_subplot(self.gs[1], sharex=self.ax_E)
        self.plot_E(self.ax_E)
        #stdev from theory
        self.plot_stdev(self.ax_stdev)
        #stdev from bva
        self.plot_bva(self.ax_stdev)
        #average values from bva
        self.plot_mean_bva(self.ax_stdev)
        self.format_plot()

    def plot_E(self, ax):
        self.rects_E = ax.bar(self.bva.Eedges[1:], self.bva.H_Epr, width = self.bva.Eedges[0] - self.bva.Eedges[1], color='grey')

    def plot_stdev(self, ax):
        E = np.linspace(0,1.0, 1000)
        sig = np.sqrt(E*(1-E)/self.bva.window_size)
        ax.plot(E, sig, color = 'black', lw = 1.5)
        # ax.scatter(self.bva.data['Epr'], self.bva.data['sigEpr'], color = '#ff8000')

    def plot_bva(self, ax):
        ax.hist2d(self.bva.data['Epr'], self.bva.data['bvaSigEpr'], bins = [200,200], range = [(0.0, 1.0), (0.0, 0.5)], cmap = 'thorben')
        # ax.scatter(self.bva.data['Epr'], self.bva.data['bvaSigEpr'], color = 'pink', alpha = 0.5, s = 5)

    def plot_mean_bva(self, ax):
        # ax.scatter(self.bva.data['Epr'], self.bva.data['bvaSigEpr'], color = 'pink', alpha = 0.5, size = 5)
        ax.scatter(self.bva.bin_centres, self.bva.bin_avg, color = '#ff8000', marker = '^')

    def format_plot(self):
        self.ax_E.set_ylabel('#Events')
        self.ax_stdev.set_ylabel('$\sigma_{E}$')
        self.ax_stdev.set_xlabel('$E_{pr}$')
        self.ax_stdev.set_interpolation='sinc'

class EvsSView(MPLBaseView):
    # Read only access
#     EvsS = Instance(EvsS)
    evss = Instance(EvsS)
    fitting = Instance(GuessesToFit)
    rectangle = Event
    selection_mode = Enum('guesses', 's_range') 
    
    def __init__(self, *args, **kwargs):
        """ Link this view to an EvsS object so we can view it."""
#         self.EvsS = EvsS
        print 'init'
        super(EvsSView, self).__init__(*args, **kwargs)
        # Initiate a plot in a MPLCanvas compatible way
        # self.fig, self.axes = plt.subplots(2, 2,  sharex=True, sharey=True)# sharex='col', sharey='row'
        # ((self.ax_E, self.ax_stats), (self.ax, self.ax_S)) = self.axes
        # plt.setp(self.axes.flat, aspect=1.0, adjustable='box-forced')
        self.fig = plt.figure(figsize = (8,8))
        self.gs = gridspec.GridSpec(2, 2,
                       width_ratios=[3,1],
                       height_ratios=[1,3],
                       hspace=0.,
                       wspace=0.,
                       )
        self.update()

    @on_trait_change('evss:updated')
    def update(self):
        """ Update the plot to reflect changes to the EvsS object """
        print 'updating'
        if self.fig:
            # self.fig.clear()
            self.plot()
            if self.fitting.show_E == True:
                print 'plotting E'
                self.plot_fitting_E()
            if self.fitting.show_S == True:
                self.plot_fitting_S()
            if self.fitting.show_guesses == True:
                self.plot_guesses()
            
            self.fig.canvas.draw()
            print 'updated'
        else:
            self.plot()

    
    def plot(self):
        """ Plot's all relevant items """
        self.plot_EvsS()
        self.plot_E()
        self.plot_S()
        # Set the select widget to default
        self.change_select_widget()
        self.add_colorbar()
        self.format()
        print 'done plotting'
    
    def plot_EvsS(self):
        self.ax = self.fig.add_subplot(self.gs[2])
        extent = [self.evss.Eedges[0], self.evss.Eedges[-1], self.evss.Sedges[0], self.evss.Sedges[-1]]
        self.mappable = self.ax.imshow(self.evss.H_EvsS, extent=extent, interpolation='sinc', cmap = 'thorben', origin='lower') #Favorites: sinc, catrom, spline36
        
    def plot_E(self):
        self.ax_E = self.fig.add_subplot(self.gs[0], sharex=self.ax)
        # self.ax_E.set_title('yo')
        self.rects_E = self.ax_E.bar(self.evss.Eedges[1:], self.evss.H_Epr, width = self.evss.Eedges[0] - self.evss.Eedges[1], color='grey')
        
    
    def plot_S(self):
        self.ax_S = self.fig.add_subplot(self.gs[3], sharey=self.ax)
        self.rects_S = self.ax_S.barh(self.evss.Sedges[1:], self.evss.H_Sraw, height = self.evss.Sedges[0] - self.evss.Sedges[1], color='grey')
    
    def add_colorbar(self):
        # self.ax_stats = self.fig.add_subplot(self.gs[1])
        # clrbar = self.fig.colorbar(self.mappable, ax=self.ax_stats, pad=.2)
        pass

    def plot_fitting(self):
        """ Plot the result of fitting over the histograms."""
        self.plot_fitting_E()
        self.plot_fitting_S()
        
    def plot_fitting_E(self):
        try:
            y = self.fitting.fit_E.func(self.evss.Ecentres, self.fitting.fit_E.popt) 
            self.ax_E.plot(self.evss.Ecentres, y, color='black', linewidth=2)
        except AttributeError:
            pass
        
    def plot_fitting_S(self):
        try:
            y = self.fitting.fit_S.func(self.evss.Scentres, self.fitting.fit_S.popt)
            self.ax_S.plot(y, self.evss.Scentres, color='black', linewidth=2)
        except AttributeError:
            pass
    
    def plot_guesses(self):
        for guess in self.fitting.guesses:
            if guess.type_E:
                E = guess.type_E.func(self.evss.Ecentres, guess.type_E.get_params())
                self.ax_E.plot(self.evss.Ecentres, E, color='black', linestyle='dashed', linewidth=2)
            if guess.type_E:
                S = guess.type_S.func(self.evss.Scentres, guess.type_S.get_params())
                self.ax_S.plot(S, self.evss.Scentres, color='black', linestyle='dashed', linewidth=2)
            
    def show(self):
        """ Show the thing! """
        plt.show()
        
    def get_font(self):
        return {'family' : 'Arial',
                'weight' : 'bold',
                }
    
    def format(self):
        """ Do all formatting of the graph here. This is the function to 
        overload when other style's are desired. 
        """
        # Change the font
        matplotlib.rc('font', **self.get_font())
        # Remove ticks for highest values
        self.ax.yaxis.set_major_locator(MaxNLocator(prune='upper'))
        self.ax.xaxis.set_major_locator(MaxNLocator(prune='upper'))
        # Set lables
        self.ax.set_xlabel('Apparent FRET (E*)', fontweight='bold')
        self.ax.set_ylabel('Stoichiometry (S)', fontweight='bold')
        # self.ax.set_aspect(1.0)
        x0,x1 = self.ax.get_xlim()
        y0,y1 = self.ax.get_ylim()
        self.ax.set_aspect((x1-x0)/(y1-y0))
        self.ax_E.set_ylabel('Events', fontweight='bold')
        self.ax_S.set_xlabel('Events', fontweight='bold')
          
        # Reduce the number of ticks on the 2D histogram
        self.ax_E.locator_params(nbins=5, axis = 'x')
        self.ax_S.locator_params(nbins=5, axis = 'y')
        # self.gs.update()
          
        # Remove ticks on E histogram
        plt.setp( self.ax_E.get_xticklabels(), visible=False)
        # Show only 3 ticks corrosponding to y values
        self.ax_E.locator_params(nbins=2, axis = 'y')
        # Similar for S
        plt.setp( self.ax_S.get_yticklabels(), visible=False)
        self.ax_S.locator_params(nbins=2, axis = 'x')
        
    def onselect_rect(self, eclick, erelease):
        """ The coordinates are set in a tuple and sent as an event. 
        Format of the tuple:
        ((eclick.xdata, eclick.ydata), (erelease.xdata, erelease.ydata))
        """
        print ' startposition : (%f, %f)' % (eclick.xdata, eclick.ydata)
        print ' endposition   : (%f, %f)' % (erelease.xdata, erelease.ydata)
        print ' used button   : ', eclick.button
        self.rectangle = ((eclick.xdata, eclick.ydata), 
                        (erelease.xdata, erelease.ydata))
        
        # Old method to select on area for the E histogram using spanselector. 
    def onselect_span(self, Smin, Smax):
        """ Behavior for the SpanSelector """
        # Change the plot options and update the graph
        self.evss.options.S_min = Smin
        self.evss.options.S_max = Smax
        # self.EvsS.update()
        # print self, Smin, Smax
        # # Redraw
        # for rect, h in zip(self.rects_E, self.EvsS.H_Epr):
        #     rect.set_height(h)
        # self.ax_E.set_xlim(self.EvsS.Eedges[0], self.EvsS.Eedges[-1])
        # self.ax_E.set_ylim(self.EvsS.H_Epr.min(), self.EvsS.H_Epr.max())
        # self.fig.canvas.draw()
        
    @on_trait_change('selection_mode')
    def change_select_widget(self):
#         if isinstance(new, undefined.__class__):
#             print 'undefined'
#             self.selector = RectangleSelector(self.ax, self.onselect_rect, drawtype='box')
#         else:
        print self.selection_mode
        if self.selection_mode == 'guesses':
            self.selector = RectangleSelector(self.ax, self.onselect_rect, drawtype='box')
        elif self.selection_mode == 's_range':
            self.selector = SpanSelector(self.ax, self.onselect_span, 'vertical', useblit=True, 
                                           rectprops=dict(alpha=0.5, facecolor='white'))
            

# Custom color map
colors = ['#ffffff', '#800080', '#13007c', '#2bc0d7', '#7fc31c', '#f5f122', '#ff8000', '#ff0000']
# colors = [(0.0, '#ffffff'), (0.001, '#800080'), (0.003, '#13007c'), (0.01, '#2bc0d7'), (0.03, '#7fc31c'), (0.1, '#f5f122'), (0.3, '#ff8000'), (1.0, '#ff0000')]
thorben_cmap = matplotlib.colors.LinearSegmentedColormap.from_list('thorben', colors)
matplotlib.cm.register_cmap(cmap=thorben_cmap)