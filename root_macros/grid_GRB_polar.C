{
// MM 17/4/09 plots GRBs in the GRID FOV
//            based on mcal_GRB_polar.C

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
   
   struct grb {
   	char name[20];
	char time[20];
	char trigger[20];
	double t0;
	float ra;
	float dec;
 	float theta;
	float phi;
   	char orb_phase[20];
	int orbit;
	float earth_dist;
	int occulted;
	int mcal;
  };
  grb agrb[1000];
  
  // build graphs 
   TGraphPolar * grSWM  = new TGraphPolar(1000); // Swift GRBs  
   TGraphPolar * grMCAL = new TGraphPolar(1000); // also seen by MCAL  
   TGraphPolar * grGBM  = new TGraphPolar(1000); // GBM GRBs  
   TGraphPolar * grSA   = new TGraphPolar(1000); // AGILE (SA) GRBs  
   TGraphPolar * grINT  = new TGraphPolar(1000); // INTEGRAL GRBs  
   int nptSWM = 0;  
   int nptMCAL = 0;  
   int nptGBM = 0;
   int nptSA  = 0;
   int nptINT  = 0;
   
  // read data file
   ifstream fmcal("/home/mcal/data_analysis/grid_GRB_090417.txt", ios_base::in);
	int n=0;
	fmcal.clear();
	while (!fmcal.eof()) {
		fmcal >> agrb[n].name >> agrb[n].time >> agrb[n].trigger >> agrb[n].t0 >> agrb[n].ra >> agrb[n].dec >> agrb[n].theta >> agrb[n].phi >> agrb[n].orb_phase >> agrb[n].orbit >> agrb[n].earth_dist >> agrb[n].occulted >> agrb[n].mcal;
		
		// check trigger type and update graphs
		if (strncmp(agrb[n].trigger, "Swift", 5) == 0 || strncmp(agrb[n].trigger, "2", 1) == 0 || strncmp(agrb[n].trigger, "3", 1) == 0) grSWM->SetPoint(nptSWM++, agrb[n].phi, agrb[n].theta);
		if (agrb[n].mcal) grMCAL->SetPoint(nptMCAL++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "GBM", 3) == 0) grGBM->SetPoint(nptGBM++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "AGILE", 5) == 0) grSA->SetPoint(nptSA++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "INTEGRAL", 8) == 0) grINT->SetPoint(nptINT++, agrb[n].phi, agrb[n].theta);
		
		cout << n << "  " << agrb[n].name << "  " << agrb[n].trigger << "  " << agrb[n].theta << "  " << agrb[n].phi << "  " << agrb[n].mcal << endl;
		n++;
		
	}	
	cout << "read " << n << " GRBs in the GRID FOV" << endl;
 
 // QUESTI DUE COMANDI VANNO LANCIATI DA SHELL. PERCHE'? BOH!
/*   grSWM->GetPolargram()->SetRangePolar(0, 360);
     grSWM->GetPolargram()->SetRangeRadial(0, 70);
*/    
   
   // graphic section
   grSWM->SetMarkerSize(2.5);
   grSWM->SetMarkerStyle(29);
   grSWM->SetMarkerColor(4);
   grSWM->SetTitle("");
    grSWM->Draw("AP");
    
   grMCAL->SetMarkerSize(2.5);
   grMCAL->SetMarkerStyle(4);
   grMCAL->SetMarkerColor(2);
   grMCAL->SetTitle("");
   grMCAL->Draw("PSAME");
    
   grGBM->SetMarkerStyle(21);
   grGBM->SetMarkerColor(2);
   grGBM->SetMarkerSize(1.5);
   grGBM->SetTitle("");
   grGBM->Draw("PSAME");
    
   grSA->SetMarkerSize(2.);
   grSA->SetMarkerStyle(22);
   grSA->SetMarkerColor(3);
   grSA->SetTitle("");
   grSA->Draw("PSAME");
    
   grINT->SetMarkerSize(2.);
   grINT->SetMarkerStyle(23);
   grINT->SetMarkerColor(6);
   grINT->SetTitle("");
   grINT->Draw("PSAME");
   
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

   // draw legend
   TCanvas c2("c2", "c2", 400, 300);
   c2.cd();
   leg = new TLegend(0.05,0.05,0.95,0.95);
	leg->AddEntry(grSAfov,"SA FOV","l");
	leg->AddEntry(grSA,"Localized by AGILE","p");
	leg->AddEntry(grMCAL,"Detected by MCAL","p");
	leg->AddEntry(grSWM,"Localized by SWIFT","p");
	leg->AddEntry(grGBM,"Localized by Fermi-GBM","p");
	leg->AddEntry(grINT,"Localized by INTEGRAL","p");
	leg->Draw();

}