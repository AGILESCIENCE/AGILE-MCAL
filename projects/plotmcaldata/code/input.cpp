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
#include "plotmcaldata.h"
	
int loadDataRoot(TFile *fi, int ns, double *time, double *data, double *dataAC)
{
	// loads data from a root file created with mcalanalyzer

	// retrieve MCAL sci. RM data
	char gname[10];
	for (int i=0; i<22; i++) {
		if (i<11) sprintf(gname, "grmX%d", i+1);
		else sprintf(gname, "grmZ%d", i-10);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
		}
	}

	// retrieve AC sci. RM data
	for (int i=0; i<5; i++) {
		sprintf(gname, "gAC%d", i);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) dataAC[j*5 + i] = y[j];
		}
	}
	return 0;
}
	