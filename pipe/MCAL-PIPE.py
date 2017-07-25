#####################################################
#                                                   #
#  #   #  ####  ###  #         ####  # ####  #####  #
#  ## ## #     #   # #         #   # # #   # #      #
#  # # # #     ##### #     ### ####  # ####  ###    #
#  #   # #     #   # #         #     # #     #      #
#  #   #  #### #   # #####     #     # #     #####  #
#                                                   #
#######################################################################################
#                                                                                     #
# $ python MCAL-PIPE.py [ CONTACT NUMBER ]                                            #
#			[ 3908 FITS FILE ABSOLUTE PATH ]                              #
#			[ 3916 FITS FILE ABSOLUTE PATH ]                              #
#			[ OUTPUTH DIRECTORY ABSOLUTE PATH ]                           #
#                                                                                     #
#######################################################################################
#                                                                                     #
# $ python MCAL-PIPE.py 50315                                                         #
#			/data01/ASDC_PROC2/DATA_2/COR/PKP050315_1_3908_000.lv1.cor.gz #
#			/data01/ASDC_PROC2/DATA_2/COR/PKP050315_1_3916_000.lv1.cor.gz #
#			/ANALYSIS3/AGILE-MCAL/050315                                  #
#                                                                                     #
#######################################################################################

import os, sys, math, os.path
from scipy.stats import poisson
import numpy as np
import matplotlib.pyplot as plt
import itertools
from itertools import groupby
import ROOT
from ROOT import TFile
from ROOT import gDirectory
import matplotlib.ticker as ticker
from mpl_toolkits.basemap import Basemap
import time

CONT = int(sys.argv[1])
INPATH_3908_FITS = str(sys.argv[2])
INPATH_3916_FITS = str(sys.argv[3])
OUTPATH_ROOT = str(sys.argv[4])
GRB_BIN = float(sys.argv[5])
GRB_SIGMA = float(sys.argv[6])
STE_SIGMA = float(sys.argv[7])

##########
# STEP 0 #
##########
####################################################
# CONVERSION OF 3908/3916 FITS FILES TO ROOT FILES #
####################################################

# creation of 3908 root file

print "\n---> STEP 0: CONVERSION OF 3908/3916 IN ROOT FILES"

os.system("mcalanalyzer %s" % INPATH_3908_FITS)

# move 3908 root file in the OUTPATH directory

os.system("mv /opt/prod/mcalsw-new/bin/mcalsw_output/RT/RT%06.d_3908.root %s" % (CONT, OUTPATH_ROOT))

# move the trigger list file into the OUTPUTH directory

os.system("mv grboutput.txt %s/%06.d_triggers.txt" % (OUTPATH_ROOT, CONT))

# creation of 3916 root file (already in the OUTPATH directory)

os.system("/home/rt/AGILE-MCAL/projects/convPKP3916toRoot/convPKP3916toRoot %s %s/RT%06.d_3916.root 1" % (INPATH_3916_FITS, OUTPATH_ROOT, CONT))

# create directories for TGFs, GRBs, STEs

os.system("mkdir %s/TGF" % OUTPATH_ROOT)
os.system("mkdir %s/GRB" % OUTPATH_ROOT)
os.system("mkdir %s/STE" % OUTPATH_ROOT)




##########
# STEP 1 #
##########
###################
# SEARCH FOR TGFs #
###################

# launch tgfsearch.C by MM to find TGFs and print them in a TGF text list

print "\n---> STEP 1: SEARCH FOR TGFs"

os.system("root -b -q '/home/rt/AGILE-MCAL/pipe/tgfsearch.C(%d, \"%s\", 0.001, 0.0003, 9)' | grep TGF > %s/TGF_%06.d_list.txt" % (CONT, OUTPATH_ROOT, OUTPATH_ROOT, CONT) )

# processing TGFs found by tgfsearch.C stored in TGF text list

TGF_TGF = []
TGF_CONTACT = []
TGF_T0 = []
TGF_COUNTS = []
TGF_HR = []
TGF_DT = []
TGF_EMAX = []
TGF_EAVG = []
TGF_LON = []
TGF_LAT = []
TGF_DATE = []
TGF_TIME = []
TGF_LC = []

TGF_BIN = 0.001
map = None

fo = open("%s/TGF_%06.d_list.txt" % (OUTPATH_ROOT, CONT), "r")

for line in fo:
	line = line.strip()
	columns = line.split()
	TGF_TGF.append(str(columns[0]))
	TGF_CONTACT.append(int(columns[1]))
	TGF_T0.append(float(columns[2]))
	TGF_DATE.append(str(columns[3]))
	TGF_TIME.append(str(columns[4]))
	TGF_LC.append(float(columns[5]))
        TGF_COUNTS.append(int(columns[6]))
        TGF_HR.append(float(columns[7]))
        TGF_DT.append(float(columns[8]))
        TGF_EMAX.append(float(columns[9]))
        TGF_EAVG.append(float(columns[10]))
        TGF_LON.append(float(columns[11]))
        TGF_LAT.append(float(columns[12]))

fo.close()

for i in range(0, len(TGF_CONTACT)):

	# read 3908 root file to get the times

	TGF_ROOTNAME  = ROOT.TFile.Open("%s/RT%06.d_3908.root" % (OUTPATH_ROOT, TGF_CONTACT[i]))
	TGF_TREENAME  = TGF_ROOTNAME.Get("tdata")
	TGF_CHAIN     = gDirectory.Get("tdata")
	TGF_N_ENTRIES = TGF_CHAIN.GetEntriesFast()

	# t is the time array with [photon time]

	TGF_t = []
	TGF_E = []

	for atrg in TGF_ROOTNAME.tdata:
		if float(atrg.time-TGF_T0[i]) <= 0.03 and float(atrg.time-TGF_T0[i]) >= -0.03:
			TGF_t.append(float(atrg.time-TGF_T0[i]))
        	        TGF_E.append(float(atrg.Etot))

	# re-binning and creation of TGF lightcurve

	TGF_l = len(TGF_t)

	TGF_t_min = float(TGF_t[0])-1
	TGF_t_max = float(TGF_t[0])+1

	TGF_N_BIN = int(((TGF_t_max-TGF_t_min)/TGF_BIN))

	TGF_BS = np.arange((float)(TGF_t_min), (float)(TGF_t_max)+2, TGF_BIN)

	TGF_LIGHTCURVE = np.zeros((TGF_N_BIN, 2))

	TGF_hist, TGF_bin_edges = np.histogram(TGF_t, bins=TGF_BS)

	for TGF_n in range(0,TGF_N_BIN-1):
		TGF_LIGHTCURVE[TGF_n]=[TGF_BS[TGF_n+1], TGF_hist[TGF_n]]

	fig = plt.figure(figsize=(18,12))
	axTGF1 = fig.add_subplot(211)
	axTGF1.step(TGF_LIGHTCURVE[:,0], TGF_LIGHTCURVE[:,1], markersize=5, c='black')
	plt.setp(axTGF1.get_xticklabels(), fontsize=16)
	plt.setp(axTGF1.get_yticklabels(), fontsize=16)
	axTGF1.set_xlim(-0.015,0.015)
	axTGF1.set_ylim(0,max(TGF_hist)+1)
	axTGF1.set_ylabel('[counts / %d ms]\n0.35 - 100 MeV\n' % (TGF_BIN*1000.), fontsize=20)
	axTGF1.set_title('TGF - CONTACT %06.d - T0 %f\n' % (TGF_CONTACT[i], TGF_T0[i]), fontsize=22)

	# plot TGF lightcurve and energy scatter plot

	TGF_ENERGIES = np.zeros((len(TGF_t), 2))

	for j in range(0,len(TGF_t)-1):
		TGF_ENERGIES[j] = [TGF_t[j], TGF_E[j]]

	axTGF2 = fig.add_subplot(212)
	axTGF2.scatter(TGF_ENERGIES[:,0], TGF_ENERGIES[:,1])
	plt.setp(axTGF2.get_xticklabels(), fontsize=16)
	plt.setp(axTGF2.get_yticklabels(), fontsize=16)
	axTGF2.set_ylim(0,40)
	axTGF2.set_xlim(-0.015,0.015)
	axTGF2.set_xlabel('\ntime - %s [s]' % TGF_T0[i], fontsize=20)
	axTGF2.set_ylabel('E [MeV]\n', fontsize=20)
	plt.savefig('%s/TGF/TGF_%06.d_%f.png' % (OUTPATH_ROOT, TGF_CONTACT[i], TGF_T0[i]) )

	# plot TGF position on a world map

	fig = plt.figure(figsize=(18,6))
	axTGF3 = fig.add_subplot(111)
	map = Basemap(projection='cyl', lat_0 = 57, lon_0 = -135, resolution = 'c', area_thresh = 0.1, llcrnrlon=-180.0, llcrnrlat=-20.0, urcrnrlon=180.0, urcrnrlat=20.0)
	map.drawcoastlines()
	map.drawcountries()
	map.drawparallels(np.arange(-90,90,15.0),labels=[1,0,0,1])
	map.drawmeridians(np.arange(-180,180,45),labels=[1,1,0,1])
	#map.fillcontinents(color = 'lightsteelblue')
	map.drawmapboundary(fill_color='white')
	X, Y = map(TGF_LON[i], TGF_LAT[i])
	map.plot(X, Y, 'ro')

	plt.savefig('%s/TGF/TGF_%06.d_%f_geo.png' % (OUTPATH_ROOT, TGF_CONTACT[i], TGF_T0[i]) )

# delete TGF list if there are no TGFs

if (os.stat("%s/TGF_%06.d_list.txt" % (OUTPATH_ROOT, CONT)).st_size == 0):
	os.system("rm %s/TGF_%06.d_list.txt" % (OUTPATH_ROOT, CONT))
else:
	os.system("mv %s/TGF_%06.d_list.txt %s/TGF/" % (OUTPATH_ROOT, CONT, OUTPATH_ROOT))




##########
# STEP 2 #
##########
###################
# SEARCH FOR GRBs #
###################

# read 3908 root file to get the times

print "\n---> STEP 2: SEARCH FOR GRBs"

GRB_ROOTNAME  = ROOT.TFile.Open("%s/RT%06.d_3908.root" % (OUTPATH_ROOT, CONT))
GRB_TREENAME  = GRB_ROOTNAME.Get("tdata")
GRB_CHAIN     = gDirectory.Get("tdata")
GRB_N_ENTRIES = GRB_CHAIN.GetEntriesFast()

GRB_t = []
GRB_t_red = []

for atrg in GRB_ROOTNAME.tdata:
   GRB_t.append(float(atrg.time))

# merge events within 4 microseconds

q = 0

while q < int(len(GRB_t)):
	if GRB_t[q]-GRB_t[q-1] <= 0.000005:
		GRB_t_red.append(float(GRB_t[q-1]))
		q += 1
	else:
		if GRB_t[q] > GRB_t[q-1]:
			GRB_t_red.append(float(GRB_t[q-1]))
	q += 1

# plot event list in a contact to check monotonicity

fig = plt.figure()
ax = fig.add_subplot(111)
plt.plot(GRB_t, markersize=5, c='b')
ax.set_ylabel('time (s)')
plt.savefig("%s/%06.d_trend.png" % (OUTPATH_ROOT, CONT))

# count number of triggers within a contact and create matrix with all triggers' lightcurves

# temporary processing for trigger evaluation:
# t2 is the 2D list with [photon time, waiting time between successive photons]
# waiting time between successive photons is used to identify the blank spaces
# between triggers inside the event list

GRB_t2 = np.zeros((len(GRB_t_red), 2))

for z in range(0,len(GRB_t_red)):
    if (abs)(GRB_t_red[z]-GRB_t_red[z-1]) >= 0.5:
	FLAG=999999 # flag value to delimit each trigger window
    else:
	FLAG=0
    GRB_t2[z] = [GRB_t_red[z], FLAG]

del map

GRB_trg_t = [map(lambda a:a[0],list(v)) for k, v in itertools.groupby(GRB_t2, lambda x: x[1] == 999999) if not k]

GRB_N_TRG = int(len(GRB_trg_t))

# focusing on each N-th trigger

GRB_ft = open("%s/GRB_%06.d_list.txt" % (OUTPATH_ROOT, CONT), "w")

GRB_P = np.zeros((GRB_N_TRG, 2))

for N in range(0,GRB_N_TRG):

	GRB_l = len(GRB_trg_t[N])

	GRB_t_min = float(GRB_trg_t[N][0])-1
	GRB_t_max = float(GRB_trg_t[N][GRB_l-1])+1

	GRB_N_BIN = int(((GRB_t_max-GRB_t_min)/GRB_BIN))

	GRB_BS = np.arange((float)(GRB_t_min), (float)(GRB_t_max)+2, GRB_BIN)

	GRB_LIGHTCURVE = np.zeros((GRB_N_BIN, 2))

	GRB_hist, GRB_bin_edges = np.histogram(GRB_trg_t[N], bins=GRB_BS)

	for m in range(0,GRB_N_BIN):
		GRB_LIGHTCURVE[m]=[GRB_BS[m+1], GRB_hist[m]]

	GRB_BKG = np.mean(GRB_LIGHTCURVE[ np.where( (GRB_LIGHTCURVE[:,1] != 0 ) ), 1 ])
	GRB_THR = GRB_BKG + GRB_SIGMA*math.sqrt(GRB_BKG)

	# number of triggered events for a given threshold

	result = [tuple(g) for u, g in groupby(GRB_LIGHTCURVE, lambda x: x[1] > GRB_THR) if u]

	if len(result) <= 20:

	    for r in range(0,GRB_N_BIN):

		if GRB_LIGHTCURVE[r][1] >= GRB_THR:
			GRB_UTC = str(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(1072915200+GRB_LIGHTCURVE[r][0])))

			GRB_ft.write("GRB %06.d\t %9.6f\t %s\t %3.3f\t %3.2f\n" % (CONT, GRB_LIGHTCURVE[r][0], GRB_UTC, GRB_BIN, GRB_BKG))

			os.system("python /home/rt/AGILE-MCAL/pipe/LCPLOT.py %d %s %f %f %f %3.2f" % (CONT, OUTPATH_ROOT, GRB_LIGHTCURVE[r][0], (GRB_LIGHTCURVE[r][0]+1-GRB_trg_t[N][0]), (GRB_trg_t[N][GRB_l-1]+1-GRB_LIGHTCURVE[r][0]), GRB_BIN))

GRB_ft.close()

# delete GRB list if there are no GRBs

if (os.stat("%s/GRB_%06.d_list.txt" % (OUTPATH_ROOT, CONT)).st_size == 0):
	os.system("rm %s/GRB_%06.d_list.txt" % (OUTPATH_ROOT, CONT))
else:
	os.system("mv %s/GRB_%06.d_list.txt %s/GRB/" % (OUTPATH_ROOT, CONT, OUTPATH_ROOT) )




##########
# STEP 3 #
##########
###################
# SEARCH FOR STEs #
###################

print "\n---> STEP 3: SEARCH FOR STEs"

STE_ft = open("%s/STE_%06.d_list.txt" % (OUTPATH_ROOT, CONT), "w")

for STE_BIN in (0.016, 0.032, 0.064, 0.128):

	# read 3908 root file to get the times

	STE_ROOTNAME  = ROOT.TFile.Open("%s/RT%06.d_3908.root" % (OUTPATH_ROOT, CONT))
	STE_TREENAME  = STE_ROOTNAME.Get("tdata")
	STE_CHAIN     = gDirectory.Get("tdata")
	STE_N_ENTRIES = STE_CHAIN.GetEntriesFast()

	STE_t = []
	STE_t_red = []

	for atrg in STE_ROOTNAME.tdata:
	    STE_t.append(float(atrg.time))

	# merge events within 4 microseconds

	j = 0

	while j < int(len(STE_t)):
		if STE_t[j]-STE_t[j-1] <= 0.000005:
			STE_t_red.append(float(STE_t[j-1]))
			j += 1
		else:
			if STE_t[j] > STE_t[j-1]:
				STE_t_red.append(float(STE_t[j-1]))
		j += 1

	# count number of triggers within a contact and create matrix with all triggers' lightcurves

	# temporary processing for trigger evaluation:
	# t2 is the 2D list with [photon time, waiting time between successive photons]
	# waiting time between successive photons is used to identify the blank spaces
	# between triggers inside the event list

	STE_t2 = np.zeros((len(STE_t_red), 2))

	for i in range(0,len(STE_t_red)):
	    if (abs)(STE_t_red[i]-STE_t_red[i-1]) >= 0.5:
		FLAG=999999 # flag value to delimit each trigger window
	    else:
		FLAG=0
	    STE_t2[i] = [STE_t_red[i], FLAG]

	STE_trg_t = [map(lambda a:a[0],list(v)) for k, v in itertools.groupby(STE_t2, lambda x: x[1] == 999999) if not k]

	STE_N_TRG = int(len(STE_trg_t))

	# focusing on each M-th trigger

	STE_P = np.zeros((STE_N_TRG, 2))

	for M in range(0,STE_N_TRG):

		STE_l = len(STE_trg_t[M])

		STE_t_min = float(STE_trg_t[M][0])-1
		STE_t_max = float(STE_trg_t[M][STE_l-1])+1

		STE_N_BIN = int(((STE_t_max-STE_t_min)/STE_BIN))

		STE_BS = np.arange((float)(STE_t_min), (float)(STE_t_max)+2, STE_BIN)

		STE_LIGHTCURVE = np.zeros((STE_N_BIN, 2))

		STE_hist, STE_bin_edges = np.histogram(STE_trg_t[M], bins=STE_BS)

		for c in range(0,STE_N_BIN):
			STE_LIGHTCURVE[c]=[STE_BS[c+1], STE_hist[c]]

		STE_BKG = np.mean(STE_LIGHTCURVE[ np.where( (STE_LIGHTCURVE[:,1] != 0 ) ), 1 ])
		STE_THR = STE_BKG + 4.0*math.sqrt(STE_BKG)

		# NUMBER OF TRIGGERED TRIGGERS FOR A GIVEN AU LOGIC

		STE_result = [tuple(g) for k, g in groupby(STE_LIGHTCURVE, lambda x: x[1] > STE_THR) if k]

		if len(STE_result) <= 20:

		    for h in range(0,STE_N_BIN):

			if STE_LIGHTCURVE[h][1] >= STE_THR:
				STE_UTC = str(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(1072915200+STE_LIGHTCURVE[h][0])))

				STE_ft.write("STE %06.d \t %9.6f\t %s\t %3.3f\t %3.2f\t %3.2f\t %d\t %3.2f\n" % (CONT, STE_LIGHTCURVE[h][0], STE_UTC, STE_BIN, STE_BKG, STE_THR, STE_LIGHTCURVE[h][1], float((STE_LIGHTCURVE[h][1]-STE_BKG)/math.sqrt(STE_BKG))))

				fig = plt.figure(figsize=(18,6))
				ax = fig.add_subplot(111)
				ax.axhline(y=STE_THR, linewidth=2, linestyle = 'dashed', c='black', alpha=0.2)
				ax.axhline(y=STE_BKG, linewidth=1, c='red', alpha=0.5)
				ax.axvline(x=STE_LIGHTCURVE[h][0], linewidth=12, color = 'magenta', alpha = 0.2)
				ax.step(STE_LIGHTCURVE[:,0], STE_LIGHTCURVE[:,1], markersize=5, c='b')
				ax.set_xlim(STE_trg_t[M][0]-1,STE_trg_t[M][STE_l-1]+1)
				ax.set_xlabel('time [s]', fontsize=18)
				plt.setp(ax.get_xticklabels(), fontsize=16)
				ax.set_ylim(0,max(STE_hist)+10)
				ax.set_ylabel('counts / %3.3f s\n' % STE_BIN, fontsize=18)
				plt.setp(ax.get_yticklabels(), fontsize=16)
				ax.set_title('CONT %06.d    TRG %03.d    T0 %9.6f    CTS %d    BKG %3.2f    S/N %3.2f\n' \
				% (CONT, M+1, STE_LIGHTCURVE[h][0], STE_LIGHTCURVE[h][1], STE_BKG, (STE_LIGHTCURVE[h][1]-STE_BKG)/math.sqrt(STE_BKG)), fontsize=18)
				plt.savefig('%s/STE/STE_%06.d_%9.6f_%1.3f.png' \
				% (OUTPATH_ROOT, CONT, STE_LIGHTCURVE[h][0], STE_BIN))

STE_ft.close()

# delete STE list if there are no STEs

if (os.stat("%s/STE_%06.d_list.txt" % (OUTPATH_ROOT, CONT)).st_size == 0):
	os.system("rm %s/STE_%06.d_list.txt" % (OUTPATH_ROOT, CONT))
else:
	os.system("mv %s/STE_%06.d_list.txt %s/STE/" % (OUTPATH_ROOT, CONT, OUTPATH_ROOT))




print "\n---> FINISHED"

#######
# END #
#######
