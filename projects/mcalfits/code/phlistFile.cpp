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

#include "phlistFile.h"


phlistFile::phlistFile(std::string filenamein,std::string Templatein):
         fitsFile(filenamein,Templatein)
{
	printf("file created\n");
	
	fitsRowNum2 = 0;
}

void phlistFile::outLine( double time, float energy, short int mult, short int maxebar, float pos)
{
    if (DEBUG_PHLISTFILE) {      
        std::cerr << " phlistFile::outLine writing line: TT, E, mult, max_E_bar, pos: "
	          << time << " " <<  energy  << " " << mult << " " << maxebar << " " << pos << std::endl ;
    }   
   
  //            writes the fits values 
  // ------------------------------------------- 
  //            Extension 1 (hdu 2) : PHOTLIST
  // ------------------------------------------- 

    int hdutogo = 2 ; 
    int hdutype_fits ;
    int status_fits=0 ;
 
    if (fits_movabs_hdu(fitsPointer,hdutogo,&hdutype_fits,&status_fits) )
    {
       std::cerr << "phlistFile:: error going to fits hdu: " << hdutogo 
                 << " fits status: " << status_fits << std::endl ; 
    }   
 
    writeCell(fitsPointer,fitsRowNum, time ,"TIME") ; 
    writeCell(fitsPointer,fitsRowNum, energy ,"ENERGY") ; 
    writeCell(fitsPointer,fitsRowNum, mult ,"MULT") ; 
    writeCell(fitsPointer,fitsRowNum, maxebar ,"MAXEBAR") ; 
    writeCell(fitsPointer,fitsRowNum, pos ,"POS") ; 
 
    fitsRowNum ++ ; 

}

void phlistFile::outArrayLine( double time, short int mult, short int *bar, float *aenergy, float *apos)
{
   // NB: need to separate the routines working on different HDU because the HDU shift is the most time consuming operation!!!
    
  //            writes the fits values 
  // ------------------------------------------- 
  //            Extension 2 (hdu 3) : EVENTS
  // ------------------------------------------- 

	int hdutogo = 3 ; 
	int hdutype_fits ;
	int status_fits=0 ;
	
	if (fits_movabs_hdu(fitsPointer,hdutogo,&hdutype_fits,&status_fits) )
	{
	std::cerr << "phlistFile:: error going to fits hdu: " << hdutogo 
			<< " fits status: " << status_fits << std::endl ; 
	}   
	writeCell(fitsPointer,fitsRowNum2, time ,"TIME") ; 
	writeCell(fitsPointer,fitsRowNum2, mult ,"MULT") ; 
		
	char colname[9];
	for (int k=0; k<30; k++) {
		int id = bar[k];
		if (id>=0) {
			sprintf(colname, "BAR%02dENE", k);
			writeCell(fitsPointer,fitsRowNum2, aenergy[id] , colname) ; 
			sprintf(colname, "BAR%02dPOS", k);
			writeCell(fitsPointer,fitsRowNum2, apos[id] , colname) ; 
		}
		if (id == mult - 1) break;
	}
  	
	fitsRowNum2 ++ ; 

}

