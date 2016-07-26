init()
{
TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
mcalstyle->SetCanvasBorderMode(0);
mcalstyle->SetPadBorderMode(0);
mcalstyle->SetPadColor(0);
mcalstyle->SetCanvasColor(0);
mcalstyle->SetTitleColor(1); // 0
mcalstyle->SetStatColor(0);
mcalstyle->SetPalette(1,0);  // 2D graph colour version
//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
mcalstyle->SetFrameBorderMode(0);
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
gROOT->SetStyle("mcalstyle");
}

plot_2D_lego()
{
    gStyle->SetPalette(1,0);
//    TFile fp("336_X9_2D.root");
//     TFile fp("3785_bar_corr_lowDT_OK.root");
     TFile fp("~/documents/AGILE/activities/DAFNE_calibrations/2556_X_E-GT-2MeV.root");
   fp.ls();
//    TPad *p = fp.Get("ec3_9");
    TPad *p = fp.Get("ec0_5");
    p->ls();
    TCanvas *c = new TCanvas();
//    TH2F *h = (TH2F *) p->GetPrimitive("BAR8_2D");
//    TH2F *h = (TH2F *) p->GetPrimitive("barcorrLowDT");
    TH2F *h = (TH2F *) p->GetPrimitive("hX");
    h->Draw("LEGO2Z");
}

plot_2D()
{
//TFile fi("~/mcalsw_output/H/H013531_3909_sel_02.root");
TFile fi("~/mcalsw_output/H/H013531_3909.root");
fi.ls();
//TObjArray *ai = (TObjArray *) fi.Get("rawHisto_sel_1");
TObjArray *ai = (TObjArray *) fi.Get("rawHisto");
TH2F *hauxiX = (TH2F *)  ai->FindObject("hX");
TH2F *hauxiZ = (TH2F *)  ai->FindObject("hZ");

TCanvas *c4;
c4 = new TCanvas("c4", "c4", 800, 400);
c4->Divide(2,1);
c4->cd(1);
hauxiX->GetYaxis()->SetRangeUser(-20., 20.);
hauxiX->Draw("COLZ");
c4->cd(2);
hauxiZ->GetXaxis()->SetRangeUser(-20., 20.);
hauxiZ->Draw("COLZ");

}

plot_pos()
{
    // old version
/*    TFile f2("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/336_X9_pos_sel-E.root");
    f2.ls();
    TPad *p2 = f2.Get("ec4_9");
    p2->ls();
    TCanvas *c2 = new TCanvas();
    gPad->SetTicks(1,1);    
    TH1F *h2 = (TH1F *) p2->GetPrimitive("sBAR8_POS");
    h2->SetStats(0);
    h2->Draw();

// label if required
   TText *t = new TText();
   t->SetTextSize(0.05);
   t->SetTextColor(1);
   t->DrawTextNDC(0.01, 0.01, "(a)");*/

    // new version  
    TFile f2("/data1/mcalsw_output_AIV/H/H000336_BURST_sel_01.root");
    f2.ls();
    TObjArray *a2 = (TObjArray *) f2.Get("rawHisto_sel");
    TCanvas *c2 = new TCanvas();
    gPad->SetTicks(1,1);   
    gPad->SetFrameBorderMode(0);
    TH1F *h2 = (TH1F *) a2->FindObject("totE");
    h2->GetYaxis()->SetTitle("Counts / (s MeV)");
    h5->SetStats(0);
//    h5->SetTitle("MCAL reconstructed energy spectrum");
    h5->Scale(1./0.02);
    h5->Draw();
  
}


plot_E()
{
    TFile f3("~/documents/AGILE/activities/calibrazioni_IASF_BO/BURST_calibration/683_X9_E.root");
    f3.ls();
    TPad *p3 = f3.Get("ec5_9");
    p3->ls();
    TCanvas *c3 = new TCanvas();
    gPad->SetTicks(1,1);   
    TH1F *h3 = (TH1F *) p3->GetPrimitive("BAR8_E");
    h3->SetStats(0);
    h3->Draw();

// label if required
   TText *t = new TText();
   t->SetTextSize(0.05);
   t->SetTextColor(1);
   t->DrawTextNDC(0.01, 0.01, "(b)");
}


plot_pd()
{
    TFile f4("4054_X9_A.root");
    f4.ls();
    TPad *p4 = f4.Get("ec1_9");
    p4->ls();
    TCanvas *c4 = new TCanvas();
    TH1F *h4 = (TH1F *) p4->GetPrimitive("BAR8_PDA");
    h4->SetStats(0);
    h4->Draw();
}


plot_totE()
{
//    TFile f5("~/mcalsw_output_COMMISSIONING/H/H000229_3909.root");
//    TFile f5("/data1/mcalsw_output_AIV/H/H006152_3909_sel_01_good_cal.root");
    TFile f5("/home/mcal/data_analysis/calibration/CGS_Na22/root/H006152_3909_sel_01.root");
    f5.ls();
    TObjArray *a5 = (TObjArray *) f5.Get("rawHisto");
    TCanvas *c5 = new TCanvas();
    c5->SetLogx();
    c5->SetLogy();
//    gPad->SetTicks(1,1);   
//    gPad->SetFrameBorderMode(0);
    TH1F *h5 = (TH1F *) a5->FindObject("totE");
    h5->GetYaxis()->SetTitle("Counts / (s MeV)");
    h5->SetStats(0);
//    h5->SetTitle("MCAL reconstructed energy spectrum");
    h5->Scale(1./0.02);
    h5->Draw();
}

plot_mult()
{
    TFile f6("3939_tot_mult.root");
    f6.ls();
    TPad *p6 = f6.Get("ec0_14");
    p6->ls();
    TCanvas *c6 = new TCanvas();
    TH1F *h6 = (TH1F *) p6->GetPrimitive("totMult");
    h6->SetStats(0);
    h6->Draw();
}


plot_CR()
{
    TFile f7("~/mcalsw_output_COMMISSIONING/H/H000229_3909.root");
    f7.ls();
    TObjArray *a7 = (TObjArray *) f7.Get("rawHisto");
    TCanvas *c7 = new TCanvas();
    TH1F *h7 = (TH1F *) p7->GetPrimitive("BAR8_CR");
    h7->SetStats(0);
    h7->Draw();
}


plot_DT()
{
    TFile f8("/data1/mcalsw_output/H/13313_ec0_10.root");
    f8.ls();
    TPad *p8 = f8.Get("ec0_10");
    p8->ls();
    TCanvas *c8 = new TCanvas();
    TH1F *h8 = (TH1F *) p8->GetPrimitive("totDT");
    h8->SetStats(0);
    h8->Draw();

	double mu = 200.;		// average total count rate
	double par[2];
	fdt  = new TF1("fdt","expo", 100., 2000. );
//	fdt  = new TF1("fdt","pol1", 100., 2000.);
	par[0] = log(mu*1.E-6*102898.); // *trebin*1.E-6
	par[1] = -mu*1.E-6;		
//	par[0] = par[1]
//	double cf = hDT->Integral(100, 5000);
//	double ef = fdt->Integral(100, 5000);
//	par[0] = par[0]+log(cf/ef);	// normalize time delay function
	fdt->SetParameters(par);
	h8->SetAxisRange(100.,  2000., "X");
	h8->Fit("fdt","QR0+");//Q
	fdt->GetParameters(par);
	cout << par[0] << "  " << par[1] << endl;
	fdt->SetRange(0., 100000. );
	fdt->SetLineColor(2);
	fdt->Draw("SAME");

}

plot_calibration_parameters()
{
    float ly, alpha;
    FILE *ft;
    ft = fopen("~/documents/AGILE/activities/barre/parameteri_barre_post_housing.txt", "r");
    for (int i=0; i<60; i++) {
	fscanf(ft, "%f %f", &ly, &alpha);
	cout << ly << "  " << alpha << endl;
    }
    fclose(ft);
}

plot_sci_RM()
{
	TFile f9("/data1/mcalsw_output_AIV/H/H006152_3909_sel_07.root");
	f9.ls();
	TObjArray *a9 = (TObjArray *) f9.Get("rawHisto");
	TCanvas *c9 = new TCanvas();
	c1->Draw();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetTicks(1,1);
	TH2F *h2 = new TH2F("h2", "", 100, 0.1, 10000., 100, 0.01, 1000);
	h2->Draw();
	h2->SetStats(0);
	h2->SetTitle("");
	h2->GetYaxis()->SetTitle("Counts / s");
	h2->GetYaxis()->SetTitleSize(0.05);
	h2->GetYaxis()->CenterTitle();
	h2->GetXaxis()->SetTitle("Energy (MeV)");
	h2->GetXaxis()->SetTitleSize(0.05);
	h2->GetXaxis()->CenterTitle();
	TGraphErrors *g9 = (TGraphErrors *) a9->FindObject("gRMX3913");
	g9->GetYaxis()->SetTitle("Counts / s");
	g9->GetYaxis()->SetTitleSize(0.05);
	g9->GetYaxis()->CenterTitle();
	g9->GetXaxis()->SetTitle("Energy (MeV)");
	g9->GetXaxis()->SetTitleSize(0.05);
	g9->GetXaxis()->CenterTitle();
	g9->Draw("P");

}

plot_sci_RM2()
{
	// se non funziona lanciare prima a mano queste 6 righe e poi rilanciare la routine! (boh???)
	TFile f9("/data1/mcalsw_output_AIV/H/H006152_3909_sel_07.root");
	f9.ls();
	TObjArray *a9 = (TObjArray *) f9.Get("rawHisto");
	TCanvas *c9 = new TCanvas("c9", "c9", 600, 800);
	c9->Divide(1, 2);
	c9->Draw();
	
	TH2F *hXERM = (TH2F *) a9->FindObject("hXERM");
    	hXERM->Scale(1./0.02);	
	TH1D *hEX = hXERM->ProjectionX("hEX", 1, 12, "");
	
	
	c9->cd(2);
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetTicks(1,1);
	// arrays to calculate the sci-rm boundaries
	int bin50loX[11];
	int bin50hiX[11];
	int bin50loZ[11];
	int bin50hiZ[11];
	float e50loX[11];
	float e50hiX[11];
	float e50loZ[11];
	float e50hiZ[11];
	
	//cout << endl << "plane X " << endl << "band counts zero_counts ratio" << endl;
	cout << endl << "plane X " << endl << "band lo50%bin lo50%E hi50%bin hi50%E " << endl;
	for (int i=1; i<=11; i++) {
		TString pname("X");
		pname += i;
		TH1D *aux = hXERM->ProjectionX(pname.Data(), i, i, "");
		bool low=1;
		bool hi=0;
		
		// dividi i bin per totE and calculate band's boundaries	
		for (int j=0; j<50000; j++) {
			if (hEX->GetBinContent(j)) av = aux->GetBinContent(j)/hEX->GetBinContent(j);
			aux->SetBinContent(j,av);
			
			if (low && av<=0.5) {
				bin50loX[i-1] = j;
				e50loX[i-1] = aux->GetBinLowEdge(j);
			} else {
				low = 0;
				hi = 1;
			}
			if (hi && av>=0.5) {
				bin50hiX[i-1] = j;
				e50hiX[i-1] = aux->GetBinLowEdge(j);
			}
		}
		
		//aux->SetLineColor(i==10 ? 13 : i);
		float ntot = aux->Integral();
		//cout << i << "  " << ntot << "  " << aux->GetBinContent(1) << "  " << aux->GetBinContent(1)/ntot << endl;
		cout << i << "  " << bin50loX[i-1] << "  " << e50loX[i-1] << "  " << bin50hiX[i-1] << "  " << e50hiX[i-1] << endl;
		if (i==1) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Events fraction");
			aux->GetYaxis()->CenterTitle();
			aux->SetStats(0);
 		   	aux->SetTitle("");
			aux->GetXaxis()->SetRangeUser(0.2, 40.);
			aux->GetYaxis()->SetRangeUser(0.01, 2.);
			aux->Draw();
		}
		else aux->Draw("SAME");
	}
	for (int i=1; i<11; i++) {
		TString s;
		s += i;
		TText t;
		t.DrawText(0.5*(e50hiX[i] + e50loX[i]), 1.2, s.Data());
	}
	
	
	c9->cd(1);
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetTicks(1,1);
	
	// draw plane X reconstructed energy
    	hEX->SetStats(0);
    	hEX->SetTitle("");
	hEX->GetYaxis()->SetTitle("Counts / s MeV");
	hEX->GetXaxis()->SetRangeUser(0.2, 40.);
	hEX->Draw();
	
	// draw sci rm graph, normalized to energy
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	TGraphErrors *g9 = (TGraphErrors *) a9->FindObject("gRMX3913");
	int n = g9->GetN();
	double *y = g9->GetY();
	double *x = g9->GetX();
	double *ey = g9->GetEY();
	double *ex = g9->GetEX();
	e50loX[0] = 0;
	e50hiX[0] = e50loX[1];
	
	// rescaling with the newly found bands
	for (int i=0; i<n; i++) x[i] = 0.5*(e50loX[i] + e50hiX[i]);
	for (int i=0; i<n; i++) ex[i] = 0.5*(e50hiX[i] - e50loX[i]);
	for (int i=0; i<n; i++) y[i] /= (e50hiX[i] - e50loX[i]);
	for (int i=0; i<n; i++) ey[i] /= (e50hiX[i] - e50loX[i]);
	
	// rescaling with standard energy bands
//	for (int i=0; i<n; i++) y[i] /= 2.*tmpEerr[i];
//	for (int i=0; i<n; i++) ey[i] /= 2.*tmpEerr[i];

	TGraphErrors *g10 = new TGraphErrors(n, x, y, ex, ey);
	
	for (int i=0; i<n; i++) cout << i<< " " << x[i] << "  " << y[i] << "  " << ex[i] << "  " << ey[i] << endl;
	
	g10->GetYaxis()->SetTitle("Counts / s MeV");
	g10->GetYaxis()->SetTitleSize(0.05);
	g10->GetYaxis()->CenterTitle();
	g10->GetXaxis()->SetTitle("Energy (MeV)");
	g10->GetXaxis()->SetTitleSize(0.05);
	g10->GetXaxis()->CenterTitle();
	g10->Draw("PSAME");
	return;
}
