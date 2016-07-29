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

#ifndef MCVLib_MCVCountRate
#define MCVLib_MCVCountRate

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include <TGraphErrors.h>
#include "MCVStdAnalysis.h"

//! A class for MCAL count rate evaluation

/*! This class calculates the mean count rate per bar by fitting the count rate distribution. 
Comparison between count rate and its variance is performed to insure correct poissonian behavior.
Comparison between count rate and that from a reference file is performed to point out non uniform behaviors.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVCountRate : public MCVStdAnalysis {
	protected:
		double	*crx;	//!
		double	*sigx;	//!
		double	*crz;	//!
		double	*sigz;	//!
		double	drmax;	
		int 	refrun;
		double	maxpoisdev;
		int	nextra;		//!< Number of bars with exceeding count rate variation
		int	npois;		//!< Number of bars with exceeding ratio sigma^2/rate
		float	refcr[30];
		float	refsig[30];
		float	dr[30];
		
		void CompareReferenceData();

	public:
		MCVCountRate();
		MCVCountRate(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVCountRate();
		virtual int Calculate();	//!< Implementation for count rate calculations. Returns 0 if it's all OK
		/*!< Calculate count rate (mean and sigma) for MCAL data (39.02)
		*/
		
		virtual void PrintResults();	//!< Print caount rate calculation results
		virtual void SaveToFile();	//!< Save offset calculation results to a text file
		ClassDef(MCVCountRate,1);	//!< required for ROOT dictionary generation using rootcint
};

#endif
