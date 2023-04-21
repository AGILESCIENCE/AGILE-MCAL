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

#ifndef MCVLib_MCVAntiCoincidence
#define MCVLib_MCVAntiCoincidence

#define MIN_E 3.
#define MAX_E 1000.
#define E_BIN 0.020

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include <TGraphErrors.h>
#include "MCVStdAnalysis.h"

//! A class for MCAL AC efficiency evaluation

class MCVAntiCoincidence : public MCVStdAnalysis {
	protected:
		int minbin, maxbin;
		float herate;			//!< E > MIN_BIN*20keV rate (evt/s)
		float herateref;		//!< maximum reference rate
	public:
		MCVAntiCoincidence();
		MCVAntiCoincidence(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVAntiCoincidence();
		virtual int Calculate();	//!< Fit the muon Landau peaks.
		virtual void PrintResults();	//!< Print peak energy values
		ClassDef(MCVAntiCoincidence,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
