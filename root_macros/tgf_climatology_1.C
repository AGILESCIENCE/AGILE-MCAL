{
	// MM 23/04/10 exclude SAA from LIS histo normalization
	// MM 05/03/10 based on tgf_candidates_analysis_6.C
	
	gROOT->Reset();
  	// gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
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
	gROOT->SetStyle("mcalstyle");
	
	int process_event_data = 1;
	int getCoordinates = 0;

	struct Trigger  {
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
			int rmid;
			int sitid;
			int ber;
			int bet;
			float lon;
			float lat;
			float h;
			float dt;
			int ntrg;	// number of reconstructed sub-ms triggers
			float mint;	// minimum time 
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
	
	TTree tvsb;
	Trigger atrg;
	 // tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_09-03-16/VSB_global_info_10.dat",	 // dataset 16/3
	 // /data1/mcal/data_analysis/VSB/tgf_candidates_Jun08-Mar09/green_candidates_COMPLETE_2.txt     // dataset Jun08-Mar09 paper JGR
	 tvsb.ReadFile("/data1/mcal/data_analysis/VSB/tgf_candidates_complete/green_candidates_ALL.txt",	 "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:mint3908/F:maxt3908/F:type/C:sa/I:lng/I:subms/I:flg01ms/I:flg16ms/I:nrm/I:rmid/I:sitid/I:ber/I:bet/I:lon/F:lat/F:h/F:dt/F:ntrg/I:mint/F:dt0/F:tgrbini/F:t90/F:tbin/F:cbkg/F:counts/F:cpeak/F:avgE/F:nEbins/I:c0/F:c1/F:c2/F:c3/F:b0/F:b1/F:b2/F:b3/F:x1/I:x2/I:x3/I:x4/I:x5/I:x6/I:x7/I:x8/I:x9/I:x10/I:x11/I:x12/I:x13/I:x14/I:x15/I:z1/I:z2/I:z3/I:z4/I:z5/I:z6/I:z7/I:z8/I:z9/I:z10/I:z11/I:z12/I:z13/I:z14/I:z15/I:cub1/I:cub2/I:cub3/I:cub4/I:cub5/I:cub6/I:cub7/I:cub8/I");
	
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("mint3908", &atrg.mint3908);
	tvsb.SetBranchAddress("maxt3908", &atrg.maxt3908);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("flg01ms", &atrg.flg01ms);
	tvsb.SetBranchAddress("flg16ms", &atrg.flg16ms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("ber", &atrg.ber);
	tvsb.SetBranchAddress("bet", &atrg.bet);
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
	tvsb.SetBranchAddress("c0", &atrg.c[0]);
	tvsb.SetBranchAddress("c1", &atrg.c[1]);
	tvsb.SetBranchAddress("c2", &atrg.c[2]);
	tvsb.SetBranchAddress("c3", &atrg.c[3]);
	tvsb.SetBranchAddress("b0", &atrg.b[0]);
	tvsb.SetBranchAddress("b1", &atrg.b[1]);
	tvsb.SetBranchAddress("b2", &atrg.b[2]);
	tvsb.SetBranchAddress("b3", &atrg.b[3]);
	
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
	
	int lonbin = 144; // 12 o 36 o 72 o 144
	TH1F *hlontot = new TH1F("hlontot", "hlontot", lonbin, -180., 180.);
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, -180., 180.);
	TH1F *hlonBUS = new TH1F("hlonBUS", "hlonBUS", lonbin, -180., 180.);
	TH1F *hlonRH = new TH1F("hlonRH", "hlonRH", lonbin, -180., 180.);
	
	int dtbin = 12;	//50
	TH1F *hdttot  = new TH1F("hdttot", "hdttot", dtbin, 0., 6000.);
	TH1F *hdt  = new TH1F("hdt", "hdt", dtbin, 0., 6000.);
	TH1F *hdtBUS  = new TH1F("hdtBUS", "hdtBUS", dtbin, 0., 6000.);
	
	int lcbin = 12; // 36
	TH1F *hlc  = new TH1F("hlc", "hlc", lcbin, 0., 24.);
	TH1F *hlcRH  = new TH1F("hlcRH", "hlcRH", lcbin, 0., 24.);
	
	TH1F *hm  = new TH1F("hm", "hm", 12, 1., 13.);	// month distributions
	TH1F *hmRH  = new TH1F("hmRH", "hmRH", 12, 1., 13.);
	
	TH2F *hdtlon = new TH2F("hdtlon", "hdtlon", 12, -180., 180., 8, 0., 6000.);	//12,8
	TH3F *hdtlonlat = new TH3F("hdtlonlat", "hdtlonlat", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH3F *hdtlonlatBUS = new TH3F("hdtlonlatBUS", "hdtlonlatBUS", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH1F *hbars  = new TH1F("hbars", "hbars", 30, -0.5, 29.5);
	// TH3F *hbarfluhr = new TH3F("hbarfluhr", "hbarfluhr", 30, -0.5, 29.5, 
	
	// c0+c1 , c2+c3 , nbars
	TH3F *topology = new TH3F("topology", "topology", 100, 0., 100.,  100, 0., 100., 30, 0.5, 30.5);
	TH3F *topologyBUS = new TH3F("topologyBUS", "topologyBUS", 100, 0., 100.,  100, 0., 100., 30, 0.5, 30.5);
	
	TH1F *ht90 = new TH1F("ht90", "ht90", 200, 0., 0.1);	//200
	TH1F *hcpeak = new TH1F("hcpeak", "hcpeak", 100, 0, 100);
	TH1F *hcounts = new TH1F("hcounts", "hcounts", 500, 0, 1000);
	TH1F *havgE = new TH1F("havgE", "havgE", 200, 0., 100.);
	TH1F *hhr = new TH1F("hhr", "hhr", 50, 0., 10.);
	
	TH1F *ht90BUS = new TH1F("ht90BUS", "ht90BUS", 2046, 0., 1.024);
	TH1F *hcpeakBUS = new TH1F("hcpeakBUS", "hcpeakBUS", 100, 0, 100);
	TH1F *hcountsBUS = new TH1F("hcountsBUS", "hcountsBUS", 1000, 0, 1000);
	TH1F *havgEBUS = new TH1F("havgEBUS", "havgEBUS", 200, 0., 100.);
	TH1F *hbarsBUS  = new TH1F("hbarsBUS", "hbarsBUS", 30, -0.5, 29.5);
	TH1F *hhrBUS = new TH1F("hhrBUS", "hhrBUS", 1000, 0., 10.);
	
	TH1F *hmint = new TH1F("hmint", "hmint", 10000, -50., 50.);
	TH1F *htgrbini = new TH1F("htgrbini", "htgrbini", 10000, -50., 50.);
	
	TH1F *hsig = new TH1F("hsig", "hsig", 100, 0., 50.);
	TH1F *hsigBUS = new TH1F("hsigBUS", "hsigBUS", 100, 0., 50.);
	
	TGraph *gGRB = new TGraph(1);
	TGraph *gBUS = new TGraph(1);
	
	TGraph *gcntbarGRB = new TGraph(1);
	TGraph *gcntbarBUS = new TGraph(1);
	
	TH1F *hmonth = new TH1F("hmonth", "hmonth", 16, 0., 48.);
	TH1F *hmonthRH = new TH1F("hmonthRH", "hmonthRH", 16, 0., 48.);

	// load LIS-OTD global lightning distributions
		
	// TFile flis("/data/mcal/LIS-OTD/flis.root");
	// TH2F *hrfc = (TH2F *) flis.Get("hrfc_com_fr");
	ifstream fin("/data/mcal/LIS-OTD/HRFC_COM_FR.txt");
	double p;
	TH2F hrfc("hrfc_com_fr", "hrfc_com_fr", 720, -180., 180., 360, -90., 90.);
	for (int i=0; i<360; i++) {
		for (int j=0; j<720; j++) {
			fin >> p;
			hrfc.SetBinContent(j, i, p);
		}
	}
	TH2F *hrfc2 = hrfc.Clone();
	TH2F *hrfc3 = hrfc.Clone();
			
	// canvas for geographical distribution
	TCanvas *c11 = new TCanvas("c11", "c11", 1000,500);	//1000,300
	c11->Divide(1,2);
	c11->cd(1);
	// TH2F *htmp = new TH2F("htmp", "htmp", 360, -180.5, 179.5, 20, -10.5, 9.5); // 60, -3., 3.
	// TH2F *htmp2 = new TH2F("htmp2", "htmp2", 100, -20., 80., 80, -40., 40.); // 60, -3., 3.
	// TH2F *htmp3 = new TH2F("htmp3", "htmp3", 100, 70., 170., 80, -40., 40.); // 60, -3., 3.
	TH2F *htmp = new TH2F("htmp", "htmp", 360, -180.5, 179.5, 20, -10.5, 9.5); // 60, -3., 3.
	TH2F *htmp2 = new TH2F("htmp2", "htmp2", 100, -20.5, 79.5, 80, -40.5, 39.5); // 60, -3., 3.
	TH2F *htmp3 = new TH2F("htmp3", "htmp3", 100, 69.5, 169.5, 80, -40.5, 39.5); // 60, -3., 3.
	
	// fill with Earth contour (from example earth.C)
	ifstream in;
	in.open("earth.dat");
	Float_t x,y;
	while (1) {
	in >> x >> y;
	if (!in.good()) break;
		htmp->Fill(x,y, 1);
		htmp2->Fill(x,y, 1);
		htmp3->Fill(x,y, 1);
	}
	in.close();
	
	
	hrfc->SetStats(0);
	hrfc->SetTitle("");
	hrfc->GetXaxis()->SetTitle("Longitude (deg)");
	hrfc->GetXaxis()->CenterTitle();
	hrfc->GetXaxis()->SetTitleOffset(0.8);
	hrfc->GetYaxis()->SetTitle("Latitude (deg)");
	hrfc->GetYaxis()->CenterTitle();
	hrfc->GetYaxis()->SetRangeUser(-10., 10.);
	hrfc->GetYaxis()->SetTitleOffset(0.8);
	// hrfc->SetContour(2);
	hrfc->Draw("COLZ");
	htmp->Draw("cont3SAME"); // mercator
	
	TLine l1(-180., 2.5, 180., 2.5);
	l1.SetLineStyle(2);
	l1.Draw();
	TLine l2(-180., -2.5, 180., -2.5);
	l2.SetLineStyle(2);
	l2.Draw();
	
	// zoom on Africa
	TCanvas *c14 = new TCanvas("c14", "c14", 600,600);
	c14->SetTicks(1,1);
	c14->SetLogz();
	hrfc2->GetYaxis()->SetRangeUser(-40., 40.);
	hrfc2->GetXaxis()->SetRangeUser(-20., 80.);
	hrfc2->SetStats(0);
	hrfc2->SetTitle("");
	hrfc2->GetXaxis()->SetTitle("Longitude (deg)");
	hrfc2->GetXaxis()->SetLabelSize(0.04);
	hrfc2->GetYaxis()->SetLabelSize(0.04);
	hrfc2->GetZaxis()->SetLabelSize(0.04);
	hrfc2->GetXaxis()->SetTitleSize(0.04);
	hrfc2->GetYaxis()->SetTitleSize(0.04);
	hrfc2->GetZaxis()->SetTitleSize(0.04);
	hrfc2->GetXaxis()->CenterTitle();
	hrfc2->GetYaxis()->CenterTitle();
	hrfc2->GetXaxis()->SetTitleOffset(0.8);
	hrfc2->GetYaxis()->SetTitle("Latitude (deg)");
	hrfc2->Draw("COLZ");
	
	htmp2->SetStats(0);
	htmp2->SetTitle("");
	htmp2->SetContour(2);
	htmp2->Draw("cont3SAME"); // mercator
	htmp2->GetXaxis()->SetLabelSize(0.04);
	htmp2->GetYaxis()->SetLabelSize(0.04);
	htmp2->GetXaxis()->SetLabelColor(10);
	htmp2->GetYaxis()->SetLabelColor(10);
	
	TLine l3(0., -40., 0., 40.);
	TLine l4(30., -40., 30., 40.);
	TLine l5(60., -40., 60., 40.);
	TLine l6(-20., 0., 80., 0.);
	TLine l7(-20., -30., 80., -30.);
	TLine l8(-20., 30., 80., 30.);
	l3.SetLineStyle(2);
	l4.SetLineStyle(2);
	l5.SetLineStyle(2);
	l6.SetLineStyle(2);
	l7.SetLineStyle(2);
	l8.SetLineStyle(2);
	l3.Draw();
	l4.Draw();
	l5.Draw();
	l6.Draw();
	l7.Draw();
	l8.Draw();

	
	// zoom on SouthEast Asia
	TCanvas *c15 = new TCanvas("c15", "c15", 600,600);
	c15->SetTicks(1,1);
	c15->SetLogz();
	hrfc3->GetYaxis()->SetRangeUser(-40., 40.);
	hrfc3->GetXaxis()->SetRangeUser(70., 170.);
	hrfc3->SetStats(0);
	hrfc3->SetTitle("");
	hrfc3->GetXaxis()->SetTitle("Longitude (deg)");
	hrfc3->GetXaxis()->SetLabelSize(0.04);
	hrfc3->GetYaxis()->SetLabelSize(0.04);
	hrfc3->GetZaxis()->SetLabelSize(0.04);
	hrfc3->GetXaxis()->SetTitleSize(0.04);
	hrfc3->GetYaxis()->SetTitleSize(0.04);
	hrfc3->GetZaxis()->SetTitleSize(0.04);
	hrfc3->GetXaxis()->CenterTitle();
	hrfc3->GetYaxis()->CenterTitle();
	hrfc3->GetXaxis()->SetTitleOffset(0.8);
	hrfc3->GetYaxis()->SetTitle("Latitude (deg)");
	hrfc3->Draw("COLZ");
	
	htmp3->SetStats(0);
	htmp3->SetTitle("");
	htmp3->SetContour(2);
	htmp3->Draw("cont3SAME"); // mercator
	htmp3->GetXaxis()->SetLabelSize(0.04);
	htmp3->GetYaxis()->SetLabelSize(0.04);
	htmp3->GetXaxis()->SetLabelColor(10);
	htmp3->GetYaxis()->SetLabelColor(10);
	
	TLine l9(90., -40., 90., 40.);
	TLine l10(120., -40., 120., 40.);
	TLine l11(150., -40., 150., 40.);
	TLine l12(70., 0., 170., 0.);
	TLine l13(70., -30., 170., -30.);
	TLine l14(70., 30., 170., 30.);
	l9.SetLineStyle(2);
	l10.SetLineStyle(2);
	l11.SetLineStyle(2);
	l12.SetLineStyle(2);
	l13.SetLineStyle(2);
	l14.SetLineStyle(2);
	l9.Draw();
	l10.Draw();
	l11.Draw();
	l12.Draw();
	l13.Draw();
	l14.Draw();
		
	// read RHESSI catalog data and fill histo
	
	RHESSI_tgf rtgf[1000];
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

		if (rtgf[nrh].lat<=2.5 && rtgf[nrh].lat>=-2.5 ) {	// && (mm<=3 || mm>=6)
			hlonRH->Fill(rtgf[nrh].lon);
			hlcRH->Fill(rtgf[nrh].loctime);
			if (mm != 2 ) hmRH->Fill(mm);
			hmonthRH->Fill((yy-2)*12 + mm -1);
			cout << nsel << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << "  " << mm << endl;
			nsel++;
		}
		
		nrh++;
	}
	
	// ----------- process entries & fill histograms ---------------
	
	float hr, hr1;	// hardness ratio
	float lc; 	// local time
	int ncGRB=0;
	int ncBUS=0;
	double t0prev = 0.;
	double coor[3];
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		
		// calculate local time
		int hh, mm, ss;
		sscanf(atrg.time, "%2d:%2d:%2d", &hh, &mm, &ss);
		lc = fmod(hh + mm/60. + ss/3600. + atrg.lon*0.06667, 24.);
		hlc->Fill(lc);
		
		// calculate month of the year
		int year, month, day;
		sscanf(atrg.date, "%4d-%2d-%2d", &year, &month, &day);
		if ((year == 2009 && month>=3) || year > 2009) hm->Fill(month);
		hmonth->Fill((year-2008)*12 + month -1);
		
		if(atrg.lon>180.) atrg.lon = atrg.lon-360.;	// longitude correction				
		hlontot->Fill(atrg.lon);
		hdttot->Fill(atrg.dt);					
		htgrbini->Fill(atrg.tgrbini);
		
		hr = (atrg.c[1] + atrg.c[0])>0 ? ((float) (atrg.c[2] + atrg.c[3])) / (atrg.c[1] + atrg.c[0]) : 100.;
		
		// draw a marker in the geographical distribution
		TMarker *m = new TMarker(atrg.lon, atrg.lat, 5); //20, 8
		
		// scale dimension according to fluence
		float dim =  0.08*atrg.counts;  //0.1
		// m->SetMarkerSize(dim);
		
		// scale color according to HR
		int color;
		
		// BW version
	/*	if (hr < 1.) color = 1;
		else if (hr < 2.) color = 15;
		else if (hr < 3.) color = 16;
		else if (hr < 4.) color = 17;
		else color = 18;
	*/		
		// color version
		if (hr < 0.5) color = 1;
		else if (hr < 1.) color = 2;
		else if (hr < 2.) color = 5;
		else if (hr < 3.) color = 3;
		else if (hr < 4.) color = 4;
		else color = 6;
		
		// m->SetMarkerColor(color);
		c11->cd(1);
		m->Draw();
		c14->cd();
		m->Draw();
		c15->cd();
		m->Draw();
		
		// -------------------------------------
			
		// calculate number of triggered bars
		int nbars = 0;
		for (int k=0; k<15; k++) if (atrg.x[k]>0) nbars++;
		for (int k=0; k<15; k++) if (atrg.z[k]>0) nbars++;
			
		
		if (1) { // year>=2009 && month>=3
			
			ht90->Fill(atrg.t90);
			hcpeak->Fill(atrg.cpeak + atrg.cbkg);
			hcounts->Fill(atrg.counts + atrg.cbkg);
			havgE->Fill(atrg.avgE);
			hsig->Fill(atrg.cbkg>0 ? atrg.cpeak/sqrt(atrg.cbkg) : -1.);
			hlon->Fill(atrg.lon);
			hdt->Fill(atrg.dt);	
			hhr->Fill(hr);
			
			hdtlonlat->Fill(atrg.lat, atrg.lon, atrg.dt);
			hdtlon->Fill(atrg.lon, atrg.dt);		
			
			for (int k=0; k<15; k++) if (atrg.x[k]>0) hbars->Fill(k, atrg.x[k]);
			for (int k=0; k<15; k++) if (atrg.z[k]>0) hbars->Fill(k+15, atrg.z[k]);
			
			gGRB->SetPoint(ncGRB, ncGRB, atrg.runid);
			gcntbarGRB->SetPoint(ncGRB, nbars, atrg.counts);
			topology->Fill(atrg.c[0] + atrg.c[1], atrg.c[2] + atrg.c[3], nbars);
			ncGRB++;
			
			// output results
			if (0) printf("%6d-%d \t%s \t%s \t%6.2f \t%6.2f \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%8.6f \t%8.6f \t%6.2f \t%6.2f\n", atrg.runid, atrg.trg, atrg.date, atrg.time, atrg.counts, atrg.cpeak, atrg.subms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t90, lc, atrg.avgE, hr);
			
			// format for makespectra (use the t90 as time interval)
			if (0) {
				//printf("%6d \t%20.6f \t%8.6f\n", atrg.runid, atrg.t0, atrg.t90, atrg.dt0, atrg.tgrbini);
				double peakdt = (fmod(atrg.t0,1.) + atrg.tgrbini);
				if (peakdt>=0. && peakdt<1.) {
					printf("%06d \t%20.6f \t%.6f\n",atrg.runid, atrg.t0 + atrg.tgrbini, atrg.t90);
				} else cout << "check trigger " << atrg.runid << "-" << atrg.trg << endl;
			}
			
			// format for makespectra (use [t0-5ms .. t0+5ms] as time interval)
			if (0) printf("%6d \t%3d \t%6.2f \t%6.2f \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%20.6f \t%8.6f \t%8.6f \t%8.6f\n", atrg.runid, atrg.trg, atrg.counts + atrg.cbkg, atrg.cpeak, atrg.subms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t0, 0.010, atrg.dt0, atrg.tgrbini - 0.005);
			
			// format for sferics correlation
			if (0) {
				double peakdt = (fmod(atrg.t0,1.) + atrg.tgrbini);
				if (peakdt>=0. && peakdt<1.) {
					printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f \t%10.0f \t%10.0f \t%10.2f \t%20.6f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (peakdt*1.E6), atrg.lon, atrg.lat, atrg.cpeak, atrg.counts, hr, atrg.t0 + atrg.tgrbini);
				} else cout << "check trigger " << atrg.runid << "-" << atrg.trg << endl;
				
			}
			
			// format for checking with Fullekrug results
			if (0) printf("%s \t%s \t%6d \t%3d \t%6.2f \t%6.2f \t%3d \t%3d \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%20.6f \t%8.6f\n", atrg.date, atrg.time, atrg.runid, atrg.trg, atrg.counts + atrg.cbkg, atrg.cpeak, atrg.subms, atrg.flg01ms, atrg.flg16ms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t0, atrg.t90);

		}

	}
	
	// normalize histo
	
	int nGRB = ht90->GetEntries();
	cout << "Number of selected GRB events: " << nGRB << "  " << ncGRB << endl;
/*	ht90->Scale(1./nGRB);
	hcpeak->Scale(1./nGRB);
	hcounts->Scale(1./nGRB);
	havgE->Scale(1./nGRB);
	hbars->Scale(1./nGRB);
	hsig->Scale(1./nGRB);
	hhr->Scale(1./nGRB);*/
	
	// normalize MCAL and RHESSI histo
	TH1F *hlonMCAL = (TH1F *) hlon->Clone();
	hlonMCAL->Sumw2();
	hlc->Sumw2();
	hlonMCAL->Scale(1./nGRB);
	hlc->Scale(1./nGRB);
	
	int nhm = hm->GetEntries();
	cout << "hm: entries = " << nhm << endl;	
	hm->Sumw2();
	hm->Scale(1./nhm);
	
	hlcRH->Sumw2();
	hlonRH->Sumw2();
	hlonRH->Scale(1./nsel);
	hlcRH->Scale(1./nsel);
	
	int nhmRH = hmRH->GetEntries();
	cout << "hmRH: entries = " << nhmRH << endl;
	hmRH->Sumw2();
	hmRH->Scale(1./nhmRH);
	
	
	// ----------- graphic section ---------------
	c11->cd(2);
	
	hlon->Sumw2();
	hlon->Scale(1./nentries);
	
	hlon->SetTitle("");
	hlon->SetStats(0);
	hlon->GetXaxis()->SetTitle("Longitude (deg)");
	hlon->GetXaxis()->CenterTitle();
	hlon->GetXaxis()->SetTitleOffset(0.8);
	hlon->GetYaxis()->SetTitle("Fraction of triggers");
	hlon->GetYaxis()->CenterTitle();
	hlon->GetYaxis()->SetTitleOffset(0.8);
	hlon->Draw();

	TH1D *h1 = hrfc->ProjectionX("", 175, 184); 	// +/- 2.5 deg sull'equatore
//	TH1D *h1 = hrfc->ProjectionX("", 170, 189); 	// +/- 5 deg sull'equatore
	double ltot = h1->Integral();
	double lsaa = h1->Integral(240, 320);		// approssimazione della SAA: [-60, -20] deg
	cout << "LIS projection: total integral = " << ltot << "  SAA integral = " << lsaa << endl;
	ltot -= lsaa;
	
	h1->Scale(1./ltot);
	h1->SetLineColor(2);
	h1->Draw("same");


/*	TCanvas ccomp("ccomp", "ccomp", 800, 800);
	ccomp.Divide(1, 2);
	ccomp.SetTicks(1,1);
	
	ccomp.cd(1);
	gPad->SetTicks(1,1);
	hlonMCAL->SetTitle("");
	hlonMCAL->GetXaxis()->SetLabelSize(0.06);
	hlonMCAL->GetXaxis()->SetTitleSize(0.06);
	hlonMCAL->GetXaxis()->SetTitleOffset(0.8);
	hlonMCAL->GetXaxis()->CenterTitle();
	hlonMCAL->GetYaxis()->SetLabelSize(0.06);
	hlonMCAL->GetYaxis()->SetTitleSize(0.06);
	hlonMCAL->GetYaxis()->SetTitleOffset(0.8);
	hlonMCAL->GetYaxis()->CenterTitle();
	hlonMCAL->GetXaxis()->SetTitle("Longitude (deg)");
	hlonMCAL->GetYaxis()->SetTitle("Normalized counts");
	hlonMCAL->SetStats(0);
	hlonMCAL->SetMarkerStyle(22);
	hlonMCAL->SetMarkerSize(1.2);
	hlonMCAL->Draw("E1P");
	// hlonRH->SetLineColor(2);
	hlonRH->SetMarkerStyle(25);
	hlonRH->Draw("E1PSAME");
	
	ccomp.cd(2);
	gPad->SetTicks(1,1);
	hlc->SetTitle("");
	hlc->GetXaxis()->SetLabelSize(0.06);
	hlc->GetXaxis()->SetTitleSize(0.06);
	hlc->GetXaxis()->SetTitleOffset(0.8);
	hlc->GetXaxis()->CenterTitle();
	hlc->GetYaxis()->SetLabelSize(0.06);
	hlc->GetYaxis()->SetTitleSize(0.06);
	hlc->GetYaxis()->SetTitleOffset(0.8);
	hlc->GetYaxis()->CenterTitle();
	hlc->GetXaxis()->SetTitle("Local time (hours)");
	hlc->GetYaxis()->SetTitle("Normalized counts");
	hlc->SetStats(0);
	hlc->SetMarkerStyle(22);
	hlc->SetMarkerSize(1.2);
	hlc->Draw("E1P");
	// hlcRH->SetLineColor(2);
	hlcRH->SetMarkerStyle(25);
	hlcRH->Draw("E1PSAME");
	
	// hlonRH->SetLineColor(2);
	//hlonRH->Draw("SAME");
	
	// TCanvas c1("lon-dt", "lon-dt", 800, 1000);
	// c1.Divide(1, 2);
	
	cout << "KolmogorovTest lon: " << hlonMCAL->KolmogorovTest(hlonRH) << endl; 
	cout << "KolmogorovTest local time: " << hlc->KolmogorovTest(hlcRH) << endl;
	
	
	// canvas for month of the year distribution
	// NB: THE EVENTS SHOULD BE NORMALIZED TO THE NUMBER OF OBSERVING MONTHES!
	TCanvas *c12 = new TCanvas("c12", "c12", 800,600);	//1000,300
	c12->cd();
	gPad->SetTicks(1,1);
	hm->SetTitle("");
	hm->GetXaxis()->SetLabelSize(0.06);
	hm->GetXaxis()->SetTitleSize(0.06);
	hm->GetXaxis()->SetTitleOffset(0.8);
	hm->GetXaxis()->CenterTitle();
	hm->GetYaxis()->SetLabelSize(0.06);
	hm->GetYaxis()->SetTitleSize(0.06);
	hm->GetYaxis()->SetTitleOffset(0.8);
	hm->GetYaxis()->CenterTitle();
	hm->GetXaxis()->SetTitle("Month of the year");
	hm->GetYaxis()->SetTitle("Normalized counts");
	hm->SetStats(0);
	hm->SetMarkerStyle(22);
	hm->SetMarkerSize(1.2);
	hm->Draw("E1P");
	// hmRH->SetLineColor(2);
	hmRH->SetMarkerStyle(25);
	hmRH->Draw("E1PSAME");
	
	TCanvas *c13 = new TCanvas("c13", "c13", 800,600);	//1000,300
	c13->Divide(1,2);
	c13->cd(1);
	gPad->SetTicks(1,1);
	hmonth->SetTitle("AGILE-MCAL");
	hmonth->GetXaxis()->SetLabelSize(0.06);
	hmonth->GetXaxis()->SetTitleSize(0.06);
	hmonth->GetXaxis()->SetTitleOffset(0.8);
	hmonth->GetXaxis()->CenterTitle();
	hmonth->GetYaxis()->SetLabelSize(0.06);
	hmonth->GetYaxis()->SetTitleSize(0.06);
	hmonth->GetYaxis()->SetTitleOffset(0.8);
	hmonth->GetYaxis()->CenterTitle();
	hmonth->GetXaxis()->SetTitle("Month of the year since Jan 2008");
	hmonth->GetYaxis()->SetTitle("Counts");
	hmonth->SetStats(0);
	hmonth->Draw();
	
	c13->cd(2);
	gPad->SetTicks(1,1);
	hmonthRH->SetTitle("RHESSI (|lat| < 2.5 deg)");
	hmonthRH->GetXaxis()->SetLabelSize(0.06);
	hmonthRH->GetXaxis()->SetTitleSize(0.06);
	hmonthRH->GetXaxis()->SetTitleOffset(0.8);
	hmonthRH->GetXaxis()->CenterTitle();
	hmonthRH->GetYaxis()->SetLabelSize(0.06);
	hmonthRH->GetYaxis()->SetTitleSize(0.06);
	hmonthRH->GetYaxis()->SetTitleOffset(0.8);
	hmonthRH->GetYaxis()->CenterTitle();
	hmonthRH->GetXaxis()->SetTitle("Month of the year since Jan 2002");
	hmonthRH->GetYaxis()->SetTitle("Counts");
	hmonthRH->SetStats(0);
	hmonthRH->Draw();
	// draw longitude
*/

}
