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

char datadir[]="/data1/AGILE_MCAL_AIV_Data/CGS_Tortona/MCAL_AC/rootfiles/";
char f_default[]="5894_E.root";
char f_AC00[]="5898_E.root";
char f_AC3030[]="5899_E.root";
char f_AC3131[]="5900_E.root";
char f_AC3131top[]="5901_E.root";

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_default);
TFile fi(filepath) ;
TPad *pi;
fi.GetObject("ec0_15;1", pi);
TH1F *hi;
hi = (TH1F *) (pi->GetPrimitive("totE"));

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetStats(0);
hi->SetTitle("Background reconstructed energy spectra");
hi->Rebin(50);
//hi->Scale(1./0.02);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

sprintf(filepath, "%s%s", datadir, f_AC00);
TFile fe(filepath) ;
TPad *pe;
fe.GetObject("ec0_15;1", pe);
TH1F *he;
he = (TH1F *) (pe->GetPrimitive("totE"));
he->SetStats(0);
he->Rebin(50);
//he->Scale(1./0.02);
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("same");

sprintf(filepath, "%s%s", datadir, f_AC3030);
TFile f3(filepath) ;
TPad *p3;
f3.GetObject("ec0_15;1", p3);
TH1F *h3;
h3 = (TH1F *) (p3->GetPrimitive("totE"));
h3->SetStats(0);
h3->Rebin(50);
//h3->Scale(4322./800.);
//h3->Scale(1./0.02);
h3->SetLineColor(3);
h3->SetMarkerColor(3);
h3->Draw("same");


sprintf(filepath, "%s%s", datadir, f_AC3131);
TFile f4(filepath) ;
TPad *p4;
f4.GetObject("ec0_15;1", p4);
TH1F *h4;
h4 = (TH1F *) (p4->GetPrimitive("totE"));
h4->SetStats(0);
h4->Rebin(50);
//h4->Scale(1./0.02);
h4->SetLineColor(4);
h4->SetMarkerColor(4);
h4->Draw("same");


sprintf(filepath, "%s%s", datadir, f_AC3131top);
TFile f5(filepath) ;
TPad *p5;
f5.GetObject("ec0_15;1", p5);
TH1F *h5;
h5 = (TH1F *) (p5->GetPrimitive("totE"));
h5->SetStats(0);
h5->Rebin(50);
//h4->Scale(1./0.02);
h5->SetLineColor(5);
h5->SetMarkerColor(5);
h5->Draw("same");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "run 5894 - AC off", "l");
leg->AddEntry(he, "run 5895 - AC on, top off, w=d=0", "l");
leg->AddEntry(h3, "run 5899 - AC on, top off, w=d=30", "l");
leg->AddEntry(h4, "run 5900 - AC on, top off, w=d=31", "l");
leg->AddEntry(h5, "run 5901 - AC on, top on, w=d=31", "l");
leg->Draw();

cout << "run 5894 integral for E>3 MeV " << hi->Integral(3, 100) << endl;
cout << "run 5898 integral for E>3 MeV " << he->Integral(3, 100) << endl;
cout << "run 5899 integral for E>3 MeV " << h3->Integral(3, 100) << endl;
cout << "run 5900 integral for E>3 MeV " << h4->Integral(3, 100) << endl;
cout << "run 5901 integral for E>3 MeV " << h5->Integral(3, 100) << endl;

/*
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
*/

}
