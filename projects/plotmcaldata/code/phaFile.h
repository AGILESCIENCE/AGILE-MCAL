//  last update: 12-sept-2007
//  -------------------------------------------
//  file phaFile.h 
//  m.galli, aug 2007
//  --------------------------------------------
//  Class to produce an pha fits file, linea per linea
//
//  conserva inoltre un vector con i valori dello spettro
//  Per il momento non li usa.
//            
//  -------------------------------------------------------
//

#ifndef DEFINED_PHAFILE           
#define	DEFINED_PHAFILE	1

#define DEBUG_PHAFILE   1         // fa stampe di debug 

#include <cstdlib>
#include <iostream>   
#include <iomanip>
#include <fstream>  

#include <string>
#include <vector>

//#include "cfitsio.h" 
//#include "fitsio2.h" 

//#include "writecell.cpp"
//#include "useful.h"

#include "fitsFile.h" 

class phaFile : public fitsFile 
{

    public:

           phaFile(std::string outpha,std::string phatemplate) ;
           //void setAndOutE( int channel, int counts) ;
           void setEline( float Elow, float Eup,int ch, int countsP1in, int countsP2in);
           void setEline( float Elow, float Eup,int ch, int countsin);
	   void OutEline(int ch, int counts) ; 
	   void print(std::string nomefile) ; 
           //void chiudi() ; 


    private:

       //int status_fits  ;    
       //fitsfile * fitsPointer    ;  
       
       //int fitsRowNum ; 
  
       //std::string fileName ; 
       //std::string templatename ; 
 
       std::vector<float>  Elower ;  // lower E
       std::vector<float>  Eupper ;  // upper E 
       std::vector<int>  channel  ;  // num of E channel 
       //std::vector<int>  countsP1    ;  // counts for plane 1 (upper)
       //std::vector<int>  countsP2    ;  // counts for plane 2 
       std::vector<int>  counts      ;  // counts for the whole detector 
};

#endif 
