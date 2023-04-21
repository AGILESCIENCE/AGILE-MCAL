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

#ifndef MCVLib_MCVBarMonitor
#define MCVLib_MCVBarMonitor

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for monitoring bar status (on / off)

/*! This class checks whether bars are on or off. 
*/

class MCVBarMonitor : public MCVStdAnalysis {
	protected:
	
	public:
		MCVBarMonitor();
		MCVBarMonitor(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVBarMonitor();
		virtual int Calculate();	//!< Checks
		virtual void PrintResults();	//!< Print 
		ClassDef(MCVBarMonitor,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
