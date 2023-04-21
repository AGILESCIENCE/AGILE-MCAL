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

#ifndef MCVLib_MCVBurstOffset
#define MCVLib_MCVBurstOffset

#include <fstream>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>
#include "MCVStdAnalysis.h"

//! A class for calculating BURST offset

/*! This class calculates the offset (pedestals) in BURST mode. 
It must work on selected dataset with selections on PD-A or PD-B. 
*/

class MCVBurstOffset : public MCVStdAnalysis {
	protected:
		double		pc[30];		//!< Fit peak center
		double		ps[30];		//!< Fit peak sigma
		double		pa[30];		//!< Fit peak amplitude
		double		offdev[30];	//!< Offset deviation
		bool 		pdB;		//!< 0= work on PD-A; 1= work on PD-B
		double		maxoffdev;	//!< Maximum allowed offset deviation before issuing an OUT OF SPEC flag
		int 		nextraoff;	//!< Number of PDs with offset out of specifications
		double 		*refoff;	//!< Pointer to reference offset array 
	
	public:
		MCVBurstOffset();
		MCVBurstOffset(bool pd, TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVBurstOffset();
		virtual int Calculate();	//!< Checks
		virtual void PrintResults();	//!< Print 
		virtual void SaveToFile();	//!< Save 
		void UpdateCalibrationFile();
		double *GetPeakCenter(){return pc;};
		ClassDef(MCVBurstOffset,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
