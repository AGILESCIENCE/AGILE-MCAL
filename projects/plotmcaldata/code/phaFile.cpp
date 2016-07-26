//  last update: 12-sept-2007 
//  ---------------------------------------------------------------
//  file phaFile.cpp 
//  m.galli, aug 2007
//
//  pha fits file class (needs cfitsio)
//
// ----------------------


#include "phaFile.h"

//#include "fitsio.h" // lo include phaFile.h

//  grparser.c : 
//  parte delle cfitsio: parser del template, con stamnpe in piu'
//  per capire cosa succede quando il parser si pianta. Altrimenti le
//  cfitsio non dicono molto e non se ne viene a capo.
//  Serve solo per i test, poi si puo' usare la versione nelle cfitsio
//#include "grparser.c"


phaFile::phaFile(std::string filenamein,std::string Templatein):
         fitsFile(filenamein,Templatein)
{

/*********
     #ifdef DEBUG_PHAFILE      
        std::cerr << " phaFile::phaFile creating a new fits file : "
                  << fileName << std::endl ;
        std::cerr << " from template : " << templatename << std::endl ;  
     #endif   
    
     
     char errtext[40] ; // fitsio error text : max per fitsio  30 char + '/0'
     int esiste_fits ; 
     int status_fits=0;         // fitsio error flag 
     fitsRowNum=0 ;
     
     const char * nomefits = fileName.c_str() ; 
     const char * nometemp = templatename.c_str() ;
     
     if( fits_file_exists(nomefits,&esiste_fits, &status_fits) )
     {  
        fits_get_errstatus( status_fits, errtext);
        std::cerr << "phaFile::phaFile Errore in test esistenza, fitsio number: " 
	          << status_fits << std::endl ; 
        std::cerr << errtext << std::endl  ;
     }
 
     if(esiste_fits != 0 )
     {
        std::cerr << "phaFile::phaFile Attenzione, il file: " 
	          << nomefits << " esiste gia' ..." << std::endl ; 
        exit (11) ;
     }   
 
     if( fits_create_template(&fitsPointer ,nomefits ,nometemp, &status_fits) )
     {  
        fits_get_errstatus(status_fits, errtext);
        std::cerr << " Errore in create template, fitsio number: " << status_fits << std::endl ; 
        std::cerr << errtext << std::endl  ;
	std::cerr << "template name: " << nometemp << std::endl  ;
	std::cerr << "fits name: " << nomefits << std::endl  ;
	exit (55) ; 
     }
**********/
 
    Elower.reserve(100) ; 
    Eupper.reserve(100) ;
    channel.reserve(100) ;
    //countsP1.reserve(100) ;
    //countsP2.reserve(100) ;
    counts.reserve(100) ;
        
}  
void phaFile::setEline( float Elow, float Eup,int ch, int countsP1in, int countsP2in)
{  
    Elower.push_back(Elow ); 
    Eupper.push_back(Eup) ;
    channel.push_back(ch) ; 
    counts.push_back(0) ;
    //countsP1.push_back(countsP1in) ;
    //countsP2.push_back(countsP2in) ; 
}   
void phaFile::setEline( float Elow, float Eup,int ch, int countsin)
{
    Elower.push_back(Elow ); 
    Eupper.push_back(Eup) ;
    channel.push_back(ch) ;
    counts.push_back(countsin) ;
    //countsP1.push_back(0) ;
    //countsP2.push_back(0) ; 
    
}
void phaFile::OutEline( int channel, int counts) 
{
    #ifdef DEBUG_PHAFILE      
        std::cerr << " phaFile::OutEline writing line: ch,counts: "
	          << channel << " " << counts   << std::endl ;
    #endif   
   
  //            writes the fits values 
  // ------------------------------------------- 
  //            Extension 1 (hdu 2) : SPECTRUM
  // ------------------------------------------- 

    int hdutogo = 2 ; 
    int hdutype_fits ;
    int status_fits=0 ;
 
    if (fits_movabs_hdu(fitsPointer,hdutogo,&hdutype_fits,&status_fits) )
    {
       std::cerr << "phaFile:: error going to fits hdu: " << hdutogo 
                 << " fits status: " << status_fits << std::endl ; 
    }   
 
    writeCell(fitsPointer,fitsRowNum,channel ,"CHANNEL") ; 
    writeCell(fitsPointer,fitsRowNum,counts  ,"COUNTS") ; 

    /************* not yet implemented 
    writeCell(fitsPointer,fitsRowNum,,"STAT_ERR") ; 
    writeCell(fitsPointer,fitsRowNum,,"SYS_ERR") ; 
    writeCell(fitsPointer,fitsRowNum,,"QUALITY") ;
    writeCell(fitsPointer,fitsRowNum,,"STAT_ERR") ; 
    writeCell(fitsPointer,fitsRowNum,,"GROUPING") ; 
    writeCell(fitsPointer,fitsRowNum,,"AREASCAL") ;
    writeCell(fitsPointer,fitsRowNum,,"BACKSCAL") ;
    ***************/    
 
    fitsRowNum ++ ; 


}
void phaFile::print(std::string nomeoutfile) 
{
  // scrive lo spettro, messo demtro linea per linea con setline,
  // su un dato file 
 
  std::ofstream * OUT = new std::ofstream ; 
  
  OUT->open(nomeoutfile.c_str()) ;
  
  if( ! *OUT  ) 
   {  
      myuse::errore(9," File open error in phaFile::print ",false) ; 
      exit(9) ;
   }  
  *OUT << "# channel,  Elower,  Eupper,  counts "
       << std::endl ;
 
  OUT->setf(std::ios::right) ;      // nei campi di output allinea il contenuto a dx
  OUT->setf(std::ios::showpoint) ;  // mette sempre il punto dei decimali 

  int nCh=Elower.size() ; 
  for ( int i=0 ; i < nCh ; i++ )
  {
     *OUT  << std::setw(4)  << channel[i]                       << " " 
           << std::setw(12) << std::setprecision(6)<< Elower[i] << " " 
           << std::setw(12) << std::setprecision(6)<< Eupper[i] << " " 
	   << std::setw(7)  << counts[i]                        << std::endl ; 
  } 
  
  OUT->close() ;
}
/***********
void phaFile::chiudi() 
{
   int status_fits=0 ;
   fits_close_file(fitsPointer, &status_fits) ;
} 
*************/ 


