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

#ifndef MCALGRID
#define MCALGRID

#include "MCVFileInfo.h"
#include "MCVFile.h"

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TObjArray.h>
#include <TKey.h>
#include <TString.h>
#include <TTree.h>
#include <TLegend.h>
#include <TGraph.h>

#include <iostream>
#include <fstream>
#include <fitsio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>

//#define NCOL 18		// <-- vecchia versione
#define NCOL 30

// Y dei piani di MCAL (centro barra) secondo geometria standard
//#define Y_PLANEX 15.206
//#define Y_PLANEZ 13.706
//#define XREF -23.5
//#define ZREF -21.5

// Y dei piani di MCAL (centro barra) secondo risultati sperimentali
//#define Y_PLANEX 12.976
//#define Y_PLANEZ 11.476

// Y dei piani di MCAL (centro barra) secondo risultati sperimentali (analisi del 10/10/2006)
//#define Y_PLANEX 12.406
//#define Y_PLANEZ 10.326
#define XREF -24.2
#define ZREF -21.6

// Y dei piani di MCAL (centro barra) secondo risultati sperimentali (analisi del 10/10/2006) 
// traslati secondo modifiche QL (+2.958 cm), 7/11/06
#define Y_PLANEX 15.364
#define Y_PLANEZ 13.284

#define MCALGRID_CONF "/share/mcalgrid_conf/mcalgrid_configuration.conf"

// initialization routines
void initGridHisto(TObjArray *);
void initMcalHisto(TObjArray *);
void initCalibrationHisto(TObjArray *);

// output routines
void drawGridHisto(TObjArray *);
void drawMcalHisto(TObjArray *);
void drawCalibrationHisto(TObjArray *, TObjArray *);



// output routines

void printerror( int );

#endif
