{
	// MM 18/08/09 modified for TGF paper after Rev.#2 comments
	// ricordarsi di lanciare prima il comando .L ~/local/mcalsw/lib/libMCV.so

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
	mcalstyle->SetLabelSize(0.1, "X");
	mcalstyle->SetLabelSize(0.1, "Y");
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
	char *trg[] = {"Trigger 11026-1", "Trigger 10309-6", "Trigger 7728-6"};
	
	// input data
	
	double atbin = 0.0002;
	double azbefore = -0.0078; //-0.0048
	double azafter  =  0.0079; // 0.0049
	double atbkgini = -2.9;
	double atbkgfin  =  0.0;
	for (int i=0; i<20; i++) {
		tbin[i]    = atbin;
		zbefore[i] = azbefore;
		zafter[i]  = azafter;	
	}
	
	// brightest
	rootfilename[0] = new TString("/MCAL_PROC/ARCHIVE/011026/RT011026_3908.root");
	t0[0] =  171889319.034464 + 0.014; 
	
	// brightest
	// rootfilename[0] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT008958_3908.root");
	// t0[0] =  159242835.342816; // tgrbini = 0.0096
	
	// second brightest
	// rootfilename[1] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007467_3908.root");
	// t0[1] =  150112496.460176; // tgrbini = 0.0036
	
	// third brightest
	// rootfilename[2] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT009669_3908.root");
	// t0[2] = 163590087.9574; // tgrbini = 0.0126
	
	// maximum HR
	// rootfilename[1] = new TString("/MCAL_PROC/ARCHIVE/007919/RT007919_3908.root");
	// t0[1] = 152878953.432224; // tgrbini = 0.0044
	rootfilename[1] = new TString("/MCAL_PROC/ARCHIVE/010309/RT010309_3908.root");
	t0[1] = 167511834.090656 + -0.011;
	
	// short and bright (max fluence/t90)
	// rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT009765_3908.root");
	// t0[5] = 164181744.99452; // tgrbini = 0.005
	
	// shortest
	rootfilename[2] = new TString("/MCAL_PROC/ARCHIVE/007728/RT007728_3908.root");
	t0[2] = 151709901.659400; 
	
	// longest
	rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT006630_3908.root");
	t0[5] = 144988286.120896;
	tbin[5] = 0.0005;
	
	// maximum energy
	rootfilename[6] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007249_3908.root");
	t0[6] = 148782321.371584; // 
	tbin[6] = 0.0005;
	
/*	rootfilename[5] = new TString("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT007568_3908.root");
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
	int nb  = 3;
	for (int i=0; i<nb; i++) {//7
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
	
	for (int i=0; i<nb; i++) {//7
		printf("%06d \t%20.6f \t%s\n", grb[i]->GetContact(), t0[i], ts[i]->AsString("s"));
	}
	
	
	double label_size   = 0.09;	// 0.07
	double text_size    = 0.09;	// 0.07
	double title_offset = 0.45;	// 0.6 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.7;
	
	TCanvas c1("c1", "", 800, 600);// 800, 1000
	//c1.Divide(2,7, 0.001, 0.001);  // 0.001,0.001
	
	TPad *apad[16];	// risolvo il problema della label dell'asse X parzialmente mancante aggiungendo due pad fittizi solo per le label!
	float fr = 0.95; // frazione del canvas destinata ai plot
	float fa = 1.-fr; // frazione del canvas destinata alle label
	for (int i=0; i<nb; i++) {//7     <------ correggere qui con il num di pad parametrizzato
		TString pname("pad_");
		pname += i*2;
		apad[i*2]  = new TPad(pname, pname, 0.001, fa + fr*(nb-1-i)/((float) nb) + 0.001, 0.499, fa + fr*(nb-i)/((float) nb) - 0.001);	// sx pads
		apad[i*2]->Draw();
		TString pname1("pad_");
		pname1 += i*2+1;
		apad[i*2+1] = new TPad(pname1, pname1, 0.501, fa + fr*(nb-1-i)/((float) nb) + 0.001, 0.999, fa + fr*(nb-i)/((float) nb) - 0.001);	// dx pads
		apad[i*2+1]->Draw();
	}	
	apad[nb*2]  = new TPad("plabsx", "plabsx", 0.01, 0.001, 0.49, fa-0.001);
	apad[nb*2]->Draw();
	apad[nb*2+1]  = new TPad("plabdx", "plabdx", 0.51, 0.001, 0.99, fa-0.001);
	apad[nb*2+1]->Draw();
	
	int nrow = nb;// 3
	int ncol = 2;// 3
	for (int i=0; i<nrow; i++) {// 9
	
		// light curve pad
		
		// c1.cd(i*2+1);
		apad[i*2]->cd();
		gPad->SetTicks(1,1);
		
		/*TString ytitle("Counts / ");
		ytitle += tbin[i]*1000.;
		ytitle += " ms"; */
		
		char ytitle[30];
		sprintf(ytitle, "Counts / %3.1f ms", tbin[i]*1000.);
		
		
		hlc[i]->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		hlc[i]->GetXaxis()->SetNdivisions(506);
		hlc[i]->GetYaxis()->SetRangeUser(0., 10.); // 1.2*hlc[i]->GetMaximum()
		hlc[i]->GetYaxis()->SetNdivisions(502);
		hlc[i]->SetStats(0);
		hlc[i]->SetTitle("");
		hlc[i]->GetXaxis()->SetLabelSize(label_size);
		/*if (i==6) {
			hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
			hlc[i]->GetXaxis()->SetTitleSize(label_size);
			hlc[i]->GetXaxis()->SetTitleOffset(0.9);// title_offset
			hlc[i]->GetXaxis()->CenterTitle();
		}*/
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetYaxis()->CenterTitle();
		hlc[i]->Draw();
			
		TLatex t3;
		t3.SetTextSize(text_size);
		t3.DrawTextNDC(xpos, zpos, trg[i]);
			
		/*TLatex t1;
		t1.SetTextSize(text_size);
		TString str1("T0 = ");
		str1 += ts[i]->AsString("s");
		str1 += " UT";
		t1.DrawTextNDC(xpos, zpos-0.1, str1);*/
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.15, 0.2, label[i]);
		
		// E vs t plot pad
		
		// c1.cd(i*2+2);
		apad[i*2+1]->cd();
		
		gPad->SetTicks(1,1);
		gPad->SetLogy();
		TString hname("htmp");
		hname += i;
		TH2F *htmp = new TH2F(hname, "", 100, zbefore[i], zafter[i], 100, 0.1, 100.);
		htmp->GetXaxis()->SetNdivisions(506);
		htmp->SetStats(0);
		htmp->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		htmp->GetYaxis()->SetRangeUser(0.1, 60.);
		htmp->GetYaxis()->SetNoExponent();
		htmp->SetTitle("");
		htmp->GetXaxis()->SetLabelSize(label_size);
		/*if (i==6) {
			htmp->GetXaxis()->SetTitle("Time-T0 (s)");
			htmp->GetXaxis()->SetTitleSize(label_size);
			htmp->GetXaxis()->SetTitleOffset(0.9);// title_offset
			htmp->GetXaxis()->CenterTitle();
		}*/
		htmp->GetYaxis()->SetLabelSize(label_size);
		htmp->GetYaxis()->SetTitle("Energy (MeV)");
		htmp->GetYaxis()->SetTitleSize(label_size);
		htmp->GetYaxis()->SetTitleOffset(title_offset);
		htmp->GetYaxis()->CenterTitle();
		htmp->Draw("");
		gEt[i]->SetMarkerStyle(7);
		gEt[i]->Draw("P");
		
	}	
	
	// draw X labels in pads 14 and 15
	float xlabel_text_size = 0.5;//0.27
	apad[nb*2]->cd();
	TLatex *lsx = new TLatex();
	lsx->SetTextSize(xlabel_text_size);
	lsx->DrawTextNDC(0.45, 0.5,"Time - T0 (s)");
	
	apad[nb*2+1]->cd();
	TLatex *ldx = new TLatex();
	ldx->SetTextSize(xlabel_text_size);
	ldx->DrawTextNDC(0.45, 0.5,"Time - T0 (s)");

}
