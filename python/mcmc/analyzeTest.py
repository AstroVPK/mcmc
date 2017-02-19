import math
import cmath
import numpy as np
import matplotlib.pyplot as plt

numPts = 10000
weibullData = np.zeros(numPts)

with open('/home/vish/code/mcmc/weibullData.txt', 'rb') as weibullDataFile:
    for i in xrange(numPts):
        weibullData[i] = float(weibullDataFile.readline().rstrip('\n'))

plt.hist(weibullData, 100, normed=1, facecolor='green', alpha=0.75)

plt.show()
