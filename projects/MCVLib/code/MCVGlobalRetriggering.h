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

#ifndef MCVLib_MCVGlobalRetriggering
#define MCVLib_MCVGlobalRetriggering

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for retriggering search in BURST data

/*! This class performs retriggering search in BURST data.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVGlobalRetriggering : public MCVStdAnalysis {
	protected:
/*		int 	totevts[30];			//!< Total number of events / bar
		double 	avgCR[30];			//!< Average count rate per bar (evt/s)
		int	tdead[30];			//!< Estimated dead time (us) per bar
		int	tfirst[30];			//!< First non-zero delta_t (must be ~equal to dead time)
		int	tmax;				//!< Maximum time for retriggering calculations
		int	nretriggering;			//!< Number of bars showing retriggering
		int	nlowdeadtime;			//!< Number of bars with suspect low dead time
		double	maxfrac;			//!< Maximum allowed excess fraction of counts
		double	nsigma;				//!< Maximum allowed significance (sigma) of counts excess
		double	mindt;				//!< Minimum allowed dead time
		double 	extracnts[30];			//!< Excess of counts
		double 	extrafrac[30];			//!< Excess of counts normalized to total events
		double 	statsig[30];			//!< Statistical significance (sigma) of counts excess
		TF1	  *    fdt;			//!< Exponential fit to delta_time histo
*/
		
		float mu;
		int	tmax;				//!< Maximum time (us) for retriggering calculations
		double	maxfrac;			//!< Maximum allowed excess fraction of counts
		double	fitlow;				//!< Lower time limit (us) for exponential fitting
		double	fithi;				//!< Upper time limit (us) for exponential fitting
		double 	extracnts;			//!< Excess of counts
		double 	extrafrac;			//!< Excess of counts normalized to total events
		double 	par[2];
		double c0, e0;
		TH1F	*hDT;				//!< Global DT histogram
		TF1	*fdt;				//!< Exponential fit to delta_time histo
	
	public:
		MCVGlobalRetriggering();
		MCVGlobalRetriggering(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVGlobalRetriggering();
		virtual int Calculate();	//!< Search for retriggering in burst data.
		virtual void PrintResults();	//!< Print retriggering calculation results
		ClassDef(MCVGlobalRetriggering,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
