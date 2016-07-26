#############################################################################
# Makefile for building: MCAL SW
# Project:  MCAL SW
# Template: system
# Use make variable_name=' options ' to override the variables or make -e to
# override the file variables with the environment variables
# 		make CFLAGS='-g' or make prefix='/usr'
# Instructions:
# - if you want, modify the sections 1) and 2) but it is not necessary
# - modify the variables of the section 3): CFLAGS
# - in section 4), modify the following action:
#		* checkout: modify the project and option -r
#			cvs checkout -r TAG_NAME prj_name
#		* all: and or remove exe and lib prerequisite
#		* clean: add or remove the files and directories that should be cleaned
#		* install: add or remove the files and directories that should be installed
#		* uninstall: add or remove the files and directories that should be uninstalled
#############################################################################

SHELL = /bin/sh

####### 0) Common definition
# DISCOS Type is the different type of DISCoS operating mode
# EGSE_DISCOS is for the DISCoS at EGSE level
DISCOS_TYPE = EGSE_DISCOS
# EGSE_TE is for the DISCoS at TE level
#DISCOS_TYPE = TE_DISCOS
####### 1) Directories for the installation

# Prefix for each installed program. Only ABSOLUTE PATH
#prefix=$(HOME)/local
#prefix=$(HOME)/mcalsw
prefix=$(MCALSW)
exec_prefix=$(prefix)
# The directory to install the binary files in.
bindir=$(exec_prefix)/bin
# The directory to install the local configuration file.
datadir=$(exec_prefix)/share
# The directory to install the libraries in.
libdir=$(exec_prefix)/lib
# The directory to install the info files in.
infodir=$(exec_prefix)/info
# The directory to install the include files in.
includedir=$(exec_prefix)/include

# Directories specific for MCAL SW

# The directory to install the root macros.
rootdir=$(exec_prefix)/root_macros
# The directory to install the shell scripts.
#scriptdir=$(exec_prefix)/scripts
scriptdir=$(exec_prefix)/bin
# Prefix for the data produced by the software
filedir=$(exec_prefix)/bin/mcalsw_output
# Path to grid offset data
gridoffdir=$(filedir)/grid_offset
# Path to count rate data
countratedir=$(filedir)/count_rate
# Path to scientific ratemeters data
scirmdir=$(filedir)/sci_RM
# Path to root files containing data
rootdatadir=$(filedir)/RT
# Path to root files containing histograms
roothistodir=$(filedir)/H
# Path to electrical calibration data
elcaldir=$(filedir)/elcal
# Path to miscellaneaous files
miscdir=$(filedir)/misc

####### 2) Directories
PRJ_DIR=projects

GTB_COMMON_INCLUDE=$(includedir)
GTB_DISCOS_LIB=$(libdir)
GTB_PACKETLIB_INCLUDE=$(includedir)
GTB_PROCESSORLIB_INCLUDE=$(includedir)
GTB_PACKETLIB_LIB=$(libdir)
GTB_PROCESSORLIB_LIB=$(libdir)
GTB_LIBQL2_INCLUDE=$(includedir)
GTB_LIBQL2_LIB=$(libdir)

####### 3) Compiler, tools and options

#Insert the optional parameter to the compiler. The CFLAGS could be changed externally by the user
CFLAGS   =
AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -fr
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p
CVS = cvs checkout

####### 4) Build rules
install: 
	test -d $(exec_prefix) || $(MKDIR) $(exec_prefix)
	test -d $(datadir) || $(MKDIR) $(datadir)
	#test -d $(rootdir) || $(MKDIR) $(rootdir)
	test -d $(scriptdir) || $(MKDIR) $(scriptdir)
	test -d $(filedir) || $(MKDIR) $(filedir)
	test -d $(gridoffdir) || $(MKDIR) $(gridoffdir)
	test -d $(countratedir) || $(MKDIR) $(countratedir)
	test -d $(scirmdir) || $(MKDIR) $(scirmdir)
	test -d $(rootdatadir) || $(MKDIR) $(rootdatadir)
	test -d $(roothistodir) || $(MKDIR) $(roothistodir)
	test -d $(miscdir) || $(MKDIR) $(miscdir)
	#$(COPY) ./root_macros/* $(rootdir)/
	$(COPY) ./scripts/* $(scriptdir)/
	#$(COPY_DIR) ./share $(datadir)/..
	cd ./$(PRJ_DIR)/MCVLib			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalfits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/viewrun			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalanalyzer		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalselect		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/hkmonitor		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/hkviewer		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalgrid		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalimaging		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/convPKP3908toRoot	&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/viewbsr			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/viewbsr_ric		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalibration		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/plotmcaldata		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/plotgrb			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mergedataset		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/root2fits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/mcalcor2fits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/makespectra		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/process3908		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	cd ./$(PRJ_DIR)/scirm_analysis		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix) install
	test -d $(prefix)/burstsearch || mkdir -p $(prefix)/burstsearch
	cd ./$(PRJ_DIR)/burstsearch             &&  $(COPY_FILE) * $(prefix)/burstsearch	
	test -d $(prefix)/boost || mkdir -p $(prefix)/boost
	cd ./$(PRJ_DIR)/boost             &&  $(COPY_FILE) * $(prefix)/boost	
	#test -d $(HOME)/.local || mkdir -p $(HOME)/.local
	#$(COPY_DIR) ./icons/share $(HOME)/.local
	#test -d $(HOME)/.config/menus || mkdir -p $(HOME)/.config/menus
	#$(COPY_FILE) ./icons/applications-kmenuedit.menu $(HOME)/.config/menus

checkout:
	test -d $(PRJ_DIR) || mkdir -p $(PRJ_DIR)
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_14 MCVLib
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_0_2 mcalfits
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_0 viewrun
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_7 mcalanalyzer
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_2 mcalselect
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_2_0 mcalgrid
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_0 mcalimaging
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_6 hkmonitor
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_2_3 hkviewer
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_0 convPKP3908toRoot
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_0 viewbsr
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_1 viewbsr_ric
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_0 mcalibration	
	cd ./$(PRJ_DIR) 	&&	$(CVS)  -r V_1_1_5 plotmcaldata
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_0 burstsearch
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_1_2 plotgrb
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_3 mergedataset
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_1_0 root2fits
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_5 boost
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_1 mcalcor2fits
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_0 makespectra
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_0 process3908
	cd ./$(PRJ_DIR)         &&      $(CVS)  -r V_1_0_0 scirm_analysis

checkout_last:
	test -d $(PRJ_DIR) || mkdir -p $(PRJ_DIR)
	cd ./$(PRJ_DIR) 	&&	$(CVS)  MCVLib
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalfits
	cd ./$(PRJ_DIR) 	&&	$(CVS)  viewrun
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalanalyzer
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalselect
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalgrid
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalimaging
	cd ./$(PRJ_DIR) 	&&	$(CVS)  hkmonitor
	cd ./$(PRJ_DIR) 	&&	$(CVS)  hkviewer
	cd ./$(PRJ_DIR) 	&&	$(CVS)  convPKP3908toRoot
	cd ./$(PRJ_DIR) 	&&	$(CVS)  viewbsr
	cd ./$(PRJ_DIR) 	&&	$(CVS)  viewbsr_ric
	cd ./$(PRJ_DIR) 	&&	$(CVS)  mcalibration
	cd ./$(PRJ_DIR) 	&&	$(CVS)  plotmcaldata
	cd ./$(PRJ_DIR)         &&      $(CVS)  burstsearch
	cd ./$(PRJ_DIR)         &&      $(CVS)  plotgrb
	cd ./$(PRJ_DIR)         &&      $(CVS)  mergedataset
	cd ./$(PRJ_DIR)         &&      $(CVS)  root2fits
	cd ./$(PRJ_DIR)         &&      $(CVS)  boost
	cd ./$(PRJ_DIR)         &&      $(CVS)  mcalcor2fits
	cd ./$(PRJ_DIR)         &&      $(CVS)  makespectra
	cd ./$(PRJ_DIR)         &&      $(CVS)  process3908
	cd ./$(PRJ_DIR)         &&      $(CVS)  scirm_analysis

#clean: delete all files from the current directory that are normally created by building the program.
clean:
	cd $(PRJ_DIR)/MCVLib 			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalfits 			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/viewrun			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalanalyzer		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalselect		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/hkmonitor			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/hkviewer			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalgrid			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalimaging		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/convPKP3908toRoot		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/viewbsr			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/viewbsr_ric		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalibration		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/plotmcaldata		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/plotgrb			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mergedataset		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/root2fits			&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/mcalcor2fits		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/makespectra		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/process3908		&& 	$(MAKE) prefix=$(prefix) clean
	cd $(PRJ_DIR)/scirm_analysis		&& 	$(MAKE) prefix=$(prefix) clean

#Delete all files from the current directory that are created by checkout, configuring or building the program.
distclean:
	test $(PRJ_DIR) = . || $(DEL_DIR) $(PRJ_DIR)

#install: compile the program and copy the executables, libraries,
#and so on to the file names where they should reside for actual use.
all:
	cd ./$(PRJ_DIR)/MCVLib			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalfits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/viewrun			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalanalyzer		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalselect		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/hkmonitor		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/hkviewer		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalgrid		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalimaging		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/convPKP3908toRoot	&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/viewbsr			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/viewbsr_ric		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/plotmcaldata		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/plotgrb			&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mergedataset		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/root2fits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/mcalcor2fits		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/makespectra		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/process3908		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)
	cd ./$(PRJ_DIR)/scirm_analysis		&& 	$(MAKE) CFLAGS=$(CFLAGS) DEBUGMSG= prefix=$(prefix)

#uninstall: delete all the installed files--the copies that the `install' target creates.
uninstall:
	cd $(PRJ_DIR)/MCVLib			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalfits			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/convPKPtoRoot		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/convPKPtoTE		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalview2			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/shiftdataset		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/viewrun			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalanalyzer		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalselect		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/hkmonitor			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/hkviewer			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalparam			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalgrid			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalimaging		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/convPKP3908toRoot		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/viewbsr			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/viewbsr_ric		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/plotmcaldata		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/plotgrb			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mergedataset		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/root2fits			&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/mcalcor2fits		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/makespectra		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/process3908		&& 	$(MAKE) prefix=$(prefix) uninstall
	cd $(PRJ_DIR)/scirm_analysis		&& 	$(MAKE) prefix=$(prefix) uninstall
