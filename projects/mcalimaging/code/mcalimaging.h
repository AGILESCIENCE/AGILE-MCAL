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
#ifndef MCALIMAGING
#define MCALIMAGING


#include "MCVFileInfo.h"
#include "MCVFile.h"

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TObjArray.h>
#include <TKey.h>
#include <TString.h>
#include <TTree.h>
#include <TLegend.h>
#include <TLine.h>

#include <iostream.h>
//#include <fitsio.h>
#include <stdlib.h>
#include <math.h>

#define NCOL 18
#define ETH 5.
//#define Y_PLANEX 15.49
//#define Y_PLANEZ 13.06
#define MCALIMAGING_CONF "/share/mcal_conf/mcalimaging_configuration.conf"
#define NMAX 24

// MCAL geometrical parameters rispetto al centro MCAL
#define X1Z -18.11
#define Z1X -18.11
#define BARSTEP 2.587
#define PSIGMA 1.
#define BARLEN    37.5

// coordinate centro refsys Kalman rispetto al centro MCAL
#define OFFSET_XK 19.4
#define OFFSET_YK -19.4

//#define Y_PLANEX  12.976
//#define Y_PLANEZ  11.476
#define Y_PLANEX 12.406
#define Y_PLANEZ 10.326
#define Y_ST_P1   38.2
#define ST_DY      1.904

// coordinate centro refsys Laben rispetto al centro MCAL
#define XREF 24.2
#define ZREF 21.6

// definizioni per le matrici di "risposta"
#define NBINS		6
/* bins definitions:
	bin 1:    0 -   50 MeV   -  response energy   40 MeV 
	bin 2:   50 -  100 MeV   -  response energy   75 MeV 
	bin 3:  100 -  200 MeV   -  response energy  150 MeV 
	bin 4:  200 -  400 MeV   -  response energy  300 MeV 
	bin 5:  400 - 1000 MeV   -  response energy  700 MeV 
	bin 6: 1000 - 3000 MeV   -  response energy 1500 MeV (TO BE MODIFIED!)
*/
#define NBARSBINX	16
#define NBARSBINZ	16
#define NEBINX		24
#define NEBINZ		24
#define EBIN		10.



struct selection {
	bool isTrigTrack;
	bool doKalmanCorr;
	int minBarX, maxBarX, minBarZ, maxBarZ;
	int minTrX, maxTrX, minTrZ, maxTrZ;
	float minE;
	int nrun;
	TString   *rootfile, *lv1file, *kalmanfile;
	TString    *parname;
	bool	buildResponseMatrix;
	bool	doMcalGuess;
};

class point {
	public:
		int plane;	// 0 == plane X; 1 == plane Z
		int bar;	// 0 == X1 ... 29 == Z15
		float X;
		float Z;
		float xk;	// Kalman x coordinate
		float yk;	// Kalman y coordinate
		float eX;
		float eZ;
		float E;
		
		point();
		point(MCALevt *inevt, int ibar);
		point(bool laben, float x, float z, float energy=0., float errx=0., float errz=0.);
		void setKalmanCoordinate();
		void setLabenCoordinate();
		void show();
};

class track {
	public:
		int plane;	// 0 == plane X; 1 == plane Z
		int evt;	// event number
		int trid;	// track id
		int nbars;	// number of bars
		float Etot;	// total energy
		int ibar;	// initial bar number (0-14 == X1-X15; 15-29 == Z1-Z15)
		float E[15];	// energy per bar
		float pos[15];	// position per bar (MCVLib reference system)
		float pX[15];	// X position (respect to MCAL center, Laben X direction)
		float pZ[15];	// Z position (respect to MCAL center, Laben X direction)
		track *prev;	// pointer to the previous track on the same plane
		
		track();
		track(MCALevt *inevt, int ibar, float eth);
		void print();
		
	protected:
		void convertPosition();
		void calcAvgParam();
};


// init routines
void initBasicHisto(TObjArray *, float *);
void initTrigTrackHisto(TObjArray *, Double_t *, int);
void initKalmanHisto(TObjArray *, Double_t *, int);
void initResponseHisto(TObjArray *, Double_t *, int);
void initMCALCorrectionHisto(TObjArray *, Double_t *, int);

// output routines
void drawBasicHisto(TObjArray *);
void drawTrigTrackHisto(TObjArray *, int, Double_t *);
void drawTrigTrackHisto2(TObjArray *, int, Double_t *);
void drawTrigTrackHisto3(TObjArray *, int, Double_t *);
void drawKalmanHisto(TObjArray *, int, Double_t *);
void drawKalmanTrackHisto(TObjArray *, int, Double_t *);	// draw histograms for Kalman tracks vs MCAL points analysis
void drawKalmanTrackHisto_energy(TObjArray *, int, Double_t *);	// draw histograms for Kalman tracks vs MCAL points analysis
void drawMCALCorrectionHisto(TObjArray *, int, Double_t *);
void drawResponseHisto(TObjArray *, int, Double_t *);
void drawResponseMatrix(TObjArray *hc, selection *sel);

void printTrackResults(TObjArray *, int, selection *, MCVFileInfo *);
int loadSelection(const char *, selection *);
void printerror( int );

// process routines
void processEvents(int, selection *, TTree *, TObjArray *);
void processEvents2(int, selection *, TTree *, TObjArray *, int, int runid=-1);
int getClosestPoint(int nx, point **px, point *p0);
int getClosestPointX(int nx, point **px, point *p0);
float getPointDistance(point *px, point *pz);
float pointFigureOfMerit1(int nx, point **px, int nz, point **pz, float *fom, int *bestx, int *bestz);
point *getEnergyCenterPlane(int n, point **p);
point *getEnergyCenterMCAL(int nx, int nz, point **px, point **pz);
float getEnergyMoment(point *cen, int n, point **p);

// MCAL response and filtering routines
void saveResponseFile(TString *, TObjArray *, int );
void loadResponseFile(TString *);
void printResponseFile();
int  getMCALEnergyBin(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose=FALSE);


#endif
