import numpy as np
import matplotlib.pyplot as plt

y_heatintg_all = []
x_heatintg_all = []
y_cooling_all = []
x_cooling_all = []

with open("../data/heating_all.txt", "r") as f:
    searchlines = f.readlines()
for line in searchlines:
	list_of_data = line.split()
	y_heatintg_all.append( float(list_of_data[1]) )
	x_heatintg_all.append( int(list_of_data[0]) )

with open("../data/cooling_all.txt", "r") as f:
    searchlines = f.readlines()
for line in searchlines:
	list_of_data = line.split()
	y_cooling_all.append( float(list_of_data[1]) )
	x_cooling_all.append( int(list_of_data[0]) )


plt.plot(x_heatintg_all, y_heatintg_all, label = "Heating", marker = ".")
plt.plot(x_cooling_all, y_cooling_all, label = "Cooling", marker = "v")
plt.ylabel("U [V]")
plt.xlabel("T [°C]")
plt.legend()
plt.minorticks_on()

plt.savefig("../result/chart_0")