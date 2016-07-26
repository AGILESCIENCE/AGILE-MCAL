#ifndef MAKESPECTRA
#define MAKESPECTRA

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <fstream>
#include <sstream>

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
#include <TKey.h>

#include "MCVFileInfo.h"
#include "GRB3908.h"
#include "GRB3913XZ.h"

#include "EChannels.h"


#define STANDARD_CONF "/share/MCVLib_conf/MCVLib_configuration.conf"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct trg {
	int runid;
	int trg;
	float counts;
	float cpeak;
	int subms;
	float hr;
	float lon;
	float lat;
	float dt;
	double t0;
	float t90;
	float dt0;	// time difference of the found trigger with respect to the initial t0 (bstart)
	float tgrbini;
};

int savePha(int, float *, float, TString *);
TCanvas *plotSpectrum(int nb, double *spbins, float *spbkg, float dtbkg, float *spgrb, float dtgrb);

#endif

