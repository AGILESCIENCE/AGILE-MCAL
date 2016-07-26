{
	// MM 18/09/12 plot the high-energy TGF catalog
	// based on macro plot_tgf_catalog_1.C
		
	gROOT->Reset();
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	//mcalstyle->SetPalette(1,0);  // 2D graph colour version
	mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(0.9, "X");
	mcalstyle->SetTitleOffset(1.0, "Y");
	gROOT->SetStyle("mcalstyle");

	// -----------  connect input tree ----------- 
	
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

//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_HE.root");
	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_4/trg4_selection_01.root");
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
	int ntgf = ttrg->GetEntries();
	cout << "Total number of AGILE TGFs = " << ntgf << endl;

	
	// -----------  prepare histograms ----------- 

	// longitude histo
	int lonbin = 72; 
	int lonbinRH = 18; 
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, -180., 180.);
	TH1F *hlonRH = new TH1F("hlonRH", "hlonRH", lonbinRH, -180., 180.);
		
	// local time histo
	int lcbin = 12; 
	int lcbinRH = 12; 
	TH1F *hlc  = new TH1F("hlc", "hlc", lcbin, 0., 24.);
	TH1F *hlcRH  = new TH1F("hlcRH", "hlcRH", lcbinRH, 0., 24.);
	
	// detection monthly rate histo
	TH1F *hrate = new TH1F("hrate", "hrate", 36, 0, 36);
	
	// intensity distribution histo
	TH1F *hintensity = new TH1F("hintensity", "hintensity", 40, 0, 80);
	
	// duration distribution histo
	TH1F *hduration = new TH1F("hduration", "hduration", 30, 0., 3.);
	
	// time difference histo
	float tbin[] = {0.0003, 0.1, 0.2, 0.5, 1., 2., 5., 10., 20., 50., 100., 200., 500., 1000., 2000., 5000., 10000., 20000., 50000., 100000., 200000., 500000., 1.e6};
	TH1F *htimediff = new TH1F("htimediff", "htimediff", 22, tbin);

	
	//  ----------- load RHESSI TGF catalog and fill histo ----------- 
		
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
	
	RHESSI_tgf rtgf[1000];
	ifstream frhessi("/home/mcal/data_analysis/VSB/RHESSI-TGF-catalog/tgflist_Jan2003-Dec2005.txt", ios_base::in);
	int nrh=0;
	int nsel=0;
	frhessi.clear();
	while (!frhessi.eof()) {
		frhessi >> rtgf[nrh].date >> rtgf[nrh].start >> rtgf[nrh].path >> rtgf[nrh].lat >> rtgf[nrh].lon >> rtgf[nrh].alt >> rtgf[nrh].decim >> rtgf[nrh].decim_chan >> rtgf[nrh].loctime >> rtgf[nrh].earthangle;
		if(rtgf[nrh].lon>180.) rtgf[nrh].lon = rtgf[nrh].lon-360.;	// longitude correction
		
		int yy, mm, dd;
		char stmp[30];
		sscanf(rtgf[nrh].path, "%2d%2d%2d%s", &yy, &mm, &dd, stmp);

		if (rtgf[nrh].lat<=2.5 && rtgf[nrh].lat>=-2.5 ) {	// && (mm<=3 || mm>=6)
			hlonRH->Fill(rtgf[nrh].lon);
			hlcRH->Fill(rtgf[nrh].loctime);
			//cout << nsel << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << "  " << mm << endl;
			nsel++;
		}
		nrh++;
	}
	cout << "Total number of selected RHESSI TGFs = " << nsel << endl;
	
	// ----------- process entries & fill histograms ---------------

	int i=0;
	double t0prev = 0.;
	while (ttrg->GetEntry(i++)) {
		hlon->Fill(atrg.lon);
		hlc->Fill(atrg.loctime);
		hrate->Fill((atrg.year - 2009)*12 + atrg.month - 3);
		hintensity->Fill(atrg.counts);
		hduration->Fill(1000. * atrg.dt);
		htimediff->Fill(atrg.t0 - t0prev);
		t0prev = atrg.t0; 
		
		// print TGF info in makespectra format
		//printf("%6d \t%20.6f \t%.6f\n", atrg.contact, atrg.t0, atrg.dt);
	}
	
	// ----------- graphics section ---------------
	
	// longitude canvas
	
	TCanvas clon("clon", "clon", 800, 600);
	clon.SetTicks(1,1);
	hlon->Sumw2();
	hlon->Scale(1./(ntgf/100.*360./lonbin));
	hlonRH->Sumw2();
	hlonRH->Scale(1./(nsel/100.*360./lonbinRH));
	
	hlon->SetTitle("");
	hlon->GetXaxis()->CenterTitle();
//	hlon->GetYaxis()->SetLabelSize(0.06);
//	hlon->GetYaxis()->SetTitleSize(0.06);
//	hlon->GetYaxis()->SetTitleOffset(0.8);
	hlon->GetYaxis()->CenterTitle();
	hlon->GetXaxis()->SetTitle("Longitude (deg)");
	hlon->GetYaxis()->SetTitle("Normalized counts / deg (%)");
	hlon->GetYaxis()->SetRangeUser(0,3);
	hlon->GetYaxis()->SetDecimals(1);
	hlon->SetStats(0);
	hlon->SetMarkerStyle(1);//22
	//hlon->SetMarkerSize(1.2);
	hlon->SetFillColor(1);
	hlon->SetFillStyle(3005);
	hlon->SetLineWidth(2);
	hlon->Draw("HIST"); 
	hlon->Draw("LESAME"); 
	// hlonRH->SetLineColor(2);
	hlonRH->SetMarkerStyle(20); // 22 triangle
	hlonRH->SetMarkerSize(1.2);
	hlonRH->Draw("LE1SAME");
	
	// local time canvas
	
	TCanvas cloctime("cloctime", "cloctime", 800, 600);
	cloctime.SetTicks(1,1);
	hlc->Sumw2();
	hlc->Scale(1./(ntgf/100.*24./lcbin));
	hlcRH->Sumw2();
	hlcRH->Scale(1./(nsel/100.*24./lcbinRH));
	
	hlc->SetTitle("");
	hlc->GetXaxis()->CenterTitle();
	hlc->GetYaxis()->CenterTitle();
	hlc->GetXaxis()->SetTitle("Local time (hours)");
	hlc->GetYaxis()->SetTitle("Normalized counts / hour (%)");
	hlc->GetYaxis()->SetRangeUser(0,14);
	hlc->GetYaxis()->SetDecimals(1);
	hlc->SetStats(0);
	hlc->SetMarkerStyle(1);//22
	hlc->SetFillColor(1);
	hlc->SetFillStyle(3005);
	hlc->SetLineWidth(2);
	hlc->Draw("HIST"); 
	hlc->Draw("LESAME"); 
	hlcRH->SetMarkerStyle(20); // 22 triangle
	hlcRH->SetMarkerSize(1.2);
	hlcRH->Draw("LE1SAME");
	
	// monthly rate canvas
	
	TCanvas crate("crate", "crate", 800, 600);
	crate.SetTicks(1,1);
	hrate->SetTitle("");
	hrate->GetXaxis()->CenterTitle();
	hrate->GetYaxis()->CenterTitle();
	hrate->GetXaxis()->SetTitle("Month after March 2009");
	hrate->GetYaxis()->SetTitle("TGFs / month");
	hrate->SetStats(0);
	hrate->Draw("HIST"); 
	
	// intensity canvas
	
	TCanvas cintensity("cintensity", "cintensity", 800, 600);
	cintensity.SetTicks(1,1);
	cintensity.SetLogy(1);
	
	hintensity->Sumw2();
	hintensity->Scale(100./ntgf);
	
	hintensity->SetTitle("");
	hintensity->GetXaxis()->CenterTitle();
	hintensity->GetYaxis()->CenterTitle();
	hintensity->GetXaxis()->SetTitle("Counts per TGF");
	hintensity->GetYaxis()->SetTitle("Fraction of TGFs (%)");
	hintensity->SetStats(0);
	hintensity->SetMarkerStyle(1);
	hintensity->Draw("HIST"); 
	hintensity->Draw("LESAME"); 
	
	// duration canvas
	
	TCanvas cduration("cduration", "cduration", 800, 600);
	cduration.SetTicks(1,1);
	
	hduration->Scale(100./ntgf);
	
	hduration->SetTitle("");
	hduration->GetXaxis()->CenterTitle();
	hduration->GetYaxis()->CenterTitle();
	hduration->GetXaxis()->SetTitle("Total duration (ms)");
	hduration->GetYaxis()->SetTitle("Fraction of TGFs (%)");
	hduration->SetStats(0);
	hduration->Draw("HIST"); 
	
	// time difference canvas
	
	TCanvas ctimediff("ctimediff", "ctimediff", 800, 600);
	ctimediff.SetTicks(1,1);
	ctimediff.SetLogx();
	ctimediff.SetLogy();
	
	//htimediff->Scale(100./ntgf);
	
	htimediff->SetTitle("");
	htimediff->GetXaxis()->CenterTitle();
	htimediff->GetYaxis()->CenterTitle();
	htimediff->GetXaxis()->SetTitle("Time difference (minutes)");
	htimediff->GetYaxis()->SetTitle("Number of TGFs");
	htimediff->SetStats(0);
	htimediff->Draw("HIST"); 

	
	// ----------- print images -----------
	
	// .eps 
	
	//clon.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/lon_HE.eps");
	//cloctime.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/loctime_HE.eps");
	//crate.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/rate_HE.eps");
	//cintensity.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/intensity_HE.eps");
	//cduration.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/duration_HE.eps");
	
	// .gif 
	
	clon.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/lon_HE.gif");
	cloctime.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/loctime_HE.gif");
	crate.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/rate_HE.gif");
	cintensity.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/intensity_HE.gif");
	cduration.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/duration_HE.gif");
}