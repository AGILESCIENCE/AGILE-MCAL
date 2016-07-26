// this ROOT macro plot a figure with bar characterization flow with IASF-Bo measurements
// to be used for the SPIE-2006 proceedings

// works on run #336 obtained @ IASF-Bo, July 2005, with MCAL DFE TE
// analized with program mcalview2

// M. Marisaldi, IASF-Bo, 11/4/06

{
    gROOT->Reset();
TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
mcalstyle->SetCanvasBorderMode(0);
mcalstyle->SetPadBorderMode(0);
mcalstyle->SetPadColor(0);
mcalstyle->SetCanvasColor(0);
mcalstyle->SetTitleColor(1); // 0
mcalstyle->SetStatColor(0);
//mcalstyle->SetPalette(1,0);  // 2D graph colour version
mcalstyle->SetPalette(9,0);  // 2D graph B/W version
mcalstyle->SetFrameBorderMode(0);
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
mcalstyle->SetLabelSize(0.05, "Z");
mcalstyle->SetTitleSize(0.05, "Z");
 mcalstyle->SetPadLeftMargin(0.1);
 mcalstyle->SetPadRightMargin(0.1);
 mcalstyle->SetPadTopMargin(0.1);
 mcalstyle->SetPadBottomMargin(0.1);
gROOT->SetStyle("mcalstyle");

TCanvas *c = new TCanvas("c", "MCAL bar calibration flow", 1000, 500);
 c->Divide(4, 2, 0.01, 0.01);

 TFile *fp;
 TPad *p;
 TH1F *h;
 TH2F *h2;
 TF1 *ftotal;
 TF1 *fpeak;
 TF1 *fbkg;
TText *t = new TText();
   t->SetTextSize(0.05);
   t->SetTextColor(1);

// NO SELECTION

// 2D

c->cd(1);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_2D.root");
p = (TPad *) fp->Get("ec3_9");
h2 = (TH2F *) p->GetPrimitive("BAR8_2D");
h2->SetTitle("");
 h2->SetAxisRange(220, 270, "X");
 h2->SetAxisRange(230, 280, "Y");

 gPad->SetLeftMargin(0.18);
 gPad->SetRightMargin(0.21);
h2->Draw("COLZ");
 TPaletteAxis *palette = (TPaletteAxis *) h2->FindObject("palette");
 palette->SetLabelSize(0.05);
 palette->SetTitleSize(0.05);
 palette->SetTitleOffset(1.2);
   t->DrawTextNDC(0.01, 0.01, "(a)");
   gPad->Update();
fp->Close();

// pd-A

c->cd(2);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_A.root");
p = (TPad *) fp->Get("ec1_9");
h = (TH1F *) p->GetPrimitive("BAR8_PDA");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("ADC channels");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetRangeUser(220, 270);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();
   t->DrawTextNDC(0.01, 0.01, "(b)");
fp->Close();

// pd-B
c->cd(3);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_B.root");
p = (TPad *) fp->Get("ec2_9");
h = (TH1F *) p->GetPrimitive("BAR8_PDB");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("ADC channels");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetRangeUser(230, 280);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();
   t->DrawTextNDC(0.01, 0.01, "(c)");
fp->Close();

// E
c->cd(4);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_E.root");
p = (TPad *) fp->Get("ec5_9");
h = (TH1F *) p->GetPrimitive("BAR8_E");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("Energy (MeV)");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetRangeUser(1, 2);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();
   t->DrawTextNDC(0.01, 0.01, "(d)");
fp->Close();

// SELECTION: single events, 1 MeV < E < 2 MeV, -10.6 cm < x < -7.0 cm

// 2D
c->cd(5);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_2D_sel-E-pos.root");
p = (TPad *) fp->Get("ec3_9");
TH2F *h2s = (TH2F *) p->GetPrimitive("sBAR8_2D");
h2s->SetTitle("");
 h2s->SetAxisRange(220, 270, "X");
 h2s->SetAxisRange(230, 280, "Y");
 h2s->GetZaxis()->SetTitleSize(0.05);
 h2s->GetZaxis()->SetTitleOffset(1.2);
 h2s->GetZaxis()->SetLabelSize(0.05);
 gPad->SetLeftMargin(0.18);
 gPad->SetRightMargin(0.21);
h2s->Draw("COLZ");
 TPaletteAxis *spalette = (TPaletteAxis *) h2s->FindObject("palette");
 spalette->SetLabelSize(0.05);
 spalette->SetTitleSize(0.05);
 spalette->SetTitleOffset(1.2);
   t->DrawTextNDC(0.01, 0.01, "(e)");
fp->Close();

// pd-A
c->cd(6);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_A_sel-E-pos.root");
p = (TPad *) fp->Get("ec1_9");
h = (TH1F *) p->GetPrimitive("sBAR8_PDA");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("ADC channels");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetTitleColor(1);
 h->GetXaxis()->SetTitleOffset(1);
 h->GetXaxis()->SetRangeUser(220, 270);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();

 ftotal = (TF1 *) p->GetPrimitive("total");
 fpeak  = (TF1 *) p->GetPrimitive("peak");
 fbkg   = (TF1 *) p->GetPrimitive("bkg");
 fpeak->SetParameters(395.201, 257.816, 1.91037);
 fpeak->SetRange(220, 270);
	fpeak->SetLineWidth(1);
		fpeak->SetLineColor(1);
		fpeak->SetLineStyle(2);
		fpeak->Draw("same");

		ftotal->SetLineWidth(1);
		ftotal->SetLineColor(1);
		//ftotal->Draw("same");

		fbkg->SetParameters(34.1145, -0.112876);
		fbkg->SetLineWidth(1);
		fbkg->SetLineColor(1);
		fbkg->SetLineStyle(3);
		fbkg->Draw("same");
   t->DrawTextNDC(0.01, 0.01, "(f)");
fp->Close();

// pd-B
c->cd(7);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_B_sel-E-pos.root");
p = (TPad *) fp->Get("ec2_9");
h = (TH1F *) p->GetPrimitive("sBAR8_PDB");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("ADC channels");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetTitleColor(1);
 h->GetXaxis()->SetTitleOffset(1);
 h->GetXaxis()->SetRangeUser(230, 280);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();

 ftotal = (TF1 *) p->GetPrimitive("total");
 fpeak  = (TF1 *) p->GetPrimitive("peak");
 fbkg   = (TF1 *) p->GetPrimitive("bkg");
fpeak->SetParameters(515.322, 262.696, 1.82736);
fpeak->SetRange(220, 270);
		fpeak->SetLineWidth(1);
		fpeak->SetLineColor(1);
		fpeak->SetLineStyle(2);
		fpeak->Draw("same");

		ftotal->SetLineWidth(1);
		ftotal->SetLineColor(2);
		//ftotal->Draw("same");

		fbkg->SetParameters(50.1791, -0.171196);
		fbkg->SetLineWidth(1);
		fbkg->SetLineColor(1);
		fbkg->SetLineStyle(3);
		fbkg->Draw("same");
   t->DrawTextNDC(0.01, 0.01, "(g)");
fp->Close();

// E
c->cd(8);
gPad->SetTicks(1,1);   
fp = new TFile("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_E_sel-E-pos.root");
p = (TPad *) fp->Get("ec5_9");
h = (TH1F *) p->GetPrimitive("sBAR8_E");
h->SetTitle("");
 h->SetStats(0);
 h->GetXaxis()->SetTitle("Energy (MeV)");
 h->GetXaxis()->CenterTitle();
 h->GetXaxis()->SetTitleSize(0.05);
 h->GetXaxis()->SetRangeUser(1, 2);
 h->GetYaxis()->SetTitle("Counts");
 h->GetYaxis()->SetTitleSize(0.05);
 h->GetYaxis()->SetTitleOffset(1.8);
 h->GetYaxis()->CenterTitle();
 gPad->SetLeftMargin(0.18);
h->Draw();

 ftotal = (TF1 *) p->GetPrimitive("total");
 fpeak  = (TF1 *) p->GetPrimitive("peak");
 fbkg   = (TF1 *) p->GetPrimitive("bkg");
fpeak->SetParameters(181.61, 1.29312, 0.0847564);
fpeak->SetRange(1., 2.);
fbkg->SetRange(1., 2.);
		fpeak->SetLineWidth(1);
		fpeak->SetLineColor(1);
		fpeak->SetLineStyle(2);
		fpeak->Draw("same");

		ftotal->SetLineWidth(1);
		ftotal->SetLineColor(2);
		//ftotal->Draw("same");

		fbkg->SetParameters(8.39489, -3.21267);
		fbkg->SetLineWidth(1);
		fbkg->SetLineColor(1);
		fbkg->SetLineStyle(3);
		fbkg->Draw("same");

   t->DrawTextNDC(0.01, 0.01, "(h)");
fp->Close();

}
