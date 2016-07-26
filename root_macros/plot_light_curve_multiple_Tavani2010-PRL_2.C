{
	// MM 18/10/10 plot lc + spectra per 3 TGF brillanti,
	// per sostituire fig.1 paper Tavani et al. 2010, PRL

	gROOT->Reset();
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
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
	TString *spfilename[20];
	double t0[20];
	double tbin[20];
	TTimeStamp ts[20];
	GRB3908 *grb[20];
	TH1D *hlc[20];
	TGraph *gEt[20];
	TGraphErrors *gsp[20];	// spettro
	TGraph *gmod[20];	// modello
	double zbefore[20];
	double zafter[20];
	double ctpeak[20];
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	char *label[] = {"(a)", "(b)", "(c)", "(d)", "(e)", "(f)", "(g)", "(h)", "(k)", "(j)", "(i)"};
	char *trg[] = {"TGF 11026-1", "TGF 13228-1", "TGF 10250-9"};
	
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
	
/*	// highest energy #2
	rootfilename[0] = new TString("/MCAL_PROC/ARCHIVE/010035/RT010035_3908.root");
	t0[0] =  165830024.91324; 
	
	// highest energy #3
	rootfilename[1] = new TString("/MCAL_PROC/ARCHIVE/012731/RT012731_3908.root");
	t0[1] = 182330290.226936;
	
	// highest energy #4, bright #2
	rootfilename[2] = new TString("/MCAL_PROC/ARCHIVE/013228/RT013228_3908.root");
	t0[2] = 185372190.863548; 
*/	
	
	// highest energy #10, bright #1
	rootfilename[0] = new TString("/MCAL_PROC/ARCHIVE/011026/RT011026_3908.root");
	t0[0] =  171889319.048464; 
	spfilename[0] = new TString("/home/mcal/data_analysis/VSB/tgf_candidates_complete/011026-1/011026g_bknpo_fixed.dat");
	
	// highest energy #4, bright #2
	rootfilename[1] = new TString("/MCAL_PROC/ARCHIVE/013228/RT013228_3908.root");
	t0[1] = 185372190.863548; 
	spfilename[1] = new TString("/home/mcal/data_analysis/VSB/tgf_candidates_complete/013228-1/013228b_bknpwrl_fixed.dat");
	
	// highest energy #6, bright #3
	rootfilename[2] = new TString("/MCAL_PROC/ARCHIVE/010250/RT010250_3908.root");
	t0[2] =  167150762.697472; 
	spfilename[2] = new TString("/home/mcal/data_analysis/VSB/tgf_candidates_complete/010250-9/010250a_bknpwrl_fixed.dat");
	
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
	
	// work on spectrum
	float x, dx, y, dy, m, dm, aux;
	for (int i=0; i<nb; i++) {//7
		gsp[i] = new TGraphErrors(10);
		gmod[i] = new TGraph(1);
		ifstream fin;
		fin.open(spfilename[i].Data());
		int npt=0;
		while (fin >> x >> dx >> y >> dy >> m >> dm >> aux) {
			cout << x << "  " << dx << "  " << y << "  " << dy << endl;
			gsp[i]->SetPoint(npt, x*0.001, y*1000.);
			gsp[i]->SetPointError(npt, dx*0.001, dy*1000.);
			gmod[i]->SetPoint(npt, x*0.001, m*1000.);
			npt++;
		}
		fin.close();
	
	}

	// graphic section
	
	for (int i=0; i<nb; i++) {//7
		printf("%06d \t%20.6f \t%s\n", grb[i]->GetContact(), t0[i], ts[i]->AsString("s"));
	}
	
	
	double label_size   = 0.08;	// 0.07
	double text_size    = 0.08;	// 0.07
	double title_offset = 1.1;	// 0.45 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.7;
	
	TCanvas c1("c1", "", 1000, 600);// 800, 1000
	c1.Divide(3,3, 0.01, 0.01);  // 0.001,0.001
	
/*	TPad *apad[9];	// risolvo il problema della label dell'asse X parzialmente mancante aggiungendo due pad fittizi solo per le label!
	float fr = 0.9; // frazione del canvas destinata ai plot
	float fa = 1.-fr; // frazione del canvas destinata alle label
	float th = 0.333;
	
	apad[0]  = new TPad("p0", "p0", 0.01, 0.66, 0.33, 0.99);
	apad[0]->Draw();
	apad[1]  = new TPad("p1", "p1", 0.34, 0.66, 0.66, 0.99);
	apad[1]->Draw();
	apad[2]  = new TPad("p2", "p2", 0.67, 0.66, 0.99, 0.99);
	apad[2]->Draw();
	
	apad[3]  = new TPad("p3", "p3", 0.01, 0.33, 0.33, 0.66);
	apad[3]->Draw();
	apad[4]  = new TPad("p4", "p4", 0.34, 0.33, 0.66, 0.66);
	apad[4]->Draw();
	apad[5]  = new TPad("p5", "p5", 0.67, 0.33, 0.99, 0.66);
	apad[5]->Draw();
	
	apad[6]  = new TPad("p6", "p6", 0.01, 0.05, 0.33, 0.33);
	apad[6]->Draw();
	apad[7]  = new TPad("p7", "p7", 0.34, 0.05, 0.66, 0.33);
	apad[7]->Draw();
	apad[8]  = new TPad("p8", "p8", 0.67, 0.05, 0.99, 0.33);
	apad[8]->Draw();

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
*/	
	int nrow = 3;// 3
	int ncol = nb;// 3
	for (int i=0; i<ncol; i++) {// 9
	
		// light curve pad
		
		c1.cd(i+1);
		//apad[i]->cd();
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
		hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
		hlc[i]->GetXaxis()->SetTitleSize(label_size);
		hlc[i]->GetXaxis()->SetTitleOffset(0.9);// title_offset
		hlc[i]->GetXaxis()->CenterTitle();
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(1.);
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
		t1.DrawTextNDC(xpos, zpos-0.1, str1);
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.15, 0.2, label[i]); */
		
		// E vs t plot pad
		
		c1.cd(i + ncol +1);
		//apad[i+ncol]->cd();
		
		gPad->SetTicks(1,1);
		//gPad->SetLogy();
		TString hname("htmp");
		hname += i;
		TH2F *htmp = new TH2F(hname, "", 100, zbefore[i], zafter[i], 2100, -10., 200.);	// 0.1
		htmp->GetXaxis()->SetNdivisions(506);
		htmp->SetStats(0);
		//htmp->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		htmp->GetYaxis()->SetRangeUser(0.1, 200.);
		htmp->GetYaxis()->SetNoExponent();
		htmp->SetTitle("");
		htmp->GetXaxis()->SetLabelSize(label_size);
		htmp->GetXaxis()->SetTitle("Time-T0 (s)");
		htmp->GetXaxis()->SetTitleSize(label_size);
		htmp->GetXaxis()->SetTitleOffset(0.9);// title_offset
		htmp->GetXaxis()->CenterTitle();
		htmp->GetYaxis()->SetLabelSize(label_size);
		htmp->GetYaxis()->SetTitle("Energy (MeV)");
		htmp->GetYaxis()->SetTitleSize(label_size);
		htmp->GetYaxis()->SetTitleOffset(1.);
		htmp->GetYaxis()->CenterTitle();
		htmp->Draw("");
		gEt[i]->SetMarkerStyle(7);
		gEt[i]->Draw("P");
		
		// plot spectrum
		c1.cd(i + 2*ncol +1);
		//apad[i+2*ncol]->cd();
		gPad->SetLogx();
		gPad->SetLogy();
		gPad->SetTicks(1,1);
		TString hname2("htmp2");
		hname2 += i;
		TH2F *htmp2 = new TH2F(hname2, "", 100, 0.2, 500., 10010, -10., 10000.);	// 0.1
		htmp2->GetXaxis()->SetNdivisions(506);
		htmp2->SetStats(0);
		//htmp2->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		//htmp2->GetYaxis()->SetRangeUser(1., 10000.);
		//htmp2->GetYaxis()->SetNoExponent();
		htmp2->GetXaxis()->SetRangeUser(0.2, 100.);
		//htmp2->GetXaxis()->SetNoExponent();
		htmp2->SetTitle("");
		htmp2->GetXaxis()->SetLabelSize(label_size);
		htmp2->GetXaxis()->SetTitle("Energy (MeV)");
		htmp2->GetXaxis()->SetTitleSize(label_size);
		htmp2->GetXaxis()->SetTitleOffset(title_offset);// title_offset
		htmp2->GetXaxis()->CenterTitle();
		htmp2->GetYaxis()->SetLabelSize(label_size);
		htmp2->GetYaxis()->SetTitle("counts MeV^{-1} s^{-1}");
		htmp2->GetYaxis()->SetTitleSize(label_size);
		htmp2->GetYaxis()->SetTitleOffset(1.);
		htmp2->GetYaxis()->CenterTitle();
		htmp2->Draw("");
		
		gsp[i]->Draw("P");
		gmod[i]->SetLineColor(4);
		gmod[i]->SetLineWidth(2);
		gmod[i]->Draw("LSAME");
	}	
	
/*	// draw X labels in pads 14 and 15
	float xlabel_text_size = 0.5;//0.27
	//apad[nb*2]->cd();
	TLatex *lsx = new TLatex();
	lsx->SetTextSize(xlabel_text_size);
	lsx->DrawTextNDC(0.45, 0.5,"Time - T0 (s)");
	
	//apad[nb*2+1]->cd();
	TLatex *ldx = new TLatex();
	ldx->SetTextSize(xlabel_text_size);
	ldx->DrawTextNDC(0.45, 0.5,"Time - T0 (s)");
*/
}
