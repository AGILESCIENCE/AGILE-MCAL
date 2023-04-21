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

#ifndef MCVLib_GRB3908
#define MCVLib_GRB3908

#include "GRB.h"

class GRB3908  : public GRB {
	
	protected:
		MCALevt 		inevt;
		TTree 			*data;		// GRB events data tree
		TH1F			*hlc[3][4][7];	// default histogram light curves 
		TH1F			*hpoisson;	// events/bin histogram for pre-burst
		TCanvas			*cStdLE;	// default light curve: Low Energy
		TCanvas			*cStdME;	// default light curve: Medium Energy
		TCanvas			*cStdHE;	// default light curve: High Energy
		TCanvas			*cpXZ;
		TCanvas 		*cStdDiag;	// standard diagnostic Canvas	
		TCanvas 		*cPosDiag;	// position distribution Canvas	
		TCanvas 		*cVSBDiag;	// VSB diagnostic Canvas	
		TCanvas 		*cTotal;	// complete diagnostic Canvas	
						
		// diagnostic histograms & graph
		TH1F			*hDTbkg;	// global time difference between every successive event (background)
		TH1F			*hDTgrb;	// global time difference between every successive event (GRB)
		TH2F			*hbarcorr;	// bar[i],bar[i+1] 2D histogram (for spurious correlations)
		TH1F			*hMult;	// total MCAL multiplicity
		TH3F			*hX;		// X plane events distribution
		TH3F			*hZ;		// Z plane events distribution
		TH1F			*hbarsbkg;
		TH1F			*hbarsgrb;
		TGraph			*gbart;		// VSB bars vs t-t0
		TGraph			*gEt;		// VSB energy vs t-t0
		
		int			nbars[30];	// bars occupation
		int			cuboni[8];	// spatial regions occupation
		double			dt0;
		int			nsubms;
		double			mint;		// minimum t-t0, diagnostic for the pre-burst problem on the 1-ms SIT
		
		void InitDiagnostic();
		void FillDiagnostic(MCALevt *); 

	public:
		GRB3908();
		GRB3908(TString rootfile);
		~GRB3908();
		int DefaultLightCurve();
		void DrawCustomLightCurve(double ti, double tf);
		void DrawDefaultLightCurve();
		void DrawPositionDiagnostic(TPad *ppos);
		void DrawVSBDiagnostic(TPad *pbar);
		void DrawDiagnostic(double ti, double tf);	// draw total diagnostic
		int CustomLightCurve(double t0=0., double tpre=-20., double tpost=50., double emin=0., double emax=0.700, int barsX=0x7FFF, int barsZ=0x7FFF, double tbin=0.032);
		float *GetPha(double ti, double tf, int nb=0, double *spbins=NULL);
		TGraph *GetEvsT(){return gEt;};
		void SaveCustomLightCurve(TString outpath);
		void SaveDiagnostic(TString outpath);
		void SaveVSBDiagnostic(TString outpath);
		int SaveIPNMsg(TString outpath);
		int SaveVSBInfo(TString outpath);
		int StandardDiagnostic();
		int VSBDiagnostic();			// diagnostic for Very Short Bursts
		void CalculatePeakFluxVSB();		// peak flux calculation for Very Short Bursts
		void CalculateT90VSB();			// duration calculation for Very Short Bursts
		void CalculateFluenceVSB();		// fluence calculation for Very Short Bursts
		void SpectralAnalysisVSB();		// spectral analysis for Very Short Bursts
		void CustomSpectralAnalysisVSB();
		double FindTrueT0(double t0L, double dt, int thr);	// apply sub-ms time window algorithm to find the time of the spike. Return corrected t0
		ClassDef(GRB3908,0);				//!< required for ROOT dictionary generation using rootcint
};



#endif
