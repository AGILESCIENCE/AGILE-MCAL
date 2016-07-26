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

	TString rootfilename("/MCAL_PROC/ARCHIVE/011108/RT011108_3908.root");
	double t0 = 172398504.772256;
	double emin=0.;
	double emax=2000.;
	double tbin=0.256;	
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	double zbefore=-50.;
	double zafter=150.;
	
	GRB3908 *grb3908 = new GRB3908(rootfilename);
	grb3908->DefaultLightCurve();
	grb3908->CustomLightCurve(t0, -100., +200., emin, emax, barsX, barsZ, tbin);
	
	TH1D *hlc = grb3908->GetLightCurve();
	hlc->GetXaxis()->SetRangeUser(zbefore, zafter);
	hlc->SetStats(0);
	hlc->SetTitle("");
	hlc->GetXaxis()->SetLabelSize(0.04);
	hlc->GetXaxis()->SetTitle("Time-T0 (s)");
	hlc->GetXaxis()->SetTitleSize(0.04);
	hlc->GetXaxis()->CenterTitle();
	hlc->GetYaxis()->SetLabelSize(0.04);
	hlc->GetYaxis()->SetTitle("Counts / 0.1 ms");
	hlc->GetYaxis()->SetTitleSize(0.04);
	hlc->GetYaxis()->CenterTitle();
	hlc->Draw();
	
	gPad->SetTicks(1,1);
}
