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

#ifndef MCVLib_MCVFitPeak
#define MCVLib_MCVFitPeak

#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for fitting source peaks

/*! This class fits a source peak 
*/

class MCVFitPeak : public MCVStdAnalysis {
	protected:
		TH1F 		*aux;
		double 		Et;		//!< Target energy (MeV) of the peak
		double		pc[30];		//!< Fit peak center
		double		ps[30];		//!< Fit peak sigma
		double		pa[30];		//!< Fit peak amplitude
		double		pr[30];		//!< Peak integral counts
		double		rki2[30];	//!< Fit reduced Ki^2
		double		Ei;		//!< Current peak energy (MeV, from histogram)
		double		dEhi;		//!< Upper delta_E
		double		dElo;		//!< Lowed delta_E
		int		ibin;		//!< Current peak bin
		int		isEarray;	//!< 1 if the passed TObjArray is the energy array, 0 otherwise
		
		void		FindPeak();	//!< Return approximate peak center near Et
	
	public:
		MCVFitPeak();
		MCVFitPeak(double E, TObjArray *hc, MCVFileInfo *in=NULL, int Earr=0);
		~MCVFitPeak();
		virtual int Calculate();	//!< Checks
		virtual void PrintResults();	//!< Print 
		virtual void SaveToFile();	//!< Save 
		double	*GetCenter() {return pc;};
		double	*GetSigma() {return ps;};
		double	*GetAmplitude() {return pa;};
		double	*GetCounts() {return pr;};
		void	SetUpperDE(double dh) {dEhi = dh;};
		void	SetLowerDE(double dl) {dElo = dl;};
		void	UpdateFit(int bar, double cen, double sig, double amp, double rk2);
		ClassDef(MCVFitPeak,1);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
