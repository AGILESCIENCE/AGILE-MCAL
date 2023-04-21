//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MCVLib_MCVFilePKP
#define MCVLib_MCVFilePKP

#include "MCVFile.h"
#include "MCVHisto.h"


//! Class for level1 fits file elaboration coming from PDHU

/*!
MCVFilePKP is derived from class MCVFile and provides a standard interface to AGILE PDHU level-1 fits files.
Configuration files are required and must be located in $(MCALSW)/share/MCVLib_conf/ 
It can work on all the packet types related to MCAL. 
It writes all the events to a ROOT file with position and energy calculation for subsequent analysis. 
During translation from fits to ROOT it performs standard analysis and produces standard histograms. 
See program convPKPtoRoot for usage example.
*/


class MCVFilePKP : public MCVFile, public MCVHisto {

	protected:
	
	public:
		MCVFilePKP();
		MCVFilePKP(const char *name);
		~MCVFilePKP();	//virtual 
		virtual void CreateRootFile();	//!< Implementation for ROOT file creation
		Bool_t Add3913SciRatemeters();	//!< Add Sci. RM standard histograms from 39.13 lv1 file
		void Process3902Data();		//!< Process GRID physical calibration data (39.02 TM packet)
		void Process3909Data();		//!< Process BURST data (39.09 or 39.08 TM packet)
		void ProcessTrigTrackData();	//!< Process DAFNE tagged data (39.02 TM packet)
		void PrintEvent(int);		//!< Print an event info
		ClassDef(MCVFilePKP,0);		//!< required for ROOT dictionary generation using rootcint
};

#endif