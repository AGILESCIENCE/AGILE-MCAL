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

#include "MCVBurst.h"
ClassImp(MCVBurst);

MCVBurst::MCVBurst()
{

}

//
//--------------------------------------------------------------------
//

MCVBurst::MCVBurst(const char *name)
{
	SetName(name);
}

//
//--------------------------------------------------------------------
//

MCVBurst::~MCVBurst()
{

}

//
//--------------------------------------------------------------------
//

void MCVBurst::PrintInfo()
{
	printf("\n----- BURST #%d -----\n", burstid);
	printf("BSTART (sec) \t= %12.6f\n", bstart*1.E-6);
	printf("BSTOP (sec) \t= %12.6f\n", bstop*1.E-6);
	printf("Energy band \t= %d\n", nume);
	printf("Geometry band \t= %d\n", numg);
	printf("SIT id \t\t= %d\n", bsrsit);
	printf("BER \t\t= %d\n", ber);
	printf("BSR \t\t= %d\n", bsr);
	printf("\n--------------------\n");
	return;
}
