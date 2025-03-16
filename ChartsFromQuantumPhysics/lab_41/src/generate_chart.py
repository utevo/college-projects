import numpy as np
import matplotlib.pyplot as plt
import numpy
import scipy.optimize as optimization

def func(x, a, b):
    return a*x + b

x_data = numpy.array([973, 1658, 1874])
y_data = numpy.array([0.661, 1.172, 1.333])


sigma = numpy.array([51.5, 116.5, 69])
label = numpy.array(["Cs-137 (max)", "Co-60 (max 1)", "Co-69 (max 2)" ])

x2_data = numpy.array([700, 973, 1658, 1874 ,1950])
fmt = numpy.array(["v", "o", "s" ])


popt, pcov = optimization.curve_fit(func, x_data, y_data, sigma = sigma) 
perr = np.sqrt(np.diag(pcov))

K = numpy.array([1815, 756])
E = func(K, popt[0], popt[1])
label2 = numpy.array(["Na-22 (max 1)", "Na-22 (max 2)"])
fmt2 = numpy.array(["p", "P"])
sigma2 = numpy.array([0.1201, 0.06788])

plt.plot(x2_data, func(x2_data, popt[0], popt[1]), '--'  )

for i in range(3):
	plt.errorbar(x_data[i], y_data[i], label=label[i], xerr=sigma[i], fmt=fmt[i], capsize=5 )
for i in range(2):
	plt.errorbar(K[i], E[i], fmt=fmt2[i], yerr=sigma2[i], label=label2[i], capsize=5)


plt.legend()
plt.minorticks_on()
plt.ylabel("Energy [MeV]")
plt.xlabel("Channel")

plt.savefig("../result/chart")