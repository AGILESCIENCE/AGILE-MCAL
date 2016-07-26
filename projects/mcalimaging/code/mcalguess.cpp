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

#include "mcalguess.h"

ClassImp(mcalguess);

mcalguess::mcalguess()
{
	dim = NBINS*NBARSBINX*NBARSBINZ*NEBINX*NEBINZ;
//	econv = 1./EBIN;
	for (int i=0; i<NBINS; i++) {
		nevt[i] = 0;
		for (int j=0; j<NBARSBINX; j++) {
			for (int k=0; k<NBARSBINZ; k++) {
				for (int p=0; p<NEBINX; p++) {
					for (int q=0; q<NEBINZ; q++) {
						prob[i][j][k][p][q] = 0.;
					}	
				}
			}
		}
	}
}

//
//--------------------------------------------------------------------
//

mcalguess::~mcalguess()
{

}

//
//--------------------------------------------------------------------
//

void mcalguess::SetEnergyArray(double *earr, float *eXarr, float *eZarr)
{
	for (int i=0; i<NBINS+1; i++) energybins[i] = earr[i];
	for (int i=0; i<NEBINX+1; i++) ebinX[i] = eXarr[i];
	for (int i=0; i<NEBINZ+1; i++) ebinZ[i] = eZarr[i];
	return;
}

//
//--------------------------------------------------------------------
//

void mcalguess::Fill(int ibin, int nbarsX, int nbarsZ, float eX, float eZ)
{
/*	// good only for equally spaced energy bins
	static float econv = 1./EBIN;
	int ieX = (int) floor(eX*econv);
	int ieZ = (int) floor(eZ*econv);
*/
	int ieX=0, ieZ=0;
	while (eX > ebinX[ieX+1] && ieX < NEBINX-1) ieX++;
	while (eZ > ebinZ[ieZ+1] && ieZ < NEBINZ-1) ieZ++;
	prob[ibin][nbarsX][nbarsZ][ieX][ieZ]++;
	nevt[ibin]++;
	return;
}

//
//--------------------------------------------------------------------
//

void mcalguess::Normalize(int ibin)
{
	if (nevt[ibin]) {
		float norm = 1./nevt[ibin];
		for (int j=0; j<NBARSBINX; j++) {
			for (int k=0; k<NBARSBINZ; k++) {
				for (int p=0; p<NEBINX; p++) {
					for (int q=0; q<NEBINZ; q++) {
						prob[ibin][j][k][p][q] *= norm;
					}	
				}
			}
		}
		cout << "mcalguess: normalized bin " << ibin << "  -  number of entries = " << nevt[ibin] << endl;
	} else cout << "mcalguess: *** cannot normalize bin " << ibin << ": zero events." << endl;
	return;
}

//
//--------------------------------------------------------------------
//

float *mcalguess::GetBinProb(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose)
{
	Calculate(nbarsX, nbarsZ, eX, eZ);
	if (verbose) PrintVerbose();
		
	return evtprob;
}

//
//--------------------------------------------------------------------
//

int mcalguess::GetBestBin(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose)
{
	Calculate(nbarsX, nbarsZ, eX, eZ);
	if (verbose) PrintVerbose();
		
	return mguess;
}

//
//--------------------------------------------------------------------
//

int mcalguess::GetBestEnergy(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose)
{
	Calculate(nbarsX, nbarsZ, eX, eZ);
	if (verbose) PrintVerbose();
		
	return avgE;
}

//
//--------------------------------------------------------------------
//

float mcalguess::GetKalmanProb(float EK, bool verbose)
{
	CalculateKalman(EK);
	if (verbose) PrintKalmanVerbose(EK);
	if (kbin < NBINS) return evtprob[kbin];
	else return 0.;
}

//
//--------------------------------------------------------------------
//

float mcalguess::GetKalmanBin(float EK, bool verbose)
{
	CalculateKalman(EK);
	if (verbose) PrintKalmanVerbose(EK);
	return kbin;
}

//
//--------------------------------------------------------------------
//

void mcalguess::PrintVerbose()
{
	printf("\n\tBars(X, Z) = (%d, %d)  -  Energy(X, Z) = (%.2f, %.2f)\n", currnbarsX, currnbarsZ, curreX, curreZ);
	printf("\t\tEnergy bins:   ");
	for (int i=0; i<NBINS; i++) printf("\t   %d", i);
	printf("\n\t\tTotal prob.:  ");
	for (int i=0; i<NBINS; i++) printf("\t%6.4f", evtprob[i]);
	printf("\n\tBest MCAL guess bin = %d  -  estimated weighted energy = %.2f MeV\n", mguess, avgE);
}

//
//--------------------------------------------------------------------
//

void mcalguess::PrintKalmanVerbose(float EK)
{
	printf("\n\tKalman energy = %.2f MeV  -  Bin = %d  -  Probability = %.4f\n", EK, kbin, kbin < NBINS ? evtprob[kbin] : 0.);

}

//
//--------------------------------------------------------------------
//

void mcalguess::Calculate(int nbarsX, int nbarsZ, float eX, float eZ)
{
	currnbarsX = nbarsX;
	currnbarsZ = nbarsZ;
	curreX = eX;
	curreZ = eZ;
	
	// get probability
	//int ieX = (int) floor(eX*econv);
	//int ieZ = (int) floor(eZ*econv);
	int ieX=0, ieZ=0;
	while (eX > ebinX[ieX+1] && ieX < NEBINX-1) ieX++;
	while (eZ > ebinZ[ieZ+1] && ieZ < NEBINZ-1) ieZ++;

	float totprob = 0.;
	for (int i=0; i< NBINS; i++) {
		evtprob[i] = prob[i][nbarsX][nbarsZ][ieX][ieZ];
		totprob += evtprob[i];
	}
	
	// normalize probability
	float norm = 1./totprob;
	for (int i=0; i< NBINS; i++) evtprob[i] *= norm;
	
	// get most probable bin  
	float maxprob = evtprob[0];
	mguess = 0;
	for (int i=1; i<NBINS; i++) {
		if (evtprob[i] > maxprob) {
			maxprob = evtprob[i];
			mguess = i;
		}
	} 
	
	// get estimated weighted energy
	avgE = 0.;	
	for (int i=1; i<NBINS; i++) avgE += evtprob[i]*avgenergy[i];
	
	return;
}

//
//--------------------------------------------------------------------
//

void mcalguess::CalculateKalman(float EK)
{
	kbin = 0;
	for (int i=1; i<NBINS; i++) {
		if (EK >= energybins[i]) kbin++;
		else break;
	}
	return;
}

//
//--------------------------------------------------------------------
//

TH2F *mcalguess::GetBarCorr(int ibin, bool verbose)
{
	TH2F *hBarCorr;
	TString name("hBarCorr_RM_");
	name += ibin;
	hBarCorr = new TH2F(name.Data(), "X and Z number of bars correlation", 16, 0, 15, 16, 0, 15);
	hBarCorr->GetXaxis()->SetTitle("Number of fired bars on plane X");
	hBarCorr->GetYaxis()->SetTitle("Number of fired bars on plane Z");
	TString title("Bar correlation ");
	title += energybins[ibin];
	title += " - ";
	title += energybins[ibin+1];
	title += " MeV";
	hBarCorr->SetTitle(title.Data());
	
	float totprob;
	if (nevt[ibin]) {
		float norm = 0;
		for (int j=0; j<NBARSBINX; j++) {
			for (int k=0; k<NBARSBINZ; k++) {
				totprob = 0.;
				for (int p=0; p<NEBINX; p++) {
					for (int q=0; q<NEBINZ; q++) {
						totprob += prob[ibin][j][k][p][q];
					}	
				}
				hBarCorr->SetBinContent(j+1, k+1, totprob);
				norm += totprob;
			}
		}
		if (verbose) cout << "Histogram " << name.Data() << ": total integral = " << norm << endl;
	}
	
	return hBarCorr;
}
	
//
//--------------------------------------------------------------------
//

TH2F *mcalguess::GetEnergyCorr(int ibin, bool verbose)
{
	TH2F *hEXEZ;
	TString name("hEXEZ_RM_");
	name += ibin;
	hEXEZ = new TH2F(name.Data(), "EZ vs EX", NEBINX, ebinX, NEBINZ, ebinZ);
	hEXEZ->GetXaxis()->SetTitle("EX (MeV)");
	hEXEZ->GetYaxis()->SetTitle("EZ (MeV)");
	TString title("Energy correlation ");
	title += energybins[ibin];
	title += " - ";
	title += energybins[ibin+1];
	title += " MeV";
	hEXEZ->SetTitle(title.Data());

	float totprob;
	if (nevt[ibin]) {
		float norm = 0;
		for  (int p=0; p<NEBINX; p++){
			for  (int q=0; q<NEBINZ; q++){
				totprob = 0.;
				for (int j=0; j<NBARSBINX; j++) {
					for (int k=0; k<NBARSBINZ; k++) {
						totprob += prob[ibin][j][k][p][q];
					}	
				}
				hEXEZ->SetBinContent(p+1, q+1, totprob);
				norm += totprob;
			}
		}
		if (verbose) cout << "Histogram " << name.Data() << ": total integral = " << norm << endl;
	}
	
	return hEXEZ;
}

