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

#ifndef MCVLib_MCVGridEnergyCal
#define MCVLib_MCVGridEnergyCal

#include <fstream>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for GRID energy calibration

/*! This class performs energy calibration on GRID physical calibration data. 
For each bar, the center of the Landau peak for muons is calculated and compared to reference value.
It works properly only for data taken with zenith pointing.
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVGridEnergyCal : public MCVStdAnalysis {
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
		MCVGridEnergyCal();
		MCVGridEnergyCal(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVGridEnergyCal();
		virtual int Calculate();	//!< Fit the muon Landau peaks.
		virtual void PrintResults();	//!< Print peak energy values
		virtual void SaveToFile();	//!< Save data to file
		void UpdateCalibrationFile();
		ClassDef(MCVGridEnergyCal,1);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
