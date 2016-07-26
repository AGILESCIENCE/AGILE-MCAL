# ***** tgf3.py *****

# MM 28/07/2015 based on tgf2.py, workaround for position data loss after PPSE failure on Jul 1st 2015

# MM 08/04/2015 to do TGF analysis at once
# runs with Python 2.3.4 (#1, Oct 13 2006, 15:52:35) VERY OLD VERSION!!! lots of commands are deprecated

# MM 18/05/2015 replace nan in processed trigger list; produce graphics
# MM 25/06/2015 write ascii TGF dataset and copy to the online repository on gtb

import sys
import os


# file & path list

file_last_contact = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/last_contact.dat"
#file_3916_index = "/home/mcal/data_analysis/AGILE_coordinates/3916.index"
file_mcal_trigger_archive = "/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive"
file_mcal_trigger = "/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat"
#file_boost = "/AGILE_PROC3/DATA/HLT/MCALBURST/BOOST_up_to_now.dat"
path_root_macros = "/home/mcal/BUILD_MCALSW.BUILD15-devel/root_macros/"
path_mcal_lv1_data = "/AGILE_PROC3/DATA/COR/"
path_mcal_proc_data = "/MCAL_PROC/ARCHIVE/"
file_tmp = "./tmp.log"

# add suffix '2' to trigger data
file_trg = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/trg2.dat"
file_trg_proc = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/trg_proc2.dat"
file_trg_root = "/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_proc2.root"
file_tgf_root = "/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf2.root"


# ----- step0: get final contacts for analysis -----
# use mcal_grb.dat (linked to 3908 data processing) instead of BOOST_up_to_now.dat (unavailable since failure, boost rejection maybe not critical after threshold change)

os.system("tail -n 1 "+file_mcal_trigger+" > "+file_tmp)
f = open(file_tmp, 'r')
for line in f:
    line = line.strip()
    columns = line.split()
    final_contact = int(float(columns[0]))
f.close()


# ----- step1: update 3916 repository -----
# useless since 3916 do not contain valid position data after 1st Jul 2015
'''
os.system("tail -n 1 " + file_3916_index + " > " + file_tmp)
tmp_file_3916 = open(file_tmp, 'r')
for line in tmp_file_3916:
    line = line.strip()
    columns = line.split()
    last_contact_3916 = int(columns[1])
tmp_file_3916.close()
    
os.system(" root -b -q '"+path_root_macros+"conv3916toRoot_2.C(" + str(last_contact_3916+1) + "," + str(final_contact)+")' > "+file_tmp) 
os.system("grep INDEX "+file_tmp+" >> "+file_3916_index)
'''

# ----- step2: update /AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive -----
# keep this step, even if all triggers will be marked as UNK (no BOOST info available)

os.system("rm tmp_grb.dat")
os.system("tail -n 1 "+file_mcal_trigger_archive+" > "+file_tmp)
f = open(file_tmp, 'r')
for line in f:
    line = line.strip()
    columns = line.split()
    last_contact_triggers = int(columns[0])
f.close()
    
for contact in range(int(columns[0]) + 1, final_contact+1):
	os.system("mcalanalyzer "+path_mcal_lv1_data+"0"+str(contact)+"/PKP0"+str(contact)+"_1_3908_000.cor.gz -grb tmp_grb.dat -noproc")
os.system("more tmp_grb.dat >> "+file_mcal_trigger_archive)
os.system("rm tmp_grb.dat")


# ----- step3: apply burst search algorithm #2 -----

print "tgf2.py: performing TGF analysis in contact range:", last_contact_triggers+1, final_contact

os.system("rm "+file_tmp)
for contact in range(last_contact_triggers + 1, final_contact + 1):
	os.system("root -b -q '"+path_root_macros+"burst_search_2.C(\""+path_mcal_proc_data+"0"+str(contact)+"/RT0"+str(contact)+"_3908.root\")' >> "+file_tmp) 

os.system("grep TRG "+file_tmp+" > "+file_trg)


# use fake macro add_geographic_info_5.C (creates the data structure and check proximity with onboard triggers only)
os.system("root -b -q '"+path_root_macros+"add_geographic_info_5.C(\""+file_trg+"\", \""+file_trg_proc+"\")' >> "+file_tmp) 

# replace nan in processed trigger list
os.system("replace nan -999 -- "+file_trg_proc)


os.system("root -b -q '"+path_root_macros+"plot_triggers_6.C(\""+file_trg_proc+"\", \""+file_trg_root+"\")' >> "+file_tmp)

os.system("root -b -q '"+path_root_macros+"plot_triggers_8.C(\""+file_trg_root+"\", \""+file_tgf_root+"\")' >> "+file_tmp)

'''
os.system("root -b -q '"+path_root_macros+"compare_tgf_newconf_2.C("+str(last_contact_triggers)+")'  >> "+file_tmp)
os.system("cp /home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf.dat /AGILE_MCAL/")
'''


