import matplotlib
matplotlib.use('Qt4Agg') # Qt since enaml uses it too
import matplotlib.pyplot as plt
import numpy as np
# plt.xkcd() # XKCD easteregg
from matplotlib import gridspec
from matplotlib.ticker import MaxNLocator, ScalarFormatter
from matplotlib.widgets import  SpanSelector, RectangleSelector
from traits.api import HasTraits, Instance, ReadOnly, Event, Enum, on_trait_change, undefined
from models import EvsS, EvsSKDE, BVA, Dataset
from functions import GuessesToFit, GuessesToFit1D

# plt.rc('text', usetex=True)

class MPLBaseView(HasTraits):
    """ Base class for all matplotlib based views.
    All such objects need to have a figure instance and a gridspec which forms a grid of subplots.

    Furthermore, an event 'updated' is provided which should be fired after updating the plot with the update() method.
    Any subclass should therefore overwrite the default update() method by it's own and call this base 
    classes's update at the end.
    """
    fig = Instance(matplotlib.figure.Figure)
    gs = Instance(gridspec.GridSpec)
    updated = Event

    def __init__(self, *args, **kwargs):
        # Change the font
        matplotlib.rc('font', **self.get_font())
        super(MPLBaseView, self).__init__(*args, **kwargs)

    def get_font(self):
        return {'family' : 'Arial',
                #'weight' : 'bold',
                'size'   : 13,
                }
    def update(self):
        self.updated = True

class DatasetView(MPLBaseView):
    """ View a Dataset. Any two properties of the dataset can be plotted against each other in a simple x vs y style.
    (This might be extended to x vs y vs z in the future.)
    Fitting to any function desired will also be supported. (As long as they exist as a AbstractFunction subclass in functions.py)
    """
    dataset = Instance(Dataset)
    fitting = Instance(GuessesToFit1D)

    def __init__(self, *args, **kwargs):
        super(DatasetView, self).__init__(*args, **kwargs)
        # Initiate a plot in a MPLCanvas compatible way
        self.fig = plt.figure(figsize = (8,8))
        self.gs = gridspec.GridSpec(1, 1,
                       hspace=0.,
                       wspace=0.,
                       )
        self.update()

    @on_trait_change('dataset:updated')
    def update(self):
        """ Update the plot to reflect changes to the DataContainer object """
        print 'dataset:updated'
        if self.fig:
            self.fig.clear()
            self.plot()           
            self.fig.canvas.draw()
            print 'self.fig'
        else:
            self.plot()
        super(DatasetView, self).update()

    def plot(self):
        self.plot_x_vs_y()
        self.plot_fitting()
        self.format_plot()

    def plot_x_vs_y(self):
        """ Plot DD + DA vs Count(DD + DA) """
        self.ax = self.fig.add_subplot(self.gs[0])
        self.ax.scatter(self.dataset.xaxis, self.dataset.yaxis, s = 10, c = 'green')

    def plot_fitting(self):
        # try:
        if self.fitting.fit_instance != None:
            x_max = np.max(self.dataset.xaxis)
            x_min = np.min(self.dataset.xaxis)
            x = np.linspace(x_min, x_max)
            y = self.fitting.fit_instance.func(x, self.fitting.fit_instance.popt) 
            self.ax.plot(x, y, color='black', linewidth=2)
        # except AttributeError:
            # pass

    def format_plot(self):
        self.ax.set_xlabel(self.dataset.axis_manager.xlabel)
        self.ax.set_ylabel(self.dataset.axis_manager.ylabel)



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
        super(BVAView, self).update()

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
    selection_mode = Enum('guesses', 'guesses', 's_range') 
    
    def __init__(self, *args, **kwargs):
        """ Link this view to an EvsS object so we can view it."""
#         self.EvsS = EvsS
        print 'init'
        super(EvsSView, self).__init__(*args, **kwargs)
        # Initiate a plot in a MPLCanvas compatible way
        self.fig = plt.figure(figsize = (8,8))
        self.gs = gridspec.GridSpec(2, 3,
                       width_ratios=[9,3,1],
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
            self.fig.clear()
            self.plot()
            if self.fitting.show_E == True:
                print 'plotting E'
                self.plot_fitting_E()
            if self.fitting.show_S == True:
                self.plot_fitting_S()
            if self.fitting.show_guesses == True:
                self.plot_guesses()
            if self.fitting.show_S_range == True:
                self.plot_S_range()
            
            self.fig.canvas.draw()
            print 'updated'
        else:
            self.plot()
        super(EvsSView, self).update()

    
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
        self.ax = self.fig.add_subplot(self.gs[3],)
        extent = [self.evss.Eedges[0], self.evss.Eedges[-1], self.evss.Sedges[0], self.evss.Sedges[-1]]
        self.mappable = self.ax.imshow(self.evss.H_EvsS, extent=extent, interpolation='spline36', cmap = 'thorben', origin='lower') #Favorites: sinc, catrom, spline36
        
    def plot_E(self):
        self.ax_E = self.fig.add_subplot(self.gs[0], sharex=self.ax)
        # self.ax_E = self.fig.add_subplot(self.gs[0], adjustable='box')
        self.rects_E = self.ax_E.bar(self.evss.Eedges[1:], self.evss.H_Epr, width = self.evss.Eedges[0] - self.evss.Eedges[1], color='grey')
        
    
    def plot_S(self):
        self.ax_S = self.fig.add_subplot(self.gs[4], sharey=self.ax,)
        # self.ax_S = self.fig.add_subplot(self.gs[4], adjustable='box')
        self.rects_S = self.ax_S.barh(self.evss.Sedges[1:], self.evss.H_Sraw, height = self.evss.Sedges[0] - self.evss.Sedges[1], color='grey')
    
    def add_colorbar(self):
        self.ax_color = self.fig.add_subplot(self.gs[5])
        self.ax_color.axis('off')
        # clrbar = self.fig.colorbar(self.mappable, ax=self.ax_color, use_gridspec=True, pad=.3, aspect = 30, fraction = .7,)
        clrbar = self.fig.colorbar(self.mappable, ax=self.ax_color, use_gridspec=True, pad = 0.3, aspect = 30, fraction = 0.5, )
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

    def plot_S_range(self):
        """ Show the S range depicted in the E histogram by drawing lines across the EvsS histogram. """
        x = np.linspace(0,1, 1000)
        S_min = np.ones((1000,))*self.evss.thresholds.S_min
        S_max = np.ones((1000,))*self.evss.thresholds.S_max
        self.ax.plot(x, S_min, color='black', linestyle = 'dashed', linewidth = 2)
        self.ax.plot(x, S_max, color='black', linestyle = 'dashed', linewidth = 2)
            
    def show(self):
        """ Show the thing! """
        plt.show()
    
    def format(self):
        """ Do all formatting of the graph here. This is the function to 
        overload when other style's are desired. 
        """
        # Set axis to use scientific notation
        formatter = ScalarFormatter(useMathText=True)
        formatter.set_scientific(True) 
        formatter.set_powerlimits((-2,2))

        self.ax_E.yaxis.set_major_formatter(formatter) 
        self.ax_S.xaxis.set_major_formatter(formatter) 

        # Hide the default scientific notation lable, because their placement sucks. We want to place it in the axis label.
        self.ax_E.yaxis.get_offset_text().set_visible(False)
        self.ax_S.xaxis.get_offset_text().set_visible(False)
        # self.ax_E.ticklabel_format(style='sci', scilimits=(-2,2))
        # self.ax_S.ticklabel_format(style='sci', scilimits=(-2,2))
        # self.ax_E.yaxis.set_major_formatter(ScalarFormatter())

        # Remove ticks for highest values
        self.ax.yaxis.set_major_locator(MaxNLocator(prune='upper'))
        self.ax.xaxis.set_major_locator(MaxNLocator(prune='upper'))
        # Set lables. Check if the alpha and beta or different from default. 
        # If so, change the label text.
        if self.evss.corr.gamma != 1.0 or self.evss.corr.beta != 1.0:
            self.ax.set_xlabel('Accurate FRET $E$', horizontalalignment='center', verticalalignment='bottom', ) # fontweight='bold', fontsize=16
            self.ax.set_ylabel('Stoichiometry $S$', horizontalalignment='center', verticalalignment='bottom', )
        else:
            self.ax.set_xlabel('Apparent FRET $E^*$', horizontalalignment='center', verticalalignment='bottom', ) # fontweight='bold', fontsize=16
            self.ax.set_ylabel('Stoichiometry $S^*$', horizontalalignment='center', verticalalignment='bottom', )
        self.ax.set_aspect('auto')

        # Calculate the order of magnitude in the E and S direction
        # magnitude_E = self.ax_E.yaxis.get_offset_text().get_text()
        # magnitude_S = self.ax_S.xaxis.get_offset_text().get_text()
        magnitude_E = int(np.log10(self.ax_E.yaxis.get_data_interval()[1]))
        magnitude_S = int(np.log10(self.ax_S.xaxis.get_data_interval()[1]))
        # Put the magnitude in the label and remove it from the plot.
        self.ax_E.set_ylabel('Events $/10^{0}$'.format(magnitude_E), horizontalalignment='center', verticalalignment='bottom',)
        self.ax_S.set_xlabel('Events $/10^{0}$'.format(magnitude_S), horizontalalignment='center', verticalalignment='bottom',)
        # self.ax_E.set_ylabel('Events {}'.format(magnitude_E), fontweight='bold', )
        # self.ax_S.set_xlabel('Events {}'.format(magnitude_S), fontweight='bold', )
        

        # Set label positions manually so they are properly alligned.
        self.ax_E.yaxis.set_label_coords(-.1, 0.5)
        self.ax.yaxis.set_label_coords(-0.1, 0.5)
        self.ax_S.xaxis.set_label_coords(0.5, -.1)
        self.ax.xaxis.set_label_coords(0.5, -0.1)
          
        # Reduce the number of ticks on the 2D histogram
        self.ax_E.locator_params(nbins=5, axis = 'x')
        self.ax_S.locator_params(nbins=5, axis = 'y')
        self.gs.update()
          
        # Remove ticks on E histogram
        plt.setp( self.ax_E.get_xticklabels(), visible=False)
        # Show only 3 ticks corrosponding to y values
        self.ax_E.locator_params(nbins=2, axis = 'y')
        # Similar for S
        plt.setp( self.ax_S.get_yticklabels(), visible=False)
        self.ax_S.locator_params(nbins=2, axis = 'x')
        
    def onselect_rect(self, eclick, erelease):
        """ The coordinates where the user indicates a Guess should be placed.

        The coordinates are set in a tuple and sent as an event. 
        Format of the tuple:
        ((eclick.xdata, eclick.ydata), (erelease.xdata, erelease.ydata))

        This rectangle event is caught elsewere and used to add a Guess object.
        """
        print ' startposition : (%f, %f)' % (eclick.xdata, eclick.ydata)
        print ' endposition   : (%f, %f)' % (erelease.xdata, erelease.ydata)
        print ' used button   : ', eclick.button
        self.rectangle = ((eclick.xdata, eclick.ydata), 
                        (erelease.xdata, erelease.ydata))
        
        # Old method to select on area for the E histogram using spanselector. 
    def onselect_span(self, Smin, Smax):
        """ Behavior for the SpanSelector. As the user selects an S range on the screen, 
        we can use this to update the S thresholds. This is really sexy.
        """
        # Change the plot options and update the graph
        self.evss.thresholds.S_min = Smin
        self.evss.thresholds.S_max = Smax
        self.evss.update()

    @on_trait_change('selection_mode')
    def change_select_widget(self):
        if self.selection_mode == 'guesses':
            self.selector = RectangleSelector(self.ax, self.onselect_rect, drawtype='box')
        elif self.selection_mode == 's_range':
            self.selector = SpanSelector(self.ax, self.onselect_span, 'vertical', useblit=True, 
                                           rectprops=dict(alpha=0.5, facecolor='white'))

    @on_trait_change('evss.thresholds.[S_max, S_min]')
    def set_show_S_range(self):
        """ If the S_range is changed to anything other than 0.0 - 1.0, show it on the screen. 
        Do not show it if the range is the default range, because it doesn't look nice.
        """
        self.fitting.show_S_range = not (self.evss.thresholds.S_max == 1.0 and self.evss.thresholds.S_min == 0.0)
            
class EvsSKDEView(EvsSView):
    """ View for the KDE based version of the EvsS 'histogram'."""
    evss = Instance(EvsSKDE)

    def plot_E(self):
        print "Plotting the KDE!!!"
        self.ax_E = self.fig.add_subplot(self.gs[0], sharex=self.ax)
        # self.ax_E = self.fig.add_subplot(self.gs[0], adjustable='box')
        self.rects_E = self.ax_E.fill_between(self.evss.Ecentres, self.evss.H_Epr, 0, color='grey')
        
    
    def plot_S(self):
        self.ax_S = self.fig.add_subplot(self.gs[4], sharey=self.ax,)
        # self.ax_S = self.fig.add_subplot(self.gs[4], adjustable='box')
        self.rects_S = self.ax_S.fill_betweenx(self.evss.Scentres, self.evss.H_Sraw, 0, color='grey')



# Custom color map
colors = ['#ffffff', '#800080', '#13007c', '#2bc0d7', '#7fc31c', '#f5f122', '#ff8000', '#ff0000']
# colors = [(0.0, '#ffffff'), (0.001, '#800080'), (0.003, '#13007c'), (0.01, '#2bc0d7'), (0.03, '#7fc31c'), (0.1, '#f5f122'), (0.3, '#ff8000'), (1.0, '#ff0000')]
thorben_cmap = matplotlib.colors.LinearSegmentedColormap.from_list('thorben', colors)
# thorben_cmap, norm = matplotlib.colors.from_levels_and_colors([5,800,1600,2500,2600,2600,2600,2600,2600], colors)
# thorben_cmap.name = 'thorben'
matplotlib.cm.register_cmap(cmap=thorben_cmap)