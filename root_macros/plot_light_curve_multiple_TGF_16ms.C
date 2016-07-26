{
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

	TString *rootfilename[20];
	double t0[20];
	double tbin[20];
	TTimeStamp ts[20];
	GRB3908 *grb[20];
	TH1D *hlc[20];
	double zbefore[20];
	double zafter[20];
	double ctpeak[20];
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	char *label[] = {"(a)", "(b)", "(c)", "(d)", "(e)", "(f)", "(g)", "(h)", "(k)", "(j)", "(i)"};
	
	// input data
	
	double atbin = 0.0002;
	double azbefore = -0.005;
	double azafter  =  0.005;
	for (int i=0; i<20; i++) {
		tbin[i]    = atbin;
		zbefore[i] = azbefore;
		zafter[i]  = azafter;	
	}
	for (int i=3; i<20; i++) tbin[i]    = 0.0005;
	
/*	rootfilename[0] = new TString("/data2/TGF_reprocessing080415/003170_2007-12-03_23:44:51/RT003170_3908.root");
	t0[0] = 123810291.577232;
*/
	rootfilename[0] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007467_3908.root");
	t0[0] = 150112496.456576;
	
	rootfilename[1] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007794_3908.root");
	t0[1] = 152118554.796208;
	
	rootfilename[2] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007919_3908.root");
	t0[2] = 152878953.427824;
	
	
	rootfilename[3] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT005945_3908.root");
	t0[3] = 140800814.2376;
	
	rootfilename[4] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT006630_3908.root");
	t0[4] = 144988286.118896;
			
	
	rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007568_3908.root");
	t0[5] = 150724236.546688;
	

		

/*	rootfilename[6] = new TString("");
	t0[6] = ;
	
	rootfilename[7] = new TString("");
	t0[7] = ;
	
	rootfilename[8] = new TString("");
	t0[8] = ;
*/	
				
/* 	// template
	
	rootfilename[] = new TString("/data2/");
	t0[] = ;
	tbin[] = ;
	zbefore[] = ;
	zafter[] = ;
*/
	
	// build light curves (load libMCV.so before)
	
	// work on 3908 data
	for (int i=0; i<6; i++) {
		grb[i] = new GRB3908(rootfilename[i]->Data());
		grb[i]->DefaultLightCurve();
		grb[i]->CustomLightCurve(t0[i], -3., +3., emin, emax, barsX, barsZ, tbin[i]);
		hlc[i] = grb[i]->GetLightCurve();
		ctpeak[i] = grb[i]->GetTPeak();
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
	}

	// graphic section
	
	for (int i=0; i<6; i++) {
		printf("%06d \t%20.6f \t%s\n", grb[i]->GetContact(), t0[i], ts[i]->AsString("s"));
	}
	
	
	double label_size   = 0.05;
	double text_size    = 0.05;
	double title_offset = 1.0;
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 1400, 800);
	c1.Divide(3,2, 0.001, 0.001);  // 3,3
	
	int nrow = 2;// 3
	int ncol = 3;// 3
	for (int i=0; i<6; i++) {// 9
		c1.cd(i+1);
		gPad->SetTicks(1,1);
		
		TString ytitle("Counts / ");
		ytitle += tbin[i]*1000.;
		ytitle += " ms";
		
		hlc[i]->GetXaxis()->SetRangeUser(ctpeak[i] + zbefore[i], ctpeak[i] + zafter[i]);
		hlc[i]->GetYaxis()->SetRangeUser(0., 1.2*hlc[i]->GetMaximum());
		hlc[i]->SetStats(0);
		hlc[i]->SetTitle("");
		hlc[i]->GetXaxis()->SetLabelSize(label_size);
		hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
		hlc[i]->GetXaxis()->SetTitleSize(label_size);
		hlc[i]->GetXaxis()->CenterTitle();
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetYaxis()->CenterTitle();
		hlc[i]->Draw();
			
		TLatex t1;
		t1.SetTextSize(text_size);
		TString str1("T0 = ");
		str1 += ts[i]->AsString("s");
		str1 += " UT";
		t1.DrawTextNDC(xpos, zpos, str1);
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.12, 0.02, label[i]);
	}	

}
