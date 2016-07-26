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


#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TString.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TGraph.h>
#include <TH2F.h>
//#include <TDatime.h>
#include <TTimeStamp.h>
#include <TLine.h>
#include <TText.h>
#include <TLegend.h>
#include <TMath.h>

#include <iostream.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>

TCanvas *hkTrgOutput(TFile *fh, TTimeStamp epoch);
TCanvas *hkTrgStatistics(TFile *fh, TTimeStamp epoch, TString *boostfile=NULL);

