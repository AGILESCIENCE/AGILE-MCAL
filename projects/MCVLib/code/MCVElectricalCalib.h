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

#ifndef MCVLib_MCVElectricalCalib
#define MCVLib_MCVElectricalCalib

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

#define SQRT2PI 2.507

//! A class for electrical calibration data analysis

/*! This class performs electrical calibration data analysis.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVElectricalCalib : public MCVStdAnalysis {
	protected:
		int pnum;	//!< Number of peaks expected
		int thr;	//!< Threshold (ADC channels) above offset to start the peak serch
		float p1amp[60];	// current peak 1
		float p1cen[60];
		float p1sig[60];
		float p1nevt[60];
		float p2amp[60];	// current peak 2
		float p2cen[60];
		float p2sig[60];
		float p2nevt[60];
		float r1cen[60];	// reference peak 1
		float r1sig[60];
		float r2cen[60];	// reference peak 2
		float r2sig[60];
		float nextraoff1;
		float nextrasig1;
		float nextraoff2;
		float nextrasig2;
		int   nminevt1;
		int   nminevt2;
		float maxoffdev;
		float maxsigdev;
		int   minevents;
		int   refrun;
		
		TF1 	*peak, *peak2;
		int FitPeaks(int ipd, TH1F *aux, float xmin, float xmax);
		
	public:
		MCVElectricalCalib();
		MCVElectricalCalib(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVElectricalCalib();
		virtual int Calculate();	//!< Search for peaks in electrical calibration data.
		virtual void PrintResults();	//!< Print calculation results
		virtual void SaveToFile();
		void SetNPeaks(int np) {pnum = np;};
		void SetThreshold(int th) {thr = th;};
		ClassDef(MCVElectricalCalib,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
