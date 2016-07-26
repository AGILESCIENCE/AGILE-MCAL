//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "MCVAntiCoincidence.h"
ClassImp(MCVAntiCoincidence);

MCVAntiCoincidence::MCVAntiCoincidence() : MCVStdAnalysis()
{
	herate = -1.;
	minbin = (int) MIN_E/E_BIN +1;
	maxbin = (int) MAX_E/E_BIN +1;
}

//
//--------------------------------------------------------------------
//

MCVAntiCoincidence::MCVAntiCoincidence(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	herate = -1.;
	minbin = (int) (MIN_E/E_BIN) +1;
	maxbin = (int) (MAX_E/E_BIN) +1;
}

//
//--------------------------------------------------------------------
//

MCVAntiCoincidence::~MCVAntiCoincidence()
{

}

//
//--------------------------------------------------------------------
//

int MCVAntiCoincidence::Calculate()
{
	TH1F *totE=NULL;
	
	if (rawHisto==NULL || info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	totE =(TH1F *) rawHisto->FindObject("totE");
	if (totE==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}

	herate = totE->Integral(minbin, maxbin);
		
	// load reference values and make comparison
	
	herateref = conf->GetValue(".max.acvetoed.herate.", -1.);
	if (herate <= herateref) status = OK;
	else status = OUT_OF_SPEC;
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVAntiCoincidence::PrintResults()
{
	printf("\n--------------------- ANTICOINCIDENCE CHECK ----------------------\n");
	if (status == OK || status == OUT_OF_SPEC) {
		printf("\nEvents rate for %.1f MeV < E < %.1f MeV = %.2f evt/s \n", MIN_E, MAX_E, herate);
		printf("Maximum allowed rate for AC-vetoed measurements = %.2f evt/s \n", herateref);
		if (status == OK) printf("AC veto is ACTIVE\n");
		else printf("AC veto is NOT ACTIVE\n");
	}
	
	return;
}
