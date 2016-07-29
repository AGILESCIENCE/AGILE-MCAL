#include "MCVFileInfo.h"
#include "MCVFile.h"
#include "MCVTree.h"

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

#include <iostream>
#include <fitsio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	TString rootfile1, rootfile2, hfile1, hfile2;
	TTree *indata1, *indata2;
	TFile *fd1, *fh1, *fd2;
	TObjArray *rh1;
	TKey *key;
	
	MCVFileInfo *info1, *info2;
	MCVTree *newdata;
	
	TH1::AddDirectory(kFALSE);
	char tmp[200];
	
	rootfile1.Append(argv[1]);
	rootfile2.Append(argv[2]);
	
//	info1->PrintFileInfo();
	
	hfile1 += argv[1];
	hfile1.Replace(0, 2, "H");
	cout << "Reading ROOT histogram file #1 " << hfile1.Data() << endl;
	fh1 = new TFile(hfile1.Data(), "UPDATE");
	TIter next0 = fh1->GetListOfKeys();
	while ((key = (TKey *)next0())) {
		if (strcmp(key->GetClassName(),"TObjArray") == 0) rh1 = (TObjArray *) fh1->Get(key->GetName());
	}

	// load root file #2
	cout << "Reading ROOT file #2 " << rootfile2.Data() << endl;
	fd2 = new TFile(rootfile2.Data());
	next0 = fd2->GetListOfKeys();
	while ((key = (TKey *)next0())) {
		if (strcmp(key->GetClassName(),"TTree") == 0) indata2 = (TTree *) fd2->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info2 = (MCVFileInfo *) fd2->Get(key->GetName());
	}
//	info2->PrintFileInfo();

	// load root file #1
	cout << "Reading ROOT file #1 " << rootfile1.Data() << endl;
	fd1 = new TFile(rootfile1.Data(), "UPDATE");
	next0 = fd1->GetListOfKeys();
	while ((key = (TKey *)next0())) {
		if (strcmp(key->GetClassName(),"TTree") == 0) indata1 = (TTree *) fd1->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info1 = (MCVFileInfo *) fd1->Get(key->GetName());
	}
	
	// merge datasets
	newdata = new MCVTree(indata1, info1, rh1, indata2, info2);
	
	fd1->cd();
	indata1->Write(indata1->GetName(), TObject::kOverwrite);
	info1->Write(info1->GetName(), TObject::kOverwrite);
	fh1->cd();
	rh1->Write("rawHisto", TObject::kOverwrite | TObject::kSingleKey);
	info1->Write(info1->GetName(), TObject::kOverwrite);
	
	fh1->Close();
	fd1->Close();
	fd2->Close();
	
	delete newdata;

	return 0;
}
