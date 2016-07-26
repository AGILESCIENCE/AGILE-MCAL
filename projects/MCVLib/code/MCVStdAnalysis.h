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

#ifndef MCVLib_MCVStdAnalysis
#define MCVLib_MCVStdAnalysis

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TSpectrum.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <math.h>

#include "MCVFileInfo.h"
#include "MCVFile.h"

//! A class collecting the standard MCAL data analysis routines

/*! Base class for standard analysis routines on histograms created with class MCVFilePKP.
The Calculate() virtual method must be implemented in derived classes to perform calculations.  
*/

enum MCVAnalysisResult {
	NO_CALC,
	WRONG_PKTTYPE,
	OK,
	BAD,
	REF_FILE_NOT_FOUND,
	REF_HISTO_NOT_FOUND,
	CONF_FILE_NOT_FOUND, 
	RUN_INFO_NOT_SET, 
	RAW_HISTO_NOT_SET, 
	OUT_OF_SPEC
};

class MCVStdAnalysis {

	protected:
		TObjArray *rawHisto;	//!< Pointer to the global collection of histograms
		MCVFileInfo *info;	//!< Information on current run
		
		TEnv *conf;		//!< Pointer to configuration file
		TCanvas *c1;		//!< Pointer to 1st canvas for graphic output
		TCanvas *c2;		//!< Pointer to 2nd canvas for graphic output
		FILE 	*fout;		//!< Pointer to output file
		FILE 	*fref;		//!< Pointer to reference file
		int	status;		//!< Analysis status (-1 means: analysis not yet completed)
		TString *refname;	//!< Pointer to reference file name
		bool	outofspec[60];	//!< TRUE if the corresponding bar/PD is out of spec
		unsigned long int  barmask;	//!< Bitmask to check whether to operate on bar i or not
		
		double GetEffectiveTime();		//!< Return effective time for all MCAL
		double GetEffectiveTime(Int_t ibar);	//!< Return effective time for a bar
		void LoadConf();	//!< Load standard configuration file. MANDATORY in constructors.
	
	public:
		MCVStdAnalysis();
		MCVStdAnalysis(TObjArray *hc, MCVFileInfo *in=NULL);
//		MCVStdAnalysis(TFile *fp);
		~MCVStdAnalysis();	//virtual 
		virtual int Calculate();	//!< Virtual method for calculations. Returns 0 if it's all OK
		TGraphErrors 	*CreateGraphErrors(char *gname);
		TGraph 		*CreateGraph(char *gname);
		virtual bool IsOK();		//!< Return TRUE if the calculated values are conformal to expectations
		virtual void PrintComment();	//!< Print sinthetic comment on results
		virtual void PrintResults();	//!< Print calculation results
		virtual void SaveToFile();	//!< Save calculation results to a text file
		void SetBarMask(unsigned long int bm) {barmask = bm;};
		void SetCanvases(TCanvas *a1, TCanvas *a2=NULL);	//!< Set pointer to canvases for graphic outputs
		void SetFileInfo(MCVFileInfo *in);			//!< Set pointer to file info
		void SetHistoCollection(TObjArray *hc);			//!< Set pointer to the histogram collection
		virtual void UpdateGlobalFile();			//!< Update global file
		ClassDef(MCVStdAnalysis,1);	//!< required for ROOT dictionary generation using rootcint
};



#endif
