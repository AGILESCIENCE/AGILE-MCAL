{
	// MM 08/01/13 print also height above Earth surface, needed to calculate time delay from sferics
	// MM 25/10/12 add T50 caomparison between AGILE, Fermi-GBM and RHESSI (new)
	// MM 20/08/12 use file produced by macro plot_triggers_5.C (tgf dataset)
	// plots based on macro tgf_candidates_analysis_8.C
		
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
	
	int rate = 0;	// if 1 load triggers and calculare trigger type per TGF (time consuming)

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
	
	struct TriggerBasic  { // struct for mcal_grb.dat data
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int rmid[9];
			int sitid[9];
			int thr[9];
			int ber[9];
			int buffer[4];
	};

//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_HE.root");
//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_new.root");
//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Aug12-Dec13.root");
//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-Dec13.root");
//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-17Feb15.root");
	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf.root");
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
	int ntgf = 0;
	
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
	TH1F *hrate = new TH1F("hrate", "hrate", 74, 0, 74);	//41
	TH1F *htrigger = new TH1F("htrigger", "htrigger", 8, 0, 8);	//36
	float exposure[] = {2231858., 2054233., 2109716., 2144276., 2206428., 2055905.,  2059397., 1235838., 1892911.,   2206900., 2089201., 1828504., 2090167., 1701923., 2032964., 1883393., 2156869., 2114825., 2070247., 2156912., 2006117., 2123715., 2154788., 1859365., 2194036., 2042841., 2041191., 2119223., 2158066., 904915., 1987115., 2212640., 2096257., 2252846., 2242495., 1830554., 1830554., 1830554., 1830554., 1830554., 1830554.}; // exposure per month in seconds NB: *** CORRECT THE EXPOSURE VALUES FROM MAR.12 TO JUL.12 (COPIED FROM fEB.12 VALUE) ***
	TH1F *hexpo = new TH1F("hexpo", "hexpo", 41, 0, 41); // exposure histogram (days)
	for (int j=1; j<=41; j++) hexpo->SetBinContent(j, exposure[j-1]/86400.); 
	
	// intensity distribution histo
	TH1F *hintensity = new TH1F("hintensity", "hintensity", 40, 0, 80);
	
	// duration distribution histo
	TH1F *hduration = new TH1F("hduration", "hduration", 120, 0., 3.);
	TH1F *hdurationGBM = new TH1F("hdurationGBM", "hdurationGBM", 120, 0., 3.); 
	TH1F *hdurationRH = new TH1F("hdurationRH", "hdurationRH", 300, 0., 3.); // RHESSI new events by T. Gjesteland
	
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
	
	
	// ---------- load MCAL trigger file mcal_grb.dat ----------
	
	ifstream fin;
	TriggerBasic amcaltrg;
	double *t03908, *auxt03908;
	int *subms3908, *flg01ms3908, *flg16ms3908, *auxsubms3908, *auxflg01ms3908, *auxflg16ms3908;
	
/*	if (rate) {
		auxt03908 = new double[500000];
		auxsubms3908 = new int[500000];
		auxflg01ms3908 = new int[500000];
		auxflg16ms3908 = new int[500000];
		fin.open("/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive", ifstream::in); //mcal_grb.dat
		int ntrg = 0;
		while (!fin.eof()) {
			fin >> amcaltrg.runid >> amcaltrg.trg >> amcaltrg.t0 >> amcaltrg.tstop >> amcaltrg.ttot >> amcaltrg.date >> amcaltrg.time >> amcaltrg.mint3908 >> amcaltrg.maxt3908 >> amcaltrg.type >> amcaltrg.sa >> amcaltrg.lng >> amcaltrg.subms >> amcaltrg.flg01ms >> amcaltrg.flg16ms >> amcaltrg.nrm;
			for (int i=0; i<amcaltrg.nrm; i++) fin >> amcaltrg.rmid[i] >> amcaltrg.sitid[i] >>  amcaltrg.thr[i] >> amcaltrg.ber[i];
			for (int i=0; i<4; i++) fin >> amcaltrg.buffer[i];
			auxt03908[ntrg] = amcaltrg.t0;
			auxsubms3908[ntrg]   = amcaltrg.subms;
			auxflg01ms3908[ntrg] = amcaltrg.flg01ms;
			auxflg16ms3908[ntrg] = amcaltrg.flg16ms;
			ntrg++;
		}
		fin.close();
		ntrg--;
		cout << "sorting times..." << endl;
		int *ind3908 = new int[ntrg];
		TMath::Sort(ntrg, auxt03908, ind3908, 0);
		cout << "number of on-board triggers = " << ntrg << endl;
		
		t03908 = new double[ntrg];
		subms3908 = new int[ntrg];
		flg01ms3908 = new int[ntrg];
		flg16ms3908 = new int[ntrg];
		for (int k=0; k<ntrg; k++) {
			t03908[k] = auxt03908[ind3908[k]];
			subms3908[k] = auxsubms3908[ind3908[k]];
			flg01ms3908[k] = auxflg01ms3908[ind3908[k]];
			flg16ms3908[k] = auxflg16ms3908[ind3908[k]];
		}
		delete auxt03908;
		delete auxsubms3908;
		delete auxflg01ms3908;
		delete auxflg16ms3908;
		delete ind3908;
	}*/
	
	// ----------- process entries & fill histograms ---------------

	int i=0;
	double t0prev = 0.;
	while (ttrg->GetEntry(i++)) {
	
		if (atrg.t0 - t0prev > 0.010 ) { // DO NOT CONSIDER MULTIPLE PEAKS AS DIFFERENT TGFs
	
			ntgf++;
			hlon->Fill(atrg.lon);
			hlc->Fill(atrg.loctime);
			hrate->Fill((atrg.year - 2009)*12 + atrg.month - 3);
			hintensity->Fill(atrg.counts);
			//hduration->Fill(1000. * atrg.dt);
			htimediff->Fill(atrg.t0 - t0prev);
			
// 			if (rate) {	// check if offline trigger is close to an onboard trigger
// 				int flag = 0; 	// defult 0: farther than 20ms from an onboard trg
// 				int id = TMath::BinarySearch(ntrg, t03908, atrg.t0);
// 				if (id>=0 && id<ntrg-1 && (fabs(t03908[id+1] - atrg.t0) < fabs(t03908[id] - atrg.t0))) id++;
// 				
// 				if (id>=0) {
// 					printf("%06d \t%20.6f \t%10.6f \t%2d \t%2d \t%2d \t%2d\n", atrg.contact, atrg.t0, t03908[id] - atrg.t0, subms3908[id], flg01ms3908[id], flg16ms3908[id], subms3908[id] + 2*flg01ms3908[id] + 4*flg16ms3908[id]);
// 					htrigger->Fill(subms3908[id] + 2*flg01ms3908[id] + 4*flg16ms3908[id]);
// 				} else printf("%06d \t%20.6f ***\n", atrg.contact, atrg.t0); 
// 			
// 			}
			
			
			// print TGF info in makespectra format
			//printf("%6d \t%20.6f \t%.6f\n", atrg.contact, atrg.t0, atrg.dt);
			
			// print TGF info for sferics correlation
			printf("%06d \t%20.6f \t%4d \t%02d \t%2d \t%2d \t%2d \t%2d.%06d \t%8.2f \t%8.2f \t%8.2f\n", atrg.contact, atrg.t0, atrg.year, atrg.month, atrg.day, atrg.hour, atrg.min, atrg.sec, atrg.usec, atrg.lon, atrg.lat, 0.001*atrg.h);
		}
		t0prev = atrg.t0; 
	}
	cout << "Total number of AGILE TGFs = " << ntgf << endl;
	
	// ----------- load AGILE timing data & fill histograms ---------------

	ifstream ftiming("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-Jul12_TIMING.dat", ios_base::in);
	int ntime = 0;
	float sigma;
	float sec;
	while (ftiming >> atrg.contact >> atrg.t0 >> atrg.year >> atrg.month >> atrg.day >> atrg.hour >> atrg.min >> sec >> atrg.lon >> atrg.lat >> atrg.dt >> sigma) {
		hduration->Fill(sigma*1349.);
		ntime++;
	}
	cout << "Total number of AGILE TGFs with good timing data = " << ntime << endl;
	
	// ----------- load Fermi-GBM timing data (Fishman et al., JGR 2011) & fill histograms ---------------

	ifstream fgbm("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/Fermi_T50.dat", ios_base::in);
	int ngbm = 0;
	float t50gbm;
	while (fgbm >> t50gbm) {
		hdurationGBM->Fill(t50gbm);
		ngbm++;
	}
	cout << "Total number of Fermi GBM TGFs with timing data = " << ngbm << endl;
	
	// ----------- load RHESSI new events timing data (Gjesteland private comm.) & fill histograms ---------------

	ifstream frh("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/RHESSI_new_T50.dat", ios_base::in);
	int nrh = 0;
	float t50rh;
	while (frh >> t50rh) {
		hdurationRH->Fill(t50rh);
		nrh++;
	}
	cout << "Total number of RHESSI new TGFs with timing data = " << nrh << endl;
	
	
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
	
	TCanvas crate("crate", "crate", 800, 1200);
	crate.Divide(1,2);
	
	crate.cd(1);
	gPad->SetTicks(1,1);
	gPad->SetBottomMargin(0.20);
	hrate->SetTitle("");
	hrate->GetYaxis()->SetRangeUser(0., 20.);
	hrate->GetXaxis()->CenterTitle();
	hrate->GetYaxis()->CenterTitle();
	hrate->GetXaxis()->SetTitle("Month");
	hrate->GetYaxis()->SetTitle("Detected TGFs / month");
	hrate->SetStats(0);
	
	hrate->GetXaxis()->SetBinLabel(2, "Apr 09");
	hrate->GetXaxis()->SetBinLabel(5, "Jul 09");
	hrate->GetXaxis()->SetBinLabel(8, "Oct 09");
	hrate->GetXaxis()->SetBinLabel(11, "Jan 10");
	hrate->GetXaxis()->SetBinLabel(14, "Apr 10");
	hrate->GetXaxis()->SetBinLabel(17, "Jul 10");
	hrate->GetXaxis()->SetBinLabel(20, "Oct 10");
	hrate->GetXaxis()->SetBinLabel(23, "Jan 11");
	hrate->GetXaxis()->SetBinLabel(26, "Apr 11");
	hrate->GetXaxis()->SetBinLabel(29, "Jul 11");
	hrate->GetXaxis()->SetBinLabel(32, "Oct 11");
	hrate->GetXaxis()->SetBinLabel(35, "Jan 12");
	hrate->GetXaxis()->SetBinLabel(38, "Apr 12");
	hrate->GetXaxis()->SetBinLabel(41, "Jul 12");
	hrate->GetXaxis()->SetBinLabel(44, "Oct 12");
	hrate->GetXaxis()->SetBinLabel(47, "Jan 13");
	hrate->GetXaxis()->SetBinLabel(50, "Apr 13");
	hrate->GetXaxis()->SetBinLabel(53, "Jul 13");
	hrate->GetXaxis()->SetBinLabel(56, "Oct 13");
	hrate->GetXaxis()->SetBinLabel(59, "Jan 14");
	hrate->GetXaxis()->SetBinLabel(62, "Apr 14");
	hrate->GetXaxis()->SetBinLabel(65, "Jul 14");
	hrate->GetXaxis()->SetBinLabel(68, "Oct 14");
	hrate->GetXaxis()->SetBinLabel(71, "Jan 15");
	hrate->GetXaxis()->SetBinLabel(74, "Apr 15");
   	hrate->LabelsOption("v");
	hrate->GetXaxis()->SetTitleOffset(1.8);
	hrate->SetMarkerStyle(22);
	hrate->DrawClone(); 
	
	TLine *l1 = new TLine(8.7, 0., 8.7, 20.);
	l1->SetLineStyle(2);
	l1->Draw();
	
	TLine *l2 = new TLine(32.13, 0., 32.13, 20.);
	l2->SetLineStyle(2);
	l2->Draw();
	
	TLine *l3 = new TLine(38.57, 0., 38.57, 20.);
	l3->SetLineStyle(2);
	l3->Draw();
		
	crate.cd(2);
	gPad->SetTicks(1,1);
	gPad->SetBottomMargin(0.20);
	hrate->Sumw2();
	TH1F hratenorm = (*hrate)/(*hexpo);
	hratenorm->SetTitle("");
	hratenorm->GetXaxis()->CenterTitle();
	hratenorm->GetYaxis()->SetRangeUser(0., 1.2);
	hratenorm->GetYaxis()->CenterTitle();
	hratenorm->GetXaxis()->SetTitle("Month");
	hratenorm->GetYaxis()->SetTitle("Exposure corrected TGFs / day");
	hratenorm->SetStats(0);
	
	hratenorm->GetXaxis()->SetBinLabel(2, "Apr 09");
	hratenorm->GetXaxis()->SetBinLabel(5, "Jul 09");
	hratenorm->GetXaxis()->SetBinLabel(8, "Oct 09");
	hratenorm->GetXaxis()->SetBinLabel(11, "Jan 10");
	hratenorm->GetXaxis()->SetBinLabel(14, "Apr 10");
	hratenorm->GetXaxis()->SetBinLabel(17, "Jul 10");
	hratenorm->GetXaxis()->SetBinLabel(20, "Oct 10");
	hratenorm->GetXaxis()->SetBinLabel(23, "Jan 11");
	hratenorm->GetXaxis()->SetBinLabel(26, "Apr 11");
	hratenorm->GetXaxis()->SetBinLabel(29, "Jul 11");
	hratenorm->GetXaxis()->SetBinLabel(32, "Oct 11");
	hratenorm->GetXaxis()->SetBinLabel(35, "Jan 12");
	hratenorm->GetXaxis()->SetBinLabel(38, "Apr 12");
	hratenorm->GetXaxis()->SetBinLabel(41, "Jul 12");
   	hratenorm->LabelsOption("v");
	hratenorm->GetXaxis()->SetTitleOffset(1.8);
	hratenorm->SetMarkerStyle(22);
	hratenorm->Draw(); 
	
	
	// trigger canvas
	
	if (rate) {
	//	TCanvas ctrigger("ctrigger", "ctrigger", 800, 600);
	//	ctrigger.SetTicks(1,1);
	//	htrigger->Draw();
		
		htrigger->Scale(1./ntgf);
		for (int j=1; j<=8; j++) printf("Fraction of events with trigger configuration n. %2d = %.3f\n", j, htrigger->GetBinContent(j));
	
	}
	
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
	
	//hduration->Scale(100./ntgf);
	
	hduration->SetTitle("");
	hduration->GetXaxis()->SetRangeUser(0., 1.);
	hduration->GetXaxis()->CenterTitle();
	hduration->GetYaxis()->CenterTitle();
	hduration->GetXaxis()->SetTitle("t_{50} (ms)");
	hduration->GetYaxis()->SetTitle("Number of TGFs / bin");
	hduration->SetStats(0);
//	hduration->Scale(1./(0.025*ntime));
	hduration->Draw("HIST"); 
	
	hdurationGBM->SetStats(0);
	hdurationGBM->SetLineStyle(2);
//	hdurationGBM->Scale(1./(0.025*ngbm));
	hdurationGBM->Draw("SAME"); 
	
	hdurationRH->SetStats(0);
	hdurationRH->SetLineStyle(3);
//	hdurationRH->Scale(1./(0.010*nrh));
//	hdurationRH->Draw("SAME"); 
	
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
	
/* 	clon.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/lon.eps");
 	cloctime.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/loctime.eps");
	crate.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/rate.eps");
	cintensity.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/intensity.eps");
	cduration.Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/images/duration.eps");*/
}
