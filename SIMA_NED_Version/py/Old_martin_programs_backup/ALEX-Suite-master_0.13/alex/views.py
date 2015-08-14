import matplotlib

matplotlib.use('Qt4Agg')  # Qt since enaml uses it too
import matplotlib.pyplot as plt
import numpy as np
# plt.xkcd() # XKCD easteregg
from matplotlib import gridspec
from matplotlib.ticker import MaxNLocator, ScalarFormatter
from matplotlib.widgets import SpanSelector, RectangleSelector
from traits.api import HasTraits, Instance, ReadOnly, Event, Enum, on_trait_change, undefined
from models import EvsS, BVA, Dataset, PlotFormatOptions, EvsSLayers, DataContainer
from functions import GuessesToFit, GuessesToFit1D
import matplotlib.font_manager
from matplotlib.ticker import MultipleLocator, FormatStrFormatter
from mpl_toolkits.axes_grid1 import make_axes_locatable # Used to make the EvsSPlot
from mpl_toolkits.axes_grid.axes_size import Scaled

# plt.rc('text', usetex=True)

class MPLBasePlot(HasTraits):
    """ Base class for all matplotlib based views.
    All such objects need to have a figure instance and a gridspec which forms a grid of subplots.

    Furthermore, an event 'updated' is provided which should be fired after updating the plot with the update() method.
    Any subclass should therefore overwrite the default update() method by it's own and call this base 
    classes's update at the end.
    """
    data_container = Instance(DataContainer)
    fig = Instance(matplotlib.figure.Figure)
    gs = Instance(gridspec.GridSpec)
    # Plot formatiing options such as font and figure size.
    format_options = Instance(PlotFormatOptions)
    updated = Event

    def __init__(self, *args, **kwargs):
        super(MPLBasePlot, self).__init__(*args, **kwargs)
        # if not self.format_options:
        #     self.format_options = PlotFormatOptions()
        # Change the font
        matplotlib.rc('font', **self.get_font())
        # Initiate a plot in a MPLCanvas compatible way
        self.fig = plt.figure(figsize=(self.format_options.width, self.format_options.height),
                              dpi=self.format_options.dpi)
        # For only god knows what reason, the next line is needed to get this shit to work...
        self.format_options.width, self.format_options.height = self.fig.get_figwidth(), self.fig.get_figheight()
        self.fig.set_tight_layout(True)
        self.fig.patch.set_alpha(0.0)

    def get_font(self):
        return {'family': self.format_options.family,
                'weight': self.format_options.weight,
                'size': self.format_options.size,
                'style': self.format_options.style,
                'variant': self.format_options.variant,
                }

    def update(self):
        print 'called'
        self.updated = True

    @on_trait_change('format_options:updated')
    def update_formatting(self):
        """ Upon changing formatting options, trigger an update. """
        print 'format_options updated'
        matplotlib.rc('font', **self.get_font())
        self.fig.set_figwidth(self.format_options.width)
        self.fig.set_figheight(self.format_options.height)
        self.fig.set_dpi(self.format_options.dpi)
        self.fig.patch.set_alpha(0.0)
        self.update()


class DatasetPlot(MPLBasePlot):
    """ View a Dataset. Any two properties of the dataset can be plotted against each other in a simple x vs y style.
    (This might be extended to x vs y vs z in the future.)
    Fitting to any function desired will also be supported. (As long as they exist as a AbstractFunction subclass in
    functions.py)
    """
    data_container = Instance(Dataset)
    fitting = Instance(GuessesToFit1D)

    def __init__(self, *args, **kwargs):
        super(DatasetPlot, self).__init__(*args, **kwargs)
        self.gs = gridspec.GridSpec(1, 1,
                                    hspace=0.,
                                    wspace=0.,
                                    )
        self.update()

    @on_trait_change('data_container:updated')
    def update(self):
        """ Update the plot to reflect changes to the DataContainer object """
        print 'data_container:updated'
        if self.fig:
            self.fig.clear()
            self.plot()
            self.fig.canvas.draw()
            print 'self.fig'
        else:
            self.plot()
        super(DatasetPlot, self).update()

    def plot(self):
        self.plot_x_vs_y()
        self.plot_fitting()
        self.format_plot()

    def plot_x_vs_y(self):
        """ Plot DD + DA vs Count(DD + DA) """
        self.ax = self.fig.add_subplot(self.gs[0])
        self.ax.scatter(self.data_container.xaxis, self.data_container.yaxis, s=10, c='green')

    def plot_fitting(self):
        # try:
        if self.fitting.fit_instance is not None:
            x_max = np.max(self.data_container.xaxis)
            x_min = np.min(self.data_container.xaxis)
            x = np.linspace(x_min, x_max)
            y = self.fitting.fit_instance.func(x, self.fitting.fit_instance.popt)
            self.ax.plot(x, y, color='black', linewidth=2)
            # except AttributeError:
            # pass

    def format_plot(self):
        self.ax.set_xlabel(self.data_container.axis_manager.xlabel)
        self.ax.set_ylabel(self.data_container.axis_manager.ylabel)


class BVAPlot(MPLBasePlot):
    """ View for the BVA object. Makes a beatiful plot of the BVA data."""
    data_container = Instance(BVA)

    def __init__(self, *args, **kwargs):
        super(BVAPlot, self).__init__(*args, **kwargs)
        self.gs = gridspec.GridSpec(2, 1,
                                    height_ratios=[1, 3],
                                    hspace=0.,
                                    wspace=0.,
                                    )
        print 'fig updated'
        self.update()

    @on_trait_change('data_container:updated')
    def update(self):
        """ Update the plot to reflect changes to the BVA object """
        if self.fig:
            self.fig.clear()
            self.plot()
            self.fig.canvas.draw()
            # print 'updated'
        else:
            self.plot()
        super(BVAPlot, self).update()

    def plot(self):
        """ Plot's all relevant items """
        self.ax_E = self.fig.add_subplot(self.gs[0])
        self.ax_stdev = self.fig.add_subplot(self.gs[1], sharex=self.ax_E)
        self.plot_E(self.ax_E)
        # stdev from theory
        self.plot_stdev(self.ax_stdev)
        # stdev from bva
        self.plot_bva(self.ax_stdev)
        # average values from bva
        self.plot_mean_bva(self.ax_stdev)
        self.format_plot()

    def plot_E(self, ax):
        self.rects_E = ax.bar(self.data_container.Eedges[1:], self.data_container.H_Epr, width=self.data_container.Eedges[0] - self.data_container.Eedges[1],
                              color='grey')

    def plot_stdev(self, ax):
        E = np.linspace(0, 1.0, 1000)
        sig = np.sqrt(E * (1 - E) / self.data_container.window_size)
        ax.plot(E, sig, color='black', lw=1.5)
        # ax.scatter(self.data_container.data['Epr'], self.data_container.data['sigEpr'], color = '#ff8000')

    def plot_bva(self, ax):
        ax.hist2d(self.data_container.data['Epr'], self.data_container.data['bvaSigEpr'], bins=[200, 200], range=[(0.0, 1.0), (0.0, 0.5)],
                  cmap='thorben')
        # ax.scatter(self.data_container.data['Epr'], self.data_container.data['bvaSigEpr'], color = 'pink', alpha = 0.5, s = 5)

    def plot_mean_bva(self, ax):
        # ax.scatter(self.data_container.data['Epr'], self.data_container.data['bvaSigEpr'], color = 'pink', alpha = 0.5, size = 5)
        ax.scatter(self.data_container.bin_centres, self.data_container.bin_avg, color='#ff8000', marker='^')

    def format_plot(self):
        self.ax_E.set_ylabel('#Events')
        self.ax_stdev.set_ylabel('$\sigma_{E}$')
        self.ax_stdev.set_xlabel('$E_{pr}$')
        self.ax_stdev.set_interpolation = 'sinc'


class EvsSPlot(MPLBasePlot):
    # Read only access
    # EvsS = Instance(EvsS)
    data_container = Instance(EvsS)
    fitting = Instance(GuessesToFit)
    rectangle = Event
    selection_mode = Enum('guesses', 'guesses', 's_range')
    evss_layers = Instance(EvsSLayers)

    def __init__(self, *args, **kwargs):
        """ Link this view to an EvsS object so we can view it."""
        super(EvsSPlot, self).__init__(*args, **kwargs)
        self.update()

    @on_trait_change('data_container:updated')
    def update(self):
        """ Update the plot to reflect changes to the EvsS object """
        if self.fig:
            self.fig.clear()
            self.plot()
            if self.evss_layers.show_E:
                print 'plotting E'
                self.plot_fitting_E()
            if self.evss_layers.show_S:
                self.plot_fitting_S()
            if self.evss_layers.show_guesses:
                self.plot_guesses()
            if self.evss_layers.show_S_range:
                self.plot_S_range()
            if self.evss_layers.show_E_range:
                self.plot_E_range()

            self.fig.canvas.draw()
            print 'updated'
        else:
            self.plot()
        super(EvsSPlot, self).update()

    def plot(self):
        """ Plot's all relevant items """
        self.plot_EvsS()
        self.plot_E()
        self.plot_S()

        # Set the ratio for the subplots.
        ratios = [Scaled(3), Scaled(1)]
        self.divider.set_horizontal(ratios)
        self.divider.set_vertical(ratios)

        # Set the select widget to default
        self.change_select_widget()
        self.add_colorbar()
        self.format()

        # Set aspect ratio. This ensures a square EvsS histogram.
        x0,x1 = self.ax.get_xlim()
        y0,y1 = self.ax.get_ylim()
        self.ax.set_aspect((x1-x0)/(y1-y0))
        print 'done plotting'

    def plot_EvsS(self):
        self.ax = self.fig.add_subplot(111)
        extent = [self.data_container.plot_options.E_min, self.data_container.plot_options.E_max, self.data_container.plot_options.S_min, self.data_container.plot_options.S_max, ]
        self.mappable = self.ax.imshow(
            self.data_container.H_EvsS, 
            extent=extent, 
            interpolation='spline36', # Favorites: sinc, catrom, spline36
            cmap='thorben',
            origin='lower',
            norm=matplotlib.colors.LogNorm() if self.data_container.plot_options.log_scale else None
        )
        self.divider = make_axes_locatable(self.ax)

    def plot_E(self):
        size = self.fig.get_size_inches()[0]/4.0
        self.ax_E = self.divider.append_axes("top", size=size, sharex=self.ax,)
        self.rects_E = self.ax_E.bar(self.data_container.Eedges[1:], self.data_container.H_Epr,
                                     width=self.data_container.Eedges[0] - self.data_container.Eedges[1], color='grey')

    def plot_S(self):
        size = self.fig.get_size_inches()[0]/4.0
        self.ax_S = self.divider.append_axes("right", size=size, sharey=self.ax,)
        self.rects_S = self.ax_S.barh(self.data_container.Sedges[1:], self.data_container.H_Sraw,
                                      height=self.data_container.Sedges[0] - self.data_container.Sedges[1], color='grey')

    def add_colorbar(self):
        cax = self.divider.append_axes("right", size='10%', pad=0.3)
        clrbar = self.fig.colorbar(self.mappable, cax=cax)

    def plot_fitting(self):
        """ Plot the result of fitting over the histograms."""
        self.plot_fitting_E()
        self.plot_fitting_S()

    def plot_fitting_E(self):
        try:
            y = self.fitting.fit_E.func(self.data_container.Ecentres, self.fitting.fit_E.popt)
            self.ax_E.plot(self.data_container.Ecentres, y, color='black', linewidth=2)
        except AttributeError:
            pass

    def plot_fitting_S(self):
        try:
            y = self.fitting.fit_S.func(self.data_container.Scentres, self.fitting.fit_S.popt)
            self.ax_S.plot(y, self.data_container.Scentres, color='black', linewidth=2)
        except AttributeError:
            pass

    def plot_guesses(self):
        for guess in self.fitting.guesses:
            if guess.type_E:
                E = guess.type_E.func(self.data_container.Ecentres, guess.type_E.get_params())
                self.ax_E.plot(self.data_container.Ecentres, E, color='black', linestyle='dashed', linewidth=2)
            if guess.type_E:
                S = guess.type_S.func(self.data_container.Scentres, guess.type_S.get_params())
                self.ax_S.plot(S, self.data_container.Scentres, color='black', linestyle='dashed', linewidth=2)

    def plot_S_range(self):
        """ Show the S range depicted in the E histogram by drawing lines across the EvsS histogram. """
        x = np.linspace(self.data_container.plot_options.S_min, self.data_container.plot_options.S_max, 1000)
        S_min = np.ones((1000,)) * self.data_container.thresholds.S_min_1D
        S_max = np.ones((1000,)) * self.data_container.thresholds.S_max_1D
        self.ax.plot(x, S_min, color='black', linestyle='dashed', linewidth=2)
        self.ax.plot(x, S_max, color='black', linestyle='dashed', linewidth=2)

    def plot_E_range(self):
        """ Show the S range depicted in the E histogram by drawing lines across the EvsS histogram. """
        y = np.linspace(self.data_container.plot_options.E_min, self.data_container.plot_options.E_max, 1000)
        E_min = np.ones((1000,)) * self.data_container.thresholds.E_min_1D
        E_max = np.ones((1000,)) * self.data_container.thresholds.E_max_1D
        self.ax.plot(E_min, y, color='black', linestyle='dashed', linewidth=2)
        self.ax.plot(E_max, y, color='black', linestyle='dashed', linewidth=2)

    @staticmethod
    def show():
        """ Show the thing! """
        plt.show()

    def format(self):
        """ Do all formatting of the graph here. This is the function to 
        overload when other style's are desired. 
        """
        # Set all axis to use scientific notation
        formatter = ScalarFormatter(useMathText=True)
        # formatter.set_scientific(True)
        # formatter.set_powerlimits((-2, 1))

        self.ax_E.yaxis.set_major_formatter(formatter)
        self.ax_S.xaxis.set_major_formatter(formatter)
        self.ax.yaxis.set_major_formatter(formatter)
        self.ax.xaxis.set_major_formatter(formatter)


        # Hide the default scientific notation lable, because their placement sucks. We want to place it in the axis label.
        self.ax_E.yaxis.get_offset_text().set_visible(False)
        self.ax_S.xaxis.get_offset_text().set_visible(False)
        # self.ax_E.ticklabel_format(style='sci', scilimits=(-2,2))
        # self.ax_S.ticklabel_format(style='sci', scilimits=(-2,2))
        # self.ax_E.yaxis.set_major_formatter(ScalarFormatter())


        # Force the right x and y range.
        self.ax.set_xlim([self.data_container.plot_options.E_min, self.data_container.plot_options.E_max])
        self.ax.set_ylim([self.data_container.plot_options.S_min, self.data_container.plot_options.S_max])

        # Force ticks to be between the given E and S range, and remove the highest value.
        self.ax.set_xticks(np.linspace(self.data_container.plot_options.E_min, self.data_container.plot_options.E_max, 6)[:-1])
        self.ax.set_yticks(np.linspace(self.data_container.plot_options.S_min, self.data_container.plot_options.S_max, 6)[:-1])
        # Remove ticks for highest values
        # self.ax.yaxis.set_major_locator(MaxNLocator(prune='upper'))
        # self.ax.xaxis.set_major_locator(MaxNLocator(prune='upper'))
        
        # Set lables. Check if the alpha and beta or different from default. 
        # If so, change the label text.
        fontdict = self.get_font()
        try:
            if self.data_container.corr.gamma != 1.0 or self.data_container.corr.beta != 1.0 and not self.data_container.corr.PIFE_correct:
                self.ax.set_xlabel('Accurate FRET $E$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict) # fontweight='bold', fontsize=16
                self.ax.set_ylabel('Stoichiometry $S$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict)
            elif self.data_container.corr.PIFE_correct:
                self.ax.set_xlabel('PIFE-corrected FRET $E$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict) # fontweight='bold', fontsize=16
                self.ax.set_ylabel('Stoichiometry $S$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict)
            else:
                self.ax.set_xlabel('Apparent FRET $E^*$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict) # fontweight='bold', fontsize=16
                self.ax.set_ylabel('Stoichiometry $S^*$', horizontalalignment='center',
                                   verticalalignment='bottom', )  # fontdict=fontdict)
            self.ax.set_aspect('auto')

            # Calculate the order of magnitude in the E and S direction
            # magnitude_E = self.ax_E.yaxis.get_offset_text().get_text()
            # magnitude_S = self.ax_S.xaxis.get_offset_text().get_text()
            magnitude_E = int(np.log10(self.ax_E.yaxis.get_data_interval()[1]))+1
            magnitude_S = int(np.log10(self.ax_S.xaxis.get_data_interval()[1]))+1
            
            # Lazy hack to force non-scientific labels.
            magnitude_E = 0
            magnitude_S = 0
            # Put the magnitude in the label and remove it from the plot.
            if magnitude_E >= 1:
                self.ax_E.set_ylabel('Events $/10^{0}$'.format(magnitude_E), horizontalalignment='center',
                                     verticalalignment='bottom', )  # fontdict=fontdict)
            else:
                self.ax_E.set_ylabel('Events'.format(magnitude_E), horizontalalignment='center',
                                     verticalalignment='bottom', ) 
            if magnitude_S >= 1:
                self.ax_S.set_xlabel('Events $/10^{0}$'.format(magnitude_S), horizontalalignment='center',
                                     verticalalignment='bottom', )  # fontdict=fontdict)
            else:
                self.ax_S.set_xlabel('Events'.format(magnitude_S), horizontalalignment='center',
                                     verticalalignment='bottom', ) 
            # self.ax_E.set_ylabel('Events {}'.format(magnitude_E), fontweight='bold', )
            # self.ax_S.set_xlabel('Events {}'.format(magnitude_S), fontweight='bold', )
        except:
            print 'I got called!'
            import sys

            sys.exit()

        # Set label positions manually so they are properly alligned.
        self.ax_E.yaxis.set_label_coords(self.format_options.y_label_offset, 0.5)
        self.ax.yaxis.set_label_coords(self.format_options.y_label_offset, 0.5)
        self.ax_S.xaxis.set_label_coords(0.5, self.format_options.x_label_offset)
        self.ax.xaxis.set_label_coords(0.5, self.format_options.x_label_offset)

        # Reduce the number of ticks on the 2D histogram
        # self.ax_E.locator_params(nbins=5, axis='x')
        # self.ax_S.locator_params(nbins=5, axis='y')
        # self.gs.update()

        # Remove ticks on E histogram
        plt.setp(self.ax_E.get_xticklabels(), visible=False)
        # Show only 3 ticks corrosponding to y values
        self.ax_E.locator_params(nbins=2, axis='y')
        # Similar for S
        plt.setp(self.ax_S.get_yticklabels(), visible=False)
        self.ax_S.locator_params(nbins=2, axis='x')


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
        self.data_container.thresholds.S_min_1D = Smin
        self.data_container.thresholds.S_max_1D = Smax
        self.data_container.update()

    @on_trait_change('selection_mode')
    def change_select_widget(self):
        if self.selection_mode == 'guesses':
            self.selector = RectangleSelector(self.ax, self.onselect_rect, drawtype='box')
        elif self.selection_mode == 's_range':
            self.selector = SpanSelector(self.ax, self.onselect_span, 'vertical', useblit=True,
                                         rectprops=dict(alpha=0.5, facecolor='white'))

    @on_trait_change('data_container.thresholds.[S_max_1D, S_min_1D]')
    def set_show_S_range(self):
        """ If the S_range is changed to anything other than 0.0 - 1.0, show it on the screen. 
        Do not show it if the range is the default range, because it doesn't look nice.
        """
        self.fitting.show_S_range = not (self.data_container.thresholds.S_max_1D == self.data_container.plot_options.S_max and self.data_container.thresholds.S_min_1D == self.data_container.plot_options.S_min)

    @on_trait_change('data_container.thresholds.[E_max_1D, E_min_1D]')
    def set_show_E_range(self):
        """ If the E_range is changed to anything other than 0.0 - 1.0, show it on the screen. 
        Do not show it if the range is the default range, because it doesn't look nice.
        """
        self.fitting.show_E_range = not (self.data_container.thresholds.E_max_1D == self.data_container.plot_options.E_max and self.data_container.thresholds.E_min_1D == self.data_container.plot_options.E_min)


# Custom color map
colors = ['#ffffff', '#800080', '#13007c', '#2bc0d7', '#7fc31c', '#f5f122', '#ff8000', '#ff0000']
# colors = [(0.0, '#ffffff'), (0.001, '#800080'), (0.003, '#13007c'), (0.01, '#2bc0d7'), (0.03, '#7fc31c'), (0.1, '#f5f122'), (0.3, '#ff8000'), (1.0, '#ff0000')]
thorben_cmap = matplotlib.colors.LinearSegmentedColormap.from_list('thorben', colors)
# thorben_cmap, norm = matplotlib.colors.from_levels_and_colors([5,800,1600,2500,2600,2600,2600,2600,2600], colors)
# thorben_cmap.name = 'thorben'
matplotlib.cm.register_cmap(cmap=thorben_cmap)