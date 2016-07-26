{
	// MM 12/11/12 reads a MCAL trigger dataset and compare with WWLLN data provided by Colin Price
	// based on wwlln_check.C and plot_tgf_catalog_1.C
	
	gROOT->Reset();
	
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");

	
	// -----------  connect input MCAL trigger tree (created with macro plot_triggers_5.C) ----------- 
	
	struct offline_trg {
		int contact;
		int id;
		double t0;
		int counts;
		float hr;
		float dt;
		float lon;
		float lat;
		float h;
		float etheta;
		float ephi;
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int usec;
		float loctime;
		float eavg;
		float emax;
		int boost;
		int flag;
		int tgflag;
		int c[8];
	};
	offline_trg atrg;

	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg2_20111023.root"); 
	TTree *ttrg = (TTree *) ftrg.Get("triggers");
	
	ttrg->SetBranchAddress("contact",&atrg.contact);
	ttrg->SetBranchAddress("id",&atrg.id);
	ttrg->SetBranchAddress("t0",&atrg.t0);
	ttrg->SetBranchAddress("counts",&atrg.counts);
	ttrg->SetBranchAddress("hr",&atrg.hr);
	ttrg->SetBranchAddress("dt",&atrg.dt);
	ttrg->SetBranchAddress("lon",&atrg.lon);
	ttrg->SetBranchAddress("lat",&atrg.lat);
	ttrg->SetBranchAddress("h",&atrg.h);
	ttrg->SetBranchAddress("etheta",&atrg.etheta);
	ttrg->SetBranchAddress("ephi",&atrg.ephi);
	ttrg->SetBranchAddress("year",&atrg.year);
	ttrg->SetBranchAddress("month",&atrg.month);
	ttrg->SetBranchAddress("day",&atrg.day);
	ttrg->SetBranchAddress("hour",&atrg.hour);
	ttrg->SetBranchAddress("min",&atrg.min);
	ttrg->SetBranchAddress("sec",&atrg.sec);
	ttrg->SetBranchAddress("usec",&atrg.usec);
	ttrg->SetBranchAddress("loctime",&atrg.loctime);
	ttrg->SetBranchAddress("eavg",&atrg.eavg);
	ttrg->SetBranchAddress("emax",&atrg.emax);
	ttrg->SetBranchAddress("boost",&atrg.boost);
	ttrg->SetBranchAddress("flag",&atrg.flag);
	ttrg->SetBranchAddress("tgflag",&atrg.tgflag);
	ttrg->SetBranchAddress("c0",&atrg.c[0]);
	ttrg->SetBranchAddress("c1",&atrg.c[1]);
	ttrg->SetBranchAddress("c2",&atrg.c[2]);
	ttrg->SetBranchAddress("c3",&atrg.c[3]);
	ttrg->SetBranchAddress("c4",&atrg.c[4]);
	ttrg->SetBranchAddress("c5",&atrg.c[5]);
	ttrg->SetBranchAddress("c6",&atrg.c[6]);
	ttrg->SetBranchAddress("c7",&atrg.c[7]);
	int ntrg = ttrg->GetEntries();
	ttrg->Print();
	
	
	// ----------- load WWLLN data file -----------
	
	UInt_t year = 2011, month = 10, day = 23;
	char wname[200];
	sprintf(wname, "/home/mcal/data_analysis/WWLLN/TGFs/A%4d%02d%02d.loc", year, month, day);
	cout << "Loading WWLLN data file " << wname << endl;
	
	ifstream fwwlln;
	fwwlln.open(wname);
	char wline[100];
	

	struct wwlln {
		int year;
		int month;
		int day;
		int hour;
		int min;
		float sec;
		Double_t sod;
		float lon;
		float lat;
		float ipeak;
		int nstat; 
	};
	wwlln aw;
	TTree tw;

	tw.Branch("year",&aw.year, "year/I");
	tw.Branch("month",&aw.month, "month/I");
	tw.Branch("day",&aw.day, "day/I");
	tw.Branch("hour",&aw.hour, "hour/I");
	tw.Branch("min",&aw.min, "min/I");
	tw.Branch("sec",&aw.sec, "sec/F");
	tw.Branch("sod",&aw.sod, "sod/D");
	tw.Branch("lon",&aw.lon, "lon/F");
	tw.Branch("lat",&aw.lat, "lat/F");
	tw.Branch("ipeak",&aw.ipeak, "ipeak/F");
	tw.Branch("nstat",&aw.nstat, "nstat/I");
	
	Double_t wsod[1000000];
	Double_t wlon[1000000];
	Double_t wlat[1000000];
	
	int nw=0;
	while (fwwlln.getline(wline, 56)) { //&& i < 100000
		sscanf(wline, "%4d/%2d/%2d,%2d:%2d:%9f,%8f,%9f,%5f,%3d", &aw.year, &aw.month, &aw.day, &aw.hour, &aw.min, &aw.sec, &aw.lat, &aw.lon, &aw.ipeak, &aw.nstat);
		aw.sod = 3600.*aw.hour + 60.*aw.min + aw.sec;
		tw.Fill();
		
		wsod[nw] = aw.sod;
		wlat[nw] = aw.lat;
		wlon[nw] = aw.lon;
		nw++;
	}
	tw.Print();
	
	cout << "sort WWLLN time array..." << endl;
	int *idx = new int[nw];
	TMath::Sort(nw, wsod, idx, 0);
	double *wsod2 = new double[nw];
	for (int i=0; i<nw; i++) wsod2[i] = wsod[idx[i]];
	 

	
	// --------- loop on triggers and find the closest sferics match ---------
	
	double tsod, dt1, dt2, dt, dist;
	for (int i=0; i<ntrg; i++) {
		ttrg->GetEntry(i);
		tsod = 3600.*atrg.hour + 60.*atrg.min + atrg.sec + 1.e-6*atrg.usec;
	
		Long64_t id = TMath::BinarySearch(nw, wsod2, tsod);
		dt1 = tsod - wsod2[id];
		dt2 = wsod2[id+1] - tsod;
		if (dt2 < dt1) id++;
		dt = tsod - wsod2[id];
		
		if (fabs(dt)<0.01) {
			dist = 111.*sqrt((atrg.lon - wlon[idx[id]])*(atrg.lon - wlon[idx[id]]) + (atrg.lat - wlat[idx[id]])*(atrg.lat - wlat[idx[id]]));
			printf("%02d  %02d  %02d.%06d  %20.6f  %20.6f  %3d  %.2f  %6.2f %6.2f %.6f %.0f\n", atrg.hour, atrg.min, atrg.sec, atrg.usec, tsod, wsod2[id], atrg.counts, atrg.hr, atrg.lon, atrg.lat, dt, dist);
		}

	}
}
