{
	// MM 23/3/09 corretto baco: millisecondi e NON secondi!!!
	// MM 6/3/09 sovrapposizione segnali onde sferiche di Fullekrug con i dati MCAL
	
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
	mcalstyle->SetLabelSize(0.06, "X");
	mcalstyle->SetLabelSize(0.06, "Y");
	gROOT->SetStyle("mcalstyle");

	TH1F hs1("hs1", "hs1", 500000, -0.250, 0.250);
	
	float time, efield;
	FILE *fin;
	GRB3908 *grb;
	
	// mtgf-1-1
	// fin = fopen("/home/mcal/data_analysis/VSB/sferics_correlation/mtgf-1-1.bst", "r");
	// TString *rootfilename = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT006996_3908.root");
	// double t0 = 147234103.659008;
	
	// mtgf-2-1
	// fin = fopen("/home/mcal/data_analysis/VSB/sferics_correlation/mtgf-2-1.bst", "r");
	// TString *rootfilename = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007007_3908.root");
	// double t0 = 147301127.546016;
	
	// mtgf-2-2
	// fin = fopen("/home/mcal/data_analysis/VSB/sferics_correlation/mtgf-2-2.bst", "r");
	// TString *rootfilename = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007007_3908.root");
	// double t0 = 147301466.136544;
	
	TString *bstname = new TString("tgf-180309-021249");	//NB: senza .bst
	TString *rootfilename = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT009805_3908.root");
	double t0 =    164427169.346816;

	TString *sfericpath = new TString("/home/mcal/data_analysis/VSB/analysis_09-03-20/sferic_correlation/");
	sfericpath->Append(bstname->Data());
	sfericpath->Append(".bst");
	
	TString *figname = new TString("~/data_analysis/VSB/analysis_09-03-20/sferic_correlation/");
	figname->Append(bstname->Data());
	figname->Append("_009805-9.gif");
	
	cout << "loading sferics data file " << sfericpath->Data() << endl;
	fin = fopen(sfericpath->Data(), "r");
		
	// load sferics and fill histo
	for (int i=1; i<=500000; i++) {
		fscanf(fin, "%e %e", &time, &efield);
		hs1.SetBinContent(i, efield);
		// if (i%10000 == 0) cout << i << "  " << time << "  " << efield << endl;
	}
	float xlo = -0.025;
	float xhi =  0.025;
	float tbin = 0.0005;
	fclose(fin);
	TCanvas c1("c1", "c1", 800, 600);
	c1.Divide(1,2);
	c1.cd(1);
	hs1.SetStats(0);
	hs1.SetTitle("Electric field");
	hs1.GetXaxis()->SetTitle("t-t0 (s)");
	hs1.GetYaxis()->SetTitle("E field V/m");
	hs1.GetYaxis()->SetTitleSize(0.06);
	hs1.GetXaxis()->SetTitleSize(0.06);
	hs1.GetXaxis()->CenterTitle();
	hs1.GetXaxis()->SetRangeUser(xlo, xhi);
	hs1.GetYaxis()->CenterTitle();
	hs1.GetXaxis()->SetTitleOffset(0.8);
	hs1.GetYaxis()->SetTitleOffset(0.8);
	hs1.Draw();
	
	
	// load MCAL light curve
	grb = new GRB3908(rootfilename->Data());
	grb->DefaultLightCurve();
	grb->CustomLightCurve(t0, -1., +1., 0., 2000., 0x7FFF, 0x7FFF, tbin);
	TH1D *hlc = grb->GetLightCurve();
	c1.cd(2);
	hlc->SetStats(0);
	// hlc->SetTitle("MCAL count rate T0 = 2008/08/31 02:21:43.659");
	// hlc->SetTitle("MCAL count rate T0 = 2008/08/31 20:58:47.546");
	hlc->SetTitle("MCAL count rate ");
	hlc->GetXaxis()->SetTitle("t-t0 (s)");
	hlc->GetXaxis()->CenterTitle();
	hlc->GetXaxis()->SetRangeUser(xlo, xhi);
	hlc->GetYaxis()->SetTitle("counts/0.5ms");
	hlc->GetYaxis()->CenterTitle();
	hlc->GetXaxis()->SetTitleOffset(0.8);
	hlc->GetYaxis()->SetTitleOffset(0.8);
	hlc->Draw();
	
	c1->Print(figname->Data());
	
}