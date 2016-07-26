#include "MCVFile.h"
#include "MCVFileInfo.h"
#include "MCVRawMonitor.h"
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
	TObjArray *rh = NULL;		// standard histogram collection
	cout << endl << "Viewrun version " << VERSION << endl;
	cout << "Working on file: " << argv[1] << endl;
	TFile *fh = new TFile(argv[1]);
	TString ftitle("File: ");
	ftitle += argv[1];
	ftitle += "  -  Viewrun version ";
	ftitle += VERSION;
	TApplication theApp("App", &argc, argv);
	TIter next = fh->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next())) {
//		printf("key: %s \t\t\tclass name: %s\n", key->GetName(), key->GetClassName());
//		if (strcmp(key->GetClassName(),"TObjArray") == 0) rh = (TObjArray *) fh->Get(key->GetName());
		if (strncmp(key->GetName(),"rawHisto", 8) == 0) rh = (TObjArray *) fh->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fh->Get(key->GetName());
	}
	if (info) {
		info->PrintFileInfo();
		info->PrintCalibrationParameters();
	}
//	rh = (TObjArray *) fh->Get("rawHisto");
	if (rh==NULL) {
		cout << "*** ERROR: no TObjArray rawHisto found" << endl;
		return 1;
	}
	if (info==NULL) {
		cout << "*** ERROR: no MCVFileInfo object found" << endl;
		return 1;
	}
	MCVRawMonitor mainFrame(gClient->GetRoot(), NULL, 800, 600); 
	mainFrame.SetWindowName(ftitle.Data());
	mainFrame.SetHistoCollection(rh);
	mainFrame.SetFileInfo(info);
//	mainFrame.Set2DLimits(-20., 20., -20., 20.);
	mainFrame.FillRaw();	
	
	theApp.Run();

	return 0;
}
