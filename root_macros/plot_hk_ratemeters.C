// root macro to plot HK histo together with scientific ratemeters (for bus power transition check)
// MM 6/2/2007

init()
{
	gROOT->Reset();
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.06, "X");
	mcalstyle->SetLabelSize(0.06, "Y");
	mcalstyle->SetTitleSize(0.06, "XYZ");
	mcalstyle->SetTitleOffset(0.8, "XYZ");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
}


plot_single()
{

//TFile *fhisto= new TFile("/data1/mcalsw_output/H/H013362_3902.root");
//TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/IABG/delta_qualifica_gennaio07/TV/hk13362.root");

//TFile *fhisto= new TFile("/data1/mcalsw_output/H/H013363_3902.root");
//TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/IABG/delta_qualifica_gennaio07/TV/hk13363.root");

//TFile *fhisto= new TFile("/data1/mcalsw_output/H/H013386_3909.root");
//TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/IABG/delta_qualifica_gennaio07/TV/hk13386.root");

//TFile *fhisto= new TFile("/data1/mcalsw_output/H/H013387_3909.root");
//TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/IABG/delta_qualifica_gennaio07/TV/hk13387.root");

//TFile *fhisto= new TFile("/data1/mcalsw_output/H/H013390_3909.root");
//TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/IABG/delta_qualifica_gennaio07/TV/hk13390.root");

TFile *fhisto= new TFile("/data1/mcalsw_output/H/H011842_3901.root");
TFile *fhk= new TFile("/data1/AGILE_MCAL_AIV_Data/CGS_Tortona/RFT_061219/hk11842.root");

TCanvas *c1 = new TCanvas("c1", "sci RM / HK comparison", 600, 1000);
c1->Divide(1, 6);

TObjArray *rawHisto = (TObjArray *) fhisto->Get("rawHisto");
TObjArray *rSciRM3913 =(TObjArray *) rawHisto->FindObject("rawSciRM3913");
c1->cd(1);
TGraph *gaux = (TGraph *) rSciRM3913->At(1);// 0 for plane Z total counts
gaux->SetTitle("Plane X RM 2 (170 - 350 keV)");
gaux->UseCurrentStyle();
//gaux->GetXaxis()->SetLabelSize(0.06);
//gaux->GetXaxis()->SetTitleSize(0.06);
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
//gaux->GetYaxis()->SetLabelSize(0.06);
//gaux->GetYaxis()->SetTitleSize(0.06);
gaux->GetYaxis()->SetTitle("Counts/s");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(4200., 6600.);
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
//gaux->GetXaxis()->SetRangeUser(0., 3600.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");
c1->cd(2);
gaux = (TGraph *) rSciRM3913->At(2);
gaux->SetTitle("Plane X RM 3 (350 - 700 keV)");
gaux->UseCurrentStyle();
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
gaux->GetYaxis()->SetTitle("Counts/s");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(4200., 6600.);
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
//gaux->GetXaxis()->SetRangeUser(0., 3600.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");
c1->cd(3);
gaux = (TGraph *) rSciRM3913->At(12);	// 11 for plane X total counts
gaux->SetTitle("Plane Z RM 2 (170 - 350 keV)");
gaux->UseCurrentStyle();
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
gaux->GetYaxis()->SetTitle("Counts/s");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(4200., 6600.);
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
//gaux->GetXaxis()->SetRangeUser(0., 3600.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");
c1->cd(4);
gaux = (TGraph *) rSciRM3913->At(13);
gaux->SetTitle("Plane Z RM 3 (350 - 700 keV)");
gaux->UseCurrentStyle();
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
gaux->GetYaxis()->SetTitle("Counts/s");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(4200., 6600.);
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
//gaux->GetXaxis()->SetRangeUser(0., 3600.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");
			
			
c1->cd(5);
fhk->GetObject("M6210_0_MCAL_PD_BIAS", gaux);
gaux->UseCurrentStyle();
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
gaux->GetYaxis()->SetTitle("ADC counts");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(4200., 6600.);
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
//gaux->GetXaxis()->SetRangeUser(0., 3600.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");
c1->cd(6);
fhk->GetObject("M6244_0_MCAL_Input_Current", gaux);
gaux->UseCurrentStyle();
gaux->GetXaxis()->SetTitle("Time (s)");
gaux->GetXaxis()->CenterTitle();
gaux->GetYaxis()->SetTitle("ADC counts");
gaux->GetYaxis()->CenterTitle();
//gaux->GetXaxis()->SetRangeUser(3800., 4400.);
gaux->GetXaxis()->SetRangeUser(3600., 6300.);
gaux->Draw("ALP");

// draw separation lines and text labels

//int nsep = 7;	// for run 13362
//float tsep[7] = {0., 450., 800., 1300., 1585., 1910., 2550.};	// for run 13362

//int nsep = 21;	// for run 13363
//float tsep[21] = {0., 180., 2580., 2710., 2911., 3120., 3502., 4085., 4358., 4526., 4665., 4833., 5015., 5231., 5480., 5656., 5846., 6050., 6148., 6235., 6400.};	// for run 13363

//int nsep = 4;	// for run 13386
//float tsep[] = {0., 3480., 4620., 9000.};	// for run 13386

//int nsep = 6;	// for run 13387
//float tsep[] = {3700., 3975., 4050., 4150., 4226., 4600.};	// for run 13387

int nsep = 22;	// for run 11842
float tsep[] = {0., 60., 140., 560., 720., 1010., 1060., 1300., 2200., 2400., 2600., 4500., 4750., 4850., 4950., 5050., 5300., 5550., 5850., 5980., 6100., 6300.};	// for run 11842

for (int j=11; j<nsep; j++) { 
	TLine *l = new TLine(tsep[j], 0., tsep[j], 500.);
	l->SetLineColor(2);
	for (int i=1; i<=6; i++) {
		c1->cd(i);
		l->Draw();
	}
	TString tt;
	tt+=j;
	TText *t = new TText(0.5*(tsep[j]+tsep[j-1]), 20., tt.Data());
	t->SetTextSize(0.1);
	t->SetTextColor(2);
	t->Draw();
}

c1->Update();

}

// ------------------------------------------------------------

plot_multiple()
{
	TDatime da(2007, 1, 1, 0, 0, 0);		// TV February 2007 in IABG
	TDatime dstart(2007, 2, 2, 12, 0, 0);
	TDatime dstop(2007, 2, 2, 18, 0, 0);
	
//	TDatime da(2005, 12, 31, 23, 0, 0);		// TV July 2006 in IABG
//	TDatime dstart(2006, 7, 20, 0, 0, 0);
//	TDatime dstop(2006, 7, 20, 24, 0, 0);
	
//	TDatime da(2005, 12, 31, 23, 0, 0);		// FFT May 2006 in CGS
//	TDatime dstart(2006, 5, 19, 0, 0, 0);
//	TDatime dstop(2006, 5, 23, 24, 0, 0);
	
	long int ti = da.Convert();
	float  tini=dstart.Convert() - da.Convert();
	float  tend=dstop.Convert() - da.Convert();

//	orbite con DAC_Ref=251
//	TFile *f= new TFile("/data1/COMMISSIONING_AGILE/threshold_tuning/MCAL_global_data_496-512.root");
//	TFile *fhk= new TFile("/data1/COMMISSIONING_AGILE/threshold_tuning/hk_496-512.root");

	TFile *f= new TFile("/data1/COMMISSIONING_AGILE/threshold_tuning/MCAL_global_data_525-526.root");
	TFile *fhk= new TFile("/data1/COMMISSIONING_AGILE/threshold_tuning/hk_525-256_dac-ref254.root");
	
	TCanvas *c1 = new TCanvas("c1", "sci RM / HK comparison", 1000, 600);
//	c1->Divide(1, 5);
	TGraph *gaux = NULL;
	
	c1->cd(1);
//	f->GetObject("grmX1", gaux);
//	f->GetObject("grmX2", gaux);
	f->GetObject("grmX3", gaux);
//	fhk->GetObject("M6572_0_MCAL_Cycled_Rt_BoardX", gaux);
//	gaux->SetTitle("Plane X RM 2 (170 - 350 keV)");
//	gaux->SetTitle("Plane X RM 3 (350 - 700 keV)");
//	gaux->SetTitle("Cyclic ratemeter board X");
	gaux->UseCurrentStyle();
	//gaux->GetXaxis()->SetLabelSize(0.06);
	//gaux->GetXaxis()->SetTitleSize(0.06);
	gaux->GetXaxis()->SetTitle("Time ");
	gaux->GetXaxis()->CenterTitle();
	//gaux->GetYaxis()->SetLabelSize(0.06);
	//gaux->GetYaxis()->SetTitleSize(0.06);
	gaux->GetYaxis()->SetTitle("Counts/s");
	gaux->GetYaxis()->CenterTitle();
	gaux->GetYaxis()->SetRangeUser(0., 150.);
//	gaux->GetXaxis()->SetTimeDisplay(1);
//	gaux->GetXaxis()->SetTimeOffset(da.Convert());
//	gaux->GetXaxis()->SetTimeFormat("%H:%M");
//	gaux->GetXaxis()->SetRangeUser(tini, tend);
	int npt = gaux->GetN();
	double minx = TMath::MinElement(npt, gaux->GetX());
	double maxx = TMath::MaxElement(npt, gaux->GetX());
	gaux->GetXaxis()->SetLimits(minx, maxx);
	double maxy = TMath::MaxElement(npt, gaux->GetY());
	gaux->GetHistogram()->SetMaximum(1.1*maxy);
	gaux->GetHistogram()->SetMinimum(0.);
	gaux->Draw("AP");
/*
	c1->cd(2);
//	f->GetObject("grmZ1", gaux);
//	f->GetObject("grmZ2", gaux);
	f->GetObject("grmZ3", gaux);
//	fhk->GetObject("M6574_0_MCAL_Cycled_Rt_BoardZ", gaux);
//	gaux->SetTitle("Plane Z RM 2 (170 - 350 keV)");
	gaux->SetTitle("Plane Z RM 3 (350 - 700 keV)");
//	gaux->SetTitle("Cyclic ratemeter board Z");
	gaux->UseCurrentStyle();
	//gaux->GetXaxis()->SetLabelSize(0.06);
	//gaux->GetXaxis()->SetTitleSize(0.06);
	gaux->GetXaxis()->SetTitle("Time ");
	gaux->GetXaxis()->CenterTitle();
	//gaux->GetYaxis()->SetLabelSize(0.06);
	//gaux->GetYaxis()->SetTitleSize(0.06);
	gaux->GetYaxis()->SetTitle("Counts/s");
	gaux->GetYaxis()->CenterTitle();
	gaux->GetYaxis()->SetRangeUser(0., 150.);
//	gaux->GetXaxis()->SetTimeDisplay(1);
//	gaux->GetXaxis()->SetTimeOffset(da.Convert());
//	gaux->GetXaxis()->SetTimeFormat("%H:%M");
//	gaux->GetXaxis()->SetRangeUser(tini, tend);
	npt = gaux->GetN();
	minx = TMath::MinElement(npt, gaux->GetX());
	maxx = TMath::MaxElement(npt, gaux->GetX());
	gaux->GetXaxis()->SetLimits(minx, maxx);
	maxy = TMath::MaxElement(npt, gaux->GetY());
	gaux->GetHistogram()->SetMaximum(1.1*maxy);
	gaux->GetHistogram()->SetMinimum(0.);
	gaux->Draw("AP");

	c1->cd(3);
	fhk->GetObject("M6215_0_MCAL_ZBARS_SIDE_A_TEMP", gaux);
	gaux->UseCurrentStyle();
	gaux->GetXaxis()->SetTitle("Time ");
	gaux->GetXaxis()->CenterTitle();
	gaux->GetYaxis()->SetTitle("ZA temperature (°C)");	//"ADC counts"
	gaux->GetYaxis()->CenterTitle();
//	gaux->GetXaxis()->SetTimeDisplay(1);
//	gaux->GetXaxis()->SetTimeOffset(da.Convert());
//	gaux->GetXaxis()->SetTimeFormat("%H:%M");
//	gaux->GetXaxis()->SetRangeUser(tini, tend);
	gaux->Draw("AP");

	c1->cd(4);
	fhk->GetObject("M6210_0_MCAL_PD_BIAS", gaux);
	gaux->UseCurrentStyle();
	gaux->GetXaxis()->SetTitle("Time ");
	gaux->GetXaxis()->CenterTitle();
	gaux->GetYaxis()->SetTitle("ADC counts");
	gaux->GetYaxis()->CenterTitle();
//	gaux->GetXaxis()->SetTimeDisplay(1);
//	gaux->GetXaxis()->SetTimeOffset(da.Convert());
//	gaux->GetXaxis()->SetTimeFormat("%H:%M");
//	gaux->GetXaxis()->SetRangeUser(tini, tend);
//	gaux->GetYaxis()->SetRangeUser(115., 135.);
	gaux->Draw("AP");
*/
	c1->cd(1);
	fhk->GetObject("M6244_0_MCAL_Input_Current", gaux);
	gaux->UseCurrentStyle();
	gaux->GetXaxis()->SetTitle("Time (s)");
	gaux->GetXaxis()->CenterTitle();
	gaux->GetYaxis()->SetTitle("ADC counts");
	gaux->GetYaxis()->CenterTitle();
//	gaux->GetXaxis()->SetTimeDisplay(1);
//	gaux->GetXaxis()->SetTimeOffset(da.Convert());
//	gaux->GetXaxis()->SetTimeFormat("%H:%M");
//	gaux->GetXaxis()->SetRangeUser(tini, tend);
//	gaux->GetYaxis()->SetRangeUser(60., 120.);
	gaux->SetMarkerColor(2);
	gaux->SetLineColor(2);
	gaux->Draw("LPSAME");

// plot run id
/*	MCVFileInfo *info = new MCVFileInfo();
	TTree *gdata = (TTree *) f->Get("runInfo");
	TBranch *br = gdata->GetBranch("run_info");
	br->SetAddress(&info);
	int nrun = gdata->GetEntries();
	TText *t;
	TLine *l;
	for (int i=0; i<nrun; i++) {
		gdata->GetEntry(i);
		if (info->tstart>tini && info->tstart<tend) {
			c1->cd(1);
			TDatime dc;
			TString srun;
			srun += info->runid;
			dc.Set(ti+info->tstart);    // NB: TERMINE -3600 SOLO PER LE MISURE PRESE DOPO IL PASSAGGIO ALL'ORA LEGALE 
			printf("%d \t%d \t%s \t%6.0f \t\t%7d \t%7.1f \t%d\n", info->runid, info->pkttype, dc.AsString(), info->teff, info->nevents, info->nevents/info->teff, info->pkttype);
			t = new TText(0.5*(info->tstart + info->tstop), 60., srun.Data());    // NB: TERMINE -3600 COME SOPRA
			t->SetTextAngle(90.);
			t->SetTextColor(2);			
			t->SetTextSize(0.08);			
			t->Draw();
			l = new TLine(info->tstart, 0., info->tstart, 1000.);
			l->SetLineColor(2);
			l->Draw(); 
			c1->cd(2);
			l->Draw(); 
		}
	}
*/
	f->Close();

// draw separation lines and text labels
/*
int nsep = 36;	
int month[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
int day[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5};
int h[] = {4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 12, 13,   14, 14, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 23, 0, 1, 1, 2, 3, 4, 4, 17, 17, 13, 14};
int min[] = {30, 5, 5, 40, 40, 15, 15, 50, 50, 25, 25, 0,  0, 35, 35, 10, 10, 45, 45, 20, 20, 55, 55, 30, 30, 5, 5, 40, 40, 15, 15, 50, 15, 50, 41, 0};
int sec[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lid[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,    1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
char *label[] = {"carica", "scarica"};

float *ts = new float[nsep];
for (int j=0; j<nsep; j++) {	// build the time array in seconds
	TDatime ds(2007, month[j], day[j], h[j], min[j], 0);
	ts[j] = ds.Convert() - da.Convert();
}

for (int j=0; j<nsep; j++) {
	TLine *l2 = new TLine(ts[j], -20., ts[j], 200.);
	l2->SetLineColor(4);
	for (int i=1; i<=5; i++) {
		c1->cd(i);
		l2->Draw();
	}
	TString tt(label[lid[j]]);
	if (j<nsep-1) {
		TText *t = new TText(0.5*(ts[j]+ts[j+1]), 90., tt.Data());
		t->SetTextSize(0.06);
		t->SetTextAngle(90.);
		t->SetTextColor(4);
		t->Draw();
	}
}
*/
c1->Update();

}
