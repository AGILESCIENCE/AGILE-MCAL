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


#ifndef DEFINED_PHLISTFILE           
#define	DEFINED_PHLISTFILE	1

#define DEBUG_PHLISTFILE   0         // fa stampe di debug 

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

class phlistFile : public fitsFile 
{

    public:

           phlistFile(std::string outphlist,std::string phlisttemplate) ;
           void outLine( double time, float energy, short int mult, short int maxebar, float pos);
           void outArrayLine( double time, short int mult, short int *bar, float *aenergy, float *apos);

	   
    protected:

       int fitsRowNum2 ; 

};

#endif 
