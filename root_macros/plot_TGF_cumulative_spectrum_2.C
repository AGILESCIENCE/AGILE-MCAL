{
	// MM 02/12/2011: reads .dat files with model included
	// MM 31/03/2011: plots the TGF cumulative spectrum together with models (like Tavani's PRL paper)
	
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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");
	
	// load data
	//ifstream in;
	int nbin=0;
	float ae, aeerr, ay, ayerr, am, amerr, id, e[200], eerr[200], y[200], yerr[200];
/*	in.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/6/6b.dat");
	while (in >> ae >> aeerr >> ay >> ayerr) {
		e[nbin] = ae * 0.001;	
		eerr[nbin] = aeerr * 0.001;	
		y[nbin] = ay * 1000.;	
		yerr[nbin] = ayerr * 1000.;	
		nbin++;
	}*/
	
	// load total spectrum
	
	ifstream incut;
	TGraph *gcut = new TGraph();
	int ncut=0;
	incut.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/7/7a_fit.dat");
	float ttot = 0.3302002;
	while (incut >> ae >> aeerr >> ay >> ayerr >> am) {
		e[nbin] = ae * 0.001;	
		eerr[nbin] = aeerr * 0.001;	
		y[nbin] = ay * 1000.;	
		yerr[nbin] = ayerr * 1000.;	
		nbin++;
		gcut->SetPoint(ncut++, ae*0.001, am*1000.);
	}
	
	// load low energy TGF cumulative spectrum
	
	ifstream inLE;
	TGraph *gLE = new TGraph();
	int nLE=0;
	TGraphErrors *gspLE = new TGraphErrors(10);
	inLE.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/7/LE/7a_LE_cutoffpl.dat");
	float tLE = 0.2767999;
	float rLE = tLE/ttot;
	while (inLE >> ae >> aeerr >> ay >> ayerr >> am >> amerr >> id) {
		gspLE->SetPoint(nLE, ae*0.001, ay*1000.*rLE);	
		gspLE->SetPointError(nLE, aeerr * 0.001, ayerr * 1000.*rLE);	
		gLE->SetPoint(nLE++, ae*0.001, am*1000.*rLE);
	}
	gLE->SetPoint(nLE++, 35., 0.1);	// estrapolazione
	
	// load high energy TGF cumulative spectrum
	
	ifstream inHE;
	TGraph *gHE = new TGraph();
	int nHE=0;
	int nHE2=0;
	TGraphErrors *gspHE = new TGraphErrors(10);
//	inHE.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/7/HE/7a_HE_fit_modified.dat");
	inHE.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/7/HE/7a_HE_bknpo.dat");
	float tHE = 0.05339999;
	float rHE = tHE/ttot;
	while (inHE >> ae >> aeerr >> ay >> ayerr >> am >> amerr >> id) {
		gspHE->SetPoint(nHE, ae*0.001, ay*1000.*rHE);	
		gspHE->SetPointError(nHE++, aeerr * 0.001, ayerr * 1000.*rHE);
		gHE->SetPoint(nHE2++, ae*0.001, am*1000.*rHE); // if (ae>=2000.) 
	}
	
	TGraphErrors *gsp = new TGraphErrors(nbin, e, y, eerr, yerr);
	TCanvas *c1 = new TCanvas();
	c1->SetLogx();
	c1->SetLogy();
	c1->SetTicks(1,1);
	gspHE->GetYaxis()->SetTitle("Counts / (s MeV)");
	gspHE->GetXaxis()->SetTitle("Energy (MeV)");
	gspHE->GetXaxis()->SetRangeUser(0.3, 800.);
	gspHE->GetYaxis()->SetRangeUser(0.01, 20000.);
	gspHE->SetTitle("");
	gspHE->Draw("AP");
//	gsp->SetTitle("");
//	gsp->Draw("PSAME");
	
	gcut->SetLineColor(2);
	gcut->Draw("LSAME");
	
	gLE->SetLineColor(4);
	gLE->Draw("LSAME");
	
	gspLE->SetTitle("");
	gspLE->Draw("PSAME");
	gHE->SetLineColor(3);
	gHE->Draw("LSAME");

}