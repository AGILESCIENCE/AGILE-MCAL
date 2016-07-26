{	
	// comparison between 3902 and 3909 files for timing comparison
	// MM 13/2/2007
	
	// event structure (from MCVLib MCVFile.h)
	gROOT->Reset();
	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
	};
	
	// input files
	TFile fg("/data1/mcalsw_output_AIV/RT/RT013369_3902.root");
	TFile fb("/data1/mcalsw_output_AIV/RT/RT013369_3909.root");
//	TFile fg("/data1/mcalsw_output/RT/RT013510_3902.root");
//	TFile fb("/data1/mcalsw_output/RT/RT013510_3909.root");
//	TFile fg("~/local/mcalsw/bin/mcalsw_output/RT/RT013540_3902.root");
//	TFile fb("~/local/mcalsw/bin/mcalsw_output/RT/RT013540_3909_sel_01.root");
//	TFile fg("~/local/mcalsw/bin/mcalsw_output/RT/RT013541_3902.root");
//	TFile fb("~/local/mcalsw/bin/mcalsw_output/RT/RT013541_3909.root");
//	TFile fg("~/local/mcalsw/bin/mcalsw_output/RT/RT013542_3902.root");
//	TFile fb("~/local/mcalsw/bin/mcalsw_output/RT/RT013542_3909.root");
//	TFile fg("~/local/mcalsw/bin/mcalsw_output/RT/RT013753_3902.root");
//	TFile fb("~/local/mcalsw/bin/mcalsw_output/RT/RT013753_3909_sel_01.root");
//	TFile fg("~/mcalsw/bin/mcalsw_output/RT/RT000253_3902.root");
//	TFile fb("~/mcalsw/bin/mcalsw_output/RT/RT000253_3909_sel_01.root");
	fg.ls();
	fb.ls();
	float mcaleth = 5.;
	TH1F *hdt = new TH1F("hdt", "hdt", 400000, -2.7E6, -2.3E6);	// good for run 13369
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, -3.6E6, -3.5E6);	// good for run 13511
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, -3.6E6, -3.5E6);	// good for run 13509 & 13510
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, -0.7E6, -0.6E6);	// good for run 13540
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, -2.6E6, -2.5E6);	// good for run 13541 
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, 0.3E6, 0.4E6);	// good for run 13542 
//	TH1F *hdt = new TH1F("hdt", "hdt", 100000, -20.E6, 20.E6);	// good for finding offset 
//	TH1F *hdt = new TH1F("hdt", "hdt", 200000, 99.9E6, 100.1E6);	// good for run 13753 
	
	// retrieve data trees
	TTree *tg = NULL;
	TTree *tb = NULL;
	tg = (TTree *) fg->Get("tdata");
//	tb = (TTree *) fb->Get("tdata_shift");
//	tb = (TTree *) fb->Get("tdata_sel_0");
	tb = (TTree *) fb->Get("tdata");
	int ng = tg->GetEntries();
	int nb = tb->GetEntries();
	cout << "GRID: " << ng << " entries; BURST: " << nb << " entries" << endl;
	
	// connect input trees
	MCALevt evtg;
	tg->SetBranchAddress("time", &evtg.time);
	tg->SetBranchAddress("mult", &evtg.mult);
	tg->SetBranchAddress("bar", evtg.bar);
	tg->SetBranchAddress("pdA", evtg.pdA);
	tg->SetBranchAddress("pdB", evtg.pdB);
	tg->SetBranchAddress("pos", evtg.pos);
	tg->SetBranchAddress("E", evtg.E);
	tg->SetBranchAddress("Etot", &evtg.Etot);
	MCALevt evtb;
	tb->SetBranchAddress("time", &evtb.time);
	tb->SetBranchAddress("mult", &evtb.mult);
	tb->SetBranchAddress("bar", evtb.bar);
	tb->SetBranchAddress("pdA", evtb.pdA);
	tb->SetBranchAddress("pdB", evtb.pdB);
	tb->SetBranchAddress("pos", evtb.pos);
	tb->SetBranchAddress("E", evtb.E);
	tb->SetBranchAddress("Etot", &evtb.Etot);
	
	int jmin=0, oldj=0;
	
	int *ig = new int[ng];		// event indexes
	int *ib = new int[ng];
	int *bgx = new int[ng];		// bar indexes
	int *bgz = new int[ng];
	int *bbx = new int[nb];
	int *bbz = new int[nb];
	float *pgx = new float[ng];	// positions
	float *pgz = new float[ng];
	float *pbx = new float[nb];
	float *pbz = new float[nb];
	float *egx = new float[ng];	// energies
	float *egz = new float[ng];
	float *ebx = new float[nb];
	float *ebz = new float[nb];
	double *timeg = new double[ng];	// time (selected)
	double *timeb = new double[nb];
	double *ttotg = new double[ng];	// time (total)
	double *ttotb = new double[nb];
	float *etotg = new float[ng];	// total MCAL grid energy
	
	int nbarsg, nbarsb, id;
	bool px, pz;
	double dt, olddt;
	int bgztmp, bgxtmp, bbztmp, bbxtmp;
	float egztmp, egxtmp, pgztmp, pgxtmp;
	float ebztmp, ebxtmp, pbztmp, pbxtmp;
	
	// select grid events and fill arrays
	int nsg = 0;
	for (int i=0; i<ng; i++) {
		tg->GetEntry(i);
		ttotg[i] = evtg.time;
		etotg[i] = evtg.Etot;
		nbarsg=0;
		px = kFALSE;
		pz = kFALSE;
		for (int k=0; k<30; k++) { 
			id = evtg.bar[k];
			if (id>=0 && evtg.E[id] > mcaleth) {
				nbarsg++;
				if (k>=15) {	
					pz = kTRUE;
					bgztmp = k;
					egztmp = evtg.E[id];
					pgztmp = evtg.pos[id];
				} else {
					px = kTRUE;
					bgxtmp = k;
					egxtmp = evtg.E[id];
					pgxtmp = evtg.pos[id];
				}
			}
		}
		if (nbarsg == 2 && px && pz) {
			timeg[nsg] = evtg.time;
			pgx[nsg] = pgxtmp;
			pgz[nsg] = pgztmp;
			egx[nsg] = egxtmp;
			egz[nsg] = egztmp;
			bgx[nsg] = bgxtmp;
			bgz[nsg] = bgztmp;
			ig[nsg] = i;
			//cout << nsg << " " << timeg[nsg] << " " << bgx[nsg] << " " << pgx[nsg] << " " << egx[nsg] << " " << bgz[nsg] << " " << pgz[nsg] << " " << egz[nsg] << endl;
			nsg++;
		}
	}
	
	// select burst events and fill arrays
	int nsb = 0;
	for (int j=0; j<nb; j++) {	// try to find corresponding burst event
		tb->GetEntry(j);
		ttotb[j] = evtb.time;
		nbarsb=0;
		px = kFALSE;
		pz = kFALSE;
		for (int k=0; k<30; k++) { 
			id = evtb.bar[k];
			if (id>=0 && evtb.E[id] > mcaleth) {
				nbarsb++;
				if (k>=15) {	
					pz = kTRUE;
					bbztmp = k;
					ebztmp = evtb.E[id];
					pbztmp = evtb.pos[id];
				} else {
					px = kTRUE;
					bbxtmp = k;
					ebxtmp = evtb.E[id];
					pbxtmp = evtb.pos[id];
				}
			}
			if (nbarsb == evtb.mult) break;
		}	
		if (nbarsb == 2 && px && pz) {
			timeb[nsb] = evtb.time;
			pbx[nsb] = pbxtmp;
			pbz[nsb] = pbztmp;
			ebx[nsb] = ebxtmp;
			ebz[nsb] = ebztmp;
			bbx[nsb] = bbxtmp;
			bbz[nsb] = bbztmp;
			ib[nsb] = j;
			//cout << nsb << " " << timeb[nsb] << " " << bbx[nsb] << " " << pbx[nsb] << " " << ebx[nsb] << " " << bbz[nsb] << " " << pbz[nsb] << " " << ebz[nsb] << endl;
			nsb++;
		}
	}
	cout << "Selected GRID: " << nsg << " entries; selected BURST: " << nsb << " entries" << endl;
	
	// find time correspondences
	for (int i=0; i<nsg; i++) {
		if (i%1000 == 0) cout << "Processing grid event " << i << endl;
		dt = 1.E8;
		jmin = 0;
		for (int j=0; j<nsb; j++) {
			if (bbx[j] == bgx[i] && bbz[j] == bgz[i]) { //&& fabs(pgx-pbx)<2. && fabs(pgz-pbz)<2.
				olddt = dt;
				oldj = jmin;
				dt = timeg[i] - timeb[j];
				if (fabs(olddt) <= fabs(dt)) {
					hdt->Fill(olddt);
					//if (i<100 ) cout << i << "  " << egx[i]+egz[i] << "  " << olddt << "  " << jmin << endl;
					break;
				}
				jmin = j;
			}
		}
	}
	
	// plot offset search results
	TCanvas *c1 = new TCanvas();
	c1->Divide(2,2);
	c1->cd(1);
	hdt->Draw();
	
	// try to sinchonize all dataset
	TH1F *hdttot = new TH1F("hdttot", "hdttot", 100000, -5.e4, 5.e4);
	TH1F *hbars = new TH1F("hbars", "hbars", 31, 0, 31);
	TH1F *hmult = new TH1F("hmult", "hmult", 31, 0, 31);
//	double toff = 2.530716e+06;	// tentative time offset for run 13368
	double toff = 2.547275e+06;	// tentative time offset for run 13369
//	double toff = 3.531443e+06;	// tentative time offset for run 13511
//	double toff = 3.551076e+06;	// tentative time offset for run 13510
//	double toff = 3.571542e+06;	// tentative time offset for run 13509
//	double toff = 0.643505e+06;	// tentative time offset for run 13540
//	double toff = 2.524463e+06;	// tentative time offset for run 13541
//	double toff = -0.301147e+06;	// tentative time offset for run 13542
//	double toff = -2.514083e+06;	// tentative time offset for run 13753
	int jprev = 0;
	double ttest;
	for (int i=0; i<ng; i++) {
		ttest = ttotg[i] + toff;
		dt = 1.e8;
		if (etotg[i] > 1.) {
			for (int j=jprev; j<nb; j++) {
				olddt = dt;
				dt = ttest - ttotb[j];
				if (fabs(dt) < fabs(olddt)) {
					jprev =j;
				} else {
					hdttot->Fill(olddt);
					if (fabs(olddt)<3.) {	// histo bar address for nearby burst events
						tb->GetEntry(jprev);
						if (evtb.mult>0) for (int k=0; k<30; k++) if (evtb.bar[k]>=0) hbars->Fill(k);
						hmult->Fill(evtb.mult);
					}
					break;
				}
			}
			if ( fabs(olddt)<3.) {
				cout << i << "  " << etotg[i] << "  " << olddt << "  " << jprev;
				//if(fabs(olddt)>10.) cout << " ***";
				tb->GetEntry(jprev);
				for (int k=0; k<30; k++) if (evtb.bar[k]>=0) cout << "  (" << k << ", " << evtb.pos[evtb.bar[k]] << ", " << evtb.E[evtb.bar[k]] << ") ";
				cout << endl;
			}
		}
	}
	c1->cd(2);
	hdttot->GetXaxis()->SetRangeUser(-10., 10.);
	hdttot->Draw();
	c1->cd(3);
	hbars->Draw();
	c1->cd(4);
	hmult->Draw();
	double n10 = hdttot->Integral(49990, 50010);
	double n3 = hdttot->Integral(49997, 50003);
	double ntot = hdttot->Integral(0, 100000);
	cout << "Number of events with -3us < dt < +3us : " << n3 << " (" << 100.*n3/ntot << "% of " << ntot << " selected events)" << endl;
}