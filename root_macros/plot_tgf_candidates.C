{
	// MM 19/02/09 produce localization plots for selected triggers only (good TGF candidates)

	
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
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	// ------------ read tgf_candidats file  ----------------
	
	struct entry {
		int runid;
		int trg;
		float fluence;
		float pflux;
		int subms;
		float hr;
		float lon;
		float lat;
		float dt;
		float t0;
		float t90;
	};
	
	ifstream fin;
	fin.open("/data1/mcal/data_analysis/VSB/tgf_candidates.dat", ifstream::in);
	int ntrg;
	fin >> ntrg;
	cout << "Reading tmin/tmax auxiliary file - number of trigger to be processed = " << ntrg << endl;
	entry *tgf = new entry[ntrg];
	
	for (int i=0; i<ntrg; i++) {
		fin >> tgf[i].runid >> tgf[i].trg >> tgf[i].fluence >> tgf[i].pflux >>  tgf[i].subms >> tgf[i].hr >> tgf[i].lon >> tgf[i].lat >> tgf[i].dt >> tgf[i].t0 >> tgf[i].t90;
	}
	
	
	
	TH2F *htmp = new TH2F("htmp", "htmp", 360, -180., 180., 60, -3., 3.);
	htmp->SetStats(0);
	htmp->SetTitle("");
	
	TH1F *hlon = new TH1F("hlon", "hlon", 12, -180., 180.);
	hlon->SetStats(0);
	hlon->SetTitle("Longitude distribution");
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800,600);	//1000,300
	c1->Divide(1,2);
	c1->cd(1);
	htmp->Draw();
	for (int i=0; i<ntrg; i++) {
		TMarker *m = new TMarker(tgf[i].lon, tgf[i].lat, 8);
		
		// scale dimension according to fluence
		float dim =  0.1*tgf[i].fluence;
		m->SetMarkerSize(dim);
		
		// scale color according to HR
		int color;
		if (tgf[i].hr < 1.) color = 2;
		else if (tgf[i].hr < 2.) color = 5;
		else if (tgf[i].hr < 3.) color = 3;
		else if (tgf[i].hr < 4.) color = 4;
		else color = 6;
		
		m->SetMarkerColor(color);
		m->Draw();
		
		hlon->Fill(tgf[i].lon);
	}
	c1->cd(2);
	hlon->Draw();
}
	