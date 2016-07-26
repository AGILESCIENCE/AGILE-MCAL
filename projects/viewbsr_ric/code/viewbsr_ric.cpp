// last update: 26-june-2006
// view bsr reconstricted from EVENTS  extension of 3908 L1 fits 
// M.galli, Martino Marisaldi, june 2006

#include "MCVFile.h"
#include "MCVFileInfo.h"
#include "MCVbsrMonitor_ric.h"
#include "version.h"

#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TEnv.h>
#include <TKey.h>
#include <TObjArray.h>
#include <string.h>



int main(int argc, char *argv[])
{
	MCVFileInfo *info = NULL;
	TObjArray *rh = NULL;
	TObjArray *bh = NULL;
	TString rootfilename;
	TApplication *theApp;
	
	bool batch=FALSE;
	
	cout << "viewbsr_ric version " << VERSION << endl;
	
	// cout help on command syntax
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-batch : work in batch mode, save .gif light curve" << endl;
		cout << "-r <filename> : load ROOT event file" << endl;
		return 0;
	}
	
	// ------------- parse command line data -------------------
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
		// work in batch mode?
		if (cmd.Contains("-batch", TString::kExact)) batch = TRUE;
		
		// input ROOT file
		if (cmd.Contains("-r", TString::kExact)) rootfilename += argv[++iopt];
		
		iopt++;
	}
	
	// ------------- main execution -------------------
	theApp = new TApplication("App", &argc, argv);		//must have an application, otherwise segmentation!
	if (!batch)  {
	//	theApp = new TApplication("App", &argc, argv);
		cout << "Work in interactive mode" << endl;
	} else cout << "Work in batch mode" << endl;
	
	cout << "Working on file: " << rootfilename.Data() << endl;
	TFile *fh = new TFile(rootfilename);
	if(fh->IsZombie())
	{
	   cerr << " File opening failed, file: "<< rootfilename.Data() << endl ; 
	   exit(1) ; 
	}
	   
	TIter next = fh->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next()))
        {
		printf("key: %s \t\t\tclass name: %s\n", key->GetName(), key->GetClassName());
//		if (strcmp(key->GetClassName(),"TObjArray") == 0) rh = (TObjArray *) fh->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fh->Get(key->GetName());
	}
	if (info) 
	{
		info->PrintFileInfo();
		info->PrintCalibrationParameters();
	}
	
	rh = (TObjArray * ) fh->Get("rawbsr_ric") ;
	bh = (TObjArray * ) fh->Get("burstArray") ;
	
	if(rh->IsEmpty())
	{
	   cerr << " Reconstructed Histos not found in file "<< argv[1] << endl ; 
	   exit(2) ; 	  
	}
	
	MCVbsrMonitor_ric mainFrame(gClient->GetRoot(), NULL, 1000, 650); 
	
	mainFrame.SetHistoCollection(rh);
	mainFrame.SetBurstCollection(bh);
	mainFrame.SetFileInfo(info);
//	mainFrame.Set2DLimits(-20., 20., -20., 20.);
	mainFrame.FillRaw();
	mainFrame.PlotAll();
	mainFrame.PrintRMTable(10., 60.);
	
	if (batch) {
		//to do
	} else  {
		theApp->Run();
	}

	return 0;
}	
	
