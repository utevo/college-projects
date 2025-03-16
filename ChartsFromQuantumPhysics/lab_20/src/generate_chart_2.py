import numpy as np
import matplotlib.pyplot as plt
import numpy
import scipy.optimize as optimization
import scipy.stats

def func(x, a, b):
    return a*x + b

y_heatintg_all = []
x_heatintg_all = []
y_cooling_all = []
x_cooling_all = []

with open("../data/heating_best.txt", "r") as f:
    searchlines = f.readlines()
for line in searchlines:
	list_of_data = line.split()
	y_heatintg_all.append( float(list_of_data[2]) )
	x_heatintg_all.append( int(list_of_data[0]) )

with open("../data/cooling_best.txt", "r") as f:
    searchlines = f.readlines()
for line in searchlines:
	list_of_data = line.split()
	y_cooling_all.append( float(list_of_data[2]) )
	x_cooling_all.append( int(list_of_data[0]) )


plt.plot(x_heatintg_all, y_heatintg_all, label = "Heatintg", marker = ".")
fit = np.polyfit(x_heatintg_all, y_heatintg_all, 1)
fit_fn = np.poly1d(fit) 
plt.plot(x_heatintg_all, fit_fn(x_heatintg_all), '--k')

plt.plot(x_cooling_all, y_cooling_all, label = "Cooling", marker = "v", color = "orange")
fit = np.polyfit(x_cooling_all, y_cooling_all, 1)
fit_fn = np.poly1d(fit) 
plt.plot(x_cooling_all, fit_fn(x_cooling_all), '--k')

plt.ylabel("1/U [1/V]")
plt.xlabel("T [°C]")
plt.legend()
plt.minorticks_on()


plt.savefig("../result/chart_2")