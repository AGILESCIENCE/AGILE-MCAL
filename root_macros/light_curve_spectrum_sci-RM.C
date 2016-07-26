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
//TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000843_3913.root");
TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H001155_3913.root");

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

int bin = 1;	// rebin factor
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
leg->AddEntry(hx, "orbit 345 X-plane integral Sci. RM", "lp");
leg->AddEntry(hz, "orbit 345 Z-plane integral Sci. RM", "lp");
leg->Draw();

// build histos for energy bands light curves
Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};

// plane X sci. RM
TCanvas *c2 = new TCanvas();
TLegend *legx = new TLegend(0.6, 0.6, 0.95, 0.95);
for (int i=1; i<11; i++) {
	TString hname("hx");
	hname += i;
	TH1D *hx = new TH1D(hname, hname, ns, 0., ns);
	for (int j=0; j<ns; j++) hx->SetBinContent(j+1, data[j*22 + i]);
	hx->Rebin(bin);
	hx->Scale(1./bin);
	hx->SetLineColor(i != 10 ? i : 13);
	if (i==1) {
		hx->GetYaxis()->SetRangeUser(0., 500.);
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

// plane Z sci. RM
TCanvas *c3 = new TCanvas();
TLegend *legz = new TLegend(0.6, 0.6, 0.95, 0.95);
for (int i=1; i<11; i++) {
	TString hname("hz");
	hname += i;
	TH1D *hz = new TH1D(hname, hname, ns, 0., ns);
	for (int j=0; j<ns; j++) hz->SetBinContent(j+1, data[j*22 + i + 11]);
	hz->Rebin(bin);
	hz->Scale(1./bin);
	hz->SetLineColor(i != 10 ? i : 13);
	if (i==1) {
		hz->GetYaxis()->SetRangeUser(0., 500.);
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
	legz->AddEntry(hz, eband, "lp");
}
legx->Draw("same");

// build bkg subtracted spectrum (for GRB only!)
if (1) {

	int tbkgini=1500.;	// bkg initial relative time
	int tbkgfin=1600.;	// bkg final relative time
	int tgrbini=1635.;	// grb initial relative time
	int tgrbfin=1636.;	// grb final relative time
	
	float bkgX[11];		// X bkg
	float grbtotX[11];	// X grb + bkg
	float bkgZ[11];		// Z bkg
	float grbtotZ[11];	// Z grb + bkg
	for (int j=0; j<11; j++) bkgX[j] = grbtotX[j] = bkgZ[j] = grbtotZ[j] = 0.;
	
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
			grbtotX[j] += data[i*22 + j];
			grbtotZ[j] += data[i*22 + j + 11];
		}
	}
	float dtgrb = time[tgrbfin]-time[tgrbini];
	cout << "GRB duration = " << dtgrb << " s" << endl;
	for (int j=1; j<11; j++) {	// normalization to 1 s
		grbtotX[j] /= dtgrb;
		grbtotZ[j] /= dtgrb;
	}
	
	TGraphErrors *gRMbkgX = new TGraphErrors(11);
	TGraphErrors *gRMbkgZ = new TGraphErrors(11);
	TGraphErrors *gRMgrbtotX = new TGraphErrors(11);
	TGraphErrors *gRMgrbtotZ = new TGraphErrors(11);
	TGraphErrors *gRMgrbX = new TGraphErrors(11);
	TGraphErrors *gRMgrbZ = new TGraphErrors(11);
	
	for (int j=1; j<11; j++) {
		gRMbkgX->SetPoint(j-1, tmpEband[j], bkgX[j]);
		gRMbkgX->SetPointError(j-1, tmpEerr[j], sqrt(bkgX[j]));
		gRMgrbtotX->SetPoint(j-1, tmpEband[j], grbtotX[j]);
		gRMgrbtotX->SetPointError(j-1, tmpEerr[j], sqrt(grbtotX[j]));
		gRMgrbX->SetPoint(j-1, tmpEband[j], grbtotX[j]-bkgX[j]);
		gRMgrbX->SetPointError(j-1, tmpEerr[j], sqrt(grbtotX[j]));
		
		gRMbkgZ->SetPoint(j-1, tmpEband[j], bkgZ[j]);
		gRMbkgZ->SetPointError(j-1, tmpEerr[j], sqrt(bkgZ[j]));
		gRMgrbtotZ->SetPoint(j-1, tmpEband[j], grbtotZ[j]);
		gRMgrbtotZ->SetPointError(j-1, tmpEerr[j], sqrt(grbtotZ[j]));
		gRMgrbZ->SetPoint(j-1, tmpEband[j], grbtotZ[j]-bkgZ[j]);
		gRMgrbZ->SetPointError(j-1, tmpEerr[j], sqrt(grbtotZ[j]));
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
	gRMgrbX->Draw("AP");

	// plot superimposed GRB and BKG spectra, for plane Z
	c4->cd(2);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gRMgrbtotZ->SetLineColor(2);
	gRMgrbtotZ->GetXaxis()->SetLimits(0.1, 2000.);
	double maxy = TMath::MaxElement(11, gRMgrbtotZ->GetY());
	gRMgrbtotZ->GetHistogram()->SetMaximum(1.1*maxy);
	double miny = TMath::MinElement(11, gRMgrbtotZ->GetY());
	gRMgrbtotZ->GetHistogram()->SetMinimum(0.9*miny);
	gRMgrbtotZ->GetXaxis()->SetTitle("Energy (MeV)");
	gRMgrbtotZ->GetXaxis()->SetTitleOffset(1.4);
	gRMgrbtotZ->GetXaxis()->CenterTitle();
	gRMgrbtotZ->GetYaxis()->SetTitle("Counts/s");
	gRMgrbtotZ->GetYaxis()->CenterTitle();
	gRMgrbtotZ->GetYaxis()->SetTitleOffset(1.5);
	gRMgrbtotZ->SetTitle("plane X: GRB + BKG count spectra");
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
	gRMgrbZ->Draw("AP");
	
	//write count spectra
	printf("%E_lo E_high \tX_cnts \t\tX_sigma \tZ_cnts \t\tZ_sigma\n");
	for (int j=1; j<11; j++) {
		printf("%9.3f - %9.3f \t%7.1f \t%7.1f \t%7.1f \t%7.1f\n", tmpEband[j]-tmpEerr[j], tmpEband[j]+tmpEerr[j], grbtotX[j]-bkgX[j], sqrt(grbtotX[j]), grbtotZ[j]-bkgZ[j], sqrt(grbtotZ[j])); 
	}
}
}
		gRMgrbX->SetPoint(j-1, tmpEband[j], grbtotX[j]-bkgX[j]);
		gRMgrbX->SetPointError(j-1, tmpEerr[j], sqrt(grbtotX[j]));
