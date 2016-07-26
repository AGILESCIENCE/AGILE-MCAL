{
gROOT->Reset();
char datadir[]="/data1/AGILE_MCAL_AIV_Data/rootfiles/";
//char f_iasf[]="628_sci-RM-X.root";
char f_laben1[]="1106_sci-RM-X.root";
char f_laben2[]="3093_sci-RM-X.root";
char f_iabg[]="3939_sciRMX.root";

char filepath[100];

sprintf(filepath, "%s%s", datadir, f_laben2);
TFile fi(filepath) ;
TPad *pi;
fi.GetObject("ec0_8;1", pi);
TGraphErrors *hi;
hi = (TGraphErrors *) (pi->GetPrimitive("Graph"));

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
//hi->SetStats(0);
hi->SetTitle("Scientific Ratemeters: X-plane spectra");
hi->GetYaxis()->SetTitle("Counts / s");
hi->Draw("AP");


sprintf(filepath, "%s%s", datadir, f_laben1);
TFile fe(filepath) ;
TPad *pe;
fe.GetObject("ec0_8;1", pe);
TGraphErrors *he;
pe->ls();
he = (TGraphErrors *) (pe->GetPrimitive("Graph"));
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("P");

/*
sprintf(filepath, "%s%s", datadir, f_dafne_bkg);
TFile f3(filepath) ;
TPad *p3;
f3.GetObject("ec0_15;1", p3);
TH1F *h3;
h3 = (TH1F *) (p3->GetPrimitive("seltotE"));
h3->SetStats(0);
//h3->Rebin(5);
h3->Scale(4322./800.);
h3->Scale(1./0.02);
h3->SetLineColor(3);
h3->SetMarkerColor(3);
h3->Draw("same");
*/

sprintf(filepath, "%s%s", datadir, f_iabg);
TFile f4(filepath) ;
TPad *p4;
f4.GetObject("ec0_8;1", p4);
p4->ls();
TGraphErrors *h4;
h4 = (TGraphErrors *) (p4->GetPrimitive("gRMX3913"));
h4->SetLineColor(4);
h4->SetMarkerColor(4);
h4->Draw("P");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
//leg->AddEntry(hi, "IASF-Bo 16/07/05, run  628, MCAL vertical", "l");
leg->AddEntry(he, "LABEN   24/09/05, run 1106, MCAL horizontal", "l");
//leg->AddEntry(h3, "BTF     07/11/05, run 2143, MCAL vertical", "l");
leg->AddEntry(hi, "LABEN   29/11/05, run 3093, MCAL horizontal", "l");
leg->AddEntry(h4, "IABG    28/01/06, run 3785, MCAL horizontal", "l");
leg->Draw();


}
