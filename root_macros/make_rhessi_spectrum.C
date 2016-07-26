{
	// MM 28/05/09 added selection of the RHESSI sample in time (June to March)
	// MM 20/05/09  -  this sw builds a cumulative spectrum from the RHESSI 1st TGF catalog (downloaded to HP1)
	
	gROOT->Reset();
	
	struct RHESSI_tgf {	// based on the RHESSI TGF catalog available here: http://scipp.ucsc.edu/~dsmith/tgflib_public/
		char date[20];
		char start[20];
		char path[30];
		float lat;
		float lon;
		float alt;	// km
		int decim;
		int decim_chan;
		float loctime;
		float earthangle;
	};
	
	int ntgf = 820;
	int id=0;
	
	// RHESSI energy channels boundaries
	int nbins = 44;
	float erh[] = {0.,20.,25.,30.,35.,40.,45.,50.,60.,70.,80.,100., 120.,140.,160.,200.,250.,300.,350.,400.,500.,600.,700.,   800.,1000.,1200.,1400.,1700.,2000.,2500.,3000.,3500.,4000.,  5000.,6000.,7000.,9000.,11000.,13000.,15000.,17500.,19999., 23000.,30000.,40000.};
	TH1F hrh("hrh", "hrh", nbins, erh);
	
	// RHESSI TGF spectrum
	int evt[45];
	for (int i=0; i<45; i++) evt[i] = 0;
	
	// RHESSI background spectrum
	float bkg[45];
	for (int i=0; i<45; i++) bkg[i] = 0;
	
	// read RHESSI catalog data 
	
	RHESSI_tgf rtgf[1000];
  	ifstream frhessi("/home/mcal/data_analysis/VSB/RHESSI-TGF-catalog/tgflist_before-Jan-05.txt", ios_base::in);
	int nrh=0;
	int nsel=0;
	frhessi.clear();
	char date[20];
	char t0[20];
	float time, energy;
	float elo, ehi, deltae, backSpec, backErr, ncounts;
	int nph, totph;
	while (!frhessi.eof()) {
		frhessi >> rtgf[nrh].date >> rtgf[nrh].start >> rtgf[nrh].path >> rtgf[nrh].lat >> rtgf[nrh].lon >> rtgf[nrh].alt >> rtgf[nrh].decim >> rtgf[nrh].decim_chan >> rtgf[nrh].loctime >> rtgf[nrh].earthangle;
		if(rtgf[nrh].lon>180.) rtgf[nrh].lon = rtgf[nrh].lon-360.;	// longitude correction
		
		int yy, mm, dd;
		char stmp[30];
		sscanf(rtgf[nrh].path, "%2d%2d%2d%s", &yy, &mm, &dd, stmp);

		if (rtgf[nrh].lat<=2.5 && rtgf[nrh].lat>=-2.5 && (mm<=3 || mm>=6)) {
			
			// read the selected spectrum
			
			TString datapath("/home/mcal/rhessi/scipp.ucsc.edu/~dsmith/tgflib_public/");
			datapath += rtgf[nrh].path;
			datapath += "/";
			datapath += rtgf[nrh].path;
			datapath += ".txt";
			
			cout << endl << nsel << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << "  " << datapath.Data() << "  " << mm << endl;
			nsel++;
			
			ifstream fdata(datapath.Data(), ios_base::in);
			fdata.clear();
			fdata >> date >> t0;
			float tprev = 0.;
			nph = 0;
			while (!fdata.eof()) {
				fdata >> time >> energy;
				if (time > tprev) {
					// cout << time << "  "  << energy << endl;
					hrh.Fill(energy);
					nph++;
					
					// fill "artigianal" histo
					int j=0;
					while (energy > erh[j] && j<44) j++;
					evt[j-1]++;
				}
				tprev = time;
			}
			cout << date << "  "  << t0 << "  "  << nph << endl;
			fdata.close();
			totph += nph;
			
			// read the associated background file
						
			TString bkgpath("/home/mcal/rhessi/scipp.ucsc.edu/~dsmith/tgflib_public/");
			bkgpath += rtgf[nrh].path;
			bkgpath += "/";
			bkgpath += rtgf[nrh].path;
			bkgpath += "_background.txt";
			
			ifstream fbkg(bkgpath.Data(), ios_base::in);
			fbkg.clear();
			for (int i=0; i<37; i++) {
				fbkg >> elo >> ehi >> deltae >> backSpec >> backErr;
				if (elo != erh[i+4]) cout << "*** wrong E band: " << elo << " != " << erh[i+4] << endl;
				bkg[i+4] += backSpec;
				ncounts = backSpec * 120. * deltae;

			}
			fbkg.close();
		}
		nrh++;
	}
		
	// show spectra
	for (int i=1; i<= nbins; i++) {
		printf("%2d \t%8.1f \t%8.1f \t%8.0f \t%12.6f\n", i, hrh->GetBinLowEdge(i), hrh->GetBinLowEdge(i+1), hrh->GetBinContent(i), bkg[i-1]);
	}
	cout << "tot photons = " << totph << "  must be = " << hrh->GetEntries() << endl;
	
	// print data to make xspec-compliant pha files
	cout << endl << "TGF file" << endl;
	for (int i=1; i<= nbins; i++) printf("%2d \t%8.0f\n", i, hrh->GetBinContent(i));
	
	cout << endl << "BKG file" << endl;
	for (int i=1; i<= nbins; i++) printf("%2d \t%8.0f\n", i, bkg[i-1]*120.*(erh[i]-erh[i-1]));
	
	
	frhessi.close();
}