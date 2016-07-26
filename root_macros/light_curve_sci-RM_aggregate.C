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
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
gROOT->SetStyle("mcalstyle");


// first commissioning 3913 file
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H000130_3913.root");
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H000132_3913.root");
//TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000132_3913.root");
//TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000214_3913.root");
//TFile fi("~/mcalsw_output_COMMISSIONING/H/H000229_3909.root");
//TFile *fi= new TFile("/data1/mcalsw_output_COMMISSIONING/misc/MCAL_global_data_COMMISSIONING_070511.root");
TFile *fi= new TFile("/data1/mcalsw_output_COMMISSIONING/misc/MCAL_global_data_STAB_TERMICA_070515.root");


TGraph *gaux = (TGraph *) fi->Get("grmX1");
int ns = gaux->GetN();
double *time = gaux->GetX();

double *data = new double[22*ns];
char gname[10];

// retrieve data
for (int i=0; i<22; i++) {
	if (i<11) sprintf(gname, "grmX%d", i+1);
	else sprintf(gname, "grmZ%d", i-10);
	TGraph *gaux=NULL;
	fi->GetObject(gname, gaux);
	if (gaux) {
		double *y = gaux->GetY();
		for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
	}
}

// build histos for integral plane X plane Z RM vs time

double *totx = new double[ns];
double *totz = new double[ns];
for (int j=0; j<ns; j++) {
	totx[j] = totz[j] = 0.;
	for (int i=1; i<11; i++) totx[j] += data[j*22 + i];
	for (int i=12; i<22; i++) totz[j] += data[j*22 + i];
}

TH1D *hx = new TH1D("hx", "hx", ns, 0., ns);
TH1D *hz = new TH1D("hz", "hz", ns, 0., ns);
for (int j=0; j<ns; j++) {
	hx->SetBinContent(j+1, totx[j]);
	hz->SetBinContent(j+1, totz[j]);
}

int bin = 100;	// rebin factor
hx->Rebin(bin);
hz->Rebin(bin);
hx->Scale(1./bin);
hz->Scale(1./bin);

TCanvas *c1 = new TCanvas();
hx->GetYaxis()->SetRangeUser(0., 500.);
hx->SetLineColor(4);
hx->SetStats(0);
TString htitle("MCAL Scientific Ratemeters integrated light curve (E>650 keV, time bin ");
htitle += bin;
htitle += " s)";
hx->SetTitle(htitle.Data());
hx->GetXaxis()->SetTitle("Time (s)");
hx->GetXaxis()->CenterTitle();
hx->GetYaxis()->SetTitle("Counts / s");
hx->GetYaxis()->CenterTitle();
hx->Draw();
hz->SetLineColor(2);
hz->Draw("SAME");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hx, "X-plane integral Sci. RM", "lp");
leg->AddEntry(hz, "Z-plane integral Sci. RM", "lp");
leg->Draw();

// build histos for energy bands light curves
Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};

// plane X light curves
TCanvas *c2 = new TCanvas();
TLegend *legx = new TLegend(0.6, 0.6, 0.95, 0.95);
for (int i=1; i<11; i++) {
	TString hname("hx");
	hname += i;
	TH1D *hx = new TH1D(hname, hname, ns, 0., ns);
	for (int j=0; j<ns; j++) hx->SetBinContent(j+1, data[j*22 + i]);
	hx->Rebin(bin);
	hx->Scale(1./bin);
	hx->SetLineColor(i);
	if (i==1) {
		hx->GetYaxis()->SetRangeUser(0., 500.);
		hx->SetLineColor(i);
		hx->SetStats(0);
		TString htitle("MCAL Scientific Ratemeters Plane X light curve");
		hx->SetTitle();
		hx->GetXaxis()->SetTitle("Time (sec)");
		hx->GetXaxis()->CenterTitle();
		hx->GetYaxis()->SetTitle("Counts / s");
		hx->GetYaxis()->CenterTitle();
		hx->Draw();
	}
	else hx->Draw("same");
	char eband[100];
	sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
	legx->AddEntry(hx, eband, "lp");
}
legx->Draw("same");

// plane Z light curves
TCanvas *c3 = new TCanvas();
TLegend *legz = new TLegend(0.6, 0.6, 0.95, 0.95);
for (int i=1; i<11; i++) {
	TString hname("hz");
	hname += i;
	TH1D *hz = new TH1D(hname, hname, ns, 0., ns);
	for (int j=0; j<ns; j++) hz->SetBinContent(j+1, data[j*22 + i+11]);
	hz->Rebin(bin);
	hz->Scale(1./bin);
	hz->SetLineColor(i);
	if (i==1) {
		hz->GetYaxis()->SetRangeUser(0., 500.);
		hz->SetLineColor(i);
		hz->SetStats(0);
		TString htitle("MCAL Scientific Ratemeters Plane Z light curve");
		hz->SetTitle();
		hz->GetXaxis()->SetTitle("Time (sec)");
		hz->GetXaxis()->CenterTitle();
		hz->GetYaxis()->SetTitle("Counts / s");
		hz->GetYaxis()->CenterTitle();
		hz->Draw();
	}
	else hz->Draw("same");
	char eband[100];
	sprintf(eband, "%5.2f - %5.2f MeV", tmpEband[i] - tmpEerr[i], tmpEband[i] + tmpEerr[i]);
	legz->AddEntry(hx, eband, "lp");
}
legz->Draw("same");

}
