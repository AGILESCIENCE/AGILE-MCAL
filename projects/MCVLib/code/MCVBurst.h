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
#ifndef MCVLib_MCVBurst
#define MCVLib_MCVBurst

#include "MCVFileInfo.h"
#include "MCVFile.h"

#include <TTree.h>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>

/*!
MCVBurst 
*/


class MCVBurst  : public TNamed {	//! structure with main burst information


	protected:
	
	public:
		int    burstid;		
		double bstart ;     	 
		double bstop ;    
		double inittime;  
		int    nburstevents;	
		int    bsrsit;		
		int    nume;		
		int    numg;		
		int    inipkt;		
		int    finpkt;		
		int    bsr;
		int    ber;
		
		MCVBurst();
		MCVBurst(const char *name);
		~MCVBurst();
		void PrintInfo();
		ClassDef(MCVBurst,1);			//!< Required for ROOT dictionary generation using rootcint
};	

#endif