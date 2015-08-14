ALEX-Tools
==========

Python program allowing easy analysis of data obtained from Alternating Laser Excistation (ALEX) experiments. 

Offers a lot of tools used in the field today, such as:
- Burst Variance Analysis
- Fitting of E histogram to obtain FRET values
- Accurate FRET
- Thresholding
- Flexible binning and plot options
- Burst Size distribution view

Requirements
============

- Python 2.7
- alex_tools
- numpy
- scipy
- matplotlib
- reportlab
- enaml==0.6.8

Adding a new View
============
Suppose we wan't to add a new view. The following objects must exist:
- A subclass DataContainer which performs all numerical operations on the data in it's .data attribute.
- A subclass of MPLBasePlot which forms the plot of the DataContainer in it's .data_container attribute.
- A subclass of WindowController to hook everything up together with any options you might need. 

Time for an example.

```python
# models.py
class BVA(DataContainer):
    """ All BVA functionality is here, e.g. generating the E histogram and the standarddeviation info.
    Plotting this info is done by the plot BVAPlot.
    """
    # Methods and attributes to make stuff work go here
    data = Array  # This is already in DataContainer but made explicit here for clarity
    thresholds = Instance(Thresholds)
    plot_options = Instance(PlotOptions)

# views.py
class BVAPlot(MPLBasePlot):
    """ View for the BVA object. Makes a beatiful plot of the BVA data."""
    data_container = Instance(BVA)
    format_options = Instance(PlotFormatOptions)

# controller.py
class BVAViewController(WindowController, 
                        ThresholdsMixin,
                        PlotOptionsMixin, 
                        PlotFormatOptionsMixin,
                        ):
    """ Controller for the BVAPlot """
    data_container = Instance(BVA)
    plot = Instance(BVAPlot)

    def get_window_title(self):
        return 'BVA View'
```

This is enough to make a new view! Off-cource it is up to you to make sure the BVAPlot actually sets it's .fig attribute with a valid matplotlib.Figure and that the data get's handled properly. Furthermore, when new Mixins are needed they must be created in mixins.py.

For this new view to be called from your enaml main program, it must set the analysis_type attribute on Controller, and the new WindowController must be available in the window_controllers dict under that key. The window_controllers dict looks like this at the time of writing:

```python
    window_controllers = Dict({'evss': EvsSViewController, 
                               'bva_viewer': BVAViewController, 
                               'dataset': DatasetViewController,
                               })
```
