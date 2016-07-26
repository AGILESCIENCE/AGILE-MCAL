// plot_light_curve_single_TGF_3
{	
	// MM 15/04/10 modified for paper
	//             work on contact 12809. not parameterized

	// MM 15/02/10 parameterized; add GRID events, if any
	//             cont	contact number
	//             t0	TGF time0
	//             tgX, eX	t0 and energy for GRID evt X
	
	// MM 28/10/09 plots a single light curve for a TGF with energy vs. time scatter plot
	// based on plot_light_curve_multiple_TGF_paper.C

	//gROOT->Reset();
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

	int cont=12809;
	double t0=182810695.148596;

	double tg1=182810695.149599;
	double e1=36.;
	double tg2=182810695.149863;
	double e2=47.;
	double tg3=-1.;
	double e3=-1.;
		
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
	gStyle->SetPaperSize(8.6, 4.3);

	TString *rootfilename;
	double t0;
	TTimeStamp ts;
	GRB3908 *grb;
	TH1D *hlc;
	TGraph *gEt;
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	
	// input data
	
	double atbin = 0.0002;
	double azbefore = -0.003;
	double azafter  =  0.004;
	double atbkgini = -2.9;
	double atbkgfin  =  0.0;
	
	// brightest
	char aux[100];
	sprintf(aux, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", cont, cont);
	rootfilename = new TString(aux);
	// t0 =  171889319.048464; 
	
	
	// build light curves (load libMCV.so before)
	
	// work on 3908 data
		
	grb = new GRB3908(aux);	//rootfilename->Data()
	grb->SetGRBBoundaries(atbkgini, atbkgfin, azbefore, azafter);
	grb->DefaultLightCurve();
	grb->CustomLightCurve(t0, -3., +3., emin, emax, barsX, barsZ, atbin);
	hlc = grb->GetLightCurve();
	// ctpeak[i] = grb[i]->GetTPeak();
	ts.Set((int)  t0, 0, epoch.GetSec(),0);
	grb->VSBDiagnostic();
	gEt = grb->GetEvsT();

	// graphic section
	
	printf("%06d \t%20.6f \t%s\n", grb->GetContact(), t0, ts.AsString("s"));
	
	
	double label_size   = 0.06;	// 0.07
	double text_size    = 0.07;	// 0.07
	double title_offset = 0.8;	// 0.6 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 800, 400);//800,1000
	c1.Divide(1,2, 0.001, 0.001);  // 2,7
	
	// light curve pad
		
	c1.cd(1);
	gPad->SetTicks(1,1);
	
	TString ytitle("Counts / ");
	ytitle += atbin*1000.;
	ytitle += " ms";
	
	hlc->GetXaxis()->SetRangeUser(azbefore, azafter);
	hlc->GetYaxis()->SetRangeUser(0., 1.2*hlc->GetMaximum());
	hlc->SetStats(0);
	hlc->SetTitle("");
	hlc->GetXaxis()->SetLabelSize(label_size);
	hlc->GetXaxis()->SetTitle("Time-T0 (s)");
	hlc->GetXaxis()->SetTitleSize(label_size);
	hlc->GetXaxis()->SetTitleOffset(title_offset);
	hlc->GetXaxis()->CenterTitle();
	hlc->GetYaxis()->SetLabelSize(label_size);
	hlc->GetYaxis()->SetTitle(ytitle);
	hlc->GetYaxis()->SetTitleSize(label_size);
	hlc->GetYaxis()->SetTitleOffset(title_offset);
	hlc->GetYaxis()->CenterTitle();
	hlc->Draw();
	
	// plot GRID evt (line), if any
	
	if (tg1 > 0.) {
		TLine l1(tg1-t0, 0, tg1-t0, 1.2*hlc->GetMaximum());
		l1.SetLineColor(2);
		l1.Draw();
	}
	
	if (tg2 > 0.) {
		TLine l2(tg2-t0, 0, tg2-t0, 1.2*hlc->GetMaximum());
		l2.SetLineColor(2);
		l2.Draw();
	}
	
	/*
	// draw the inset zoomed on evt 12809
	
	 inpad = new TPad("inpad", "inpad", 0.3, 0.6, 0.5, 0.8)
	  inpad->Draw()
	*/ 
		
/*		
	TLatex t3;
	t3.SetTextSize(text_size);
	TString str3("contact ");
	str3 += cont;
	t3.DrawTextNDC(xpos, zpos, str3);
		
	TLatex t1;
	t1.SetTextSize(text_size);
	TString str1("");	// T0 = 
	str1 += ts.AsString("s");
	str1 += " UT";
	t1.DrawTextNDC(xpos, zpos-0.1, str1);
	
	TLatex t2;
	t2.SetTextSize(text_size);
	char str2[200];	// T0 = 
	sprintf(str2,"TT %.6f", t0);
	t1.DrawTextNDC(xpos, zpos-0.2, str2);
*/	
	// E vs t plot pad
	
	c1.cd(2);
	gPad->SetTicks(1,1);
	gPad->SetLogy();
	TString hname("htmp");
	double emax = 100;
	if (e1 > emax) emax = e1;
	if (e2 > emax) emax = e2;
	if (e3 > emax) emax = e3;
	
	TH2F *htmp = new TH2F(hname, "", 100, azbefore, azafter, 100, 0.1, 1.2*emax);
	htmp->SetStats(0);
	htmp->GetXaxis()->SetRangeUser(azbefore, azafter);
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
	htmp->GetYaxis()->SetNoExponent(1);
	htmp->GetYaxis()->CenterTitle();
	htmp->Draw("");
	gEt->SetMarkerStyle(7);
	gEt->Draw("P");
	
	// plot GRID evt (marker), if any
	
	if (e1 > 0.) {
		TMarker m1(tg1-t0, e1, 2);
		m1.SetMarkerColor(2);
		m1.Draw();
	}
	
	if (e2 > 0.) {
		TMarker m2(tg2-t0, e2, 2);
		m2.SetMarkerColor(2);
		m2.Draw();
	}
	
	// save a gif file

	/*char cname[200];
	sprintf(cname, "/home/mcal/data_analysis/VSB/grid/plot/light_curve/%06d_%08d_%06d.gif", cont, ts.GetDate(), ts.GetTime());
	c1.Print(cname);
	*/
	
}
