{
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
}
