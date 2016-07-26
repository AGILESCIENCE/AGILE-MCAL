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


track::track(): prev(NULL) 
{

}

//
//--------------------------------------------------------------------
//

track::track(MCALevt *inevt, int ib, float eth): prev(NULL) 
{

	// init a new track for event inevt, starting from bar ibar
	int iend, id;
	
	ibar = ib;
	nbars = 0;
	Etot = 0.;
	evt = -1;
	trid = -1;
	if (ib<=14) {
		iend = 14;
		plane = 0;
	} else {
		iend = 29;
		plane = 1;
	}
	
	// fill in energy and position arrays
	for (int i=ib; i<=iend; i++) {
		id = inevt->bar[i];
		if (inevt->E[id] < eth) break;
		E[nbars] = inevt->E[id];
		pos[nbars] = inevt->pos[id];
		Etot += inevt->E[id];
		nbars++;
	}
	convertPosition();
	calcAvgParam();
}

//
//--------------------------------------------------------------------
//

void track::print() 
{
	// print track data to standard output

	printf("\nEvt %d - plane %c - tr-id %d\n", evt, plane ? 'Z' : 'X', trid);
	printf("\tbar id:");
	for (int i=0; i<nbars; i++) printf("\t%d", ibar+i);
	printf("\n\tE:");
	for (int i=0; i<nbars; i++) printf("\t%6.2f", E[i]);
	printf("\n\tpos:");
	for (int i=0; i<nbars; i++) printf("\t%6.2f", pos[i]);
	printf("\n\tX:");
	for (int i=0; i<nbars; i++) printf("\t%6.2f", pX[i]);
	printf("\n\tZ:");
	for (int i=0; i<nbars; i++) printf("\t%6.2f", pZ[i]);
	printf("\n");
}

//
//--------------------------------------------------------------------
//

void track::convertPosition()
{
	// convert positions from MCVLib to Laben reference systems with center in MCAL center
	
	if (plane == 0) { // X plane track
		for (int i=0; i<nbars; i++) {
			pX[i] = -pos[i];
			pZ[i] = X1Z + (ibar + i)*BARSTEP;
		}
	} else {	  // Z plane track
		for (int i=0; i<nbars; i++) {
			pX[i] = Z1X + (ibar-15 + i)*BARSTEP;
			pZ[i] = -pos[i];
		}
	}	
	
}

//
//--------------------------------------------------------------------
//

void track::calcAvgParam()
{
	// calculate energy-weighted interaction center
	// WARNING: chance coincidences not considered yet
}

//
//--------------------------------------------------------------------
//

point::point() 
{
	X = Z = eX = eZ = xk = yk = E = 0.;
}

//
//--------------------------------------------------------------------
//

point::point(bool laben, float x, float z, float energy, float errx, float errz)
{
	if (laben) {
		X = x;
		Z = z;
		setKalmanCoordinate();
	} else {
		xk = x;
		yk = z;
		setLabenCoordinate();
	}
	E = energy;
	eX = errx;
	eZ = errz;
}

//
//--------------------------------------------------------------------
//

point::point(MCALevt *inevt, int ibar) 
{
	if (ibar<=14) plane = 0;
	else plane = 1;
	int id = inevt->bar[ibar];
	E = inevt->E[id];
	bar = ibar;
	float xm, zm; // coordinate rispetto al centro MCAL, assi come refsys Laben
	
	if (plane == 0) { // X plane track
			xm = -inevt->pos[id];
			zm = X1Z + ibar*BARSTEP;
	
			// passo dal refsys delle barre al refsys Laben
			X = -XREF + xm;
			Z = -ZREF + zm;
			eX = PSIGMA;
			eZ = 0.5*BARSTEP;
			
			// passo dal refsys Laben al refsys Kalman
			setKalmanCoordinate();
			
	} else {	  // Z plane track
			xm = Z1X + (ibar-15)*BARSTEP;
			zm = -inevt->pos[id];
			
			// passo dal refsys delle barre al refsys Laben
			X = -XREF + xm;
			Z = -ZREF +zm;
			eX = 0.5*BARSTEP;
			eZ = PSIGMA;
			
			// passo dal refsys Laben al refsys Kalman
			setKalmanCoordinate();
	}	
}

//
//--------------------------------------------------------------------
//

void point::setKalmanCoordinate()
{
	xk = OFFSET_XK - XREF - X;
	yk = ZREF - OFFSET_YK + Z;
	return;
}

//
//--------------------------------------------------------------------
//

void point::setLabenCoordinate()
{
	X = OFFSET_XK - XREF - xk;
	Z = yk - ZREF + OFFSET_YK;
	return;
}

//
//--------------------------------------------------------------------
//

void point::show()
{
	printf("\tX = %5.2f ; Z =  %5.2f ; xk = %5.2f ; yk =  %5.2f ; E = %5.2f\n", X, Z, xk, yk, E);
}
