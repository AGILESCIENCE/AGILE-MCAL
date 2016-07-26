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
char f_pre_beam[]="2245_E_PRE-BEAM.root";
char f_beam[]="2245_E_BEAM.root";

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_pre_beam);
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
//hi->SetTitle("MCAL reconstructed energy spectra");
hi->SetTitle(0);
//hi->Rebin(50);
hi->Scale(1./0.02);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

sprintf(filepath, "%s%s", datadir, f_beam);
TFile fe(filepath) ;
TPad *pe;
fe.GetObject("ec0_15;1", pe);
TH1F *he;
he = (TH1F *) (pe->GetPrimitive("totE"));
he->SetStats(0);
//he->Rebin(50);
he->Scale(1./0.02);
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("same");
gPad->SetTicks(1,1);

TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 100000, 0, 2000.);
//TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 2000, 0, 2000.);
for (int i=0; i<100000; i++) hdiff->SetBinContent(i, he->GetBinContent(i) - hi->GetBinContent(i));
hdiff->SetLineColor(3);
hdiff->SetMarkerColor(3);
hdiff->Draw("same");


cout << "Excess rate = " << (he->Integral() - hi->Integral())*0.02 << " evt/s" << endl;


TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "beam OFF", "lp");
leg->AddEntry(he, "beam ON", "lp");
leg->Draw();

float ti = 240.;
float te = 240.;
float x[100000], ex[100000], y[100000], ey[100000];
for (int i=0; i<2000; i++) {
    x[i] = i*0.02+0.01;
    ex[i] = 0.01;
    y[i] = he->GetBinContent(i) - hi->GetBinContent(i);    
    ey[i] = sqrt(hi->GetBinContent(i)/ti + he->GetBinContent(i)/te);
}

TGraphErrors *gr1 = new TGraphErrors(100000, x, y, ex, ey);
TCanvas *c1;
c2 = new TCanvas();
c2->Draw();
gPad->SetLogx();
gPad->SetLogy();
gr1->Draw("AP");

}
