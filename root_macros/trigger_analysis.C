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
	Trigger aflg;
	

	// load the trigger file flagged by FF (BOOST and ECLIPSE events are already rejected)
	TTree tspike;
	tspike.ReadFile("/home/martino/C_projects/workdir/BUILD_MCALSW_devel/root_macros/TGF_bus_cleaned_5900-7604.out", 	"runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:type/C:sa/I:lng/I:subms/I:nrm/I:rmid/I:sitid/I:lon/F:lat/F:h/F:dt/F");
	tspike.SetBranchAddress("runid", &aflg.runid);
	tspike.SetBranchAddress("trg", &aflg.trg);
	tspike.SetBranchAddress("t0", &aflg.t0);
	tspike.SetBranchAddress("tstop", &aflg.tstop);
	tspike.SetBranchAddress("ttot", &aflg.ttot);
	tspike.SetBranchAddress("date", &aflg.date);
	tspike.SetBranchAddress("time", &aflg.time);
	tspike.SetBranchAddress("type", &aflg.type);
	tspike.SetBranchAddress("sa", &aflg.sa);
	tspike.SetBranchAddress("lng", &aflg.lng);
	tspike.SetBranchAddress("subms", &aflg.subms);
	tspike.SetBranchAddress("nrm", &aflg.nrm);
	tspike.SetBranchAddress("rmid", &aflg.rmid);
	tspike.SetBranchAddress("sitid", &aflg.sitid);
	tspike.SetBranchAddress("lon", &aflg.lon);
	tspike.SetBranchAddress("lat", &aflg.lat);
	tspike.SetBranchAddress("h", &aflg.h);
	tspike.SetBranchAddress("dt", &aflg.dt);	
	int nflagged = tspike.GetEntries();
	cout << "Total number of flagged entries = " << nflagged << endl;

	// ----------- process entries & fill histograms ---------------
	
	int swtrg[5]
	for (int i=0; i<5;i++) swtrg[i]=0;
	
	for (int i=0; i<nflagged; i++) {
		tspike.GetEntry(i);
		


	}

}