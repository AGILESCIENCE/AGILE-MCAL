{
// localizations updated to 081005

TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
mcalstyle->SetCanvasBorderMode(0);
mcalstyle->SetPadBorderMode(0);
mcalstyle->SetPadColor(0);
mcalstyle->SetCanvasColor(0);
mcalstyle->SetTitleColor(1); // 0
mcalstyle->SetStatColor(0);
mcalstyle->SetPalette(1,0);
mcalstyle->SetFrameBorderMode(0);
mcalstyle->SetLabelSize(0.04, "X");
mcalstyle->SetLabelSize(0.04, "Y");
gROOT->SetStyle("mcalstyle");
   
   TCanvas * CPol = new TCanvas("CPol3","AGILE GRBs",800,800);
   

   Double_t rmin=0;
   Double_t rmax=TMath::Pi()*2;
   Double_t r[];
   
   // GRBs localized by SWIFT and seen by MCAL only
   int nptSWM=11;
   Double_t thetaSWM[20] = {166., 120., 77.9, 99.1, 110.16, 84.36, 155.43, 74.63, 113.6, 38.86, 48.};
   Double_t phiSWM[20]   = {353., 133., 274.1, 168.1, 355., 186.06, 143.67, 163.52, 201.0, 186.21, 338.6};
   // GRB071020
   // GRB070714
   // GRB071112C
   // GRB071117
   // GRB071227
   // GRB080319C
   // GRB080328
   // GRB080603B
   // GRB080607
   // GRB080613
   // GRB080721
   
   TGraphPolar * grSWM = new TGraphPolar(nptSWM,phiSWM,thetaSWM);
   grSWM->SetMarkerSize(2.5);
   grSWM->SetMarkerStyle(29);
   grSWM->SetMarkerColor(4);
   grSWM->SetTitle("");
   grSWM->Draw("AP");
 // QUESTI DUE COMANDI VANNO LANCIATI DA SHELL. PERCHE'? BOH!
/*   grSWM->GetPolargram()->SetRangePolar(0, 360);
   grSWM->GetPolargram()->SetRangeRadial(0, 180);
*/    
 
/* setting grafici per pubblicazione:
grSWM->SetRadialLabelSize(0.025);
grSWM->SetNdivRadial(008);	// set number of division / grid
grSWM->SetPolarLabelSize(0.025);
*/
  
   // GRBs localized by IPN and seen by MCAL and/or SA
   int nptIPM=8;
   Double_t thetaIPM[10] = {48.2, 135.8, 129., 67.7, 93.48, 72.82, 76.3, 37.6};
   Double_t phiIPM[10]   = {88.3, 225.0, 315., 75.13, 209.35, 139.95, 262.6, 1.12};
   // GRB070824 (SA)
   // GRB070825
   // GRB070915
   // GRB071125
   // GRB080122
   // GRB080204
   // GRB080407B (no GCN)
   // GRB080514B
   
   TGraphPolar * grIPM = new TGraphPolar(nptIPM,phiIPM,thetaIPM);
   grIPM->SetMarkerSize(2);
   grIPM->SetMarkerStyle(20);
   grIPM->SetMarkerColor(4);
   grIPM->SetTitle("");
   grIPM->Draw("PSAME");
    
   // GRBs localized by GBM and seen by MCAL 
   int nptGBM=4;
   Double_t thetaGBM[10] = {57., 75., 114.8, 90.4};
   Double_t phiGBM[10]   = {90.6, 11.5, 93.9, 163.3};   
   // GRB 080817
   // GRB 080825
   // GRB 080906
   // GRB 080916
   
   TGraphPolar *grGBM = new TGraphPolar(nptGBM,phiGBM,thetaGBM);
   grGBM->SetMarkerStyle(21);
   grGBM->SetMarkerColor(2);
   grGBM->SetMarkerSize(1.5);
   grGBM->SetTitle("");
   grGBM->Draw("PSAME");
     
   // GRBs localized by SA
/*   int nptSA=5;
   Double_t thetaSA[10] = {21.0, 19.9, 13.28, 12.9, 16.8};
   Double_t phiSA[10]   = {157.1, 152.3, 259.68, 357.0, 255.0}; 
   // GRB070724B
   // GRB071104
   // GRB080229B
   // GRB080408
   // GRB080507
*/     
   // GRBs localized by SA AND detected by MCAL
   int nptSA=3;
   Double_t thetaSA[5] = {19.9, 13.2, 20.58};
   Double_t phiSA[5]   = {152.3, 183.3, 7.71}; // baro: sono lo stesso punto ma altrimenti non fa il plot
   // GRB071104
   // GRB080723
   // GRB081001

  
   TGraphPolar * grSA = new TGraphPolar(nptSA,phiSA,thetaSA);
   grSA->SetMarkerSize(2.5);
   grSA->SetMarkerStyle(22);
   grSA->SetMarkerColor(3);
   grSA->SetTitle("");
   grSA->Draw("PSAME");
   
   // draw GRID FOV
   Double_t gr[1000];
   Double_t gtheta[1000];
   for (Int_t ipt = 0; ipt < 1000; ipt++) {
      gr[ipt] = ipt*360./1000;
      gtheta[ipt] = 60.;
   }
   TGraphPolar * grGRIDfov = new TGraphPolar(1000,gr,gtheta);
   grGRIDfov->SetLineColor(2);
   grGRIDfov->SetLineWidth(3);
   grGRIDfov->SetTitle("");
   grGRIDfov->Draw("LPSAME");
   
   // draw SA FOV
   Double_t sr[1000];
   Double_t stheta[1000];
   for (Int_t ipt = 0; ipt < 1000; ipt++) {
      sr[ipt] = ipt*360./1000;
      stheta[ipt] = 34.;
   }
   TGraphPolar * grSAfov = new TGraphPolar(1000,sr,stheta);
   grSAfov->SetLineColor(3);
   grSAfov->SetLineWidth(3);
   grSAfov->SetTitle("");
   grSAfov->Draw("LPSAME");

   TCanvas c2("c2", "c2", 400, 300);
   c2.cd();
   // draw legend
   leg = new TLegend(0.05,0.05,0.95,0.95);
leg->AddEntry(grGRIDfov,"GRID FOV","l");
leg->AddEntry(grSAfov,"SA FOV","l");
leg->AddEntry(grSWM,"Localized by SWIFT - detected by MCAL","p");
leg->AddEntry(grIPM,"Localized by IPN - detected by MCAL and/or SA","p");
leg->AddEntry(grSA,"Localized by SA","p");
leg->AddEntry(grGBM,"Localized by GBM","p");
leg->Draw();
   
   
//   return CPol;
}