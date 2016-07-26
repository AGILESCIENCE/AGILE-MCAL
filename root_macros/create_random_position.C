{
	// MM 27/02/13 print also satellite height and time at millisecond level 

	// MM 14/11/12 modified output format for blind test on WWLLN sferics data
	// see script ~/data_analysis/VSB/tgf_candidates_complete/plot_WWLLN.rb

	// MM 27/02/12 creates a random AGILE trigger dataset based on real satellite position
	// needed for cross check with LINET detections over Central America.
	// correlation work by Ferran Fabro at UPC, in visit at IASF-Bo 20/02 - 03/03/2012
	
	// based on get_coordinate_2.C and add_geographic_info_3.C
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	gStyle->SetPalette(1,0);  // 2D graph colour version
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time

	FILE *fp;
	fp = fopen("./random_position.dat", "w");
	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};

	// read 3916 index and fill array
	double tini[40000];
	double tfin[40000];
	int cont[40000];
	ifstream fidx3916;
	fidx3916.open("/home/mcal/data_analysis/AGILE_coordinates/3916.index", ifstream::in);
	int n3916 = 0;
	char aux[10];
	while (fidx3916 >> aux >> cont[n3916] >> tini[n3916] >> tfin[n3916]) n3916++;
	fidx3916.close();
	cout << "Number of 3916 entries = " << n3916 << endl;
	
	//for (int i=0; i<10; i++) printf("%6d \t%.6f \t%.6f\n", cont[n3916-11+i], tini[n3916-11+i], tfin[n3916-11+i]);
	
	
	//double tstart = 242003278.; // Start time (TT) 01 Sep 2011
	//double tstop  = 257391126.; // Stop time (TT) 27 Feb 2012
	double tstart = 162950400.; // Start time (TT) 01 Mar 2009
	double tstop  = 257644800.; // Stop time (TT) 27 Feb 2012
	double dt = tstop - tstart;
	int nrdm=100000;	// number of random times to be simulated
	TH2F *h2lonlat = new TH2F("h2lonlat", "h2lonlat", 360, -180., 180., 5, -2.5, 2.5);
	
	// crates random times dataset
	TRandom1 *rnd = new TRandom1(0);
	double *atime = new double[nrdm];
	rnd->RndmArray(nrdm, atime);
	Long64_t *aidx = new Long64_t[nrdm];
	TMath::Sort(nrdm, atime, aidx, 0);
	//for (int i=0; i<nrdm; i++) printf("%.6f\n", atime[aidx[i]]);
	//cout << endl;
	//for (int i=0; i<nrdm; i++) printf("%.6f\n", tstart + atime[aidx[i]]*dt);
	// cout << endl;
	for (int i=0; i<nrdm; i++) atime[i] = tstart + atime[i]*dt;
	
	// creates repositoies for fake data
	int *acont = new int[nrdm];		// contact array
	double *alon = new double[nrdm];	// longitude array
	double *alat = new double[nrdm];	// latitude array
	double *ah = new double[nrdm];        // altitude array

	// loop on random times
	gpsevt evt;
        MCVFile3916 *f;
	char infile[100];
	
	for (int i=0; i<nrdm; i++) { 
	
		// get contact for chosen time
		int k = TMath::BinarySearch(n3916, tini, atime[i]);
		acont[i] = cont[k];
             
		// retrieve geographic info
		evt.lon = 999.;
		evt.lat = evt.etheta = evt.ephi = evt.h = 0.;
		evt.time = atime[i];
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", acont[i]);
                f = new MCVFile3916(infile);
 		int coorok = f->GetCoordinates(&evt);
                delete f;   
		
		if (coorok == 1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", acont[i]+1);
			f = new MCVFile3916(infile);
			int newcor = f->GetCoordinates(&evt);
			delete f;   
			if (newcor) cout << "*** Time " << atime[i] << " could not be found in contacts " << acont[i] << " and " << acont[i]+1 << endl;
		}
		
		if (coorok == -1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", acont[i]-1);
			f = new MCVFile3916(infile);
			int newcor = f->GetCoordinates(&evt);
			newcor = f->GetCoordinates(&evt);
			delete f;   
			if (newcor) cout << "*** Time " << atime[i] << " could not be found in contacts " << acont[i] << " and " << acont[i]-1 << endl;
		}
		alon[i] = evt.lon;
		alat[i] = evt.lat;
		ah[i] = 0.001 * evt.h;
	}
	
	// output results
	TTimeStamp ts;
	UInt_t year, month, day, date;
	UInt_t hour, min, sec, time;
	double asec;
	for (int i=0; i<nrdm; i++) {
		if (alon[aidx[i]] > -180. && alon[aidx[i]] < 180.) {
			h2lonlat->Fill(alon[aidx[i]],alat[aidx[i]]);
			ts.Set((int)  atime[aidx[i]], 0, epoch.GetSec(),0);	// calculate UT time
			date = ts.GetDate(kTRUE, 0, &year, &month, &day);
			time = ts.GetTime(kTRUE, 0, &hour, &min, &sec);
			asec = sec + atime[aidx[i]] - TMath::Floor(atime[aidx[i]]);
			fprintf(fp, "%06d \t%.6f \t%04d \t%02d \t%02d \t%02d \t%02d \t%8.3f \t%8.3f \t%8.3f \t%8.3f\n", acont[aidx[i]], atime[aidx[i]], year, month, day, hour, min, asec, alon[aidx[i]], alat[aidx[i]], ah[aidx[i]]);
		}
	}
	
	fclose(fp);
	// graphics section
	//h2lonlat->Draw("COLZ");

}
