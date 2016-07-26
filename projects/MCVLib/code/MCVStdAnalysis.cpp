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

#include "MCVStdAnalysis.h"

ClassImp(MCVStdAnalysis);

MCVStdAnalysis::MCVStdAnalysis(): fout(NULL), fref(NULL), conf(NULL), c1(NULL), c2(NULL), rawHisto(NULL), info(NULL), refname(NULL)
{
	for (int i=0; i<60; i++) outofspec[i] = FALSE;
	status = NO_CALC;
	barmask = 0xFFFFFFFF;
	LoadConf();
}

//
//--------------------------------------------------------------------
//

MCVStdAnalysis::MCVStdAnalysis(TObjArray *hc, MCVFileInfo *in): fout(NULL), fref(NULL), conf(NULL), c1(NULL), c2(NULL), info(NULL), refname(NULL)
{
	for (int i=0; i<60; i++) outofspec[i] = FALSE;
	status = NO_CALC;
	barmask = 0xFFFFFFFF;
	LoadConf();
	if (in) SetFileInfo(in);
	SetHistoCollection(hc);
}

//
//--------------------------------------------------------------------
//

MCVStdAnalysis::~MCVStdAnalysis()
{
	if (refname) delete refname;
}

//
//--------------------------------------------------------------------
//

int MCVStdAnalysis::Calculate()
{
	return 0;
}

//
//--------------------------------------------------------------------
//

bool MCVStdAnalysis::IsOK()
{
	return FALSE;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::PrintComment()
{
	// Write short comment depending on calculation restults
	switch (status) {
		case NO_CALC:
			printf(" no calculation performed yet.\n");
		break;
		case OK:
			printf(" OK.\n");
		break;		
		case WRONG_PKTTYPE:
			printf(" Cannot apply this routine to this packet type.\n");
		break;
		case REF_FILE_NOT_FOUND:
			printf(" Reference file %s not found\n", refname->Data());
		break;
		case REF_HISTO_NOT_FOUND:
			printf(" Reference histogram not found\n");
		break;
		case CONF_FILE_NOT_FOUND: 
			printf(" Main configuration file not found\n");
		break;
		case RUN_INFO_NOT_SET: 
			printf(" Run information not set\n");
		break;
		case RAW_HISTO_NOT_SET: 
			printf(" Raw histogram collection not set\n");
		break;
		case OUT_OF_SPEC: 
			printf(" Out of specification. See detailed results.\n");
		break;
		default:
		break;
	}		
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::PrintResults()
{
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::SaveToFile()
{
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::SetCanvases(TCanvas *a1, TCanvas *a2)
{
	c1=a1;
	c2=a2;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::SetHistoCollection(TObjArray *hc) 
{
	rawHisto = hc;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::SetFileInfo(MCVFileInfo *in) 
{
	info = in;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::LoadConf()
{
	TString   calFile;		
	calFile.Append(getenv("MCALSW"));
	calFile += STANDARD_CONF;
	conf = new TEnv(calFile.Data());
	if (!conf) cout << "*** cannot find standard configuration file " << calFile.Data() << endl;
	return;
}

//
// --------------------------------------------------
//

double MCVStdAnalysis::GetEffectiveTime(Int_t ibar)
{
	// Returns effective time calculated on bar ibar
	Double_t efft=0.; 
	static TObjArray *rCR = NULL;
	if (!rCR) rCR  = (TObjArray *) rawHisto->FindObject("rawCRate");
	TH1F *aux = (TH1F *) rCR->At(ibar);
	if (aux->GetEntries()) {
		int nbins = aux->GetNbinsX();
		for (int i=1; i<nbins; i++) if (aux->GetBinContent(i) > 0) efft += 1.;
	} else efft = -1.;
	return efft;
}

//
// --------------------------------------------------
//

double MCVStdAnalysis::GetEffectiveTime()
{
	// Returns effective time calculated on global CR histo
	Double_t efft=0.;
	static TH1F *totCR = NULL;
	if (!totCR) totCR = (TH1F *)  rawHisto->FindObject("totCR");
	if (totCR->GetEntries()) {
		int nbins = totCR->GetNbinsX();
		for (int i=1; i<nbins; i++) if (totCR->GetBinContent(i) > 0) efft += 1.;
	} else efft = -1.;
	return efft;
}

//
//--------------------------------------------------------------------
//

void MCVStdAnalysis::UpdateGlobalFile()
{
	return;
}

//
//--------------------------------------------------------------------
//

TGraphErrors *MCVStdAnalysis::CreateGraphErrors(char *gname)
{
	//cout << "*** there is no graph!" << endl;
	TGraphErrors *aux;
	aux = new TGraphErrors(0);
	aux->SetName(gname);
	aux->SetTitle(gname);
	return aux;
}

//
//--------------------------------------------------------------------
//

TGraph *MCVStdAnalysis::CreateGraph(char *gname)
{
	//cout << "*** there is no graph!" << endl;
	TGraph *aux;
	aux = new TGraph(0);
	aux->SetName(gname);
	aux->SetTitle(gname);
	return aux;
}