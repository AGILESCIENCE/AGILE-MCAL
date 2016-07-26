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
char f_pre_beam[]="2556_E_BKG.root";
char f_beam[]="2556_E_BEAM.root";
float ti = 400.;
float te = 200.;

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_pre_beam);
TFile fi(filepath) ;
TPad *pi;
fi.GetObject("ec0_15;1", pi);
TH1F *hi;
hi = (TH1F *) (pi->GetPrimitive("totE"));
sprintf(filepath, "%s%s", datadir, f_beam);
TFile fe(filepath) ;
TPad *pe;
fe.GetObject("ec0_15;1", pe);
TH1F *he;
he = (TH1F *) (pe->GetPrimitive("totE"));
he->SetStats(0);

cout << "Estimated beam flux E>0.7 MeV = " << he->Integral(35, 100000) - hi->Integral(35, 100000) << endl;
cout << "Estimated beam flux E>1.4 MeV = " << he->Integral(70, 100000) - hi->Integral(70, 100000) << endl;
cout << "Estimated beam flux E>2.8 MeV = " << he->Integral(140, 100000) - hi->Integral(140, 100000) << endl;
cout << "Estimated beam flux E>5.6 MeV = " << he->Integral(280, 100000) - hi->Integral(280, 100000) << endl;

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetStats(0);
//hi->SetTitle("MCAL reconstructed energy spectra");
hi->SetTitle(0);
hi->Rebin(50);
//hi->Scale(1./0.02);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

he->Rebin(50);
//he->Scale(1./0.02);
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("same");
gPad->SetTicks(1,1);

//TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 100000, 0, 2000.);
TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 2000, 0, 2000.);
for (int i=0; i<100000; i++) hdiff->SetBinContent(i, he->GetBinContent(i) - hi->GetBinContent(i));
hdiff->SetLineColor(3);
hdiff->SetMarkerColor(3);
hdiff->Draw("same");

double rate = hdiff->Integral();
cout << "Excess rate = " << rate << " evt/s " << endl;


TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "beam OFF", "lp");
leg->AddEntry(he, "beam ON", "lp");
leg->Draw();

float de = 1.;
float hif = 1./(ti*de);
float hef = 1./(te*de);
float x[2000], ex[2000], y[2000], ey[2000];
for (int i=0; i<2000; i++) {
    x[i] = i+0.5;
    ex[i] = 0.5;
    y[i] = (he->GetBinContent(i+1) - hi->GetBinContent(i+1))/rate;    
    ey[i] = sqrt(hi->GetBinContent(i+1)*hif + he->GetBinContent(i+1)*hef)/rate;
}

TGraphErrors *gr1 = new TGraphErrors(2000, x, y, ex, ey);
TCanvas *c2;
c2 = new TCanvas();
c2->Draw();
TH2F *haux = new TH2F("haux", "", 100, 0.1, 3000., 100, 0.00001, 1.);
haux->SetTitle("Background subtracted BEAM-ON MCAL reconstructed energy spectrum - normalized to 1");
haux->GetXaxis()->SetTitle("Energy (MeV)");
haux->GetYaxis()->SetTitle("Probability / MeV");
haux->SetStats(0);
haux->Draw();
gPad->SetLogx();
gPad->SetLogy();
gr1->Draw("PSAME");

}
