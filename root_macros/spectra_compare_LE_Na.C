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

//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013106_3909.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013110_3909.root");

// 19-20/12/06 satellite RFT @CGS
// RFT Na22 on top
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H011817_3909_sel_01.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H011818_3909.root");


// 4-5/3/07 satellite FFT @IABG
// FFT Na22 on top
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013595_3909.root");

// FFT Na22 on AC side 1
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013596_3909.root");

// FFT Na22 on AC side 2
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013597_3909.root");

// FFT Na22 on AC side 3
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013598_3909.root");

// FFT Na22 on AC side 4
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013599_3909.root");

// selection for low energy burst threshold test
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013594_3909_sel_02.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013597_3909_sel_01.root");

// selection for low energy burst threshold test (BELLO)
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013642_3909_sel_01.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013643_3908.root");

// selection for energy reconstruction efficiency:
//	standard exponential approach
//TFile fi("/data1/mcalsw_output/H/H000628_BURST_0.root");	// bkg
//TFile fe("/data1/mcalsw_output/H/H000634_BURST_0.root");	// Na22 flood in front of MCAL @IASF-Bo 16/7/05
//TFile fe("/data1/mcalsw_output/H/H000632_BURST_0.root");	// Cs137 flood in front of MCAL @IASF-Bo 16/7/05

//	polinomial functions approach
//TFile fi("/data1/mcalsw_output/H/H000628_BURST_2.root");	// bkg
//TFile fe("/data1/mcalsw_output/H/H000634_BURST_2.root");	// Na22 flood in front of MCAL @IASF-Bo 16/7/05
//TFile fe("/data1/mcalsw_output/H/H000632_BURST_2.root");	// Cs137 flood in front of MCAL @IASF-Bo 16/7/05

//	polinomial functions approach (cut in ADC)
//TFile fi("/data1/mcalsw_output/H/H000628_BURST_2_cut.root");	// bkg
//TFile fe("/data1/mcalsw_output/H/H000632_BURST_2_cut.root");	// Cs137 flood in front of MCAL @IASF-Bo 16/7/05

// Na22 flood tests @CGS 13-15/9/06
TFile fi("/data1/mcalsw_output_AIV/H/H011679_3909_sel_01_0.root");	// bkg, rec.meth. 0
//TFile fi("/data1/mcalsw_output/H/H011679_3909_sel_01_2.root");	// bkg, rec.meth. 2
//TFile fi("/data1/mcalsw_output_AIV/H/H011679_3909_sel_01.root");	// bkg, only single events
//TFile fe("/data1/mcalsw_output/H/H011683_3909_sel_01_0.root");	// Na22 flood theta=46° phi=0° @CGS 13/9/06, rec.meth. 0
//TFile fe("/data1/mcalsw_output/H/H011683_3909_sel_01_2.root");	// Na22 flood theta=46° phi=0° @CGS 13/9/06, rec.meth. 2
//TFile fi("/data1/mcalsw_output_AIV/H/H011696_3909.root");		// bkg, rec.meth. 0

//TFile fe("/data1/mcalsw_output_AIV/H/H011697_3909_sel_02.root");	// Na22 flood theta=90° phi=0° @CGS 13/9/06, rec.meth. 0
TFile fe("/data1/mcalsw_output_AIV/H/H011680_3909_sel_01.root");	// Na22 flood theta=0° phi=0° @CGS 13/9/06, rec.meth. 0
//TFile fe("/data1/mcalsw_output_AIV/H/H011680_3909_sel_02.root");	// Na22 flood theta=0° phi=0° @CGS 13/9/06, only single

// 6-7/4/07 extra RFT @SHAR
// Na22 on top
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013712_3909_sel_04.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013715_3909.root");

// Na22 on AC side 4
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013741_3909_sel_03.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013742_3909.root");

// FFT Na22 on AC side 1
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013741_3909_sel_03.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013743_3909.root");

// FFT Na22 on AC side 2
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013741_3909_sel_03.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013744_3909.root");

// FFT Na22 on AC side 3
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H013741_3909_sel_03.root");
//TFile fe("~/local/mcalsw/bin/mcalsw_output/H/H013745_3909.root");

TObjArray *ai = (TObjArray *) fi->Get("rawHisto");
TObjArray *ae = (TObjArray *) fe->Get("rawHisto");
//TObjArray *ai = (TObjArray *) fi->Get("rawHisto_sel_0");
//TObjArray *ae = (TObjArray *) fe->Get("rawHisto_sel_0");
float ti = 928.;
float te = 10.;

TH1F *hi;
hi = (TH1F *) ai->FindObject("totE");

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
//gPad->SetLogx();
//gPad->SetLogy();
hi->SetStats(0);
//hi->SetTitle("MCAL reconstructed energy spectra");
hi->SetTitle(0);
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
gPad->SetTicks(1,1);

TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 200, 0, 4.);
//TH1F *hdiff = new TH1F("hdiff", "bkg subtracted beam spectrum", 2000, 0, 2000.);
for (int i=0; i<200; i++) hdiff->SetBinContent(i, he->GetBinContent(i) - hi->GetBinContent(i));
hdiff->SetLineColor(3);
hdiff->SetMarkerColor(3);
hdiff->Draw("same");

// draw simulated spectrum, for comparison
if (0) {
	//TFile fs("/data1/mcalsw_output/H/H000000_BURST_Na22_T090_P000_0.root");// Na22 simulated spectrum flood theta=90° phi=0° 
	TFile fs("/data1/mcalsw_output/H/H000000_BURST.root");	// Na22 simulated spectrum flood theta=90° phi=0° 
	
	TObjArray *as = (TObjArray *) fs->Get("rawHisto");
	TH1F *hs;
	hs = (TH1F *) as->FindObject("totE");
	hs->Scale(1./0.02);
	hs->SetLineColor(4);
	hs->Draw("same");
}

// fit 1275 keV peak of bkg-subtracted spectrum
if (0) {
	double Ei = 1.275;
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;
	double 	par[5], par1[3]; 
	
	int ibin = hdiff->GetXaxis()->FindBin(Ei);
	par1[0] = hdiff->GetBinContent(ibin);
	par1[1] = Ei; 
	par1[2] = 0.060;
	peak  = new TF1("peak","gaus", Ei-0.2,  Ei+0.2); // (int) 0.1
	bkg   = new TF1("bkg", "expo", Ei-0.3,  Ei+0.3);
	total = new TF1("total","expo(0)+gaus(2)", Ei-0.3,  Ei+0.3);
	peak->SetParameters(par1);
	hdiff->Fit("peak","QR0+");
	hdiff->Fit("bkg","QR0+");
	peak->GetParameters(&par[2]);
	bkg->GetParameters(&par[0]);
	total->SetParameters(par);
	hdiff->Fit("total","R0+");	//Q if you want it quiet
	total->GetParameters(par);
	total->SetLineWidth(2);
	total->SetLineColor(2);
	total->Draw("same");
	
	peak->SetRange(Ei-0.3,  Ei+0.3);
	peak->SetParameters(&par[2]);
	peak->SetLineWidth(1);
	peak->SetLineColor(3);
	peak->Draw("same");
	
	bkg->SetParameters(par);
	bkg->SetLineWidth(1);
	bkg->SetLineColor(4);
	bkg->Draw("same");
}

// fit 511 keV peak of bkg-subtracted spectrum
if (0) {
	double Ei = 0.511;
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;
	double 	par[5], par1[3]; 
	
	int ibin = hdiff->GetXaxis()->FindBin(Ei);
	par1[0] = hdiff->GetBinContent(ibin);
	par1[1] = Ei; 
	par1[2] = 0.060;
	peak  = new TF1("peak2","gaus", Ei-0.1,  Ei+0.1); // (int) 0.1
	bkg   = new TF1("bkg2", "expo", Ei+0.2,  Ei+0.5);
	total = new TF1("total2","expo(0)+gaus(2)", Ei-0.1,  Ei+0.5);
	peak->SetParameters(par1);
	hdiff->Fit("peak","QR0+");
	hdiff->Fit("bkg","QR0+");
	peak->GetParameters(&par[2]);
	bkg->GetParameters(&par[0]);
	total->SetParameters(par);
	hdiff->Fit("total","R0+");	//Q if you want it quiet
	total->GetParameters(par);
	total->SetLineWidth(2);
	total->SetLineColor(2);
	total->Draw("same");
	
	peak->SetRange(Ei-0.3,  Ei+0.3);
	peak->SetParameters(&par[2]);
	peak->SetLineWidth(1);
	peak->SetLineColor(3);
	peak->Draw("same");
	
	bkg->SetParameters(par);
	bkg->SetLineWidth(1);
	bkg->SetLineColor(4);
	bkg->Draw("same");
}


/*
cout << "Excess rate = " << (he->Integral() - hi->Integral())*0.02 << " evt/s" << endl;


TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "run 13106 background", "lp");
leg->AddEntry(he, "run 13110 Na22 source", "lp");
leg->AddEntry(hdiff, "bkg subtracted spectrum", "lp");
leg->Draw();

float ti = 240.;
float te = 240.;
float x[100000], ex[100000], y[100000], ey[100000];
for (int i=0; i<2000; i++) {
    x[i] = i*0.02+0.01;
    ex[i] = 0.01;
    y[i] = he->GetBinContent(i) - hi->GetBinContent(i);    
    ey[i] = sqrt(hi->GetBinContent(i)/ti + he->GetBinContent(i)/te);
}

TGraphErrors *gr1 = new TGraphErrors(100000, x, y, ex, ey);
TCanvas *c2;
c2 = new TCanvas();
c2->Draw();
//gPad->SetLogx();
//gPad->SetLogy();
gr1->Draw("AP");

TF1 *peak, *bkg, *total;
double par[6];
gr1->GetXaxis()->SetRangeUser(0.8,  1.9);
gr1->GetYaxis()->SetRangeUser(0.,  350.);
peak  = new TF1("peak","gaus", 1.20,  1.35); // (int)
bkg   = new TF1("bkg", "expo", 0.9, 1.8);
total = new TF1("total","pol1(0)+gaus(2)", 0.9, 1.8);
gr1->Fit("peak","R0+");
gr1->Fit("bkg","R0+");
peak->GetParameters(&par[2]);
bkg->GetParameters(&par[0]);
total->SetLineColor(2);
total->SetParameters(par);
gr1->Fit("total","R0+");
total->GetParameters(par);

total->SetLineWidth(2);
total->SetLineColor(2);
total->Draw("same");

peak->SetParameters(&par[2]);
peak->SetRange(1., 1.6);
peak->SetLineWidth(1);
peak->SetLineColor(3);
peak->Draw("same");

bkg->SetParameters(par);
bkg->SetLineWidth(1);
bkg->SetLineColor(4);
bkg->Draw("same");




// work on single bars

TCanvas *c3;
c3 = new TCanvas("c3", "c3", 800, 600);
c3->Divide(5, 6);
TObjArray *aiE = (TObjArray *) ai->FindObject("rawE");
TObjArray *aeE = (TObjArray *) ae->FindObject("rawE");

TH1F *hauxi;
TH1F *hauxe;
TH1F *hs;
for (int i=0; i<30; i++) {
	c3->cd(i+1);
	TString hname("hs");
	hname += i;
	hauxi = (TH1F *) aiE->At(i);
	hauxe = (TH1F *) aeE->At(i);
 	hs = new TH1F(hname.Data(), hname.Data(), 7500, 0., 150.);	
	for (int j=0; j<7500; j++) hs->SetBinContent(j, hauxe->GetBinContent(j) - hauxi->GetBinContent(j));
	hs->GetXaxis()->SetRangeUser(0., 2.);
	hs->Draw();
}

*/
// raw source image

TCanvas *c4;
c4 = new TCanvas("c4", "c4", 800, 400);
c4->Divide(2,1);
TH2F *hsX;
TH2F *hsZ;
	hsX = new TH2F("hsX", "X plane events distribution", 15, 0., 15., 60, -30., 30.);
	hsX->SetStats(0);
	hsX->SetTitle("Events distribution on X plane");
	hsX->GetXaxis()->SetTitle("Bar number");
	hsX->GetXaxis()->SetTitleSize(0.05);
	hsX->GetZaxis()->SetTitle("Counts/s cm");
	hsX->GetZaxis()->CenterTitle();
	hsX->GetZaxis()->SetTitleSize(0.05);
	hsX->GetZaxis()->SetLabelSize(0.05);
	hsX->GetXaxis()->CenterTitle();
	hsX->GetYaxis()->SetTitle("Position (cm)");
	hsX->GetYaxis()->SetTitleSize(0.05);
	hsX->GetYaxis()->CenterTitle();
	
	hsZ = new TH2F("hsZ", "Z plane events distribution", 60, -30., 30., 15, 0., 15.);
	hsZ->SetStats(0);
	hsZ->SetTitle("Events distribution on Z plane");
	hsZ->GetXaxis()->SetTitle("Position (cm)");
	hsZ->GetXaxis()->SetTitleSize(0.05);
	hsZ->GetZaxis()->SetTitle("Counts/s cm");
	hsZ->GetZaxis()->CenterTitle();
	hsZ->GetZaxis()->SetTitleSize(0.05);
	hsZ->GetZaxis()->SetLabelSize(0.05);
	hsZ->GetXaxis()->CenterTitle();
	hsZ->GetYaxis()->SetTitle("Bar number");
	hsZ->GetYaxis()->SetTitleSize(0.05);
	hsZ->GetYaxis()->CenterTitle();
	
TH2F *hauxiX = (TH2F *)  ai->FindObject("hX");
TH2F *hauxiZ = (TH2F *)  ai->FindObject("hZ");;
TH2F *hauxeX = (TH2F *)  ae->FindObject("hX");
TH2F *hauxeZ = (TH2F *)  ae->FindObject("hZ");
	
for (int i=1; i<=15; i++) {
	for (int j=1; j<=60; j++) {
		//hsX->SetBinContent(i, j, hauxeX->GetBinContent(i,j)-hauxiX->GetBinContent(i,j));
		//hsZ->SetBinContent(j, i, hauxeZ->GetBinContent(j,i)-hauxiZ->GetBinContent(j,i));
		hsX->SetBinContent(i, j, hauxiX->GetBinContent(i,j));
		hsZ->SetBinContent(j, i, hauxiZ->GetBinContent(j,i));
	}
}
hsZ->GetXaxis()->SetRangeUser(-20, 20);
hsX->GetYaxis()->SetRangeUser(-20, 20);
c4->cd(1);
hsX->Draw("COLZ");
c4->cd(2);
hsZ->Draw("COLZ");

}
