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

#include "MCVFile.h"
#include "MCVFilePKP.h"
#include "MCVFileTE.h"
#include "MCVFileInfo.h"
#include "MCVTree.h"
#include "MCVRawMonitor.h"
#include "MCVGridOffset.h"
#include "MCVCountRate.h"
#include "MCVRetriggering.h"
#include "MCVSciRM.h"
#include "MCVEnergyThreshold.h"
#include "MCVGridEnergyCal.h"
#include "MCVAntiCoincidence.h"
#include "MCVGlobalRetriggering.h"
#include "version.h"

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TObjArray.h>
#include <TKey.h>
#include <TString.h>
#include <TTimeStamp.h>

#include <fitsio.h>
#include <iostream.h>
#include <string.h>
#include <fstream>

void printerror( int status);

int process3908(fitsfile *fptr, TString *boostfile, TString *grbfilename);
