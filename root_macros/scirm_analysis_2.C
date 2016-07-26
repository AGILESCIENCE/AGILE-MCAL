{
// MM 02/04/2014 visualize root sci RM files created with project scirm_analysis
	
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
	gROOT->SetStyle("mcalstyle");

	struct rm {
		int id;
		double t; // it was float
		int xtot;
		int ztot;
		int x[10];
		int z[10];
		float xhr;
		float zhr;
		long int ac[5];
		double lon;
		double lat;
		
		double h;
		double etheta;
		double ephi;
		float lt; // local time
		int flag;
	};
	rm srm;
	
	TChain trm("scirm");
	char fname[100];
	int cini = 43300;
	int cfin = 43395;
	cout << "Loading contacts:" << endl;
	for (int i=cini; i<=cfin; i++) {
		sprintf(fname, "/home/mcal/data_analysis/scirm/rootfiles/%06d_scirm.root", i);
		TFile f(fname);
		if (f.IsZombie() == 0) trm.Add(fname);
	}
	//trm.Print();
	
	trm.Draw("10.*xtot:t");
	//trm.Draw("etheta:ephi", "xhr==0", "colz");
	//trm.Draw("xtot:lon", "xhr==0 && etheta<40 && (lt<5.7 || lt>6.2) && (lon<-65. || lon>-10.)", "colz");
	//trm.Draw("xtot:lt", "xhr==0 && etheta<60 && (lt<5.7 || lt>6.2) && (lon<-65. || lon>-10.)", "colz");
	 
}
