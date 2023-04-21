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

#ifndef MCVLib_MCVBkgSubtraction
#define MCVLib_MCVBkgSubtraction


#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TObjArray.h>
#include <TLegend.h>
#include <TKey.h>

#include "MCVStdAnalysis.h"
#include "MCVFitPeak.h"

//! A class for background subtraction and subsequent analysis

class MCVBkgSubtraction : public MCVStdAnalysis {
	protected:
		TString 	refname, simname;
		TFile 		*fbkg;		//!< Background reference file pointer
		TFile 		*fsim;		//!< Simulation reference file pointer
		TH1F		*hstotE;	//!< Background subtracted total energy spectrum
		TObjArray	*asrawE;	//!< Background subtracted single bars energy spectrum array
		TObjArray 	*abkgrawE;	//!< Background single bars energy spectrum array
		TObjArray 	*arawE;		//!< Current measure single bars energy spectrum array
		TObjArray	*asimrawE;	//!< Simulation single bars energy spectrum array
		TObjArray	*asim;		//!< Simulation histogram array
		TH1F		*hsimtotE;	//!< Simulation total energy spectrum
		TH2F 		*hsX;		//!< Background subtracted plane X raw image
		TH2F 		*hsZ;		//!< Background subtracted plane Z raw image
		
		float 	epeak;
		int 	simCompare;
		double		pc[30];		//!< Fit peak center
		double		ps[30];		//!< Fit peak sigma
		double		pr[30];		//!< Peak integral counts
		double		pcsim[30];	//!< Fit peak center
		double		pssim[30];	//!< Fit peak sigma
		double		prsim[30];	//!< Peak integral counts
		
		double		pctot;		//!< Fit peak center (bkg subtracted total spectrum)
		double		pstot;		//!< Fit peak sigma (bkg subtracted total spectrum)
		double		prtot;		//!< Peak integral counts (bkg subtracted total spectrum)
		double		pcsimtot;	//!< Fit peak center (simulation total spectrum)
		double		pssimtot;	//!< Fit peak sigma (simulation total spectrum)
		double		prsimtot;	//!< Peak integral counts (simulation total spectrum)
		
	public:
		MCVBkgSubtraction();
		MCVBkgSubtraction(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVBkgSubtraction();
		virtual int Calculate();	//!< Background subtraction and source peak fitting
		virtual void PrintResults();	//!< 
		void DrawZoom();		//!< Draw enlarged view for histograms
		void FitTotEHisto();		//!< Fit total bkg subtracte energy histo
		void FitSimTotEHisto();		//!< Fit total bkg subtracte energy histo
		void SimulationCompare();	//!< Compare each bar spectrum with simulations
		ClassDef(MCVBkgSubtraction,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
