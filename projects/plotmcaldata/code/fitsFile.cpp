//  last update: 13-sept-2007 
//  ---------------------------------------------------------------
//  file fitsFile.cpp 
//  m.galli, sept 2007
//
//  fits file class (needs cfitsio)
//
// ----------------------


#include "fitsFile.h"

//#include "fitsio.h" // lo include FITSFile.h

//  grparser.c : 
//  parte delle cfitsio: parser del template, con stamnpe in piu'
//  per capire cosa succede quando il parser si pianta. Altrimenti le
//  cfitsio non dicono molto e non se ne viene a capo.
//  Serve solo per i test, poi si puo' usare la versione nelle cfitsio
//#include "grparser.c"


fitsFile::fitsFile(std::string filenamein,std::string Templatein):
                 fileName(filenamein),templatename(Templatein)
{
     #ifdef DEBUG_FITSFILE      
        std::cerr << " fitsFile::fitsFile creating a new fits file : "
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
        std::cerr << "fitsFile::fitsFile Errore in test esistenza, fitsio number: " 
	          << status_fits << std::endl ; 
        std::cerr << errtext << std::endl  ;
     }
 
     if(esiste_fits != 0 )
     {
        std::cerr << "fitsFile::fitsFile Attenzione, il file: " 
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
          
}
void fitsFile::errorefits(int status_fits,int colnum,int rownum,char * messaggio) 
{
      char errtext[40] ; // max per fitsio  30 char + '/0' 

      std::cerr << std::endl ;
      std::cerr << " fitsFile class: FITS ERROR ! "<< std::endl ;
      std::cerr << messaggio << std::endl ;
      std::cerr << " colnum: " << colnum << " row: "<< rownum
	        << " fits status: " << status_fits << std::endl ;       	      
      fits_get_errstatus( status_fits, errtext);	  
      std::cerr << " fits says: " << errtext <<  std::endl ;         
    
      exit(299) ;  
}
  
void fitsFile::chiudi() 
{
   int status_fits=0 ;
   fits_close_file(fitsPointer, &status_fits) ;
}
void fitsFile::addComment(std::string commento,std::string hduname)
{    
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,hduname.c_str(),80) ; 
 
    if (fits_movnam_hdu(fitsPointer,ANY_HDU,nome,0,&status_fits))    
    {
       errorefits(status_fits,0,
                         0," addComment: hdu moving");
    }   
    addComment(commento) ; 
}
void fitsFile::addComment(std::string commento)
{
    int status_fits ; 
    fits_write_comment(fitsPointer,commento.c_str(),&status_fits);
    if(status_fits)   
    {
   	errorefits(status_fits,0,0," Routine fitsFile::addComment ") ; 
    }
}
void fitsFile::addHistory(std::string commento,std::string hduname)
{
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,hduname.c_str(),80) ; 
 
    if (fits_movnam_hdu(fitsPointer,ANY_HDU,nome,0,&status_fits))
    {
       errorefits(status_fits,0,
                   0," addHistory: hdu moving") ; 
    }   
    addHistory(commento) ; 
}
void fitsFile::addHistory(std::string commento)
{
    int status_fits ; 
    fits_write_history(fitsPointer,commento.c_str(),&status_fits);
    if(status_fits)   
    {
   	errorefits(status_fits,0,0," Routine fitsFile::addHistory ") ; 
    }
}
// ############### Mettere qui sotto dei templates !!!!!!!!!!

void  fitsFile::setIntKey(std::string keyname, int value,std::string hduname) 
{    
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,hduname.c_str(),80) ; 
 
    if (fits_movnam_hdu(fitsPointer,ANY_HDU,nome,0,&status_fits))
    {
       errorefits(status_fits,0,
                   0," setIntKey: hdu moving") ; 
    }   
    setIntKey(keyname,  value ) ; 
}
void  fitsFile::setIntKey(std::string keyname, int value) 
{
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,keyname.c_str(),80) ; 
    
    fits_update_key(fitsPointer,TINT,nome,&value,NULL,&status_fits);
    if(status_fits)   
    {
   	errorefits(status_fits,value,0," Routine setIntKey ") ; 
    }    
}
void  fitsFile::setFloatKey(std::string keyname, float value,std::string hduname) 
{
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,hduname.c_str(),80) ; 

    if (fits_movnam_hdu(fitsPointer,ANY_HDU,nome,0,&status_fits))
    {
       errorefits(status_fits,0,
                   0," setFloatKey: hdu moving ") ; 
    }   
    setFloatKey( keyname,  value ) ; 
}
void fitsFile::setFloatKey(std::string keyname, float value) 
{
    int status_fits=0 ; 
    char nome[80] ;
    strncpy(nome,keyname.c_str(),80) ; 

    fits_update_key(fitsPointer,TFLOAT,nome,&value,NULL,&status_fits);
    if(status_fits)   
    {
   	errorefits(status_fits,0,0," Routine setFloatKey ") ; 
    }    
}
int fitsFile::getIntKey(std::string keyname) 
{
    int value ;
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,keyname.c_str(),80) ; 
    
    fits_read_key(fitsPointer,TINT,nome,&value,NULL,&status_fits);

    if(status_fits)   
    {
   	errorefits(status_fits,value,0," Routine getIntKey ") ; 
    }       
    return value ;  
}
float fitsFile::getFloatKey(std::string keyname) 
{
    float value ;
    int status_fits=0 ;
    char nome[80] ;
    strncpy(nome,keyname.c_str(),80) ; 
   
    fits_read_key(fitsPointer,TFLOAT,nome,&value,NULL,&status_fits);

    if(status_fits)   
    {
   	errorefits(status_fits,0,0," Routine getIntKey ") ; 
    }       
    return value ;  
}
// ############### Mettere qui sopradei templates !!!!!!!!!!
    
