
#include "makespectra.h"
#include "version.h"


using namespace std ; 

int main(int argc, char *argv[])
{
	cout << endl << "makespectra version " << VERSION << endl;
	if (argc <4) {
		cout << "usage:" << endl << "makespectra <data_file> <energy_channel_file> <output_path> [options]" << endl;
		cout << "Options:" << endl << "-bars <0xIIII>  <0xIIII>: set selected bars (two words of 4 hex digits: MS X-plane, LS Z-plane)" << endl << "-sp : build pha from ascii file in (channel#(1-76)  value) format" << endl << "-bkg <tbkgini> <tbkgfin>: set background start and stop time (s) with respect to T0"  << endl << "-grb <tgrbini> <tgrbfin>: set transient start and stop time (s) with respect to T0. If not set, duration from file will be assumed" << endl;
		return 0;
	}

	int ntrg=0;
	TFile *fi;
	GRB3908 *grb3908;
	float tbkgini = 1.;	//-20
	float tbkgfin =  21.;	//-5
	float tgrbini = 0.;	//-20
	float tgrbfin =  1.;	//-5
	bool isGRBSet = 0;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	bool buildFromFile = 0;	// if 1 read spectral file (channel#(1-76)  value)  and format in fits
				// should be modified to include duration info
	
	
	// ------------- parse command line data (derived from plotgrb.cc) -------------------
	int iopt=4;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
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
		
		// input bkg boundaries selection
		if (cmd.Contains("-bkg")) {
			TString saux1(argv[++iopt]);
			sscanf(saux1.Data(), "%f", &tbkgini);
			TString saux2(argv[++iopt]);
			sscanf(saux2.Data(), "%f", &tbkgfin);
			cout << "Selected background interval with respect to t0: " << tbkgini << "  " << tbkgfin << endl; 
		}
		
		// input grb boundaries selection
		if (cmd.Contains("-grb")) {
			TString saux1(argv[++iopt]);
			sscanf(saux1.Data(), "%f", &tgrbini);
			TString saux2(argv[++iopt]);
			sscanf(saux2.Data(), "%f", &tgrbfin);
			isGRBSet = 1;
			cout << "Selected transient interval with respect to t0: " << tgrbini << "  " << tgrbfin << endl; 
		}
				
		// input bars selection
		if (cmd.Contains("-sp")) buildFromFile = 1;
		
		iopt++;
	}
	
	// ------------- main execution -------------------
	
	// load spectral class	
	EChannels * Ech = new EChannels(argv[2]) ;

	int nb = Ech->getnChannels();
	double *spbins = new double[nb+1];
	float *totbkg  = new float[nb];
	float *totgrb  = new float[nb];
	
	float tbkg = 0.;
	float tgrb = 0.;
	for (int i=0; i<nb; i++) {
		spbins[i] = 1000.*Ech->getElower(i);
		totbkg[i] = 0.;
		totgrb[i] = 0.;
	}
	spbins[nb] = 1000.*Ech->getEupper(nb-1);
	for (int i=0; i<=nb; i++) cout << i << "  " << spbins[i] << endl;

		
	// load orbit data
	ifstream fin;
	fin.open(argv[1], ifstream::in);
	
	if (buildFromFile) {	// create pha from ascii file
		
		int ch;
		for (int i=0; i<nb; i++) fin >> ch >> totgrb[i];
		tgrb = 1.;	// to be modified
		tbkg = 1.;	// to be modified
	
	} else {		// default execution: compile cumulative spectrum for multiple triggers
	
		fin >> ntrg;
		cout << "Reading trigger file " << argv[1] << " - number of trigger to be processed = " << ntrg << endl;
		trg *atrg = new trg[ntrg];
		double *maxE = new double[ntrg];	// array to store the maximum energy in the processed triggers
	
		float *matgrb = new float[nb*ntrg];	// sprectra 2D matrix
		int   *runid  = new int[ntrg];		// contact array
		float   *t90  = new float[ntrg];	// t90 array
		
		for (int i=0; i<ntrg; i++) {
			fin >> atrg[i].runid >> atrg[i].t0 >> atrg[i].t90;
		}
		
		for (int i=0; i<ntrg; i++) {
			printf("%3d Processing orbit %6d at t0=%20.6f\n", i, atrg[i].runid, atrg[i].t0);
			
			//TString rootfilename("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT"); // old archive path
			TString rootfilename("");
			char saux[100];
			sprintf(saux, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", atrg[i].runid, atrg[i].runid);
			rootfilename += saux;
			
			fi= new TFile(rootfilename.Data());
			if (fi->IsZombie()) { // fi is not a valid root file: continue with next
				cout << rootfilename.Data() << " is not a valid ROOT file: exiting!" << endl;
			fi->Close();
				continue;
			}		
			fi->Close();
			
			cout << "Reading root file " << rootfilename.Data() << endl;
			if (!isGRBSet) tgrbfin = atrg[i].t90;
			grb3908 = new GRB3908(rootfilename);
			grb3908->SetGRBBoundaries(tbkgini, tbkgfin, tgrbini, tgrbfin);
			grb3908->CustomLightCurve(atrg[i].t0, -30., +30., 0., 2000., barsX, barsZ, 0.001);
			//grb3908->SetT0(atrg[i].t0+atrg[i].dt0);
				
			// get background and grb spectra
			float *spbkg;
			if (tgrbini>=tbkgini && tgrbfin<=tbkgfin) { // grb time interval is included in bkg interval
				// NB: NOT all superposition possibilities are addressed! ONLY grb interval included in bkg interval.
				float *spbkg1 = grb3908->GetPha(tbkgini, -0.1, nb, spbins); // tgrbini - 1.e-6
				float *spbkg2 = grb3908->GetPha(0.1, tbkgfin, nb, spbins); // tgrbfin + 1.e-6
				spbkg = new float[nb];
				for (int i=0; i<nb; i++) spbkg[i] = spbkg1[i] + spbkg2[i];
			}
			else spbkg = grb3908->GetPha(tbkgini, tbkgfin, nb, spbins);
			float *spgrb = grb3908->GetPha(tgrbini, tgrbfin, nb, spbins);
			printf("i \tE_lo \t\tE_hi \t\tbkg \t\tgrb \t\tgrb counts/s\n");
			for (int j=0; j<nb; j++) {
				printf("%3d \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%20.3f\n", j, spbins[j], spbins[j+1], spbkg[j], spgrb[j], spgrb[j]/atrg[i].t90 ); 
				totbkg[j] += spbkg[j];
				totgrb[j] += spgrb[j];
				
				matgrb[i*nb + j] = spgrb[j];
			}
			tgrb += (tgrbfin - tgrbini);
			if (tgrbini>=tbkgini && tgrbfin<=tbkgfin) tbkg += (tbkgfin - tbkgini -0.2); // += ((tbkgfin - tbkgini) - (tgrbfin - tgrbini))
			else tbkg += (tbkgfin - tbkgini);
			grb3908->VSBDiagnostic();	// otherwise maxE is 0
			maxE[i] = grb3908->GetMaxE();
			
			runid[i] = atrg[i].runid;
			t90[i]   = atrg[i].t90;
			
			delete grb3908;
	
			cout << endl << endl;
			printf("i \tE_lo \t\tE_hi\t");
			for (int i=0; i<ntrg; i++) printf(" \t%6d", runid[i]);
			cout << endl;
			for (int j=0; j<nb; j++) {
				printf("%3d \t%8.3f \t%8.3f", j, spbins[j], spbins[j+1]);
				for (int i=0; i<ntrg; i++) printf(" \t%6.0f", matgrb[i*nb+j]);
			cout << endl;
			}
			printf("t90\t\t\t");
			for (int i=0; i<ntrg; i++) printf(" \t%.6f", t90[i]);
			cout << endl << endl;
			
			// print max E info
			printf("runid \tmax E (MeV)\n");
			for (int i=0; i<ntrg; i++)  printf("%6d \t%10.3f\n", atrg[i].runid, maxE[i]);
		}
	}
	
	//show total results
	cout << endl << "Total GRB time = " << tgrb << " s" << endl;
	cout << "Total BKG time = " << tbkg << " s" << endl;
	printf("i \tE_lo \t\tE_hi \t\tbkg \t\tgrb \t\tgrb counts/s\n");
	for (int j=0; j<nb; j++) {
		printf("%3d \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%20.3f\n", j, spbins[j], spbins[j+1], totbkg[j], totgrb[j], totgrb[j]/tgrb ); 
	}
	
	// create XSPEC compliant pha fits files
	TString bkgphafile("cumulative_bkg.pha");		// work on bkg
	//bkgphafile += "cumulative_bkg.pha";
	savePha(nb, totbkg, tbkg, &bkgphafile);
	
	TString grbphafile("cumulative_grb.pha");		// work on grb
	//grbphafile += "cumulative_grb.pha";
	savePha(nb, totgrb, tgrb, &grbphafile);
	
	return 0;
}

