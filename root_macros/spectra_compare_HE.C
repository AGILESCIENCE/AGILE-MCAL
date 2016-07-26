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

char datadir[]="/data1/AGILE_MCAL_AIV_Data/rootfiles/";
char f_iasf[]="628_E.root";
char f_laben1[]="1106_E.root";
char f_dafne_bkg[]="2143_E_beam-off.root";
//char f_laben2[]="3093_E.root";
char f_iabg[]="3939_totE.root";

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_iasf);
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

sprintf(filepath, "%s%s", datadir, f_laben1);
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

sprintf(filepath, "%s%s", datadir, f_dafne_bkg);
TFile f3(filepath) ;
TPad *p3;
f3.GetObject("ec0_15;1", p3);
TH1F *h3;
h3 = (TH1F *) (p3->GetPrimitive("seltotE"));
h3->SetStats(0);
h3->Rebin(50);
h3->Scale(4322./800.);
//h3->Scale(1./0.02);
h3->SetLineColor(3);
h3->SetMarkerColor(3);
h3->Draw("same");

/*
sprintf(filepath, "%s%s", datadir, f_laben2);
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
*/

sprintf(filepath, "%s%s", datadir, f_iabg);
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

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "IASF-Bo 16/07/05, \nrun  628, MCAL vertical", "l");
leg->AddEntry(he, "LABEN   24/09/05, run 1106, MCAL horizontal", "l");
leg->AddEntry(h3, "BTF     07/11/05, run 2143, MCAL vertical", "l");
//leg->AddEntry(h4, "LABEN   29/11/05, run 3093, MCAL horizontal", "l");
leg->AddEntry(h4, "IABG    30/01/06, run 3939, MCAL horizontal", "l");
leg->Draw();

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
