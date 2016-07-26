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

#include "hkviewer.h"
#include "version.h"
using namespace std;

TCanvas *hkTrgOutput(TFile *fh, TTimeStamp epoch)
{
	// --------------- draws BURST search triggers ----------------
	
	int npt;
	double tini=-1., tend=-1.;
	TTimeStamp ts;
	
	TText tt;
	TString str("MCAL Team - IASF-Bo - hkviewer version ");
	str += VERSION;
	
	TText tt1;
	TString str1("T start: ");
	
	TCanvas *c1 = new TCanvas("hkview_BURST_trg", "HK BURST trg monitor", 1000, 750);
	c1->Divide(1, 3);
	c1->cd(1);
	
	// plot partial triggers
	c1->cd(1);
	gPad->SetLogy();
	TLegend *leg1 = new TLegend(0.75, 0.45, 0.95, 0.95);
	double *y;
	double ymax=1., auxmax;
	
	TGraph	*aux1 = NULL;
	fh->GetObject("M6329_0_PDHU_SA_MCAL_partial_BURST_Tri9", aux1);	// partial trg X1 LE		
	if (aux1) {
		npt = aux1->GetN();
		double *x = aux1->GetX();
		tini = x[0];
		tend = x[npt-1];
		//ts.Set((UInt_t) x[0] + epoch.Convert());
		ts.Set((int) x[0] , 0, epoch.GetSec(),0);
		str1 += ts.AsString();
	
		aux1->GetXaxis()->SetTitle("Time");
		aux1->GetXaxis()->SetTimeDisplay(1);
		aux1->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
		aux1->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		aux1->SetLineColor(1);
		aux1->GetXaxis()->SetRangeUser(tini, tend);
		aux1->GetYaxis()->SetRangeUser(0.1, 1000.);
		aux1->GetXaxis()->SetTitleOffset(0.8);
		aux1->GetXaxis()->SetTitleSize(0.06);
		aux1->GetXaxis()->CenterTitle();				
		aux1->GetYaxis()->SetTitle("Counts");
		aux1->GetYaxis()->CenterTitle();
		aux1->GetYaxis()->SetTitleOffset(0.8);
		aux1->GetYaxis()->SetTitleSize(0.06);
		aux1->SetTitle("MCAL partial BURST triggers");
		aux1->Draw("ALP");
		tt.DrawTextNDC(0.6, 0.95, str.Data());
		tt1.DrawTextNDC(0.2, 0.95, str1.Data());
		leg1->AddEntry(aux1, "X1-X8 Low Energy", "lp");
		auxmax = TMath::MaxElement(aux1->GetN(), aux1->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux2 = NULL;
	fh->GetObject("M6330_0_PDHU_SA_MCAL_partial_BURST_Tri10", aux2);	// partial trg X2 LE		
	if (aux2) {
		aux2->SetLineColor(2);
		aux2->Draw("LPSAME");
		leg1->AddEntry(aux2, "X9-X15 Low Energy", "lp");
		auxmax = TMath::MaxElement(aux2->GetN(), aux2->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux3 = NULL;
	fh->GetObject("M6331_0_PDHU_SA_MCAL_partial_BURST_Tri11", aux3);	// partial trg Z1 LE		
	if (aux3) {
		aux3->SetLineColor(3);
		aux3->Draw("LPSAME");
		leg1->AddEntry(aux3, "Z1-Z8 Low Energy", "lp");
		auxmax = TMath::MaxElement(aux3->GetN(), aux3->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux4 = NULL;
	fh->GetObject("M6332_0_PDHU_SA_MCAL_partial_BURST_Tri12", aux4);	// partial trg Z2 LE		
	if (aux4) {
		aux4->SetLineColor(4);
		aux4->Draw("LPSAME");
		leg1->AddEntry(aux4, "Z9-Z15 Low Energy", "lp");
		auxmax = TMath::MaxElement(aux4->GetN(), aux4->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux5 = NULL;
	fh->GetObject("M6333_0_PDHU_SA_MCAL_partial_BURST_Tri13", aux5);	// partial trg X1 ME		
	if (aux5) {
		aux5->SetLineColor(5);
		aux5->Draw("LPSAME");
		leg1->AddEntry(aux5, "X1-X8 Medium Energy", "lp");
		auxmax = TMath::MaxElement(aux5->GetN(), aux5->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux6 = NULL;
	fh->GetObject("M6334_0_PDHU_SA_MCAL_partial_BURST_Tri14", aux6);	// partial trg X2 ME		
	if (aux6) {
		aux6->SetLineColor(6);
		aux6->Draw("LPSAME");
		leg1->AddEntry(aux6, "X9-X15 Medium Energy", "lp");
		auxmax = TMath::MaxElement(aux6->GetN(), aux6->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux7 = NULL;
	fh->GetObject("M6335_0_PDHU_SA_MCAL_partial_BURST_Tri15", aux7);	// partial trg Z1 ME		
	if (aux7) {
		aux7->SetLineColor(7);
		aux7->Draw("LPSAME");
		leg1->AddEntry(aux7, "Z1-Z8 Medium Energy", "lp");
		auxmax = TMath::MaxElement(aux7->GetN(), aux7->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux8 = NULL;
	fh->GetObject("M6336_0_PDHU_SA_MCAL_partial_BURST_Tri16", aux8);	// partial trg Z2 ME		
	if (aux8) {
		aux8->SetLineColor(8);
		aux8->Draw("LPSAME");
		leg1->AddEntry(aux8, "Z9-Z15 Medium Energy", "lp");
		auxmax = TMath::MaxElement(aux8->GetN(), aux8->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*aux9 = NULL;
	fh->GetObject("M6337_0_PDHU_SA_MCAL_partial_BURST_Tri17", aux9);	// partial trg HE		
	if (aux9) {
		aux9->SetLineColor(9);
		aux9->Draw("LPSAME");
		leg1->AddEntry(aux9, "MCAL High Energy", "lp");
		auxmax = TMath::MaxElement(aux9->GetN(), aux9->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	leg1->Draw();
	
	// plot rejected triggers
	c1->cd(2);
	gPad->SetLogy();
	TLegend *leg2 = new TLegend(0.75, 0.65, 0.95, 0.95);
	ymax=1.;
	
	TGraph	*auxr1 = NULL;
	fh->GetObject("M6345_0_Pdhu_Trig_Rej_BURST_Trig_Log5", auxr1);	// rejected trg 64 ms		
	if (auxr1) {
		auxr1->GetXaxis()->SetTitle("Time");
		auxr1->GetXaxis()->SetTimeDisplay(1);
		auxr1->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
		auxr1->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		auxr1->SetLineColor(1);
		auxr1->GetXaxis()->SetRangeUser(tini, tend);
		auxr1->GetYaxis()->SetRangeUser(0.1, 1000.);
		auxr1->GetXaxis()->SetTitleOffset(0.8);
		auxr1->GetXaxis()->SetTitleSize(0.06);
		auxr1->GetXaxis()->CenterTitle();				
		auxr1->GetYaxis()->SetTitle("Counts");
		auxr1->GetYaxis()->CenterTitle();
		auxr1->GetYaxis()->SetTitleOffset(0.8);
		auxr1->GetYaxis()->SetTitleSize(0.06);
		auxr1->SetTitle("MCAL rejected BURST triggers");
		auxr1->Draw("ALP");
		leg2->AddEntry(auxr1, "SIT   64 ms", "lp");
		auxmax = TMath::MaxElement(auxr1->GetN(), auxr1->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*auxr2 = NULL;
	fh->GetObject("M6346_0_Pdhu_Trig_Rej_BURST_Trig_Log6", auxr2);	// rejected trg 256 ms
	if (auxr2) {
		auxr2->SetLineColor(2);
		auxr2->Draw("LPSAME");
		leg2->AddEntry(auxr2, "SIT  256 ms", "lp");
		auxmax = TMath::MaxElement(auxr2->GetN(), auxr2->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*auxr3 = NULL;
	fh->GetObject("M6347_0_Pdhu_Trig_Rej_BURST_Trig_Log7", auxr3);	// rejected trg 1024 ms
	if (auxr3) {
		auxr3->SetLineColor(3);
		auxr3->Draw("LPSAME");
		leg2->AddEntry(auxr3, "SIT 1024 ms", "lp");
		auxmax = TMath::MaxElement(auxr3->GetN(), auxr3->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*auxr4 = NULL;
	fh->GetObject("M6348_0_Pdhu_Trig_Rej_BURST_Trig_Log8", auxr4);	// rejected trg 8192 ms
	if (auxr4) {
		auxr4->SetLineColor(4);
		auxr4->Draw("LPSAME");
		leg2->AddEntry(auxr4, "SIT 8192 ms", "lp");
		auxmax = TMath::MaxElement(auxr4->GetN(), auxr4->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	leg2->Draw();

	// plot BBFP rejected triggers
	c1->cd(3);
	gPad->SetLogy();
	TLegend *leg3 = new TLegend(0.75, 0.65, 0.95, 0.95);
	ymax=1.;
	
	TGraph	*auxb1 = NULL;
	fh->GetObject("M6351_0_PDHU_MCAL_BURST_Trig_Rej_BBFP1", auxb1);	// BBFP 1	
	if (auxb1) {
		auxb1->GetXaxis()->SetTitle("Time");
		auxb1->GetXaxis()->SetTimeDisplay(1);
		auxb1->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
		auxb1->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
		auxb1->SetLineColor(1);
		auxb1->GetXaxis()->SetRangeUser(tini, tend);
		auxb1->GetYaxis()->SetRangeUser(0.1, 1000.);
		auxb1->GetXaxis()->SetTitleOffset(0.8);
		auxb1->GetXaxis()->SetTitleSize(0.06);
		auxb1->GetXaxis()->CenterTitle();				
		auxb1->GetYaxis()->SetTitle("Counts");
		auxb1->GetYaxis()->CenterTitle();
		auxb1->GetYaxis()->SetTitleOffset(0.8);
		auxb1->GetYaxis()->SetTitleSize(0.06);
		auxb1->SetTitle("MCAL rejected BURST triggers");
		auxb1->Draw("ALP");
		leg3->AddEntry(auxb1, "BBFP 1", "lp");
		auxmax = TMath::MaxElement(auxb1->GetN(), auxb1->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*auxb2 = NULL;
	fh->GetObject("M6352_0_PDHU_MCAL_BURST_Trig_Rej_BBFP2", auxb2);	// BBFP 2
	if (auxb2) {
		auxb2->SetLineColor(2);
		auxb2->Draw("LPSAME");
		leg3->AddEntry(auxb2, "BBFP 2", "lp");
		auxmax = TMath::MaxElement(auxb2->GetN(), auxb2->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	TGraph	*auxb3 = NULL;
	fh->GetObject("M6353_0_PDHU_MCAL_BURST_Trig_Rej_BBFP3", auxb3);	// BBFP 3
	if (auxb3) {
		auxb3->SetLineColor(3);
		auxb3->Draw("LPSAME");
		leg3->AddEntry(auxb3, "BBFP 3", "lp");
		auxmax = TMath::MaxElement(auxb3->GetN(), auxb3->GetY());
		if (auxmax > ymax) ymax = auxmax;
	}
	
	leg3->Draw();	
	
	return c1;
}

//
//-------------------------------------------
//

TCanvas *hkTrgStatistics(TFile *fh, TTimeStamp epoch, TString *boostfile)
{
	// ---------------  calculate and draw trigger diagnostics  -------------
	
	int npt;
	double tini=-1., tend=-1.;
//	TTimeStamp ts;
	
	TText tt;
	TString str("MCAL Team - IASF-Bo - hkviewer version ");
	str += VERSION;
	
	TText tt1;
	TString str1("T start: ");
	
	TCanvas *c1 = new TCanvas("hkview_BURST_stat", "HK BURST trg statistics", 1000, 750);
	TPad *spad1 = new TPad("spad1","The first subpad",0.,.5,1.,1.);
	TPad *spad2 = new TPad("spad2","The second subpad",0.,0.,1.,.5);
	spad1->Draw();
	spad2->Draw();
	spad1->Divide(3, 1);
	
	// retrieve boost information
	int nrow=0;
	double *bini=NULL, *bend=NULL, *cont=NULL;
	string *label=NULL;
	if (boostfile) {
		cout << "Opening BOOST file " << boostfile->Data() << endl;
		ifstream fp(boostfile->Data());
		if (fp.fail()) cout << "BOOST file does not exist" << endl;
		else {
			string str;
			while ( !fp.eof() ) {
				getline(fp, str);
				nrow++;
			}
			cout << "Number of rows = " << nrow << endl;
			fp.close();
			fp.clear();	// <-- FONDAMENTALE! altrimenti non resetta lo status EOF del file
			cont = new double[nrow];
			bini = new double[nrow];
			bend = new double[nrow];
			label = new string[nrow];
			fp.open(boostfile->Data());
			for (int i=0; i<nrow; i++) {
				fp >> cont[i] >> bini[i] >> bend[i] >> label[i];
				//cout << cont << "  " << bini[i] << "  " << bend[i] << endl;
			}
		}
	} else cout << "No BOOST file specified" << endl;
	
	TH1F *hpartial = new TH1F("hpartial", "Partial trg distribution", 9, 1., 10.);
	TH1F *hpdist = new TH1F("hpdist", "Partial trg number distribution", 9, 1., 10.);
	TH2F *hp2 = new TH2F("hp2", "Double part. trg. distribution", 9, 1., 10., 9, 1., 10.);
	
	TGraph	*aux1[9];
	double *x;
	double *y[9]; 
	int naux, ntrg;
	for (int i=0; i<9; i++) aux1[i] = NULL;	
	fh->GetObject("M6329_0_PDHU_SA_MCAL_partial_BURST_Tri9", aux1[0]);	// partial trg X1 LE		
	fh->GetObject("M6330_0_PDHU_SA_MCAL_partial_BURST_Tri10", aux1[1]);	// partial trg X2 LE		
	fh->GetObject("M6331_0_PDHU_SA_MCAL_partial_BURST_Tri11", aux1[2]);	// partial trg Z1 LE		
	fh->GetObject("M6332_0_PDHU_SA_MCAL_partial_BURST_Tri12", aux1[3]);	// partial trg Z2 LE		
	fh->GetObject("M6333_0_PDHU_SA_MCAL_partial_BURST_Tri13", aux1[4]);	// partial trg X1 ME		
	fh->GetObject("M6334_0_PDHU_SA_MCAL_partial_BURST_Tri14", aux1[5]);	// partial trg X2 ME		
	fh->GetObject("M6335_0_PDHU_SA_MCAL_partial_BURST_Tri15", aux1[6]);	// partial trg Z1 ME		
	fh->GetObject("M6336_0_PDHU_SA_MCAL_partial_BURST_Tri16", aux1[7]);	// partial trg Z2 ME		
	fh->GetObject("M6337_0_PDHU_SA_MCAL_partial_BURST_Tri17", aux1[8]);	// partial trg HE		
	if (!aux1[0]) return c1;	// data missing: return
	
	int iboost=0;
	
	npt = aux1[0]->GetN();
	x = aux1[0]->GetX();
	tini = x[0];
	tend = x[npt-1];
	for (int i=0; i<9; i++) y[i] = aux1[i]->GetY();
	TGraph *gpdist = new TGraph(npt);
	
	// init boost index
	while(iboost<nrow && bini[iboost] < x[0]) iboost++;
	bool bflag = 1;

	// scan data and fill histos
	for (int j=0; j<npt; j++) {
	
		// is event outside a boost?
		if (nrow && iboost<=nrow) {
			if (bflag) {
				if (x[j]>bini[iboost]) bflag = 0;
			} else {
				if (x[j]>bend[iboost]) {
					bflag = 1;
					iboost++;
				}
			}
		}
		
		ntrg=0;
		if (bflag) {	// if yes, fill in histos
			int i1=-1, i2=-1;
			for (int i=0; i<9; i++) {
				naux = (y[i])[j];
				if (naux > 0) {
					hpartial->AddBinContent(i+1, naux);
					ntrg++;
					if (i1>-1 && i2==-1) i2=i;
					if (i1==-1) i1=i;
				}
			}
			hpdist->Fill(ntrg);
			if (ntrg == 2) hp2->Fill(i1+1, i2+1);
		}
		gpdist->SetPoint(j, x[j], ntrg);
	}
	
	// drawing section
	
	TPad* spad1_1 = (TPad*)(spad1->GetPrimitive("spad1_1"));
	spad1_1->cd();
	hpartial->GetXaxis()->SetTitle("Ratemeter id");
	hpartial->GetXaxis()->SetTitleSize(0.05);
	hpartial->GetXaxis()->SetLabelSize(0.05);
	hpartial->GetYaxis()->SetTitle("Counts");
	hpartial->GetYaxis()->SetTitleSize(0.05);
	hpartial->GetYaxis()->SetLabelSize(0.05);
	hpartial->Draw();
	
	TPad* spad1_2 = (TPad*)(spad1->GetPrimitive("spad1_2"));
	spad1_2->cd();
	gPad->SetLogy();
	hpdist->GetXaxis()->SetTitle("Number of RM");
	hpdist->GetXaxis()->SetTitleSize(0.05);
	hpdist->GetXaxis()->SetLabelSize(0.05);
	hpdist->GetYaxis()->SetTitle("Counts");
	hpdist->GetYaxis()->SetTitleSize(0.05);
	hpdist->GetYaxis()->SetLabelSize(0.05);
	hpdist->Draw();
	
	TPad* spad1_3 = (TPad*)(spad1->GetPrimitive("spad1_3"));
	spad1_3->cd();
	hp2->GetXaxis()->SetTitle("RM-1 id");
	hp2->GetXaxis()->SetTitleSize(0.05);
	hp2->GetXaxis()->SetLabelSize(0.05);
	hp2->GetYaxis()->SetTitle("RM-2 id");
	hp2->GetYaxis()->SetTitleSize(0.05);
	hp2->GetYaxis()->SetLabelSize(0.05);
	hp2->Draw("COLZ");
	
	spad2->cd();
	gpdist->GetXaxis()->SetTitle("Time");
	gpdist->GetXaxis()->SetTimeDisplay(1);
	gpdist->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
	gpdist->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
	gpdist->SetLineColor(1);
	gpdist->GetXaxis()->SetRangeUser(tini, tend);
	gpdist->GetYaxis()->SetRangeUser(0., 10.);
	gpdist->GetXaxis()->SetTitleOffset(0.8);
	gpdist->GetXaxis()->SetTitleSize(0.06);
	gpdist->GetXaxis()->CenterTitle();				
	gpdist->GetYaxis()->SetTitle("Counts");
	gpdist->GetYaxis()->CenterTitle();
	gpdist->GetYaxis()->SetTitleOffset(0.8);
	gpdist->GetYaxis()->SetTitleSize(0.06);
	gpdist->SetTitle("Number of simultaneous partial BURST triggers");
	gpdist->Draw("ALP");
	
	delete bini;
	delete bend;
	
	return c1;
}
