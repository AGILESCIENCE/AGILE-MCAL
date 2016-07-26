{
	// MM 26/04/10 updated to Mar.'10
	//             exclude triggers with local time within 2-5
	
	// MM 10/12/09 compare offline vs online triggers, created for AGU presentation
	
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
	mcalstyle->SetLabelSize(0.06, "XYZ");
	mcalstyle->SetTitleSize(0.06, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(0.8, "XYZ");
	gROOT->SetStyle("mcalstyle");

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
	
	struct offline_trg  {
			int runid;
			double t0;
			float t90;
			float lon;
			float lat;
			float counts;
			float loctime;
	};
	
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
	
	// read offline triggers
	
	TTree toff;
	offline_trg aoff;
	 // tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_09-03-16/VSB_global_info_10.dat",	 // dataset 16/3
	 // /data1/mcal/data_analysis/VSB/tgf_candidates_Jun08-Mar09/green_candidates_COMPLETE_2.txt     // dataset Jun08-Mar09 paper JGR
// 	 toff.ReadFile("/data1/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/cut8/trg2_cut8.txt",	 "runid/I:trg/I:t0/D:t90/F:lon/F:lat/F:counts/F:loctime/F");	
	 toff.ReadFile("/data1/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg2_cut8_5000-15154.txt",	 	"runid/I:trg/I:t0/D:t90/F:lon/F:lat/F:counts/F:loctime/F");	
	toff.SetBranchAddress("runid", &aoff.runid);
	toff.SetBranchAddress("t0", &aoff.t0);
	toff.SetBranchAddress("t90", &aoff.t90);
	toff.SetBranchAddress("lon", &aoff.lon);
	toff.SetBranchAddress("lat", &aoff.lat);
	toff.SetBranchAddress("counts", &aoff.counts);
	toff.SetBranchAddress("loctime", &aoff.loctime);

	int noff = toff.GetEntries();
	cout << "Total number of offline entries = " << noff << endl;
	
	// ----------- create histograms ---------------
	
	int lonbin = 72; // 12 o 36 o 72
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, -180., 180.);
	TH1F *ht90 = new TH1F("ht90", "ht90", 200, 0., 0.1);	//200
	TH1F *hcounts = new TH1F("hcounts", "hcounts", 500, 0, 1000);
	int lcbin = 24; // 36
	TH1F *hlc  = new TH1F("hlc", "hlc", lcbin, 0., 24.);
	
	TH1F *hlonOFF = new TH1F("hlonOFF", "hlonOFF", lonbin, -180., 180.);
	TH1F *ht90OFF = new TH1F("ht90OFF", "ht90OFF", 200, 0., 0.1);	//200
	TH1F *hcountsOFF = new TH1F("hcountsOFF", "hcountsOFF", 500, 0, 1000);
	TH1F *hlcOFF  = new TH1F("hlcOFF", "hlcOFF", lcbin, 0., 24.);
	
	// ----------- process entries & fill histograms ---------------
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		
		// calculate local time
		int hh, mm, ss;
		sscanf(atrg.time, "%2d:%2d:%2d", &hh, &mm, &ss);
		
		int year, month, day;
		sscanf(atrg.date, "%4d-%2d-%2d", &year, &month, &day);
		
		// if (year < 2009 || (year == 2009 && month<=9)) {
		if (1) {
			float lc = fmod(hh + mm/60. + ss/3600. + atrg.lon*0.06667, 24.);
			hlc->Fill(lc);
			ht90->Fill(atrg.t90);
			hcounts->Fill(atrg.counts + atrg.cbkg);
			hlon->Fill(atrg.lon);
		}
		
	}
	
	int nlc=0;
	for (int i=0; i<noff; i++) {
		toff.GetEntry(i);
		
		// calculate local time
		if (aoff.loctime<2. || aoff.loctime>5.) {	// 
			hlcOFF->Fill(aoff.loctime);
			ht90OFF->Fill(aoff.t90);
			hcountsOFF->Fill(aoff.counts);
			hlonOFF->Fill(aoff.lon);
			nlc++;
		}
	}
	cout << " n offline trg with loctime <2 || > 5 = " << nlc << endl;
	
	// ----------- graphic section ---------------
	
	TCanvas ccomp("ccomp", "ccomp", 800, 800);
	ccomp.Divide(1, 2);
	ccomp.SetTicks(1,1);
	
	ccomp.cd(1);
	gPad->SetTicks(1,1);
	hlon->SetTitle("");
	hlonOFF->GetXaxis()->SetLabelSize(0.06);
	hlonOFF->GetXaxis()->SetTitleSize(0.06);
	hlonOFF->GetXaxis()->SetTitleOffset(0.8);
	hlonOFF->GetXaxis()->CenterTitle();
	hlonOFF->GetYaxis()->SetLabelSize(0.06);
	hlonOFF->GetYaxis()->SetTitleSize(0.06);
	hlonOFF->GetYaxis()->SetTitleOffset(0.8);
	hlonOFF->GetYaxis()->CenterTitle();
	hlonOFF->GetXaxis()->SetTitle("Longitude (deg)");
	hlonOFF->GetYaxis()->SetTitle("Events");
	hlon->SetStats(0);
	hlonOFF->SetMarkerStyle(22);
	hlonOFF->SetMarkerSize(1.2);
	hlonOFF->SetTitle("");
	hlonOFF->SetStats(0);
	hlonOFF->SetLineColor(2);
	hlonOFF->Draw();	// E1P
	hlon->Draw("SAME");	// E1P
	
	ccomp.cd(2);
	gPad->SetTicks(1,1);
	hlc->SetTitle("");
	hlcOFF->GetXaxis()->SetLabelSize(0.06);
	hlcOFF->GetXaxis()->SetTitleSize(0.06);
	hlcOFF->GetXaxis()->SetTitleOffset(0.8);
	hlcOFF->GetXaxis()->CenterTitle();
	hlcOFF->GetYaxis()->SetLabelSize(0.06);
	hlcOFF->GetYaxis()->SetTitleSize(0.06);
	hlcOFF->GetYaxis()->SetTitleOffset(0.8);
	hlcOFF->GetYaxis()->CenterTitle();
	hlcOFF->GetXaxis()->SetTitle("Local time (hours)");
	hlcOFF->GetYaxis()->SetTitle("Events");
	hlc->SetStats(0);
	hlcOFF->SetMarkerStyle(22);
	hlcOFF->SetMarkerSize(1.2);
	hlcOFF->SetTitle("");
	hlcOFF->SetStats(0);
	hlcOFF->SetLineColor(2);
	hlcOFF->Draw();	// E1P
	hlc->Draw("SAME");	// E1P
	
	TCanvas c4("c4", "c4", 400, 800);
	c4.Divide(1, 2);
	c4.SetTicks(1,1);
	
	c4.cd(1);
	gPad->SetTicks(1,1);
	hcounts->SetTitle("Total number of events distribution");
	hcounts->SetStats(0);
	hcounts->GetXaxis()->SetTitle("Number of photons");
	hcounts->GetYaxis()->SetTitle("Events");
	hcountsOFF->SetLineColor(2);
	hcountsOFF->Draw();	// E1P
	hcounts->Draw("SAME");
	
	c4.cd(2);
	ht90->SetTitle("Burst duration distribution");
	ht90->SetStats(0);
	ht90->GetXaxis()->SetTitle("Duration (s)");
	ht90->GetYaxis()->SetTitle("Events");
	ht90->GetXaxis()->SetTitleSize(0.06);
	ht90->GetXaxis()->SetTitleOffset(0.8);
	ht90->GetXaxis()->SetLabelSize(0.06);
	ht90->GetYaxis()->SetTitleSize(0.06);
	ht90->GetYaxis()->SetTitleOffset(0.8);
	ht90->GetYaxis()->SetLabelSize(0.06);
	ht90OFF->SetLineColor(2);
	ht90OFF->Draw();	// E1P
	ht90->Draw("SAME");


}