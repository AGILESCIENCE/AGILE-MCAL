//
// C++ Interface: MCVbsrInfo
// 
// Description: 
//
//
// Author: Martino Marisaldi; marcello galli,,+39 51 6098312 <marcello.galli@bologna.enea.it>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
// file : MCVbsrInfo.h
// last update: 29-mar-2006
//
// ----------------------------------------------------------
//! Class for level1 fits file elaboration coming from PDHU

/*!
MCVbsrInfo  contains a description of a 3908 L1 fits file  
*/
// file : MCVbsrInfo.h 
// last update: 28-mar-2006
//
// ----------------------------------------------------------

#ifndef MCVLib_MCVbsrInfo
#define MCVLib_MCVbsrInfo

struct MCVbsrInfo  
{                       //! description of  L1 3908, extname: PACKETS and RATEMTRS
    int numpkt ;        //! num of packets records 
    int numbsr ;        //! num of bsr (ratemeter) records) 
    
};

struct MCALpkt 
{                              //! description of  L1 3908, extname: PACKETS 

    int paktnumb  ;      //!  packet keyword name: PAKTNUMB ( packet  number)
    int pakseq   ;       //! packet keyword name: PKTSEQCN  ( packet sequence number)
    int pakid ;          //! packet keyword name: PACKETID ( packet identifier in the conctact)
    int numpackets;      //! packet keyword name: BSRTPKYS (tot num of packets)
    int numbsr ;         //! packet keyword name: BSRBLCKS (num of bsr blocks)
    int bebnum ;         //!packet keyword name: BEBNUM (num of burst blocks)
    bool mcal ;
    bool sa ; 
    bool mcalsubm ;     //!  packet keyword name: MCSMSFLG , type: 1B 
    bool sasubm   ;     //!  packet keyword name: SASMSFLG , type: 1B 
    bool mcal16ms ;     //!  packet keyword name:  MCAL16MS
    bool mcal1ms ;      //!  packet keyword name: MCAL1MS
    bool sa16ms ;       //!  packet keyword name: SA16MS
    bool sa1ms ;        //!  packet keyword name:  SA1MS 
    double bstart ;     //!    packet keywords: BSTARTHI + BSTARTLO 
    double bstop ;      //!    packet keywords: BSTOPHI + BSTOPLO  
    
    
} ;   
    
struct MCALbsr
{                                   //!   L1 3908, extname:  RATEMTRS
    int pktnum  ;         //!  packet keyword name: PAKTNUMB ( packet  number)
    int evnum ;           //!   packet keyword name:EVNUMBER 
    int saormcal ;        // 0=sa = false ; 1 = mcal = true 
    
    unsigned int nume ;    //!  packet keyword name:RTMTIDCH
    unsigned int numg ;    //!  packet keyword name:RTMTIDCH
    unsigned int numt ;    //!  packet keyword name:RMTIDSIT
    unsigned int dr ;      //!  packet keyword name:BSCIRTMT
    unsigned int br ;      //!  packet keyword name:BKGERTMT
    
    double time ;          //!  packet keyword name:TIME

    unsigned int numch ;   //! num ratemeter channel 

} ;

#endif
 