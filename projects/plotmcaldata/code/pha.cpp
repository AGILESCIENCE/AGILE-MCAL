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
#include "plotmcaldata.h"
#include "version.h"

#include "phaFile.h" 
#include "EChannels.h" 

using namespace std ; 


int savePhaP1P2(int nch2, float *spP1, float *spP2, float dtime)
{
	// by M. Galli, copiata e adattata by M. Marisaldi 8 Oct. 2007

	string outFileName = "spectrum.out"  ;
	string phatemplate = "phatemplate.txt" ;
	
	string outphaP1 = "!phaP1.fits" ;  // spectrum for plane 1 
	string outphaP2 = "!phaP2.fits" ;  // spectrum for plane 2 
  
	EChannels * Ech = new EChannels("canali_sciRM.dat") ;     // Energy channel limits
	
	phaFile * phaP1 = new phaFile(outphaP1,phatemplate); // for plane 1 (upper or X)
	phaFile * phaP2 = new phaFile(outphaP2,phatemplate); // for plane 2 (lower or Z)

   
	int nch1= Ech->getnChannels() ; // num of detector channels ; (num of out Ei values)
	int nch = nch1 < nch2 ? nch1 : nch2;
 
	//  sets the num of E bins in the SPECTRUM extension
	
	std::string hduname="SPECTRUM" ;
	
	std::string keyname1="DETCHANS" ; int value1=nch ;     // max num of channels
	std::string keyname2="TLMIN001" ; int value2=1   ;     // first ch. num
	std::string keyname3="TLMAX001" ; int value3=nch ;     // last ch num 
	std::string keyname4="EXPOSURE" ; float value4=dtime ;     // grb integration time 
	
	phaP1-> setIntKey(keyname1, value1, hduname) ;
	phaP1-> setIntKey(keyname2, value2, hduname) ;
	phaP1-> setIntKey(keyname3, value3, hduname) ;
	phaP1-> setFloatKey(keyname4, value4, hduname) ;
	
	phaP2-> setIntKey(keyname1, value1, hduname) ;
	phaP2-> setIntKey(keyname2, value2, hduname) ;
	phaP2-> setIntKey(keyname3, value3, hduname) ;
	phaP2-> setFloatKey(keyname4, value4, hduname) ;
	
	for ( int i=0 ; i < nch ; i++ )   // for each E channel ( plane 1)
	{
		// i+1: fits channels begin from 1, not 0 , as all arrays in C,C++,java etc.
		int channel=i+1 ; 
		phaP1->OutEline( channel, spP1[i]>0. ? (int) spP1[i] : 0) ;     
		phaP2->OutEline( channel, spP2[i]>0. ? (int) spP2[i] : 0) ;      
	}
		
	phaP1->chiudi() ; 
	phaP2->chiudi() ; 
	
	return 0;

}

//
//--------------------------------------------------------------------
//

int savePha(int nch2, float *spP1, float dtime, TString *fname)
{
	// by M. Galli, copiata e adattata by M. Marisaldi 10 Oct. 2007

	string phatemplate = "phatemplate.txt" ;
	
	EChannels * Ech = new EChannels("canali_sciRM.dat") ;     // Energy channel limits
	
	phaFile * phaP1 = new phaFile(fname->Data(),phatemplate); // for plane 1 (upper or X)
   
	int nch1= Ech->getnChannels() ; // num of detector channels ; (num of out Ei values)
	int nch = nch1 < nch2 ? nch1 : nch2;
 
	//  sets the num of E bins in the SPECTRUM extension
	
	std::string hduname="SPECTRUM" ;
	
	std::string keyname1="DETCHANS" ; int value1=nch ;     // max num of channels
	std::string keyname2="TLMIN001" ; int value2=1   ;     // first ch. num
	std::string keyname3="TLMAX001" ; int value3=nch ;     // last ch num 
	std::string keyname4="EXPOSURE" ; float value4=dtime ;     // grb integration time 
	
	phaP1-> setIntKey(keyname1, value1, hduname) ;
	phaP1-> setIntKey(keyname2, value2, hduname) ;
	phaP1-> setIntKey(keyname3, value3, hduname) ;
	phaP1-> setFloatKey(keyname4, value4, hduname) ;
	
	for ( int i=0 ; i < nch ; i++ )   // for each E channel ( plane 1)
	{
		// i+1: fits channels begin from 1, not 0 , as all arrays in C,C++,java etc.
		int channel=i+1 ; 
		phaP1->OutEline( channel, spP1[i]>0. ? (int) spP1[i] : 0) ;     
	}
		
	phaP1->chiudi() ; 
	
	return 0;

}
