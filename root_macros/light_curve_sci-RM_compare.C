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
TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000345_3913.root");

TObjArray *ai = (TObjArray *) fi->Get("rawHisto");
//TObjArray *ae = (TObjArray *) fe->Get("rawHisto");

//TGraphErrors *hi;
TObjArray *rawSciRM3913;
rawSciRM3913 = (TObjArray *) ai->FindObject("rawSciRM3913");

//hi = (TGraphErrors *) ai->FindObject("gRMX3913");
//hz = (TGraphErrors *) ai->FindObject("gRMZ3913");

TGraph *gaux = (TGraph *) rawSciRM3913->At(1);
int ns = gaux->GetN();
double *time = gaux->GetX();

double *data = new double[22*ns];

// retrieve data
for (int i=0; i<22; i++) {
	TGraph *gaux = (TGraph *) rawSciRM3913->At(i);
	double *y = gaux->GetY();
	for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
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

int bin = 10;	// rebin factor
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
//hz->SetLineColor(2);
//hz->Draw("SAME");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hx, "orbit 345 X-plane integral Sci. RM - AC ON nominal", "lp");
//leg->AddEntry(hz, "orbit 345 Z-plane integral Sci. RM", "lp");


// add a second plot
	TH1D *hx2, *hz2;

if (1) {
	TFile fe("/data1/mcalsw_output_COMMISSIONING/H/H000132_3913.root");
	
	TObjArray *ae = (TObjArray *) fe->Get("rawHisto");
	
	TObjArray *rawSciRM3913;
	rawSciRM3913 = (TObjArray *) ae->FindObject("rawSciRM3913");
	
	//hi = (TGraphErrors *) ai->FindObject("gRMX3913");
	//hz = (TGraphErrors *) ai->FindObject("gRMZ3913");
	
	TGraph *gaux = (TGraph *) rawSciRM3913->At(1);
	int ns = gaux->GetN();
	double *time = gaux->GetX();
	
	double *data = new double[22*ns];
	hx2 = new TH1D("hx2", "hx2", ns, 0., ns);
	hz2 = new TH1D("hz2", "hz2", ns, 0., ns);
	
	// retrieve data
	for (int i=0; i<22; i++) {
		TGraph *gaux = (TGraph *) rawSciRM3913->At(i);
		double *y = gaux->GetY();
		for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
	}
	
	// build histos for integral plane X plane Z RM vs time
	
	double *totx = new double[ns];
	double *totz = new double[ns];
	for (int j=0; j<ns; j++) {
		totx[j] = totz[j] = 0.;
		for (int i=1; i<11; i++) totx[j] += data[j*22 + i];
		for (int i=12; i<22; i++) totz[j] += data[j*22 + i];
	}
	
	for (int j=0; j<ns; j++) {
		hx2->SetBinContent(j+1, totx[j]);
		hz2->SetBinContent(j+1, totz[j]);
	}
	
	int bin = 10;	// rebin factor
	hx2->Rebin(bin);
	hz2->Rebin(bin);
	hx2->Scale(1./bin);
	hz2->Scale(1./bin);
	
	hx2->GetYaxis()->SetRangeUser(0., 500.);
	hx2->SetLineColor(2);
	hx2->SetStats(0);
	TString htitle("MCAL Scientific Ratemeters integrated light curve (E>650 keV, time bin ");
	htitle += bin;
	htitle += " s)";
	hx2->SetTitle(htitle.Data());
	hx2->GetXaxis()->SetTitle("Time (s)");
	hx2->GetXaxis()->CenterTitle();
	hx2->GetYaxis()->SetTitle("Counts / s");
	hx2->GetYaxis()->CenterTitle();
	hx2->Draw("SAME");
	hz2->SetLineColor(2);
//	hz->Draw("SAME");
	
	leg->AddEntry(hx2, "orbit 131 X-plane integral Sci. RM - AC OFF", "lp");
//	leg->AddEntry(hz, "orbit 345 Z-plane integral Sci. RM", "lp");
}

leg->Draw();

}
