plot_light_curve_single_TGF(int cont=11026, double time0=171889319.048464, double tg1=-1., double e1=-1., double tg2=-1., double e2=-1., double tg3=-1., double e3=-1.)
{
	// MM 15/02/10 parameterized; add GRID events, if any
	//             cont	contact number
	//             time0	TGF t0
	//             tgX, eX	t0 and energy for GRID evt X
	
	// MM 28/10/09 plots a single light curve for a TGF with energy vs. time scatter plot
	// based on plot_light_curve_multiple_TGF_paper.C

	gROOT->Reset();
	// gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
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
	char *trg[] = {"Trigger 8958-8", "Trigger 7467-3", "Trigger 9669-1", "Trigger 7919-1", "Trigger 7728-6", "Trigger 6630-2", "Trigger 7249-4"};
	
	// input data
	
	double atbin = 0.0002;
	double azbefore = -0.010;
	double azafter  =  0.010;
	double atbkgini = -2.9;
	double atbkgfin  =  0.0;
	for (int i=0; i<20; i++) {
		tbin[i]    = atbin;
		zbefore[i] = azbefore;
		zafter[i]  = azafter;	
	}
	
	// brightest
	char aux[100];
	sprintf(aux, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", cont, cont);
	rootfilename[0] = new TString(aux);
	t0[0] =  171889319.048464; 
	// t0[0] =  time0; 
	
	
	// build light curves (load libMCV.so before)
	
	// work on 3908 data
	for (int i=0; i<1; i++) {
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
	
	for (int i=0; i<1; i++) {
		printf("%06d \t%20.6f \t%s\n", grb[i]->GetContact(), t0[i], ts[i]->AsString("s"));
	}
	
	
	double label_size   = 0.06;	// 0.07
	double text_size    = 0.07;	// 0.07
	double title_offset = 0.8;	// 0.6 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 800, 600);//800,1000
	c1.Divide(1,2, 0.001, 0.001);  // 2,7
	
		// light curve pad
		
	c1.cd(1);
	gPad->SetTicks(1,1);
	
	TString ytitle("Counts / ");
	ytitle += tbin[i]*1000.;
	ytitle += " ms";
	
	hlc[0]->GetXaxis()->SetRangeUser(zbefore[0], zafter[0]);
	hlc[0]->GetYaxis()->SetRangeUser(0., 1.2*hlc[0]->GetMaximum());
	hlc[0]->SetStats(0);
	hlc[0]->SetTitle("");
	hlc[0]->GetXaxis()->SetLabelSize(label_size);
	hlc[0]->GetXaxis()->SetTitle("Time-T0 (s)");
	hlc[0]->GetXaxis()->SetTitleSize(label_size);
	hlc[0]->GetXaxis()->SetTitleOffset(title_offset);
	hlc[0]->GetXaxis()->CenterTitle();
	hlc[0]->GetYaxis()->SetLabelSize(label_size);
	hlc[0]->GetYaxis()->SetTitle(ytitle);
	hlc[0]->GetYaxis()->SetTitleSize(label_size);
	hlc[0]->GetYaxis()->SetTitleOffset(title_offset);
	hlc[0]->GetYaxis()->CenterTitle();
	hlc[0]->Draw();
		
	TLatex t3;
//	t3.SetTextSize(text_size);
//	t3.DrawTextNDC(xpos, zpos, trg[0]);
		
	TLatex t1;
	t1.SetTextSize(text_size);
	TString str1("");	// T0 = 
	str1 += ts[0]->AsString("s");
	str1 += " UT";
	t1.DrawTextNDC(xpos, zpos-0.1, str1);
	
	TLatex t2;
//	t2.SetTextSize(text_size);
//	t2.DrawTextNDC(0.12, 0.02, label[0]);
	
	// E vs t plot pad
	
	c1.cd(2);
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
	gEt[0]->SetMarkerStyle(7);
	gEt[0]->Draw("P");
		
	

}
