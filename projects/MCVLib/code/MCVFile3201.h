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

#ifndef MCVLib_MCVFile3201
#define MCVLib_MCVFile3201

//#include <fitsio.h>

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <TROOT.h>
#include <TString.h>
//#include <TObjArray.h>
//#include <TEnv.h>
#include <TFile.h>
#include <TTree.h>
//#include <TF1.h>
//#include <TRandom3.h>
//#include <math.h>
#include <TMath.h>

#include "MCVFileInfo.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct hkevt {

	// HK needed by FF to search for MCAL freezing, after instructions from TPZ

	Double_t	time;
	char orphase;
	char plopmode;
	char digitbit;
	char rtmbit;
	char zstatus;
	Int_t zvalue;
	Int_t sentgnd;
};


class MCVFile3201 {
	
	protected:
		int 	status;
		int 	nfields;
		char 	comment[100];
		int	contact;
		char *posvali;	//!
		char	isRoot;	// 1 if it's a valid root file
		char	isFits;	// 1 if it's a valid fits file
		
		TFile	*fi;	// input root file pointer
		TFile	*fo;	// output root file pointer
		int nevents;
		int verbose;
		double mintime, maxtime;
		int nentries;
		
		TTree *tdata;
		hkevt evt;

		Double_t *time;	//!
		char *orphase;	//!
		char *plopmode;	//!
		char *digitbit;	//!
		char *rtmbit;	//!
		char *zstatus;	//!
		Int_t *zvalue;	//!
		Int_t *sentgnd;	//!

				
		TString   filename;	// input file
		TString   outname;	// output file
			
		void FillTree();
		void LoadFitsFile();
		void LoadRootFile();
		void printerror(int);	// Print out cfitsio error messages and exit program

	
	public:
		
		MCVFile3201(const char *name, int v=0);
		~MCVFile3201();	
		void PrintResults();
		void WriteRootFile(const char *name);
		ClassDef(MCVFile3201,0);	//!< required for ROOT dictionary generation using rootcint
};

#endif
