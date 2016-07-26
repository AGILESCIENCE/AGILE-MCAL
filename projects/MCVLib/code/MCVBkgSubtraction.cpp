//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "MCVBkgSubtraction.h"
ClassImp(MCVBkgSubtraction);

MCVBkgSubtraction::MCVBkgSubtraction() : MCVStdAnalysis(), fbkg(NULL), fsim(NULL), hstotE(NULL), asrawE(NULL), abkgrawE(NULL), arawE(NULL), asimrawE(NULL), hsimtotE(NULL), hsX(NULL), hsZ(NULL), asim(NULL)
{
}

//
//--------------------------------------------------------------------
//

MCVBkgSubtraction::MCVBkgSubtraction(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in), fbkg(NULL), fsim(NULL), hstotE(NULL), asrawE(NULL), abkgrawE(NULL), arawE(NULL), asimrawE(NULL), hsimtotE(NULL), hsX(NULL), hsZ(NULL), asim(NULL)
{
}

//
//--------------------------------------------------------------------
//

MCVBkgSubtraction::~MCVBkgSubtraction()
{

}

//
//--------------------------------------------------------------------
//

int MCVBkgSubtraction::Calculate()
{
	TH1F *hbkg, *he, *hsbarE;
	
	epeak = conf->GetValue(".source.energy.", -1.);
	refname.Append(getenv("MCALSW"));
	refname.Append(conf->GetValue(".bkg.file.", "bkg.root"));
	fbkg = new TFile(refname.Data());
	if (!fbkg) {
		cout << "MCVBkgSubtraction::Calculate(): Cannot find reference file " << refname.Data() << endl;
		status = REF_FILE_NOT_FOUND;
		return status;
	}
	if (!rawHisto) {
		status = RAW_HISTO_NOT_SET;
		return status;
	}
	
	TObjArray *abkg;
	TIter next = fbkg->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next())) {
		if (strncmp(key->GetName(),"rawHisto", 8) == 0) abkg = (TObjArray *) fbkg->Get(key->GetName());
	}
	
	// work on total energy histogram
	hbkg = (TH1F *) abkg->FindObject("totE");
	he   = (TH1F *) rawHisto->FindObject("totE");
	hstotE = new TH1F("hstotE", "bkg subtracted spectrum", 100000, 0, 2000.);
	for (int i=0; i<100000; i++) hstotE->SetBinContent(i, he->GetBinContent(i) - hbkg->GetBinContent(i));
		
	// update graphics if necessary	
	if (c1) {
		c1->cd(15);
		hstotE->SetLineColor(2);
		hstotE->SetMarkerColor(2);
		hstotE->Draw("same");
		c1->Update();
	}
	
	// work on single bars energy histograms
	asrawE   = new TObjArray(30,0);
	abkgrawE = (TObjArray *) abkg->FindObject("rawE");	// get background bars energy histograms
	arawE = (TObjArray *) rawHisto->FindObject("rawE");	// get current bars energy histograms
	for (int i=0; i<30; i++) {
		TString hname("hsBAR");
		hname += i;
		hname += "_E";
		hbkg = (TH1F *) abkgrawE->At(i);
		he   = (TH1F *) arawE->At(i);
		hsbarE = new TH1F(hname.Data(), "bkg subtracted spectrum", 100000, 0, 2000.);
		for (int i=0; i<100000; i++) hsbarE->SetBinContent(i, he->GetBinContent(i) - hbkg->GetBinContent(i));
		asrawE->AddLast(hsbarE);
		// update graphics if necessary	
		if (c2) {
			c2->cd(i+1);
			hsbarE->SetLineColor(2);
			hsbarE->SetMarkerColor(2);
			hsbarE->Draw("same");
		}
	}
	asrawE->SetName("asrawE");
	
	// fit bkg subtracted histogram peaks for all bars
	if (epeak) {
		MCVFitPeak *anFitPeak;
		anFitPeak = new MCVFitPeak(epeak, asrawE, info, 1);
		if (c2) anFitPeak->SetCanvases(c2);
		anFitPeak->Calculate();
		cout << endl << "BACKGROUND SUBTRACTED BARS FIT" << endl;
		anFitPeak->PrintResults();
		if (c2) c2->Update();
		
		// retrieve peak info
		double *auxpc = anFitPeak->GetCenter();
		double *auxps = anFitPeak->GetSigma();
		double *auxpr = anFitPeak->GetCounts();
		for (int i=0; i<30; i++) {
			pc[i] = auxpc[i];
			ps[i] = auxps[i];
			pr[i] = auxpr[i];
		}
	}
	
	// build bkg subtracted raw images
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
	
	TH2F *hauxiX = (TH2F *)  abkg->FindObject("hX");
	TH2F *hauxiZ = (TH2F *)  abkg->FindObject("hZ");;
	TH2F *hauxeX = (TH2F *)  rawHisto->FindObject("hX");
	TH2F *hauxeZ = (TH2F *)  rawHisto->FindObject("hZ");
	
	for (int i=1; i<=15; i++) {
		for (int j=1; j<=60; j++) {
			hsX->SetBinContent(i, j, hauxeX->GetBinContent(i,j)-hauxiX->GetBinContent(i,j));
			hsZ->SetBinContent(j, i, hauxeZ->GetBinContent(j,i)-hauxiZ->GetBinContent(j,i));
		}
	}
	hsZ->GetXaxis()->SetRangeUser(-20, 20);
	hsX->GetYaxis()->SetRangeUser(-20, 20);
	
	FitTotEHisto();
		
	simCompare = conf->GetValue(".sim.compare.", 0);
	if (simCompare) SimulationCompare();
	if (c2) DrawZoom();
	return status = OK;
}

//
//--------------------------------------------------------------------
//

void MCVBkgSubtraction::FitTotEHisto()
{
	// fit total energy histo after bkg subtraction
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;
	double 	par[5], par1[3]; 
	float dElo=0.3;
	float dEhi=0.4;
	
	cout << endl << "Fitting " << epeak << " MeV peak in total bkg-subtracted spectrum" << endl;
	
	int ibin = hstotE->GetXaxis()->FindBin(epeak);
	par1[0] = hstotE->GetBinContent(ibin);
	par1[1] = epeak; 
	par1[2] = 0.060;
	peak  = new TF1("peak","gaus", epeak-0.2,  epeak+0.2); // (int) 0.1
	bkg   = new TF1("bkg", "expo", epeak-dElo,  epeak+dEhi);
	total = new TF1("total","expo(0)+gaus(2)", epeak-dElo,  epeak+dEhi);
	peak->SetParameters(par1);
	hstotE->Fit("peak","QR0+");
	hstotE->Fit("bkg","QR0+");
	peak->GetParameters(&par[2]);
	bkg->GetParameters(&par[0]);
	total->SetParameters(par);
	hstotE->Fit("total","R0+");
	total->GetParameters(par);
	if (c1) {
		//hstotE->SetAxisRange(epeak-dElo,  epeak+dEhi, "X");
		c1->cd(15);
		total->SetLineWidth(2);
		total->SetLineColor(2);
		total->Draw("same");

		peak->SetRange(epeak-dElo,  epeak+dEhi);
		peak->SetParameters(&par[2]);
		peak->SetLineWidth(1);
		peak->SetLineColor(3);
		peak->Draw("same");
		
		bkg->SetParameters(par);
		bkg->SetLineWidth(1);
		bkg->SetLineColor(4);
		bkg->Draw("same");
		c1->Update();
	} 
	pctot = par[3];
	pstot = par[4];
	prtot = par[2]*par[4]*sqrt(2.*TMath::Pi())/0.020;
	cout << "Total rate in the photopeak: " << prtot << " evt/s" << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVBkgSubtraction::FitSimTotEHisto()
{
	// fit total energy histo for simulations
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;
	double 	par[5], par1[3]; 
	float dElo=0.3;
	float dEhi=0.4;
	
	cout << endl << "Fitting " << epeak << " MeV peak in total simulation spectrum" << endl;
	
	int ibin = hsimtotE->GetXaxis()->FindBin(epeak);
	par1[0] = hsimtotE->GetBinContent(ibin);
	par1[1] = epeak; 
	par1[2] = 0.060;
	peak  = new TF1("peak","gaus", epeak-0.2,  epeak+0.2); // (int) 0.1
	bkg   = new TF1("bkg", "expo", epeak-dElo,  epeak+dEhi);
	total = new TF1("total","expo(0)+gaus(2)", epeak-dElo,  epeak+dEhi);
	peak->SetParameters(par1);
	hsimtotE->Fit("peak","QR0+");
	hsimtotE->Fit("bkg","QR0+");
	peak->GetParameters(&par[2]);
	bkg->GetParameters(&par[0]);
	total->SetParameters(par);
	hsimtotE->Fit("total","R0+");
	total->GetParameters(par);
	pcsimtot = par[3];
	pssimtot = par[4];
	prsimtot = par[2]*par[4]*sqrt(2.*TMath::Pi())/0.020;
	cout << "Total rate in the photopeak: " << prsimtot << " evt/s" << endl;
}

//
//--------------------------------------------------------------------
//

void MCVBkgSubtraction::SimulationCompare()
{
	simname.Append(getenv("MCALSW"));
	simname.Append(conf->GetValue(".sim.file.", "sim.root"));
	fsim = new TFile(simname.Data());
	if (!fsim) {
		cout << "MCVBkgSubtraction::SimulationCompare(): Cannot find reference file " << simname.Data() << endl;
		status = REF_FILE_NOT_FOUND;
		return;
	}
	
	TIter next = fsim->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next())) {
		if (strncmp(key->GetName(),"rawHisto", 8) == 0) asim = (TObjArray *) fsim->Get(key->GetName());
	}
	
	//asim = (TObjArray *) fsim->Get("rawHisto");  // overwritten by the above loop
	asimrawE = (TObjArray *) asim->FindObject("rawE");	// get simulation bars energy histograms
	hsimtotE = (TH1F *) asim->FindObject("totE");
		
	// fit bkg subtracted histogram peaks for all bars
	if (epeak) {
		MCVFitPeak *anFitPeakSim;
		anFitPeakSim = new MCVFitPeak(epeak, asimrawE, info, 1);
		//anFitPeakSim->SetUpperDE(-0.075);	// non da buoni risultati
		//anFitPeakSim->SetLowerDE(0.225);
		anFitPeakSim->Calculate();
		cout << endl << "SOURCE SIMULATION BARS FIT" << endl;
		anFitPeakSim->PrintResults();
		
		// retrieve peak info
		double *auxpc = anFitPeakSim->GetCenter();
		double *auxps = anFitPeakSim->GetSigma();
		double *auxpr = anFitPeakSim->GetCounts();
		for (int i=0; i<30; i++) {
			pcsim[i] = auxpc[i];
			pssim[i] = auxps[i];
			prsim[i] = auxpr[i];
		}
	}
	FitSimTotEHisto();
}

//
//--------------------------------------------------------------------
//

void MCVBkgSubtraction::DrawZoom()
{
	TCanvas *cz;
	TH1F *hbkg, *he, *hsbarE, *hsim;
	TF1 *peak, *simpeak;
	TF1 *bkg, *simbkg;
	TF1 *total, *simtotal;
	double 	par[5];
	
	for (int i=0; i<30; i++) {
		if (i%6 == 0) { // create new canvas
			TString cname("cz");
			cname += (i/6 +1);
			TString ctitle("Zoomed bars ");
			ctitle += (i/6 +1);
			cz = new TCanvas(cname.Data(), ctitle.Data(), 1000, 600);
			cz->Divide(3, 2);
		}
		cz->cd(i%6 + 1);
		he = (TH1F *) arawE->At(i);
		he->GetXaxis()->SetRangeUser(0., 2.);
		he->Draw();
		hsbarE = (TH1F *) asrawE->At(i);
		hsbarE->Draw("SAME");
		
		total = hsbarE->GetFunction("total");
		total->SetLineWidth(2);
		total->SetLineColor(2);
		total->Draw("SAME");
		total->GetParameters(par);
		
		bkg = hsbarE->GetFunction("bkg");
		bkg->SetRange(epeak-0.3,  epeak+0.3);
		bkg->SetParameters(par);
		bkg->SetLineWidth(1);
		bkg->SetLineColor(4);		
		bkg->Draw("SAME");
		
		peak = hsbarE->GetFunction("peak");
		peak->SetRange(epeak-0.3,  epeak+0.3);
		peak->SetParameters(&par[2]);
		peak->SetLineWidth(1);
		peak->SetLineColor(3);
		peak->Draw("SAME");
		
		if (simCompare) {	// add simulation information
			hsim = (TH1F *) asimrawE->At(i);
			hsim->SetLineColor(6);		
			hsim->Draw("SAME");
		
			simtotal = hsim->GetFunction("total");
			simtotal->SetLineWidth(2);
			simtotal->SetLineColor(2);
			simtotal->SetLineStyle(2);
			simtotal->Draw("SAME");
			simtotal->GetParameters(par);
			
			simbkg = hsim->GetFunction("bkg");
			simbkg->SetRange(epeak-0.3,  epeak+0.3);
			simbkg->SetParameters(par);
			simbkg->SetLineWidth(1);
			simbkg->SetLineColor(4);
			simbkg->SetLineStyle(2);
			simbkg->Draw("SAME");
			
			simpeak = hsim->GetFunction("peak");
			simpeak->SetRange(epeak-0.3,  epeak+0.3);
			simpeak->SetParameters(&par[2]);
			simpeak->SetLineWidth(1);
			simpeak->SetLineColor(3);
			simpeak->SetLineStyle(2);
			simpeak->Draw("SAME");
		}
	}
			
	// draw total energy spectrum with simulations
	cz = new TCanvas("cz6", "total energy spectra", 800, 600);
	he   = (TH1F *) rawHisto->FindObject("totE");
	he->GetXaxis()->SetRangeUser(0., 2.);
	he->Draw();
	he->SetStats(0);
	hstotE->Draw("same");
		
	total = hstotE->GetFunction("total");
	total->SetLineWidth(2);
	total->SetLineColor(2);
	total->Draw("SAME");
	total->GetParameters(par);
	
	bkg = hstotE->GetFunction("bkg");
	bkg->SetRange(epeak-0.3,  epeak+0.3);
	bkg->SetParameters(par);
	bkg->SetLineWidth(1);
	bkg->SetLineColor(4);		
	bkg->Draw("SAME");
	
	peak = hstotE->GetFunction("peak");
	peak->SetRange(epeak-0.3,  epeak+0.3);
	peak->SetParameters(&par[2]);
	peak->SetLineWidth(1);
	peak->SetLineColor(3);
	peak->Draw("SAME");
	
	TLegend *leg = new TLegend(0.7, 0.8, 0.95, 0.95);
	leg->AddEntry(he, "raw spectrum", "lp");
	leg->AddEntry(hstotE, "bkg subtracted spectrum", "lp");

	if (simCompare) {
		hsimtotE->SetLineColor(6);		
		hsimtotE->Draw("SAME");
		leg->AddEntry(hsimtotE, "simulated spectrum", "lp");
	
		simtotal = hsimtotE->GetFunction("total");
		simtotal->SetLineWidth(2);
		simtotal->SetLineColor(2);
		simtotal->SetLineStyle(2);
		simtotal->Draw("SAME");
		simtotal->GetParameters(par);
		
		simbkg = hsimtotE->GetFunction("bkg");
		simbkg->SetRange(epeak-0.3,  epeak+0.3);
		simbkg->SetParameters(par);
		simbkg->SetLineWidth(1);
		simbkg->SetLineColor(4);
		simbkg->SetLineStyle(2);
		simbkg->Draw("SAME");
		
		simpeak = hsimtotE->GetFunction("peak");
		simpeak->SetRange(epeak-0.3,  epeak+0.3);
		simpeak->SetParameters(&par[2]);
		simpeak->SetLineWidth(1);
		simpeak->SetLineColor(3);
		simpeak->SetLineStyle(2);
		simpeak->Draw("SAME");
	}
	leg->Draw();
	
	// draw bkg subtracted and simulation raw images
	cz = new TCanvas("cz7", "raw images", 600, 600);
	cz->Divide(2, 2);
	cz->cd(1);
	hsX->Draw("COLZ");
	cz->cd(2);
	hsZ->Draw("COLZ");
	if (simCompare) {
		cz->cd(3);
		asim->FindObject("hX")->Draw("COLZ");
		cz->cd(4);
		asim->FindObject("hZ")->Draw("COLZ");
	}	
	return;
}

//
//--------------------------------------------------------------------
//

void MCVBkgSubtraction::PrintResults()
{
	if ((status == OK || status == OUT_OF_SPEC) && simCompare) {
		printf("\n--------------------- BKG SUBTRACTION ANALYSIS AND SIMULATION COMPARISON ----------------------\n");
		printf("\n\t\tBkg subtracted data \t\t\t\tSimulations\n");
		printf("plane \tbar \tcenter (MeV) \tsigma (MeV) \tcounts \t\tcenter (MeV) \tsigma (MeV) \tcounts \t\trate dev. (%)\n");
		for (int i=0; i<15; i++) printf("X \t%d \t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f\n", i+1, pc[i], ps[i], pr[i], pcsim[i], pssim[i], prsim[i], 100.*(prsim[i]/pr[i]-1.));
		for (int i=15; i<30; i++) printf("Z \t%d \t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f \t\t%.3f\n", i-14, pc[i], ps[i], pr[i], pcsim[i], pssim[i], prsim[i], 100.*(prsim[i]/pr[i]-1.));
		printf("\ntot \t\t%.3f \t\t%.3f \t\t%.3f \t%.3f \t\t%.3f \t%.3f \t%.3f\n", pctot, pstot, prtot, pcsimtot, pssimtot, prsimtot, 100.*(prsimtot/prtot-1.));
		printf("\n--------------------------------------------------------------------------\n");	
	}
	
	return;
}

