import math
import cmath
import numpy as np
import matplotlib.pyplot as plt

numPts = 10000


weibullData = np.zeros(numPts)
with open('/home/vish/code/mcmc/weibullData.txt', 'rb') as weibullDataFile:
    numPtsLine = weibullDataFile.readline()
    numPts = int(numPtsLine.rstrip('\n').split(' ')[1])
    shapeLine = weibullDataFile.readline()
    shapeWords = shapeLine.rstrip('\n').split(' ')
    shape = float(shapeWords[1].rstrip(';'))
    shapeEst = float(shapeWords[3])
    displLine = weibullDataFile.readline()
    displWords = displLine.rstrip('\n').split(' ')
    displ = float(displWords[1].rstrip(';'))
    displEst = float(displWords[3])
    scaleLine = weibullDataFile.readline()
    scaleWords = scaleLine.rstrip('\n').split(' ')
    scale = float(scaleWords[1].rstrip(';'))
    scaleEst = float(scaleWords[3])
    for i in xrange(numPts):
        weibullData[i] = float(weibullDataFile.readline().rstrip('\n'))

fig1 = plt.figure(1, figsize=(16, 16))
fig1.clf()
ax1 = fig1.add_subplot(111)
plt.hist(weibullData, 100, normed=1, facecolor='green', alpha=0.75)
bbox_props = dict(boxstyle='round,pad=0.3', fc='white', ec='black', lw=2)
ent = '\n'
text1 = r'$\mathrm{shape} - \langle shape \rangle = %e$'%(shape - shapeEst)
text2 = r'$\mathrm{disp} - \langle disp \rangle = %e$'%(displ - displEst)
text3 = r'$\mathrm{scale} - \langle scale \rangle = %e$'%(scale - scaleEst)
t = ax1.text(2.0, 0.5, text1 + ent + text2 + ent + text3,
             ha='center', va='center', rotation=0, size=15, bbox=bbox_props)

exponentialData = np.zeros(numPts)
with open('/home/vish/code/mcmc/exponentialData.txt', 'rb') as exponentialDataFile:
    numPtsLine = exponentialDataFile.readline()
    numPts = int(numPtsLine.rstrip('\n').split(' ')[1])
    displLine = exponentialDataFile.readline()
    displWords = displLine.rstrip('\n').split(' ')
    displ = float(displWords[1].rstrip(';'))
    displEst = float(displWords[3])
    scaleLine = exponentialDataFile.readline()
    scaleWords = scaleLine.rstrip('\n').split(' ')
    scale = float(scaleWords[1].rstrip(';'))
    scaleEst = float(scaleWords[3])
    for i in xrange(numPts):
        exponentialData[i] = float(exponentialDataFile.readline().rstrip('\n'))

fig2 = plt.figure(2, figsize=(16, 16))
fig2.clf()
ax2 = fig2.add_subplot(111)
plt.hist(exponentialData, 100, normed=1, facecolor='green', alpha=0.75)
bbox_props = dict(boxstyle='round,pad=0.3', fc='white', ec='black', lw=2)
ent = '\n'
text1 = r'$\mathrm{disp} - \langle disp \rangle = %e$'%(displ - displEst)
text2 = r'$\mathrm{scale} - \langle scale \rangle = %e$'%(scale - scaleEst)
t = ax2.text(2.0, 0.5, text1 + ent + text2,
             ha='center', va='center', rotation=0, size=15, bbox=bbox_props)


rayleighData = np.zeros(numPts)
with open('/home/vish/code/mcmc/rayleighData.txt', 'rb') as rayleighDataFile:
    numPtsLine = rayleighDataFile.readline()
    numPts = int(numPtsLine.rstrip('\n').split(' ')[1])
    displLine = rayleighDataFile.readline()
    displWords = displLine.rstrip('\n').split(' ')
    displ = float(displWords[1].rstrip(';'))
    displEst = float(displWords[3])
    scaleLine = rayleighDataFile.readline()
    scaleWords = scaleLine.rstrip('\n').split(' ')
    scale = float(scaleWords[1].rstrip(';'))
    scaleEst = float(scaleWords[3])
    for i in xrange(numPts):
        rayleighData[i] = float(rayleighDataFile.readline().rstrip('\n'))

fig3 = plt.figure(3, figsize=(16, 16))
fig3.clf()
ax3 = fig3.add_subplot(111)
plt.hist(rayleighData, 100, normed=1, facecolor='green', alpha=0.75)
bbox_props = dict(boxstyle='round,pad=0.3', fc='white', ec='black', lw=2)
ent = '\n'
text1 = r'$\mathrm{disp} - \langle disp \rangle = %e$'%(displ - displEst)
text2 = r'$\mathrm{scale} - \langle scale \rangle = %e$'%(scale - scaleEst)
t = ax3.text(2.0, 0.5, text1 + ent + text2,
             ha='center', va='center', rotation=0, size=15, bbox=bbox_props)


plt.show()
