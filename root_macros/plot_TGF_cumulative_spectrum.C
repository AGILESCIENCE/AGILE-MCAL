{
	// MM 31/03/2011
	// plots the TGF cumulative spectrum together with models (like Tavani's PRL paper)
	
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
	ifstream in;
	int nbin=0;
	float ae, aeerr, ay, ayerr, am, amerr, id, e[200], eerr[200], y[200], yerr[200];
	in.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/6/6b.dat");
	while (in >> ae >> aeerr >> ay >> ayerr) {
		e[nbin] = ae * 0.001;	
		eerr[nbin] = aeerr * 0.001;	
		y[nbin] = ay * 1000.;	
		yerr[nbin] = ayerr * 1000.;	
		nbin++;
	}
	
	ifstream incut;
	TGraph *gcut = new TGraph();
	int ncut=0;
	incut.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/6/6a_cutoffpo.dat");
	while (incut >> ae >> aeerr >> ay >> ayerr >> am >> amerr >> id) {
		gcut->SetPoint(ncut++, ae*0.001, am*1000.);
	}
	
	ifstream inbkn;
	TGraph *gbkn = new TGraph();
	int nbkn=0;
	inbkn.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/6/6a_bknpo.dat");
	while (inbkn >> ae >> aeerr >> ay >> ayerr >> am >> amerr >> id) {
		gbkn->SetPoint(nbkn++, ae*0.001, am*1000.);
	}
	
	TGraphErrors *gsp = new TGraphErrors(nbin, e, y, eerr, yerr);
	TCanvas *c1 = new TCanvas();
	c1->SetLogx();
	c1->SetLogy();
	c1->SetTicks(1,1);
	gsp->GetXaxis()->SetTitle("Counts / (s MeV)");
	gsp->GetYaxis()->SetTitle("Energy (MeV)");
	gsp->GetXaxis()->SetRangeUser(0.3, 800.);
	gsp->GetYaxis()->SetRangeUser(0.01, 20000.);
	gsp->SetTitle("");
	gsp->Draw("AP");
	
	gcut->SetLineColor(2);
	gcut->Draw("LSAME");
	
	gbkn->SetLineColor(4);
	gbkn->Draw("LSAME");

}