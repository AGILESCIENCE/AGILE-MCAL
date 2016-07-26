{	
	// comparison between 3901 and 3908 files for grid/burst calibration
	// MM 9/3/2008
	
	// event structure (from MCVLib MCVFile.h)
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
	mcalstyle->SetLabelSize(0.05, "xyz");
	mcalstyle->SetTitleSize(0.05, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
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
	int cont=0;
	
	// input files
	TFile fg("/home/mcal/data_analysis/calibration/grid_burst/RT003100_3550_3901.root");
	TFile fb("/home/mcal/data_analysis/calibration/RT003100_3550_3908_sel_01.root");
//	TFile fg("/home/mcal/data_analysis/calibration/grid_burst/RT003100_3103_3901.root");
//	TFile fb("/home/mcal/data_analysis/calibration/RT003100_3103_3908_sel_01.root");

//	fg.ls();
//	fb.ls();
	
	// retrieve data trees
	TTree *tg = NULL;
	TTree *tb = NULL;
	tg = (TTree *) fg->Get("tdata");
	tb = (TTree *) fb->Get("tdata_sel_0");
	//tb = (TTree *) fb->Get("tdata");
	int ng = tg->GetEntries();
	int nb = tb->GetEntries();
	cout << "GRID: " << ng << " entries; BURST: " << nb << " entries" << endl;
	
	// init histo
	TGraph *ga[30];
	int na[30];
	for (int i=0; i<30; i++) {
		TString gname("ga_");
		gname += i;
		ga[i] = new TGraph(10000);	// 1 ADC channel bin
		na[i] = 0;
	}
	TGraph *gb[30];
	for (int i=0; i<30; i++) {
		TString gname("gb_");
		gname += i;
		gb[i] = new TGraph(10000);	// 1 ADC channel bin
	}
	
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

	int ib=0;
	int ngood=0;
	tb->GetEntry(ib++);
	for (int i=0; i<ng; i++) {
		tg->GetEntry(i);
		if (i%20000 == 0) cout << "processed " << i << " events" << endl;
		while (ib<nb && evtg.time > evtb.time) {
			tb->GetEntry(ib++);
			if ( fabs(evtb.time - evtg.time - 0.000015) <= 0.000001 ) {
				ngood++;
				//printf("EVENT %06d \t%4d \t%4d \t%4d \t%20.6f \t%20.6f \t%6f \t%6f\n", cont, ngood, i, ib, evtg.time, evtb.time, evtg.Etot, evtb.Etot);
				int nbars = 0;
				for (int j=0; j<30; j++) {
					int idg = evtg.bar[j];
					int idb = evtb.bar[j];
					if (idg >= 0 && idb >= 0) {
						ga[j]->SetPoint(na[j], evtb.pdA[idb], evtg.pdA[idg]);
						gb[j]->SetPoint(na[j]++, evtb.pdB[idb], evtg.pdB[idg]);
						nbars++;
						if (nbars == evtg.mult) break;
					}
				}
			}
		} 
	}
	
	
	// graphic section
	TCanvas *c3 = new TCanvas("pdA", "pdA", 800, 600);
	c3->Divide(6, 5);
	TF1 *funa[30];
	TF1 *funb[30];
	double offa[30];
	double offb[30];
	double ma[30];
	double mb[30];
	double k2a[30];
	double k2b[30];
	double 	par[2];
	for (int i=0; i<30; i++) {
		c3->cd(i+1);
		TString fname("fa_");
		fname += i;
		funa[i] = new TF1(fname, "pol1", 50., 400.);
		ga[i]->Fit(fname, "QR0+");
		funa[i]->SetLineWidth(1);
		funa[i]->SetLineColor(2);
		k2a[i] = ga[i]->GetCorrelationFactor();
		ga[i]->Draw("AP");
		funa[i]->Draw("same");
		funa[i]->GetParameters(par);
		offa[i] = par[0];
		ma[i] = par[1];
		k2a[i] = ga[i]->GetCorrelationFactor();
		cout << offa[i] << "  " << ma[i] << "  " << k2a[i] << endl;
	}
	
	TCanvas *c4 = new TCanvas("pdB", "pdB", 800, 600);
	c4->Divide(6, 5);
	for (int i=0; i<30; i++) {
		c4->cd(i+1);
		TString fname("fb_");
		fname += i;
		funb[i] = new TF1(fname, "pol1", 50., 400.);
		gb[i]->Fit(fname, "QR0+");
		funb[i]->SetLineWidth(1);
		funb[i]->SetLineColor(2);
		k2b[i] = gb[i]->GetCorrelationFactor();
		gb[i]->Draw("AP");
		funb[i]->Draw("same");
		funb[i]->GetParameters(par);
		offb[i] = par[0];
		mb[i] = par[1];
		k2b[i] = gb[i]->GetCorrelationFactor();
	}
	
	// print results
	printf("id \toff_A \tm_A \tcorr_A \toff_B \tm_B \tcorr_B\n");
	for (int i=0; i<30; i++) {
		printf("%3d \t%8.3f \t%8.3f \t%8.6f \t%8.3f \t%8.3f \t%8.6f\n", i, offa[i], ma[i], k2a[i], offb[i], mb[i], k2b[i]);
	}
}
