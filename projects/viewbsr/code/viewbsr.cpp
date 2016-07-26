#include "MCVFile.h"
#include "MCVFileInfo.h"
#include "MCVbsrMonitor.h"
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
	
	cout << "viewbsr version " << VERSION << endl << "Working on file: " << argv[1] << endl;
	TFile *fh = new TFile(argv[1]);
	
	if(fh->IsZombie())
	{
	   cerr << " File opening failed, file: "<< argv[1] << endl ; 
	   exit(1) ; 
	}
	   
	TApplication theApp("App", &argc, argv);
	
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
	
	rh = (TObjArray * ) fh->Get("rawbsr") ; 
	
	MCVbsrMonitor mainFrame(gClient->GetRoot(), NULL, 1000, 650); 
	
	mainFrame.SetHistoCollection(rh);
	mainFrame.SetFileInfo(info);
//	mainFrame.Set2DLimits(-20., 20., -20., 20.);
	mainFrame.FillRaw();
	mainFrame.PlotAll();
	theApp.Run();

	return 0;
}	
	