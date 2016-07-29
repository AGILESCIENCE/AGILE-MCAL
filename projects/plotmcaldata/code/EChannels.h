//  last update: 27-sept-2007
//  -------------------------------------------
//  file EChannels.h 
//  m.galli, aug 2007
//  --------------------------------------------
//  container class for the energy limits of the 
//  detector channels Ei
//  
//  format 
//  ch_number  Elower Eupper(Gev)
//  ........... 
// 
//  --------------------------------------------------------------
//  => !! ch_number in input file BEGINS WITH 1, not with 0 !! <= 
//        But, inside this routine all vectors begin by 0 .... 
//  --------------------------------------------------------------
//  the lower bound is inside the ch, the upper bound is in the next ch
//   
//  lines beginning with # are comments, blank lines are ignored   
//  there is no need to have lines ordered by energy
//  if E limits overlaps for some channel, the first is used  
//    
//  ----------------------------------------------------------
//

#ifndef DEFINED_ECHANNELS           
#define	DEFINED_ECHANNELS	1

#define DEBUG_ECHANNELS    1         // fa stampe di debug 

#include <string>
#include <iostream>      
#include <fstream>     
#include <iomanip>	 	
#include <sstream> 
#include <stdlib.h>
#include <vector>
#include <typeinfo>
#include <string.h>
#include "useful.h"

class  EChannels
{
    public:
        EChannels(std::string filenamein) ; //reads input file wuth channel data
	~EChannels() ;
	int findChannel(float E) ;   // finds the channel in wich a given energy falls
	inline int getnChannels() { return nCh ; }   // number of Ei channels
	                                             // returns -1 if no channel found 

	inline float getEupper( int channel ){ return Eupper[number[channel]] ; }   					     
	inline float getElower( int channel ){ return Elower[number[channel]] ; } 
	inline float getdE    ( int channel ){ return dE[number[channel]] ; } 
	  					     
						               
        bool testChannels() ; 

    private:
    
        std::string fileName ; 
	int nCh                    ;  // num of E channels
	std::vector<float>  Elower ;  // lower E
	std::vector<float>  Eupper ;  // upper E 
	std::vector<int>    number ;  // number of channel
	std::vector<float>  dE     ;  // E channel E interval: Eupper-Elower 
} ;

#endif        // su ifdnef DEFINED_ECHANNELS
