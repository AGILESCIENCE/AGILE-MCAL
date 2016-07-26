#ifndef PLOTMCALDATA
#define PLOTMCALDATA

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <fstream>
#include <sstream>
#include <math.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TString.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TObjArray.h>
#include <TEnv.h>
#include <TFile.h>
#include <TTree.h>
#include <TText.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TDatime.h>
#include <TTimeStamp.h>
#include <TMath.h>

#define STANDARD_CONF "/share/MCVLib_conf/MCVLib_configuration.conf"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

int loadDataRoot(TFile *fi, int ns, double *time, double *data, double *dataAC);

TCanvas *standardOutput1(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, float xzmax);
TCanvas *standardOutput2(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, float xzmax);
TCanvas *zoomOutput1(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, double tgrbini, int nbefore, int nafter, int orbit, int ib, int fb, bool pub);
TCanvas *grbSpectrum(int ns, double *time, double *data, TTimeStamp epoch, double tgrb0, int orbit, int tbkgini, int tbkgfin, int tgrbini, int tgrbfin);
//int saveLightCurve(int ns, double *time, double *data, TTimeStamp epoch, double tgrbini, int nbefore, int nafter, int orbit, int ib, int fb, int iplane);
int savePhaP1P2(int, float *, float *, float);
int savePha(int, float *, float, TString *);
#endif
