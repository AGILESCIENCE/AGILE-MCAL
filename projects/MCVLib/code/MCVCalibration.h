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
#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TEnv.h>
#include <TFile.h>
#include <TKey.h>
#include <TLine.h>
#include <TMarker.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TObjArray.h>
#include <TF1.h>


class MCVCalibration {
 private:
 	int 	nhit[31][2];		//[bar id][PD (0=A, 1=B)] static
	float	data[31][2][3][30];	//[bar id][PD (0=A, 1=B)][x(cm), y(ch), y_err(ch)][point]
 	float	y2[31][2][30];		//second derivative array for spline interpolation: [bar id][PD (0=A, 1=B)][point]
	float	X;
	float	Y;
	float	dataK[31][2][2][30];	//[bar id][A/B, B/A][x(cm), y][point]
 	float	y2K[31][2][30];		//second derivative array for function K spline interpolation: [bar id][point]
	double 	u0A[30];
     	double 	u0B[30];
     	double 	alphaA[30];
    	double 	alphaB[30];
     	double 	oA[30];
     	double 	oB[30];
	double 	param[12];
	float poserr[20];	
	
	bool	calib;		// TRUE if calibration data on current bar are availble, FALSE otherwise
	int	cbar;		// bar-id (0..29) of the current bar under calibration
	double	cgainA;		// gain (ch/MeV) of the current bar under calibration, PD-A
	double	cgainB;		// gain (ch/MeV) of the current bar under calibration, PD-B
	double	calphaA;	// attenuation coefficient (cm^-1) of the current bar under calibration, PD-A
	double	calphaB;	// attenuation coefficient (cm^-1) of the current bar under calibration, PD-B
	
	TString   calibName;		
	TString   XDatFile;		
	TString   ZDatFile;		
	TString   calFile;		
	TEnv *stdconf;
	TEnv *conf;
	
	TObjArray *hcpdA;
	TObjArray *hcpdB;
	TObjArray *hcBonA;
	
 	void 	GetXDataStatus();
 	void 	GetZDataStatus();
	void	SortData();
	void	SetGraph();
 	void	FitCurve(int bar, int model);
/*	void	SavePolPar(int bar);
	void	SaveExpoPar();
	TF1 	*ExpoModel(TGraphErrors *gr, int pd);
	TF1 	*ExpoPolModel(TGraphErrors *gr, int pd);
	TF1 	*PolModel(TGraphErrors *gr, int pd);
*/

 public:
	MCVCalibration();
	virtual ~MCVCalibration();
	TObjArray *GetPdAGraph(){return hcpdA;};
	TObjArray *GetPdBGraph(){return hcpdB;};
	TObjArray *GetBonAGraph(){return hcBonA;};
   	ClassDef(MCVCalibration,0);
};

