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

#ifndef MCVLib_GRB3913XZ
#define MCVLib_GRB3913XZ

#define VERBOSE 1

#include "GRB3913.h"

class GRB3913XZ  {
	
	protected:
		TCanvas		*cpXZ;
		
		double *time;
		double *data;
		double *dataAC;
		double *dataEbin;
		double t90X;
		double t90Z;
		int ns;
		
		GRB3913 *pX;
		GRB3913 *pZ;

	public:
		GRB3913XZ();
		GRB3913XZ(TString rootfile);
		~GRB3913XZ();
		double GetT90() {return (t90X > t90Z ? t90X : t90Z);};
		int LightCurve(double t0L, double tpre, double tpost);
		TH1D *GetLightCurve(int ipZ) {return (ipZ == 0 ? pX->GetLightCurve() : pZ->GetLightCurve());};
		TH2D *GetLightCurve2D(int ipZ) {return (ipZ == 0 ? pX->GetLightCurve2D() : pZ->GetLightCurve2D());};
		void DrawCustomLightCurve(double ti, double tf);
		void SaveCustomLightCurve(TString outpath);
		int SaveInfo(TString infofile);
		int SaveAnalysis(TString outpath);
		void SpectralAnalysis();
		void CustomSpectralAnalysis();
		void SetGRBBoundaries( double tbkgini, double tbkgfin, double tgrbini, double tgrbfin);
		ClassDef(GRB3913XZ,1);				//!< required for ROOT dictionary generation using rootcint
};



#endif
