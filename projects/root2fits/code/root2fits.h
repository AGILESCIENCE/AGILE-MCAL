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
#include "MCVFileInfo.h"

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TObjArray.h>
#include <TKey.h>
#include <TString.h>
#include <TTimeStamp.h>

#include <iostream>
#include <string.h>
#include <fstream>
#include "fitsio.h" 

void printerror( int status);

