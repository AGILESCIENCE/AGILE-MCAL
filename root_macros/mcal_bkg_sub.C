{
gROOT->Reset();
TFile *fp = new TFile("bkg_energy.root");

TCanvas *c1 = (TCanvas *) fp->Get("ec5");
c1->Draw();
TPad *p1 = (TPad *) c1->FindObject("ec5_1");

TH1F *h1 = (TH1F *) p1->GetPrimitive("BAR0_E");
TCanvas *c2 = new TCanvas();
h1->Draw(); 


TFile *fp2 = new TFile("na22_x1.root");
TPad *p2 = (TPad *) fp2->Get("ec5_1"); 
TH1F *h2 = (TH1F *) p2->GetPrimitive("BAR0_E");
h2->Draw("SAME");

h1->SetNormFactor(38528);
TH1F *h3 = new TH1F("bkg_sub", "bkg_sub", 2000, 0., 50.);
for (int i=0; i< 2000; i++) h3->SetBinContent(i, h2->GetBinContent(i) - 0.404698*h1->GetBinContent(i));
h3->Draw("SAME");

}
