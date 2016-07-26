//  last update: 27-sept-2007 
//  ---------------------------------------------------------------
//  file EChannels.cpp 
//  m.galli, aug 2007
//
//  Container class for the enery limits of the detector
//  channels, which are read from an input file 
// ----------------------


#include "EChannels.h"

EChannels::EChannels( std::string nomefile ):fileName(nomefile),nCh(-1)
{
   std::string stringain ; 
   std::string::size_type index ; 
   
   int numberin ;
   float Elowerin,Eupperin ; 

   std::ifstream INFILE              ;     // iput file     
   INFILE.open(fileName.c_str()) ;      
 
   if( ! INFILE  ) 
   {  
      myuse::errore(3," File open error in EChannels::EChannels ",true) ; 
      exit(3) ;
   }  
   
   Elower.reserve(100) ; // un po' di spazio iniziale per il vector
   Eupper.reserve(100) ; 
   number.reserve(100) ;
   dE.reserve(100) ;
   
   while ( std::getline(INFILE,stringain,'\n') )
   {
       if(stringain.empty() ) continue ;          // linea vuota 
       if(stringain[0]=='#')  continue ;          // riga che inizia con # e' un commento 
       index=stringain.find_first_not_of(" \t") ; // primo char non blank o tab
       if(index == std::string::npos ) continue ; // linea bianca: npos:ret code di notfound 
       if(stringain[index]=='#') continue ;       // prima non blank: '#' : commento
  
       std::istringstream STRINGASTREAM; // faccio uno stream per una stringa
       STRINGASTREAM.str(stringain) ;    // metto la linea (stringa letta) nello stream
     
       if( ! (STRINGASTREAM >> numberin >> Elowerin >> Eupperin ) ) // leggo dallo stream
       {
          myuse::errore(123," read error in EChannels::EChannels  , line skipped",false) ; 
	  std::cerr << "wrong data: "<<numberin<<" "<<Elowerin<<" "<<Eupperin<< std::endl ;
	  continue ; 
       }
      
       Elower.push_back(Elowerin) ; 
       Eupper.push_back(Eupperin) ; 
       number.push_back(numberin-1) ; // CH begins from 1, ARRAYS HRE FROM 0 !!! 
       
       dE.push_back(Eupperin-Elowerin) ;
   }

   nCh=Elower.size() ; 
 
   if(nCh < 1 ) 
   {
        myuse::errore(nCh," EChannels::EChannels file with no channels",true) ; 
	return ;
   }

  #ifdef DEBUG_ECHANNELS 
     for (int jj=0; jj< nCh ; jj++ ) 
     std::cerr<< " EChannels::EChannels " << jj << " num: "<< number[jj] 
              <<" Elower: " << Elower[jj] << " Eupper: " << Eupper[jj] << std::endl ; 
  #endif 

  if( ! testChannels() )
  {
        myuse::errore(nCh," EChannels::EChannels not ordered by energy !",true) ; 
	return ;
  }    
}
int EChannels::findChannel(float E) 
{
   for( int i=0 ; i<nCh ; i++)
   {
       if( E >= Elower[i] && E < Eupper[i] ) return number[i] ;    
   }
   return -1 ; 
}
EChannels::~EChannels()
{


}
bool EChannels::testChannels() 
{
   bool tuttook=true ; 
   int wrongch=-1    ;  // first wrong channel 
   
   if(Elower[0] > Eupper [0])
   {
      tuttook=false ;
      wrongch=0 ;
      myuse::errore(wrongch," EChannels::EChannels not ordered by energy !",false) ;
   }
   for( int i=1 ; i<nCh ; i++)
   {
      if ( (Elower[i] != Eupper [i-1]) ||
           (Elower[i] >= Eupper [i]  )    )
      {
             tuttook=false ;
             wrongch=i ; 
             myuse::errore(wrongch," EChannels::EChannels not ordered by energy !",false) ;
  
      }   
   }
   return tuttook ; 
}
