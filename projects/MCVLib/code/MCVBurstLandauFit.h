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

#ifndef MCVLib_MCVBurstLandauFit
#define MCVLib_MCVBurstLandauFit

#include <fstream>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for BURST energy calibration fitting MIP tracks on single bars and on grlobal spectra

class MCVBurstLandauFit : public MCVStdAnalysis {
	protected:
		float mpv[30];			//!< Most Probable Value (MeV) of the muon Landau peak
		float sig[30];			//!< Sigma (MeV) of the muon Landau peak
		float amp[30];			//!< Integral of the muon Landau peak normalized to effective time (evt/s)
		float mE[30];			//!< Mean Energy loss (MeV) obtained integrating Landau distribution
		float refmpv[30];		//!< Reference MPV (MeV) of the muon Landau peak
		float refsig[30];		//!< Reference Sigma (MeV) of the muon Landau peak
		float refamp[30];		//!< Reference Integral of the muon Landau peak (evt/s)
		float meanmpv;			//!< Average MPV (MeV) over all MCAL bars
		float meanE;			//!< Average mean energy loss (MeV) over all MCAL bars
		int   noutmpv;			//!< Number of bars with MPV out of specification
		int   noutsig;			//!< Number of bars with sigma out of specification
		int   noutamp;			//!< Number of bars with rate out of specification
		float maxmpvdev;		//!< Maximum allowed deviation (%) in MPV from reference
		float maxsigdev;		//!< Maximum allowed deviation (%) in Landau sigma from reference
		float maxampdev;		//!< Maximum allowed deviation (%) in rate (evt/s) from reference
		int   refrun;			//!< Reference run for data comparison
		
	public:
		MCVBurstLandauFit();
		MCVBurstLandauFit(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVBurstLandauFit();
		virtual int Calculate();	//!< Fit the muon Landau peaks.
		virtual void PrintResults();	//!< Print peak energy values
		//virtual void SaveToFile();	//!< Save data to file
		//void UpdateCalibrationFile();
		ClassDef(MCVBurstLandauFit,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
