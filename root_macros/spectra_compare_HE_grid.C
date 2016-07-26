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
char f_3901[]="5853_E.root";
char f_3902[]="10479_E.root";

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_3901);
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
hi->SetTitle("MCAL reconstructed energy spectra");
hi->Rebin(50);
//hi->Scale(1./0.02);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

sprintf(filepath, "%s%s", datadir, f_3902);
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


TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "GRID observation 39.01 run 5839", "l");
leg->AddEntry(he, "GRID physical calibration 39.02 run 10479", "l");
leg->Draw();


}
