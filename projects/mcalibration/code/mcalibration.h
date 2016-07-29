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

#ifndef MCALIBRATION
#define MCALIBRATION

#include "MCVFileInfo.h"
#include "MCVCalibration.h"

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
#include <fitsio.h>
#include <stdlib.h>
#include <math.h>


void drawPdGraphics(MCVCalibration* calib);
void drawLightAsymmetry(MCVCalibration* calib);
#endif
