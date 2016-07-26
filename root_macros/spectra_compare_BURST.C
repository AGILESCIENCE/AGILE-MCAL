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
mcalstyle->SetLabelSize(0.04, "X");
mcalstyle->SetLabelSize(0.04, "Y");
gROOT->SetStyle("mcalstyle");

	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
	};

//background data
TFile fi("~/local/mcalsw/bin/mcalsw_output/RT/RT013642_3909_sel_01.root");
double ti=200.;
fi.ls();

// burst data
TFile fe1("~/local/mcalsw/bin/mcalsw_output/RT/RT013643_3908_sel_01.root");
double te1=6.;
fe1.ls();

TFile fe2("~/local/mcalsw/bin/mcalsw_output/RT/RT013643_3908_sel_02.root");
double te2=6.;
fe2.ls();

TFile fe3("~/local/mcalsw/bin/mcalsw_output/RT/RT013643_3908_sel_03.root");
double te3=6.;
fe3.ls();

	TTree *tg = (TTree *) fi->Get("tdata_sel_0");
	int ng = tg->GetEntries();
	cout << "BKG " << ng << " entries; " << endl;
	
	// connect input trees
	MCALevt evtg;
	tg->SetBranchAddress("time", &evtg.time);
	tg->SetBranchAddress("mult", &evtg.mult);
	tg->SetBranchAddress("bar", evtg.bar);
	tg->SetBranchAddress("pdA", evtg.pdA);
	tg->SetBranchAddress("pdB", evtg.pdB);
	tg->SetBranchAddress("pos", evtg.pos);
	tg->SetBranchAddress("E", evtg.E);
	tg->SetBranchAddress("Etot", &evtg.Etot);

double Estep[] = {0.0, 0.17, 0.28, 0.35, 0.5, 0.7, 1., 1.4, 2., 2.8, 5.6, 11.2, 22.4, 45., 90., 200.};
int nE = 16;
TH1F *hEg = new TH1F("hEg", "hEg", nE-1, Estep);
TH1F *hEb = new TH1F("hEb", "hEb", nE-1, Estep);

// histo background events 
for (int i=0; i<ng; i++) {
	tg->GetEntry(i);
	hEg->Fill(evtg.Etot);
}

TTree *tb;
int nb;

// histo burst events 1
	tb = (TTree *) fe1->Get("tdata_sel_0");
	int nb = tb->GetEntries();
	cout << "burst 1 " << nb << " entries; " << endl;
	
	MCALevt evtb;
	tb->SetBranchAddress("time", &evtb.time);
	tb->SetBranchAddress("mult", &evtb.mult);
	tb->SetBranchAddress("bar", evtb.bar);
	tb->SetBranchAddress("pdA", evtb.pdA);
	tb->SetBranchAddress("pdB", evtb.pdB);
	tb->SetBranchAddress("pos", evtb.pos);
	tb->SetBranchAddress("E", evtb.E);
	tb->SetBranchAddress("Etot", &evtb.Etot);
	
	for (int i=0; i<nb; i++) {
		tb->GetEntry(i);
		hEb->Fill(evtb.Etot);
	}
/*
// histo burst events 2
	tb = (TTree *) fe2->Get("tdata_sel_1");
	int nb = tb->GetEntries();
	cout << "burst 2 " << nb << " entries; " << endl;
	
	MCALevt evtb;
	tb->SetBranchAddress("time", &evtb.time);
	tb->SetBranchAddress("mult", &evtb.mult);
	tb->SetBranchAddress("bar", evtb.bar);
	tb->SetBranchAddress("pdA", evtb.pdA);
	tb->SetBranchAddress("pdB", evtb.pdB);
	tb->SetBranchAddress("pos", evtb.pos);
	tb->SetBranchAddress("E", evtb.E);
	tb->SetBranchAddress("Etot", &evtb.Etot);
	
	for (int i=0; i<nb; i++) {
		tb->GetEntry(i);
		hEb->Fill(evtb.Etot);
	}

// histo burst events 3
	tb = (TTree *) fe3->Get("tdata_sel_2");
	int nb = tb->GetEntries();
	cout << "burst 3 " << nb << " entries; " << endl;
	
	MCALevt evtb;
	tb->SetBranchAddress("time", &evtb.time);
	tb->SetBranchAddress("mult", &evtb.mult);
	tb->SetBranchAddress("bar", evtb.bar);
	tb->SetBranchAddress("pdA", evtb.pdA);
	tb->SetBranchAddress("pdB", evtb.pdB);
	tb->SetBranchAddress("pos", evtb.pos);
	tb->SetBranchAddress("E", evtb.E);
	tb->SetBranchAddress("Etot", &evtb.Etot);
	
	for (int i=0; i<nb; i++) {
		tb->GetEntry(i);
		hEb->Fill(evtb.Etot);
	}
*/
// make background subtraction

float *x = new float[nE];
float *ex = new float[nE];
float *yg = new float[nE];
float *eyg = new float[nE];
float *yb = new float[nE];
float *eyb = new float[nE];
float *ys = new float[nE];
float *eys = new float[nE];
for (int i=0; i<nE; i++) {
	x[i]  = 0.5*(Estep[i+1] + Estep[i]);
	ex[i] = 0.5*(Estep[i+1] - Estep[i]);
}
TH1F *hEs = new TH1F("hEs", "hEs", nE-1, Estep);
double rb= te1/ti;
for (int i=1; i<nE; i++) {
	hEs->SetBinContent(i, hEb->GetBinContent(i) - hEg->GetBinContent(i)*rb);
	printf("%d \t%6.2f \t%6.2f \t%.2f \t%d \t%.2f\n", i, Estep[i-1], Estep[i], hEg->GetBinContent(i)*rb,  hEb->GetBinContent(i), hEb->GetBinContent(i) - hEg->GetBinContent(i)*rb);
	
	yg[i-1] = hEg->GetBinContent(i)*rb;
	eyg[i-1] = 3.*sqrt(yg[i-1]);
	
	yb[i-1] = hEb->GetBinContent(i);
	eyb[i-1] = 3.*sqrt(yb[i-1]);
	
	ys[i-1] = hEb->GetBinContent(i) - hEg->GetBinContent(i)*rb;
	eys[i-1] = 3.*sqrt(yg[i-1] + yb[i-1]);
	ys[i-1] = ys[i-1]/(te1*2*ex[i-1]);
	eys[i-1] = eys[i-1]/(te1*2*ex[i-1]);
}
TGraphErrors *gEg = new TGraphErrors(nE-1, x, yg, ex, eyg);
TGraphErrors *gEb = new TGraphErrors(nE-1, x, yb, ex, eyb);
TGraphErrors *gEs = new TGraphErrors(nE-1, x, ys, ex, eys);

// create graphics

	// graphic section
//	hEg->Scale(rb);
//	hEg->Draw();
//	hEb->SetLineColor(2);
//	hEb->Draw("SAME");
//	hEs->SetLineColor(3);
//	hEs->Draw("SAME");
	
	gEb->SetLineColor(2);
	gEb->Draw("AP");
	gEg->Draw("PSAME");
//	gEs->SetLineColor(3);
	TH2F *htmp = new TH2F("htmp", "htmp", 1000, 0.01, 10., 1000, 1., 10000.);
	htmp->SetStats(0);
//	htmp->Draw();
	htmp->GetXaxis()->SetTitle("Energy (MeV)");
	htmp->GetYaxis()->SetTitle("Counts / s MeV");
	htmp->GetXaxis()->SetLabelSize(0.04);
	htmp->GetYaxis()->SetLabelSize(0.04);
//	gEs->Draw("P");
	gPad->SetLogx();
	gPad->SetLogy();




}
