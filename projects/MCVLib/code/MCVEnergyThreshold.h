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

#ifndef MCVLib_MCVEnergyThreshold
#define MCVLib_MCVEnergyThreshold

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TLine.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for retriggering search in BURST data

/*! This class performs retriggering search in BURST data.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVEnergyThreshold : public MCVStdAnalysis {
	protected:
		float	th50[30];		//!< threshold level at 50% of the background maximum
		float	th90[30];		//!< threshold level at 90% of the background maximum
		float	ref50[30];		//!< reference threshold level at 50% of the background maximum
		float	ref90[30];		//!< reference threshold level at 90% of the background maximum
		int	refrun;			//!< reference run
		float 	maxdev;			//!< maximum allowed deviation from reference (%)
		int	nout50;
		int	nout90;			//!< number of bars with energy threshold out of specifications
		
	public:
		MCVEnergyThreshold();
		MCVEnergyThreshold(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVEnergyThreshold();
		virtual int Calculate();	//!< Search for retriggering in burst data.
		virtual void PrintResults();	//!< Print retriggering calculation results
		virtual void SaveToFile();	//!< Save threshold calculation results to a text file
		ClassDef(MCVEnergyThreshold,1);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
