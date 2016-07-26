{	
	// MM 17/06/10 minor change: print burst info in plot
	//             bug corrected in cumulative graph calculation
	// MM 09/06/10 implement error calculation
	// MM 03/06/10 implement T90 and T50 algorithm based on Koshut et al. (1996), ApJ 463, 570-592 


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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");

	// ------- input data (to be modified for analysis, do not change anything else!)
	
	int cont=33800;			// orbit, e.g.10007
	double t0=311028115.794896;	// T0 (AGILE terrestrial time),  e.g. 165659724.65
	double atbin = 0.1;		// temporal bin (s) for analysis,  e.g. 0.02
	double azbefore = -1.;		// background start time (s) with respect to T0,  e.g. -40
	double azafter  =  32.;		// background stop time (s) with respect to T0,  e.g. 20 
	double atgrbini = -0.1;		// GRB start time (s) with respect to T0,  e.g. -2
	double atgrbfin  = 25.;		// GRB stop time (s) with respect to T0,  e.g. 12
	TString funct("pol1");		// background fit function: polN = polinomial of grade N (pol0 = constant),  e.g. pol1

	// -------------------------------------------------
		
	TString *rootfilename;
	double t0;
	TTimeStamp ts;
	GRB3908 *grb;
	TH1D *hlc;
	TGraph *gEt;
	TF1 *fbkg = new TF1("fbkg", funct.Data(), azbefore, azafter);	
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	
	
	
	// work on 3908 data
		
	char aux[100];
	//sprintf(aux, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", cont, cont);
	sprintf(aux, "/home/mcal/data_analysis/grb/131108A/RT%06d_3908.root", cont, cont);
	rootfilename = new TString(aux);
	grb = new GRB3908(rootfilename->Data());
	grb->SetGRBBoundaries(azbefore, atgrbini, azbefore, azafter);
	grb->DefaultLightCurve();
	grb->CustomLightCurve(t0, azbefore, azafter, emin, emax, barsX, barsZ, atbin);
	hlc = grb->GetLightCurve();
	ts.Set((int)  t0, 0, epoch.GetSec(),0);

	
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
	// hlc->SetFillColor(2);
	hlc->Draw();
	
	
	// create background graph to fit
	
	int nb = hlc->GetNbinsX();
	int ib=0;
	TGraph *gbk = new TGraph(1);
	for (int i=0; i<nb; i++) {
		double xbin = hlc->GetBinLowEdge(i) + 0.5*atbin;
		if (xbin < atgrbini || xbin > atgrbfin) {
			gbk->SetPoint(ib++, xbin, hlc->GetBinContent(i));
		}
	}
	gbk->SetMarkerStyle(2);
	gbk->SetMarkerColor(2);
	gbk->Draw("PSAME");
	
	// fit background
	
	gbk->Fit("fbkg", "F0");
	fbkg->SetLineColor(3);
	fbkg->Draw("SAME");
	
	double chi2=0.;
	int n = gbk->GetN();
	double *x = gbk->GetX();
	double *y = gbk->GetY();
	
	for (int i=0; i< n; i++) {
		//cout << i << "  " << x[i] << "  " << y[i]  << "  " << fbkg->Eval(x[i]) << "  " << (y[i] - fbkg->Eval(x[i]))*(y[i] - fbkg->Eval(x[i])) / fbkg->Eval(x[i]) << endl;
		chi2 += (y[i] - fbkg->Eval(x[i]))*(y[i] - fbkg->Eval(x[i])) / fbkg->Eval(x[i]);
	}
	chi2 /= (n-1);
	cout << "reduced chi^2 = " << chi2 << endl;
	
	
	// create bkg subtracted cumulative histogram
	
	TGraph *gsum = new TGraph(1);
	float sum=0., ds;
	for (int i=1; i<=nb; i++) {
		ds = hlc->GetBinContent(i) - fbkg->Eval(hlc->GetBinLowEdge(i) + 0.5*atbin);
		sum += ds;
		gsum->SetPoint(i-1, hlc->GetBinLowEdge(i) + 0.5*atbin, sum);
		
		//cout << i-1  << "  " << hlc->GetBinLowEdge(i) + 0.5*atbin << "  " << sum << "  " << hlc->GetBinContent(i) << "  " << fbkg->Eval(hlc->GetBinLowEdge(i) + 0.5*atbin) << "  " << ds << endl;
	}
	
	// bkg subtracted cumulative plot
	
	c1.cd(2);
	gPad->SetTicks(1,1);
	gsum->SetTitle("");
	gsum->Draw("ALP");
	gsum->GetXaxis()->SetRangeUser(azbefore, azafter);
	gsum->GetXaxis()->SetTitle("Time-T0 (s)");
	gsum->GetXaxis()->CenterTitle();
	gsum->GetYaxis()->SetTitle("Counts");
	gsum->GetYaxis()->SetTitleSize(label_size);
	gsum->GetYaxis()->SetTitleOffset(title_offset);
	gsum->GetYaxis()->CenterTitle();
	
	float s0=0.;	// bkg-subtracted zero-fluence level, equivalent of Lz in Koshut et al. (1996)
	float s100=0.;	// bkg-subtracted total-fluence level, equivalent of Lt in Koshut et al. (1996)
	float dsf0=0.;	// variance of bkg-subtracted zero-fluence level, equivalent of (dLz)^2 in Koshut et al. (1996)
	float dsf100=0.;	// variance of bkg-subtracted total-fluence level, equivalent of (dLt)^2 in Koshut et al. (1996)
	double *xs = gsum->GetX();
	double *ys = gsum->GetY();
	int n0=0;
	int n100=0;
	for (int i=0; i<nb; i++) if (xs[i]<=atgrbini) {
		s0 += ys[i];
		dsf0 += (ys[i]*ys[i]);
		n0++;
	}	
	for (int i=0; i<nb; i++) if (xs[i]>=atgrbfin) {
		s100 += ys[i];
		dsf100 += (ys[i]*ys[i]);
		n100++;
	}
	s0 /= n0;
	s100 /= n100;
	dsf0 = dsf0/n0 - s0*s0;
	dsf100 = dsf100/n100 - s100*s100;
	
	float fluence = s100 - s0;
	float ferr = TMath::Sqrt(dsf0 + dsf100);
	cout << "zero level fluence  = " << s0 << "  +/- "   << TMath::Sqrt(dsf0)   << endl;
	cout << "total level fluence = " << s100 << "  +/- " << TMath::Sqrt(dsf100) << endl;
	cout << "tot fluence = " << fluence << "  +/- " << ferr << endl;
	
	TLine l0(azbefore, s0, azafter, s0);
	l0.SetLineColor(2);
	l0.SetLineStyle(2);
	l0->Draw();
	
	TLine l100(azbefore, s100, azafter, s100);
	l100.SetLineColor(2);
	l100.SetLineStyle(2);
	l100->Draw();
	
	double t5, t25, t75, t95;
	double s5, s25, s75, s95;
	int i5, i25, i75, i95;
	
	
	// calculate T90
	s5 = s0 + 0.05*fluence;
	for (int i=0; i<nb; i++) {
		if (xs[i]<atgrbini) continue;
		if (ys[i]> s5) break;
	}
	i5 = i-1;
	t5 = xs[i5];
	TLine l5(azbefore, s5, azafter, s5);
	l5.SetLineColor(4);
	l5.SetLineStyle(2);
	l5->Draw();
	
	s95 = s0 + 0.95*fluence;
	for (int i=0; i<nb; i++) {
		if (xs[i]<atgrbini) continue;
		if (ys[i]> s95) break;
	}
	i95 = i;
	t95 = xs[i95];
	TLine l95(azbefore, s95, azafter, s95);
	l95.SetLineColor(4);
	l95.SetLineStyle(2);
	l95->Draw();
	
	TLine lt5(t5, 0., t5, 1.1*s100);
	lt5.SetLineColor(4);
	lt5.SetLineStyle(2);
	lt5->Draw();
	
	TLine lt95(t95, 0., t95, 1.1*s100);
	lt95.SetLineColor(4);
	lt95.SetLineStyle(2);
	lt95->Draw();
	
	double t90 = t95 - t5;
	
	// calculate T50
	
	s25 = s0 + 0.25*fluence;
	for (int i=0; i<nb; i++) {
		if (xs[i]<atgrbini) continue;
		if (ys[i]> s25) break;
	}
	i25 = i-1;
	t25 = xs[i25];
	TLine l25(azbefore, s25, azafter, s25);
	l25.SetLineColor(3);
	l25.SetLineStyle(2);
	l25->Draw();
	
	s75 = s0 + 0.75*fluence;
	for (int i=0; i<nb; i++) {
		if (xs[i]<atgrbini) continue;
		if (ys[i]> s75) break;
	}
	i75 = i;
	t75 = xs[i75];
	TLine l75(azbefore, s75, azafter, s75);
	l75.SetLineColor(3);
	l75.SetLineStyle(2);
	l75->Draw();
	
	TLine lt25(t25, 0., t25, 1.1*s100);
	lt25.SetLineColor(3);
	lt25.SetLineStyle(2);
	lt25->Draw();
	
	TLine lt75(t75, 0., t75, 1.1*s100);
	lt75.SetLineColor(3);
	lt75.SetLineStyle(2);
	lt75->Draw();
	
	double t50 = t75 - t25;

	
	// calculate errors on T90 and T50
	
	int i0, i100;
	for (i0=0; i0<nb; i0++) {
		if (xs[i0]<atgrbini) continue;
		if (ys[i0]> s0) break;
	}
	i0--;
	for (i100=0; i100<nb; i100++) {
		if (ys[i100]> s100) break;
	}
	i100--;
	
	// calculates statistical contribution to errors, see eq.9 in Koshut et al. (1996)
	float dsc5=0., dsc25=0., dsc75=0., dsc95=0.;
	for (int i=i0; i<=i5; i++) dsc5 += hlc->GetBinContent(i); 
	for (int i=i0; i<=i25; i++) dsc25 += hlc->GetBinContent(i); 
	for (int i=i0; i<=i75; i++) dsc75 += hlc->GetBinContent(i); 
	for (int i=i0; i<=i95; i++) dsc95 += hlc->GetBinContent(i); 
	
	// calculates bkg fluctuation contribution to errors, see eq.12-14 in Koshut et al. (1996)
	float dsf5  = 0.95*0.95*dsf0 + 0.05*0.05*dsf100;
	float dsf95 = 0.05*0.05*dsf0 + 0.95*0.95*dsf100;
	float dsf25 = 0.75*0.75*dsf0 + 0.25*0.25*dsf100;
	float dsf75 = 0.25*0.25*dsf0 + 0.75*0.75*dsf100;
	
	
	// calculate total errors on fluence (statistical + bkg fluctuation), see eq.8 in Koshut et al. (1996)
	float ds5  = TMath::Sqrt(dsf5  + dsc5);
	float ds95 = TMath::Sqrt(dsf95 + dsc95);
	float ds25 = TMath::Sqrt(dsf25 + dsc25);
	float ds75 = TMath::Sqrt(dsf75 + dsc75);
	cout << " 5% level fluence  = " << s5  << "  +/- "   << TMath::Sqrt(dsc5)  << " (cnt) " << TMath::Sqrt(dsf5)  << " (fluc) "  << ds5  << " (tot) "  << endl;
	cout << "25% level fluence  = " << s25 << "  +/- "   << TMath::Sqrt(dsc25)  << " (cnt) " << TMath::Sqrt(dsf25)  << " (fluc) "  << ds25  << " (tot) "  << endl;
	cout << "75% level fluence  = " << s75 << "  +/- "   << TMath::Sqrt(dsc75)  << " (cnt) " << TMath::Sqrt(dsf75)  << " (fluc) "  << ds75  << " (tot) "  << endl;
	cout << "95% level fluence  = " << s95 << "  +/- "  << TMath::Sqrt(dsc95) << " (cnt) " << TMath::Sqrt(dsf95) << " (fluc) "  << ds95 << " (tot) "  << endl;
	
	
	
	// calculate total errors on durations, see eq.15-16 in Koshut et al. (1996)
	
	// error on T90
	int i5m, i5p;
	for (i5m=0; i5m<nb; i5m++) {
		if (xs[i5m]<atgrbini) continue;
		if (ys[i5m]> s5 - ds5) break;
	}
	i5m--;
	for (i5p=0; i5p<nb; i5p++) {
		if (xs[i5p]<atgrbini) continue;
		if (ys[i5p]> s5 + ds5) break;
	}
	float dt5 = xs[i5p] - xs[i5m];
	
	int i95m, i95p;
	for (i95m=0; i95m<nb; i95m++) {
		if (xs[i95m]<atgrbini) continue;
		if (ys[i95m]> s95 - ds95) break;
	}
	i95m--;
	for (i95p=0; i95p<nb; i95p++) {
		if (xs[i95p]<atgrbini) continue;
		if (ys[i95p]> s95 + ds95) break;
	}
	float dt95 = xs[i95p] - xs[i95m];
	float dt90 = TMath::Sqrt(dt5*dt5 + dt95*dt95);
	cout << endl;
	cout << "t5 = " << t5 << "  +/- "   << dt5  << "  t5+ = " << xs[i5p]  << "  t5- = "  <<  xs[i5m]  << endl;
	cout << "t95 = " << t95 << "  +/- "   << dt95  << "  t95+ = " << xs[i95p]  << "  t95- = "  <<  xs[i95m]  << endl;
	cout << "T90 (s) = " << t90 << "  +/- "   << dt90 << endl;
	
	// error on T50
	int i25m, i25p;
	for (i25m=0; i25m<nb; i25m++) {
		if (xs[i25m]<atgrbini) continue;
		if (ys[i25m]> s25 - ds25) break;
	}
	i25m--;
	for (i25p=0; i25p<nb; i25p++) {
		if (xs[i25p]<atgrbini) continue;
		if (ys[i25p]> s25 + ds25) break;
	}
	float dt25 = xs[i25p] - xs[i25m];
	
	int i75m, i75p;
	for (i75m=0; i75m<nb; i75m++) {
		if (xs[i75m]<atgrbini) continue;
		if (ys[i75m]> s75 - ds75) break;
	}
	i75m--;
	for (i75p=0; i75p<nb; i75p++) {
		if (xs[i75p]<atgrbini) continue;
		if (ys[i75p]> s75 + ds75) break;
	}
	float dt75 = xs[i75p] - xs[i75m];
	float dt50 = TMath::Sqrt(dt25*dt25 + dt75*dt75);
	cout << endl;
	cout << "t25 = " << t25 << "  +/- "   << dt25  << "  t25+ = " << xs[i25p]  << "  t25- = "  <<  xs[i25m]  << endl;
	cout << "t75 = " << t75 << "  +/- "   << dt75  << "  t95+ = " << xs[i75p]  << "  t75- = "  <<  xs[i75m]  << endl;
	cout << "T50 (s) = " << t50 << "  +/- "   << dt50 << endl;
	
	// print results with spreadsheet format
	cout << endl;
	printf("%06d \t%20.6f \t%6.0f \t%6.0f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%6.3f \t%s \t%6.3f\n", cont, t0, fluence, ferr, t90, dt90, t50, dt50, atbin, azbefore, azafter, atgrbini, atgrbfin, funct.Data(), chi2);
	
	
	// add contact info to canvas
		
	double label_size   = 0.06;	// 0.07
	double text_size    = 0.06;	// 0.07
	double title_offset = 0.8;	// 0.6 ma occorre sistemare i boundary dei plot di dx, che a mano non si riesce!
	double xpos = 0.15;
	double zpos = 0.8;
		
	c1.cd(1);
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
	t1.DrawTextNDC(xpos, zpos-0.08, str1);
	
	TLatex t2;
	t2.SetTextSize(text_size);
	char str2[200];	// T0 = 
	sprintf(str2,"TT %.6f", t0);
//	t2.DrawTextNDC(xpos, zpos-0.16, str2);
		
	c1.cd(2);
	TLatex t4;
	t4.SetTextSize(text_size);
	char str4[200];
	sprintf(str4,"fluence = %.0f +/- %.0f counts", fluence, ferr);
	t4.DrawTextNDC(xpos, zpos, str4);
		
	TLatex t7;
	t7.SetTextSize(text_size);
	char str7[200];
	sprintf(str7,"T90 = %.3f +/- %.3f s", t90, dt90);
	t7.DrawTextNDC(xpos, zpos-0.08, str7);
	
	TLatex t6;
	t6.SetTextSize(text_size);
	char str6[200];	// T0 = 
	sprintf(str6,"T50 = %.3f +/- %.3f s", t50, dt50);
	t6.DrawTextNDC(xpos, zpos-0.16, str6);

		
	// save a gif file

	char cname[200];
	sprintf(cname, "./%06d_%08d_%06d.gif", cont, ts.GetDate(), ts.GetTime());
	c1.Print(cname);

}
