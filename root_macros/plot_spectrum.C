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

	TFile f11("~/data_analysis/calibration/CGS_Na22/root/RT006152_3909_sel_01.root");
	TTree *t11 = (TTree *) f11.Get("tdata_shift");
	int nbins = 10000;
	float time = 11500.; // measurement duration
	
	float *bins = new float[nbins];
	float e1 = 3.;	// 1° intervallo di energia
	float bin1 = 0.02;
	int n1 = (int) e1/bin1;
	
	float e2 = 30.;	// 2° intervallo di energia
	float bin2 = 0.1;
	int n2 = n1 + (int) (e2 - e1)/bin2;
	
	float e3 = 300.;	// 2° intervallo di energia
	float bin3 = 1.;
	int n3 = n2 + (int) (e3 - e2)/bin3;
	
	for (int i=0; i<=n1; i++) bins[i] = i*bin1;			// 20 kev fino a 3 MeV
	for (int i=n1+1; i<=n2; i++) bins[i] = e1 + (i-n1)*bin2;	// 100 kev fino a 3 MeV
	for (int i=n2+1; i<=n3; i++) bins[i] = e2 + (i-n2)*bin3;	// 100 kev fino a 3 MeV
	
	
//	TH1F *h11 = new TH1F("h11", "h11", nbins, 0., 200.);
	TH1F *h11 = new TH1F("h11", "h11", n3, bins);
	float Etot;
	t11->SetBranchAddress("Etot", &Etot);
	int i=0;
	while (t11->GetEntry(i++)) {
		h11->Fill(Etot);
		if (i%100000 == 0) cout << "processed events: " << i << endl;
	}
	cout << "rate (counts/sec) = " << h11->Integral()/time << endl;
 
	// renormalize
	for (int i=1; i<n3-1; i++) {
		float de = h11->GetBinLowEdge(i+1) - h11->GetBinLowEdge(i);
		float newval = h11->GetBinContent(i)/(de*time);
		h11->SetBinContent(i, newval);
	}
	
	// dump spectrum
	for (int i=1; i<n3-1; i++) printf("%6d \t%8.3f \t%8.3f \t%f\n", i, h11->GetBinLowEdge(i), h11->GetBinLowEdge(i+1), h11->GetBinContent(i));

    	// plot
	TCanvas *c11 = new TCanvas("c11", "c11", 800, 600);
	c11->SetLogx();
	c11->SetLogy();
	h11->Draw();

}
