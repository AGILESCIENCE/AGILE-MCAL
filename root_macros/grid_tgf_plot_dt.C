{
	// MM 30/06/11 plots t-t0 histogram, based on grid_tgf_analysis_2.C
	
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
	
	TH1F *hdt = new TH1F("hdt", "hdt", 100,-0.1, 0.1);
	TH1F *hdtB = new TH1F("hdtB", "hdtB", 100,-0.1, 0.1);
	
	// read GRID data file: class A		
	ifstream in;
	in.open("/home/mcal/data_analysis/VSB/grid/Jan10-Feb11/grid_results_Jan10-Feb11.dat");	// lista A 
	grecord gevt;
	char aux[20];
	int  iaux;
	while (in >> aux >> gevt.contact >> gevt.t0 >> gevt.ievt >> gevt.t >> gevt.dt) {	
		hdt->Fill(gevt.dt);		
	}
	in.close();
	in.clear();
	
	// read GRID data file: class B (cut3) not A		
	in.open("/home/mcal/data_analysis/VSB/grid/classB_cut3_notA/grid_results_classB_cut3_notA.dat");	// lista A 
	while (in >> aux >> gevt.contact >> gevt.t0 >> gevt.ievt >> gevt.t >> gevt.dt) {	
		hdtB->Fill(gevt.dt);		
	}
	in.close();
		
	// graphic section
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	c1->Divide(2,2);
	c1->cd(1);
	hdt->Draw();
	c1->cd(2);
	hdtB->Draw();
}

