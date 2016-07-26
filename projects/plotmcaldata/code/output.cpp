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
#include "plotmcaldata.h"
#include "version.h"

TCanvas *standardOutput1(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, float xzmax)
{
	int nbins = (int) ((time[ns-1] - time[0])/1.024) + 1;
//	TDatime ts(time[0] + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int)  time[0], 0, epoch.GetSec(),0);
	
	// build histos for integral plane X plane Z RM vs time
	double *totx = new double[ns];
	double *totz = new double[ns];
	for (int j=0; j<ns; j++) {
		totx[j] = totz[j] = 0.;
		for (int i=1; i<11; i++) totx[j] += data[j*22 + i];
		for (int i=12; i<22; i++) totz[j] += data[j*22 + i];
	}
		
	// build total MCAL X and Z RM histos
	TH1D *hx = new TH1D("hx", "hx", nbins, time[0]-1., time[ns-1]);
	TH1D *hz = new TH1D("hz", "hz", nbins, time[0]-1., time[ns-1]);
	for (int j=0; j<ns-1; j++) {	//ns
		hx->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), totx[j]);
		hz->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), totz[j]);
	}
	
	// build Ac panel4 RM histos
	TH1D *hac40 = new TH1D("hac40", "hac40", nbins, time[0]-1., time[ns-1]);
	for (int j=0; j<ns; j++) hac40->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), dataAC[j*5+4]);
	
	hx->Rebin(bin);
	hz->Rebin(bin);
	hx->Scale(1./bin);
	hz->Scale(1./bin);
	
	TCanvas *c1 = new TCanvas("c1", "MCAL scientific ratemeters monitoring system 1", 1000, 750);
	c1->Divide(1, 3);
	
	// draw cumulative sci RM
	c1->cd(1);
	
	double ymax=500.;	// to be chosen from config file
	hx->GetYaxis()->SetRangeUser(0., ymax);
	hx->SetLineColor(4);
	hx->SetStats(0);
	TString htitle("MCAL Scientific Ratemeters integrated light curve (E>350 keV, time bin ");
	htitle += bin;
	htitle += " s)";
	hx->SetTitle(htitle.Data());
//	hx->GetXaxis()->SetTitle("Time (s)");

	// use UTC time
	hx->GetXaxis()->SetTitle("Time");
	hx->GetXaxis()->SetTitleOffset(0.8);
	hx->GetXaxis()->SetTimeDisplay(1);
	hx->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
	hx->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
	
	hx->GetXaxis()->CenterTitle();
	hx->GetYaxis()->SetTitle("Counts / s");
	hx->GetYaxis()->CenterTitle();
	hx->Draw();
	hz->SetLineColor(2);
	hz->Draw("SAME");
	
	//draw AC panel4 RM on the same pad
	hac40->Rebin(bin);
	hac40->Scale(1./bin);
	Float_t rightmax = 1.1*hac40->GetMaximum();
	Float_t scale = ymax/rightmax;
	cout << gPad->GetUymax() << "  " << rightmax << endl;
	hac40->SetLineColor(kGreen);
	hac40->Scale(scale);
	hac40->Draw("same");
	
	//draw an axis on the right side
	TGaxis *axis = new TGaxis(time[ns-1] + 1, 0., time[ns-1] + 1, ymax,0,rightmax,510,"+L");
	axis->SetLineColor(kGreen);
	axis->SetLabelColor(kGreen);
	axis->Draw();
	
	TLegend *leg = new TLegend(0.75, 0.75, 0.95, 0.95);
	leg->AddEntry(hx, "X-plane integral Sci. RM", "lp");
	leg->AddEntry(hz, "Z-plane integral Sci. RM", "lp");
	leg->AddEntry(hac40, "AC panel 4 Sci. RM (right axis)", "lp");
	leg->Draw();
	
	TText tt;
	TString str("MCAL Team - IASF-Bo - plotmcaldata version ");
	str += VERSION;
	tt.DrawTextNDC(0.5, 0.95, str.Data());
	
	TText tt1;
	TString str1("T start: ");
	str1 += ts.AsString();
	tt1.DrawTextNDC(0.15, 0.8, str1.Data());
	
	// build histos for energy bands light curves
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	
	// plane X light curves (low energy)
	c1->cd(2);
	int ng = ns/bin;
	double xi, xf, y;
	TLegend *legx = new TLegend(0.75, 0.75, 0.95, 0.95);
	for (int i=1; i<5; i++) {	// <11
	
		// TGraph option (not good)
		/*TGraphErrors *gx = new TGraphErrors(ng); 
		for (int j=0; j<ng; j++) {
			y=0.;
			for (int k=0; k<bin; k++) y += data[(j*bin+k)*22 + i];
			y /= bin;
			gx->SetPoint(j, 0.5*(time[j*bin] + time[(j+1)*bin - 1] + 1.), y);
			gx->SetPointError(j, 0.5*(time[(j+1)*bin - 1] + 1. - time[j*bin]), 0.);
		}
		
		gx->SetLineColor(i != 10 ? i : 13);
		gx->SetMarkerColor(i != 10 ? i : 13);		
		if (i==1) {
			gx->SetLineColor(i);
			gx->SetMarkerColor(i);
			TString gtitle("MCAL Scientific Ratemeters Plane X light curve");
			gx->SetTitle(gtitle);
			gx->GetXaxis()->SetTitle("Time (sec)");
			gx->GetXaxis()->CenterTitle();
			gx->GetYaxis()->SetTitle("Counts / s");
			gx->GetYaxis()->CenterTitle();
			gx->GetYaxis()->SetRangeUser(0., 100.);
			gx->Draw("APZ");
		}
		else gx->Draw("PZSAME");
		char eband[100];
		sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		legx->AddEntry(gx, eband, "lp");*/
	
		TString hname("hx");
		hname += i;
		TH1D *hx = new TH1D(hname, hname, nbins, time[0]-1., time[ns-1]);
		for (int j=0; j<ns; j++) hx->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), data[j*22 + i]);
		hx->Rebin(bin);
		hx->Scale(1./bin);
		hx->SetLineColor(i != 10 ? i : 13);
		if (i==1) {
			hx->SetLineColor(i);
			hx->SetStats(0);
			TString htitle("MCAL Scientific Ratemeters Plane X low energy light curve");
			hx->SetTitle(htitle);
			hx->GetXaxis()->SetTitle("Time (sec)");
			hx->GetXaxis()->CenterTitle();
			hx->GetYaxis()->SetTitle("Counts / s");
			hx->GetYaxis()->CenterTitle();
			hx->GetYaxis()->SetRangeUser(0., xzmax);
			hx->Draw();
			hx->GetXaxis()->SetTitle("Time");
			hx->GetXaxis()->SetTitleOffset(0.8);
			hx->GetXaxis()->SetTimeDisplay(1);
			hx->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
			hx->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		}
		else hx->Draw("same");
		char eband[100];
		sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		legx->AddEntry(hx, eband, "lp");
	}
	legx->Draw("same");
	
	// plane Z light curves (low energy)
	c1->cd(3);
	TLegend *legz = new TLegend(0.75, 0.75, 0.95, 0.95);
	for (int i=1; i<5; i++) {	// <11
		TString hname("hz");
		hname += i;
		TH1D *hz = new TH1D(hname, hname, nbins, time[0]-1., time[ns-1]);
		for (int j=0; j<ns; j++) hz->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), data[j*22 + i+11]);
		hz->Rebin(bin);
		hz->Scale(1./bin);
		hz->SetLineColor(i != 10 ? i : 13);
		if (i==1) {
			hz->SetLineColor(i);
			hz->SetStats(0);
			TString htitle("MCAL Scientific Ratemeters Plane Z low energy light curve");
			hz->SetTitle(htitle);
			hz->GetXaxis()->SetTitle("Time (sec)");
			hz->GetXaxis()->CenterTitle();
			hz->GetYaxis()->SetTitle("Counts / s");
			hz->GetYaxis()->CenterTitle();
			hz->GetYaxis()->SetRangeUser(0., xzmax);
			hz->Draw();
			hz->GetXaxis()->SetTitle("Time");
			hz->GetXaxis()->SetTitleOffset(0.8);
			hz->GetXaxis()->SetTimeDisplay(1);
			hz->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
			hz->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		}
		else hz->Draw("same");
		char eband[100];
		sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		legz->AddEntry(hz, eband, "lp");
	}
	legz->Draw("same");

	return c1;
}

//
//--------------------------------------------------------------------
//

TCanvas *standardOutput2(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, float xzmax)
{
	int nbins = (int) ((time[ns-1] - time[0])/1.024) + 1;
//	TDatime ts(time[0] + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int)  time[0], 0, epoch.GetSec(),0);
	
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	TCanvas *c2 = new TCanvas("c2", "MCAL scientific ratemeters monitoring system 2", 1000, 750);
	c2->Divide(1, 3);
	
	// plane X light curves (high energy)
	c2->cd(1);
	TLegend *legxhe = new TLegend(0.75, 0.6, 0.95, 0.95);
	for (int i=5; i<11; i++) {	// <11	
		TString hname("hx");
		hname += i;
		TH1D *hx = new TH1D(hname, hname, nbins, time[0]-1., time[ns-1]);
		for (int j=0; j<ns; j++) hx->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), data[j*22 + i]);
		hx->Rebin(bin);
		hx->Scale(1./bin);
		hx->SetLineColor(i != 10 ? i : 13);
		if (i==5) {
			hx->SetStats(0);
			TString htitle("MCAL Scientific Ratemeters Plane X high energy light curve");
			hx->SetTitle(htitle);
			hx->GetXaxis()->SetTitle("Time (sec)");
			hx->GetXaxis()->CenterTitle();
			hx->GetYaxis()->SetTitle("Counts / s");
			hx->GetYaxis()->CenterTitle();
			hx->GetYaxis()->SetRangeUser(0., 40.);
			hx->Draw();
			hx->GetXaxis()->SetTitle("Time");
			hx->GetXaxis()->SetTitleOffset(0.8);
			hx->GetXaxis()->SetTimeDisplay(1);
			hx->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
			hx->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		}
		else hx->Draw("same");
		char eband[100];
		sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		legxhe->AddEntry(hx, eband, "lp");
	}
	legxhe->Draw("same");
	TText tt1;
	TString str1("T start: ");
	str1 += ts.AsString();
	tt1.DrawTextNDC(0.15, 0.8, str1.Data());
	
	// plane Z light curves (high energy)
	c2->cd(2);
	TLegend *legzhe = new TLegend(0.75, 0.6, 0.95, 0.95);
	for (int i=5; i<11; i++) {	// <11
		TString hname("hz");
		hname += i;
		TH1D *hz = new TH1D(hname, hname, nbins, time[0]-1., time[ns-1]);
		for (int j=0; j<ns; j++) hz->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), data[j*22 + i+11]);
		hz->Rebin(bin);
		hz->Scale(1./bin);
		hz->SetLineColor(i != 10 ? i : 13);
		if (i==5) {
			hz->SetStats(0);
			TString htitle("MCAL Scientific Ratemeters Plane Z high energy light curve");
			hz->SetTitle(htitle);
			hz->GetXaxis()->SetTitle("Time (sec)");
			hz->GetXaxis()->CenterTitle();
			hz->GetYaxis()->SetTitle("Counts / s");
			hz->GetYaxis()->CenterTitle();
			hz->GetYaxis()->SetRangeUser(0., 40.);
			hz->Draw();
			hz->GetXaxis()->SetTitle("Time");
			hz->GetXaxis()->SetTitleOffset(0.8);
			hz->GetXaxis()->SetTimeDisplay(1);
			hz->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
			hz->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		}
		else hz->Draw("same");
		char eband[100];
		sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		legzhe->AddEntry(hz, eband, "lp");
	}
	legzhe->Draw("same");
	
	// draw AC sci RM
	c2->cd(3);
	TLegend *legac = new TLegend(0.75, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {	// <11
		TString hname("hac");
		hname += i;
		TH1D *hac = new TH1D(hname, hname, nbins, time[0]-01., time[ns-1]);
		for (int j=0; j<ns; j++) hac->SetBinContent(floor((time[j]-time[0])/1.024 +0.5), dataAC[j*5+i]);
		hac->Rebin(bin);
		hac->Scale(1./bin);
		hac->SetLineColor(i+1);
		if (i==0) {
			hac->SetStats(0);
			TString htitle("AC Scientific Ratemeters light curve");
			hac->SetTitle(htitle);
			hac->GetXaxis()->CenterTitle();
			hac->GetYaxis()->SetTitle("Counts / s");
			hac->GetYaxis()->CenterTitle();
			hac->GetYaxis()->SetRangeUser(0., 5000.);
			hac->Draw();
			hac->GetXaxis()->SetTitle("Time");
			hac->GetXaxis()->SetTitleOffset(0.8);
			hac->GetXaxis()->SetTimeDisplay(1);
			hac->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
			hac->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
			legac->AddEntry(hac, "AC TOP", "lp");
		}
		else {
			hac->Draw("same");
			TString acname("AC lat. ");
			acname += i;
			legac->AddEntry(hac, acname, "lp");
		}
	}
	legac->Draw("same");
	
	return c2;
}

//
//--------------------------------------------------------------------
//

TCanvas *zoomOutput1(int ns, double *time, double *data, double *dataAC, int bin, TTimeStamp epoch, double tgrbini, int nbefore, int nafter, int orbit, int ib, int fb, bool pub)
{
	
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	
//	TDatime ts(tgrbini + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int)  tgrbini, 0, epoch.GetSec(),0);
	TText tt1;
	tt1.SetTextSize(0.08);
	
	// find the bin 0
	int binini=0;
	while (time[binini]<tgrbini) binini++;
	binini += 8;
	
	// drawing parameters (to be set via command line)
	int bstart=ib;		// start energy band (included)
	int bstop=fb;		// stop energy band (included)
	
	TCanvas *c3bis = new TCanvas("c3bis", "MCAL zoomed ratemeters monitoring", 1000, 188*(bstop-bstart+1));//
	c3bis->Divide(2, bstop-bstart+1, 0.001, 0.001);	// 0.01
	
	// find out the maximum Y range
	float hirange=0.;	// Y axis max range
	for (int i=bstart; i<=bstop; i++) {
		for (int j=binini-nbefore; j<=binini+nafter; j++) {
			if (data[j*22 + i]>hirange) hirange = data[j*22 + i];
			if (data[j*22 + i + 11]>hirange) hirange = data[j*22 + i + 11];
		}
	}
	hirange *= 1.1;
	
	TString str1("AGILE MCAL");
	if (orbit) {
		str1 += " rev. ";
		str1 += orbit;
	}
	str1 += (" Time0: ");
	str1 += tgrbini;
	str1 += " - ";
	str1 += ts.AsString();
	str1 += " UT";

	for (int i=bstart; i<=bstop; i++) {
	
		// draw X light curve
		c3bis->cd(2*(i-bstart)+1);
		TString hnamex("h2x");
		hnamex += i;
		TH1D *hx = new TH1D(hnamex, hnamex, ns, -binini, ns-binini);
		for (int j=0; j<ns; j++) hx->SetBinContent(j+1, data[j*22 + i]);
		hx->Rebin(bin);
		hx->Scale(1./bin);
		if (!pub) hx->SetLineColor(i != 5 ? i : 1);
		hx->GetYaxis()->SetRangeUser(0., hirange);
		hx->GetXaxis()->SetRangeUser(-nbefore, nafter);
		hx->SetStats(0);
	//	TString htitle("MCAL Plane X energy band ");
	//	htitle += i;
		char ebandx[100];
		sprintf(ebandx, "Plane X %5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		TText t1;
		t1.SetTextSize(0.08);
		hx->GetXaxis()->SetTitle("Time - T0 (1.024 s bin)");
		hx->GetXaxis()->CenterTitle();
		hx->GetYaxis()->SetTitle("Counts / bin");
		hx->GetYaxis()->CenterTitle();
		hx->GetXaxis()->SetTitleOffset(1.0);
		hx->GetYaxis()->SetTitleOffset(0.5);
		hx->SetTitle("");
		hx->Draw();
		t1.DrawTextNDC(0.15, 0.8, ebandx);
		if (i==bstart && !pub) tt1.DrawTextNDC(0.15, 0.88, str1.Data());
		
		// draw Z light curve
		c3bis->cd(2*(i-bstart)+2);
		TString hnamez("h2z");
		hnamez += i;
		TH1D *hz = new TH1D(hnamez, hnamez, ns, -binini, ns-binini);
		for (int j=0; j<ns; j++) hz->SetBinContent(j+1, data[j*22 + i + 11]);
		hz->Rebin(bin);
		hz->Scale(1./bin);
		if (!pub) hz->SetLineColor(i != 5 ? i : 1);
		hz->GetYaxis()->SetRangeUser(0., hirange);
		hz->GetXaxis()->SetRangeUser(-nbefore, nafter);
		hz->SetStats(0);
		char ebandz[100];
		sprintf(ebandz, "Plane Z %5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
		
		hz->GetXaxis()->SetTitle("Time - T0 (1.024 s bin)");
		hz->GetXaxis()->CenterTitle();
		hz->GetYaxis()->SetTitle("Counts / bin");
		hz->GetYaxis()->CenterTitle();
		hz->GetXaxis()->SetTitleOffset(1.0);
		hz->GetYaxis()->SetTitleOffset(0.5);
		hz->SetTitle("");
		hz->Draw();
		t1.DrawTextNDC(0.15, 0.8, ebandz);
	}
	return c3bis;
}

//
//--------------------------------------------------------------------
//
    // obsolete: use plotgrb instead
/*
int saveLightCurve(int ns, double *time, double *data, TTimeStamp epoch, double tgrb0, int nbefore, int nafter, int orbit, int ib, int fb, int iplane)
{
	// save light curve in IPN compliant format
	
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	
	FILE *fp;
	char pname[100];
	
	cout << "Saving light curve file in IPN-compliant format" << endl;
	
	// find the bin 0
	int binini=0;
	while (time[binini]<tgrb0-nbefore) binini++;
	binini += 8;
	tgrb0 = time[binini] - 8.192; 		// set the initial time of the LC
	
	// extract the data subset (integrate on the selected bands)
	double *drm= new double[ns];
	for (int i=0; i<ns; i++) {
		drm[i]=0.;
		for (int j=ib; j<=fb; j++) {
			drm[i] += data[i*22 + j + 11*iplane];
		}
	}
	
	// calculate the background between t0-110 and t0-10
	double bkgval = 0.;
	int bkg0=-110;
	int bkg1=-10;
	for (int i=binini+nbefore+bkg0; i<binini+nbefore+bkg1; i++) bkgval += drm[i];
	bkgval /= (bkg1-bkg0);
	
//	TDatime ts(tgrb0 + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int)  tgrb0, 0, epoch.GetSec(),0);
	// get time0 information
	char caux[10];
	int year, month, day, hour, min, sec;
	sprintf(caux, "%d", ts.GetDate(1));
	sscanf(caux, "%4d%2d%2d", &year, &month, &day);
	sprintf(caux, "%06d", ts.GetTime(1));
	sscanf(caux, "%2d%2d%2d", &hour, &min, &sec);

	sprintf(pname, "LC_%02d%02d%02d_plane%c_band%d-%d.txt", year-2000, month, day, iplane==0 ? 'X' : 'Z', ib, fb);
	fp = fopen(pname, "w");
	
	// write the file header
	TDatime t0(year, month, day, 0, 0, 0);
	cout << t0.Convert() << "  " << ts.Convert() << "  " << ts.Convert()-t0.Convert() << endl;
	fprintf(fp, "'AGILEMCAL' '%02d/%02d/%02d' %.6f\n", ts.GetDay(), ts.GetMonth(), ts.GetYear()-2000,  (float) (ts.Convert()-t0.Convert())+fmod(tgrb0, 1.));
	fprintf(fp, "%.0f %.0f\n", (tmpEband[ib]-tmpEerr[ib])*1000., (tmpEband[fb]+tmpEerr[fb])*1000.);
	fprintf(fp, "%.5f %.5f\n", bkgval, 1.024);
	
	// save LC data
	for (int i=binini; i<binini+nbefore+nafter; i++) {
	//	fprintf(fp, "%14.6f %13.5f\n", time[i]-time[binini], drm[i]);	// correct
		fprintf(fp, "%14.6f %13.5f\n", (i-binini)*1.024, drm[i]);
		printf("%14.3f %14.6f %13.5f %.6f\n", time[i], time[i]-time[binini], drm[i], time[i]-time[i-1] - 1.024);	// check x il Dt
	}
	
	fclose(fp);
	delete drm;

	return 0;
}
*/

//
//--------------------------------------------------------------------
//

TCanvas *grbSpectrum(int ns, double *time, double *data, TTimeStamp epoch, double tgrb0, int orbit, int tbkgini, int tbkgfin, int tgrbini, int tgrbfin)
{
	
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	
//	TDatime ts(tgrb0 + epoch.Convert());
	TTimeStamp ts;
	ts.Set((int) tgrb0 , 0, epoch.GetSec(),0);
	
	float bkgX[11];		// X bkg
	float grbtotX[11];	// X grb + bkg
	float bkgZ[11];		// Z bkg
	float grbtotZ[11];	// Z grb + bkg
	float grbX[11];		// X grb (bkg subtracted)
	float grbZ[11];		// Z grb (bkg subtracted)
	
	for (int j=0; j<11; j++) bkgX[j] = grbtotX[j] = bkgZ[j] = grbtotZ[j] = grbX[j] = grbZ[j] = 0.;
	
	// suffix for pad printing
	TString suffix("_");
	suffix += orbit;
	suffix += "_bkg";
	suffix += tbkgini;
	suffix += "_";
	suffix += tbkgfin;
	suffix += "_grb";
	suffix += tgrbini;
	suffix += "_";
	suffix += tgrbfin;
	suffix += ".gif";
	
	// find the bin 0
	int binini=0;
	while (time[binini]<tgrb0) binini++;
	binini += 8;
	
	tbkgini += binini;
	tbkgfin += binini;
	tgrbini += binini;
	tgrbfin += binini;
	
	// calculate bkg
	for (int i=tbkgini; i<tbkgfin; i++) {
		for (int j=1; j<11; j++) {
			bkgX[j] += data[i*22 + j];
			bkgZ[j] += data[i*22 + j + 11];
		}
	}
	float dtbkg = time[tbkgfin]-time[tbkgini];
	cout << "BKG integration duration = " << dtbkg << " s" << endl;
	for (int j=1; j<11; j++) {	// normalization to 1 s
		bkgX[j] /= dtbkg;
		bkgZ[j] /= dtbkg;
	}
	
	// calculate grb+bkg
	for (int i=tgrbini; i<tgrbfin; i++) {
		for (int j=1; j<11; j++) {
			cout << i << "  " << j << "  " << data[i*22 + j] << "  " << data[i*22 + j + 11] << "  " << grbtotX[j] << "  " << grbtotZ[j] << endl;
			grbtotX[j] += data[i*22 + j];
			grbtotZ[j] += data[i*22 + j + 11];
		}
	}
	
	cout << endl;
	
	float dtgrb = time[tgrbfin]-time[tgrbini];
	cout << "GRB duration = " << dtgrb << " s" << endl;
	for (int j=1; j<11; j++) {	// normalization to 1 s
		grbtotX[j] /= dtgrb;
		grbtotZ[j] /= dtgrb;
		cout << j << "  " << grbtotX[j] << "  " << grbtotZ[j] << endl;
	}
	
	TGraphErrors *gRMbkgX = new TGraphErrors(11);
	TGraphErrors *gRMbkgZ = new TGraphErrors(11);
	TGraphErrors *gRMgrbtotX = new TGraphErrors(11);
	TGraphErrors *gRMgrbtotZ = new TGraphErrors(11);
	TGraphErrors *gRMgrbX = new TGraphErrors(11);
	TGraphErrors *gRMgrbZ = new TGraphErrors(11);
	
	for (int j=1; j<11; j++) {
		gRMbkgX->SetPoint(j-1, tmpEband[j], bkgX[j]);
		gRMbkgX->SetPointError(j-1, tmpEerr[j], 3*sqrt(bkgX[j]/dtbkg));
		gRMgrbtotX->SetPoint(j-1, tmpEband[j], grbtotX[j]);
		gRMgrbtotX->SetPointError(j-1, tmpEerr[j], 3*sqrt(grbtotX[j]/dtgrb));
		gRMgrbX->SetPoint(j-1, tmpEband[j], grbtotX[j]-bkgX[j]);
		gRMgrbX->SetPointError(j-1, tmpEerr[j], 3*sqrt(grbtotX[j]/dtgrb));
		
		gRMbkgZ->SetPoint(j-1, tmpEband[j], bkgZ[j]);
		gRMbkgZ->SetPointError(j-1, tmpEerr[j], 3*sqrt(bkgZ[j]/dtbkg));
		gRMgrbtotZ->SetPoint(j-1, tmpEband[j], grbtotZ[j]);
		gRMgrbtotZ->SetPointError(j-1, tmpEerr[j], 3*sqrt(grbtotZ[j]/dtgrb));
		gRMgrbZ->SetPoint(j-1, tmpEband[j], grbtotZ[j]-bkgZ[j]);
		gRMgrbZ->SetPointError(j-1, tmpEerr[j], 3*sqrt(grbtotZ[j]/dtgrb));
	}
	
	// plot superimposed GRB and BKG spectra, for plane X
	TCanvas *c4 = new TCanvas("c4", "BURST count spectra", 1000, 750);
	c4->Divide(2,2);
	c4->cd(1);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gRMgrbtotX->SetLineColor(2);
	gRMgrbtotX->GetXaxis()->SetLimits(0.1, 2000.);
	double maxy = TMath::MaxElement(11, gRMgrbtotX->GetY());
	gRMgrbtotX->GetHistogram()->SetMaximum(1.1*maxy);
	double miny = TMath::MinElement(11, gRMgrbtotX->GetY());
	gRMgrbtotX->GetHistogram()->SetMinimum(0.9*miny);
	gRMgrbtotX->GetXaxis()->SetTitle("Energy (MeV)");
	gRMgrbtotX->GetXaxis()->SetTitleOffset(1.4);
	gRMgrbtotX->GetXaxis()->CenterTitle();
	gRMgrbtotX->GetYaxis()->SetTitle("Counts/s");
	gRMgrbtotX->GetYaxis()->CenterTitle();
	gRMgrbtotX->GetYaxis()->SetTitleOffset(1.5);
	gRMgrbtotX->SetTitle("plane X: GRB + BKG count spectra");
	gRMgrbtotX->GetXaxis()->SetTitleSize(0.04);
	gRMgrbtotX->GetXaxis()->SetTitleOffset(1.);
	gRMgrbtotX->GetXaxis()->SetLabelSize(0.04);
	gRMgrbtotX->GetYaxis()->SetTitleSize(0.04);
	gRMgrbtotX->GetYaxis()->SetTitleOffset(1.);
	gRMgrbtotX->GetYaxis()->SetLabelSize(0.04);
	gRMgrbtotX->Draw("AP");
	gRMbkgX->Draw("PSAME");
		
	// plot BKG subtracted GRB spectrum, for plane X
	c4->cd(3);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gRMgrbX->GetXaxis()->SetLimits(0.1, 2000.);
	maxy = TMath::MaxElement(11, gRMgrbtotX->GetY());
	gRMgrbX->GetHistogram()->SetMaximum(1.1*maxy);
	miny = TMath::MinElement(11, gRMgrbtotX->GetY());
	gRMgrbX->GetHistogram()->SetMinimum(0.9*miny);
	gRMgrbX->GetXaxis()->SetTitle("Energy (MeV)");
	gRMgrbX->GetXaxis()->SetTitleOffset(1.4);
	gRMgrbX->GetXaxis()->CenterTitle();
	gRMgrbX->GetYaxis()->SetTitle("Counts/s");
	gRMgrbX->GetYaxis()->CenterTitle();
	gRMgrbX->GetYaxis()->SetTitleOffset(1.5);
	gRMgrbX->SetTitle("plane X: GRB count spectrum");
	gRMgrbX->GetXaxis()->SetTitleSize(0.04);
	gRMgrbX->GetXaxis()->SetTitleOffset(1.);
	gRMgrbX->GetXaxis()->SetLabelSize(0.04);
	gRMgrbX->GetYaxis()->SetTitleSize(0.04);
	gRMgrbX->GetYaxis()->SetTitleOffset(1.);
	gRMgrbX->GetYaxis()->SetLabelSize(0.04);
	gRMgrbX->Draw("AP");

	// plot superimposed GRB and BKG spectra, for plane Z
	c4->cd(2);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gRMgrbtotZ->SetLineColor(2);
	gRMgrbtotZ->GetXaxis()->SetLimits(0.1, 2000.);
	maxy = TMath::MaxElement(11, gRMgrbtotZ->GetY());
	gRMgrbtotZ->GetHistogram()->SetMaximum(1.1*maxy);
	miny = TMath::MinElement(11, gRMgrbtotZ->GetY());
	gRMgrbtotZ->GetHistogram()->SetMinimum(0.9*miny);
	gRMgrbtotZ->GetXaxis()->SetTitle("Energy (MeV)");
	gRMgrbtotZ->GetXaxis()->SetTitleOffset(1.4);
	gRMgrbtotZ->GetXaxis()->CenterTitle();
	gRMgrbtotZ->GetYaxis()->SetTitle("Counts/s");
	gRMgrbtotZ->GetYaxis()->CenterTitle();
	gRMgrbtotZ->GetYaxis()->SetTitleOffset(1.5);
	gRMgrbtotZ->SetTitle("plane X: GRB + BKG count spectra");
	gRMgrbtotZ->GetXaxis()->SetTitleSize(0.04);
	gRMgrbtotZ->GetXaxis()->SetTitleOffset(1.);
	gRMgrbtotZ->GetXaxis()->SetLabelSize(0.04);
	gRMgrbtotZ->GetYaxis()->SetTitleSize(0.04);
	gRMgrbtotZ->GetYaxis()->SetTitleOffset(1.);
	gRMgrbtotZ->GetYaxis()->SetLabelSize(0.04);
	gRMgrbtotZ->Draw("AP");
	gRMbkgZ->Draw("PSAME");
	
	// plot BKG subtracted GRB spectrum, for plane Z
	c4->cd(4);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gRMgrbZ->GetXaxis()->SetLimits(0.1, 2000.);
	maxy = TMath::MaxElement(11, gRMgrbtotZ->GetY());
	gRMgrbZ->GetHistogram()->SetMaximum(1.1*maxy);
	miny = TMath::MinElement(11, gRMgrbtotZ->GetY());
	gRMgrbZ->GetHistogram()->SetMinimum(0.9*miny);
	gRMgrbZ->GetXaxis()->SetTitle("Energy (MeV)");
	gRMgrbZ->GetXaxis()->SetTitleOffset(1.4);
	gRMgrbZ->GetXaxis()->CenterTitle();
	gRMgrbZ->GetYaxis()->SetTitle("Counts/s");
	gRMgrbZ->GetYaxis()->CenterTitle();
	gRMgrbZ->GetYaxis()->SetTitleOffset(1.5);
	gRMgrbZ->SetTitle("plane Z: GRB count spectrum");
	gRMgrbZ->GetXaxis()->SetTitleSize(0.04);
	gRMgrbZ->GetXaxis()->SetTitleOffset(1.);
	gRMgrbZ->GetXaxis()->SetLabelSize(0.04);
	gRMgrbZ->GetYaxis()->SetTitleSize(0.04);
	gRMgrbZ->GetYaxis()->SetTitleOffset(1.);
	gRMgrbZ->GetYaxis()->SetLabelSize(0.04);
	gRMgrbZ->Draw("AP");
	
	//write count spectra
	printf("E_lo           E_high | \t X_bkg \t\tX_cnts/s MeV \t\tX_sigma | \t Z_bkg \t\tZ_cnts/s MeV \t\tZ_sigma\n");
	for (int j=1; j<11; j++) {
		double de = 2.*tmpEerr[j];
		printf("%9.3f - %9.3f | \t%7.1f \t%7.1f \t%7.1f | \t%7.1f \t%7.1f \t%7.1f\n", tmpEband[j]-tmpEerr[j], tmpEband[j]+tmpEerr[j], bkgX[j], grbtotX[j]-bkgX[j], sqrt(grbtotX[j]/dtgrb), bkgZ[j], grbtotZ[j]-bkgZ[j], sqrt(grbtotZ[j]/dtgrb)); 
	}
	
	for (int j=1; j<11; j++) {	// conteggi totali (NON /s) da scrivere nel file PHA colonna COUNTS
		grbX[j] = (grbtotX[j]-bkgX[j])*dtgrb;
		grbZ[j] = (grbtotZ[j]-bkgZ[j])*dtgrb;
		grbtotX[j] *= dtgrb;
		grbtotZ[j] *= dtgrb;
		bkgX[j] *= dtbkg;
		bkgZ[j] *= dtbkg;
		
		cout << j << "  " << bkgX[j] << "  " << grbtotX[j] << "  " << bkgZ[j] << "  " << grbtotZ[j] << endl;

	}
	
	//savePhaP1P2(10, &grbX[1], &grbZ[1], dtgrb);	//old version
/*	
	TString *grbP1name = new TString("!phaP1_grb.fits");	// save GRB (bkg subtracted) data
	savePha(10, &grbX[1], dtgrb, grbP1name);
	TString *grbP2name = new TString("!phaP2_grb.fits");
	savePha(10, &grbZ[1], dtgrb, grbP2name);
	
	TString *bkgP1name = new TString("!phaP1_bkg.fits");	// save BKG data
	savePha(10, &bkgX[1], dtbkg, bkgP1name);
	TString *bkgP2name = new TString("!phaP2_bkg.fits");
	savePha(10, &bkgZ[1], dtbkg, bkgP2name);
	
	TString *totP1name = new TString("!phaP1_tot.fits");	// save total (GRB+BKG) data
	savePha(10, &grbtotX[1], dtgrb, totP1name);
	TString *totP2name = new TString("!phaP2_tot.fits");
	savePha(10, &grbtotZ[1], dtgrb, totP2name);

	
	// save pictures	
	c4->cd(1);
	TString p1name("totX");
	p1name += suffix;
	gPad->Print(p1name.Data());
	
	c4->cd(2);
	TString p2name("totZ");
	p2name += suffix;
	gPad->Print(p2name.Data());
	
	c4->cd(3);
	TString p3name("grbX");
	p3name += suffix;
	gPad->Print(p3name.Data());
	
	c4->cd(4);
	TString p4name("grbZ");
	p4name += suffix;
	gPad->Print(p4name.Data());
*/
	return c4;
}

//
//--------------------------------------------------------------------
//

