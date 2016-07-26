//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef mcalguess_classdef
#define mcalguess_classdef

#include "mcalimaging.h"


class mcalguess : public TNamed {
	public:
		mcalguess();
		~mcalguess();
		void Fill(int nbin, int nbarsX, int nbarsZ, float eX, float eZ);
		int *GetNEvents() {return nevt;};
		int GetNEvents(int i) {return nevt[i];};
		float *GetBinProb() {return evtprob;};
		float *GetBinProb(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose=FALSE);
		int GetBestBin() {return mguess;};
		int GetBestBin(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose=FALSE);
		int GetBestEnergy() {return avgE;};
		int GetBestEnergy(int nbarsX, int nbarsZ, float eX, float eZ, bool verbose=FALSE);
		TH2F *GetBarCorr(int ibin, bool verbose=FALSE);
		TH2F *GetEnergyCorr(int ibin, bool verbose=FALSE);
		float GetKalmanProb(float EK, bool verbose=FALSE);
		float GetKalmanBin(float EK, bool verbose=FALSE);
		void Normalize(int ibin);
		void SetEnergyArray(double *earr, float *eXarr, float *eZarr);
	
	protected:
		int nevt[NBINS];
		long dim;
		float econv;
		int mguess;
		int kbin;
		double energybins[NBINS+1];
		double avgenergy[NBINS];
		float evtprob[NBINS];
		float ebinX[NEBINX+1];
		float ebinZ[NEBINZ+1];
		float prob[NBINS][NBARSBINX][NBARSBINZ][NEBINX][NEBINZ];
		
		int currnbarsX;
		int currnbarsZ;
		float curreX;
		float curreZ;
		float avgE;
		void PrintVerbose();
		void PrintKalmanVerbose(float EK);
		void Calculate(int nbarsX, int nbarsZ, float eX, float eZ);
		void CalculateKalman(float EK);
		
		ClassDef(mcalguess,1);
};

#endif
