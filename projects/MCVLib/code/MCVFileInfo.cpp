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
#include "MCVFileInfo.h"
ClassImp(MCVFileInfo);

MCVFileInfo::MCVFileInfo()
{

}

//
//--------------------------------------------------------------------
//

MCVFileInfo::~MCVFileInfo()
{
//	if (tdata) delete tdata;
}

//
//--------------------------------------------------------------------
//

void MCVFileInfo::PrintFileInfo() 
{
	cout << endl << "----------------------------------------------" << endl;
	printf("Run:\t\t\t %d\n", runid);
	printf("Fits file:\t\t %s\n", filename);
	printf("Number of events:\t %d\n", nevents);
	printf("Measure time:\t\t %.0f s\n", teff);
	printf("Mean count rate:\t %.1f evt/s\n", nevents/teff);
	
	if (pdhuformat==kTRUE) printf("\nData format:\t\t PDHU level-1\nPacket type:\t\t %d\n", pkttype); 
	else  {
		printf("\nData format:\t\t MCAL DFE-TE\n");
		if (grid==kTRUE) printf("Data mode:\t\t GRID\n");
		else printf("Data mode:\t\t BURST\n");
	}
	cout << "----------------------------------------------" << endl << endl;
	return;	
}

//
//--------------------------------------------------------------------
//

void MCVFileInfo::PrintCalibrationParameters()
{
	cout << endl << "Current calibration parameter file: " << calFile.Data() << endl << endl;
	printf("i\t bar\t oA (ch)\t oB (ch)\t u0A (ch/MeV)\t u0B (ch/MeV)\t alphaA (cm^-1)\t alphaB (cm^-1)\n\n");
	for (int i=0; i<30; i++) {
		if (i<15) printf("%d\t X%2d\t ", i, i+1);
		else printf("%d\t Z%2d\t ", i, i-14);
		printf("%8.2f\t %8.2f\t %8.3f\t %8.3f\t %8.4f\t %8.4f\n", oA[i], oB[i], u0A[i], u0B[i], alphaA[i], alphaB[i]);
	}
	cout << endl;
	
	cout << "Current offset used for Scientific Ratemeters calculations: " << endl;
	printf("i\t bar\t oA (ch)\t oB (ch)\t Offset (ch)\n"); 
	for (int i=0; i<30; i++) {
		if (i<15) printf("%d\t X%2d\t ", i, i+1);
		else printf("%d\t Z%2d\t ", i, i-14);
		printf("%6d\t\t %6d\t\t %6d\n", soA[i], soB[i], soA[i] + soB[i]);
	}
	return;
}


