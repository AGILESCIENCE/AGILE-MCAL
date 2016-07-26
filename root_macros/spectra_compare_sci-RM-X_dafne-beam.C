{
gROOT->Reset();
char datadir[]="/data1/AGILE_MCAL_AIV_Data/rootfiles/";
char f_beam_off[]="2245_SciRM_BEAM-OFF.root";
char f_beam_on[]="2245_SciRM_BEAM-ON.root";

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

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_beam_off);
TFile fi(filepath) ;
TPad *pi;
fi.GetObject("ec0_8;1", pi);
TGraphErrors *hi;
hi = (TGraphErrors *) (pi->GetPrimitive("gRMX3913"));

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
//hi->SetStats(0);
hi->SetTitle("Scientific Ratemeters: X-plane spectra");
hi->GetYaxis()->SetTitle("Counts / s");
hi->SetLineColor(1);
hi->SetMarkerColor(1);
hi->Draw("AP");


sprintf(filepath, "%s%s", datadir, f_beam_on);
TFile fe(filepath) ;
TPad *pe;
fe.GetObject("ec0_8;1", pe);
TGraphErrors *he;
pe->ls();
he = (TGraphErrors *) (pe->GetPrimitive("gRMX3913"));
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("P");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(he, "run 2245 BEAM ON", "l");
leg->AddEntry(hi, "run 2245 BEAM OFF", "l");
leg->Draw();

// subtract bkg

double *x, *ex, *y1, *y2, *ey1, *ey2;
x = hi->GetX();
ex = hi->GetEX();
y1 = hi->GetY();
ey1 = hi->GetEY();
y2 = he->GetY();
ey2 = he->GetEY();
int nn = hi->GetN();
double *ey = new double[nn];
double *y = new double[nn];

y[0]=0;
ey[0]=0;
double tot=0.;
printf("band \tE_lo (MeV) \tE_hi (MeV) \tN (evt/s) \terr\n");
for (int i=1; i<nn; i++) {
    y[i] = y2[i] - y1[i];   
    tot += y[i];
    ey[i] = sqrt(ey1[i]*ey1[i] + ey2[i]*ey2[i]);
//    cout << i << " " << x[i] << " " << ex[i] << " " << y[i] << " " << ey[i] << endl;
    printf("%d \t%7.2f \t%7.2f \t%6.2f \t\t%6.2f\n", i, x[i]-ex[i], x[i]+ex[i], y[i], ey[i]);
}

cout << "Excess events after background subtraction = " << tot << endl;

TGraphErrors *gr1 = new TGraphErrors(nn, x, y, ex, ey);
TCanvas *c2;
c2 = new TCanvas();
c2->Draw();
gPad->SetLogx();
gPad->SetLogy();
TH2F *haux = new TH2F("haux", "", 100, 0.1, 3000., 100, 0.01, 100.);
haux->SetTitle("Background subtracted BEAM-ON Scientific ratemeter spectrum");
haux->GetXaxis()->SetTitle("Energy (MeV)");
haux->GetYaxis()->SetTitle("Counts / s channel");
haux->SetStats(0);
haux->Draw();
gr1->Draw("PSAME");

}
