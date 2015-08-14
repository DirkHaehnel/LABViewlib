from subprocess import check_output
from collections import namedtuple
import csv
from models import *
from views import *
import numpy as np

def run_burst_search(filename, ReducParam):
    """ Calls the sm2burst C++ utility on the given .sm file """
    check_output(["./sm2burst", filename] + list(params))

ReducParam = namedtuple('ReducParam', ['NI_TimeResolution', 'AlexPeriod', 
                                       'FirstHalf', 'Arrival_Threshold',
                                       'Min_Neighbors', 'Min_PhotPerBurst', 
                                       'ReductionMode', 'UseBurstArrivMat', 
                                       'BinningTime'
                                       ])
params = ReducParam("12.5E-09", "1E-04", "5E-05", "0.0005", "7", "12", "3", "0","0.001")
# run_burst_search("/Users/tBuLi/Dropbox/RuG/Projecten/MatlabALEX/008_g80uW_r30uW_ds22_8bp_PBS_2mMTX_pH7-4/008_g80uW_r30uW_ds22_8bp_PBS_2mMTX_pH7-4_#4.sm", params)

# with open("/Users/tBuLi/Dropbox/RuG/Projecten/MatlabALEX/005_g80uW_r30uW_ds24_PBS_2mMTX_pH7-4/005.sm.wrapped.csv", 'rU') as file:
with open("/Users/tBuLi/Dropbox/RuG/Projecten/MatlabALEX/008_g80uW_r30uW_ds22_8bp_PBS_2mMTX_pH7-4/008_g80uW_r30uW_ds22_8bp_PBS_2mMTX_pH7-4_#4.sm.wrapped.csv", 'rU') as file:
    csvreader = csv.reader(file)
    raw_data = list(csvreader)
    # Create a named array so columns can be adressed by their name
    data = np.array(raw_data[12:])
    data = np.array(map(tuple,data), [(n, 'float64') for n in raw_data[11]])

evss = EvsS(data, [50,50])
print np.all(evss.data == evss.data_orig)
th = Thresholds()
evss.apply_thresholds(th)
evss.update()
print np.all(evss.data == evss.data_orig)
view = EvsSView(evss)
view.plot()
view.show()