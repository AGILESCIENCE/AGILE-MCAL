{
	// MM 01/12/08 include anche info sul background, messe fluence e bkg come float
	// MM 18/11/08 valuta la significativita' degli eventi su diversi tempi scala
	
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
			float counts;
			float cpeak;
			float avgE;
			int nEbins;
			float c[4];
			float b[4];
			int x[15];
			int z[15];
			int cub[8];		// spatial zones (cuboni)
	};
	
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
	TTree tvsb;
	Trigger atrg;
	// "/data1/mcal/data_analysis/VSB/analysis_08-10-27_16ms-only_BUS/VSB_global_info_8.dat"
	// "/data1/mcal/data_analysis/VSB/analysis_08-11-25_16ms_E-ge-500keV/VSB_global_info_160.dat"
	// "/data1/mcal/data_analysis/VSB/analysis_08-11-22/VSB_global_info_160.dat"
	 tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_08-11-22_subms_only/VSB_global_info_1.dat",	 "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:type/C:sa/I:lng/I:subms/I:nrm/I:rmid/I:sitid/I:lon/F:lat/F:h/F:dt/F:ntrg/I:mint/F:dt0/F:tgrbini/F:t90/F:tbin/F:cbkg/F:counts/F:cpeak/F:avgE/F:nEbins/I:c0/F:c1/F:c2/F:c3/F:b0/F:b1/F:b2/F:b3/F:x1/I:x2/I:x3/I:x4/I:x5/I:x6/I:x7/I:x8/I:x9/I:x10/I:x11/I:x12/I:x13/I:x14/I:x15/I:z1/I:z2/I:z3/I:z4/I:z5/I:z6/I:z7/I:z8/I:z9/I:z10/I:z11/I:z12/I:z13/I:z14/I:z15/I:cub1/I:cub2/I:cub3/I:cub4/I:cub5/I:cub6/I:cub7/I:cub8/I");
	
	
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
	
	tvsb.SetBranchAddress("x1", &atrg.x[0]);
	tvsb.SetBranchAddress("x2", &atrg.x[1]);
	tvsb.SetBranchAddress("x3", &atrg.x[2]);
	tvsb.SetBranchAddress("x4", &atrg.x[3]);
	tvsb.SetBranchAddress("x5", &atrg.x[4]);
	tvsb.SetBranchAddress("x6", &atrg.x[5]);
	tvsb.SetBranchAddress("x7", &atrg.x[6]);
	tvsb.SetBranchAddress("x8", &atrg.x[7]);
	tvsb.SetBranchAddress("x9", &atrg.x[8]);
	tvsb.SetBranchAddress("x10", &atrg.x[9]);
	tvsb.SetBranchAddress("x11", &atrg.x[10]);
	tvsb.SetBranchAddress("x12", &atrg.x[11]);
	tvsb.SetBranchAddress("x13", &atrg.x[12]);
	tvsb.SetBranchAddress("x14", &atrg.x[13]);
	tvsb.SetBranchAddress("x15", &atrg.x[14]);
	tvsb.SetBranchAddress("z1", &atrg.z[0]);
	tvsb.SetBranchAddress("z2", &atrg.z[1]);
	tvsb.SetBranchAddress("z3", &atrg.z[2]);
	tvsb.SetBranchAddress("z4", &atrg.z[3]);
	tvsb.SetBranchAddress("z5", &atrg.z[4]);
	tvsb.SetBranchAddress("z6", &atrg.z[5]);
	tvsb.SetBranchAddress("z7", &atrg.z[6]);
	tvsb.SetBranchAddress("z8", &atrg.z[7]);
	tvsb.SetBranchAddress("z9", &atrg.z[8]);
	tvsb.SetBranchAddress("z10", &atrg.z[9]);
	tvsb.SetBranchAddress("z11", &atrg.z[10]);
	tvsb.SetBranchAddress("z12", &atrg.z[11]);
	tvsb.SetBranchAddress("z13", &atrg.z[12]);
	tvsb.SetBranchAddress("z14", &atrg.z[13]);
	tvsb.SetBranchAddress("z15", &atrg.z[14]);
	tvsb.SetBranchAddress("cub1", &atrg.cub[0]);
	tvsb.SetBranchAddress("cub2", &atrg.cub[1]);
	tvsb.SetBranchAddress("cub3", &atrg.cub[2]);
	tvsb.SetBranchAddress("cub4", &atrg.cub[3]);
	tvsb.SetBranchAddress("cub5", &atrg.cub[4]);
	tvsb.SetBranchAddress("cub6", &atrg.cub[5]);
	tvsb.SetBranchAddress("cub7", &atrg.cub[6]);
	tvsb.SetBranchAddress("cub8", &atrg.cub[7]);	

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
	
	
	// ----------- create histograms ---------------
	
	int lonbin = 36; // 36
	TH1F *hlontot = new TH1F("hlontot", "hlontot", lonbin, 0., 360.);
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, 0., 360.);
	TH1F *hlonBUS = new TH1F("hlonBUS", "hlonBUS", lonbin, 0., 360.);
	
	int dtbin = 50;	//50
	TH1F *hdttot  = new TH1F("hdttot", "hdttot", dtbin, 0., 6000.);
	TH1F *hdt  = new TH1F("hdt", "hdt", dtbin, 0., 6000.);
	TH1F *hdtBUS  = new TH1F("hdtBUS", "hdtBUS", dtbin, 0., 6000.);
	
	TH2F *hdtlon = new TH2F("hdtlon", "hdtlon", 12, -180., 180., 8, 0., 6000.);	//12,8
	TH3F *hdtlonlat = new TH3F("hdtlonlat", "hdtlonlat", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH3F *hdtlonlatBUS = new TH3F("hdtlonlat", "hdtlonlat", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH1F *hbars  = new TH1F("hbars", "hbars", 30, -0.5, 29.5);
	
	TH1F *ht90 = new TH1F("ht90", "ht90", 64, 0., 1.024);
	TH1F *hcpeak = new TH1F("hcpeak", "hcpeak", 100, 0, 100);
	TH1F *hcounts = new TH1F("hcounts", "hcounts", 1000, 0, 1000);
	TH1F *havgE = new TH1F("havgE", "havgE", 200, 0., 100.);
	TH1F *hhr = new TH1F("hhr", "hhr", 50, 0., 10.);
	
	TH1F *ht90BUS = new TH1F("ht90BUS", "ht90BUS", 64, 0., 1.024);
	TH1F *hcpeakBUS = new TH1F("hcpeakBUS", "hcpeakBUS", 100, 0, 100);
	TH1F *hcountsBUS = new TH1F("hcountsBUS", "hcountsBUS", 1000, 0, 1000);
	TH1F *havgEBUS = new TH1F("havgEBUS", "havgEBUS", 200, 0., 100.);
	TH1F *hbarsBUS  = new TH1F("hbarsBUS", "hbarsBUS", 30, -0.5, 29.5);
	TH1F *hhrBUS = new TH1F("hhrBUS", "hhrBUS", 50, 0., 10.);
	
	TH1F *hmint = new TH1F("hmint", "hmint", 10000, -50., 50.);
	TH1F *htgrbini = new TH1F("htgrbini", "htgrbini", 10000, -50., 50.);
	
	TH1F *hsig = new TH1F("hsig", "hsig", 100, 0., 50.);
	TH1F *hsigBUS = new TH1F("hsigBUS", "hsigBUS", 100, 0., 50.);
	
	TGraph *gGRB = new TGraph(1);
	TGraph *gBUS = new TGraph(1);
	
	TGraph *gcntbarGRB = new TGraph(1);
	TGraph *gcntbarBUS = new TGraph(1);
	
	// ----------- process entries & fill histograms ---------------
	
	float hr, hr1;
	int ncGRB=0;
	int ncBUS=0;
	double t0prev = 0.;
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		// cout << i << endl; &&  atrg.subms==1
		if (atrg.t0 != t0prev && atrg.mint<-1. && (atrg.runid<7302 || atrg.runid>7342) && (atrg.runid<6068 || atrg.runid>6140) ) {
		
			hdtlon->Fill(atrg.lon<=180. ? atrg.lon : atrg.lon-360., atrg.dt);		
			hmint->Fill(atrg.dt0+atrg.mint);
			// for debugging the "missing pre-burst" issue		
			// if (atrg.dt0+atrg.mint > -20.) printf("%06d  %3d  %20.6f  %10.6f\n", atrg.runid, atrg.trg, atrg.t0, atrg.dt0+atrg.mint);
			
			hlontot->Fill(atrg.lon);
			hdttot->Fill(atrg.dt);					
			htgrbini->Fill(atrg.tgrbini);
			
			// main selection: atrg.dt>3000. NB: rimossi trg negli intervalli in cui subms e' disabilitata
			// && (atrg.runid<7302 || atrg.runid>7342) && (atrg.runid<6068 || atrg.runid>6140) &&  atrg.subms==0	 && atrg.cbkg>0  
			if (fabs(atrg.tgrbini)<0.060 && atrg.counts>0 ) {
				
				// calculate numbre of triggered bars
				// hr = ((float)(atrg.c1 + atrg.c2 + atrg.c3)) / atrg.counts;
				hr = (atrg.c1 + atrg.c0)>0 ? ((float) (atrg.c2 + atrg.c3)) / (atrg.c1 + atrg.c0) : 0.;
				int nbars = 0;
				for (int k=0; k<15; k++) if (atrg.x[k]>0) nbars++;
				for (int k=0; k<15; k++) if (atrg.z[k]>0) nbars++;
			
				if (atrg.counts>40) cout << atrg.runid << "  " << atrg.trg << "  " << atrg.counts << "  " << atrg.subms << endl;
				
				// is it flagged as a GRB?
				
				if (atrg.type[0]=='G' ) {
					ht90->Fill(atrg.t90);
					hcpeak->Fill(atrg.cpeak);
					hcounts->Fill(atrg.counts);
					havgE->Fill(atrg.avgE);
					hsig->Fill(atrg.cbkg>0 ? atrg.cpeak/sqrt(atrg.cbkg) : -1.);
					hlon->Fill(atrg.lon);
					hdt->Fill(atrg.dt);	
					hhr->Fill(hr);
					
					hdtlonlat->Fill(atrg.lat, atrg.lon<=180. ? atrg.lon : atrg.lon-360., atrg.dt);
					
					for (int k=0; k<15; k++) if (atrg.x[k]>0) hbars->Fill(k, atrg.x[k]);
					for (int k=0; k<15; k++) if (atrg.z[k]>0) hbars->Fill(k+15, atrg.z[k]);
					
					gGRB->SetPoint(ncGRB, ncGRB, atrg.runid);
					// gcntbarGRB->SetPoint(ncGRB, nbars, atrg.counts);
					// gcntbarGRB->SetPoint(ncGRB, atrg.c1 + atrg.c2 + atrg.c3, atrg.counts);
					gcntbarGRB->SetPoint(ncGRB, nbars, atrg.c2 + atrg.c3);
					ncGRB++;
					
				}
				
				// is it flagged as a BUS event?
				
				if (atrg.type[0]=='B') {
					ht90BUS->Fill(atrg.t90);
					hcpeakBUS->Fill(atrg.cpeak);
					hcountsBUS->Fill(atrg.counts);
					havgEBUS->Fill(atrg.avgE);
					hsigBUS->Fill(atrg.cbkg>0 ? atrg.cpeak/sqrt(atrg.cbkg) : -1.);
					hlonBUS->Fill(atrg.lon);
					hdtBUS->Fill(atrg.dt);					
					hhrBUS->Fill(hr);
					
					hdtlonlatBUS->Fill(atrg.lat, atrg.lon<=180. ? atrg.lon : atrg.lon-360., atrg.dt);
					
					for (int k=0; k<15; k++) if (atrg.x[k]>0) hbarsBUS->Fill(k, atrg.x[k]);
					for (int k=0; k<15; k++) if (atrg.z[k]>0) hbarsBUS->Fill(k+15, atrg.z[k]);
					
					gBUS->SetPoint(ncBUS, ncBUS, atrg.runid);
					// gcntbarBUS->SetPoint(ncBUS, nbars, atrg.counts);
					// gcntbarBUS->SetPoint(ncBUS, atrg.c1 + atrg.c2 + atrg.c3, atrg.counts);
					gcntbarBUS->SetPoint(ncBUS, nbars, atrg.c2 + atrg.c3);
					ncBUS++;
				}
			}
		}
		t0prev = atrg.t0;
		

		
		if (process_event_data) {
		
		}

	}
	
	// normalize histo
	
	int nGRB = ht90->GetEntries();
	cout << "Number of selected GRB events: " << nGRB << "  " << ncGRB << endl;
	ht90->Scale(1./nGRB);
	hcpeak->Scale(1./nGRB);
	hcounts->Scale(1./nGRB);
	havgE->Scale(1./nGRB);
	hbars->Scale(1./nGRB);
	hsig->Scale(1./nGRB);
	hhr->Scale(1./nGRB);
	
	int nBUS = ht90BUS->GetEntries();
	cout << "Number of selected BUS events: " << nBUS << endl;
	ht90BUS->Scale(1./nBUS);
	hcpeakBUS->Scale(1./nBUS);
	hcountsBUS->Scale(1./nBUS);
	havgEBUS->Scale(1./nBUS);
	hbarsBUS->Scale(1./nBUS);
	hsigBUS->Scale(1./nBUS);
	hhrBUS->Scale(1./nBUS);	
	
	// ----------- graphic section ---------------
	
	TCanvas c1("lon-dt", "lon-dt", 800, 1000);
	c1.Divide(1, 2);
	
	// draw longitude
	
	c1.cd(1);
	//hlon->SetLineColor();
	hlontot->SetLineColor(3);
	hlontot->GetXaxis()->SetTitle("Longitude (deg)");
	hlontot->GetYaxis()->SetTitle("Counts");
	hlontot->SetStats(0);
	hlontot->Draw();
	hlon->Draw("SAME");
	hlonBUS->SetLineColor(2);
	hlonBUS->Draw("SAME");
/*	
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
	hdttot->SetLineColor(3);
	hdttot->SetStats(0);
	hdttot->GetXaxis()->SetTitle("Time from eclipse (s)");
	hdttot->GetYaxis()->SetTitle("Counts");
	hdttot->Draw();
	hdt->Draw("SAME");
	hdtBUS->SetLineColor(2);
	hdtBUS->Draw("SAME");
	
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
	hbars->GetXaxis()->SetTitle("MCAL bar address");
	hbars->GetYaxis()->SetTitle("Normalized counts");
	hbars->Draw();
	hbarsBUS->SetLineColor(2);
	hbarsBUS->Draw("SAME");
	
	TCanvas c4("c4", "c4", 800, 600);
	c4.Divide(3, 2);
	c4.cd(1);
	gPad->SetLogx();
	gPad->SetLogy();
	ht90->SetTitle("Burst duration distribution");
	//ht90->SetTitle("");
	ht90->SetStats(0);
	ht90->GetXaxis()->SetTitle("Duration (s)");
	ht90->GetYaxis()->SetTitle("Fraction of triggers");
	ht90->Draw();
	ht90BUS->SetLineColor(2);
	ht90BUS->Draw("SAME");
	
	c4.cd(2);
	gPad->SetLogx();
	gPad->SetLogy();
	hcounts->SetTitle("Total number of events distribution");
	hcounts->SetStats(0);
	hcounts->GetXaxis()->SetTitle("Number of events");
	hcounts->GetYaxis()->SetTitle("Fraction of triggers");
	hcounts->Draw();
	hcountsBUS->SetLineColor(2);
	hcountsBUS->Draw("SAME");
	
	c4.cd(3);
	gPad->SetLogy();
	hcpeak->SetTitle("Peak flux distribution");
	hcpeak->SetStats(0);
	hcpeak->GetXaxis()->SetTitle("Peak flux (counts/bin)");
	hcpeak->GetYaxis()->SetTitle("Fraction of triggers");
	hcpeak->Draw();
	hcpeakBUS->SetLineColor(2);
	hcpeakBUS->Draw("SAME");
	
	c4.cd(4);
	gPad->SetLogx();
	gPad->SetLogy();
	havgE->SetTitle("Average energy distribution");
	havgE->SetStats(0);
	havgE->GetXaxis()->SetTitle("Energy (MeV)");
	havgE->GetYaxis()->SetTitle("Fraction of triggers");
	havgE->Draw();
	havgEBUS->SetLineColor(2);
	havgEBUS->Draw("SAME");
	
	c4.cd(5);
	hhr->SetTitle("Hardness ratio");
	hhr->SetStats(0);
	hhr->GetXaxis()->SetTitle("HR");
	hhr->GetYaxis()->SetTitle("Fraction of triggers");
	hhr->Draw();
	hhrBUS->SetLineColor(2);
	hhrBUS->Draw("SAME");
	
	TCanvas c5("c5", "c5", 800, 600);
	hmint->Draw();
	htgrbini->SetLineColor(2);
	htgrbini->Draw("SAME");
	
		
	TCanvas c5("csig", "csig", 800, 600);
	c5.cd();
	hsig->SetTitle("Significance histogram");
	hsig->SetStats(0);
	hsig->GetXaxis()->SetTitle("Significance (sigma)");
	hsig->GetYaxis()->SetTitle("Normalized counts");
	hsig->Draw();
	hsigBUS->SetLineColor(2);
	hsigBUS->Draw("SAME");
		
	TCanvas c6("c6", "c6", 800, 600);
	c6.cd();
	gBUS->SetMarkerColor(2);
	if (ncBUS>ncGRB) {
		gBUS->Draw("AP");
		gBUS->GetHistogram()->GetXaxis()->SetTitle("n. of trigger");
		gBUS->GetHistogram()->GetYaxis()->SetTitle("Orbit number");
		gGRB->Draw("PSAME");
	} else {
		gGRB->Draw("AP");
		gGRB->GetHistogram()->GetXaxis()->SetTitle("n. of trigger");
		gGRB->GetHistogram()->GetYaxis()->SetTitle("Orbit number");
		gBUS->Draw("PSAME");
	}
		
	TCanvas c7("c7", "c7", 800, 600);
	c7.cd();
	gcntbarGRB->SetMarkerStyle(2);
	gcntbarGRB->Draw("AP");
	gcntbarBUS->SetMarkerStyle(4);
	gcntbarBUS->SetMarkerColor(2);
	gcntbarBUS->Draw("PSAME");
		
	TCanvas c8("c8", "c8", 800, 600);
	c8->Divide(3,2);
	
	c8->cd(1);
	TH2F *zy = hdtlonlat->Project3D("zy");
	zy->SetStats(0);
	zy->SetTitle("GRB trg: Dt vs. Lon");
	zy->GetXaxis()->SetTitle("Lon");
	zy->GetXaxis()->CenterTitle();
	zy->GetYaxis()->SetTitle("Dt");
	zy->GetYaxis()->CenterTitle();
	zy->GetYaxis()->SetTitleOffset(2.);
	zy->DrawClone("LEGO2Z");
	
	c8->cd(2);
	TH2F *xy = hdtlonlat->Project3D("xy");
	xy->SetStats(0);
	xy->SetTitle("GRB trg: Lat vs. Lon");
	xy->GetXaxis()->SetTitle("Lon");
	xy->GetXaxis()->CenterTitle();
	xy->GetYaxis()->SetTitle("Lat");
	xy->GetYaxis()->CenterTitle();
	xy->DrawClone("LEGO2Z");
	
	c8->cd(3);
	TH2F *zx = hdtlonlat->Project3D("zx");
	zx->SetStats(0);
	zx->SetTitle("GRB trg: Dt vs. Lat");
	zx->GetXaxis()->SetTitle("Lat");
	zx->GetXaxis()->CenterTitle();
	zx->GetYaxis()->SetTitle("Dt");
	zx->GetYaxis()->CenterTitle();
	zx->GetYaxis()->SetTitleOffset(2.);
	zx->DrawClone("LEGO2Z");	
	
	c8->cd(4);
	TH2F *zyBUS = hdtlonlatBUS->Project3D("zy");
	zyBUS->SetStats(0);
	zyBUS->SetTitle("BUS trg: Dt vs. Lon");
	zyBUS->GetXaxis()->SetTitle("Lon");
	zyBUS->GetXaxis()->CenterTitle();
	zyBUS->GetYaxis()->SetTitle("Dt");
	zyBUS->GetYaxis()->CenterTitle();
	zyBUS->GetYaxis()->SetTitleOffset(2.);
	zyBUS->DrawClone("LEGO2Z");
	
	c8->cd(5);
	TH2F *xyBUS = hdtlonlatBUS->Project3D("xy");
	xyBUS->SetStats(0);
	xyBUS->SetTitle("BUS trg: Lat vs. Lon");
	xyBUS->GetXaxis()->SetTitle("Lon");
	xyBUS->GetXaxis()->CenterTitle();
	xyBUS->GetYaxis()->SetTitle("Lat");
	xyBUS->GetYaxis()->CenterTitle();
	xyBUS->DrawClone("LEGO2Z");
	
	c8->cd(6);
	TH2F *zxBUS = hdtlonlatBUS->Project3D("zx");
	zxBUS->SetStats(0);
	zxBUS->SetTitle("BUS trg: Dt vs. Lat");
	zxBUS->GetXaxis()->SetTitle("Lat");
	zxBUS->GetXaxis()->CenterTitle();
	zxBUS->GetYaxis()->SetTitle("Dt");
	zxBUS->GetYaxis()->CenterTitle();
	zxBUS->GetYaxis()->SetTitleOffset(2.);
	zxBUS->DrawClone("LEGO2Z");
	
	
	// free memory
	delete elo;
	delete ehi;
}