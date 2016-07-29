// last update: 9-aug-2007 
// file: useful.h (reduced version for RESPgen)
// some useful routines,
// most templates,  be included here and there
//  

#ifndef DEFINED_USEFUL 
#define DEFINED_USEFUL 1

#include <iostream> 
#include <iomanip> 
#include <string>
#include <stdlib.h> 

namespace myuse
{
   template <typename TIPO> void zero(int n, TIPO * a)
   {
      TIPO b=(TIPO) 0 ;      
      for (int i=0;i<n;i++) a[i]=b ; 
   }
   
   template <typename TIPO> void delarray (int n,TIPO**a)
   {
      if(a) 
      {
         for (int i=0;i<n;i++) if(a[i]) delete[] a[i] ;
         delete[] a ;
      }
   }     
   
   template <typename TIPO> int indexOf(TIPO nome, TIPO * array,int n )
   {
       // index Of Name In  Array 
       int i ;
       for(i=0; i<n ; i++) if( nome == array[i]) return i ;
       return -1 ; 
   }
 
   int indexOfString(std::string & nome, std::string * array,int n );

   void errore(int errorNumber,const char * messaggio,bool fatal);
  
   template <typename TIPO> inline bool testBounds(TIPO i,TIPO i1,TIPO i2)
   {
       return ( i >= i1 && i < i2 )  ; 
   }
 

}

#endif	
