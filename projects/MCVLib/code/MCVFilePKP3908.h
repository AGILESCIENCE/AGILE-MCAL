
//
// Last update:  3-july-2006
// C++ Interface: MCVFilePKP3908
//
// Description: 
//
//
// Author: Martino Marisaldi; marcello galli,,+39 51 6098312 <marcello.galli@bologna.enea.it>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
// file : MCVFilePKP3908.h , m.galli, june 2006 
//
// ----------------------------------------------------------

#ifndef MCVLib_MCVFilePKP3908
#define MCVLib_MCVFilePKP3908

//                        MCVFilePKP.h includes also the root headers 
#include <TString.h> 

#include "MCVFilePKP.h"
#include "MCVBurst.h"

#include "MCVHisto3908.h"
//#include "MCVbsrInfo.h" 

#define MAX_BURST 100

//! Class for level1 fits file elaboration coming from PDHU

/*!
MCVFilePKP3908  is derived from class MCVFilePKP and adds some functions to read
a 3908 L1 fits file  
*/

class MCVFilePKP3908 : public MCVFilePKP, public MCVHisto3908 
{

    protected:
        
    public:

        MCVFilePKP3908();
        MCVFilePKP3908(const char *name);
        ~MCVFilePKP3908();         //virtual 
        
        // virtual void CreateRootFile()    //!< This  is in MCVFILEPKP3908
        void CreateRootFile3908();   //!< Custom Method for ROOT file creation 

	void PrintBurstInfo();
        void Process3908bsrData() ;     //!< Process BURST data ( 39.08 TM packet, hdu: RATEMTRS)
        void Process3908pktData() ;     //!< Process BURST data ( 39.08 TM packet, hdu: PACKETS)
        void Process3908Data() ;        //!< Process BURST bar data ( 39.08 TM packet, hdu: EVENTS)
        void ProcessTEData() ;          //!< Process BURST bar data ( TE style, for simulations by M.Galli, hdu: AGILE_Binary)

        void Resize_bsr_ric()  ;     // set max range for bins in bsr_ric histo                                           //!< similar to  Process3909Data
        void Resize_bsr_ric(double tinizio, double tfine)  ; // user given range 
	           
        void PrintEvent3908(int nevt) ;
	void UpdateBurst(int, MCALbsr *);
               
        // MCALevt currevt ; // e' definito in MCVFile.h ;
        MCVbsrInfo bsrinfo ;           //! for header specific to bsr data 
        MCALbsr    currbsr ;           //!  struct holding current bsr data 
        MCALpkt    currpkt ;           //! struct holding curent packet data 
	int	   pktnumb;	       //! number of packets
	int	  *burstid;	       //! 
	double     inittime;	       //! first event time
	
	TObjArray *burstarray;
	Int_t      nburst;
        
        TTree * tdatabsr ;             //!< Pointer to the data tree

        ClassDef(MCVFilePKP3908,0); //!< required for ROOT dictionary generation using rootcint
        //ClassDef(MCVFilePKP3908,1);  // 0 means no streamer for this class ???

    private:

} ;

#endif