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

#include "MCVHisto.h"
ClassImp(MCVHisto);

MCVHisto::MCVHisto():gMultX(NULL)
{
	TH1F *hA, *hB, *hpos, *hE, *hDT, *hMult; 
	TH1F *hCR, *hCRDist;
	TH2F *h2D;
	TGraph *gaux;
	char colname[30];
	
	// creates container class for ROOT histograms
	rawPDA = new TObjArray(30,0);
	rawPDB = new TObjArray(30,0);
	raw2D  = new TObjArray(30,0);
	rawPos = new TObjArray(30,0);
	rawE   = new TObjArray(30,0);
	rawCRate    = new TObjArray(30,0);
	rawCRDist   = new TObjArray(30,0);
	rawDTime    = new TObjArray(30,0);
	rawMult     = new TObjArray(30,0);
	rawSciRM3913= new TObjArray(22,0);
	rawACRM3913 = new TObjArray(5,0);
	rawHisto    = new TObjArray(30,0);   
	rawPDA->SetName("rawPDA");
	rawPDB->SetName("rawPDB");
	raw2D->SetName("raw2D");
	rawPos->SetName("rawPos");
	rawE->SetName("rawE");
	rawCRate->SetName("rawCRate");
	rawCRDist->SetName("rawCRDist");
	rawDTime->SetName("rawDTime");
	rawMult->SetName("rawMult");
	rawSciRM3913->SetName("rawSciRM3913");
	rawACRM3913->SetName("rawACRM3913");

	// creates raw histograms and adds them to container classes   
	for (int i=0; i<30; i++) { 
		sprintf(colname, "BAR%d_PDA",  i); 
		hA = new TH1F(colname, colname, 4096, 0, 4096);
		hA->GetXaxis()->SetTitle("ADC counts (ch)");
		hA->GetXaxis()->SetTitleSize(0.05);
		hA->GetXaxis()->SetLabelSize(0.05);
		hA->GetXaxis()->CenterTitle();
		hA->GetYaxis()->SetTitle("Counts");
		hA->GetYaxis()->SetTitleSize(0.05);
//		hA->GetYaxis()->SetTitleOffset(1.2);
		hA->GetYaxis()->SetLabelSize(0.05);
		hA->GetYaxis()->CenterTitle();
		rawPDA->AddLast(hA);
	
		sprintf(colname, "BAR%d_PDB",  i); 
		hB = new TH1F(colname, colname, 4096, 0, 4096);
		hB->GetXaxis()->SetTitle("ADC counts (ch)");
		hB->GetXaxis()->SetTitleSize(0.05);
		hB->GetXaxis()->SetLabelSize(0.05);
		hB->GetXaxis()->CenterTitle();
		hB->GetYaxis()->SetTitle("Counts");
		hB->GetYaxis()->SetTitleSize(0.05);
//		hB->GetYaxis()->SetTitleOffset(1.2);
		hB->GetYaxis()->SetLabelSize(0.05);
		hB->GetYaxis()->CenterTitle();
		rawPDB->AddLast(hB);
	
		sprintf(colname, "BAR%d_2D",  i); 
		h2D = new TH2F(colname, colname, 600, 0, 600, 600, 0, 600);
		h2D->SetAxisRange(190., 330., "X");
		h2D->SetAxisRange(190., 330., "Y");
		h2D->SetStats(0);
		h2D->GetXaxis()->SetTitle("PD-A (ADC channels)");
		h2D->GetXaxis()->SetLabelSize(0.05);
		h2D->GetXaxis()->SetTitleSize(0.05);
		h2D->GetXaxis()->CenterTitle();
		h2D->GetYaxis()->SetTitle("PD-B (ADC channels)");
		h2D->GetYaxis()->SetLabelSize(0.05);
		h2D->GetYaxis()->SetTitleSize(0.05);
		h2D->GetYaxis()->SetTitleOffset(1.4);
		h2D->GetYaxis()->CenterTitle();
		h2D->GetZaxis()->SetTitle("Counts");
		h2D->GetZaxis()->CenterTitle();
		raw2D->AddLast(h2D);
	
		sprintf(colname, "BAR%d_POS",  i); 
		hpos = new TH1F(colname, colname, 60, -30., 30.);
		hpos->GetXaxis()->SetTitle("Position (cm)");
		hpos->GetXaxis()->SetTitleSize(0.05);
		hpos->GetXaxis()->SetLabelSize(0.05);
		hpos->GetXaxis()->CenterTitle();
		hpos->GetYaxis()->SetTitle("Counts/cm");
		hpos->GetYaxis()->SetTitleSize(0.05);
//		hpos->GetYaxis()->SetTitleOffset(1.2);
		hpos->GetYaxis()->SetLabelSize(0.05);
		hpos->GetYaxis()->CenterTitle();
		rawPos->AddLast(hpos);
	
		sprintf(colname, "BAR%d_E",  i); 
		hE = new TH1F(colname, colname, 7500, 0., 150.);//2500 ->~20 keV/bin
		hE->GetXaxis()->SetTitle("Energy (MeV)");
		hE->GetXaxis()->SetTitleSize(0.05);
		hE->GetXaxis()->SetLabelSize(0.05);
		hE->GetXaxis()->CenterTitle();
		hE->GetYaxis()->SetTitle("Counts/s bin");
		hE->GetYaxis()->SetTitleSize(0.05);
//		hE->GetYaxis()->SetTitleOffset(1.2);
		hE->GetYaxis()->SetLabelSize(0.05);
		hE->GetYaxis()->CenterTitle();
		rawE->AddLast(hE);
	
		sprintf(colname, "BAR%d_CR",  i); 
		hCR = new TH1F(colname, colname, 2000, 0., 2000.); // to avoid information loss
		hCR->GetXaxis()->SetTitle("Time (s)");
		hCR->GetXaxis()->SetTitleSize(0.05);
		hCR->GetXaxis()->SetLabelSize(0.05);
		hCR->GetXaxis()->CenterTitle();
		hCR->GetYaxis()->SetTitle("Counts/s");
		hCR->GetYaxis()->SetTitleSize(0.05);
//		hCR->GetYaxis()->SetTitleOffset(1.2);
		hCR->GetYaxis()->SetLabelSize(0.05);
		hCR->GetYaxis()->CenterTitle();
		rawCRate->AddLast(hCR);
	
		sprintf(colname, "BAR%d_DT",  i); 
		hDT = new TH1F(colname, colname, 100000, 0, 100000);   // test distanza temporale eventi (bin 1 us)
		hDT->GetXaxis()->SetTitle("Delta t (us)");
		hDT->GetXaxis()->SetTitleSize(0.05);
		hDT->GetXaxis()->SetLabelSize(0.05);
		hDT->GetXaxis()->CenterTitle();
		hDT->GetYaxis()->SetTitle("Counts");
		hDT->GetYaxis()->SetTitleSize(0.05);
//		hDT->GetYaxis()->SetTitleOffset(1.2);
		hDT->GetYaxis()->SetLabelSize(0.05);
		hDT->GetYaxis()->CenterTitle();
		rawDTime->AddLast(hDT);
		
		sprintf(colname, "BAR%d_MULT",  i); 
		hMult = new TH1F(colname, colname, 31, 0, 31);   // events multiplicity
		hMult->GetXaxis()->SetTitle("Multiplicity");
		hMult->GetXaxis()->SetTitleSize(0.05);
		hMult->GetXaxis()->SetLabelSize(0.05);
		hMult->GetXaxis()->CenterTitle();
		hMult->GetYaxis()->SetTitle("Counts");
		hMult->GetYaxis()->SetTitleSize(0.05);
//		hMult->GetYaxis()->SetTitleOffset(1.2);
		hMult->GetYaxis()->SetLabelSize(0.05);
		hMult->GetYaxis()->CenterTitle();
		rawMult->AddLast(hMult);
		
		sprintf(colname, "BAR%d_CRDist",  i);  		// count rate distribution
		hCRDist  = new TH1F(colname, colname, 2000, 0., 2000.);  // count rate
		hCRDist->GetXaxis()->SetTitle("Count rate (Hz)");
		hCRDist->GetXaxis()->SetTitleSize(0.05);
		hCRDist->GetXaxis()->SetLabelSize(0.05);
		hCRDist->GetXaxis()->CenterTitle();
		hCRDist->GetYaxis()->SetTitle("Counts");
		hCRDist->GetYaxis()->SetTitleSize(0.05);
//		hCRDist->GetYaxis()->SetTitleOffset(1.2);
		hCRDist->GetYaxis()->SetLabelSize(0.05);
		hCRDist->GetYaxis()->CenterTitle();
		rawCRDist->AddLast(hCRDist);
	}
	rawHisto->AddLast(rawPDA);
	rawHisto->AddLast(rawPDB);
	rawHisto->AddLast(raw2D);
	rawHisto->AddLast(rawPos);
	rawHisto->AddLast(rawE);
	rawHisto->AddLast(rawCRate);
	rawHisto->AddLast(rawDTime);
	rawHisto->AddLast(rawMult);
	rawHisto->AddLast(rawCRDist);
	
	// extra histograms for global monitoring
	sciRMX = new TH1F("sciRMX", "sciRMX", 11, 1., 12.);	// creates scientific RM histo
	sciRMX->SetTitle("Scientific ratemeters");
	sciRMX->GetXaxis()->SetTitle("Ratemeter band");
	sciRMX->GetZaxis()->SetTitle("Counts");
	
	sciRMZ = new TH1F("sciRMZ", "sciRMZ", 11, 1., 12.);
	sciRMZ->SetTitle("Scientific ratemeters");
	sciRMZ->GetXaxis()->SetTitle("Ratemeter band");
	sciRMZ->GetZaxis()->SetTitle("Counts");
	
	hRMXR = new TH1F("hRMXR", "hRMXR", 11, 1., 12.);	// creates scientific RM histo
//	hRMXR->SetTitle("Scientific ratemeters");
//	hRMXR->GetXaxis()->SetTitle("Ratemeter band");
//	hRMXR->GetZaxis()->SetTitle("Counts");
	
	hRMZR = new TH1F("hRMZR", "hRMZR", 11, 1., 12.);
//	hRMZR->SetTitle("Scientific ratemeters");
//	hRMZR->GetXaxis()->SetTitle("Ratemeter band");
//	hRMZR->GetZaxis()->SetTitle("Counts");
	
	gRMX = new TGraphErrors(20);
	gRMX->SetName("gRMX");
	gRMX->SetTitle("Sci. RM energy spectrum on X plane");
	gRMX->GetXaxis()->SetTitle("Energy (MeV)");
	gRMX->GetXaxis()->SetTitleOffset(1.4);
	gRMX->GetXaxis()->CenterTitle();
	gRMX->GetYaxis()->SetTitle("Counts/s");
	gRMX->GetYaxis()->CenterTitle();
	gRMX->GetYaxis()->SetTitleOffset(1.5);
	
	gRMZ = new TGraphErrors(20);
	gRMZ->SetName("gRMZ");
	gRMZ->SetTitle("Sci. RM energy spectrum on Z plane");
	gRMZ->GetXaxis()->SetTitle("Energy (MeV)");
	gRMZ->GetXaxis()->SetTitleOffset(1.4);
	gRMZ->GetXaxis()->CenterTitle();
	gRMZ->GetYaxis()->SetTitle("Counts/s");
	gRMZ->GetYaxis()->CenterTitle();
	gRMZ->GetYaxis()->SetTitleOffset(1.5);
	
	gRMX3913 = new TGraphErrors(20);
	gRMX3913->SetName("gRMX3913");
	
	gRMZ3913 = new TGraphErrors(20);
	gRMZ3913->SetName("gRMZ3913");
	
	gRMXR = new TGraphErrors(20);
	gRMXR->SetName("gRMXR");
	
	gRMZR = new TGraphErrors(20);
	gRMZR->SetName("gRMZR");
	
	totDT  = new TH1F("totDT", "totDT", 100000, 0, 100000);
	totDT->SetTitle("Time difference between successive events");
	totDT->GetXaxis()->SetTitle("Delta time (us)");
	totDT->GetYaxis()->SetTitle("Counts");
	totDT->SetAxisRange(0., 100., "X");
	
	barcorr = new TH2F("barcorr", "barcorr", 30, 0., 30., 30, 0., 30.);
	barcorr->SetTitle("bar correlation");
	barcorr->GetXaxis()->SetTitle("Bar[i] address");
	barcorr->GetXaxis()->SetTitleSize(0.05);
	barcorr->GetXaxis()->CenterTitle();
	barcorr->GetYaxis()->SetTitle("Bar[i+1] address");
	barcorr->GetYaxis()->SetTitleSize(0.05);
	barcorr->GetYaxis()->CenterTitle();
	barcorr->SetStats(0);
	
	barcorrLowDT = new TH2F("barcorrLowDT", "barcorrLowDT", 30, 0., 30., 30, 0., 30.);
	barcorrLowDT->SetTitle("bar correlation for delta_t < 10 us");
	barcorrLowDT->GetXaxis()->SetTitle("Bar[i] address");
	barcorrLowDT->GetXaxis()->SetTitleSize(0.05);
	barcorrLowDT->GetXaxis()->CenterTitle();
	barcorrLowDT->GetYaxis()->SetTitle("Bar[i+1] address");
	barcorrLowDT->GetYaxis()->SetTitleSize(0.05);
	barcorrLowDT->GetYaxis()->CenterTitle();
	barcorrLowDT->SetStats(0);
	
	totCR  = new TH1F("totCR", "totCR", 10000, 0., 10000.);
	totCR->SetTitle("MCAL count rate");
	totCR->GetXaxis()->SetTitle("Time (s)");
	totCR->GetXaxis()->SetTitleSize(0.05);
	totCR->GetXaxis()->CenterTitle();
	totCR->GetYaxis()->SetTitle("Counts/s (Hz)");
	totCR->GetYaxis()->SetTitleSize(0.05);
	totCR->GetYaxis()->CenterTitle();
	totCR->SetStats(0);
	
	totMult  = new TH1F("totMult", "totMult", 31, 0, 31);
	totMult->SetTitle("MCAL multiplicity distribution");
	totMult->GetXaxis()->SetTitle("Number of bars");
	totMult->GetXaxis()->SetTitleSize(0.05);
	totMult->GetXaxis()->CenterTitle();
	totMult->GetYaxis()->SetTitle("Counts");
	totMult->GetYaxis()->SetTitleSize(0.05);
	totMult->GetYaxis()->CenterTitle();
	totMult->SetStats(1);
	
	totE  = new TH1F("totE", "totE", 100000, 0., 2000.);
	totE->SetTitle("MCAL energy spectrum");
	totE->GetXaxis()->SetTitle("Energy (MeV)");
	totE->GetXaxis()->SetTitleSize(0.05);
	totE->GetXaxis()->CenterTitle();
	totE->GetYaxis()->SetTitle("Counts/s bin");
	totE->GetYaxis()->SetTitleSize(0.05);
	totE->GetYaxis()->CenterTitle();
	totE->SetStats(1);
	
	hX = new TH2F("hX", "X plane events distribution", 15, 0., 15., 60, -30., 30.);
	hX->SetStats(0);
	hX->SetTitle("Events distribution on X plane");
	hX->GetXaxis()->SetTitle("Bar number");
	hX->GetXaxis()->SetTitleSize(0.05);
	hX->GetZaxis()->SetTitle("Counts/s cm");
	hX->GetZaxis()->CenterTitle();
	hX->GetZaxis()->SetTitleSize(0.05);
	hX->GetZaxis()->SetLabelSize(0.05);
	hX->GetXaxis()->CenterTitle();
	hX->GetYaxis()->SetTitle("Position (cm)");
	hX->GetYaxis()->SetTitleSize(0.05);
	hX->GetYaxis()->CenterTitle();
	
	hZ = new TH2F("hZ", "Z plane events distribution", 60, -30., 30., 15, 0., 15.);
	hZ->SetStats(0);
	hZ->SetTitle("Events distribution on Z plane");
	hZ->GetXaxis()->SetTitle("Position (cm)");
	hZ->GetXaxis()->SetTitleSize(0.05);
	hZ->GetZaxis()->SetTitle("Counts/s cm");
	hZ->GetZaxis()->CenterTitle();
	hZ->GetZaxis()->SetTitleSize(0.05);
	hZ->GetZaxis()->SetLabelSize(0.05);
	hZ->GetXaxis()->CenterTitle();
	hZ->GetYaxis()->SetTitle("Bar number");
	hZ->GetYaxis()->SetTitleSize(0.05);
	hZ->GetYaxis()->CenterTitle();
	
	gCRX = new TGraphErrors(15);
	gCRX->SetName("gCRX");
	gCRX->SetTitle("Count rate X plane");
	gCRX->SetMarkerStyle(21);
	gCRX->GetXaxis()->SetTitle("Bar number");
	gCRX->GetXaxis()->SetTitleSize(0.05);
	gCRX->GetXaxis()->CenterTitle();
	gCRX->GetYaxis()->SetTitle("Count rate (Hz)");
	gCRX->GetYaxis()->CenterTitle();
	gCRX->GetYaxis()->SetTitleSize(0.05);
	
	gCRZ = new TGraphErrors(15);
	gCRZ->SetName("gCRZ");
	gCRZ->SetTitle("Count rate Z plane");
	gCRZ->SetMarkerStyle(21);
	gCRZ->GetXaxis()->SetTitle("Bar number");
	gCRZ->GetXaxis()->SetTitleSize(0.05);
	gCRZ->GetXaxis()->CenterTitle();
	gCRZ->GetYaxis()->SetTitle("Count rate (Hz)");
	gCRZ->GetYaxis()->CenterTitle();
	gCRZ->GetYaxis()->SetTitleSize(0.05);
	
	gMultX = new TGraphErrors(15);
	gMultX->SetName("gMultX");
	gMultX->SetTitle("Multiple-to-total events ratio on X plane");
	gMultX->SetMarkerStyle(21);
	gMultX->GetXaxis()->SetTitle("Bar number");
	gMultX->GetXaxis()->CenterTitle();
	
	gMultZ = new TGraphErrors(15);
	gMultZ->SetName("gMultZ");
	gMultZ->SetTitle("Multiple-to-total events ratio on Z plane");
	gMultZ->SetMarkerStyle(21);
	gMultZ->GetXaxis()->SetTitle("Bar number");
	gMultZ->GetXaxis()->CenterTitle();
	
	for (int i=0; i<11; i++) {
		sprintf(colname, "Sci_RM_X_%d", i+1); 
		gaux = new TGraph(10);
		gaux->SetName(colname);
		gaux->SetTitle(colname);
		gaux->GetXaxis()->SetTitle("Time (s)");
//		gaux->GetXaxis()->SetTitleOffset(1.4);
		gaux->GetXaxis()->CenterTitle();
		gaux->GetYaxis()->SetTitle("Counts/s");
		gaux->GetYaxis()->CenterTitle();
//		gaux->GetYaxis()->SetTitleOffset(1.5);
		rawSciRM3913->AddLast(gaux);
	}
	
	for (int i=0; i<11; i++) {
		sprintf(colname, "Sci_RM_Z_%d", i+1); 
		gaux = new TGraph(10);
		gaux->SetName(colname);
		gaux->SetTitle(colname);
		gaux->GetXaxis()->SetTitle("Time (s)");
//		gaux->GetXaxis()->SetTitleOffset(1.4);
		gaux->GetXaxis()->CenterTitle();
		gaux->GetYaxis()->SetTitle("Counts/s");
		gaux->GetYaxis()->CenterTitle();
//		gaux->GetYaxis()->SetTitleOffset(1.5);
		rawSciRM3913->AddLast(gaux);
	}	
	
	for (int i=0; i<5; i++) {
		sprintf(colname, "AC_RM_%d", i); 
		gaux = new TGraph(10);
		gaux->SetName(colname);
		gaux->SetTitle(colname);
		gaux->GetXaxis()->SetTitle("Time (s)");
//		gaux->GetXaxis()->SetTitleOffset(1.4);
		gaux->GetXaxis()->CenterTitle();
		gaux->GetYaxis()->SetTitle("Counts/s");
		gaux->GetYaxis()->CenterTitle();
//		gaux->GetYaxis()->SetTitleOffset(1.5);
		rawACRM3913->AddLast(gaux);
	}	
	
	rawHisto->AddLast(sciRMX);
	rawHisto->AddLast(sciRMZ);
	rawHisto->AddLast(totDT);
	rawHisto->AddLast(barcorr);
	rawHisto->AddLast(barcorrLowDT);
	rawHisto->AddLast(totCR);
	rawHisto->AddLast(totMult);
	rawHisto->AddLast(totE);
	rawHisto->AddLast(hX);
	rawHisto->AddLast(hZ);
	rawHisto->AddLast(gCRX);
	rawHisto->AddLast(gCRZ);
	rawHisto->AddLast(gMultX);
	rawHisto->AddLast(gMultZ);
	rawHisto->AddLast(gRMX);
	rawHisto->AddLast(gRMZ);
	rawHisto->AddLast(gRMX3913);
	rawHisto->AddLast(gRMZ3913);
	rawHisto->AddLast(rawSciRM3913);
	rawHisto->AddLast(hRMXR);
	rawHisto->AddLast(hRMZR);
	rawHisto->AddLast(gRMXR);
	rawHisto->AddLast(gRMZR);
	rawHisto->AddLast(rawACRM3913);
	
	InitSciRMEnergyBand();
	
	hSubMS  = new TH1F("hSubMS", "Sub-ms trigger distribution", 10, 1., 11.);
	rawHisto->AddLast(hSubMS);
}

//
//--------------------------------------------------------------------
//

MCVHisto::MCVHisto(TObjArray *inh)
{
	// initialize the class with a previously built histogram collection
	rawHisto = inh;
	rawPDA =(TObjArray *) rawHisto->FindObject("rawPDA");
	rawPDB =(TObjArray *) rawHisto->FindObject("rawPDB");
	raw2D  =(TObjArray *) rawHisto->FindObject("raw2D");
	rawPos =(TObjArray *) rawHisto->FindObject("rawPos");
	rawE   =(TObjArray *) rawHisto->FindObject("rawE");
	rawCRate =(TObjArray *) rawHisto->FindObject("rawCRate");
	rawDTime =(TObjArray *) rawHisto->FindObject("rawDTime");
	rawMult =(TObjArray *) rawHisto->FindObject("rawMult");
	rawCRDist =(TObjArray *) rawHisto->FindObject("rawCRDist");
	
	sciRMX =(TH1F *) rawHisto->FindObject("sciRMX");
	sciRMZ =(TH1F *) rawHisto->FindObject("sciRMZ");
	hRMXR =(TH1F *) rawHisto->FindObject("hRMXR");
	hRMZR =(TH1F *) rawHisto->FindObject("hRMZR");
	totDT =(TH1F *) rawHisto->FindObject("totDT");
	totCR =(TH1F *) rawHisto->FindObject("totCR");
	totMult =(TH1F *) rawHisto->FindObject("totMult");
	totE =(TH1F *) rawHisto->FindObject("totE");
	
	barcorr =(TH2F *) rawHisto->FindObject("barcorr");
	barcorrLowDT =(TH2F *) rawHisto->FindObject("barcorrLowDT");
	hX =(TH2F *) rawHisto->FindObject("hX");
	hZ =(TH2F *) rawHisto->FindObject("hZ");
	
	gRMX =(TGraphErrors *) rawHisto->FindObject("gRMX");
	gRMZ =(TGraphErrors *) rawHisto->FindObject("gRMZ");
	gRMX3913 =(TGraphErrors *) rawHisto->FindObject("gRMX3913");
	gRMZ3913 =(TGraphErrors *) rawHisto->FindObject("gRMZ3913");
	gRMXR =(TGraphErrors *) rawHisto->FindObject("gRMXR");
	gRMZR =(TGraphErrors *) rawHisto->FindObject("gRMZR");
	gCRX =(TGraphErrors *) rawHisto->FindObject("gCRX");
	gCRZ =(TGraphErrors *) rawHisto->FindObject("gCRZ");
	gMultX =(TGraphErrors *) rawHisto->FindObject("gMultX");
	gMultZ =(TGraphErrors *) rawHisto->FindObject("gMultZ");
	
	InitSciRMEnergyBand();
}

//
//--------------------------------------------------------------------
//

void MCVHisto::InitSciRMEnergyBand()
{
	// init sci. RM default energy bands
	Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
	Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};
	for (int i=0; i<11; i++) {
		Eband[i]=tmpEband[i];
		Eerr[i]=tmpEerr[i];
		EbandUL[i]=Eband[i] + Eerr[i];
	}
	
	for (int i=0; i<30; i++) prtime[i]=-1.E+6;
	prt = 0.;
	prmult = 0;
	nlowDT = 0;
	gf = 1.;
}

//
//--------------------------------------------------------------------
//

MCVHisto::~MCVHisto()
{

}

//
//--------------------------------------------------------------------
//

void MCVHisto::FillHisto(MCALevt *evt)
{
	int i=0;
	int m=0;
	int id, mult, cbar, limsup, irm;
	Double_t ti, dt;
	double eX=0., eZ=0.;
	
	mult = evt->mult;
	ti = evt->time;
	do {
		if (evt->bar[i] >= 0) {
			id = evt->bar[i];
			cbar = i;
			
			// fill individual bar histograms
			((TH1F *) rawCRate->At(i))->Fill(ti);
			((TH1F *) rawPDA->At(i))->Fill(evt->pdA[id]);
			((TH1F *) rawPDB->At(i))->Fill(evt->pdB[id]);
			((TH2F *) raw2D->At(i))->Fill(evt->pdA[id], evt->pdB[id]);
			((TH1F *) rawPos->At(i))->Fill(evt->pos[id]);
			if (evt->E[id] > 0.) ((TH1F *) rawE->At(i))->Fill(evt->E[id]);
			((TH1F *) rawMult->At(i))->Fill(mult);		
			((TH1F *) rawDTime->At(i))->Fill((ti - prtime[i])*1.E6);
			//if (ti - prtime[i] <= 1) printf("DT <= 1 on bar %d at time %.0f\n", i, ti);
			prtime[i] = ti;
			m++;
			
			if (i<15) eX += evt->E[id];
			else eZ += evt->E[id];
		}
		i++;
	} while (m<mult && i<30);
	
	// fill global monitoring histograms
	if (evt->Etot > 0.) totE->Fill(evt->Etot);
	totMult->Fill(mult);
	totCR->Fill(evt->time);
	dt = (ti - prt)*1.E6;
	if (dt<0) dt = 1.E8;
	totDT->Fill(dt);
	
	// update calculated scientific ratemeters histograms
	if (evt->totEX>0) {		// update plane X scientific RM
		if (gf != 1.) evt->totEX = (Int_t) (evt->totEX / gf);
		limsup = 8;
		irm=1;
		for (int k=0; k<12; k++) {
			if (evt->totEX < limsup) {
				sciRMX->Fill(irm);
				break;
			}
			if (irm < 11) irm++;
			limsup *=2;
		}
	}
	
	if (evt->totEZ>0) {		// update plane Z scientific RM
		if (gf != 1.) evt->totEZ = (Int_t) (evt->totEZ / gf);
		limsup = 8;
		irm=1;
		for (int k=0; k<12; k++) {
			if (evt->totEZ < limsup) {
				sciRMZ->Fill(irm);
				break;
			}
			if (irm < 11) irm++;
			limsup *=2;
		}
	}
	
	// update bar correlation histograms
	if (mult==1 && prmult==1) {	// consider only consecutive single events
		barcorr->Fill(prbar, cbar);
		if (dt<=10.) {
			barcorrLowDT->Fill(cbar, prbar);
			if (cbar==prbar) { // anomalous consecutive events on the same bar
				nlowDT++;
				if (nlowDT==1) lowDTti = prt*1.E-6;
				lowDTtf = ti*1.E-6;
			}
		}
	}
	
	// update reconstructed energy scientific ratemeters
	if (eX > 0.) {
		i=0;
		while (eX>EbandUL[i]) i++;
		hRMXR->Fill(i+1);
	}
	if (eZ > 0.) {
		i=0;
		while (eZ>EbandUL[i]) i++;
		hRMZR->Fill(i+1);
	}

	prmult = mult;
	prbar = cbar;
	prt = ti;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVHisto::ResizeCountRateHisto(int time)
{
	// resize count rate histograms according to measure duration
	for (int i=0; i<30; i++) ((TH1F *) rawCRate->At(i))->SetBins(time+1800, 0., time+1800);
	totCR->SetBins(time+1800, 0., time+1800);
}

//
//--------------------------------------------------------------------
//

void MCVHisto::FillCountRateDistribution()
{
	// filling count rate distribution histogram
	TH1F *hCRDist;
	TH1F *hCRate;
	int minCR, maxCR, nbins;
	long  cnts;
	
	for (int i=0; i<30; i++) {
		hCRate  = (TH1F *) rawCRate->At(i);
		hCRDist = (TH1F *) rawCRDist->At(i);
		minCR = (int) 0.75*(hCRate)->GetMinimum();
		maxCR = (int) 1.25*(hCRate)->GetMaximum();
		hCRDist->SetBins(maxCR-minCR, minCR, maxCR);
		nbins = hCRate->GetNbinsX();
		if (hCRate->GetEntries()) {
			for (int k=1; k<=nbins; k++) {
				cnts = hCRate->GetBinContent(k);
				if (cnts) hCRDist->Fill(cnts);
			}
		}
	}
	return;	
}

//
//--------------------------------------------------------------------
//

void MCVHisto::FillGlobalHisto(int pkttype, Double_t tottime)
{
	// filling global histograms (after all events have been processed)
	TH1F *aux;

	// get count rate on X plane
	
/*	// old approach: distribution mean value
	if (pkttype == 3901) {
		for (int i=0; i<15; i++) {
			aux = (TH1F *) rawPDA->At(i);	
			if (aux->GetEntries()) {
				gCRX->SetPoint(i, i+1, aux->GetEntries()/tottime);
				gCRX->SetPointError(i, 0, sqrt(aux->GetEntries())/tottime);
			} 
		}	
	} else {
		for (int i=0; i<15; i++) {
			aux = (TH1F *) rawCRDist->At(i);	
			if (aux->GetEntries()) {
				gCRX->SetPoint(i, i+1, aux->GetMean());
				gCRX->SetPointError(i, 0, aux->GetRMS());
			} 
		}
	}*/
	
	// new approach: nevents/teff
	for (int i=0; i<15; i++) {
		aux = (TH1F *) rawPDA->At(i);	
		if (aux->GetEntries()) {
		//	double te = GetEffectiveTime(i);
		//	gCRX->SetPoint(i, i+1, aux->GetEntries()/te);
		//	gCRX->SetPointError(i, 0, sqrt(aux->GetEntries())/te);
			
			gCRX->SetPoint(i, i+1, aux->GetEntries()/tottime);
			gCRX->SetPointError(i, 0, sqrt(aux->GetEntries())/tottime);
		} 
	}	
	
	gCRX->GetXaxis()->SetTitle("Bar number");
	gCRX->GetXaxis()->SetTitleSize(0.05);
	gCRX->GetXaxis()->SetLabelSize(0.05);
	gCRX->GetXaxis()->CenterTitle();
	gCRX->GetYaxis()->SetTitle("Count rate (Hz)");
	gCRX->GetYaxis()->CenterTitle();
	gCRX->GetYaxis()->SetTitleSize(0.05);
	gCRX->GetYaxis()->SetLabelSize(0.05);

	// get count rate on Z plane 
/*	if (pkttype == 3901) {
		for (int i=0; i<15; i++) {
			aux = (TH1F *) rawPDA->At(i+15);	
			if (aux->GetEntries()) {
				gCRZ->SetPoint(i, i+1, aux->GetEntries()/tottime);
				gCRZ->SetPointError(i, 0, sqrt(aux->GetEntries())/tottime);
			} 
		}		
	} else {
		for (int i=0; i<15; i++) {
			aux = (TH1F *) rawCRDist->At(i+15);	
			if (aux->GetEntries()) {
				gCRZ->SetPoint(i, i+1, aux->GetMean());
				gCRZ->SetPointError(i, 0, aux->GetRMS());
			}
		}
	}*/
	
	// new approach: nevents/teff
	for (int i=0; i<15; i++) {
		aux = (TH1F *) rawPDA->At(i+15);	
		if (aux->GetEntries()) {
		//	double te = GetEffectiveTime(i);
		//	gCRZ->SetPoint(i, i+1, aux->GetEntries()/te);
		//	gCRZ->SetPointError(i, 0, sqrt(aux->GetEntries())/te);
			
			gCRZ->SetPoint(i, i+1, aux->GetEntries()/tottime);
			gCRZ->SetPointError(i, 0, sqrt(aux->GetEntries())/tottime);
		} 
	}		
	
	gCRZ->GetXaxis()->SetTitle("Bar number");
	gCRZ->GetXaxis()->SetLabelSize(0.05);
	gCRZ->GetXaxis()->SetTitleSize(0.05);
	gCRZ->GetXaxis()->CenterTitle();
	gCRZ->GetYaxis()->SetTitle("Count rate (Hz)");
	gCRZ->GetYaxis()->CenterTitle();
	gCRZ->GetYaxis()->SetTitleSize(0.05);
	gCRZ->GetYaxis()->SetLabelSize(0.05);
		
	// get multiple-to-total events ratio on X plane (BURST DATA)
	for (int i=0; i<15; i++) {
		aux = (TH1F *) rawMult->At(i);		
		if (aux->GetEntries()) {
			Double_t tmp = aux->Integral(3,31) / aux->Integral();
			gMultX->SetPoint(i, i+1, tmp);
			gMultX->SetPointError(i, 0, (1./sqrt(aux->Integral(3,31)) +1./sqrt(aux->Integral()))*tmp);
		}
	}
	gMultX->GetXaxis()->SetTitle("Bar number");
	gMultX->GetXaxis()->CenterTitle();
	gMultX->GetXaxis()->SetLabelSize(0.05);
	gMultX->GetYaxis()->SetLabelSize(0.05);
	
	// get multiple-to-total events ratio on Z plane (BURST DATA)
	for (int i=0; i<15; i++) {
		aux = (TH1F *) rawMult->At(i+15);	
		if (aux->GetEntries()) {
			Double_t tmp = aux->Integral(3,31) / aux->Integral();
			gMultZ->SetPoint(i, i+1, tmp);
			gMultZ->SetPointError(i, 0, (1./sqrt(aux->Integral(3,31)) +1./sqrt(aux->Integral()))*tmp);
		} 
	}
	gMultZ->GetXaxis()->SetTitle("Bar number");
	gMultZ->GetXaxis()->CenterTitle();
	gMultZ->GetXaxis()->SetLabelSize(0.05);
	gMultZ->GetYaxis()->SetLabelSize(0.05);
		
	// fill position distribution on X and Z plane (BURST DATA)
	double taux;
	for (int i=0; i<15; i++) {
		aux = (TH1F *) rawPos->At(i);
	//	taux = GetEffectiveTime(i);	
		taux = tottime;	
		
		if (aux->GetEntries()) {
			for (int j=1; j<=60; j++) hX->SetBinContent(i+1, 60-j, aux->GetBinContent(j)/taux); // the "60-j" term is to conform to the Laben reference frame
		}
		
		aux = (TH1F *) rawPos->At(i+15);
	//	taux = GetEffectiveTime(i+15);	
		if (aux->GetEntries()) {
			for (int j=1; j<=60; j++) hZ->SetBinContent(60-j, i+1, aux->GetBinContent(j)/taux);
		}		
	}
//	hX->Scale(1./tottime);
//	hZ->Scale(1./tottime);
	
	// create normalized scientific ratemeters spectra
	Float_t cntX, cntZ, cntXerr, cntZerr;
//	Double_t tottime = GetEffectiveTime(0);
	for (int i=1; i<11; i++) {
		cntX = sciRMX->GetBinContent(i+1);
		cntXerr = sqrt(cntX);
		cntX /= tottime;
		cntXerr /= tottime;
		cntZ = sciRMZ->GetBinContent(i+1);
		cntZerr = sqrt(cntZ);
		cntZ /= tottime;
		cntZerr /= tottime;	
		gRMX->SetPoint(i-1, Eband[i], cntX);
		gRMX->SetPointError(i-1, Eerr[i], cntXerr);
		gRMZ->SetPoint(i-1, Eband[i], cntZ);
		gRMZ->SetPointError(i-1, Eerr[i], cntZerr);
	}
		
	// Scaling total energy histogram by effective time
	totE->Sumw2();
	totE->Scale(1./tottime);
		
	// Scaling calculated sci. RM histograms by effective time
	//totE->Sumw2();
	//totE->Scale(1./tottime);
	
	// normalize correlation histogram
	// work on barcorr
/*	float acorr;
	double *crx = gCRX->GetY();
	double *crz = gCRZ->GetY();
	double mcr[30][30];
	for (int i=0; i<30; i++) {
		for (int j=0; j<30; j++) {
			if (i<15 && j<15) mcr[i][j] = tottime*(crx[i]*crx[j]);
			else if (i<15 && j>=15) mcr[i][j] = tottime*(crx[i]*crz[j-15]);
			else if (i>=15 && j<15) mcr[i][j] = tottime*(crz[i-15]*crx[j]);
			else mcr[i][j] = tottime*(crz[i-15]*crz[j-15]);
		}
	}
	
	
	for (int i=0; i<30; i++) {
		for (int j=0; j<30; j++) {
			acorr = barcorr->GetBinContent(i+1,j+1);
			barcorr->SetBinContent(i+1,j+1, acorr/mcr[i][j]);
		}
	}*/
	
	// simple normalization
	long int nevt = barcorr->GetEntries();
	barcorr->Scale(1./nevt);
	nevt = barcorrLowDT->GetEntries();
	barcorrLowDT->Scale(1./nevt);
	
	// normalize Energy histograms
	for (int i=0; i<30; i++) {
		((TH1F *) rawE->At(i))->Sumw2();
	//	((TH1F *) rawE->At(i))->Scale(1./GetEffectiveTime(i));
		((TH1F *) rawE->At(i))->Scale(1./tottime);
	}
}

//
// --------------------------------------------------
//

Double_t MCVHisto::GetEffectiveTime(Int_t ibar)
{
	// Returns effective time calculated on bar ibar
	Double_t efft=0.;
	TH1F *aux = (TH1F *) rawCRate->At(ibar);
	if (aux->GetEntries()) {
		int nbins = aux->GetNbinsX();
		for (int i=1; i<nbins; i++) if (aux->GetBinContent(i) > 0) efft += 1.;
	} else efft = -1.;
	return efft;
}

//
// --------------------------------------------------
//

Double_t MCVHisto::GetEffectiveTime()
{
	// Returns effective time calculated on global CR histo
	Double_t efft=0.;
	if (totCR->GetEntries()) {
		int nbins = totCR->GetNbinsX();
		for (int i=1; i<nbins; i++) if (totCR->GetBinContent(i) > 0) efft += 1.;
	} else efft = -1.;
	return efft;
}
