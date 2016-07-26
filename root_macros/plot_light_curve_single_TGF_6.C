int plot_light_curve_single_TGF_6(int cont = 26226, double t0=264863788.626369, double dt=0.001164, double atbin = 0.0001, char *outpath = "./")
{	
	// MM 25/10/12 parameterized also bin duration and output path

	// MM 17/10/12 added duration calculation by gaussian fit
	// MM 18/09/12 includes simplified T50 calculations
	// MM 15/03/11 non parameterized version, same formatting as v4. why v4 closes at the end, while v3 does not?

	// MM 21/04/10 plot with some color for presentation & animated gif creation. Do not print GRID event

	// MM 15/02/10 parameterized; add GRID events, if any
	//             cont	contact number
	//             t0	TGF time0
	//             tgX, eX	t0 and energy for GRID evt X
	
	// MM 28/10/09 plots a single light curve for a TGF with energy vs. time scatter plot
	// based on plot_light_curve_multiple_TGF_paper.C

	//gROOT->Reset();
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

//	int cont=9672;
//	double t0=163608362.205007;
//	double dt=0.000414;

/*	double tg1=182810695.149599;
	double e1=34.;
	double tg2=182810695.149863;
	double e2=50;
	double tg3=-1.;
	double e3=-1.;
*/	
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
	
	//double atbin = 0.0001;
	double azbefore = -0.006; // -0.006
	double azafter  =  0.006; //  0.006
	double atbkgini = -2.9;
	double atbkgfin  =  0.0;
	double tstart = -3.;
	double tstop  =  3.;
	
	// brightest
	char aux[100];
	sprintf(aux, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", cont, cont);
	rootfilename = new TString(aux);
	// t0 =  171889319.048464; 
	
	
	// build light curves (load libMCV.so before)
	
	// work on 3908 data
		
	grb = new GRB3908(rootfilename->Data());
	grb->SetGRBBoundaries(atbkgini, atbkgfin, azbefore, azafter);
	grb->DefaultLightCurve();
	grb->CustomLightCurve(t0, tstart, tstop, emin, emax, barsX, barsZ, atbin);
	hlc = grb->GetLightCurve();
	// ctpeak[i] = grb[i]->GetTPeak();
	ts.Set((int)  t0, 0, epoch.GetSec(),0);
	grb->VSBDiagnostic();
	gEt = grb->GetEvsT();

	// calculates background
	
	int binstart1 = (int) ((-0.510 - tstart)/atbin);
	int binstop1  = (int) ((-0.010 - tstart)/atbin);
	int binstart2 = (int) ((0.010 - tstart)/atbin);
	int binstop2  = (int) ((0.510 - tstart)/atbin);
	double bkg = hlc->Integral(binstart1, binstop1) + hlc->Integral(binstart2, binstop2);
	//printf("%6d  %6d  %6d  %6d  %6.0f\n", binstart1, binstop1, binstart2, binstop2, bkg);
	
	// graphic section
	
	printf("%06d \t%20.6f \t%s\n", grb->GetContact(), t0, ts.AsString("s"));
	
	
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
	hlc->SetFillColor(2);
	hlc->Draw();
		
	// find duration by Gaussian fit
	TF1 *tgf = new TF1("tgf","gaus", -0.0005,  dt + 0.0005); // (int) 0.1
	tgf->SetParameter(0, hlc->GetMaximum());
	tgf->SetParameter(1, 0.5*dt);
	tgf->SetParameter(2, 0.5*dt);
	cout << "Starting par: " << tgf->GetParameter(0) << "  " << tgf->GetParameter(1) << "  " << tgf->GetParameter(2) << endl; 
	tgf->SetLineWidth(2);
	tgf->SetLineColor(4);
	hlc->Fit("tgf","R+");
	double chi2 = tgf->GetChisquare();
	double mean = tgf->GetParameter(1);
	double meanerr = tgf->GetParError(1);
	double sigma = tgf->GetParameter(2);
	double sigmaerr = tgf->GetParError(2);
	
	TLine l0(0., 0., 0., 1.2*hlc->GetMaximum());
	TLine l1(dt, 0., dt, 1.2*hlc->GetMaximum());
	l0.SetLineStyle(2);
	l1.SetLineStyle(2);
	l0.Draw();
	l1.Draw();
	
	// plot GRID evt (line), if any
	
/*	if (tg1 > 0.) {
		TLine l1(tg1-t0, 0, tg1-t0, 1.2*hlc->GetMaximum());
		l1.SetLineColor(2);
		l1.Draw();
	}
	
	if (tg2 > 0.) {
		TLine l2(tg2-t0, 0, tg2-t0, 1.2*hlc->GetMaximum());
		l2.SetLineColor(2);
		l2.Draw();
	}
*/		
		
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
	
	// E vs t plot pad
	
	c1.cd(2);
	gPad->SetTicks(1,1);
	gPad->SetLogy();
	TString hname("htmp");
	double emax = 120;
/*	if (e1 > emax) emax = e1;
	if (e2 > emax) emax = e2;
	if (e3 > emax) emax = e3;
*/	
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
	htmp->GetYaxis()->CenterTitle();
	htmp->Draw("");
	gEt->SetMarkerStyle(3);
	gEt->SetMarkerColor(4);
	gEt->Draw("P");
	
	TLine l2(0., 0., 0., 150.);
	TLine l3(dt, 0., dt, 150.);
	l2.SetLineStyle(2);
	l3.SetLineStyle(2);
	l2.Draw();
	l3.Draw();
	
	// plot GRID evt (marker), if any
	
/*	if (e1 > 0.) {
		TMarker m1(tg1-t0, e1, 2);
		m1.SetMarkerColor(2);
		m1.Draw();
	}
	
	if (e2 > 0.) {
		TMarker m2(tg2-t0, e2, 2);
		m2.SetMarkerColor(2);
		m2.Draw();
	}
*/	
	// save a gif file

	char cname[200];
//	sprintf(cname, "/home/mcal/data_analysis/VSB/tgf_candidates_complete/light_curves/gif/%06d_%08d_%06d.gif", cont, ts.GetDate(), ts.GetTime());
//	sprintf(cname, "/home/mcal/data_analysis/VSB/tgf_candidates_complete/highest_energy/%06d_%08d_%06d.gif", cont, ts.GetDate(), ts.GetTime());

	sprintf(cname, "%s/%06d_%08d_%06d_%03dus.pdf", outpath, cont, ts.GetDate(), ts.GetTime(), (int) (atbin*1.e6));
	c1.Print(cname);

	// calculates T50 (almost like Koshut paper)
	int n = gEt->GetN();
	float *at = gEt->GetX();
	int n25 = (int) n*0.25;
	int n75 = (int) n*0.75;
	double t50 = at[n75-1] -at[n25];

	// print out timing results
	printf("TIMING %6d  %20.6f  %.6f  %6.2f  %.6f  %.6f  %.6f  %.6f  %.6f  %.6f  %.6f  %.0f\n", cont, t0, dt, chi2, mean, meanerr, sigma, sigmaerr, t50, TMath::Mean(n, at), TMath::RMS(n, at), bkg);

		
return 0;

}
