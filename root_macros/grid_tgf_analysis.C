{
	// MM 05/11/10 plots only GRID/TGF waiting time. All other analysis done by subsequent macros
	// MM 01/02/10 modified to include evt data
	// MM 11/01/10 plot histogram for GRID/TGF waiting time
	
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
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	struct grecord {
		int contact;	
		double t0;	// TGF time 0
		int ievt;	// GRID 3901 event index
		double t;	// GRID event time
		double dt;	// t-t0
	};
	
	struct GRIDevt {
		Double_t	time;
		float		energy;
		float		theta;
		float		phi;
		float		RA;
		float		Dec;
		float		ph_earth; 
		char		brname03;
		char		evstatus;
	};

	TH1F *hdt = new TH1F("hdt", "hdt", 100,-0.1, 0.1);
//	TH1F *hph_earth = new TH1F("hph_earth", "hph_earth", 18, 0., 180.);
//	TH1F *hph_earth_R = new TH1F("hph_earth_R", "hph_earth_R", 18, 0., 180.);	// reversed: |ph_earth - 180|
//	TH1F *hE = new TH1F("hE", "hE", 1000, 0.1, 10000.1);
	
	
	ifstream in;
	in.open("/home/mcal/data_analysis/VSB/grid/Jan10-Aug10/grid_results_Jan10-Aug10_1.dat");
	grecord gevt;
	GRIDevt eevt;
	char aux[20];
	int  iaux;
	while (in >> aux >> gevt.contact >> gevt.t0 >> gevt.ievt >> gevt.t >> gevt.dt ) {
	
		hdt->Fill(gevt.dt);
	}
	in.close();
	
	// graphic section
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	hdt->Draw();
}