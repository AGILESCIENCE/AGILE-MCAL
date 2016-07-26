
#include "mcalimaging.h"
#include "version.h"

float rbars[NBINS][NBARSBINX][NBARSBINZ];
float renergy[NBINS][NEBINX][NEBINZ];


double Estep[] = {1., 50., 100., 200., 400., 1000., 3000., 10000.};
float mcalEbin[] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130., 140., 150., 160., 170., 180., 190., 200., 250., 300., 400., 500.};


int main(int argc, char *argv[])
{
	selection sel;
	int nbars, nbarsX, nbarsZ, id, nevents, ntrX, ntrZ, nsel=0;
	float eX, eZ;
	int nEtag = 7;
	
//	Double_t Estep[] = {1., 51., 99., 200., 400., 1000.};

	TStyle *mcalstyle;
	TString   calFile; 

	TObjArray *histo;   
	
	cout << "mcalimaging version " << VERSION << endl;

	if (argc>1) {
		// load configuration file
		calFile += argv[1];
	} else {
		// load default configuration file
		calFile.Append(getenv("MCALSW"));
		calFile += MCALIMAGING_CONF;
	}
	
	if (loadSelection(calFile.Data(), &sel)) return -1;
	
	// set graphics environment
	TApplication theApp("App", &argc, argv);
	mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.04, "xyz");
	mcalstyle->SetTitleSize(0.04, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	// init histogram collection
	histo = new TObjArray(200,0);
	initBasicHisto(histo, mcalEbin);

	// open ROOT file
	TFile *fd;
	TTree *indata = NULL;
	MCVFileInfo *info = NULL;
	initTrigTrackHisto(histo, Estep, nEtag);	
	initKalmanHisto(histo, Estep, nEtag);
	initResponseHisto(histo, Estep, nEtag);
	initMCALCorrectionHisto(histo, Estep, nEtag);
	
//	loadResponseFile(sel.parname);
//	printResponseFile();
	
	for (int ifile=0; ifile<sel.nrun; ifile++) { // 0
		cout << "Reading ROOT file " << (sel.rootfile[ifile]).Data() << endl;
		fd = new TFile((sel.rootfile[ifile]).Data());
		TIter next0 = fd->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next0())) {
			if (strcmp(key->GetClassName(),"TTree") == 0) indata = (TTree *) fd->Get(key->GetName());
			if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fd->Get(key->GetName());
		}
		nevents = indata->GetEntries();
		
		
		processEvents2(ifile, &sel, indata, histo, nEtag, info->runid);
	
		//printTrackResults(histo, nevents, &sel, info);	
	}
	
	// graphic section
	drawBasicHisto(histo);
	if (sel.isTrigTrack) {
		drawTrigTrackHisto(histo, nEtag, Estep);
//		drawTrigTrackHisto2(histo, nEtag, Estep);
//		drawTrigTrackHisto3(histo, nEtag, Estep);
		if (sel.doKalmanCorr) {
			drawKalmanHisto(histo, nEtag, Estep);
		//	drawKalmanTrackHisto(histo, nEtag, Estep);
		//	drawKalmanTrackHisto_energy(histo, nEtag, Estep);
		//	drawResponseHisto(histo, nEtag, Estep);
			drawMCALCorrectionHisto(histo, nEtag, Estep);
		}
	}
	drawResponseMatrix(histo, &sel);
	
//	saveResponseFile(sel.parname, histo, 4);
	
	theApp.Run();
	return 0;
	
}
