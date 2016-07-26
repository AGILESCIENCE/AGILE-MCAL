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
		char 	filename[200];	//!< File name
		int 	runid;		//!< Run-id
		int 	pkttype;	//!< Packet type (3902, 3909 etc...)
		long nevents;		//!< Number of events
		unsigned long tstart;	//!< Run start time 
		unsigned long tstop;	//!< Run stop time
		unsigned long ttot;	//!< Total run duration (s) calculated from fits file keyword
		Bool_t 	pdhuformat;	//!< TRUE if it is PDHU format, FALSE if it is MCAL DFE-TE format
		Bool_t 	pdhudata;	//!< TRUE if it is PDHU data, FALSE if it is MCAL DFE-TE data
		Bool_t 	grid;		//!< TRUE if it is grid data, FALSE if it is burst data
		
		// calibration parameters
		unsigned int	seed;	//!< Initial seed for random number generator
		TString   calFile;	//!< Calibration file name 
		Double_t barLen;	//!< Bar length
		Double_t gainFactor;	//!< Gain multiplicative coefficient for GRID energy calculations (~1.2)
		Double_t u0A[30];	//!< PD-A gain near PD (ch/MeV)
		Double_t u0B[30];	//!< PD-B gain near PD (ch/MeV)
		Double_t alphaA[30];	//!< PD-A attenuation coefficient (cm^-1)
		Double_t alphaB[30];	//!< PD-B attenuation coefficient (cm^-1)
		Double_t oA[30];	//!< Offset of PD-A (ADC channels)
		Double_t oB[30];	//!< Offset of PD-B (ADC channels)
		Int_t	soA[30];	//!< PD-A offset as stored in PDHU for scientific RM calculation	
		Int_t	soB[30];	//!< PD-B offset as stored in PDHU for scientific RM calculation	

		// bar correlation parameters
		Int_t		nlowDT;		//!< Number of consecutive events on the same bar with DT<10 us
		Double_t	lowDTti;	//!< Time (s) of the first "impossible" event
		Double_t	lowDTtf;	//!< Time (s) of the last "impossible" event
		
		// data-driven informations
		Double_t	teff;		//!< Effective time (s) as calculated on bar X1
		
		MCVFileInfo();
		~MCVFileInfo();	//virtual 
		void PrintCalibrationParameters();	//!< Prints calibration parameters
		void PrintFileInfo();			//!< Prints file informations
		ClassDef(MCVFileInfo,1);		//!< required for ROOT dictionary generation using rootcint
};

#endif