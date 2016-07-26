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

#ifndef MCVLib_GRB
#define MCVLib_GRB

#include <iostream>
#include <stdlib.h>
#include <string>
#include <TROOT.h>
#include <TString.h>
#include <TObjArray.h>
#include <TEnv.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TF1.h>
#include <TMath.h>
#include <TLine.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TTimeStamp.h>
#include <TLatex.h>
#include <TProfile2D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define VERBOSE 1

#include "MCVFileInfo.h"
#include "MCVFile.h"

class GRB {
	
	protected:
		MCVFileInfo 		*info;		// dataset basic information
		TFile 			*fi;		// input file
							// (3 energy bands, 4 spatial regions, 7 time bin)
		TH1D			*hclc;		// custom light curve
		TH2D			*h2clc;		// custom light curve, energy selected
		TGraphErrors		*ghr;		// graph for hardness ratio evaluation
		TStyle 			*mcalstyle;	// default graphic style
		//TCanvas			*cCustom;	// custom light curve
		TTimeStamp		*epoch;		// reference date for TT time
		TTimeStamp		*tzero;		// reference date for TT time
			
		// diagnostic histograms
		TH1F			*hEbkg;		// total MCAL energy spectrum (background)
		TH1F			*hEgrb;		// total MCAL energy spectrum (GRB)

		TH1F			*hLE;		// low energy counts for hardness ratio calculation
		TH1F			*hHE;		// high energy counts for hardness ratio calculation
		double			thr;		// energy threshold for hardness ratio calculations
		
		int 			nevents;	// number of events
		int 			nbins;
		int 			barsX;
		int 			barsZ;
		double			tstart;
		double 			tstop;
		double			t0;
		double			tbin;
		double			emin;
		double			emax;
		int			orbit;
		int			alertcode;
		float			tsearch;	// delta-t around t0 to search for the peak
		
		int			burstid;
		
		// spectral analysis
		int			nEbins;		// number of energy bins
		double			*mcalEbin;	// boundaries of the energy bins
		float 			*pha;	
		
		// energy selected histogram
		double			*bkg;		// background per bin
		double			*ebkg;		// statistical error on background per bin
		double			*fpeak;		// peak flux
		double			*efpeak;	// error on peak flux
		double			*tpeak;
		double			*fluence;	// fluence in t90 time interval (counts)
		double			*efluence;	// error on fluence in t90 time interval (counts)
		int			*binpeak;

		// HR calculations
		double 			bkgLE;
		double 			bkgHE;
		double 			ebkgLE;
		double 			ebkgHE;
		
		// selected histogram
		double			cbkg;		// background per bin
		double			ecbkg;		// statistical error on background per bin
		double			cfpeak;		// peak flux
		double			ecfpeak;	// error on peak flux
		double			ctpeak;
		double			cfluence;	// fluence in t90 time interval (counts)
		double			ecfluence;	// error on fluence in t90 time interval (counts)
		int			cbinpeak;
		int			i90lo;		// low index for t90 calculation
		int			i90hi;		// high index for t90 calculation
		int			i0;		// index corresponding to t0
		double			hr;		// default hardness ratio
		double			ehr;		// error on default hardness ratio
		double			t90;
		double			et90;
		double			avgE;		// average GRB/VSB energy
		double			maxE;		// average GRB/VSB energy
		
		// basic GRB info
		double 			tbkgini;
		double 			tbkgfin;
		double 			tgrbini;
		double 			tgrbfin;
		
		int			year;
		int			month;
		int			day;
		int			hour;
		int			min;
		int			sec;

	public:
		GRB();
		~GRB();
		void CalculateBackground();
		void CalculatePeakFlux();
		void CalculateT90();
		void CalculateFluence();
		void CalculateHR();
		void DrawLightCurve(TPad *cpad, double ti=0., double tf=0.);
		int GetAlertTypeCode() {return alertcode;};
		int GetNEbins() {return nEbins;};
		int GetContact() {return info->runid;};
		double GetT90() {return t90;};
		double GetTPeak() {return ctpeak;};
		double GetMaxE() {return maxE;};
		double GetSelFluence() {return cfluence;};
		int GetI90Lo() {return i90lo;};
		int GetI90Hi() {return i90hi;};
		TH1D *GetLightCurve() {return hclc;};
		TH2D *GetLightCurve2D() {return h2clc;};
		int SaveIPN(TString ipnfile, double tbefore=-10., double tafter=+10.);
		TString *GetFileHeader();
		void SetGRBBoundaries( double tbkgini, double tbkgfin, double tgrbini, double tgrbfin);
		void SetBurstId(int id) {burstid = id;};
		void SetT90(int ilo, int ihi);
		void SetT0(double at0) {t0 = at0;};
		void SetTSearch(float auxt) {auxt>0. ? tsearch=auxt : tsearch=30.;};
		int SaveInfo(TString infofile, int newline=1);
		int SaveMsg(TString msgfile);
		int SaveAnalysis(TString outpath);
		void SpectralAnalysis();
		void CustomSpectralAnalysis();
		ClassDef(GRB,1);				//!< required for ROOT dictionary generation using rootcint
};



#endif
