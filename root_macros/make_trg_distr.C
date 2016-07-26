int make_trg_distr(char *inpath="./tmp.dat", char *outpath="./map.gif"  , char *earthpath="/home/mcal/BUILD_MCALSW.BUILD14-devel/root_macros/earth.dat")
{
	// MM 20/08/09 Creates figures to be shown by the online perl script
	//             modified from tgf_candidates_analysis_3.C 
	
	// MM 18/08/09 added world map to geographycal distribution, after comments from Rev.#2 for TGF paper
	// MM 28/05/09 added selection of the RHESSI sample in time (June to March)
	// MM 14/04/09 created based on vsb_analysis_090320.C and plot_tgf_candidates.C
	// MM 25/03/09 modified to analyze Fullekrug data for dataset 09-03-20 (contacts 9787 - 9839)
	// MM 17/03/09 modified to analyze Fullekrug data for dataset 09-03-16 (contacts 9747 - 9780)
	// MM 12/03/09 modified to analyze Fullekrug data
	// MM 12/02/09 modified columns according to the new version of mcalanalizer
	// MM 01/12/08 include anche info sul background, messe fluence e bkg come float
	// MM 18/11/08 valuta la significativita' degli eventi su diversi tempi scala
	
	gROOT->Reset();
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);  // 2D graph colour version
	//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	mcalstyle->SetLineWidth(0.1);
	gROOT->SetStyle("mcalstyle");
	
	int process_event_data = 1;

	struct Trigger  {
			int runid;
			int trg;
			char date[20];
			char time[20];
			char type[5];
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			float lon;
			float lat;
			float theta;
			float hr;
			float counts;
			float cpeak;
			float t90;
	};
	
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
		
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
/*	TTree tvsb;
	Trigger atrg;
	 // tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_09-03-16/VSB_global_info_10.dat",	 // dataset 16/3
	 tvsb.ReadFile(inpath, "runid/I:trg/I:date/C:time/C:type/C:subms/I:flg01ms/I:flg16ms/I:lon/F:lat/F:theta/F:hr/F:counts/F:cpeak/F:t90/F");
	//"/home/mcal/tmp/last_mcal_triggers.dat"
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("flg01ms", &atrg.flg01ms);
	tvsb.SetBranchAddress("flg16ms", &atrg.flg16ms);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("theta", &atrg.theta);
	tvsb.SetBranchAddress("hr", &atrg.hr);
	tvsb.SetBranchAddress("counts", &atrg.counts);
	tvsb.SetBranchAddress("cpeak", &atrg.cpeak);
	tvsb.SetBranchAddress("t90", &atrg.t90);

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
*/	
	// ----------- create histograms ---------------
	int lonbin = 12; // 36
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, -180., 180.);
	
	// canvas for geographical distribution
	TCanvas *c11 = new TCanvas("c11", "c11", 800,400);	//1000,300
	TPad *p1 = new TPad("p1", "p1", 0.01, 0.51, 0.75, 0.99);
	TPad *p2 = new TPad("p2", "p2", 0.80, 0.01, 0.99, 0.99);
	TPad *p3 = new TPad("p3", "p3", 0.01, 0.01, 0.75, 0.49);
	p1->Draw();
	p2->Draw();
	p3->Draw();
	p1->cd();
	TH2F *htmp = new TH2F("htmp", "htmp", 360, -180., 180., 20, -10., 10.); // 60, -3., 3.
	
	// fill with Earth contour (from example earth.C)
	ifstream in;
	in.open(earthpath);
	if (!in) {
		cout << "Unable to open file (earth.dat)";
		exit(1); // terminate with error
	}
	Float_t x,y;
	while (in >> x >> y) {
		htmp->Fill(x,y, 1);
	}
	in.close();
	
	htmp->SetStats(0);
	htmp->SetTitle("");
	htmp->GetXaxis()->SetTitle("Longitude (deg)");
	htmp->GetXaxis()->CenterTitle();
	htmp->GetXaxis()->SetTitleOffset(0.8);
	htmp->GetYaxis()->SetTitle("Latitude (deg)");
	htmp->GetYaxis()->CenterTitle();
	htmp->GetYaxis()->SetTitleOffset(0.8);
	htmp->SetContour(2);
	htmp->Draw("cont3"); // mercator
	
	p1->cd();
	TLine l1(-180., 2.5, 180., 2.5);
	l1.SetLineStyle(2);
	l1.Draw();
	TLine l2(-180., -2.5, 180., -2.5);
	l2.SetLineStyle(2);
	l2.Draw();
	p1->Update();
	
	// read RHESSI catalog data and fill histo
	
/*	RHESSI_tgf rtgf[1000];
  	ifstream frhessi("/home/mcal/data_analysis/VSB/RHESSI-TGF-catalog/tgflist_before-Jan-06.txt", ios_base::in);
	int nrh=0;
	int nsel=0;
	frhessi.clear();
	while (!frhessi.eof()) {
		frhessi >> rtgf[nrh].date >> rtgf[nrh].start >> rtgf[nrh].path >> rtgf[nrh].lat >> rtgf[nrh].lon >> rtgf[nrh].alt >> rtgf[nrh].decim >> rtgf[nrh].decim_chan >> rtgf[nrh].loctime >> rtgf[nrh].earthangle;
		if(rtgf[nrh].lon>180.) rtgf[nrh].lon = rtgf[nrh].lon-360.;	// longitude correction
		// cout << nrh << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << endl;
		
		int yy, mm, dd;
		char stmp[30];
		sscanf(rtgf[nrh].path, "%2d%2d%2d%s", &yy, &mm, &dd, stmp);

		if (rtgf[nrh].lat<=2.5 && rtgf[nrh].lat>=-2.5 && (mm<=3 || mm>=6)) {
			hlonRH->Fill(rtgf[nrh].lon);
			hlcRH->Fill(rtgf[nrh].loctime);
			cout << nsel << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << "  " << mm << endl;
			nsel++;
		}
		
		nrh++;
	}
*/	
	// ----------- process entries & fill histograms ---------------
	
	float hr, hr1;	// hardness ratio
	float lc; 	// local time
	int ncGRB=0;
	int ncBUS=0;
	double t0prev = 0.;
	
	int nmax = 1000;
	Trigger *atrg = new Trigger[nmax];
	ifstream inFile;
	inFile.open(inpath);
	if (!inFile) {
		cout << "Unable to open file (trigger data)";
		exit(1); // terminate with error
	}
	int nentries = 0;
	while (inFile >>  atrg[nentries].runid >> atrg[nentries].trg >> atrg[nentries].date >> atrg[nentries].time >> atrg[nentries].type >> atrg[nentries].subms >> atrg[nentries].flg01ms >> atrg[nentries].flg16ms >> atrg[nentries].lon >> atrg[nentries].lat >> atrg[nentries].theta >> atrg[nentries].hr >> atrg[nentries].counts >> atrg[nentries].cpeak >> atrg[nentries].t90) {
		
		// calculate local time
		int hh, mm, ss;
		sscanf(atrg[nentries].time, "%2d:%2d:%2d", &hh, &mm, &ss);
		lc = fmod(hh + mm/60. + ss/3600. + atrg[nentries].lon*0.06667, 24.);
		//hlc->Fill(lc);
		
		if(atrg[nentries].lon>180.) atrg[nentries].lon = atrg[nentries].lon-360.;	// longitude correction				
		hlon->Fill(atrg[nentries].lon);
		
		// draw a marker in the geographical distribution
		if (atrg[nentries].counts>0. && atrg[nentries].cpeak>0.) {
			TMarker *m;
			int markertype = 4;
			int color = 1;
			if (atrg[nentries].hr < 0.2) {	// basso HR: marker vuoti
				if (atrg[nentries].type[0]=='G' ) markertype = 4; // cerchi: GRB
				else markertype = 26;
			} else {		// alto HR: marker pieni
				if (atrg[nentries].type[0]=='G' ) markertype = 8; // cerchi: GRB
				else markertype = 22;
			}
			m = new TMarker(atrg[nentries].lon, atrg[nentries].lat, markertype); // 
			// scale dimension according to fluence
			float dim =  0.08*atrg[nentries].counts;  //0.1
			m->SetMarkerSize(dim);
			
			// scale color according to HR
			if (atrg[nentries].hr < 0.5) color = 1;
			else if (atrg[nentries].hr < 1.) color = 2;
			else if (atrg[nentries].hr < 2.) color = 5;
			else if (atrg[nentries].hr < 3.) color = 3;
			else if (atrg[nentries].hr < 4.) color = 4;
			else color = 6;
			
			m->SetMarkerColor(color);
			m->Draw();
		}		
		nentries++;
		if (nentries == nmax) {
			cout << "Warning: reached maximum number of triggers (" << nentries << ")" << endl;
			exit(1); // terminate with error
		}
	}
	inFile.close();
	if (!nentries) {
		cout << "Zero entries read from trigger file " << inpath << endl;
		exit(1); // terminate with error
	}
	cout << "Processed " << nentries << " entries" << endl;
	
/*	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		
		// calculate local time
		int hh, mm, ss;
		sscanf(atrg.time, "%2d:%2d:%2d", &hh, &mm, &ss);
		lc = fmod(hh + mm/60. + ss/3600. + atrg.lon*0.06667, 24.);
		//hlc->Fill(lc);
		
		if(atrg.lon>180.) atrg.lon = atrg.lon-360.;	// longitude correction				
		hlon->Fill(atrg.lon);
		//hdttot->Fill(atrg.dt);					
		//htgrbini->Fill(atrg.tgrbini);
			
		
		// draw a marker in the geographical distribution
		if (atrg.counts>0. && atrg.cpeak>0.) {
			TMarker *m;
			int markertype = 4;
			int color = 1;
			if (atrg.hr < 0.2) {	// basso HR: marker vuoti
				if (atrg.type[0]=='G' ) markertype = 4; // cerchi: GRB
				else markertype = 26;
			} else {		// alto HR: marker pieni
				if (atrg.type[0]=='G' ) markertype = 8; // cerchi: GRB
				else markertype = 22;
			}
			m = new TMarker(atrg.lon, atrg.lat, markertype); // 
			// scale dimension according to fluence
			float dim =  0.08*atrg.counts;  //0.1
			m->SetMarkerSize(dim);
			
			// scale color according to HR
			if (atrg.hr < 0.5) color = 1;
			else if (atrg.hr < 1.) color = 2;
			else if (atrg.hr < 2.) color = 5;
			else if (atrg.hr < 3.) color = 3;
			else if (atrg.hr < 4.) color = 4;
			else color = 6;
			
			m->SetMarkerColor(color);
			m->Draw();
		}
		// -------------------------------------
			
	}
	p1->Update();
*/	

	
	// ----------- graphic section ---------------
	// c11->cd(2);
	p3->cd();
	hlon->SetTitle("");
	hlon->SetStats(0);
	hlon->GetXaxis()->SetTitle("Longitude (deg)");
	hlon->GetXaxis()->CenterTitle();
	hlon->GetXaxis()->SetTitleOffset(0.8);
	hlon->GetYaxis()->SetTitle("Number of triggers");
	hlon->GetYaxis()->CenterTitle();
	hlon->GetYaxis()->SetTitleOffset(0.8);
	hlon->Draw();
	
	// draw a sort of legend...
	p2->cd();
	TText *t = new TText();
	t->SetTextSize(0.13);
	
	TMarker *m0 = new TMarker(0.1, 0.9, 20); 
	m0->SetMarkerSize(2.);
	m0->SetMarkerColor(1);
	m0->Draw();
	t->DrawTextNDC(0.2, 0.88, "0.2 < HR < 0.5");
	
	TMarker *m1 = new TMarker(0.1, 0.8, 20); 
	m1->SetMarkerSize(2.);
	m1->SetMarkerColor(2);
	m1->Draw();
	t->DrawTextNDC(0.2, 0.78, "0.5 < HR < 1.");
	
	TMarker *m2 = new TMarker(0.1, 0.7, 20); 
	m2->SetMarkerSize(2.);
	m2->SetMarkerColor(5);
	m2->Draw();
	t->DrawTextNDC(0.2, 0.68, "1.  < HR < 2.");
	
	TMarker *m3 = new TMarker(0.1, 0.6, 20); 
	m3->SetMarkerSize(2.);
	m3->SetMarkerColor(3);
	m3->Draw();
	t->DrawTextNDC(0.2, 0.58, "2.  < HR < 3.");
	
	TMarker *m4 = new TMarker(0.1, 0.5, 20); 
	m4->SetMarkerSize(2.);
	m4->SetMarkerColor(4);
	m4->Draw();
	t->DrawTextNDC(0.2, 0.48, "3.  < HR < 4.");
	
	TMarker *m5 = new TMarker(0.1, 0.4, 20); 
	m5->SetMarkerSize(2.);
	m5->SetMarkerColor(6);
	m5->Draw();
	t->DrawTextNDC(0.2, 0.38, "       HR > 4.");
	
	TMarker *m6 = new TMarker(0.15, 0.3, 24); 
	m6->SetMarkerSize(1.);
	m6->Draw();
	t->DrawTextNDC(0.3, 0.28, "10 counts");
	
	TMarker *m7 = new TMarker(0.15, 0.2, 24); 
	m7->SetMarkerSize(2.);
	m7->Draw();
	t->DrawTextNDC(0.3, 0.18, "20 counts");
	
	TMarker *m8 = new TMarker(0.15, 0.11, 24); 
	m8->SetMarkerSize(3.);
	m8->Draw();
	t->DrawTextNDC(0.3, 0.08, "30 counts");	
	
	TMarker *m9 = new TMarker(0.15, 0.03, 26); 
	m9->SetMarkerSize(2.);
	m9->Draw();
	t->DrawTextNDC(0.3, 0.0, "instrumental");	
	p2->Update();
	
	c11->Print(outpath);
	
	return 0;
}