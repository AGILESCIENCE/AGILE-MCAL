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
#ifndef MCVLib_MCVFileInfo
#define MCVLib_MCVFileInfo

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include "TNamed.h"
#include "TString.h"

//! A class for collecting basic information on run 

/*! This class contains the basic information (run-id, packet type, duration, calibration parameters and so on...) on a certain run. 
It is derived from ROOT class TNamed so it can be written in a ROOT file to create a database of run informations and to easily allow historical comparison between different runs. 
*/

class MCVFileInfo : public TNamed {

	public:
		// basic informations
		char 	filename[200];	
		int 	runid;		
		int 	pkttype;	
		long nevents;		
		unsigned long tstart;	 
		unsigned long tstop;	
		unsigned long ttot;	
		Bool_t 	pdhuformat;	
		Bool_t 	pdhudata;	
		Bool_t 	grid;		
		
		// calibration parameters
		unsigned int	seed;	
		TString   calFile;	 
		Double_t barLen;	
		Double_t gainFactor;	
		Double_t u0A[30];	
		Double_t u0B[30];	
		Double_t alphaA[30];	
		Double_t alphaB[30];	
		Double_t oA[30];	
		Double_t oB[30];	
		Int_t	soA[30];		
		Int_t	soB[30];		

		// bar correlation parameters
		Int_t		nlowDT;		
		Double_t	lowDTti;	
		Double_t	lowDTtf;	
		
		// data-driven informations
		Double_t	teff;		
		
		MCVFileInfo();
		~MCVFileInfo();	//virtual 
		void PrintCalibrationParameters();	
		void PrintFileInfo();			
		ClassDef(MCVFileInfo,1);		
};

#endif