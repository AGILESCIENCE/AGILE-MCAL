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

#ifndef MCVLib_MCVBarCorrelation
#define MCVLib_MCVBarCorrelation


#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH2F.h>

#include "MCVStdAnalysis.h"

//! A class for analysis on bar correlation histograms

class MCVBarCorrelation : public MCVStdAnalysis {
	protected:
		int 	maxiter;
		double	maxsigma;
		double 	*avgbc;
		double 	*avgbclow;
		double 	*stdbc;
		double 	*stdbclow;
		void Loop(TH2F *hbc, double *, double *);
	public:
		MCVBarCorrelation();
		MCVBarCorrelation(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVBarCorrelation();
		virtual int Calculate();	//!< Fit the muon Landau peaks.
		virtual void PrintResults();	//!< Print peak energy values
		ClassDef(MCVBarCorrelation,1);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
