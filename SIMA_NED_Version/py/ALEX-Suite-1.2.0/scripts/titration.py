""" Main program. """
from __future__ import division
from alex.models import *
from alex.merge_bursts import DataManager
from alex.controller import Controller
from alex.functions import GuessesToFit, Guess, Gaussian, Zero
from alex.views import EvsSView
from scipy.optimize import curve_fit
import numpy as np
import matplotlib.pyplot as plt
import csv
import threading
from alex.leastsqbound import leastsqbound
from scipy.optimize import leastsq
from uncertainties import ufloat
import os
import copy
# from uncertainties import unumpy as unp


def get_concentration(path):
    """ Get the concentration from a path name using the default naming scheme.
  Don't change the scheme!
  """
    last = path.split('/')[-1]
    num = last.split('_')[-1]
    # Drop the unit
    return num[:-2]


def titration_curve(x, K, B):
    """Function to fit to."""
    return B * x / (K + x)


# def gaussian(x, p):
# x0, sig, A = p
# return A*np.exp( -(x - x0)**2 / (2*sig**2) )

# def two_gaussians(x, p):
# return gaussian(x, p[0:3]) + gaussian(x, p[3::])

# def error_two_gaussians(p, x, y):
# return two_gaussians(x, p) - y

# def error_gaussian(p, x, y):
# return gaussian(x, p) - y

def dfunc_gaussian(p, x, y):
    """ The gradient of a Gaussian with respect to their parameters.
    This is needed to calculate the jacobian and therefore fit errors.
    In mathematical terms, the returned list is:
    [df/d(x0), df/d(sig), df/dA]
    where f is the Gaussian distribution A*exp( -(x - x0)**2 / (2*sig**2) )
    """
    x0, sig, A = p
    v = np.exp(-(x - x0) ** 2 / (2 * sig ** 2))
    return [(x - x0) * A * v / sig ** 2, ((x - x0) ** 2 / sig ** 3) * A * v, v]
    # return np.array([((x - x0)*gaussian(x, p))/sig**2, ((x-x0)**2/sig**3)*gaussian(x, p), gaussian(x, p)/A])


def dfunc_two_gaussians(p, x, y):
    """ See dfunc for futher information. This simply splits the arguments over two dfunc functions."""
    return dfunc_gaussian(p[0:3], x, y) + dfunc_gaussian(p[3::], x, y)


def compare(evss, concentration, E1=False, E2=False, sig1=False, sig2=False, num_guesses=1, plot=False,
            number_of_bursts=False):
    # Object for fitting based on user specified guesses
    fitting = GuessesToFit(evss=evss)
    print fitting.evss.H_Epr, num_guesses
    fitting.guesses = [Guess() for i in range(num_guesses)]
    print fitting.guesses, E1, E2

    if E1 and E2:
        fitting.guesses[0].type_E.x0 = E1
        fitting.guesses[0].type_E.x0_fixed = True
        fitting.guesses[1].type_E.x0 = E2
        fitting.guesses[1].type_E.x0_fixed = True
        # fitting.guesses[0].type_E.A = 10000
        # fitting.guesses[1].type_E.A = 10000
        print 'definitelly here'
        fitting.guesses[1].type_S = Zero()
    if sig1 and sig2:
        fitting.guesses[0].type_E.sig = sig1
        fitting.guesses[0].type_E.sig_fixed = True
        fitting.guesses[1].type_E.sig = sig2
        fitting.guesses[1].type_E.sig_fixed = True
    elif num_guesses is not 2:
        pass
    else:
        raise Exception("I should not be here!")

    try:
        print fitting.guesses, fitting.guesses[0].type_E.x0, fitting.guesses[1].type_E.x0
    except:
        pass

    params = []
    for guess in fitting.guesses:
        params += guess.get_params_E()
        Func_E, Func_S = fitting._get_prototypes()

    # Calculate the residuals, which are to be minimised by the least squares algorithm.
    error_func = lambda p, x, y: Func_E.func(x, p) - y

    # if E1 and E2:
    # # In the case of two gaussians
    # try:
    # popt, cov_x, infodic, mesg, ier = leastsqbound(error_func, params, Dfun = dfunc_two_gaussians, args=(evss.Ecentres, evss.H_Epr), bounds=Func_E.get_bounds(), full_output=True, col_deriv=True)
    # s_sq = (error_two_gaussians(popt, evss.Ecentres, evss.H_Epr)**2).sum()/(len(evss.H_Epr)-len(params))
    # pcov = cov_x * s_sq
    # except TypeError:
    # import inspect
    # raise Exception(len(params), inspect.getargspec(error_func), inspect.getargspec(dfunc_two_gaussians))
    # else:
    # popt, cov_x, infodic, mesg, ier = leastsqbound(error_func, params, Dfun = dfunc_gaussian, args=(evss.Ecentres, evss.H_Epr), bounds=Func_E.get_bounds(), full_output=True, col_deriv=True)
    # s_sq = (error_gaussian(popt, evss.Ecentres, evss.H_Epr)**2).sum()/(len(evss.H_Epr)-len(params))
    # pcov = cov_x * s_sq

    popt, cov_x, infodic, mesg, ier = leastsqbound(
        error_func,
        params,
        Dfun=dfunc_two_gaussians if E1 and E2 else dfunc_gaussian,
        args=(evss.Ecentres, evss.H_Epr),
        bounds=Func_E.get_bounds(),
        full_output=True,
        col_deriv=True
    )
    s_sq = (error_func(popt, evss.Ecentres, evss.H_Epr) ** 2).sum() / (len(evss.H_Epr) - len(params))
    pcov = cov_x * s_sq

    # Do the fitting!
    fitting.execute()

    # Plot's are not shown on screen, but saved to a directory.
    if plot:
        # Start a plot instance
        evss_view = EvsSView(evss=evss, fitting=fitting)
        # evss_view.fig.suptitle('{} E1 = {}+-{}, E2 = {}+-{}'.format(concentration, E1, sig1, E2, sig2))
        # evss_view.update()
        # evss_view.show()
        type_of_analysis = 'weighed' if evss.options.weighted else 'thresholded'
        try:
            os.mkdir('{}_{}'.format(type_of_analysis, number_of_bursts))
        except OSError:
            pass
        plt.savefig(
            '{}_{}/{} E1 = {}+-{}, E2 = {}+-{}.pdf'.format(type_of_analysis, number_of_bursts, concentration, E1, sig1,
                                                           E2, sig2), bbox_inches='tight')

    # return E and dE values for every guess up to E2. dE is contained in pcov
    return [guess.type_E for guess in fitting.guesses] + [pcov]


def load_data(initial_dir, final_dir, list_of_dirs, data_manager):
    data_manager.sm_path = initial_dir
    # Load the .sm files.
    data_manager.execute()
    initial_data = data_manager.merged_data

    data_manager.sm_path = final_dir
    # Load the .sm files.
    data_manager.execute()
    final_data = data_manager.merged_data

    other = []
    for path in list_of_dirs:
        data_manager.sm_path = path
        # Load the .sm files.
        data_manager.execute()
        other.append((get_concentration(path), data_manager.merged_data))

    return initial_data, final_data, other


def titrate(initial_data_orig, final_data_orig, other_data_orig, thresholds, plot_options, corr, number_of_bursts=False,
            plot=True):
    # corr = FretCorrections()

    # Calculate position of E1
    temp_plot_options = copy.deepcopy(plot_options)
    temp_plot_options.weighted = False
    evss = EvsS(data=initial_data, options=temp_plot_options, thresholds=thresholds, corr=corr)
    # Throw away part of the dataset if we wan't to check the effect of the amount of data on the calculation.
    if number_of_bursts:
        evss = EvsS(data=evss.data[:int(number_of_bursts)], options=plot_options, thresholds=thresholds, corr=corr)
    else:
        evss = EvsS(data=initial_data, options=plot_options, thresholds=thresholds, corr=corr)
    fit_E, pcov = compare(evss, plot=plot, number_of_bursts=number_of_bursts, concentration='unliganded')
    E1 = fit_E.x0
    sig1 = fit_E.sig

    # Calculate position of E2
    evss = EvsS(data=final_data, options=temp_plot_options, thresholds=thresholds, corr=corr)
    if number_of_bursts:
        evss = EvsS(data=evss.data[:int(number_of_bursts)], options=plot_options, thresholds=thresholds, corr=corr)
    else:
        evss = EvsS(data=final_data, options=plot_options, thresholds=thresholds, corr=corr)
    fit_E, pcov = compare(evss, plot=plot, number_of_bursts=number_of_bursts, concentration='500uM')
    E2 = fit_E.x0
    sig2 = fit_E.sig

    # Using the fixed E1, E2, sig1, sig2 just found we will now fit to all other conditions.
    for concentration, data in other_data:
        evss = EvsS(data=data, options=temp_plot_options, thresholds=thresholds, corr=corr)
        if number_of_bursts:
            evss = EvsS(data=evss.data[:int(number_of_bursts)], options=plot_options, thresholds=thresholds, corr=corr)
        else:
            evss = EvsS(data=data, options=plot_options, thresholds=thresholds, corr=corr)
        # raise Exception(evss.H_Epr)
        fit_E1, fit_E2, pcov = compare(evss, plot=plot, num_guesses=2, E1=E1, E2=E2, sig1=sig1, sig2=sig2,
                                       number_of_bursts=number_of_bursts, concentration=concentration)
        Area1 = fit_E1.get_area()
        Area2 = fit_E2.get_area()
        ratio = Area2 / (Area2 + Area1)

        # pcov is the Jacobian covariance matrix. The standard deviations are given by the sqrt of the diagonal element.
        stdevs = np.sqrt(np.diag(pcov))

        # Function to calculate the area under a Gaussian.
        area = lambda A, sig: np.abs(A * sig * np.sqrt(2 * np.pi))
        # ufloat's are used for automatic error propagation.
        A1 = ufloat(fit_E1.A, stdevs[2])
        A2 = ufloat(fit_E2.A, stdevs[5])
        usig1 = ufloat(np.abs(fit_E1.sig), stdevs[1])
        usig2 = ufloat(np.abs(fit_E2.sig), stdevs[4])
        ratio = area(A2, usig2) / (area(A2, usig2) + area(A1, usig1))
        # raise Exception(ratio, area(A1, usig1), area(A2, usig2), A1, A2, usig1, usig2, stdevs)
        yield concentration, ratio.nominal_value, ratio.std_dev


def titration_fit(concentration, ratio, dratio=False):
    if dratio:
        return curve_fit(titration_curve, concentration, ratio, sigma=dratio)  # , absolute_sigma = True)
    else:
        return curve_fit(titration_curve, concentration, ratio)


def titration_plot(concentration, ratio, popt, title, dratio=False, number_of_bursts=False, ):
    c_continous = np.linspace(0, np.max(concentration))
    r_orig_fit = titration_curve(c_continous, *popt)

    # plt.hold(False)
    plt.figure()
    if dratio:
        p1 = plt.errorbar(concentration, ratio, fmt='kx', yerr=dratio, marker='s')
    else:
        p1 = plt.plot(concentration, ratio, 'rx')
    plt.plot(c_continous, r_orig_fit, 'k')
    plt.ylabel('open/(open+closed)')
    plt.xlabel('Substrate Concentration')
    plt.ticklabel_format(style='sci', axis='x', scilimits=(0, 0))
    if number_of_bursts:
        plt.title('{}'.format(number_of_bursts))

    plt.savefig('{}_{}.pdf'.format(title, number_of_bursts))
    # plt.show()


if __name__ == '__main__':
    # Burst search setting
    params = ReducParams()
    params.reduction_mode = 1  # 0 = APBS, 1 = DCBS

    # Initiate a data_manager mass-converter, for reading .sm files in bulk
    data_manager = DataManager(setting=params)

    # These objects are needed by the EvsS object. Although I suppose you could actually
    # use the FretCorrections object in particular to include accurate FRET corrections.
    metadata = Metadata()
    corr = FretCorrections()

    base_dir = '/Users/Martin/Downloads/'
    initial_dir = base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#1_unliganded"
    final_dir = base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#9_Asn_500uM"
    list_of_dirs = [base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#2_Ans_100nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#3_Ans_200nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#4_Ans_300nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#5_Asn_500nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#6_Ans_700nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#7_Asn_1000nM",
                    base_dir + "#4_P65_B10_4000D_30uWG_15uWR/#8_Asn_1500nM",
                    ]
    #
    #
    # EXAMPLE IN WINDOWS
    #
    #

    # initial_dir = "#4_P65_B10_4000D_30uWG_15uWR\\#1_unliganded"
    # final_dir = "#4_P65_B10_4000D_30uWG_15uWR\\#9_Asn_500uM"
    # list_of_dirs = ["#4_P65_B10_4000D_30uWG_15uWR\\#2_Ans_100nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#3_Ans_200nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#4_Ans_300nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#5_Asn_500nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#6_Ans_700nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#7_Asn_1000nM",
    # "#4_P65_B10_4000D_30uWG_15uWR\\#8_Asn_1500nM",
    # ]


    #
    #
    # Set the other settings desired. For more options, you are going to have to read the source.
    #
    #
    # Thresholded.
    plot_options_thresholded = PlotOptions()
    thresholded = Thresholds()
    thresholded.S_min = 0.3
    thresholded.S_max = 0.8
    thresholded.total_min = 250

    # Weighed.
    # plot_options_weighed = PlotOptions()
    # weighed = Thresholds()
    # weighed.S_min = 0.3
    # weighed.S_max = 0.8
    # thresholded.total_min = 100
    # plot_options_weighed.weighted = True

    # If you want to compare the effect of different settings, add them to the list.
    conditions = [
        (thresholded, plot_options_thresholded, False),
        # (thresholded, plot_options_weighed, 1000),
        # (thresholded, plot_options_thresholded, 2000),
        # etc.
    ]

    initial_data, final_data, other_data = load_data(initial_dir, final_dir, list_of_dirs, data_manager)
    with open('results.csv', 'w') as result_file:
        writer = csv.writer(result_file)
        writer.writerow(['Type', 'Number of Bursts limit', 'k', 'B', 'dk', 'dB'])
        for thresholds, plot_options, number_of_bursts in conditions:
            result = titrate(initial_data, final_data, other_data, thresholds, plot_options, corr,
                             number_of_bursts=number_of_bursts, plot=True)
            result = list(result)
            concentration = [float(c) * 10 ** -9 for c, r, dr in result]
            ratio = [float(r) for c, r, dr in result]
            dratio = [float(dr) for c, r, dr in result]
            popt, pcov = titration_fit(concentration, ratio, dratio)
            # plot_that_shit = threading.Thread(target = titration_plot, args = (concentration, ratio, popt, number_of_bursts))
            # plot_that_shit.start()
            title = 'weighed' if plot_options.weighted else 'thresholded'
            titration_plot(concentration, ratio, popt, title, dratio, number_of_bursts)

            writer.writerow([title, number_of_bursts] + list(popt) + list(np.sqrt(np.diag(pcov))))