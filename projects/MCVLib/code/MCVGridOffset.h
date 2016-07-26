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

#ifndef MCVLib_MCVGridOffset
#define MCVLib_MCVGridOffset

#include <fstream>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for MCAL GRID offset calculation

/*! This class performs the standard analysis on GRID 39.02 MCAL data to derive offset and their width.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVGridOffset : public MCVStdAnalysis {
	protected:
		// grid offset data
		float groff[60];	//!< Grid offset center (from fit)
		float grsig[60];	//!< Grid offset sigma (from fit)
		float gramp[60];	//!< Grid offset amplitude (from fit)
		float refoff[60];	//!< Grid reference offset center (from file)
		float refsig[60];	//!< Grid reference offset sigma (from file)
		int nextraoff;		//!< Number of PDs with offset out of specifications
		int nextrasig;		//!< Number of PDs with sigma out of specifications
		double maxoffdev;	//!< Maximum allowed offset deviation from reference (ch)
		double maxsigdev;	//!< Maximum allowed sigma deviation from reference (%)
		TF1 	*peak;
		float avgXAdoff;
		float avgXAdsig;
		float avgXBdoff;
		float avgXBdsig;
		float avgZAdoff;
		float avgZAdsig;
		float avgZBdoff;
		float avgZBdsig;
	
	public:
		MCVGridOffset();
		MCVGridOffset(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVGridOffset();
		virtual int Calculate();	//!< Implementation for offset calculations. Returns 0 if it's all OK
		/*!< Calculate offset (center and sigma) for GRID physical calibration (39.02)
		*/
		
		float *GetGroff() {return groff;};	//!< Return calculated offset array (from fit)
		float *GetGrsig() {return grsig;};	//!< Return calculated sigma array (from fit)
		float *GetGramp() {return gramp;};	//!< Return calculated amplitude array (from fit)
		virtual void PrintResults();		//!< Print offset calculation results
		virtual void SaveToFile();		//!< Save offset calculation results to a text file
		virtual void UpdateGlobalFile();
		void UpdateCalibrationFile();
		ClassDef(MCVGridOffset,1);	//!< required for ROOT dictionary generation using rootcint
};

#endif
