#include "plotmcaldata.h"
#include "version.h"

using namespace std ; 

int main(int argc, char *argv[])
{
	
	TApplication *theApp;
	TString outpath;
	TString histoname;
	bool batch=FALSE;
	
	cout << "plotmcaldata version " << VERSION << endl;
	
	// load std configuration
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	TEnv *conf = new TEnv(confname);
	
	// cout help on command syntax
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-b : batch mode (save png images of output)" << endl;
		cout << "-p <path> : set output path for png images in batch mode" << endl;
		cout << "-f <file_name> : open histogram file <file_name>" << endl;
		cout << "-r <n> : rebin histograms with <n> pitch (default is 1)" << endl;
		cout << "-z <TT time> : zoom around time (s)" << endl;
		cout << "-nb <n bins> : number of bins shown before (to be used with the -z option)" << endl;
		cout << "-na <n bins> : number of bins shown after (to be used with the -z option)" << endl;
		cout << "-c <contact n.> : contact number" << endl;
		cout << "-ib <band n.> : initial energy band (default 2: 0.35-0.70 MeV, to be used with the -z option)" << endl;
		cout << "-sb <band n.> : final energy band (default 5: 2.8-5.6 MeV, to be used with the -z option)" << endl;
		cout << "-ipn <plane (0=X 1=Z)>: save a light curve file formatted for IPN triangulation, for plane <plane>" << endl;
		cout << "-sp <tbkg_ini> <tbkg_fin> <tgrb_ini> <tgrb_fin>: plot bkg subtracted count spectrum. Times are relative to <TT time> (to be used with the -z option)" << endl;
		cout << "-eps : b/w publication style" << endl;
		return 0;
	}
	
	// parse command line data
	int iopt=1;
	int bin = 1;		// rebin factor 
	double xzmax = 160.; 	// Y range limit for plots
	int nbefore = 50;
	int nafter = 50;
	int orbit = 0;
	int ib = 2;		// default initial energy band for grb plot
	int fb = 5;		// default final energy band for grb plot
	double tgrb0=0.;
	int tbkgini=0.;		// bkg initial relative time (since contact start)
	int tbkgfin=0.;		// bkg final relative time
	int tgrbini=0.;		// grb initial relative time
	int tgrbfin=0.;		// grb final relative time
	bool plotSpectrum=FALSE;
	bool pub=FALSE;
	int iplane=-1;		// MCAL plane index (0=X, 1=Z) for saving light curves in IPN format
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		if (cmd.Contains("-b")) batch=TRUE;
		if (cmd.Contains("-eps")) pub=TRUE;
		if (cmd.Contains("-p")) outpath += argv[++iopt];
		if (cmd.Contains("-f", TString::kExact)) histoname += argv[++iopt];
		if (cmd.Contains("-r")) {
			TString saux(argv[++iopt]);
			bin = saux.Atoi();
		}
		if (cmd.Contains("-z")) {
			TString saux(argv[++iopt]);
			tgrb0 = saux.Atoi();
		}
		if (cmd.Contains("-nb")) {
			TString saux(argv[++iopt]);
			nbefore = saux.Atoi();
		}
		if (cmd.Contains("-na")) {
			TString saux(argv[++iopt]);
			nafter = saux.Atoi();
		}
		if (cmd.Contains("-c")) {
			TString saux(argv[++iopt]);
			orbit = saux.Atoi();
		}
		if (cmd.Contains("-ib")) {
			TString saux(argv[++iopt]);
			int tmpb = saux.Atoi();
			if (tmpb<1 || tmpb>11) cout << "Wrong initial band argument: must be 1 <= ib <=11" << endl;
			else ib = tmpb;
		}
		if (cmd.Contains("-sb")) {
			TString saux(argv[++iopt]);
			int tmpb = saux.Atoi();
			if (tmpb<1 || tmpb>11) cout << "Wrong final band argument: must be 1 <= fb <=11" << endl;
			else fb = tmpb;
		}
		if (cmd.Contains("-ipn")) {
			TString saux(argv[++iopt]);
			iplane = saux.Atoi();
			if (iplane<0 || iplane>1) {
				cout << "Wrong plane assignment: must be 0=X, 1=Z. Setting default to plane X" << endl;
				iplane=0;
			}
		}
		if (cmd.Contains("-sp")) {
			plotSpectrum = TRUE;
			TString saux1(argv[++iopt]);
			tbkgini = saux1.Atoi();
			TString saux2(argv[++iopt]);
			tbkgfin = saux2.Atoi();
			TString saux3(argv[++iopt]);
			tgrbini = saux3.Atoi();
			TString saux4(argv[++iopt]);
			tgrbfin = saux4.Atoi();
		}
		iopt++;
	}
	if (fb<ib) {
		cout << "wrong band assignment: must be ib<=fb. Resetting to default" << endl;
		ib=2;
		fb=5;
	}
	
	// set graphics environment
	if (!batch)  theApp = new TApplication("App", &argc, argv);
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	if (tgrb0) {	// good for zoomed views
		mcalstyle->SetLabelSize(0.08, "xyz");
		mcalstyle->SetTitleSize(0.08, "xyz");
		mcalstyle->SetPadBottomMargin(0.20);
		mcalstyle->SetPadTopMargin(0.05);
	} else {	// good for standard views
		mcalstyle->SetLabelSize(0.06, "xyz");
		mcalstyle->SetTitleSize(0.06, "xyz");
	}
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();

	// load global histo file
	if (histoname.Length() == 0) {
		histoname.Append(getenv("MCALSW"));
		histoname += conf->GetValue(".global.history.file.", "./global.root");
	}
	TFile *fi= new TFile(histoname.Data());
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << histoname.Data() << " is not a valid ROOT file: exiting!" << endl;
		return 0;
	}

	TGraph *gaux = (TGraph *) fi->Get("grmX1");
	int ns = gaux->GetN();
	double *time = gaux->GetX();
	double *data = new double[22*ns];
	double *dataAC = new double[5*ns];
	
	loadDataRoot(fi, ns, time, data, dataAC);
	cout << ns << "  " << time[0] << "  " << time[ns-1] << endl;
	int nbins = (int) ((time[ns-1] - time[0])/1.024) + 1;

	// works fine only for corrected TT data now
//	TDatime epoch(2004, 1, 1, 0, 1, 6.184);
//	TDatime epoch(2003, 12, 31, 23, 1, 6.184);	// MJD ref. -1h. why? time zone? boh???

//	TDatime epoch(2003, 12, 31, 23, 0, 1);		// buono per ora LEGALE (estate)
//	TDatime epoch(2004, 1, 1, 0, 0, 0);		// buono per ora SOLARE (inverno). c'e' o no 1 sec???
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	
	cout << "Epoch = " << epoch.GetSec() << endl;
	printf("time[0] = %f  -  ", time[0]);
	UInt_t telapsed = (UInt_t) time[0] ;
//	TDatime ts(telapsed + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int) telapsed , 0, epoch.GetSec(),0);
	cout << ts.AsString() << endl;
		
	TCanvas *c1 = standardOutput1(ns, time, data, dataAC, bin, epoch, xzmax);
	TCanvas *c2 = standardOutput2(ns, time, data, dataAC, bin, epoch, xzmax);
	
	TCanvas *c3 = NULL;
	if (tgrb0>0) c3 = zoomOutput1(ns, time, data, dataAC, bin, epoch, tgrb0, nbefore, nafter, orbit, ib, fb, pub);
	
	TCanvas *c4 = NULL;
	if (plotSpectrum && tgrb0>0) {
		c4 = grbSpectrum(ns, time, data, epoch, tgrb0, orbit, tbkgini, tbkgfin, tgrbini, tgrbfin);
	}
	//if (iplane != -1) saveLightCurve(ns, time, data, epoch, tgrb0, nbefore, nafter, orbit, ib, fb, iplane);
	
	// save output images if required
	if (outpath.Length() > 0 && outpath.EndsWith("/")==FALSE) outpath += "/";
	if (batch) {
		if (!c3) {	// no -z option: save all orbit ratemeters images
			TString c1out(outpath);
			c1out += "amcal_sci-rm_1.png";
			c1->Print(c1out.Data());
			
			TString c2out(outpath);
			c2out += "amcal_sci-rm_2.png";
			c2->Print(c2out.Data());
		} else {	// -z option: save only zoomed light curve
			TString c3out(outpath);
			//TDatime ts(tgrb0 + epoch.Convert());
			TTimeStamp ts;
			ts.Set((int) tgrb0 , 0, epoch.GetSec(),0);
			// get time0 information
			char caux[10];
			int year, month, day, hour, min, sec;
			sprintf(caux, "%d", ts.GetDate(1));
			sscanf(caux, "%4d%2d%2d", &year, &month, &day);
			sprintf(caux, "%06d", ts.GetTime(1));
			sscanf(caux, "%2d%2d%2d", &hour, &min, &sec);

			char pname[100];
			sprintf(pname, "MCAL_%02d%02d%02d_%02d-%02d-%02d.png", year-2000, month, day, hour, min, sec);
			c3out += pname;
			cout << "saving zoomed light curves to file " << c3out.Data() << endl;
			c3->Print(c3out.Data());		
		}
		
		double *totx = new double[ns];
		double *totz = new double[ns];
		for (int j=0; j<ns; j++) {
			totx[j] = totz[j] = 0.;
			for (int i=1; i<11; i++) totx[j] += data[j*22 + i];
			for (int i=12; i<22; i++) totz[j] += data[j*22 + i];
		}
		
		TString frmname(outpath);
		frmname += "scirm.dat";
		FILE *frm;
		frm = fopen(frmname.Data(), "w");
		fprintf(frm, "%20.6f \t%d \nid \tt-t0 \t\tXtot \tZtot \t", time[0], ns);
		for (int i=1; i<11; i++) fprintf(frm, "X%02d \t", i);
		for (int i=1; i<11; i++) fprintf(frm, "Z%02d \t", i);
		for (int i=0; i<5; i++) fprintf(frm, "AC%02d \t", i);
		fprintf(frm, "\n");
		for (int j=0; j<ns-2; j++) {
			fprintf(frm, "%5d \t%10.3f \t%6.0f \t%6.0f \t", j, time[j]-time[0], totx[j], totz[j]);
			for (int i=1; i<11; i++)  fprintf(frm, "%6.0f \t", data[j*22 + i]);
			for (int i=12; i<22; i++) fprintf(frm, "%6.0f \t", data[j*22 + i]);
			for (int i=0; i<5; i++)   fprintf(frm, "%6.0f \t", dataAC[j*5+i]);
			fprintf(frm, "\n");
		}
		fclose(frm);
		
	} else  theApp->Run();
	return 0;
}