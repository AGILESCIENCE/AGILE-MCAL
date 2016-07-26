{
// MM 19/4/09 draw polar plot of MCAL GRB detections, read by an ascii file
// based on mcal_GRB_polar.C and grid_GRB_polar.C

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
   
   struct mcal_grb {
   	char name1[20];	// the string "GRB"
	char name2[20];
	int orbit;
	int type;	// 3908, 3913
	double t0;
	char date[20];
	char time[20];
	float t90;
	float fluence;
	char trigger[20];
	float ra;
	float dec;
 	float theta;
	float phi;
  };
  mcal_grb agrb[1000];
  
  // build graphs 
   TGraphPolar * grSWM  = new TGraphPolar(1000); // Swift GRBs  
   TGraphPolar * grIPN  = new TGraphPolar(1000); // IPN GRBs  
   TGraphPolar * grGBM  = new TGraphPolar(1000); // GBM GRBs  
   TGraphPolar * grSA   = new TGraphPolar(1000); // AGILE (SA) GRBs  
   TGraphPolar * grINT  = new TGraphPolar(1000); // INTEGRAL GRBs  
   int nptSWM = 0;  
   int nptIPN = 0;  
   int nptGBM = 0;
   int nptSA  = 0;
   int nptINT  = 0;
   
  // read data file
   ifstream fmcal("/home/martino/data_analysis/root_macros/mcal_GRB_090630.txt", ios_base::in);
	int n=0;
	if (!fmcal) {
		cout << "Cannot open data file.	Exiting!" << endl;
	}
	while (fmcal >> agrb[n].name1 >> agrb[n].name2 >> agrb[n].orbit >> agrb[n].type >> agrb[n].t0 >> agrb[n].date >> agrb[n].time >> agrb[n].t90 >> agrb[n].fluence >> agrb[n].trigger >> agrb[n].ra >> agrb[n].dec >> agrb[n].theta >> agrb[n].phi) {
		
		// check trigger type and update graphs
		if (strncmp(agrb[n].trigger, "Swift", 5) == 0) grSWM->SetPoint(nptSWM++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "IPN", 3) == 0) grIPN->SetPoint(nptIPN++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "GBM", 3) == 0) grGBM->SetPoint(nptGBM++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "SA", 2) == 0) grSA->SetPoint(nptSA++, agrb[n].phi, agrb[n].theta);
		if (strncmp(agrb[n].trigger, "INTEGRAL", 8) == 0) grINT->SetPoint(nptINT++, agrb[n].phi, agrb[n].theta);
		
		cout << n << "  " << agrb[n].name2 << "  " << agrb[n].trigger << "  " << agrb[n].theta << "  " << agrb[n].phi << endl;
		n++;		
	}
	fmcal.close();
	cout << "read " << n << " GRBs detected by MCAL" << endl;
	cout << nptSA <<  " localized by Super-AGILE" << endl;
	cout << nptSWM << " localized by Swift" << endl;
	cout << nptIPN << " localized by IPN" << endl;
	cout << nptGBM << " localized by Fermi-GBM" << endl;
	cout << nptINT << " localized by INTEGRAL" << endl;

 
 // QUESTI DUE COMANDI VANNO LANCIATI DA SHELL. PERCHE'? BOH!
/*   grSWM->GetPolargram()->SetRangePolar(0, 360);
     grSWM->GetPolargram()->SetRangeRadial(0, 180);
*/    
   
   // graphic section
   grSWM->SetMarkerSize(2.5);
   grSWM->SetMarkerStyle(29);
   grSWM->SetMarkerColor(4);
   grSWM->SetTitle("");
    grSWM->Draw("AP");
    
   grIPN->SetMarkerSize(2.);
   grIPN->SetMarkerStyle(20);
   grIPN->SetMarkerColor(kCyan);
   grIPN->SetTitle("");
   grIPN->Draw("PSAME");
    
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

   // draw legend
   TCanvas c2("c2", "c2", 400, 300);
   c2.cd();
   leg = new TLegend(0.05,0.05,0.95,0.95);
	leg->AddEntry(grSAfov,"SA FOV","l");
	leg->AddEntry(grGRIDfov,"GRID FOV","l");
	leg->AddEntry(grSA, "Localized by Super-AGILE","p");
	leg->AddEntry(grIPN,"Localized by IPN","p");
	leg->AddEntry(grSWM,"Localized by SWIFT","p");
	leg->AddEntry(grGBM,"Localized by Fermi-GBM","p");
	leg->AddEntry(grINT,"Localized by INTEGRAL","p");
	leg->Draw();

}