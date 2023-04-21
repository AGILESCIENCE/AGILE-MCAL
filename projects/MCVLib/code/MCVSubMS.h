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

#ifndef MCVLib_MCVSubMS
#define MCVLib_MCVSubMS
#include "MCVFile.h"
#include <TH1F.h>

#define MAXWIN 10
#define CHKTIME 16000.

//! An implementation of Sub-ms burst search algorithm

struct subwin {
	double tini;	// initial time (s)
	double tfin;	// final time (s)
	int n;		// number of events in window
	subwin *prev;	// pointer to previous window
	subwin *next;	// pointer to successive window
	MCALevt evtarr[10];	// array of events in current window
};

class MCVSubMS {
	protected:
		TH1F *hSubMS;
		subwin *iniwin[4];	// pointer to initial window
		subwin *finwin[4];	// pointer to final window
		int nwin[4];		// number of open windows
		int nthr[4];		// threshold (>)
		int ntrg[4];		// number of triggers
		bool activewin[4];	// TRUE if window is active
		double winduration[4];	// window time duration
		double tprev[4];	// previous trigger time (to account for 16 ms trigger verification interval)
		void AddEvt(subwin *, MCALevt *);
	public:
		MCVSubMS(TH1F *);
		~MCVSubMS();
		void ProcessEvent(MCALevt *evt);
		subwin *InitSubwin(double, double);
		ClassDef(MCVSubMS,0);	//!< Required for ROOT dictionary generation using rootcint
};

#endif
