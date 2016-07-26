{

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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");
	
	int process_event_data = 1;

	struct Trigger  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			char type[5];
			int sa;
			int lng;
			int subms;
			int nrm;
			int rmid;
			int sitid;
			float lon;
			float lat;
			float h;
			float dt;
			int ntrg;	// number of reconstructed sub-ms triggers
			float mint;	// minimum time (should be ~-3. for datasets with correct pre-burst)
			float dt0;	// time difference of the found trigger with respect to the initial t0 (bstart)
			float tgrbini;
			float t90;
			float tbin;	// added since 15/10/08
			float cbkg;	// added since 15/10/08 
			int counts;
			int cpeak;
			float avgE;
			int nEbins;
			int c0;
			int c1;
			int c2;
			int c3;
			int x1;
			int x2;
			int x3;
			int x4;
			int x5;
			int x6;
			int x7;
			int x8;
			int x9;
			int x10;
			int x11;
			int x12;
			int x13;
			int x14;
			int x15;
			int z1;
			int z2;
			int z3;
			int z4;
			int z5;
			int z6;
			int z7;
			int z8;
			int z9;
			int z10;
			int z11;
			int z12;
			int z13;
			int z14;
			int z15;
			int c1;		// spatial zones (cuboni)
			int c2;
			int c3;
			int c4;
			int c5;
			int c6;
			int c7;
			int c8;
	};
	
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
	TTree tvsb;
	Trigger atrg;
	// "/data1/mcal/data_analysis/VSB/analysis_08-10-27_16ms-only_BUS/VSB_global_info_8.dat"
	tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_08-11-17_16ms_only/VSB_global_info_8.dat", 	"runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:type/C:sa/I:lng/I:subms/I:nrm/I:rmid/I:sitid/I:lon/F:lat/F:h/F:dt/F:ntrg/I:mint/F:dt0/F:tgrbini/F:t90/F:tbin/F:cbkg/F:counts/I:cpeak/I:avgE/F:nEbins/I:c0/I:c1/I:c2/I:c3/I:x1/I:x2/I:x3/I:x4/I:x5/I:x6/I:x7/I:x8/I:x9/I:x10/I:x11/I:x12/I:x13/I:x14/I:x15/I:z1/I:z2/I:z3/I:z4/I:z5/I:z6/I:z7/I:z8/I:z9/I:z10/I:z11/I:z12/I:z13/I:z14/I:z15/I:c1/I:c2/I:c3/I:c4/I:c5/I:c6/I:c7/I:c8/I");
	
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("h", &atrg.h);
	tvsb.SetBranchAddress("dt", &atrg.dt);
	tvsb.SetBranchAddress("ntrg", &atrg.ntrg);
	tvsb.SetBranchAddress("mint", &atrg.mint);
	tvsb.SetBranchAddress("dt0", &atrg.dt0);
	tvsb.SetBranchAddress("tgrbini", &atrg.tgrbini);
	tvsb.SetBranchAddress("t90", &atrg.t90);
	tvsb.SetBranchAddress("tbin", &atrg.tbin);
	tvsb.SetBranchAddress("cbkg", &atrg.cbkg);
	tvsb.SetBranchAddress("counts", &atrg.counts);
	tvsb.SetBranchAddress("cpeak", &atrg.cpeak);
	tvsb.SetBranchAddress("avgE", &atrg.avgE);
	tvsb.SetBranchAddress("nEbins", &atrg.nEbins);
	tvsb.SetBranchAddress("c0", &atrg.c0);
	tvsb.SetBranchAddress("c1", &atrg.c1);
	tvsb.SetBranchAddress("c2", &atrg.c2);
	tvsb.SetBranchAddress("c3", &atrg.c3);
	tvsb.SetBranchAddress("x1", &atrg.x1);
	tvsb.SetBranchAddress("x2", &atrg.x2);
	tvsb.SetBranchAddress("x3", &atrg.x3);
	tvsb.SetBranchAddress("x4", &atrg.x4);
	tvsb.SetBranchAddress("x5", &atrg.x5);
	tvsb.SetBranchAddress("x6", &atrg.x6);
	tvsb.SetBranchAddress("x7", &atrg.x7);
	tvsb.SetBranchAddress("x8", &atrg.x8);
	tvsb.SetBranchAddress("x9", &atrg.x9);
	tvsb.SetBranchAddress("x10", &atrg.x10);
	tvsb.SetBranchAddress("x11", &atrg.x11);
	tvsb.SetBranchAddress("x12", &atrg.x12);
	tvsb.SetBranchAddress("x13", &atrg.x13);
	tvsb.SetBranchAddress("x14", &atrg.x14);
	tvsb.SetBranchAddress("x15", &atrg.x15);
	tvsb.SetBranchAddress("z1", &atrg.z1);
	tvsb.SetBranchAddress("z2", &atrg.z2);
	tvsb.SetBranchAddress("z3", &atrg.z3);
	tvsb.SetBranchAddress("z4", &atrg.z4);
	tvsb.SetBranchAddress("z5", &atrg.z5);
	tvsb.SetBranchAddress("z6", &atrg.z6);
	tvsb.SetBranchAddress("z7", &atrg.z7);
	tvsb.SetBranchAddress("z8", &atrg.z8);
	tvsb.SetBranchAddress("z9", &atrg.z9);
	tvsb.SetBranchAddress("z10", &atrg.z10);
	tvsb.SetBranchAddress("z11", &atrg.z11);
	tvsb.SetBranchAddress("z12", &atrg.z12);
	tvsb.SetBranchAddress("z13", &atrg.z13);
	tvsb.SetBranchAddress("z14", &atrg.z14);
	tvsb.SetBranchAddress("z15", &atrg.z15);
	tvsb.SetBranchAddress("c1", &atrg.c1);
	tvsb.SetBranchAddress("c2", &atrg.c2);
	tvsb.SetBranchAddress("c3", &atrg.c3);
	tvsb.SetBranchAddress("c4", &atrg.c4);
	tvsb.SetBranchAddress("c5", &atrg.c5);
	tvsb.SetBranchAddress("c6", &atrg.c6);
	tvsb.SetBranchAddress("c7", &atrg.c7);
	tvsb.SetBranchAddress("c8", &atrg.c8);

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
	// load the trigger file flagged by FF (BOOST and ECLIPSE events are already rejected)
	
	// ----------- load energy bands for spectra calculation ----------
	
	ifstream echannels;
	string str;
	echannels.open("/home/martino/mcalsw/share/mcalfits_conf/canali_KW_2_modified.dat", ifstream::in);
	int nrow=0;
	while ( echannels.good() && !echannels.eof() ) {
		getline(echannels, str);
		nrow++;
	}
	echannels.close();
	echannels.clear();
	float *elo = new float[nrow];
	float *ehi = new float[nrow];
	int ech;
	
	echannels.open("/home/martino/mcalsw/share/mcalfits_conf/canali_KW_2_modified.dat", ifstream::in);
	for (int i=0; i<nrow; i++) {
		echannels >> ech >> elo[i] >> ehi[i];
	}
	
	// for (int i=0; i<nrow; i++) cout << i+1 << "  " << elo[i] << "  " << ehi[i] << endl;
	
	
	// ----------- create histograms ---------------
	
	int lonbin = 36; // 36
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, 0., 360.);
	TH1F *hlon1 = new TH1F("hlon1", "hlon1", lonbin, 0., 360.);		// sub-ms only
	TH1F *hlon2 = new TH1F("hlon2", "hlon2", lonbin, 0., 360.);		// HW only
	TH1F *hlon3 = new TH1F("hlon3", "hlon3", lonbin, 0., 360.);		// 1-ms only
	TH1F *hlon4 = new TH1F("hlon4", "hlon4", lonbin, 0., 360.);		// 16-ms only
	TH1F *hlon5 = new TH1F("hlon5", "hlon5", lonbin, 0., 360.);		// sub-ms AND 1-ms 
	TH1F *hlon6 = new TH1F("hlon6", "hlon6", lonbin, 0., 360.);		// sub-ms AND 16-ms 
	
	int dtbin = 50;	//50
	TH1F *hdt  = new TH1F("hdt", "hdt", dtbin, 0., 6000.);
	TH1F *hdt1  = new TH1F("hdt1", "hdt1", dtbin, 0., 6000.);		// sub-ms only
	TH1F *hdt2  = new TH1F("hdt2", "hdt2", dtbin, 0., 6000.);		// HW only
	TH1F *hdt3  = new TH1F("hdt3", "hdt3", dtbin, 0., 6000.);		// 1-ms only
	TH1F *hdt4  = new TH1F("hdt4", "hdt4", dtbin, 0., 6000.);		// 16-ms only
	TH1F *hdt5  = new TH1F("hdt5", "hdt5", dtbin, 0., 6000.);		// sub-ms AND 1-ms only
	TH1F *hdt6  = new TH1F("hdt6", "hdt6", dtbin, 0., 6000.);		// sub-ms AND 16-ms only
	
	TH2F *hdtlon = new TH2F("hdtlon", "hdtlon", 12, -180., 180., 8, 0., 6000.);	//12,8
	TH1F *hbars  = new TH1F("hbars", "hbars", 30, -0.5, 29.5);
	
	TH1F *ht90 = new TH1F("ht90", "ht90", 64, 0., 1.024);
	TH1F *hcpeak = new TH1F("hcpeak", "hcpeak", 100, 0, 100);
	TH1F *hcounts = new TH1F("hcounts", "hcounts", 1000, 0, 1000);
	TH1F *havgE = new TH1F("havgE", "havgE", 200, 0., 100.);
	
	TH1F *hmint = new TH1F("hmint", "hmint", 10000, -50., 50.);
	TH1F *htgrbini = new TH1F("htgrbini", "htgrbini", 10000, -50., 50.);
	
	// ----------- process entries & fill histograms ---------------
	
	float hr, hr1;
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		
		hmint->Fill(atrg.dt0+atrg.mint);
		// for debugging the "missing pre-burst" issue		
		if (atrg.dt0+atrg.mint > -20.) printf("%06d  %3d  %20.6f  %10.6f\n", atrg.runid, atrg.trg, atrg.t0, atrg.dt0+atrg.mint);
		
		
		htgrbini->Fill(atrg.tgrbini);
		hdtlon->Fill(atrg.lon<=180. ? atrg.lon : atrg.lon-360., atrg.dt);
		
			
		if (atrg.type[0]=='B'  && fabs(atrg.tgrbini)<0.060 && atrg.counts>0  && atrg.subms==0 && atrg.cbkg>0  && atrg.dt>3000.) {
			hlon->Fill(atrg.lon);
			hdt->Fill(atrg.dt);		
			
			ht90->Fill(atrg.t90);
			hcpeak->Fill(atrg.cpeak);
			hcounts->Fill(atrg.counts);
			havgE->Fill(atrg.avgE);
			
			//if (atrg.t90>0.060) cout << atrg.runid << "  " << atrg.trg << "  " << atrg.t90 << "  " << atrg.avgE << endl;
			
			if (atrg.x1) hbars->Fill(0, atrg.x1);
			if (atrg.x2) hbars->Fill(1, atrg.x2);
			if (atrg.x3) hbars->Fill(2, atrg.x3);
			if (atrg.x4) hbars->Fill(3, atrg.x4);
			if (atrg.x5) hbars->Fill(4, atrg.x5);
			if (atrg.x6) hbars->Fill(5, atrg.x6);
			if (atrg.x7) hbars->Fill(6, atrg.x7);
			if (atrg.x8) hbars->Fill(7, atrg.x8);
			if (atrg.x9) hbars->Fill(8, atrg.x9);
			if (atrg.x10) hbars->Fill(9, atrg.x10);
			if (atrg.x11) hbars->Fill(10, atrg.x11);
			if (atrg.x12) hbars->Fill(11, atrg.x12);
			if (atrg.x13) hbars->Fill(12, atrg.x13);
			if (atrg.x14) hbars->Fill(13, atrg.x14);
			if (atrg.x15) hbars->Fill(14, atrg.x15);
			if (atrg.z1) hbars->Fill(15, atrg.z1);
			if (atrg.z2) hbars->Fill(16, atrg.z2);
			if (atrg.z3) hbars->Fill(17, atrg.z3);
			if (atrg.z4) hbars->Fill(18, atrg.z4);
			if (atrg.z5) hbars->Fill(19, atrg.z5);
			if (atrg.z6) hbars->Fill(20, atrg.z6);
			if (atrg.z7) hbars->Fill(21, atrg.z7);
			if (atrg.z8) hbars->Fill(22, atrg.z8);
			if (atrg.z9) hbars->Fill(23, atrg.z9);
			if (atrg.z10) hbars->Fill(24, atrg.z10);
			if (atrg.z11) hbars->Fill(25, atrg.z11);
			if (atrg.z12) hbars->Fill(26, atrg.z12);
			if (atrg.z13) hbars->Fill(27, atrg.z13);
			if (atrg.z14) hbars->Fill(28, atrg.z14);
			if (atrg.z15) hbars->Fill(29, atrg.z15);
		}
		
	/*	if (atrg.lon >=0. && atrg.lon < 60.) { 
			hlon1->Fill(atrg.lon);
			hdt1->Fill(atrg.dt);
		}
		
		if (atrg.lon >=60. && atrg.lon < 120.) { 	
			hlon2->Fill(atrg.lon);
			hdt2->Fill(atrg.dt);
		}
		
		if (atrg.lon >=120. && atrg.lon < 180.) { 
			hlon3->Fill(atrg.lon);
			hdt3->Fill(atrg.dt);
		}
		
		if (atrg.lon >=180. && atrg.lon < 240.) { 	
			hlon4->Fill(atrg.lon);
			hdt4->Fill(atrg.dt);
		}
		
		if (atrg.lon >=240. && atrg.lon < 300.) { 
			hlon5->Fill(atrg.lon);
			hdt5->Fill(atrg.dt);
		}
		
		if (atrg.lon >=300. && atrg.lon < 360.) { 
			hlon6->Fill(atrg.lon);
			hdt6->Fill(atrg.dt);
		}*/
		
		if (process_event_data) {
		
		}

	}
	
	// ----------- graphic section ---------------
	
	TCanvas c1("lon-dt", "lon-dt", 800, 1000);
	c1.Divide(1, 2);
	
	// draw longitude
	
	c1.cd(1);
	//hlon->SetLineColor();
	hlon->GetXaxis()->SetTitle("Longitude (deg)");
	hlon->GetYaxis()->SetTitle("Counts");
	hlon->SetStats(0);
	hlon->Draw();
/*	hlon1->SetLineColor(2);
	hlon1->Draw("SAME");
	hlon2->SetLineColor(3);
	hlon2->Draw("SAME");
	hlon3->SetLineColor(4);
	hlon3->Draw("SAME");
	hlon4->SetLineColor(5);
	hlon4->Draw("SAME");
	hlon5->SetLineColor(6);
	hlon5->Draw("SAME");
	hlon6->SetLineColor(7);
	hlon6->Draw("SAME");
	
	TLegend *leglon = new TLegend(0.7, 0.7, 0.95, 0.95);
	leglon->AddEntry(hlon, "all triggers", "l");
	leglon->AddEntry(hlon1, "sure spike-related", "l");
	leglon->AddEntry(hlon2, "cleaned triggers", "l");
	leglon->AddEntry(hlon3, "1-ms only", "l");
	leglon->AddEntry(hlon4, "16-ms only", "l");
	leglon->AddEntry(hlon5, "sub-ms AND 1-ms only", "l");
	leglon->AddEntry(hlon6, "sub-ms AND 16-ms only", "l");
	leglon->Draw(); */
	
	// draw time since eclipse
	
	c1.cd(2);
	hdt->SetStats(0);
	hdt->GetXaxis()->SetTitle("Time from eclipse (s)");
	hdt->GetYaxis()->SetTitle("Counts");
	hdt->Draw();
	TLegend *legdt = new TLegend(0.7, 0.7, 0.95, 0.95);
	
	/*TH1D *hdttot = hdtlon->ProjectionY("dttot", 1, 13, "");
	hdttot->GetXaxis()->SetTitle("Time from eclipse (s)");
	hdttot->GetYaxis()->SetTitle("Counts");
	hdttot->Draw();
	
	for (int i=0; i<6; i++) {
		TString pname("dt");
		pname += i+1;
		TH1D *aux = hdtlon->ProjectionY(pname.Data(), 2*i+1, 2*i+2, "");
		aux->SetLineColor(2+i);
		aux->Draw("SAME");
		TString slon("Lon. = ");
		slon += i*60;
		slon += " - ";
		slon += (i+1)*60;
		slon += " deg ";
		legdt->AddEntry(aux, slon, "l");
	}

	hdt1->SetLineColor(2);
	hdt1->Draw("SAME");
	hdt2->SetLineColor(3);
	hdt2->Draw("SAME");
	hdt3->SetLineColor(4);
	hdt3->Draw("SAME");
	hdt4->SetLineColor(5);
	hdt4->Draw("SAME");
	hdt5->SetLineColor(6);
	hdt5->Draw("SAME");
	hdt6->SetLineColor(7);
	hdt6->Draw("SAME");
	
	legdt->AddEntry(hdt, "all triggers", "l");
	legdt->AddEntry(hdt1, "sure spike-related", "l");
	legdt->AddEntry(hdt2, "cleaned triggers", "l");
	legdt->AddEntry(hdt3, "1-ms only", "l");
	legdt->AddEntry(hdt4, "16-ms only", "l");
	legdt->AddEntry(hdt5, "sub-ms AND 1-ms only", "l");
	legdt->AddEntry(hdt6, "sub-ms AND 16-ms only", "l"); 
	
	legdt->Draw();*/
	
	TCanvas c2("2dlon-dt", "2dlon-dt", 800, 600);
	c2.cd();
	hdtlon->SetStats(0);
	hdtlon->Draw("COLZ");
	
	TCanvas c3("cbars", "cbars", 800, 600);
	c3.cd();
	hbars->SetTitle("");
	hbars->SetStats(0);
	hbars->Draw();
	
	TCanvas c4("c4", "c4", 800, 600);
	c4.Divide(2, 2);
	c4.cd(1);
	gPad->SetLogx();
	gPad->SetLogy();
	//ht90->SetTitle("Burst duration distribution");
	ht90->SetTitle("");
	ht90->SetStats(0);
	ht90->GetXaxis()->SetTitle("Duration (s)");
	ht90->GetYaxis()->SetTitle("Number of triggers");
	ht90->Draw();
	
	c4.cd(2);
	gPad->SetLogx();
	gPad->SetLogy();
	hcounts->SetTitle("Total number of events distribution");
	hcounts->GetXaxis()->SetTitle("Number of events");
	hcounts->GetYaxis()->SetTitle("Number of triggers");
	hcounts->Draw();
	
	c4.cd(3);
	gPad->SetLogy();
	hcpeak->SetTitle("Peak flux distribution");
	hcpeak->GetXaxis()->SetTitle("Peak flux (counts/100us)");
	hcpeak->GetYaxis()->SetTitle("Number of triggers");
	hcpeak->Draw();
	
	c4.cd(4);
	gPad->SetLogx();
	gPad->SetLogy();
	havgE->SetTitle("Average energy distribution");
	havgE->GetXaxis()->SetTitle("Energy (MeV)");
	havgE->GetYaxis()->SetTitle("Number of triggers");
	havgE->Draw();
	
	TCanvas c5("c5", "c5", 800, 600);
	hmint->Draw();
	htgrbini->SetLineColor(2);
	htgrbini->Draw("SAME");
	
	
	// free memory
	delete elo;
	delete ehi;
}