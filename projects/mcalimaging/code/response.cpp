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

#include "mcalimaging.h"

extern float rbars[NBINS][NBARSBINX][NBARSBINZ];
extern float renergy[NBINS][NEBINX][NEBINZ];

void loadResponseFile(TString *parname)
{

	TFile *fp = new TFile(parname->Data());
	
	for (int i=0; i<NBINS; i++) {
		TString hname("hBarCorr_");
		hname += (i+1);
		TH2F *hBarCorr = (TH2F *) fp->Get(hname.Data());
		if (hBarCorr) {
			for (int j=0; j<NBARSBINX; j++) {
				for (int k=0; k<NBARSBINZ; k++) {
					rbars[i][j][k] = hBarCorr->GetBinContent(j+1, k+1);
				}
			}
		} else cout << "*** cannot find object " << hname.Data() << endl;
	}
	
	for (int i=0; i<NBINS; i++) {
		TString hname("hEXEZ_");
		hname += (i+1);
		TH2F *hEXEZ = (TH2F *) fp->Get(hname.Data());
		if (hEXEZ) {
			for (int j=0; j<NEBINX; j++) {
				for (int k=0; k<NEBINZ; k++) {
					renergy[i][j][k] = hEXEZ->GetBinContent(j+1, k+1);
				}
			}
		} else cout << "*** cannot find object " << hname.Data() << endl;
	}
	
	fp->Close();
	return;
}

//
//--------------------------------------------------------------------
//

void saveResponseFile(TString *parname, TObjArray *hc, int nbin)
{

	TFile *fp = new TFile(parname->Data(), "update");

	TH2F *hBarCorr = (TH2F *) hc->FindObject("hBarCorr");
	if (hBarCorr) {	
		TString newname("hBarCorr");
		newname += "_";
		newname += nbin;
		TH2F *hnew = (TH2F *) hBarCorr->Clone(newname.Data());
		hnew->Write();
	} else cout << "*** cannot find object hBarCorr" << endl;

	TH2F *hEXEZ = (TH2F *) hc->FindObject("hEXEZ");
	if (hEXEZ) {	
		TString newname("hEXEZ");
		newname += "_";
		newname += nbin;
		TH2F *hnew = (TH2F *) hEXEZ->Clone(newname.Data());
		hnew->Write();
	} else cout << "*** cannot find object hEXEZ" << endl;
	
	
	fp->Close();

	return;
}

//
//--------------------------------------------------------------------
//

void printResponseFile()
{
	float aux;
	
	cout << "BARS CORRELATION RESPONSE ARRAY" << endl;
	for (int i=0; i<NBINS; i++) {
		printf("%d\n", i+1);
		for (int j=0; j<NBARSBINX; j++) {
			for (int k=0; k<NBARSBINZ; k++) {
				aux = rbars[i][j][k];
				if (aux) printf("\t%6.4f", aux);
				else printf("\t");
			}
			printf("\n");
		}
	}
	
	cout << "ENERGY CORRELATION RESPONSE ARRAY" << endl;
	for (int i=0; i<NBINS; i++) {
		printf("%d\n", i+1);
		for (int j=0; j<NBARSBINX; j++) {
			for (int k=0; k<NBARSBINZ; k++) {
				aux = renergy[i][j][k];
				if (aux) printf("\t%6.4f", aux);
				else printf("\t");
			}
			printf("\n");
		}
	}
	
	return;
}

//
//--------------------------------------------------------------------
//

int  getMCALEnergyBin(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose)
{
	float barprob[NBINS];
	float energyprob[NBINS];
	float prob[NBINS];
	float maxprob = 0.;
	int   mcalguess = 0;
	
	for (int i=0; i<NBINS; i++) {
		barprob[i] = rbars[i][nbarsX][nbarsZ];
		energyprob[i] = renergy[i][(int) floor(eX/EBIN)][(int) floor(eZ/EBIN)];
		prob[i] = barprob[i]*energyprob[i];
		if (prob[i] > maxprob) {
			maxprob = prob[i];
			mcalguess = i;
		}
	} 
	
	if (verbose) {
		printf("\n\tBars(X, Z) = (%d, %d)  -  Energy(X, Z) = (%.2f, %.2f)\n", nbarsX, nbarsZ, eX, eZ);
		printf("\t\tEnergy bins:   ");
		for (int i=0; i<NBINS; i++) printf("\t   %d", i);
		printf("\n\t\tBars prob.:   ");
		for (int i=0; i<NBINS; i++) printf("\t%6.4f", barprob[i]);
		printf("\n\t\tEnergy prob.: ");
		for (int i=0; i<NBINS; i++) printf("\t%6.4f", energyprob[i]);
		printf("\n\t\tTotal prob.:  ");
		for (int i=0; i<NBINS; i++) printf("\t%6.4f", prob[i]);
		printf("\n\tBest MCAL guess bin = %d\n", mcalguess);
	}
	
	return mcalguess;
}

