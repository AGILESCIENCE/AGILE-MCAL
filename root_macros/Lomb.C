// test the Lomb normalized periodogram implemented in class MCVAnalysis
// M.Marisaldi 20/1/06

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
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
gROOT->SetStyle("mcalstyle");
}

void run()
{
// load data from file

TFile *fp = new TFile("/data1/mcalsw_output/RT/RT002350_3902_sel_02.root");
fp->ls();
TTree *t2 = (TTree *) fp->Get("tdata_sel_1");
 MCVFileInfo *in= (MCVFileInfo *) fp->Get("run002350_3902_info_sel_1");
t2->Print();
TH1D *ht = new TH1D("ht", "ht", 5000, 0., 5000000000.);
t2->Draw("time>> ht");
ht->Draw();
TH1D *hA1 = new TH1D("hA1", "hA1", 4096, 0., 4095.);
TH1F *hE = new TH1F("hE", "hE", 10000, 0., 200.);

 MCVAnalysis *an = new MCVAnalysis(t2, in);
 an->SetOfac(4.);
 an->SetHifac(10.);
 an->SetADCRelativeLimit(2);
 an->DoLombNormPeriod(3.E9, 3.5E9, 20, 0);
 TGraph *gr = an->GetLombNormPeriod();
 TCanvas *c2 = new TCanvas();
 gr->Draw("ALP");
}

//------------------------------------------------------------------

void runAll()
{
// load data from file

TFile *fp = new TFile("/data1/mcalsw_output/RT/RT005470_3902.root");
fp->Print();
TTree *t2 = (TTree *) fp->Get("tdata");
 MCVFileInfo *in= (MCVFileInfo *) fp->Get("run005470_3902_info");
t2->Print();
TH1D *ht = new TH1D("ht", "ht", 5000, 0., 5000000000.);
t2->Draw("time>> ht");
ht->Draw();
TH1D *hA1 = new TH1D("hA1", "hA1", 4096, 0., 4095.);
TH1F *hE = new TH1F("hE", "hE", 10000, 0., 200.);

 MCVAnalysis *an = new MCVAnalysis(t2, in);
 an->SetOfac(4.);
 an->SetHifac(4.);
 an->SetADCRelativeLimit(2.);

 TGraph *gr=NULL, *gr2=NULL;
 TCanvas *c2 = new TCanvas("c2", "Lomb normalized periodogram", 800, 600);
 c2->Divide(6,5);

 float fmax[30], prob[30];
 int nevt[30];
 for (int i=0; i<30; i++) {
     c2->cd(i+1);
     int pd=1; // PD-A=0; PD-B=1
     an->DoLombNormPeriod(100.E6, 500.E6, i, pd);
     gr = an->GetLombNormPeriod();
     gr2 = (TGraph *) gr->Clone();
	gr2->GetXaxis()->SetTitle("Frequency (Hz)");
	gr2->GetXaxis()->SetTitleSize(0.05);
	gr2->GetXaxis()->CenterTitle();
//	gr2->GetYaxis()->SetTitle("");
//	gr2->GetYaxis()->CenterTitle();
//	gr2->GetYaxis()->SetTitleSize(0.05);
	fmax[i] = an->GetMaxFreq();
	nevt[i] = an->GetEvents();
	prob[i] = an->GetProb();
     gr2->Draw("ALP");
     c2->Update();
 }

 printf("bar \tPD \tevents \tmax. freq \tprobability\n");
 for (int i=0; i<30; i++) printf("%c%d \t%c \t%6d \t%7.2f \t\t%8.3E\n", (i<15 ? 'X' : 'Z'), (i<15 ? i+1 : i-14), (pd==0 ? 'A' : 'B'), nevt[i], fmax[i], prob[i]);
}

//------------------------------------------------------------------

void sim()
{
// work on a simulated dataset

    gROOT->Reset();

    float off=200.;   // offset
    float amp=1.3;     // periodic sigmal amplitude
    float w=188.5;     // periodic signal angular frequency (rad/s)  -  30Hz
//    float w=314.15;     // periodic signal angular frequency (rad/s)  -  50Hz
//    float w=31.415;     // periodic signal angular frequency (rad/s)  -  5Hz
    float noise=2.;   // gaussian noise rms
    float fsig=29.6;   // mean frequency of sampled signal (Hz)
    float ttot=364.; // total sampling time (s)

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
    data->SetTitle("Data");
	data->GetXaxis()->SetTitle("Time (s)");
	data->GetXaxis()->CenterTitle();
	data->GetXaxis()->SetTitleSize(0.05);
	data->GetYaxis()->SetTitle("Signal");
	data->GetYaxis()->CenterTitle();
	data->GetYaxis()->SetTitleSize(0.05);
    data->Draw("ALP");

    MCVAnalysis *an = new MCVAnalysis();
    an->SetOfac(8.);
    an->SetHifac(4.);
    an->DoLombNormPeriod(x, y, nevt);
    TGraph *gr = an->GetLombNormPeriod();
    gr->SetTitle("Lomb normalized periodogram");
    TCanvas *c2 = new TCanvas();
	gr->GetXaxis()->SetTitle("Frequency (Hz)");
	gr->GetXaxis()->SetTitleSize(0.05);
	gr->GetXaxis()->CenterTitle();
    gr->Draw("ALP");

}
