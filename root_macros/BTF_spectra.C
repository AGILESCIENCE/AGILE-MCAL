{
gROOT->Reset();
TFile fi("E_inside.root") ;
TPad *pi;
fi.GetObject("ec0_15;1", pi);
TH1F *hi;
hi = (TH1F *) (pi->GetPrimitive("seltotE"));

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetStats(0);
hi->SetTitle("run2245 BEAM ON - Beam spot reconstructed energy spectrum");
hi->Scale(208./240.);
hi->Rebin(5);
hi->Scale(10.);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

TFile fe("E_esterni.root") ;
TPad *pe;
fe.GetObject("ec0_15;1", pe);
TH1F *he;
he = (TH1F *) (pe->GetPrimitive("seltotE"));

TCanvas *c2;
c2 = new TCanvas();
c2->Draw();
gPad->SetLogx();
gPad->SetLogy();
he->SetStats(0);
he->SetTitle("run2245 BEAM ON - Region outside beam spot reconstructed energy spectrum");
he->Scale(224./240.);
he->Rebin(5);
he->Scale(10.);
he->GetYaxis()->SetTitle("Counts / (s MeV)");
he->Draw("");

TFile fb("E_bkg_beam.root") ;
TPad *pb;
fb.GetObject("ec0_15;1", pb);
TH1F *hb;
hb = (TH1F *) (pb->GetPrimitive("seltotE"));
TCanvas *c3;
c3 = new TCanvas();
c3->Draw();
gPad->SetLogx();
gPad->SetLogy();
hb->SetStats(0);
hb->SetTitle("run2245 BKG during beam operation (outside beam spot) spectrum");
//hb->Scale(224./240.);
hb->Rebin(5);
hb->Scale(10.);
hb->GetYaxis()->SetTitle("Counts / (s MeV)");
hb->Draw("");

TFile fn("E_bkg_no-beam_selection.root") ;
TPad *pn;
fn.GetObject("ec0_15;1", pn);
TH1F *hn;
hn = (TH1F *) (pn->GetPrimitive("seltotE"));
TCanvas *c4;
c4 = new TCanvas();
c4->Draw();
gPad->SetLogx();
gPad->SetLogy();
hn->SetStats(0);
hn->SetTitle("run2245 Background (NO BEAM, outside beam spot) spectrum");
hn->Rebin(5);
hn->Scale(10.);
hn->GetYaxis()->SetTitle("Counts / (s MeV)");
hn->Draw("");

}
