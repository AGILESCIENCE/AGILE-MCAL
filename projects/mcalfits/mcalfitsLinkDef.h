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
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

writeCell
template < typename TIPO > 
            int writeCell( fitsfile * fptrout, int rownum0, TIPO valore, char * nomecol) 

#pragma link C++ function 
printVector(const std::vector<int>& );
#pragma link C++ class fitsFile;
#pragma link C++ class EChannels;
#pragma link C++ class phaFile;
#endif
