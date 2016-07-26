#include "MCVFile.h"
#include "MCVFileInfo.h"
#include "MCVSelectionGUI.h"
#include "MCVTree.h"
#include "version.h"

#include <TROOT.h>
#include <TH1.h>
#include <TApplication.h>
#include <TFile.h>
#include <TKey.h>
#include <TEnv.h>
#include <TObjArray.h>

int main(int argc, char *argv[])
{	
	bool batch=FALSE;
	bool muon=FALSE;
	TString rootfilename;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	
	
	TH1::AddDirectory(kFALSE);
	TFile *fd, *fh;
	TTree *tdata = NULL;
	TObjArray *rh = NULL;
	TEnv *conf;
	
	MCVFileInfo *info = NULL;
	MCVTree *newdata;
	MCVSelection sel;
	int options = 0;
	
	cout << endl << "mcalselect version " << VERSION << endl;
	TString wtitle("File: ");
	wtitle += argv[1];
	wtitle += "  -  mcalselect version ";
	wtitle += VERSION;
	
	// cout help on command syntax
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-batch : work in batch mode, save .gif light curve" << endl;
		cout << "-r <filename> : load ROOT event file " << endl;
		cout << "-eband <E_min> <E_max> : set energy interval (MeV)" << endl;
		cout << "-bars <0xIIII>  <0xIIII>: set selected bars (two words of 4 hex digits: MS X-plane, LS Z-plane)" << endl;
		cout << "-time <time_start> <time_stop>: time interval with respect to time start" << endl;
		cout << "-muon : do muon track selection (may be obsolete)" << endl;
		return 0;
	}
	
	// ------------- parse command line data -------------------
	
	sel.SetDefault();
	for (int i=0; i<30; i++) (sel.bars)[i] = 1;
	sel.nbars = 30;
	sel.SetName("sel_0");
	
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
		// work in batch mode?
		if (cmd.Contains("-batch", TString::kExact)) batch = TRUE;
		
		// do muon track selection?
		if (cmd.Contains("-muon", TString::kExact)) muon = TRUE;
		
		// input ROOT file 
		if (cmd.Contains("-r", TString::kExact)) rootfilename += argv[++iopt];
				
		// input energy selection
		if (cmd.Contains("-eband")) {
			TString saux1(argv[++iopt]);
			double tmpemin = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmpemax = saux2.Atof();
			if (tmpemin<0. || tmpemax<0. || tmpemin>tmpemax) cout << "Wrong energy band assignment. Ignore energy selection." << endl;
			else {
				sel.energyEnable = TRUE;
				sel.minE = tmpemin;
				sel.maxE = tmpemax;
			}
		}
		
		// input bars selection
		if (cmd.Contains("-bars")) {
			TString saux1(argv[++iopt]);
			sscanf(saux1.Data(), "%X", &barsX);
			TString saux2(argv[++iopt]);
			sscanf(saux2.Data(), "%X", &barsZ);
			int barid = 0X0001;
			sel.nbars = 0;
			for (int i=0; i<15; i++) {
				if (barid & barsX) {
					(sel.bars)[i] = 1;
					(sel.nbars)++;
				}
				else (sel.bars)[i] = 0;
				if (barid & barsZ) {
					(sel.bars)[i+15] = 1;
					(sel.nbars)++;
				}
				else (sel.bars)[i+15] = 0;
				barid<<=1;
			}
		}
		
		// input time interval
		if (cmd.Contains("-time")) {
			TString saux1(argv[++iopt]);
			double tmpt0 = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmptpre = saux2.Atof();
			if (tmpt0<0. || tmptpre<tmpt0) cout << "Wrong time interval. Ignore time selection." << endl;
			else {
				sel.timeEnable = TRUE;
				sel.minTime = tmpt0;
				sel.maxTime = tmptpre;
			}
		}
		iopt++;
	}

	// read dataset
	fd = new TFile(rootfilename, "read");
	if (!fd->IsOpen()) {
		cout << "*** File " << rootfilename.Data() << " cannot be opened or is not a valid ROOT file" << endl;
		return -1;
	}
	TIter next0 = fd->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next0())) {
		if (strcmp(key->GetClassName(),"TTree") == 0) tdata = (TTree *) fd->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fd->Get(key->GetName());
	}
	if (!tdata) {
		cout << "*** Cannot find the data tree" << endl;
		return -1;
	} 
	if (!info) {
		cout << "*** Cannot find the run information" << endl;
		return -1;
	}
	
	// load the Histogram file
	TString   calFile;		
	calFile.Append(getenv("MCALSW"));
	calFile += STANDARD_CONF;
	conf = new TEnv(calFile.Data());
	if (!conf) cout << "*** cannot find standard configuration file " << calFile.Data() << endl;
	TString hname(rootfilename), refname;
	hname.Remove(0,2);
	hname.Prepend("H");
	refname.Append(getenv("MCALSW"));
	refname.Append(conf->GetValue(".root.histo.path.", "H/"));
	refname.Append(hname);
//	cout << "Opening Histogram file " << refname.Data() << endl;
	fh = new TFile(refname, "read");
	if (!fd->IsOpen()) {
		cout << "*** Histogram file " << refname.Data() << " cannot be opened or is not a valid ROOT file" << endl;
		return -1;
	}
	
	TIter next = fh->GetListOfKeys();
	while ((key = (TKey *)next())) {
		if (strncmp(key->GetName(),"rawHisto", 8) == 0) rh = (TObjArray *) fh->Get(key->GetName());
	}
	if (!rh) {
		cout << "*** Cannot find the histogram array" << endl;
		return -1;
	}
	info->PrintFileInfo();	
	
	if (batch) {
		sel.PrintSelection();
		newdata = new MCVTree(tdata, info, rh);
		if (muon) newdata->MuonTrackSelection();
		else newdata->DoSelection(&sel);
		newdata->SaveData();
		delete newdata;
		cout << "All data processed" << endl;
	} else {
		TApplication theApp("App", &argc, argv);
		MCVSelectionGUI mainSelection(gClient->GetRoot(), NULL, 800, 600, tdata, info, rh); 
		mainSelection.SetWindowName(wtitle.Data());
		theApp.Run();
	}
	return 0;
}
