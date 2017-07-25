##################
#                #
# ####  ##  ###  #
# #    #  # #  # #
# ##   #### ###  #
# #    #  # # #  #
# #    #  # #  # #
#                #
##################
#
# USAGE: $ python FAR.py 50232 50234 /home/rt/ursipipe/OUT 0.096 4.4
#

import os, sys, math, os.path
import numpy as np
import matplotlib.pyplot as plt
import itertools
from itertools import groupby
import ROOT
from ROOT import TFile
from ROOT import gDirectory

IN_CONT = int(sys.argv[1])
FIN_CONT = int(sys.argv[2])
OUTPATH_ROOT = str(sys.argv[3])
BIN = float(sys.argv[4])
THR = float(sys.argv[5])

T = []
k = []
sigma = []
gransigma = []


for CONTACT in range(IN_CONT, FIN_CONT):

	if os.path.isfile("%s/%06.d/RT%06.d_3908.root" % (OUTPATH_ROOT, CONTACT, CONTACT)) == False:
	   continue

	######################################################################
	# READ ROOT FILE FOR THE WHOLE CONTACT AND GET THE LIST OF TIMES t[] #
	######################################################################

	ROOTNAME  = ROOT.TFile.Open("%s/%06.d/RT%06.d_3908.root" % (OUTPATH_ROOT, CONTACT, CONTACT))
	TREENAME  = ROOTNAME.Get("tdata")
	CHAIN     = gDirectory.Get("tdata")
	N_ENTRIES = CHAIN.GetEntriesFast()

	# t is the time array with [photon time]

	t = []

	t_red = []

	EVENTS = []

	for atrg in ROOTNAME.tdata:
		t.append(float(atrg.time))

	############################
	# MATCH EVENTS WITHIN 5 us #
	############################

	j = 0

	while j < int(len(t)):
		if t[j]-t[j-1] <= 0.000005:
			t_red.append(float(t[j-1]))
			j += 1
		else:
			if t[j] > t[j-1]:
				t_red.append(float(t[j-1]))
		j += 1

	for k in range(1,len(t_red)):
		EVENTS.append(float(t_red[k]))

	#############################################################################
	# COUNT NUMBER OF TRIGGERS AND CREATE MATRIX WITH ALL TRIGGERS' LIGHTCURVES #
	#############################################################################

	# temporary processing for trigger evaluation

	# t2 is the 2D list with [photon time, waiting time between successive photons]
	# waiting time between successive photons is used to identify the blank spaces
	# between triggers inside the event list

	t2 = np.zeros((len(EVENTS), 2))

	for i in range(0,len(EVENTS)):
	    if abs(EVENTS[i]-EVENTS[i-1]) >= 0.5:
		FLAG=999999 # flag value to delimit each trigger window
	    else:
		FLAG=0
	    t2[i] = [EVENTS[i], FLAG]

	trg_t = [map(lambda a:a[0],list(v)) for k, v in itertools.groupby(t2, lambda x: x[1] == 999999) if not k]

	N_TRG = int(len(trg_t))

	##########################################
	# FOCALIZING ON EACH N-th SINGLE TRIGGER #
	##########################################

	for N in range(0,N_TRG):
	    
		l = len(trg_t[N])

		t_min = trg_t[N][0]
		t_max = trg_t[N][l-1]

		N_BIN = int(((t_max-t_min)/BIN))

		BS   = np.arange((int)(t_min)+(0*BIN/4.0), (int)(t_max)+(0*BIN/4.0)+2, BIN)
		BS8  = np.arange((int)(t_min)+(1*BIN/4.0), (int)(t_max)+(1*BIN/4.0)+2, BIN)
		BS12 = np.arange((int)(t_min)+(2*BIN/4.0), (int)(t_max)+(2*BIN/4.0)+2, BIN)
		BS16 = np.arange((int)(t_min)+(3*BIN/4.0), (int)(t_max)+(3*BIN/4.0)+2, BIN)

		LIGHTCURVE = np.zeros((N_BIN, 2))
		LIGHTCURVE8 = np.zeros((N_BIN, 2))
		LIGHTCURVE12 = np.zeros((N_BIN, 2))
		LIGHTCURVE16 = np.zeros((N_BIN, 2))

		hist, bin_edges = np.histogram(trg_t[N], bins=BS)
		hist8, bin_edges8 = np.histogram(trg_t[N], bins=BS8)
		hist12, bin_edges12 = np.histogram(trg_t[N], bins=BS12)
		hist16, bin_edges16 = np.histogram(trg_t[N], bins=BS16)

		for n in range(0,N_BIN):
			LIGHTCURVE[n]=[BS[n+1], hist[n]]
			LIGHTCURVE8[n]=[BS[n+1], hist8[n]]
			LIGHTCURVE12[n]=[BS[n+1], hist12[n]]
			LIGHTCURVE16[n]=[BS[n+1], hist16[n]]
		
		BKG = np.mean(LIGHTCURVE[ np.where( (LIGHTCURVE[:,1] != 0 ) ), 1 ])

		for n in range(0,N_BIN):

			#print "%9.6f\t%d\t%3.2f\t%3.2f\n" % (BS[n+1], hist[n], BKG, max(float((hist[n]-BKG)/math.sqrt(BKG)), float((hist8[n]-BKG)/math.sqrt(BKG)), float((hist12[n]-BKG)/math.sqrt(BKG)), float((hist16[n]-BKG)/math.sqrt(BKG))))

			sigma.append( max(float((hist[n]-BKG)/math.sqrt(BKG)), float((hist8[n]-BKG)/math.sqrt(BKG)), float((hist12[n]-BKG)/math.sqrt(BKG)), float((hist16[n]-BKG)/math.sqrt(BKG)) ))

			if max( float((hist[n]-BKG)/math.sqrt(BKG)), float((hist8[n]-BKG)/math.sqrt(BKG)), float((hist12[n]-BKG)/math.sqrt(BKG)), float((hist16[n]-BKG)/math.sqrt(BKG)) ) >= THR:

				gransigma.append( max(float((hist[n]-BKG)/math.sqrt(BKG)), float((hist8[n]-BKG)/math.sqrt(BKG)), float((hist12[n]-BKG)/math.sqrt(BKG)), float((hist16[n]-BKG)/math.sqrt(BKG)) ))



print "total number of %.3f s bins = %d" % (BIN, len(sigma))
print "total number of %.3f s bins with S/N >= %3.2f sigma = %d" % (BIN, THR, len(gransigma))
print "FAR = %3.9f" % float(float(len(gransigma))/float(len(sigma)))

#######
# END #
#######
