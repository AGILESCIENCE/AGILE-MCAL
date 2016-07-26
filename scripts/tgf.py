# MM 08/04/2015 to do TGF analysis at once
# runs with Python 2.3.4 (#1, Oct 13 2006, 15:52:35) VERY OLD VERSION!!! lots of commands are deprecated

# MM 18/05/2015 replace nan in processed trigger list; produce graphics

import sys
import os

last_contact_triggers = 41954
final_contact = 42153

# file & path list

file_3916_index = "/home/mcal/data_analysis/AGILE_coordinates/3916.index"
file_mcal_trigger_archive = "/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive"
path_root_macros = "/home/mcal/BUILD_MCALSW.BUILD15-devel/root_macros/"
path_mcal_lv1_data = "/AGILE_PROC3/DATA/COR/"
path_mcal_proc_data = "/MCAL_PROC/ARCHIVE/"
file_tmp = "./tmp.log"
file_trg = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/trg.dat"
file_trg_proc = "/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/trg_proc.dat"
file_trg_root = "/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_proc.root"
file_tgf_root = "/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf.root"

# ----- step1: update 3916 repository -----

os.system("tail -n 1 " + file_3916_index + " > " + file_tmp)
tmp_file_3916 = open(file_tmp, 'r')
for line in tmp_file_3916:
    line = line.strip()
    columns = line.split()
    last_contact_3916 = int(columns[1])
tmp_file_3916.close()
    
os.system(" root -b -q '"+path_root_macros+"conv3916toRoot_2.C(" + str(last_contact_3916+1) + "," + str(final_contact)+")' > "+file_tmp) 
os.system("grep INDEX "+file_tmp+" >> "+file_3916_index)

# ----- step2: update /AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive -----

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

os.system("rm "+file_tmp)
for contact in range(last_contact_triggers + 1, final_contact+1):
	os.system("root -b -q '"+path_root_macros+"burst_search_2.C(\""+path_mcal_proc_data+"0"+str(contact)+"/RT0"+str(contact)+"_3908.root\")' >> "+file_tmp) 

os.system("grep TRG "+file_tmp+" > "+file_trg)


os.system("root -b -q '"+path_root_macros+"add_geographic_info_4.C(\""+file_trg+"\", \""+file_trg_proc+"\")' >> "+file_tmp) 

# replace nan in processed trigger list
os.system("replace nan -999 -- "+file_trg_proc)

os.system("root -b -q '"+path_root_macros+"plot_triggers_6.C(\""+file_trg_proc+"\", \""+file_trg_root+"\")' >> "+file_tmp)
os.system("root -b -q '"+path_root_macros+"plot_triggers_7.C(\""+file_trg_root+"\", \""+file_tgf_root+"\")' >> "+file_tmp)
os.system("root -b -q '"+path_root_macros+"compare_tgf_newconf_2.C("+str(last_contact_triggers)+")'  >> "+file_tmp)

