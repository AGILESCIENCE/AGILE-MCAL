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

#include "MCVBarCorrelation.h"
ClassImp(MCVBarCorrelation);

MCVBarCorrelation::MCVBarCorrelation() : MCVStdAnalysis()
{
}

//
//--------------------------------------------------------------------
//

MCVBarCorrelation::MCVBarCorrelation(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
}

//
//--------------------------------------------------------------------
//

MCVBarCorrelation::~MCVBarCorrelation()
{

}

//
//--------------------------------------------------------------------
//

int MCVBarCorrelation::Calculate()
{

	maxiter  = conf->GetValue(".barcorr.max.iter.", 100);
	maxsigma = conf->GetValue(".barcorr.sigma.max.", 10.);
	float maxvalue = conf->GetValue(".barcorr.value.max.", 1.);
	
	avgbc = new double[maxiter];
	avgbclow = new double[maxiter];
	stdbc = new double[maxiter];
	stdbclow = new double[maxiter];
	for (int k=0; k<maxiter ; k++) avgbc[k] = avgbclow[k] = stdbc[k] = stdbclow[k] = 0.;


	TH2F *hbc=NULL, *hbclow=NULL;
	if (rawHisto==NULL || info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	
	// retrieve bar correlation histograms
	TH2F *haux;
	haux =(TH2F *) rawHisto->FindObject("barcorr");
	hbc =(TH2F *) haux->Clone("auxbarcorr");
	haux =(TH2F *) rawHisto->FindObject("barcorrLowDT");
	hbclow =(TH2F *) haux->Clone("auxbarcorrLowDT");
		
	if (hbc==NULL || hbclow==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}

	// brutal approach
	float auxval;
	int nr=0, nrlow=0;
	TH1F *hd = new TH1F("hd", "hd", 10000, 0., 1.);
	TH1F *hdlow = new TH1F("hdlow", "hdlow", 10000, 0., 1.);
	TH1F *hb1 = new TH1F("hb1", "hb1", 30, 0, 30);
	TH1F *hb2 = new TH1F("hb2", "hb2", 30, 0, 30);
	
	// working on histo barcorr
	printf("\nWorking on high DT bar correlation histogram\n");
	for (int i=1; i<31; i++) {
		for (int j=1; j<31; j++) {
			auxval = hbc->GetBinContent(i, j);
			hd->Fill(auxval);
			/*if (auxval>maxvalue) {
				nr++;
				printf("\tsuspect point #%d: (%d,%d) \tvalue=%6f\n", nr, i, j, auxval);
			}*/
		}
	}
	
	// working on histo barcorrLowDT
	printf("\nWorking on low DT bar correlation histogram\n");
	for (int i=1; i<31; i++) {
		for (int j=1; j<31; j++) {
			auxval = hbclow->GetBinContent(i, j);
			hdlow->Fill(auxval);
			if (auxval>maxvalue) {
				nrlow++;
				printf("\tsuspect point #%d: (%d,%d) \tvalue=%6f\n", nrlow, i, j, auxval);
				hb1->Fill(i-1);
				hb2->Fill(j-1);
			}		
		}
	}
	hb1->Scale(1./nrlow);
	hb2->Scale(1./nrlow);
	
	TCanvas *ch = new TCanvas("cd", "cd", 800, 300);
	ch->Divide(2,1);
	ch->cd(1);
	hd->GetXaxis()->SetRangeUser(0., 0.01);
	hd->Draw();
	hdlow->SetLineColor(2);
	hdlow->Draw("SAME");
	ch->cd(2);
	hb2->SetLineColor(2);
	hb2->Draw();
	hb1->Draw("SAME");
	
	cout << endl << nrlow << endl << endl;
	for (int i=1; i<=30;i++) cout << hb1->GetBinContent(i) << endl;
	cout << endl;
	for (int i=1; i<=30;i++) cout << hb2->GetBinContent(i) << endl;
	
	// fine (but useless!) approach: work on low DT bar correlation histo
//	printf("Low DT histo:\n");
//	Loop(hbclow, avgbc, stdbc);
	
}

//
//--------------------------------------------------------------------
//

void MCVBarCorrelation::Loop(TH2F *hbc, double *avg, double *std)
{
	double auxbc, maxbc;
	int imax, jmax;
	int nrem = 0;
	for (int k=0; k<maxiter; k++) {
		
		// get mean and std. dev. 
		maxbc = 0.;
		for (int i=1; i<31; i++) {
			for (int j=1; j<31; j++) {
				auxbc = hbc->GetBinContent(i, j);
				if (auxbc>0.) {
					avg[k] += auxbc;
					std[k] += auxbc*auxbc;
					if (auxbc>maxbc) {
						maxbc = auxbc;
						imax = i;
						jmax = j;
					}
				}
			}
		}
		avg[k] /= 900.;
		std[k] = sqrt(std[k]/900. - avg[k]*avg[k]);
		printf("Cycle %2d \tmean=%6f \tstd.dev=%.2E \tmax. point: (%d,%d) \tvalue=%6f \tdev=%6.2f sigma\n", k, avg[k], std[k], imax, jmax, hbc->GetBinContent(imax, jmax), (hbc->GetBinContent(imax, jmax) - avg[k])/std[k]);
		
		// removing highest points
		for (int i=1; i<31; i++) {
			for (int j=1; j<31; j++) {
				auxbc = (hbc->GetBinContent(i, j) - avg[k])/std[k];
				if (auxbc > maxsigma) {
					nrem++;
					printf("\tremoving point #%d: (%d,%d) \tvalue=%6f \tdev=%6.2f sigma\n", nrem, i, j, hbc->GetBinContent(i, j), auxbc);
				 	hbc->SetBinContent(i, j, -1.);
				}
			}
		}
	
	}


	return;
}
//
//--------------------------------------------------------------------
//

void MCVBarCorrelation::PrintResults()
{
	printf("\n--------------------- BAR CORRELATION ANALYSIS ----------------------\n");
	if (status == OK || status == OUT_OF_SPEC) {
	
	}
	
	return;
}
