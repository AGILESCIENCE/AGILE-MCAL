{
gROOT->Reset();
gStyle->SetPalette(1,0);
char datadir[]="~/data_analysis/AGILE_MCAL_AIV_Data/rootfiles/";
char f_iasf[]="2556_plane-X_E-GE-2MeV.root";
char filepath[100];

sprintf(filepath, "%s%s", datadir, f_iasf);
TFile fi(filepath) ;
TPad *pi;
fi.GetObject("ec0_5;1", pi);
TH1F *hi;
hi = (TH1F *) (pi->GetPrimitive("hX"));

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
hi->SetStats(0);
hi->Draw("COLZ");


}
