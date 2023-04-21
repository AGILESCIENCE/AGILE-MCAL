
// last update: 3-july-2006
// C++ Interface: MCVHisto3980
//
// Description: 
//
//
// Author: Martino Marisaldi; marcello galli,,+39 51 6098312 <marcello.galli@bologna.enea.it>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
// file : MCVHisto3908.h  M.galli, june 2006 
//
//  this class defines  the following plots or hists: 
//           
//    TH2F      * Et_mcal ; // 2 dim histo: ( num events : E event- time )(3908 EVENTS)
//             
//    TGraph* gdr_E2_mcal[6]    //!< MCAL plots for E2: unsegmented x 6 sits (3908 RATEMTRS) 
//    TGraph* gdr_mcal[2][4][6] //!< MCAL plots for E0,1 x 4 geom.segm. x 6 sits 
//    TGraph* gdr_sa  [2][4][6] //!< SA   plots for E0,1 x 4 geom.segm. x 6 sits 
//     
//    and bsr histo reconstructed from bar data:    (reconstructed from 3908 EVENTS)
//	     
//    TH1I* gdr_E2_mcal_ric[6]    //!< MCAL histo for E2: unsegmented x 6 sits  
//    TH1I* gdr_mcal_ric[2][4][6] //!< MCAL histo for E0,1 x 4 geom.segm. x 6 sits 
//    TH1I* gdr_sa_ric [2][4][6]  //!< SA histo for E0,1 x 4 geom.segm. x 6 sits 
//	     
//
//   UNITS for histo *_ric are:  msec, events/(sit int time) 
//   UNITS for TH2F           :  msec, events/msec 
//
//   WARNING! in L1 3908 time units are microsec !!! 
//   WARNING! in gams MC E units are Gev !!! 
// ----------------------------------------------------------

#include <iostream> 

//! MCVHisto3908 class: Histograms specific to the 3908 packet analisys 


#ifndef MCVLib_MCVHisto3908
#define MCVLib_MCVHisto3908

//#include "MCVFileInfo.h"
//#include "MCVFile.h"

#include "TObjArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors.h"

//#include "MCVbsrInfo.h" 
#include "MCVFile.h"

class MCVHisto3908
{
    protected: 
	                     // sit integration times:1,16,64,256,1024,8192 msec 
	static const int nsit = 6 ;      //! number of  sits = 2 hard+4 soft
	//static const int firstsoftsitnum = 2 ; //! number of the first soft SIT 
        static const int ngeom  = 4 ;      //! num of geom.  parts of the detector   
        static const int nEsegm = 2 ; //! num of geom.  segmented energy ranges 
	                              //!                      (E0,E1 segm. E2 unsegm.)
        int sitmsecs[6] ; //!

    public:
		           // Note: ! exclude a member from the rootcint generated streamer
	                   
	TObjArray * rawEt ; //!< container class for E-t 2 dim bar events histograms  
	                   
	TObjArray * rawbsr;     //!< container class for bsr-time graph (9ch x 4 soft sits)   
	TObjArray * rawbsr_ric; //!< same for bsr-time histo (reconstructed from EVENTS))   
	     

	TH2F      * Et_mcal ;  //!< 2 dim E-t histograms for bars 
       //TH2F      * Et_sa ;   //!< 2 dim E-t histograms for sa
	     
        TGraph* gdr_E2_mcal[6]    ; //!< MCAL plots for E2: unsegmented x 4 sits  
	TGraph* gdr_mcal[2][4][6] ; //!< MCAL plots for E0,1 x 4 geom.segm. x 4 sits 
//	TGraph* gdr_sa  [2][4][6] ; //!< SA   plots for E0,1 x 4 geom.segm. x 4 sits 

        TH1I* gdr_E2_mcal_ric[6]     ; //!< MCAL histo for E2: unsegmented x 6 sits  
        TH1I* gdr_mcal_ric[2][4][6]  ; //!< MCAL histo for E0,1 x 4 geom.segm. x 6 sits 
//        TH1I* gdr_sa _ric [2][4][6]; //!< SA histo for E0,1 x 4 geom.segm. x 6 sits 
	     

	MCVHisto3908()  ;
       ~MCVHisto3908()  ; 
	void FillHisto3908( MCALevt *evt )     ;               // to fill: TH2F Et_mcal
	void FillHisto3908_ric( MCALevt *evt ) ;               // to fill: TH1I gdr*_ric 
	void FillGraph3908( int numpoint, MCALbsr * currbsr) ; // to fill: TGraph gdr*
        void FillGlobalHisto3908( )  ;  
		    
	ClassDef(MCVHisto3908,0); //!< required for ROOT dictionary generation using rootcint

     private: 
	       
	double t0bsr ;      //! primo t dei dati bsr ; i plot iniziano da t=0 
	double t0bsr_ric ;  //! primo t dei dati bsr_ric ; i plot iniziano da t=0 	
	double t0Et  ;        //! primo t dei dati E-t; i plot iniziano da t=0 

        double pesohistEt ;   //! weight of each event, to have correct z axix units
        double pesohistbsr ;  //! weight of each event, to have correct z axix units
	
	
	
				      
        // for hist and plot msec are used as time units (on L1 fits we have microsec)
	
        static const int nummsec = 60000 ; //! num of msec for E-t histo ( 1 minutes ) 
        static const long minsec  = 0.0      ; //! E-t: t range: from 0 to 10 minutes 
        static const long maxsec  = 600000.0 ; //! step : 10 msec 
     
//        static const int nummsec_ric ; //unused, the time step == sit integration time 
        static const long minsec_ric  = 0.0      ; //! E-t: t range: from 0 to 10 minutes 
        static const long maxsec_ric  = 600000.0 ; //! step : 10 msec 


        int numE ;                          //! num of E points 
        static const long stepE = 0.500 ; //! bin step: UNITS: MEV 
        static const long  minE  = 0.0   ; //! E-t: E range: from 0 to 100 Mev 
        static const long  maxE  = 100.0 ; //! 
     
//        static const double MCALEbsrlim0 = 0.701  ; //! MCAL bsr E range limits (Mev)    
//        static const double MCALEbsrlim1 = 1.4021 ; //! 

	int MCALElim0;
	int MCALElim1;

//        static double SAEbsrlim=0.020  ;    //! SA bsr E range limits (TBC)    
	
        int Findie( MCALevt *evt )     ; // finds the E bsr range for the event
        int Findig( MCALevt *evt )     ; // finds the geometry bsr zone for the event

        void DumpMCALEvent(MCALevt *evt ) ; // Prints the MCALevt struct 

} ;



#endif
