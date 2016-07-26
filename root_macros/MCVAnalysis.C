// test the Lomb normalized periodogram implemented in class MCVAnalysis
// M.Marisaldi 20/1/06

void run()
{
// load data from file

gROOT->Reset();
TFile *fp = new TFile("RT003341.root");
fp->Print();
TTree *t2 = (TTree *) fp->Get("tdata");
t2->Print();
TH1D *ht = new TH1D("ht", "ht", 5000, 0., 5000000000.);
t2->Draw("time>> ht");
ht->Draw();
TH1D *hA1 = new TH1D("hA1", "hA1", 4096, 0., 4095.);
TH1F *hE = new TH1F("hE", "hE", 10000, 0., 200.);

 MCVAnalysis *an = new MCVAnalysis(t2);
 an->SetOfac(2.);
 an->SetHifac(100.);
 an->DoLombNormPeriod(0., 2.E8, 20, 1);
 TGraph *gr = an->GetLombNormPeriod();
 TCanvas *c2 = new TCanvas();
 gr->Draw("ALP");
}


void sim()
{
// work on a simulated dataset

    gROOT->Reset();

    float off=200.;   // offset
    float amp=0.5;     // periodic sigmal amplitude
    float w=314.;     // periodic signal angular frequency (rad/s)
    float noise=2.;   // gaussian noise rms
    float fsig=30.;   // mean frequency of sampled signal (Hz)
    float ttot=100.; // total sampling time (s)

    Int_t nevt=ttot*fsig;
    Float_t *xtmp = new Float_t[nevt];
    Float_t *x = new Float_t[nevt];
    Float_t *y = new Float_t[nevt];
    Int_t *index = new Int_t[nevt];
    gRandom->SetSeed();
    gRandom->RndmArray(nevt, xtmp);
    TMath::Sort(nevt, xtmp, index, kFALSE);
    for (int i=0; i<nevt; i++) {
	x[i] = xtmp[index[i]]*ttot;
	y[i] = off + amp*sin(w*x[i]) + gRandom->Gaus(0.,noise);
    }
    for (int i=0; i<100; i++) cout << i << "  " << x[i] << "  " << y[i] << endl;
    delete xtmp;
    delete index;
    TGraph *data = new TGraph(nevt, x, y);
    data->Draw("ALP");

    MCVAnalysis *an = new MCVAnalysis();
    an->SetOfac(2.);
    an->SetHifac(10.);
    an->DoLombNormPeriod(x, y, nevt);
    TGraph *gr = an->GetLombNormPeriod();
    TCanvas *c2 = new TCanvas();
    gr->Draw("ALP");

}

void corr()
{
    int bar1=20;
    int pd1=0;
    int bar2=20;
    int pd2=1;
    float tstart=0.;
    float tstop=1000.E6;
    TFile *fp = new TFile("/data1/AGILE_MCAL_AIV_Data/test/RT003058.root");
    TTree *t2 = (TTree *) fp->Get("tdata");
    MCVFileInfo *info = (MCVFileInfo *) fp->Get("run003058_3902_info");
    info->PrintFileInfo();
    info->PrintCalibrationParameters();
    MCVAnalysis *an = new MCVAnalysis(t2, info);
    cout << "corr. coeff.= " << an->GetLinearCorrelation(bar1, pd1, bar2, pd2, tstart, tstop) << endl;
}

void corr2()
{
    gStyle->SetPalette(1,0);
    float tstart=0.;
    float tstop=500.E6;
    TFile *fp = new TFile("/data1/mcalsw_output/RT/RT005413_3902.root");
    fp->ls();
    TTree *t2 = (TTree *) fp->Get("tdata");
    MCVFileInfo *info = (MCVFileInfo *) fp->Get("run005413_3902_info");
    info->PrintFileInfo();
    info->PrintCalibrationParameters();
    MCVAnalysis *an = new MCVAnalysis(t2, info);
    TH2F *hc = an->GetCorrelationMatrix(tstart, tstop);
    hc->GetXaxis()->SetTitle("XA - XB - ZA - ZB");
    hc->GetXaxis()->CenterTitle();
    hc->GetYaxis()->SetTitle("XA - XB - ZA - ZB");
    hc->GetYaxis()->CenterTitle();
    hc->SetTitle("run 5413 correlation matrix");
    hc->SetStats(0);
    hc->Draw("COLZ");
}

