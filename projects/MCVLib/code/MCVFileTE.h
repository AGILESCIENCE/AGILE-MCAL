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

#ifndef MCVLib_MCVFileTE
#define MCVLib_MCVFileTE

#include "MCVFile.h"
#include "MCVHisto.h"

#define MAXLEN 100000

//! Class for level1 fits file elaboration coming from MCAL DFE Test Equipment

/*!
MCVFileTE is derived from class MCVFile and provides a standard interface to AGILE DFE-TE level-1 fits files.
Configuration files are required and must be located in $(MCALSW)/share/MCVLib_conf/ 
THIS CLASS IS OBSOLETE AND IT IS NOT COMPLETELY IMPLEMENTED YET
*/

class MCVFileTE : public MCVFile, public MCVHisto {
	
	public:
		MCVFileTE();
		MCVFileTE(const char *name);
		~MCVFileTE();	//virtual 
		
		void BuildTimeArray();
		virtual void CreateRootFile();	//!< Implementation for ROOT file creation
		double *GetCountRate(double tini, double tstep, int nstep);
		double *GetCountRate(int bar, double tini, double tstep, int nstep);
		void ProcessData();
		ClassDef(MCVFileTE,0);
};

#endif