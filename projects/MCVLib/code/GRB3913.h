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

#ifndef MCVLib_GRB3913
#define MCVLib_GRB3913

#include "GRB.h"

class GRB3913  : public GRB {
	
	protected:
		double *time;
		double *data;
		double *dataAC;
		double *dataEbin;
		int ns;
		int plane;	// 0 = plane X, 1 = plane Z

	public:
		GRB3913();
		GRB3913(TString rootfile, int plane=0);
		~GRB3913();
		int LightCurve(double t0L, double tpre, double tpost);
		ClassDef(GRB3913,1);				//!< required for ROOT dictionary generation using rootcint
};



#endif
