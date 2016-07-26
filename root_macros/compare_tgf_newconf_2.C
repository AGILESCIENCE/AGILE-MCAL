int compare_tgf_newconf_2(int inicont=0, char *incat="/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-16Mar15.root", char * innew="/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf.root") 
{
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  c1->Divide(2,2);
  c1->cd(1);

  TFile *f1 = new TFile(incat);
  TTree *t1 = (TTree *) f1->Get("triggers");
  TH1F *hlon1 = new TH1F("hlon1", "Longitude distribution", 36, -180., 180.);
  t1->Draw("lon>>hlon1");

  //TFile f2("/Users/martinomarisaldi/Data/TGF/LE_TGF_newconf.root");
  TFile *f2 = new TFile(innew);
  TTree *t2 = (TTree *) f2->Get("triggers");
  int contact, id, year, month, day, hour, min, sec, usec;
  float lon, lat, h;
  double time0;
  t2->SetBranchAddress("contact", &contact);
  t2->SetBranchAddress("id", &id);
  t2->SetBranchAddress("t0", &time0);
  t2->SetBranchAddress("year", &year);
  t2->SetBranchAddress("month", &month);
  t2->SetBranchAddress("day", &day);
  t2->SetBranchAddress("hour", &hour);
  t2->SetBranchAddress("min", &min);
  t2->SetBranchAddress("sec", &sec);
  t2->SetBranchAddress("usec", &usec);
  t2->SetBranchAddress("lon", &lon);
  t2->SetBranchAddress("lat", &lat);
  t2->SetBranchAddress("h", &h);
  t2->SetScanField(0);
  TString sel("contact > ");
  sel += inicont;
  if (inicont > 0) t2->Scan("year:month:day:hour:min:sec:usec:lon:lat:h:contact:t0", sel.Data(),"col=::::::06d:10.3f:10.3f:10.3f::20.6f");
  int nold = t1->GetEntries();
  int nnew = t2->GetEntries();
  cout << "Old configuration: " << nold << " events" << endl;
  cout << "New configuration: " << nnew << " events" << endl;
  TH1F *hlon2 = new TH1F("hlon2", "hlon2", 36, -180., 180.);
  TH1F *hlon3 = new TH1F("hlon3", "hlon3", 36, -180., 180.);


  //---------- plot longitude distribution -----------

  t2->Draw("lon>>hlon2", "", "same");
  if (inicont > 0) t2->Draw("lon>>hlon3",  sel.Data(), "same");  
  
  hlon1->SetStats(0);
  hlon2->SetFillColor(kRed);
  hlon3->SetFillColor(kBlack);
  hlon1->SetLineWidth(2);
  hlon1->GetXaxis()->SetTitle("Longitude");
  hlon1->GetXaxis()->SetTitleSize(0.05);
  hlon1->GetXaxis()->CenterTitle();
  hlon1->GetXaxis()->SetLabelSize(0.05);
  hlon1->GetYaxis()->SetTitle("Number of TGFs");
  hlon1->GetYaxis()->SetTitleSize(0.05);
  hlon1->GetYaxis()->SetTitleOffset(0.85);
  hlon1->GetYaxis()->CenterTitle();
  hlon1->GetYaxis()->SetLabelSize(0.05);
  gPad->SetTicks(1,1);

  float text_size = 0.07;
  TLatex label1;
  label1.SetTextSize(text_size);
  label1.DrawTextNDC(0.15, 0.2, "(a)");

  
  //---------- plot local time distribution -----------
 
  c1->cd(2);
  TH1F *hlt1 = new TH1F("hlt1", "Local time distribution", 12, 0., 24.);
  t1->Draw("loctime>>hlt1");
  TH1F *hlt2 = new TH1F("hlt2", "hlt2", 12, 0., 24.);
  t2->Draw("loctime>>hlt2", "", "same");
  TH1F *hlt3 = new TH1F("hlt3", "hlt3", 12, 0., 24.);
  if (inicont > 0) t2->Draw("loctime>>hlt3",  sel.Data(), "same");  
  
  hlt1->SetStats(0);
  hlt2->SetFillColor(kRed);
  hlt3->SetFillColor(kBlack);
  hlt1->SetLineWidth(2);
  hlt1->GetXaxis()->SetTitle("Local time");
  hlt1->GetXaxis()->SetTitleSize(0.05);
  hlt1->GetXaxis()->CenterTitle();
  hlt1->GetXaxis()->SetLabelSize(0.05);
  hlt1->GetYaxis()->SetTitle("Number of TGFs");
  hlt1->GetYaxis()->SetTitleSize(0.05);
  hlt1->GetYaxis()->SetTitleOffset(0.85);
  hlt1->GetYaxis()->CenterTitle();
  hlt1->GetYaxis()->SetLabelSize(0.05);
  gPad->SetTicks(1,1);

  TLatex label2;
  label2.SetTextSize(text_size);
  label2.DrawTextNDC(0.15, 0.2, "(b)");

  
  //---------- plot duration distribution -----------

  c1->cd(3);
  TH1F *hd1 = new TH1F("hd1", "Duration distribution", 100, 0., 0.005);
  t1->Draw("dt>>hd1");
  TH1F *hd2 = new TH1F("hd2", "hd2", 100, 0., 0.005);
  t2->Draw("dt>>hd2", "", "same");
  TH1F *hd3 = new TH1F("hd3", "hd3", 100, 0., 0.005);
  if (inicont > 0) t2->Draw("dt>>hd3",  sel.Data(), "same");  
  
  hd1->SetStats(0);
  hd2->SetFillColor(kRed);
  hd3->SetFillColor(kBlack);
  hd1->SetLineWidth(2);
  hd1->GetXaxis()->SetRangeUser(0., 0.0025);
  hd1->GetXaxis()->SetTitle("Total duration (s)");
  hd1->GetXaxis()->SetTitleSize(0.05);
  hd1->GetXaxis()->CenterTitle();
  hd1->GetXaxis()->SetLabelSize(0.05);
  hd1->GetYaxis()->SetTitle("Number of TGFs");
  hd1->GetYaxis()->SetTitleSize(0.05);
  hd1->GetYaxis()->SetTitleOffset(0.85);
  hd1->GetYaxis()->CenterTitle();
  hd1->GetYaxis()->SetLabelSize(0.05);
  gPad->SetTicks(1,1);

  TLatex label3;
  label3.SetTextSize(text_size);
  label3.DrawTextNDC(0.15, 0.2, "(c)");


  //---------- plot intensity distribution -----------

  c1->cd(4);
  TH1F *hc1 = new TH1F("hc1", "Intensity distribution", 50, 0., 100.);
  t1->Draw("counts>>hc1");
  TH1F *hc2 = new TH1F("hc2", "Intensity distribution", 50, 0., 100.);
  t2->Draw("counts>>hc2", "", "same");
  TH1F *hc3 = new TH1F("hc3", "Intensity distribution", 50, 0., 100.);
  if (inicont > 0) t2->Draw("counts>>hc3",  sel.Data(), "same");  
  //  hc1->Sumw2();
  //  hc2->Sumw2();

  gPad->SetLogx();
  gPad->SetLogy();
  hc1->SetStats(0);
  hc2->SetFillColor(kRed);
  hc3->SetFillColor(kBlack);
  hc1->SetLineWidth(2);
  hc1->GetXaxis()->SetRangeUser(8., 100.);
  hc1->GetXaxis()->SetTitle("Counts");
  hc1->GetXaxis()->SetTitleSize(0.05);
  hc1->GetXaxis()->CenterTitle();
  hc1->GetXaxis()->SetLabelSize(0.05);
  hc1->GetYaxis()->SetTitle("Number of TGFs");
  hc1->GetYaxis()->SetTitleSize(0.05);
  hc1->GetYaxis()->SetTitleOffset(0.85);
  hc1->GetYaxis()->CenterTitle();
  hc1->GetYaxis()->SetLabelSize(0.05);
  gPad->SetTicks(1,1);

  TF1 *fit1 = new TF1("fit1", "[0]*x^[1]", 18., 60.);
  fit1->SetParameters(20000., -3.);
  hc1->Fit("fit1", "R0+");
  fit1->SetLineColor(kBlack);
  fit1->Draw("same");

  TF1 *fit2 = new TF1("fit2", "[0]*x^[1]", 12., 60.);
  fit2->SetParameters(11600., -2.4);
  hc2->Fit("fit2", "R0+");
  fit2->SetLineColor(kBlack);
  fit2->Draw("same");

  TLatex label4;
  label4.SetTextSize(text_size);
  label4.DrawTextNDC(0.15, 0.2, "(d)");


  //---------- plot detection rate -----------

  TCanvas *c4 = new TCanvas();
  t2->GetEntry(nnew-1);
  int ndays = 1.1*(time0-347155200.)/86400.;
  TH1F *ht1 = new TH1F("ht1", "TGF daily detection rate", ndays, 0., ndays);  // 2500, 0., 2500. for start date 1 Mar 2009
  TH1F *ht2 = new TH1F("ht2", "TGF daily detection rate", ndays, 0., ndays);
  TH1F *ht3 = new TH1F("ht3", "TGF daily detection rate", ndays, 0., ndays);
  t2->Draw("(t0-347155200.)/86400. >> ht2"); // start date 1 Jan 2015
  //t1->Draw("(t0-352252800.)/86400. >> ht1"); // start date 1 Mar 2015
  //t1->Draw("(t0-162950400.)/86400. >> ht1", "", "same"); // start date 1 Mar 2009
  t1->Draw("(t0-347155200.)/86400. >> ht1", "", "same"); // start date 1 Jan 2015
  if (inicont > 0) t2->Draw("(t0-347155200.)/86400. >>ht3",  sel.Data(), "same");  

  ht2->SetStats(0);
  ht2->SetFillColor(kRed);
  ht3->SetFillColor(kBlack);
  ht1->SetLineWidth(2);
  ht2->GetXaxis()->SetTitle("Days since 1 Jan 2015");
  ht2->GetXaxis()->SetTitleSize(0.05);
  ht2->GetXaxis()->CenterTitle();
  ht2->GetXaxis()->SetLabelSize(0.05);
  ht2->GetYaxis()->SetTitle("TGFs / day");
  ht2->GetYaxis()->SetTitleSize(0.05);
  ht2->GetYaxis()->SetTitleOffset(0.85);
  ht2->GetYaxis()->CenterTitle();
  ht2->GetYaxis()->SetLabelSize(0.05);
  c4->SetTicks(1,1);


  // ------ save all pictures ------
  
  //c1->Print("tgf_distributions.png");
  c1->Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/tgf_distributions.pdf");
  //c1->Print("tgf_distributions.eps");
  //c4->Print("rate.png");  
  c4->Print("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/rate.pdf");  
  

 // ------ print TGF list ------

  FILE *fo;
  fo = fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf.dat", "w");
  int j=0;
  //while (t2->GetEntry(j++)) fprintf(fo,"%d \t%02d \t%02d \t%02d \t%02d \t%02d.%06d \t%8.3f \t%8.3f \t%.3f \t%06d \t%d \t%.6f\n", year, month, day, hour, min, sec, usec, lon, lat, h*.001, contact, id, time0);
  while (t2->GetEntry(j++)) fprintf(fo,"%d \t%02d \t%02d \t%02d \t%02d \t%02d.%06d \t%8.3f \t%8.3f \t%.3f \t%06d \t%.6f\n", year, month, day, hour, min, sec, usec, lon, lat, h*.001, contact, time0);
  fclose(fo);

  return 0;
}
