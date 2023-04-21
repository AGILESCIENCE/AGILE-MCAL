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

#ifndef MCVLib_MCVSciRM
#define MCVLib_MCVSciRM

#include <TString.h>
#include <TEnv.h>
#include <TGraphErrors.h>
#include <TDatime.h>
#include "MCVStdAnalysis.h"

//! A class for scientific ratemeters analysis

/*! This class performs analysis on scientific ratemeters data.
Sci. RM coming from 39.13 packet (normalized to time unit) are compared to reference ones. 
If BURST data, also the consistency between the 39.13 Sci. RM and those calculated from data is evaluated. 
The implemented routines are derived from those developed for program mcalview2.  
*/

class MCVSciRM : public MCVStdAnalysis {
	protected:
		TGraphErrors *grmX;	//!< Pointer to TGraph with calculated ratemeters on plane X
		TGraphErrors *grmZ;	//!< Pointer to TGraph with calculated ratemeters on plane Z
		TGraphErrors *grmX3913;	//!< Pointer to TGraph with RM coming from 39.13 packet on plane X
		TGraphErrors *grmZ3913;	//!< Pointer to TGraph with RM coming from 39.13 packet on plane Z
		TH1F	*hX;		//!< scientific ratemeters on plane X as derived from data
		TH1F	*hZ;		//!< scientific ratemeters on plane Z as derived from data
		TH1F	*hXR;		//!< scientific ratemeters on plane X as derived from reconstructed energy
		TH1F	*hZR;		//!< scientific ratemeters on plane Z as derived from reconstructed energy
		double *E;		//!< Array of RM energy bands center
		double *E_err;		//!< Array of RM energy bands half width
		double *rmX3913;	//!< Array of 39.13 RM on plane X
		double *rmX3913_err;	//!< Array of 39.13 RM sigma on plane X
		double *rmZ3913;	//!< Array of 39.13 RM on plane Z
		double *rmZ3913_err;	//!< Array of 39.13 RM sigma on plane Z
		double *rmX;		//!< Array of calculated RM on plane X
		double *rmX_err;	//!< Array of calculated RM sigma on plane X
		double *rmZ;		//!< Array of calculated RM on plane Z
		double *rmZ_err;	//!< Array of calculated RM sigma on plane Z
		float   refE[11];	//!< Array of reference energy bands center
		float   refE_err[11];	//!< Array of reference energy bands half width
		float   refX[11];	//!< Array of reference RM on plane X
		float   refX_err[11];	//!< Array of reference RM on sigma plane X
		float   refZ[11];	//!< Array of reference RM on plane Z
		float   refZ_err[11];	//!< Array of reference RM on sigma plane Z
		int	refrun;		//!< Reference run-id
		float	nsigma;		//!< Maximum allowed deviation from reference (#sigma) before issuing a warning flag
		float	dev1X[11];	//!< Array of deviations (#sigma) of calculated from 39.13 RM on plane X
		float	dev1Z[11];	//!< Array of deviations (#sigma) of calculated from 39.13 RM on plane Z
		float	dev2X[11];	//!< Array of deviations (#sigma) of 39.13 from reference RM on plane X
		float	dev2Z[11];	//!< Array of deviations (#sigma) of 39.13 from reference RM on plane Z
		int	nout1X;
		int	nout1Z;
		int	nout2X;
		int	nout2Z;
	public:
		MCVSciRM();
		MCVSciRM(TObjArray *hc, MCVFileInfo *in=NULL);
		~MCVSciRM();
		virtual int Calculate();	//!< Performs analysis on scientific ratemeters data.
		virtual void PrintResults();	//!< Print calculation results
		virtual void SaveToFile();
		virtual void UpdateGlobalFile();
		ClassDef(MCVSciRM,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
