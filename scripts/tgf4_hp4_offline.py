# ***** tgf4_offline.py *****

# MM 08/12/2015 path modified for running on agilehp4
# MM 17/11/2015 apply tgf search on selected contact range for offline data analysis (based on the online version tgf4.py)

# --------------------

# MM 04/11/2015 produce a trigger list with only longitude selection onCentral America for correlation with Steve Cummer's data
# MM 09/10/2015 @Duke. Based on tgf3.py, use again position info available in 3916 files since contact 43616 (28/09/2015)

# MM 28/07/2015 based on tgf2.py, workaround for position data loss after PPSE failure on Jul 1st 2015

# MM 08/04/2015 to do TGF analysis at once
# runs with Python 2.3.4 (#1, Oct 13 2006, 15:52:35) VERY OLD VERSION!!! lots of commands are deprecated

# MM 18/05/2015 replace nan in processed trigger list; produce graphics
# MM 25/06/2015 write ascii TGF dataset and copy to the online repository on gtb

import sys
import os

# flags to activate / deactivate processing

process_3916 = 0
process_3908 = 0
tgf_search   = 1 
add_geo_info = 0 

# file & path list

#file_last_contact = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/last_contact.dat"
#file_3916_index = "/home/mcal/data_analysis/AGILE_coordinates/3916.index"
#file_mcal_trigger_archive = "/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive"
#file_mcal_trigger = "/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat"
#file_boost = "/AGILE_PROC3/DATA/HLT/MCALBURST/BOOST_up_to_now.dat"
path_root_macros = "/home/agileuser/BUILD_MCALSW/root_macros/"
path_mcal_lv1_data = "/storage1/agile/agile2/LV1corr/"
path_mcal_proc_data = "/home/agileuser/MCAL_PROC/ARCHIVE/"
file_tmp = "./tmp.log"
#path_mcal_proc_data = "/home/mcal/data_analysis/MCAL_PROC2/ARCHIVE/"

file_trg = "/home/agileuser/BUILD_MCALSW/scripts/trg4_" + sys.argv[1] + "-" + sys.argv[2] +".dat"
file_trg_proc = "/home/agileuser/BUILD_MCALSW/scripts/trg_proc4_" + sys.argv[1] + "-" + sys.argv[2] +".dat"
file_trg_root = "/home/agileuser/BUILD_MCALSW/scripts/trg_proc4_" + sys.argv[1] + "-" + sys.argv[2] +".root"
file_tgf_root = "/home/agileuser/BUILD_MCALSW/scripts/LE_TGF_newconf4_" + sys.argv[1] + "-" + sys.argv[2] +".root"

# ----- update 3916 repository -----

if process_3916 == 1 :
	os.system(" root -b -q '"+path_root_macros+"conv3916toRoot_2.C(" + sys.argv[1] + "," + sys.argv[2] +", 1)' > "+file_tmp) 

# ----- update local MCAL root archive ----- (NB: workaround until new MCALSW version installed on hm_17@gtb)

if process_3908 == 1 :
	os.system("ruby mcal_make_archive.rb " + sys.argv[1] + "  " + sys.argv[2])

# ----- apply burst search algorithm #2 -----

if tgf_search == 1 :
	os.system("rm "+file_tmp)
	for contact in range(int(sys.argv[1]), int(sys.argv[2])) : 
		os.system("root -b -q '"+path_root_macros+"burst_search_2.C(\""+path_mcal_proc_data+"0"+str(contact)+"/RT0"+str(contact)+"_3908.root\")' >> "+file_tmp) 
	os.system("grep TRG "+file_tmp+" > "+file_trg)

if add_geo_info == 1 :
	os.system("root -b -q '"+path_root_macros+"add_geographic_info_4.C(\""+file_trg+"\", \""+file_trg_proc+"\")' >> "+file_tmp) 

	# replace nan in processed trigger list
	os.system("replace nan -999 -- "+file_trg_proc)
	os.system("root -b -q '"+path_root_macros+"plot_triggers_6.C(\""+file_trg_proc+"\", \""+file_trg_root+"\")' >> "+file_tmp)
	os.system("root -b -q '"+path_root_macros+"plot_triggers_9.C(\""+file_trg_root+"\", \""+file_tgf_root+"\")' >> "+file_tmp)


