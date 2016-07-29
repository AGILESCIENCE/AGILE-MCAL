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

#ifndef MCVLib_MCVFile3916
#define MCVLib_MCVFile3916

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
#include <TVector3.h>

#include "MCVFileInfo.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct gpsevt {
	Double_t	gpsxpos;
	Double_t	gpsypos;
	Double_t	gpszpos;
	Double_t	time;
	Double_t	lon;
	Double_t	lat;
	Double_t	h;
	Double_t	etheta;
	Double_t	ephi;
	Int_t		posvali;
};


class MCVFile3916 {
	
	protected:
		int 	status;
		int 	nfields;
		char 	comment[100];
		int	contact;
		Int_t *iX;		//!
		Int_t *iY;		//!
		Int_t *iZ;		//!
		Double_t *time;		//!
		Double_t *ephi;		//!
		Double_t *etheta;	//!
		char *posvali;		//!
		char	isRoot;	// 1 if it's a valid root file
		char	isFits;	// 1 if it's a valid fits file
		
		TFile	*fi;	// input root file pointer
		TFile	*fo;	// output root file pointer
		int nevents;
		
		TTree *tdata;
		gpsevt evt;
		
		TString   filename;	// input file
		TString   outname;	// output file
	
		// GPS ellipsoid reference (WGS84) aux var
		double a;
		double b;
		double f;
		double e;
		double e1;
		double rad2deg; 
		double p, theta, N;
		double mintime, maxtime;
		int nentries;
		int verbose;
		int eci;

		// sidereal time calculation aux var
		double o1;
		double o2;
		double m;
		
		void FillTree();
		void LoadFitsFile();
		void LoadRootFile();
		void printerror(int);	// Print out cfitsio error messages and exit program

	
	public:
		
		MCVFile3916(const char *name, int v=0, int e=0);
		~MCVFile3916();	
		void Ecef2lla(double X=0., double Y=0., double Z=0.);
		void Eci2lla(double X=0., double Y=0., double Z=0., double t=0.);
		int GetCoordinates(gpsevt *aevt);
		void PrintResults();
		void WriteRootFile(const char *name);
		int IsRoot(){return isRoot;};
		ClassDef(MCVFile3916,1);	//!< required for ROOT dictionary generation using rootcint
};

#endif
