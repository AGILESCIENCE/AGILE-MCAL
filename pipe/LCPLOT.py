#################################
#                               #
# #     ### ###  #     ##  #### #
# #    #    #  # #    #  #  #   #
# #    #    ###  #    #  #  #   #
# #    #    #    #    #  #  #   #
# ####  ### #    ####  ##   #   #
#                               #
###########################################################
#                                                         #
# LCPLOT creates light curves in different energy ranges  #
#                                                         #
# $ python LCPLOT.py [CONTACT] [T0] [TSTART] [TSTOP]      #
#                                                         #
###########################################################

import os, sys, math, os.path
from scipy.stats import poisson
import numpy as np
import matplotlib.pyplot as plt
import itertools
from itertools import groupby
import ROOT
from ROOT import TFile
from ROOT import gDirectory

CONTACT = []
TRIGGER = []
T0 = []

CONTACT = int(sys.argv[1])
OUTPATH_ROOT = str(sys.argv[2])
T0 = float(sys.argv[3])
PRE = float(sys.argv[4])
POST = float(sys.argv[5])
BIN = float(sys.argv[6])

# python lightcurve.py 50232 /ANALYSIS3/ASDC_NEW3/ 410609511.700845 10. 15. 0.064

######################################################################
# READ ROOT FILE FOR THE WHOLE CONTACT AND GET THE LIST OF TIMES t[] #
######################################################################

ROOTNAME  = ROOT.TFile.Open("%s/RT%06.d_3908.root" % (OUTPATH_ROOT, CONTACT))
TREENAME  = ROOTNAME.Get("tdata")
CHAIN     = gDirectory.Get("tdata")
N_ENTRIES = CHAIN.GetEntriesFast()

# t is the time array with [photon time]

tt = []
ttt = []
E = []
t1 = []
t2 = []
t3 = []
t4 = []
t5 = []
t6 = []
EE = []*2
events = []

for atrg in ROOTNAME.tdata:
    tt.append(float(atrg.time-T0))
    ttt.append(float(atrg.time))
    E.append(float(atrg.Etot))
    if abs(atrg.time-T0) <= 0.02:
	events.append(float(atrg.time-T0))

for j in range(0,len(tt)-1):
	if E[j] <= 0.7:
		t1.append(tt[j])
	elif E[j] > 0.7 and E[j] <= 1.4:
		t2.append(tt[j])
	elif E[j] > 1.4 and E[j] <= 2.8:
		t3.append(tt[j])
	elif E[j] > 2.8:
		t4.append(tt[j])
	if E[j] < 1.4:
		t5.append(tt[j])
	if E[j] >= 1.4:
		t6.append(tt[j])

#######################
# BACKGROUND ESTIMATE #
#######################

t_minBKG = T0-PRE
t_maxBKG = T0+POST

N_BKG = int(((t_maxBKG-t_minBKG)/BIN))

BSBKG = np.arange((int)(t_minBKG), (int)(t_maxBKG)+1, BIN)

BACKGROUND = np.zeros((N_BKG, 2))

histbkg, bin_edgesbkg = np.histogram(ttt, bins=BSBKG)

for nBKG in range(0,N_BKG):
	BACKGROUND[nBKG]=[BSBKG[nBKG], histbkg[nBKG]]

BKG = np.mean(BACKGROUND[ np.where( (BACKGROUND[:,1] != 0 ) ), 1 ])
THR = BKG + 5*math.sqrt(BKG)

print "BKG TOT ", BKG

###############################################
# BINNING AND TOTAL LIGHTCURVE OF THE CONTACT #
###############################################

l0 = len(tt)

t_min0 = tt[0]
t_max0 = tt[l0-1]

N_BIN = int(((t_max0-t_min0)/BIN))

BS0 = np.arange((int)(t_min0), (int)(t_max0)+2, BIN)

LIGHTCURVE0 = np.zeros((N_BIN, 2))

hist0, bin_edges0 = np.histogram(tt, bins=BS0)

for n0 in range(0,N_BIN-1):
    LIGHTCURVE0[n0]=[BS0[n0+1], hist0[n0]]

N_BKG0 = int(((t_max0-t_min0)/BIN))

BACKGROUND0 = np.zeros((N_BKG0, 2))

histbkg0, bin_edgesbkg0 = np.histogram(tt, bins=BS0)

for nBKG0 in range(0,N_BKG0):
	BACKGROUND0[nBKG0]=[BS0[nBKG0], histbkg0[nBKG0]]

BKG0 = np.mean(BACKGROUND0[ np.where( (BACKGROUND0[:,1] != 0 ) ), 1 ])
THR0 = BKG0 + 5*math.sqrt(BKG0) 

fig = plt.figure(figsize=(12,18))
ax0 = fig.add_subplot(811)
ax0.step(LIGHTCURVE0[:,0], LIGHTCURVE0[:,1], markersize=5, c='black')
ax0.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
plt.axhline(y=THR, linewidth=3, color = 'black', linestyle = 'dashed', alpha = 0.3)
ax0.set_ylabel('[counts / %3.3f s]\n0.35 - 100 MeV' % BIN)
ax0.set_title('CONT %06.d - BIN %3.3f ms -  T0 %f\n' % (CONTACT, BIN, T0), fontsize=22)


###########################################

l1 = len(t1)

t_min1 = t1[0]
t_max1 = t1[l1-1]

N_BIN = int(((t_max1-t_min1)/BIN))

BS1 = np.arange((int)(t_min1), (int)(t_max1)+2, BIN)

LIGHTCURVE1 = np.zeros((N_BIN, 2))

hist1, bin_edges1 = np.histogram(t1, bins=BS1)

for n1 in range(0,N_BIN-1):
    LIGHTCURVE1[n1]=[BS1[n1+1], hist1[n1]]


N_BKG1 = int(((t_max1-t_min1)/BIN))

BACKGROUND1 = np.zeros((N_BKG1, 2))

histbkg1, bin_edgesbkg1 = np.histogram(t1, bins=BS1)

for nBKG1 in range(0,N_BKG1):
	BACKGROUND1[nBKG1]=[BS1[nBKG1], histbkg1[nBKG1]]

BKG1 = np.mean(BACKGROUND1[ np.where( (BACKGROUND1[:,1] != 0 ) ), 1 ])

ax1 = fig.add_subplot(812)
ax1.step(LIGHTCURVE1[:,0], LIGHTCURVE1[:,1], markersize=5, linewidth=1, c='b')
ax1.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax1.set_ylabel('[counts / %3.3f s]\n< 0.7 MeV' % BIN)

###########################################

l2 = len(t2)

t_min2 = t2[0]
t_max2 = t2[l2-1]

N_BIN = int(((t_max2-t_min2)/BIN))

BS2 = np.arange((int)(t_min2), (int)(t_max2)+2, BIN)

LIGHTCURVE2 = np.zeros((N_BIN, 2))

hist2, bin_edges2 = np.histogram(t2, bins=BS2)

for n2 in range(0,N_BIN-1):
    LIGHTCURVE2[n2]=[BS2[n2+1], hist2[n2]]

N_BKG2 = int(((t_max2-t_min2)/BIN))

BACKGROUND2 = np.zeros((N_BKG2, 2))

histbkg2, bin_edgesbkg2 = np.histogram(t2, bins=BS2)

for nBKG2 in range(0,N_BKG2):
	BACKGROUND2[nBKG2]=[BS2[nBKG2], histbkg2[nBKG2]]

BKG2 = np.mean(BACKGROUND2[ np.where( (BACKGROUND2[:,1] != 0 ) ), 1 ])

#ax.legend()
ax2 = fig.add_subplot(813)
ax2.step(LIGHTCURVE2[:,0], LIGHTCURVE2[:,1], markersize=5, linewidth=1, c='b')
ax2.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax2.set_ylabel('[counts / %3.3f s]\n0.7 - 1.4 MeV' % BIN)

############################################

l3 = len(t3)

t_min3 = t3[0]
t_max3 = t3[l3-1]

N_BIN = int(((t_max3-t_min3)/BIN))

BS3 = np.arange((int)(t_min3), (int)(t_max3)+2, BIN)

LIGHTCURVE3 = np.zeros((N_BIN, 2))

hist3, bin_edges3 = np.histogram(t3, bins=BS3)

for n3 in range(0,N_BIN-1):
    LIGHTCURVE3[n3]=[BS3[n3+1], hist3[n3]]

N_BKG3 = int(((t_max3-t_min3)/BIN))

BACKGROUND3 = np.zeros((N_BKG3, 2))

histbkg3, bin_edgesbkg3 = np.histogram(t3, bins=BS3)

for nBKG3 in range(0,N_BKG3):
	BACKGROUND3[nBKG3]=[BS3[nBKG3], histbkg3[nBKG3]]

BKG3 = np.mean(BACKGROUND3[ np.where( (BACKGROUND3[:,1] != 0 ) ), 1 ])

ax3 = fig.add_subplot(814)
ax3.step(LIGHTCURVE3[:,0], LIGHTCURVE3[:,1], markersize=5, linewidth=1, c='b')
ax3.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax3.set_ylabel('[counts / %3.3f s]\n1.4 - 2.8 MeV' % BIN)

###########################################

l4 = len(t4)

t_min4 = t4[0]
t_max4 = t4[l4-1]

N_BIN = int(((t_max4-t_min4)/BIN))

BS4 = np.arange((int)(t_min4), (int)(t_max4)+2, BIN)

LIGHTCURVE4 = np.zeros((N_BIN, 2))

hist4, bin_edges4 = np.histogram(t4, bins=BS4)

for n4 in range(0,N_BIN-1):
    LIGHTCURVE4[n4]=[BS4[n4+1], hist4[n4]]

N_BKG4 = int(((t_max4-t_min4)/BIN))

BACKGROUND4 = np.zeros((N_BKG4, 2))

histbkg4, bin_edgesbkg4 = np.histogram(t4, bins=BS4)

for nBKG4 in range(0,N_BKG4):
	BACKGROUND4[nBKG4]=[BS4[nBKG4], histbkg4[nBKG4]]

BKG4 = np.mean(BACKGROUND4[ np.where( (BACKGROUND4[:,1] != 0 ) ), 1 ])

#ax.legend()
ax4 = fig.add_subplot(815)
ax4.step(LIGHTCURVE4[:,0], LIGHTCURVE4[:,1], markersize=5, linewidth=1, c='b')
ax4.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax4.set_ylabel('[counts / %3.3f s]\n> 2.8 MeV' % BIN)

###########################################

l5 = len(t5)

t_min5 = t5[0]
t_max5 = t5[l5-1]

N_BIN = int(((t_max5-t_min5)/BIN))

BS5 = np.arange((int)(t_min5), (int)(t_max5)+2, BIN)

LIGHTCURVE5 = np.zeros((N_BIN, 2))

hist5, bin_edges5 = np.histogram(t5, bins=BS5)

for n5 in range(0,N_BIN-1):
    LIGHTCURVE5[n5]=[BS5[n5+1], hist5[n5]]
 
N_BKG5 = int(((t_max5-t_min5)/BIN))

BACKGROUND5 = np.zeros((N_BKG5, 2))

histbkg5, bin_edgesbkg5 = np.histogram(t5, bins=BS5)

for nBKG5 in range(0,N_BKG5):
	BACKGROUND5[nBKG5]=[BS5[nBKG5], histbkg5[nBKG5]]

BKG5 = np.mean(BACKGROUND5[ np.where( (BACKGROUND5[:,1] != 0 ) ), 1 ])

ax5 = fig.add_subplot(816)
ax5.step(LIGHTCURVE5[:,0], LIGHTCURVE5[:,1], markersize=5, linewidth=1, c='blue')
ax5.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax5.set_ylabel('[counts / %3.3f s]\n< 1.4 MeV' % BIN)

###########################################

l6 = len(t6)

t_min6 = t6[0]
t_max6 = t6[l6-1]

N_BIN = int(((t_max6-t_min6)/BIN))

BS6 = np.arange((int)(t_min6), (int)(t_max6)+2, BIN)

LIGHTCURVE6 = np.zeros((N_BIN, 2))

hist6, bin_edges6 = np.histogram(t6, bins=BS6)

for n6 in range(0,N_BIN-1):
    LIGHTCURVE6[n6]=[BS6[n6+1], hist6[n6]]
 
N_BKG6 = int(((t_max6-t_min6)/BIN))

BACKGROUND6 = np.zeros((N_BKG6, 2))

histbkg6, bin_edgesbkg6 = np.histogram(t6, bins=BS6)

for nBKG6 in range(0,N_BKG6):
	BACKGROUND6[nBKG6]=[BS6[nBKG6], histbkg6[nBKG6]]

BKG6 = np.mean(BACKGROUND6[ np.where( (BACKGROUND6[:,1] != 0 ) ), 1 ])

ax6 = fig.add_subplot(817)
ax6.step(LIGHTCURVE6[:,0], LIGHTCURVE6[:,1], markersize=5, linewidth=1, c='blue')
ax6.set_xlim(-PRE,POST)
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
ax6.set_ylabel('[counts / %3.3f s]\n>= 1.4 MeV' % BIN)

###############################
# PLOT COUNTS IN SCATTER PLOT #
###############################

ENERGIES = np.zeros((len(tt), 2))

for j in range(0,len(tt)-1):
	ENERGIES[j] = [tt[j], E[j]]

ax7 = fig.add_subplot(818)
ax7.scatter(ENERGIES[:,0], ENERGIES[:,1], s=2)
ax7.set_ylim(0,560)
ax7.set_xlim(-PRE,POST)
ax7.set_xlabel('time - %s [s]' % T0, fontsize=20)
ax7.set_ylabel('E [MeV]')
plt.axvline(x=0, linewidth=5, color = 'magenta', alpha = 0.3)
plt.savefig('%s/GRB/GRB_%06.d_%9.6f.png' % (OUTPATH_ROOT, CONTACT, T0) )


#######
# END #
#######
