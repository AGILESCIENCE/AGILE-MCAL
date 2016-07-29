//  last update: 19-sept-2007
//  -------------------------------------------
//  file FitsFile.h 
//  m.galli, sept 2007
//  --------------------------------------------
//  The parent of classes for fits files, as:
//  phaFile, rspFile, arfFile
//  contains basic methods to access to these files 
//            
//  -------------------------------------------------------
//

#ifndef DEFINED_FITSFILE           
#define	DEFINED_FITSFILE	1

#define DEBUG_FITSFILE   1         // fa stampe di debug 

#include <cstdlib>
#include <iostream>   
#include <iomanip>
#include <fstream>  
#include <typeinfo>
#include <string.h>
#include <vector>
#include <stdio.h>

//#include "cfitsio.h" 
//#include "fitsio2.h" 
#include "fitsio.h" 

//#include "writecell.cpp"

#ifndef DEFINED_WRITECELL           
#define	DEFINED_WRITECELL	1

#include <cstdlib>
#include <iostream>   
#include <iomanip>
#include <fstream>  

//#define DEBUG_WRITECELL 1

template < typename TIPO > 
            int writeCell( fitsfile * fptrout, int rownum0, TIPO valore, char * nomecol) 
 {
    int status_fits=0 ; 

    int firstelem=1 ; 
    int colnum ;
    int tipodati ;
    int rownum ;
    
    rownum=rownum0+1 ; // fitsio parte da 1 a numerare le rows, ma
                       // ogni programma C parte da 0 , per cui l'uno
		       // lo aggiungo qui. 

   // int tipodati=TLONG ; 
 
    // determino il tipo del dato fornito in argomento , in base a questo fisso tipodati
    // tipodati serve al fitsio per fare le conversioni automatiche :
    //         chiamo fits_get_colnum che dal nome mi da il numero della colonna 
    //         chiamo fits_write_col per scrivere la colonna 
    
    // ATTENZIONE, ai codici di tipo di dato di cfitsio :
    // sorgono ambiguita': per  fits I  e' uno short int = 16 bits, 
    // invece per gcc un int e' 32 bits, che qui e' long 
    // si puo' usare il tipo TINT(31), che e' definito come platform dependent in cfitsio 
    // 
    //       TBIT  = 1 =	X = 1 bit
    //       TBYTE = 11 =1B = 8 bit = 1 byte ,  char in gcc Linux i386 
    //       TSHORT(=21)= 1I = int 16 bits in gcc  Linux  i386      
    //       TLONG (41)== TINT32BIT (41)= 1J = int in gcc , 32 bits.
    //                          (TINT32BIT usato solo in get_coltyep in fitsio3)
    //       TLONGLONG = 1K = 64 bits = long long int in gcc Linux  i386
    //
    //       TSBYTE(12) : 8 bit signed
    //       TUINT(30) = V = unsigned int 16 bits 
    //       TUSHORT(20) = U = unsigned short 16 bits 
    //       TULONG(40) 
    //       TFLOAT(42) = E = 32 bit float 
    //       TDOUBLE(82) = D = 64 bit float 
    //       TLOGICAL ( 14) = L 
    //       TSTRING ( 16 ) = A 
    
    //if     ( typeid(valore).name() == typeid(int).name() )     tipodati = TINT32BIT ;
    if     ( typeid(valore).name() == typeid(int).name() )           tipodati = TINT ;
    else if( typeid(valore).name() == typeid(long int).name() )      tipodati = TLONG ;
    else if( typeid(valore).name() == typeid(short int).name() )     tipodati = TSHORT ; 
    else if( typeid(valore).name() == typeid(unsigned int).name() )  tipodati = TULONG ; 
    else if( typeid(valore).name() == typeid(unsigned long).name() ) tipodati = TULONG ; 
    else if( typeid(valore).name() == typeid(double).name() )        tipodati = TDOUBLE ;    
    else if( typeid(valore).name() == typeid(float).name()  )        tipodati = TFLOAT ;   
    else if( typeid(valore).name() == typeid(long long int).name() ) tipodati = TLONGLONG ; 
    //else if( typeid(valore).name() == typeid(int).name() ) tipodati =     
    //else if( typeid(valore).name() == typeid(int).name() ) tipodati =     
    //else if( typeid(valore).name() == typeid(int).name() ) tipodati =    
    else
    {
        std::cerr << " ERRORE: tipo di dato sconosciuto in writeCell " << std::endl ;
	std::cerr << " comando di scrittura ignorato !!!!! "  << std::endl ;
	std::cerr << " rownum : " << rownum << " valore: " << valore 
	          << " col.name: " << nomecol << std::endl ; 
	std::cerr << " tipo dati:  " << typeid(valore).name()  << std::endl ; 

        #ifdef DEBUG_WRITECELL
          std::cerr << " writeCell: rownum: "<<rownum <<" value: " <<valore 
	            <<" name: " << nomecol   << " tipo fits: "
	            << tipodati << " tipo: " << typeid(valore).name() << std::endl  ;    
        #endif

        return 1 ;
    } 	

    #ifdef DEBUG_WRITECELL
     std::cerr << " writeCell: rownum: "<<rownum <<" value: " <<valore 
          <<" name: "<<nomecol  << " tipo fits: "<< tipodati 
	  << " tipo: " << typeid(valore).name() << std::endl  ;    
    #endif

    status_fits=0 ; 
    fits_get_colnum(fptrout,CASEINSEN,nomecol,&colnum,&status_fits) ;
    if(status_fits)
    { 
      std::cerr << std::endl <<" WRITECELL MAYBE WRONG COL : " 
                << nomecol << " value: " << valore 
	        << " colnum: " << colnum << std::endl ;      
    }
    status_fits=0 ; 
    fits_write_col(fptrout,tipodati,colnum,rownum,firstelem, (long)1, &valore, &status_fits);
    if(status_fits)   
    {
      std::cerr << std::endl<< " WRITECELL fits_write_col WRONG: colname: " 
                << nomecol<< " row: " << rownum  << " colnum: " << colnum << std::endl 		
		<< " first element: " << firstelem << " value: " << valore 
		<< " value type: "  << typeid(valore).name() 
		<< " fitsio type: " << tipodati                << std::endl  ; 
		char errtext[40] ; // fitsio error text : max per fitsio  30 char + '/0'
		fits_get_errstatus( status_fits, errtext);  
		std::cerr << " fitsio err message: " << errtext << std::endl  ;	              
   }
   return 0;	// MM added 20 Aug. 2008 to avoid warning 
 } 

#endif



#include "useful.h"

class fitsFile
{

    public:

           fitsFile(std::string outpha,std::string phatemplate) ;
	   void print(std::string nomefile) ; 
           void chiudi() ; 

           void addComment(std::string commento,std::string hduname) ;
	   void addComment(std::string commento) ;
           void addHistory(std::string commento,std::string hduname) ;
	   void addHistory(std::string commento) ;

	   float getFloatKey(std::string keyname) ;
	   int   getIntKey(std::string keyname) ;
	   void  setFloatKey(std::string keyname, float value,std::string hduname) ;
	   void  setIntKey(std::string keyname, int value,std::string hduname) ;
	   void  setFloatKey(std::string keyname, float value) ;
	   void  setIntKey(std::string keyname, int value) ;
	   void  setStringKey(std::string keyname, char *value,std::string hduname) ;
	   void  setStringKey(std::string keyname, char *value) ;
	   
	   
    protected:

       //int status_fits  ;    // now local to each routine 
       fitsfile * fitsPointer    ;  
       
       int fitsRowNum ; 
  
       std::string fileName ; 
       std::string templatename ; 

       void errorefits(int status_fits,int colnum,int rownum,
                       char * messaggio) ;  
};

#endif 
