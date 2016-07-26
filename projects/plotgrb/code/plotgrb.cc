
#include "plotgrb.h"
#include "version.h"


using namespace std ; 

int main(int argc, char *argv[])
{
	
	TApplication *theApp;
	TString outpath;
	TString rootfilename;
	TString chfilename;
	TCanvas *cspectrum = NULL;
	MCVFileInfo 		*info = NULL;		// dataset basic information
	
	bool batch=FALSE;
	bool saveIPN=FALSE;
	bool saveGIF=FALSE;
	bool def=FALSE;
	bool diagnostic=FALSE;
	bool spectra=FALSE;
	bool doStdAnalysis=TRUE;
	bool doCustomAnalysis=FALSE;
	bool subms=FALSE;
	
	double emin=0., emax=2000., tbin=0.032;	// default selection parameters
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	double t0=0.;
	double tpre=-20.;
	double tpost=+50.;
	double tbefore=-10.;
	double tafter=+10.;
	double zbefore=0.;
	double zafter=0.;
	
	double tbkgini=-80.;
	double tbkgfin=-10.;
	double tgrbini=0.;
	double tgrbfin=+15.;
	double tsearch=30.;
	
	double t90;
	double dt;		// time window search (s) for subms flag
	int thr;		// static threshold for subms flag
	
	
	cout << endl << "plotgrb version " << VERSION << endl;
	
	// load std configuration
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	TEnv *conf = new TEnv(confname);
	
	// cout help on command syntax
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-batch : work in batch mode, save .gif light curve" << endl;
		cout << "-r <filename> : load ROOT event file" << endl;
		cout << "-o <filepath>: save output in the specified path" << endl;
		cout << "-z <t_before> <t_after>: zoom light curves in the specified range" << endl;
		cout << "-default : plot default light curves" << endl;
		cout << "-eband <E_min> <E_max> : set energy interval (MeV)" << endl;
		cout << "-bars <0xIIII>  <0xIIII>: set selected bars (two words of 4 hex digits: MS X-plane, LS Z-plane)" << endl;
		cout << "-tbin <time_bin> : set time binning (seconds)" << endl;
		cout << "-t0 <time_zero> <pre_burst> <post_burst>: initial TT time (seconds); if set light curve is refered to t0" << endl;
		cout << "-ipn <t_before> <t_after>: save light curve in IPN format on <outpath>, from t0+t_before to t0+t_after" << endl;
		cout << "-diag : make standard diagnostics, (set bkg and GRB time intervals with proper options)" << endl;
		cout << "-sp : make spectra for XSPEC, (set bkg and GRB time intervals with proper options)" << endl;
		cout << "-ch <channel_filename>: load file with energy channels specs (default is canali_1.dat)" << endl;
		cout << "-save : save .gif light curve" << endl;
		cout << "-bkg <t_bkg_ini> <t_bkg_fin>: set background time interval, time referred to t0" << endl;
		cout << "-grb <t_grb_ini> <t_grb_fin>: set GRB time interval, time referred to t0" << endl;
		cout << "-an : do custom analysis, (set bkg and GRB time intervals with proper options)" << endl;
		cout << "-noan : do NOT do standard analysis" << endl;
		cout << "-subms <dt> <thr>: perform dedicated analysis for sub-ms triggers, search <thr> events in <dt> time window " << endl;
		cout << "-ts <t_search (s)> : search for peak ONLY in interval t0 +/- t_search" << endl;
		return 0;
	}
	
	// ------------- parse command line data -------------------
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
		// work in batch mode?
		if (cmd.Contains("-batch", TString::kExact)) batch = TRUE;
		
		// work in batch mode?
		if (cmd.Contains("-save", TString::kExact)) saveGIF = TRUE;
		
		// is output path specified?
		if (cmd.Contains("-o", TString::kExact)) outpath += argv[++iopt];
		
		// input ROOT file
		if (cmd.Contains("-r", TString::kExact)) rootfilename += argv[++iopt];
		
		// prepare default light curves?
		if (cmd.Contains("-def", TString::kExact)) def = TRUE;
		
		// do standard diagnostics?
		if (cmd.Contains("-diag", TString::kExact)) diagnostic = TRUE;
		
		// prepare spectra for fitting?
		if (cmd.Contains("-sp", TString::kExact)) spectra = TRUE;
		
		// input channel file for building spectra
		if (cmd.Contains("-ch", TString::kExact)) chfilename += argv[++iopt];
		
		// input channel file for building spectra
		if (cmd.Contains("-an", TString::kExact)) {
			doCustomAnalysis = TRUE;
			doStdAnalysis = FALSE;
		}
		
		// input channel file for building spectra
		if (cmd.Contains("-noan", TString::kExact)) {
			doStdAnalysis = FALSE;
		}
		
		// input energy band
		if (cmd.Contains("-eband")) {
			TString saux1(argv[++iopt]);
			double tmpemin = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmpemax = saux2.Atof();
			if (tmpemin<0. || tmpemax<0. || tmpemin>tmpemax) cout << "Wrong energy band assignment" << endl;
			else {
				emin = tmpemin;
				emax = tmpemax;
			}
			cout << "Selected energy band: " << emin << " - " << emax << " MeV" << endl;
		}
		
		// input time binning
		if (cmd.Contains("-tbin")) {
			TString saux(argv[++iopt]);
			double tmptbin = saux.Atof();
			if (tmptbin<0.) cout << "Wrong time binning assignment" << endl;
			else tbin = tmptbin;
			cout << "Selected time bin: " << tbin << endl;
		}
		
		// input bars selection
		if (cmd.Contains("-bars")) {
			TString saux1(argv[++iopt]);
			sscanf(saux1.Data(), "%X", &barsX);
			TString saux2(argv[++iopt]);
			sscanf(saux2.Data(), "%X", &barsZ);
			cout << "Selected bars: Plane X: "; 
			for (int i=0X4000; i>0; i>>=1) cout << ((i & barsX) ? 'X' : '-');
			cout << "  Plane Z: ";
			for (int i=0X4000; i>0; i>>=1) cout << ((i & barsZ) ? 'X' : '-');
			cout << endl;
		}
		
		// input t0, pre-burst and post-burst
		if (cmd.Contains("-t0")) {
			TString saux1(argv[++iopt]);
			double tmpt0 = saux1.Atof();
			if (tmpt0<0.) cout << "Wrong time zero assignment. Must be > 0" << endl;
			else t0 = tmpt0;
			TString saux2(argv[++iopt]);
			double tmptpre = saux2.Atof();
			if (tmptpre>0.) cout << "Wrong pre-burst assignment. Must be < 0" << endl;
			else tpre = tmptpre;
			TString saux3(argv[++iopt]);
			double tmptpost = saux3.Atof();
			if (tmptpost<0.) cout << "Wrong post-burst assignment. Must be > 0" << endl;
			else tpost = tmptpost;
			//printf("Selected time zero: %18.3f TT, pre-burst= %f s, post-burst= %f s\n", t0, tpre, tpost);
		}
		
		// save IPN-style light curve
		if (cmd.Contains("-ipn", TString::kExact)) {
			saveIPN = TRUE;
			TString saux2(argv[++iopt]);
			double tmptbefore = saux2.Atof();
			if (tmptbefore>0.) cout << "Wrong pre-burst assignment. Must be < 0" << endl;
			else tbefore = tmptbefore;
			TString saux3(argv[++iopt]);
			double tmptafter = saux3.Atof();
			if (tmptafter<0.) cout << "Wrong post-burst assignment. Must be > 0" << endl;
			else tafter = tmptafter;
		}
		
		// zoom to specified range
		if (cmd.Contains("-z", TString::kExact)) {
			TString saux1(argv[++iopt]);
			double tmpzbefore = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmpzafter = saux2.Atof();
			if ( tmpzafter <= tmpzbefore) {
				cout << "Wrong t_zoom_after assignment. Must be > t_zoom_before" << endl;
			} else {
				zbefore = tmpzbefore;
				zafter  = tmpzafter;
			}
		}
		
		// set background time interval
		if (cmd.Contains("-bkg", TString::kExact)) {
			TString saux1(argv[++iopt]);
			tbkgini = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmptbkgfin = saux2.Atof();
			if (tmptbkgfin <= tbkgini) {
				cout << "Wrong t_bkg_fin assignment. Must be > t_bkg_ini" << endl;
				tbkgfin = tbkgini + 1.;
			} else tbkgfin = tmptbkgfin;
			printf("Set background to the following time interval: [%.4f, %.4f]\n", tbkgini, tbkgfin);
		}
		
		// set GRB time interval
		if (cmd.Contains("-grb", TString::kExact)) {
			TString saux3(argv[++iopt]);
			tgrbini = saux3.Atof();
			TString saux4(argv[++iopt]);
			double tmptgrbfin = saux4.Atof();
			if (tmptgrbfin < tgrbini) {
				cout << "Wrong t_grb_fin assignment. Must be >= t_grb_ini" << endl;
				tgrbfin = tgrbini + 1.;
			} else tgrbfin = tmptgrbfin;
			printf("Set GRB to the following time interval: [%.4f, %.4f]\n", tgrbini, tgrbfin);
		}
		
		// it must be performed dedicated analysis for sub-millisecond triggers?
		if (cmd.Contains("-subms", TString::kExact)) {
			subms = TRUE;
			TString saux1(argv[++iopt]);
			dt = saux1.Atof();
			TString saux2(argv[++iopt]);
			thr = saux2.Atoi();
		}
		
		// input time window for peak search
		if (cmd.Contains("-ts")) {
			TString saux(argv[++iopt]);
			tsearch = saux.Atof();
			cout << "Selected t_search (s): " << tsearch << endl;
		}
		iopt++;
	}
	
	// ------------- main execution -------------------
	
	// check for data type (3908 / 3913)
	
	TFile *fi;
	fi= new TFile(rootfilename.Data());
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << rootfilename.Data() << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {
	
		// get file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
		}
	}
	fi->Close();
	
	if (outpath.Length() > 0 && outpath.EndsWith("/")==FALSE) outpath += "/";
	if (!batch)  {
		theApp = new TApplication("App", &argc, argv);
		cout << "Work in interactive mode" << endl;
	} else cout << "Work in batch mode" << endl;
		
	TString infoname(outpath);
	infoname.Append("grb_info.dat");		
	TString msgname(outpath);
	msgname.Append("mcal_grb3908_msg.txt");

	// work on 3913 data
	
	if (info->pkttype == 3913) {
		GRB3913XZ *grb = new GRB3913XZ(rootfilename);
		grb->SetGRBBoundaries(tbkgini, tbkgfin, tgrbini, tgrbfin);
		grb->LightCurve(t0, tpre, tpost);
		if (doStdAnalysis) grb->SpectralAnalysis();
		if (doCustomAnalysis) grb->CustomSpectralAnalysis();
		t90 = grb->GetT90();
		if (!zbefore && !zafter) {
			zbefore = -5.*t90;
			zafter  =  5.*t90;
		}
		grb->DrawCustomLightCurve(zbefore, zafter);
		if (saveGIF) {
			grb->SaveAnalysis(outpath);
			grb->SaveCustomLightCurve(outpath);
			grb->SaveInfo(infoname);
		}
		//if (saveIPN && t0) grb->SaveIPN(ipnfilename, tbefore, tafter);
		if (!batch) theApp->Run();	
	}
	
	// work on 3908 data
	
	if (info->pkttype == 3908 || info->pkttype == 3909 || info->pkttype == 3901 || info->pkttype == 3902) {
		GRB3908 *grb3908 = new GRB3908(rootfilename);
		grb3908->SetGRBBoundaries(tbkgini, tbkgfin, tgrbini, tgrbfin);
		if (tsearch != 30.) grb3908->SetTSearch(tsearch);
		
		grb3908->DefaultLightCurve();
		if (def) grb3908->DrawDefaultLightCurve();
		// if (subms) grb3908->FindTrueT0(t0, dt, thr);
		grb3908->CustomLightCurve(t0, tpre, tpost, emin, emax, barsX, barsZ, tbin);
		
		// do selection on tbin (to be modified with a proper flag
		if (!subms) {	// standard analysis procedure
			if (doStdAnalysis) grb3908->SpectralAnalysis();
			if (doCustomAnalysis) grb3908->CustomSpectralAnalysis();
			if (diagnostic) grb3908->VSBDiagnostic();
		} else {		// Very Short Burst analysis procedure
			if (doStdAnalysis) grb3908->SpectralAnalysisVSB();
			if (doCustomAnalysis) grb3908->CustomSpectralAnalysisVSB();			
			if (diagnostic) grb3908->VSBDiagnostic();
		}
		
		//grb3908->CalculateHR();
		if (diagnostic) grb3908->StandardDiagnostic();
		t90 = grb3908->GetT90();
		if (!zbefore && !zafter) {
			zbefore = -5.*t90;
			zafter  =  5.*t90;
		}
		grb3908->DrawCustomLightCurve(zbefore, zafter);
		if (diagnostic) grb3908->DrawDiagnostic(zbefore, zafter);
		
		if (spectra) {
			//EChannels * Ech = new EChannels("/home/martino/mcalsw/share/mcalfits_conf/canali_sciRM.dat") ;
			if (chfilename.Length() == 0) { // load default channel file
				chfilename.Append(getenv("MCALSW"));
				chfilename += "/share/mcalfits_conf/canali_1.dat";
			}
			EChannels * Ech = new EChannels(chfilename.Data()) ;

			int nb = Ech->getnChannels();
			double *spbins = new double[nb+1];
			for (int i=0; i<nb; i++) spbins[i] = 1000.*Ech->getElower(i);
			spbins[nb] = 1000.*Ech->getEupper(nb-1);
			
			// get background and grb spectra
			float *spbkg = grb3908->GetPha(tbkgini, tbkgfin, nb, spbins);
			float *spgrb = grb3908->GetPha(tgrbini, tgrbfin, nb, spbins);
			printf("i \tE_lo \t\tE_hi \t\tbkg \t\tgrb \t\tgrb counts/s\n");
			for (int i=0; i<nb; i++) printf("%3d \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f\n", i, spbins[i],
			spbins[i+1], spbkg[i], spgrb[i], spgrb[i]/(tgrbfin-tgrbini) - spbkg[i]/(tbkgfin-tbkgini)); 
			
			// create XSPEC compliant pha fits files
			TString bkgphafile(outpath);		// work on bkg
			bkgphafile += "!";
			bkgphafile += grb3908->GetFileHeader()->Data();
			bkgphafile += "_bkg.pha";
			savePha(nb, spbkg, tbkgfin-tbkgini, &bkgphafile);
			
			TString grbphafile(outpath);		// work on grb
			grbphafile += "!";
			grbphafile += grb3908->GetFileHeader()->Data();
			grbphafile += "_grb.pha";
			savePha(nb, spgrb, tgrbfin-tgrbini, &grbphafile);
			
			cspectrum = plotSpectrum(nb, spbins, spbkg, tbkgfin-tbkgini, spgrb, tgrbfin-tgrbini);
		}
		
		if (saveIPN && t0) grb3908->SaveIPN(outpath, tbefore, tafter);
		if (saveGIF) {
			grb3908->SaveAnalysis(outpath);
			grb3908->SaveCustomLightCurve(outpath);
		 	grb3908->SaveInfo(infoname);
			grb3908->SaveMsg(msgname);
			grb3908->SaveIPNMsg(outpath);
			grb3908->SaveVSBInfo(outpath);
			if (cspectrum) {
				TString grbspname(outpath);		// work on grb
				grbspname += grb3908->GetFileHeader()->Data();
				grbspname += "_spectrum.gif";
				cspectrum->Print(grbspname);
			}
			if (diagnostic) {
				grb3908->SaveDiagnostic(outpath);
				// grb3908->SaveVSBDiagnostic(outpath);				
			}
		}
		if (!batch) theApp->Run();	
	}
	
	return 0;
}
