#!/bin/python

import matplotlib.pyplot as plt

data = open("log","r").read()
data = data.split("\n")
data = data[:-6]
data = [float(d) for d in data]
data = data[::-1]

plt.plot(data)
plt.xlabel("Length of no-op slide")
plt.ylabel("Access time in clock cycles")
plt.show()
