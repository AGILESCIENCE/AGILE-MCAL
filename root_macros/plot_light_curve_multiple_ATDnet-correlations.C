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
	TGraph *gEt[20];
	double zbefore[20];
	double zafter[20];
	double ctpeak[20];
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	char *label[] = {"(a)", "(b)", "(c)", "(d)", "(e)", "(f)", "(g)", "(h)", "(k)", "(j)", "(i)"};
	char *trg[] = {"Trigger 9846-7", "Trigger 10236-6", "Trigger 11921-22", "Trigger 10719-19"};
	
	// input data
	
	double atbin = 0.0002;
	double azbefore = -0.005;
	double azafter  =  0.005;
	double atbkgini = -2.9;
	double atbkgfin  =  0.0;
	for (int i=0; i<20; i++) {
		tbin[i]    = atbin;
		zbefore[i] = azbefore;
		zafter[i]  = azafter;	
	}
	
	rootfilename[0] = new TString("/MCAL_PROC/ARCHIVE/009846/RT009846_3908.root");
	t0[0] =  164676530.559712; // tgrbini = 0.0096
	
	// 
	rootfilename[1] = new TString("/MCAL_PROC/ARCHIVE/010236/RT010236_3908.root");
	t0[1] =  167064941.089152; // tgrbini = 0.0036
	
	// 
	rootfilename[2] = new TString("/MCAL_PROC/ARCHIVE/011921/RT011921_3908.root");
	t0[2] = 177376065.708844; // tgrbini = 0.0126
	
	// 
	rootfilename[3] = new TString("/MCAL_PROC/ARCHIVE/010719/RT010719_3908.root");
	t0[3] = 170021030.759944; // tgrbini = 0.0044
/*	
	// longest
	rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT006630_3908.root");
	t0[5] = 144988286.120896;
	tbin[5] = 0.0005;
	
	// maximum energy
	rootfilename[6] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007249_3908.root");
	t0[6] = 148782321.371584; // 
	tbin[6] = 0.0005;
	
	rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007568_3908.root");
	t0[5] = 150724236.546688;
	

		

	rootfilename[6] = new TString("");
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
	int ntgf = 4;
	for (int i=0; i<ntgf; i++) {
		grb[i] = new GRB3908(rootfilename[i]->Data());
		grb[i]->SetGRBBoundaries(atbkgini, atbkgfin, zbefore[i], zafter[i]);
		grb[i]->DefaultLightCurve();
		grb[i]->CustomLightCurve(t0[i], -3., +3., emin, emax, barsX, barsZ, tbin[i]);
		hlc[i] = grb[i]->GetLightCurve();
		// ctpeak[i] = grb[i]->GetTPeak();
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
		grb[i]->VSBDiagnostic();
		gEt[i] = grb[i]->GetEvsT();
	}

	// graphic section
	
	for (int i=0; i<ntgf; i++) {
		printf("%06d \t%20.6f \t%s\n", grb[i]->GetContact(), t0[i], ts[i]->AsString("s"));
	}
	
	
	double label_size   = 0.06;	// 0.07
	double text_size    = 0.08;	// 0.07
	double title_offset = 0.8;	// 0.6 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 800, 600);
	c1.Divide(2,ntgf, 0.001, 0.001);  // 3,3
	
	int nrow = ntgf;// 3
	int ncol = 2;// 3
	for (int i=0; i<ntgf; i++) {// 9
	
		// light curve pad
		
		c1.cd(i*2+1);
		gPad->SetTicks(1,1);
		
		TString ytitle("Counts / ");
		ytitle += tbin[i]*1000.;
		ytitle += " ms";
		
		hlc[i]->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		hlc[i]->GetYaxis()->SetRangeUser(0., 1.2*hlc[i]->GetMaximum());
		hlc[i]->SetStats(0);
		hlc[i]->SetTitle("");
		hlc[i]->GetXaxis()->SetLabelSize(label_size);
		hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
		hlc[i]->GetXaxis()->SetTitleSize(label_size);
		hlc[i]->GetXaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetXaxis()->CenterTitle();
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetYaxis()->CenterTitle();
		hlc[i]->Draw();
			
		TLatex t3;
		t3.SetTextSize(text_size);
		t3.DrawTextNDC(xpos, zpos, trg[i]);
			
		TLatex t1;
		t1.SetTextSize(text_size);
		TString str1("T0 = ");
		str1 += ts[i]->AsString("s");
		str1 += " UT";
		t1.DrawTextNDC(xpos, zpos-0.1, str1);
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.12, 0.02, label[i]);
		
		// E vs t plot pad
		
		c1.cd(i*2+2);
		gPad->SetTicks(1,1);
		gPad->SetLogy();
		TString hname("htmp");
		hname += i;
		TH2F *htmp = new TH2F(hname, "", 100, zbefore[i], zafter[i], 100, 0.1, 100.);
		htmp->SetStats(0);
		htmp->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		htmp->SetTitle("");
		htmp->GetXaxis()->SetLabelSize(label_size);
		htmp->GetXaxis()->SetTitle("Time-T0 (s)");
		htmp->GetXaxis()->SetTitleSize(label_size);
		htmp->GetXaxis()->SetTitleOffset(title_offset);
		htmp->GetXaxis()->CenterTitle();
		htmp->GetYaxis()->SetLabelSize(label_size);
		htmp->GetYaxis()->SetTitle("Energy (MeV)");
		htmp->GetYaxis()->SetTitleSize(label_size);
		htmp->GetYaxis()->SetTitleOffset(title_offset);
		htmp->GetYaxis()->CenterTitle();
		htmp->Draw("");
		gEt[i]->SetMarkerStyle(7);
		gEt[i]->Draw("P");
		
	}	

}
