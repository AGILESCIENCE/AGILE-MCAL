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
#include "MCVSelection.h"
ClassImp(MCVSelection);

MCVSelection::MCVSelection()
{
	SetDefault();
}

//
//--------------------------------------------------------------------
//

MCVSelection::~MCVSelection()
{

}

//
//--------------------------------------------------------------------
//

void MCVSelection::PrintSelection()
{
	printf("\n-------- Selection: %s --------\n", GetName());
	printf("selected %d bars:\n", nbars);
	for (int i=0; i<30; i++) if (bars[i]) printf(" %d", i);
	printf("\n");
	if (pdAEnable)    printf("%d \t<= PD-A <= \t%d\n", minA, maxA);
	if (pdBEnable)    printf("%d \t<= PD-B <= \t%d\n", minB, maxB);
	if (multEnable)   printf("%d \t<= Multiplicity <= \t%d\n", minMult, maxMult);
	if (posEnable)    printf("%6f \t<= Position (cm) <= \t%6f\n", minPos, maxPos);
	if (energyEnable) printf("%6f \t<= Energy (MeV) <= \t%6f\n", minE, maxE);
	if (timeEnable)   printf("%6f \t<= Time (s) <= \t%6f\n", minTime, maxTime);
	printf("----------------------------------------\n");
	return;
}

//
//--------------------------------------------------------------------
//

void MCVSelection::SetDefault()
{
	pdAEnable = 0;
	pdBEnable = 0;
	multEnable = 0;
	posEnable = 0;
	energyEnable = 0;
	timeEnable = 0;
	relToOffset = 1;
	isBarStrong = 0;
	isPosStrong = 0;
	isEStrong = 0;
	
	nbars = 0;
	for (int i=0; i<30; i++) bars[i] = 0;
	minA = minB = MINPHA;
	maxA = maxB = MAXPHA;
	minMult = 1;
	maxMult = 30;
	minPos = MINPOS;
	maxPos = MAXPOS;
	minE = MINE;
	maxE = MAXE;
	minTime = MINTIME;
	maxTime = MAXTIME;
	saveTracksFlg = 0;
	return;
}
