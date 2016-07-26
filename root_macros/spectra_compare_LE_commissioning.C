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

// good for desktop
TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_01.root");// 
TFile fe("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_02.root");// 
TFile fe2("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_03.root");// 
TFile fe3("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_04.root");//  
TFile fe4("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_05.root");// 
TFile fe5("/data1/mcalsw_output_COMMISSIONING/H/H000496_3909_sel_06.root");//  

TObjArray *ai = (TObjArray *) fi->Get("rawHisto_sel_0");
TObjArray *ae = (TObjArray *) fe->Get("rawHisto_sel_1");
TObjArray *ae2 = (TObjArray *) fe2->Get("rawHisto_sel_2");
TObjArray *ae3 = (TObjArray *) fe3->Get("rawHisto_sel_3");
TObjArray *ae4 = (TObjArray *) fe4->Get("rawHisto_sel_4");
TObjArray *ae5 = (TObjArray *) fe5->Get("rawHisto_sel_5");

TH1F *hi;
hi = (TH1F *) ai->FindObject("totE");

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetStats(0);
hi->SetTitle("MCAL reconstructed energy spectra");
//hi->SetTitle(0);
//hi->Rebin(50);
hi->Scale(1./0.02);
hi->GetYaxis()->SetTitle("Counts / (s MeV)");
hi->Draw("");

TH1F *he;
he = (TH1F *) ae->FindObject("totE");
he->SetStats(0);
//he->Rebin(50);
he->Scale(1./0.02);
he->SetLineColor(2);
he->SetMarkerColor(2);
he->Draw("same");

TH1F *he2;
he2 = (TH1F *) ae2->FindObject("totE");
he2->SetStats(0);
//he2->Rebin(50);
he2->Scale(1./0.02);
he2->SetLineColor(3);
he2->SetMarkerColor(3);
he2->Draw("same");

TH1F *he3;
he3 = (TH1F *) ae3->FindObject("totE");
he3->SetStats(0);
//he3->Rebin(50);
he3->Scale(1./0.02);
he3->SetLineColor(4);
he3->SetMarkerColor(4);
he3->Draw("same");

TH1F *he4;
he4 = (TH1F *) ae4->FindObject("totE");
he4->SetStats(0);
//he4->Rebin(50);
he4->Scale(1./0.02);
he4->SetLineColor(5);
he4->SetMarkerColor(5);
he4->Draw("same");

TH1F *he5;
he5 = (TH1F *) ae5->FindObject("totE");
he5->SetStats(0);
//he5->Rebin(50);
he5->Scale(1./0.02);
he5->SetLineColor(6);
he5->SetMarkerColor(6);
he5->Draw("same");



gPad->SetTicks(1,1);


TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi,  "t =     [0, 400 s]  DAC_ref = 240", "lp");
leg->AddEntry(he,  "t =   [440, 840 s]  DAC_ref = 243", "lp");
leg->AddEntry(he2, "t =  [880, 1280 s]  DAC_ref = 246", "lp");
leg->AddEntry(he3, "t = [1320, 1720 s]  DAC_ref = 249", "lp");
leg->AddEntry(he4, "t = [1760, 2160 s]  DAC_ref = 251", "lp");
leg->AddEntry(he5, "t = [2200, 2600 s]  DAC_ref = 240", "lp");
leg->Draw();

}
