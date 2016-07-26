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

/*	// good for laptop
TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H011817_3909_sel_01.root");// 19-20/12/06 satellite RFT @CGS
TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_01.root");// 4-5/3/07 satellite FFT @IABG
//TFile fe2("~/local/mcalsw/bin/mcalsw_output/H/H013712_3909_sel_01.root");// 4/4/07 satellite RFT @SHAR
TFile fe2("~/local/mcalsw/bin/mcalsw_output/H/H013710_3909.root");// 4/4/07 satellite RFT @SHAR
TFile fe3("~/local/mcalsw/bin/mcalsw_output/H/H014005_3909.root");// 14/4/07 satellite RFT @SHAR on umbilical tower 
*/

// good for desktop
TFile fi("/data1/mcalsw_output_AIV/H/H011817_3909_sel_01.root");// 19-20/12/06 satellite RFT @CGS
TFile fe("/data1/mcalsw_output_AIV/H/H013594_3909_sel_01.root");// 4-5/3/07 satellite FFT @IABG
TFile fe2("/data1/mcalsw_output_AIV/H/H013710_3909.root");// 4/4/07 satellite RFT @SHAR
TFile fe3("/data1/mcalsw_output_AIV/H/H014005_3909.root");// 14/4/07 satellite RFT @SHAR on umbilical tower 

TObjArray *ai = (TObjArray *) fi->Get("rawHisto");
TObjArray *ae = (TObjArray *) fe->Get("rawHisto");
TObjArray *ae2 = (TObjArray *) fe2->Get("rawHisto");
TObjArray *ae3 = (TObjArray *) fe3->Get("rawHisto");

TH1F *hi;
hi = (TH1F *) ai->FindObject("totE");

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetStats(0);
//hi->SetTitle("MCAL reconstructed energy spectra");
hi->SetTitle(0);
hi->Rebin(3);
hi->Scale(1./0.06);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

TH1F *he;
he = (TH1F *) ae->FindObject("totE");
he->SetStats(0);
he->Rebin(3);
he->Scale(1./0.06);
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("same");

TH1F *he2;
he2 = (TH1F *) ae2->FindObject("totE");
he2->SetStats(0);
he2->Rebin(3);
he2->Scale(1./0.06);
he2->SetLineColor(3);
he2->SetMarkerColor(3);
he2->Draw("same");

TH1F *he3;
he3 = (TH1F *) ae3->FindObject("totE");
he3->SetStats(0);
he3->Rebin(3);
he3->Scale(1./0.06);
he3->SetLineColor(4);
he3->SetMarkerColor(4);
he3->Draw("same");
gPad->SetTicks(1,1);

/*
TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "run 11817 19/12/06 CGS", "lp");
leg->AddEntry(he, "run 13594 05/03/07 IABG", "lp");
leg->AddEntry(he2, "run 13710 04/04/07 SHAR clean room", "lp");
leg->AddEntry(he3, "run 14005 14/04/07 SHAR integrated on launcher", "lp");
leg->Draw();
*/

}
