// last update: 19-mar-2007 
// file: useful.cpp (reduced version for RESPgen)
// some useful routines,
// templates are in the useful.h file 
// (multiple copies of the templates are eliminated by the linker)
//

#include "useful.h" 
  
namespace myuse
{
   void errore(int errorNumber,const char * messaggio,bool fatal)
   {
      std::cerr << "Error! Reported number: " << errorNumber
                << " Message: "  << messaggio                  
                << std::endl ; 
      if(fatal) 
      { 
         std::cerr << "Fatal error. Program cant go on!" << std::endl ;
         exit(errorNumber) ; 
      }
    }  
    
   int indexOfString(std::string & nome, std::string * array,int n )
   {
       // index Of Name In  Array of strings passed by reference 
       
       for( int i=0; i<n ; i++) if( nome == array[i]) return i ;
       return -1 ; 
   }
   

}  // end of myuse namespace
