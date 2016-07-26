//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//


//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "GRB.h"

ClassImp(GRB);



GRB::GRB() : fi(NULL), info(NULL), hclc(NULL)
{
	// NB: this constructor MUST be implemented even is useless, otherwise you will get 
	// /home/martino/mcalsw/lib/libMCV.so: undefined reference to `GRB::GRB[in-charge]()'
	
	// default initialization
	nevents = 0;
	epoch = new TTimeStamp(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
		
	// set default style
	mcalstyle = new TStyle("mcalstyle","grbplot default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.06, "xyz");
	mcalstyle->SetTitleSize(0.06, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();

	emin = 0.;
	emax = 2000.;
	tbkgini = -40.;
	tbkgfin = -10.;
	tgrbini = 0.;
	tgrbfin = +10.;
	alertcode = 20;
	burstid = 0;
	t90 = 10.;
	
	// default spectral boundaries (Sci. RM like)
	nEbins = 4;
	double stdEbin[] = {0., 0.7, 1.4, 2.8, 1433.6};
	mcalEbin = new double[nEbins+1];
	for (int i=0; i<nEbins+1; i++) mcalEbin[i] = stdEbin[i];
	bkg = new double[nEbins];
	ebkg = new double[nEbins];
	fpeak = new double[nEbins];
	efpeak = new double[nEbins];
	tpeak = new double[nEbins];
	binpeak = new int[nEbins];
	fluence = new double[nEbins];
	efluence = new double[nEbins];
	pha = new float[nEbins];
	for (int i=0; i<nEbins; i++) bkg[i] = fpeak[i] = tpeak[i] = fluence[i] = 0.;
	cbkg = 0.;
	cfpeak = cfluence = 0.;
	i90hi = i90lo = 0;
	thr = 0.500;
	avgE = 0.;
	maxE = 0.;
	i0 = 1;
	tsearch = 30.;		// tserach setting good for SRM and/or macroscopic GRB; to be set shorter for VSB
	ctpeak = 0.;

}

//
//--------------------------------------------------------------------
//

GRB::~GRB()
{
	fi->Close();
}

//
//--------------------------------------------------------------------
//

void GRB::DrawLightCurve(TPad *cCustom, double ti, double tf)
{
	// plot custom light curves 
//	cCustom = new TCanvas("cCustom", "AGILE-MCAL light curve", 3000/(nEbins + 1), 1000);
	cCustom->Divide(1,nEbins + 1);	// YOU MUST CREATE A PAD OTHERWISE IT WILL CRASH WHEN PRINTING IN BATCH MODE!!!
	cCustom->cd(1);
	
	
			// default plot, suitable for screen and mail attach
	hclc->SetStats(0);
	hclc->GetXaxis()->SetTitle("t - t0 (s)");
	hclc->GetXaxis()->SetTitleOffset(0.7);	// 0.7
	hclc->GetXaxis()->CenterTitle();
	TString ytitle("counts / ");
	ytitle += tbin*1000;
	ytitle += " ms";
	hclc->GetYaxis()->SetTitle(ytitle);
	hclc->GetYaxis()->CenterTitle();
	
	hclc->GetYaxis()->SetTitleOffset(0.7);	// 0.7
	hclc->GetXaxis()->SetTitleOffset(0.85);
//	hclc->SetTitle("AGILE-MCAL light curve");
	hclc->SetTitle("");
	
	if (ti!=0. || tf!=0.) hclc->GetXaxis()->SetRangeUser(ti, tf);
	hclc->Draw();
	if (cbkg > 0.) {	// draw background dashed line
		TLine *l;
		l = new TLine(ti, cbkg, tf, cbkg);
		l->SetLineStyle(2);
		l->Draw();
	}
	
	
	if (t0) {	// draw text labels only if selection is done
		TLatex tl;
		tl.SetTextSize(0.07);	//0.07
		TString str1("T0 = ");
		str1 += tzero->AsString("s");
		str1 += " UT";
		tl.DrawTextNDC(0.15, 0.80, str1);
		
		char str2[100];
		sprintf (str2, "%.3f TT", t0);
		tl.DrawTextNDC(0.15, 0.73, str2);
		
		char str5[100];
		sprintf (str5, "Orbit: %d", info->runid);
		tl.DrawTextNDC(0.15, 0.66, str5);
		
		TString str3("Energy range: ");
		str3 += emin;
		str3 += " - ";
		str3 += emax;
		str3 += " MeV";
		tl.DrawTextNDC(0.15, 0.59, str3);
		
		char str4[100];
		sprintf(str4, "Selected bars: %04X %04X", barsX, barsZ);
		tl.DrawTextNDC(0.15, 0.52, str4);
	
		TString str("MCAL Team - INAF/IASF-Bologna");
		tl.DrawTextNDC(0.15, 0.45, str);
		
		// draw T90 boundaries, if calculated
		if (i90lo && i90hi) {
			TLine *l1, *l2;
			l1 = new TLine(hclc->GetBinLowEdge(i90lo), 0, hclc->GetBinLowEdge(i90lo), 1.2*(cfpeak + cbkg));
			l1->SetLineStyle(2);
			l1->Draw();			
			l2 = new TLine(hclc->GetBinLowEdge(i90hi), 0, hclc->GetBinLowEdge(i90hi), 1.2*(cfpeak + cbkg));
			l2->SetLineStyle(2);
			l2->Draw();			
		}
	}
	
	// draw light curves in different energy bands
	TLatex t2;
	t2.SetTextSize(0.07);	// 0.07
	for (int i=0; i<nEbins; i++) {
		cCustom->cd(i+2);
		TString pname("hE");
		pname += i+1;
		pname += "_";
		pname += burstid;
		TH1D *haux = h2clc->ProjectionX(pname.Data(), i+1, i+1, "");
		haux->SetStats(0);
		char stitle[100];
		sprintf(stitle, "%.2f - %.2f MeV", mcalEbin[i], mcalEbin[i+1]);
		TString htitle(stitle);
		haux->SetTitle("");
		if (ti!=0. || tf!=0.) haux->GetXaxis()->SetRangeUser(ti, tf);
		haux->GetXaxis()->SetTitle("t - t0 (s)");
		haux->GetXaxis()->CenterTitle();
		haux->GetYaxis()->SetTitle(ytitle);
		haux->GetYaxis()->SetTitleOffset(0.7);
		haux->GetXaxis()->SetTitleOffset(0.85);
		haux->GetYaxis()->CenterTitle();
		haux->Draw();
		t2.DrawTextNDC(0.15, 0.7, stitle);
		
		// draw background line, if calculated
		if (bkg[i] > 0.) {
			TLine *l;
			l = new TLine(ti, bkg[i], tf, bkg[i]);
			l->SetLineStyle(2);
			l->Draw();
		}
	}
	
	
	// to uncomment (comment the previous one) for TGF publication
	/*
	float lsize = 0.11;	
	hclc->SetStats(0);
	hclc->GetXaxis()->SetTitle("t - t0 (s)");
	hclc->GetXaxis()->SetTitleSize(lsize);
	hclc->GetXaxis()->SetTitleOffset(0.4);	// 0.7
	hclc->GetXaxis()->CenterTitle();
	hclc->GetXaxis()->SetLabelSize(lsize);
	TString ytitle("counts / ");
	ytitle += tbin*1000;
	ytitle += " ms";
	hclc->GetYaxis()->SetTitle(ytitle);
	hclc->GetYaxis()->SetTitleSize(lsize);
	hclc->GetYaxis()->SetLabelSize(lsize);
	hclc->GetYaxis()->CenterTitle();
	
	hclc->GetYaxis()->SetTitleOffset(0.4);	// 0.7
	hclc->GetXaxis()->SetTitleOffset(0.85);
//	hclc->SetTitle("AGILE-MCAL light curve");
	hclc->SetTitle("");
	
	if (ti!=0. || tf!=0.) hclc->GetXaxis()->SetRangeUser(ti, tf);
	hclc->Draw();
	if (cbkg > 0.) {	// draw background dashed line
		TLine *l;
		l = new TLine(ti, cbkg, tf, cbkg);
		l->SetLineStyle(2);
		l->Draw();
	}
	
	if (t0) {	// draw text labels only if selection is done
		TLatex tl;
		tl.SetTextSize(lsize);	//0.07
		char str5[100];
		sprintf (str5, "Trigger # %d", info->runid);
		tl.DrawTextNDC(0.15, 0.80, str5);
		
		TString str3("Energy range: 0.3-100 MeV");
		tl.DrawTextNDC(0.15, 0.70, str3);
		
		// draw T90 boundaries, if calculated
		if (i90lo && i90hi) {
			TLine *l1, *l2;
			l1 = new TLine(hclc->GetBinLowEdge(i90lo), 0, hclc->GetBinLowEdge(i90lo), 1.2*(cfpeak + cbkg));
			l1->SetLineStyle(2);
			l1->Draw();			
			l2 = new TLine(hclc->GetBinLowEdge(i90hi), 0, hclc->GetBinLowEdge(i90hi), 1.2*(cfpeak + cbkg));
			l2->SetLineStyle(2);
			l2->Draw();			
		}
	}
	
	// draw light curves in different energy bands
	TLatex t2;
	t2.SetTextSize(lsize);	// 0.07
	for (int i=0; i<nEbins; i++) {
		cCustom->cd(i+2);
		TString pname("hE");
		pname += i+1;
		pname += "_";
		pname += burstid;
		TH1D *haux = h2clc->ProjectionX(pname.Data(), i+1, i+1, "");
		haux->SetStats(0);
		char stitle[100];
		if (i==0) sprintf(stitle, "0.30 - %.2f MeV", mcalEbin[i+1]);
		else if (i==nEbins-1) sprintf(stitle, "> %.2f MeV", mcalEbin[i]);
		else sprintf(stitle, "%.2f - %.2f MeV", mcalEbin[i], mcalEbin[i+1]);
		TString htitle(stitle);
		haux->SetTitle("");
		if (ti!=0. || tf!=0.) haux->GetXaxis()->SetRangeUser(ti, tf);
		haux->GetXaxis()->SetTitle("t - t0 (s)");
		haux->GetXaxis()->CenterTitle();
		haux->GetXaxis()->SetTitleSize(lsize);
		haux->GetXaxis()->SetLabelSize(lsize);
		haux->GetYaxis()->SetTitle(ytitle);
		haux->GetYaxis()->SetTitleOffset(0.4);
		haux->GetYaxis()->SetTitleSize(lsize);
		haux->GetYaxis()->SetLabelSize(lsize);
		haux->GetXaxis()->SetTitleOffset(0.85);
		haux->GetYaxis()->CenterTitle();
		haux->Draw();
		t2.DrawTextNDC(0.15, 0.7, stitle);
		
		// draw background line, if calculated
		if (bkg[i] > 0.) {
			TLine *l;
			l = new TLine(ti, bkg[i], tf, bkg[i]);
			l->SetLineStyle(2);
			l->Draw();
		}
	}
	*/
	
	return;
}

//
//--------------------------------------------------------------------
//

int GRB::SaveIPN(TString outpath, double tbefore, double tafter)
{
	if (t0 == 0) {
		cout << "GRB::SaveIPN() : cannot save light curve. Create custom light curve first!" << endl;
		return 1;
	} else cout << "GRB::SaveIPN() : Saving light curve file in IPN-compliant format" << endl;
	
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	fname += "_lc.txt";
	
	FILE *fp;
	fp = fopen(fname.Data(), "a");
	
	// calculate the background between t0+tstart and t0-10
	double bkgval = 0.;
	int nb=0;
	// while (hclc->GetBinLowEdge(nb) < -10.) bkgval += hclc->GetBinContent(nb++);
	// double bkgtime = hclc->GetBinLowEdge(nb) - hclc->GetBinLowEdge(1);
	
	// calculate background on selected histogram (based on method GRB::CalculateBackground() )
	double tc;
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		if (tc>=tbkgfin) break;
		if (tc>=tbkgini) {
			bkgval += hclc->GetBinContent(j);
			nb++;
		}
	}
	double bkgtime = tbkgfin - tbkgini;
	
	cout << "Background: " << bkgval << " counts in " << nb << " bins  ";
	if (nb) bkgval/=nb;
	cout << bkgval << " counts/bin" << endl;
	
	// get initial and final time bins
	int bin1=1;
	while (hclc->GetBinLowEdge(bin1) < tbefore) bin1++;
//	double t1 = hclc->GetBinLowEdge(bin1);
	int binfin=nbins;
	while (hclc->GetBinLowEdge(binfin) > tafter) binfin--;
	
	// write the file header
	TTimeStamp daystart(year, month, day, 0, 0, 0, 0, 1, -epoch->GetSec());	// NB: FORSE DA CORREGGERE PER IL PASSAGGIO ORA LEGALE/SOLARE!!!
//	cout << t0 << endl << t1 << endl << daystart.GetSec() << endl;
//	double binstart = t0 + t1 - daystart.GetSec();
	double binstart = t0 - daystart.GetSec();
	printf("IPN header:\n'AGILEMCAL' '%02d/%02d/%02d' %.6f\n", day, month, year-2000, binstart);
	printf("%.0f %.0f\n", emin*1000., emax*1000.);
	printf("%.5f %.5f\n", bkgval, tbin);
	
	fprintf(fp, "'AGILEMCAL' '%02d/%02d/%02d' %.6f\n", day, month, year-2000, binstart);
	fprintf(fp, "%.0f %.0f\n", emin*1000., emax*1000.);
	fprintf(fp, "%.5f %.5f\n", bkgval, tbin);
	
	// save LC data
	for (int i=bin1; i<binfin; i++) {
		fprintf(fp, "%14.6f %13.5f\n", hclc->GetBinLowEdge(i), hclc->GetBinContent(i));
	//	printf("%14.6f %13.5f\n", hclc->GetBinLowEdge(i)-t1, hclc->GetBinContent(i));
	}
	
	fclose(fp);

	return 0;
}

//
//--------------------------------------------------------------------
//

TString *GRB::GetFileHeader()
{
	char tmps[50];
	if (t0) {
		sprintf(tmps, "MCAL_%06d_%02d%02d%02d_%02d-%02d-%02d", info->runid, year-2000, month, day, hour, min, sec);
	} else sprintf(tmps, "MCAL");
	TString *sheader = new TString(tmps);
	
	return sheader;
}

//
//--------------------------------------------------------------------
//

void GRB::SetGRBBoundaries( double tbkginiL, double tbkgfinL, double tgrbiniL, double tgrbfinL)
{
	tbkgini = tbkginiL;
	tbkgfin = tbkgfinL;
	tgrbini = tgrbiniL;
	tgrbfin = tgrbfinL;
	t90 = tgrbfin - tgrbini;
	return;
}

//
//--------------------------------------------------------------------
//

int GRB::SaveMsg(TString msgfile)
{
	if (t0 == 0) {
		cout << "GRB::SaveMsg() : cannot save output message. Create custom light curve first (need t0)!" << endl;
		return 1;
	} else cout << "GRB::SaveMsg() : Saving output message in BACODINE format to file " << msgfile.Data() << endl;
	
	FILE *fp;
	fp = fopen(msgfile.Data(), "w");
	fprintf(fp, "ALERT_TYPE:   MCAL 3908 ground trigger\n");
	fprintf(fp, "ALERT_TYPE_CODE:   %d\n", alertcode);
	fprintf(fp, "BURST_START:   %f\n", t0);
	fprintf(fp, "ORBIT_NUM:   %d\n", info->runid);

	fclose(fp);
	
	return 0;
}

//
//--------------------------------------------------------------------
//

int GRB::SaveAnalysis(TString outpath)
{
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	fname += ".txt";
	
	FILE *fp;
	fp = fopen(fname.Data(), "a");
	
	fprintf(fp,"\nOrbit: \t\t%d \n", info->runid);
	fprintf(fp,"Data type: \t%d \n", info->pkttype);
	fprintf(fp,"Time0:  \t%.6f (TT)   %s (UT) \n", t0, tzero->AsString("s"));
	fprintf(fp,"Temporal bin:  \t%.6f s \n", tbin);
	fprintf(fp,"Selection: \tEnergy range %6.2f - %6.2f MeV\n\t\tBars: %04X (X)  %04X (Z)\n", emin, emax, barsX, barsZ);
	fprintf(fp,"\nRescaled bkg calculation interval to values %.6f - %.6f s respect to t0\n", tbkgini, tbkgfin); 
	if (tbkgini == tbkgfin) fprintf(fp,"WARNING: GRB duration set to zero!\n");
	fprintf(fp,"\nBackground calculation \nid \tE band (MeV) \t\tbkg (counts/bin)\n");
	for (int i=0; i<nEbins; i++) fprintf(fp,"%d \t%6.1f - %6.1f \t%8.3f +/- %6.3f \n", 
		     i, mcalEbin[i], mcalEbin[i+1], bkg[i], ebkg[i]);
	fprintf(fp,"sel \t%6.1f - %6.1f \t%8.3f +/- %6.3f \n", emin, emax, cbkg, ecbkg);
	fprintf(fp,"\nPeak flux \nid \tE band (MeV) \t\tF_peak (counts/tbin) \tt_peak-t0 (s) \tsignificance\n");
	for (int i=0; i<nEbins; i++) fprintf(fp,"%d \t%6.1f - %8.1f \t%8.1f +/- %8.1f \t%8.6f \t%8.2f\n", 
	     i, mcalEbin[i], mcalEbin[i+1], fpeak[i], efpeak[i], tpeak[i], bkg[i]>0. ? fpeak[i]/sqrt(bkg[i]) : 0.);
	fprintf(fp,"sel \t%6.1f - %8.1f \t%8.1f +/- %8.1f \t%8.6f \t%8.2f\n", 
		emin, emax, cfpeak, ecfpeak, ctpeak, cbkg>0. ? (cfpeak-cbkg)/sqrt(cbkg) : 0.);
	fprintf(fp,"Estimated T90 = %.6f +/- %.6f s\n", t90, et90);
	fprintf(fp,"\nFluence in T90 (counts) \nid \tE band (MeV) \t\tFluence (counts)\n");
	for (int i=0; i<nEbins; i++) fprintf(fp,"%d \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", 
		     i, mcalEbin[i], mcalEbin[i+1], fluence[i], efluence[i]);
	fprintf(fp,"sel \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", emin, emax, cfluence, ecfluence); 
	fprintf(fp,"\nDefault hardness ratio (E>0.7 MeV / 0.35<E<0.7 MeV) \nHR = %6.3f +/- %6.3f\n", hr, ehr);
	fclose(fp);
	
	return 0;
}

//
//--------------------------------------------------------------------
//

int GRB::SaveInfo(TString infofile, int newline)
{
	FILE *fp;
	fp = fopen(infofile.Data(), "a");
	
	// orbit - pkttype - t0 (TT) - t0 (UT) - alert code - tbin 
	if (newline) fprintf(fp, "%6d  %4d  %20.6f  %s  %2d  %9.6f  ", 
	                     info->runid, info->pkttype, t0, tzero->AsString("s"), alertcode, tbin);
	
	// t90 - HR - nEbins - cbkg - bkg[0] .. bkg[nEbins-1]
	fprintf(fp, "%10.5f  %6.3f  %3d  %7.2f  ", t90, hr, nEbins, cbkg);
	for (int i=0; i<nEbins; i++) fprintf(fp, "%7.2f  ", bkg[i]);
	
	// cfpeak - fpeak[0] .. fpeak[nEbins-1]
	fprintf(fp, "  %10.2f  ", cfpeak);
	for (int i=0; i<nEbins; i++) fprintf(fp, "%10.2f  ", fpeak[i]);
	
	// cfluence - fluence[0] .. fluence[nEbins-1]
	fprintf(fp, "  %10.2f  ", cfluence);
	for (int i=0; i<nEbins; i++) fprintf(fp, "%10.2f  ", fluence[i]);
	
	if (info->pkttype == 3908 || !newline) fprintf(fp, "\n");
	fclose(fp);
	
	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB::SpectralAnalysis()
{
	printf("\nOrbit: \t\t%d \n", info->runid);
	printf("Data type: \t%d \n", info->pkttype);
	printf("Time0:  \t%.6f (TT)   %s (UT) \n", t0, tzero->AsString("s"));
	printf("Temporal bin:  \t%.6f s \n", tbin);
	printf("Selection: \tEnergy range %6.2f - %6.2f MeV\n\t\tBars: %04X (X)  %04X (Z)\n", emin, emax, barsX, barsZ);
		
	// --------------- BACKGROUND CALCULATION --------------
	CalculateBackground();
	
	// --------------- PEAK FLUX CALCULATION --------------
	CalculatePeakFlux();
	
	
	// --------------- T90 CALCULATION --------------
	CalculateT90();
	
	
	// --------------- FLUENCE CALCULATION IN T90 --------------
	CalculateFluence();	
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CustomSpectralAnalysis()
{
	printf("\nOrbit: \t\t%d \n", info->runid);
	printf("Data type: \t%d \n", info->pkttype);
	printf("Time0:  \t%.6f (TT)   %s (UT) \n", t0, tzero->AsString("s"));
	printf("Temporal bin:  \t%.6f s \n", tbin);
	printf("Selection: \tEnergy range %6.2f - %6.2f MeV\n\t\tBars: %04X (X)  %04X (Z)\n", emin, emax, barsX, barsZ);
		
	// --------------- BACKGROUND CALCULATION --------------
	CalculateBackground();
	
	// --------------- PEAK FLUX CALCULATION --------------
	CalculatePeakFlux();
	
	
	// --------------- T90 GRB TIME INTERVAL SETTING --------------
	double tc;
	double val;
	
	// if grb time are equal, get GRB time interval around peak flux
	if (tgrbini == tgrbfin) {
		tgrbini = ctpeak -fabs(tgrbini);
		tgrbfin = ctpeak +fabs(tgrbini);
	}
	
	// recalculate tgrbini and tgrbfin to match time bin
	cout << "tgrbini= " << tgrbini << endl;
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (fabs(tc-tgrbini)<0.9*tbin) {	// tc>=tgrbini non va bene quando tgrbini==0  && val>0.
			printf("%20.6f  %20.6f  %20.6f\n", hclc->GetBinLowEdge(j-1), tc, hclc->GetBinLowEdge(j+1));
			tgrbini = tc;
			i90lo = j;
			break;
		}
	}
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (fabs(tc-tgrbfin)<0.9*tbin ) {		// tc>=tgrbfin && val>0.
			tgrbfin = tc;
			i90hi = j;
			break;
		}
	}
	if (VERBOSE) cout << endl << "Rescaled GRB calculation interval to values " << tgrbini << "  -  " << tgrbfin 
			  << " s respect to t0" << endl;
	t90 = tgrbfin - tgrbini;
	et90 = tbin*sqrt(2.);	// two bins summed in quadrature
	
	if (VERBOSE) printf("Estimated T90 = %.3f +/- %.3f s\n", t90, et90);
	
	// --------------- FLUENCE CALCULATION IN T90 --------------
	CalculateFluence();	
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CalculateBackground()
{
	double tc;
	double val;
	// recalculate tbkgini and tbkgfin to match time bin
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (tc>=tbkgini && val>0.) {
			tbkgini = tc;
			break;
		}
	}
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (tc>=tbkgfin && val>0.) {
			tbkgfin = tc;
			break;
		}
	}
	if (VERBOSE) cout << endl << "Rescaled bkg calculation interval to values " << tbkgini << "  -  " << tbkgfin 
			  << " s respect to t0" << endl;
	
	// calculate background on different energy bands
//	cout << tbkgini << "  " << tbkgfin << endl;
	for (int i= 0; i<nEbins; i++) {
		TH1D *haux = h2clc->ProjectionX("htmp", i+1, i+1, "");
		bkg[i] = 0.;
		for (int j=1; j<nbins; j++) {
			tc = haux->GetBinLowEdge(j);
			if (tc>=tbkgini && tc<tbkgfin) {
				bkg[i] += haux->GetBinContent(j);
			}
		}
		ebkg[i] = sqrt(bkg[i]);
		if (tbkgfin>tbkgini) {
			bkg[i] /= (tbkgfin - tbkgini)/tbin;
			ebkg[i] /= (tbkgfin - tbkgini)/tbin;
		}
		delete haux;
	}
	
	// calculate background on selected histogram
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		if (tc>=tbkgini && tc<tbkgfin) {
			cbkg += hclc->GetBinContent(j);
			bkgLE += hLE->GetBinContent(j);
			bkgHE += hHE->GetBinContent(j);
		}
	}
	ecbkg = sqrt(cbkg);
	ebkgLE = sqrt(bkgLE);
	ebkgHE = sqrt(bkgHE);
	if (tbkgfin>tbkgini) {
		cbkg /= (tbkgfin - tbkgini)/tbin;
		ecbkg /= (tbkgfin - tbkgini)/tbin;

		bkgLE /= (tbkgfin - tbkgini)/tbin;
		ebkgLE /= (tbkgfin - tbkgini)/tbin;
	
		bkgHE /= (tbkgfin - tbkgini)/tbin;
		ebkgHE /= (tbkgfin - tbkgini)/tbin;
	}

	if (VERBOSE) {
		printf("\nBackground calculation \nid \tE band (MeV) \t\tbkg (counts/bin)\n");
		for (int i=0; i<nEbins; i++) printf("%d \t%6.1f - %6.1f \t%8.3f +/- %6.3f \n", 
		     i, mcalEbin[i], mcalEbin[i+1], bkg[i], ebkg[i]);
		printf("sel \t%6.1f - %6.1f \t%8.3f +/- %6.3f \n", emin, emax, cbkg, ecbkg);
	}
	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CalculatePeakFlux()
{
	// calculate i0: histogram index corresponding to t0
	
	while (hclc->GetBinLowEdge(i0)<=0.) i0++;
	i0--;
	if (VERBOSE) cout << "t0 at bin " << i0 << " corresponding to time " << hclc->GetBinLowEdge(i0) << endl;
	
	// calculate peak flux on different energy bands only in a time interval close to t0	
	
	double tc;
	double val;
	int nsearch = (int) (tsearch/tbin +1.);
	if (VERBOSE) cout << "tsearch = " << tsearch << "; peak flux search bin interval: " << i0-nsearch << " - " << i0+nsearch << endl;
	for (int i= 0; i<nEbins; i++) {
		TH1D *haux = h2clc->ProjectionX("htmp2", i+1, i+1, "");
		for (int j=i0-nsearch; j<=i0+nsearch; j++) {
			val = haux->GetBinContent(j);
			if (val > fpeak[i]) {
				fpeak[i] = val;
				tpeak[i] = haux->GetBinLowEdge(j);
				binpeak[i] = j;
			}
		}
		efpeak[i] = sqrt(fpeak[i]);
		fpeak[i] -= bkg[i];
		//fpeak[i] /= tbin;
		efpeak[i] = sqrt(efpeak[i]*efpeak[i] + bkg[i]);	// peak and bkg errors summed in quadrature
		//efpeak[i] /= tbin;
		delete haux;
	}
	
	// calculate peak flux on selected histogram only in a time interval close to t0
	cbinpeak = i0;
	for (int j=i0-nsearch; j<=i0+nsearch; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (val > cfpeak) {
			cfpeak = val;
			ctpeak = hclc->GetBinLowEdge(j);
			cbinpeak = j;
		}
	}
	ecfpeak = sqrt(cfpeak);
	cfpeak -= cbkg;
	//cfpeak /= tbin;
	ecfpeak = sqrt(ecfpeak*ecfpeak + cbkg);
	//ecfpeak /= tbin;

	if (VERBOSE) {		
		printf("\nPeak flux \nid \tE band (MeV) \t\tF_peak (counts/tbin) \tt_peak-t0 (s) \tsignificance\n");
		for (int i=0; i<nEbins; i++) printf("%d \t%6.1f - %8.1f \t%8.1f +/- %8.1f \t%8.3f \t%8.2f\n", 
		     i, mcalEbin[i], mcalEbin[i+1], fpeak[i], efpeak[i], tpeak[i], bkg[i]>0. ? fpeak[i]/sqrt(bkg[i]) : 0.);
		printf("sel \t%6.1f - %8.1f \t%8.1f +/- %8.1f \t%8.3f \t%8.2f\n", 
		       emin, emax, cfpeak, ecfpeak, ctpeak, cbkg>0. ? (cfpeak-cbkg)/sqrt(cbkg) : 0.);
	}

	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CalculateT90()
{
	double totcnts=0.;
	double auxbkg;
	double cnts;
	int nstop = 0;
	int jstart, jstop;
	
	double cthr = cbkg + 3.*sqrt(cbkg);
	
	// estimate total number of counts
	for (int j=cbinpeak; j<nbins; j++) {
		cnts = hclc->GetBinContent(j);
		jstop = j;
		if (cnts < cthr) nstop++;	// condizione di stop precedente:  (cnts < cbkg)
		if (nstop >= 3) break;
		totcnts += (cnts - cbkg);
	}
	nstop = 0;
	for (int j=cbinpeak-1; j>0; j--) {
		cnts = hclc->GetBinContent(j);
		jstart = j;
		if (cnts < cthr) nstop++;	// condizione di stop precedente:  (cnts < cbkg)
		if (nstop >= 3) break;
		totcnts += (cnts - cbkg);
	}
	if (VERBOSE) cout << endl << "Estimated number of GRB counts = " << totcnts << " between bins " << jstart << " - " << jstop << " (peak bin = " << cbinpeak << ")" << endl;
	double curcnts=0.;
	
	// find lower limit starting from peak bin
	for (int j=cbinpeak; j<jstop; j++) curcnts += (hclc->GetBinContent(j) - cbkg);
	i90lo = cbinpeak;
	do {
		i90lo--;
		curcnts += (hclc->GetBinContent(i90lo) - cbkg);
		//cout << i90lo << "  " << curcnts << endl;
	} while (curcnts < 0.95*totcnts && i90lo>jstart);
	i90lo++;
	
	// find upper limit 
	curcnts = 0.;
	for (int j=i90lo; j<=cbinpeak; j++) curcnts += (hclc->GetBinContent(j) - cbkg);
	// i90hi = i90lo;
	i90hi = cbinpeak+1;
	do {
		curcnts += (hclc->GetBinContent(i90hi) - cbkg);
		//cout << i90hi << "  " << curcnts << endl;
		i90hi++;
	} while (curcnts < 0.90*totcnts && i90hi<jstop);
	i90hi--;
	if (i90hi <= i90lo) i90hi++;
	t90 = hclc->GetBinLowEdge(i90hi) - hclc->GetBinLowEdge(i90lo);
	et90 = tbin*sqrt(2.);	// two bins summed in quadrature
	
	// recalculate tgrbini and tgrbfin
	tgrbini = hclc->GetBinLowEdge(i90lo);
	tgrbfin = hclc->GetBinLowEdge(i90hi);
	
	if (VERBOSE) printf("Estimated T90 = %.3f +/- %.3f s\n", t90, et90);

	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CalculateFluence()
{
	double fhi=0.;
	double auxbkg;
	
	for (int i=0; i<nEbins; i++) fluence[i] = 0.;
	cfluence = 0.;
	
	// calculate total and grb counts (~ fluence) on different energy bands between t0 and t90
	for (int i= 0; i<nEbins; i++) {
		TH1D *haux = h2clc->ProjectionX("htmp", i+1, i+1, "");
		for (int j=i90lo; j<i90hi; j++) fluence[i] += haux->GetBinContent(j);
		delete haux;
		
		if (i>0) fhi += fluence[i];
		
		efluence[i] = sqrt(fluence[i]);
		auxbkg = (i90hi-i90lo)*bkg[i];		
		fluence[i] -= auxbkg;
		efluence[i] = sqrt(efluence[i]*efluence[i] + auxbkg);
	}
	for (int j=i90lo; j<i90hi; j++) cfluence += hclc->GetBinContent(j);
	ecfluence = sqrt(cfluence);
	auxbkg = (i90hi-i90lo)*cbkg;
	cfluence -= auxbkg;
	ecfluence = sqrt(ecfluence*ecfluence + auxbkg);
			
	
	if (VERBOSE) {
		printf("\nFluence in T90 (counts) \nid \tE band (MeV) \t\tFluence (counts)\n");
		for (int i=0; i<nEbins; i++) printf("%d \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", 
		     i, mcalEbin[i], mcalEbin[i+1], fluence[i], efluence[i]);
		printf("sel \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", emin, emax, cfluence, ecfluence); 
	}
	
	// calculate hardness ratio
	// per ora: HR = E>0.7 MeV / 0.35<E<0.7 MeV
	
	auxbkg = 0.;
	for (int i= 1; i<nEbins; i++) auxbkg += bkg[i];
	auxbkg *= (i90hi-i90lo);
	double efhi = sqrt(fhi);
	fhi -= auxbkg;
	efhi = sqrt(efhi*efhi + auxbkg);
	hr = fluence[0]>0. ? fhi / fluence[0] : 0.;
	ehr = hr * sqrt(efhi*efhi/(fhi*fhi) + efluence[0]*efluence[0]/(fluence[0]*fluence[0]));
	
	if (VERBOSE) {
		printf("\nDefault hardness ratio (E>0.7 MeV / 0.35<E<0.7 MeV) \nHR = %6.3f +/- %6.3f\n", hr, ehr);
	}
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB::CalculateHR()
{
	int id = 0;
	float nLE, nHE, tt;
	for (int i=1; i<=nbins; i++) {
		tt = hLE->GetBinLowEdge(i);
		if (tt>=tgrbini && tt<=tgrbfin) {
			nLE = hLE->GetBinContent(i) - bkgLE;
			nHE = hHE->GetBinContent(i) - bkgHE;
			if (nLE>0. && nHE>0.) {
				ghr->SetPoint(id, tt+0.5*tbin, nHE/nLE);	
				ghr->SetPointError(id, 0.5*tbin, (nHE/nLE)*sqrt(hLE->GetBinContent(i)/(nLE*nLE) + hHE->GetBinContent(i)/(nHE*nHE)));
				cout << id << "  " << tt+0.5*tbin << "  " << nLE << "  " << nHE << "  " << nHE/nLE << endl;
				id++;	
			}
		}
	}
/*	// to be implemented later
		
	TCanvas *chr = new TCanvas("chr", "chr", 800, 600);
	chr->Divide(1,3);
	chr->cd(1);
	hLE->GetXaxis()->SetRangeUser(0, 7);
	hLE->Draw();
	chr->cd(2);
	hHE->GetXaxis()->SetRangeUser(0, 7);
	hHE->Draw();
	chr->cd(3);
	ghr->GetXaxis()->SetRangeUser(0, 7);
	ghr->Draw("ALP");
	cout << "bkg LE = " << bkgLE << "   bkg HE = " << bkgHE << endl;  */
	return;
}

//
//--------------------------------------------------------------------
//

void GRB::SetT90(int ilo, int ihi)
{
	// re-set T90 boundaries and recalculate fluence and HR (useful for Sci. RM)
	if (ihi <= ilo) {
		cout << "*** GRB::SetT90() ERROR: ihi MUST be > ilo!" << endl;
		return;
	}
	
	i90lo = ilo;
	i90hi = ihi;
	cout << "Setting t90 boundaries between bins " << i90lo << " - " << i90hi << endl;
	
	t90 = hclc->GetBinLowEdge(i90hi) - hclc->GetBinLowEdge(i90lo);
	et90 = tbin*sqrt(2.);	// two bins summed in quadrature
	
	if (VERBOSE) printf("Estimated T90 = %.3f +/- %.3f s\n", t90, et90);
	
	// --------------- FLUENCE CALCULATION IN T90 --------------
	CalculateFluence();	
	
	return;
}
