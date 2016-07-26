
// last update: 3-july-2006
// C++ Implementation: MCVHisto3908
//
// Description: 
//
//
// Author: Martino Marisaldi; marcello galli,,+39 51 6098312 <marcello.galli@bologna.enea.it>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
// file : MCVHisto3908.cpp 
//
// ----------------------------------------------------------

#include "MCVHisto3908.h"

//#include <fitsio.h>

ClassImp(MCVHisto3908);

MCVHisto3908::MCVHisto3908() 
{  
     
     int numpunti ; 
     char colname[30];
     
                     //  sit: 2hw+4soft = 1,16,64,256,1024,8192 msec 
                     // for mcal:(E2 + (E0,E1 * 4 geom))x6 sit = 9x6 = 54 
	             // for SA  :      (E0,E1 * 4 geom))x6 sit = 8x6 = 48
		     
     int numdrmcal = ( 1 + nEsegm * ngeom ) * nsit ;
//     int numdrsa   = (     nEsegm * ngeom ) * nsit  ;

     numE=(int) ((maxE-minE)/stepE) ; // num of Et histo E steps      
     
     sitmsecs[0] =    1 ;
     sitmsecs[1] =   16 ;
     sitmsecs[2] =   64 ;
     sitmsecs[3] =  256 ;
     sitmsecs[4] = 1024 ;
     sitmsecs[5] = 8192 ;
     
     MCALElim0 = 3;	// default low energy band upper limit (<0.7 MeV)
     MCALElim1 = 4;	// default medium energy band upper limit (<1.4 MeV)
     
     cerr <<" MCVHisto3908 defining histo and graphs for "<< numdrmcal <<" graphs"<<endl ; 
     
     
     rawEt      = new TObjArray(10,0) ;        // for TH2F   with  E-t  histo
     rawbsr     = new TObjArray(numdrmcal,0) ; // for Tgraph with  dr-t plots (from 3908) 
     rawbsr_ric = new TObjArray(numdrmcal,0) ; // for TH1I with reconstructed dr-t plots 

//                when SA will be included numdrmcal+numdrsa should be allocated 
  
     rawEt -> SetName("rawEt") ; 
     rawbsr-> SetName("rawbsr") ;
     rawbsr_ric-> SetName("rawbsr_ric") ;
               
    cerr<< " Et_mcal limits : numsec, min,max sec "<< nummsec << " " << minsec << " " << maxsec << endl ;    
    cerr<< " Et_mcal limits : numE, min, maxE "   << numE << " " << minE << " " << maxE << endl ;   
     
     t0bsr     = -10.0 ; // says that the initial time is uninitialized 
     t0bsr_ric = -10.0 ;     
     t0Et      = -10.0 ;
			          
    // ...... setting UNITS for histo .........
    // peso degli eventi: l'inverso dell'integrale dello step : 
    //    se ho n events in uno step di 10msec
    //    e voglio  ev/msec devo dividere per 10 per avere  event/msec 			             
    pesohistEt= 1./( (maxsec-minsec)/(float)nummsec * (maxE-minE)/(float)numE ) ; 
    //pesohistbsr=1.0/(sitmsecs[it]) ; // to have events/sit integration time
    
    cerr<< " initial time set as uninitialized: " 
        << t0bsr << " " << t0bsr_ric << " " << t0Et << endl ;
    cerr<< " Et histoevent weight : " << pesohistEt << endl ;
   
    Et_mcal = new TH2F("Et_mcal","Et_mcal",nummsec,minsec,maxsec,numE,minE,maxE) ; 
    Et_mcal->SetTitle(" Mcal Events, energy-time histogram") ; 
    Et_mcal->GetXaxis()->SetTitle("Time (msec)") ;
    Et_mcal->GetXaxis()->SetTitleSize(0.05);
    Et_mcal->GetXaxis()->SetLabelSize(0.05);
    Et_mcal->GetXaxis()->CenterTitle();
    
    Et_mcal->GetYaxis()->SetTitle("Energy (Mev)"); 
    Et_mcal->GetYaxis()->SetTitleSize(0.05);
    Et_mcal->GetYaxis()->SetLabelSize(0.05);
    Et_mcal->GetYaxis()->CenterTitle();
    Et_mcal->GetYaxis()->SetTitleOffset(1.5);
    
    Et_mcal->GetZaxis()->SetTitle("counts/msec/Mev") ; 
    Et_mcal->GetZaxis()->SetTitleSize(0.05);
    Et_mcal->GetZaxis()->SetLabelSize(0.05);
    Et_mcal->GetZaxis()->CenterTitle();
    Et_mcal->GetZaxis()->SetTitleOffset(1.5);
    
    rawEt->AddLast(Et_mcal);
    
    
    for (int it=0; it<nsit; it++) 
    {  
    
      numpunti=(int)
             (maxsec_ric-minsec_ric)/sitmsecs[it]; // *_ric depend on the sit integ. time

      // Mcal unsegmented burst ratemeters for the E2 Energy range 
      
      sprintf(colname, "DR_E2_T%d",sitmsecs[it]);  
     
      gdr_E2_mcal[it] = new TGraph(nummsec) ; // 60 sec, defined in MCVHisto.h
      gdr_E2_mcal[it]->SetName(colname);
      gdr_E2_mcal[it]->SetTitle(colname);
      gdr_E2_mcal[it]->GetXaxis()->SetTitle("Time (msec)");
//    gdr_E2_mcal[it]->GetXaxis()->SetTitleOffset(1.4);
      gdr_E2_mcal[it]->GetXaxis()->CenterTitle();
      gdr_E2_mcal[it]->GetYaxis()->SetTitle("Counts");
      gdr_E2_mcal[it]->GetYaxis()->CenterTitle();
      gdr_E2_mcal[it]->GetYaxis()->SetTitleOffset(1.5);

      rawbsr->AddLast(gdr_E2_mcal[it]);  

      sprintf(colname, "DR_E2_T%d_rec",sitmsecs[it]);  

      gdr_E2_mcal_ric[it] = new TH1I("colname","colname",numpunti,minsec_ric,maxsec_ric); 
      gdr_E2_mcal_ric[it]->SetName(colname);
      gdr_E2_mcal_ric[it]->SetTitle(colname);
      gdr_E2_mcal_ric[it]->GetXaxis()->SetTitle("Time (msec)");
//    gdr_E2_mcal_ric[it]->GetXaxis()->SetTitleOffset(1.45);
      gdr_E2_mcal_ric[it]->GetXaxis()->CenterTitle();
      gdr_E2_mcal_ric[it]->GetYaxis()->SetTitle("Counts/int.time");
      gdr_E2_mcal_ric[it]->GetYaxis()->CenterTitle();
      gdr_E2_mcal_ric[it]->GetYaxis()->SetTitleOffset(1.5);

      rawbsr_ric->AddLast(gdr_E2_mcal_ric[it]);  
     
      for (int ie=0; ie<nEsegm; ie++) {
      for (int ig=0; ig<ngeom ; ig++) {     
     
      sprintf(colname, "MCAL_DR_E%d_G%d_T%d",ie,ig,sitmsecs[it]);  
     
      gdr_mcal[ie][ig][it] = new TGraph(nummsec) ; 
      gdr_mcal[ie][ig][it]->SetName(colname);
      gdr_mcal[ie][ig][it]->SetTitle(colname);
      gdr_mcal[ie][ig][it]->GetXaxis()->SetTitle("Time (msec)");
//    gdr_mcal[ie][ig][it]->GetXaxis()->SetTitleOffset(1.45);
      gdr_mcal[ie][ig][it]->GetXaxis()->CenterTitle();
      gdr_mcal[ie][ig][it]->GetYaxis()->SetTitle("Counts");
      gdr_mcal[ie][ig][it]->GetYaxis()->CenterTitle();
      gdr_mcal[ie][ig][it]->GetYaxis()->SetTitleOffset(1.5);
      
      rawbsr->AddLast(gdr_mcal[ie][ig][it]);  

      sprintf(colname, "MCAL_DR_E%d_G%d_T%d_rec",ie,ig,sitmsecs[it]);  
     
      gdr_mcal_ric[ie][ig][it] = new TH1I("colname","colname",numpunti,minsec_ric,maxsec_ric) ; 
      gdr_mcal_ric[ie][ig][it]->SetName(colname);
      gdr_mcal_ric[ie][ig][it]->SetTitle(colname);
      gdr_mcal_ric[ie][ig][it]->GetXaxis()->SetTitle("Time (msec)");
//    gdr_mcal_ric[ie][ig][it]->GetXaxis()->SetTitleOffset(1.45);
      gdr_mcal_ric[ie][ig][it]->GetXaxis()->CenterTitle();
      gdr_mcal_ric[ie][ig][it]->GetYaxis()->SetTitle("Counts/int.time");
      gdr_mcal_ric[ie][ig][it]->GetYaxis()->CenterTitle();
      gdr_mcal_ric[ie][ig][it]->GetYaxis()->SetTitleOffset(1.5);
      
      rawbsr_ric->AddLast(gdr_mcal_ric[ie][ig][it]);  

//      ...............................................
//       SA unused, by now .......

//      sprintf(colname, "SA_DR_E%d_G%d_T%d",ie,ig,sitmsecs[it]);  
//     
//      gdr_sa[ie][ig][it] = new TGraph(nummsec) ; 
//      gdr_sa[ie][ig][it]->SetName(colname);
//      gdr_sa[ie][ig][it]->SetTitle(colname);
//      gdr_sa[ie][ig][it]->GetXaxis()->SetTitle("Time (s)");
////    gdr_sa[ie][ig][it]->GetXaxis()->SetTitleOffset(1.4);
//      gdr_sa[ie][ig][it]->GetXaxis()->CenterTitle();
//      gdr_sa[ie][ig][it]->GetYaxis()->SetTitle("Counts");
//      gdr_sa[ie][ig][it]->GetYaxis()->CenterTitle();
////    gdr_sa[ie][ig][it]->GetYaxis()->SetTitleOffset(1.5);
//
//      rawbsr->AddLast(gdr_sa[ie][ig][it]);  
           
     }}  // ie,ig 
    
    }  // it 

            
            
}
MCVHisto3908::~MCVHisto3908() 
{ 


}



void MCVHisto3908::FillGraph3908( int numpoint, MCALbsr * currbsr)
{ 
 // fills hist with the current event 

   int ie,ig,it ; 
   double timebsr ; 
    
   ie=currbsr->nume ; 
   ig=currbsr->numg ; 
   it=currbsr->numt ;
   
   
   if(t0bsr < 0.0 )   // sets the initial time (plot starts from 0)
   {
      t0bsr=currbsr->time ; 
      std::cerr << " Setting initial time: " << t0bsr << std::endl ; 
   }  
    
   timebsr= (currbsr->time - t0bsr)*1.E-3 ; // time bstr plot starting from 0,msec
      
   if(numpoint ) {	// < 4
     std::cerr << " FillGraph3908 ie,ig,it: " << ie << " " << ig << " " << it 
               << " point: "  <<  numpoint 
	       << " time: "   << timebsr 
	       << " dr: " << currbsr->dr << " br: " << currbsr->br 
	       << std::endl ; 
     std::cerr << " t0bsr " << t0bsr << " currbst->time: " << currbsr->time  
               << " diff: " << timebsr << std::endl ; 
	       
   }
   
   
   if( it >= nsit || it < 0 )
   {
      std::cerr << " Error in sit number,in MCVHisto3908::FillGraph3908 " << it << std::endl ; 
      exit(100) ; 
   }
   
   
   if(currbsr->saormcal)   // true if mcal bsr 
   {
      if(  ie == 2 ) // unsegmented E2 range 
      {
          gdr_E2_mcal[it]->SetPoint(numpoint, timebsr, (Double_t)currbsr->dr );
      }
      if (ie > 2) // hardware SIT or sub-ms
      {
      }
      else
      {
         if(  ie > 1 || ie <0 )
         {
	   std::cerr << " Error in E number,in MCVHisto3908::FillGrapg3908 " << ie << std::endl ; 
	   exit(101) ; 
	 }
	 if ( ig >= ngeom  || ig <0 )
         {
	   std::cerr << " Error in G number,in MCVHisto3908::FillGrapg3908 " << ig << std::endl ; 
	   exit(101) ; 
	 }
	 gdr_mcal[ie][ig][it]->SetPoint(numpoint,timebsr , (Double_t)currbsr->dr );
      } 
    }
    else    // sa bsr 
    {
        std::cerr << " Error, SA record found, but SA is not implemented ! " << std::endl ; 
        
//
//          if(  ie > 1 || ie <0 )
//         {
//	   std::cerr << " Error in E number,in MCVHisto3908::FillGrapg3908 " << ie << std::endl ; 
//	   exit(101) ; 
//	 }
//	 if ( ig >= ngeom  || ig <0 )
//         {
//	   std::cerr << " Error in G number,in MCVHisto3908::FillGrapg3908 " << ig << std::endl ; 
//	   exit(101) ; 
//	 }
//	 gdr_sa[ie][ig][it]->SetPoint(numpoint,timebsr , (Double_t)currbsr->dr );   
    }  
}
void MCVHisto3908::FillHisto3908( MCALevt *evt )
{ 
    double timeEt ;       

    //cerr << " Into FillHisto3908 "  ; 
    //DumpMCALEvent(evt );
    
    if(t0Et < 0.0 )   // sets the initial time (plot starts from 0)
    {
       t0Et = evt->time ; 
       std::cerr << " Initial time for E-t histo: " << t0Et << std::endl ; 
    }  
    timeEt=( evt->time - t0Et )  * 1.E3 ; // time from usec to  millisec
      
    if(timeEt < 10.0 ) 
    { 
       cerr << " Fillhisto3908 for event timed: "<<  timeEt<< " Etot: " << evt->Etot 
            << " weight: " << pesohistEt << endl ; 
    }	 
	              // E-t 2 dim histo, time : millisec ; E : 
    Et_mcal->Fill( timeEt ,(double) evt->Etot,pesohistEt) ;  
    return ; 
} 
void MCVHisto3908::FillHisto3908_ric( MCALevt *evt )
{ 
    double timebsr_ric ; 
    int ie,ig,it ;      

    // if(evt->time < 10.0 ) 
    // {
    //    cerr << " Into FillHisto3908_ric "  ; 
    //    DumpMCALEvent( evt ) ; 
    // }

    if(t0bsr_ric < 0.0 )   // sets the initial time (plot starts from 0)
    {
       t0bsr_ric=evt->time ; 
       std::cerr << "FillHisto3908_ric setting initial time for rec. histo: " 
                 << t0bsr_ric << std::endl ; 
    }  

    timebsr_ric=(evt->time - t0bsr_ric) * 1.E3 ;// time bstr plot starting from 0,msec
                                                 // L1 3908 uses usec , here msec !!! 
                                                // COR 3908 uses sec , here msec !!!   MM 23/11/07

    ie = Findie(evt) ;      // find event energy E and spatial zone ig 
    ig = Findig(evt) ; 

    if(timebsr_ric < 10.0 ) 
    { 
    
       cerr << " Into FillHisto3908_ric "  ; 
       DumpMCALEvent( evt ) ; 
       cerr << " Fillhisto3908_ric for event timed: " 
            << timebsr_ric << " Etot: " << evt->Etot 
	    <<" found bsr e,g: " << ie << " " << ig << endl ; 
    }	 

    if(ie<0 || ie >2)
    {
      cerr << " Fillhisto3908_ric ERROR! Event ignored, Energy out of range " << endl ; 
      return ;
    }
    if(ig<0 || ig > 3)
    {
      cerr<< " Fillhisto3908_ric ERROR! Event ignored, zone out of range "<<endl ; 
      return ;    
    }

    for (it=0 ; it < nsit ; it++) 
    {
       // if(timebsr_ric < 10.0 ) 
       // { 
       //    cerr << " Fillhisto3908: fill bsr reconstructed  for sit e,g,t: "
       //        << ie << " " << ig << " " << it <<endl ; 
       // }	 

       if(ie==2)
       {
          gdr_E2_mcal_ric[it]->Fill(timebsr_ric) ; 
       }
       else
       {
          gdr_mcal_ric[ie][ig][it]->Fill(timebsr_ric) ;
       }  
    }
} 


void MCVHisto3908::FillGlobalHisto3908( )
{ 
 // fills hist defined after all data have been read 

}
int MCVHisto3908::Findie(MCALevt *evt )
{
   //cerr <<" Etot: " << evt->Etot 
   //     << " limits: "<< MCALEbsrlim0 << " " << MCALEbsrlim1 << endl ; 

   // NB: questo approccio non va bene: bisogna usare le energie ricostruite con l'algoritmo dei ratemeters scientifici!
/*   if( evt->Etot < MCALEbsrlim0 ) return 0 ; 
   if( evt->Etot < MCALEbsrlim1 ) return 1 ; 
   return 2 ; */
   
 	
	// use scientific ratemeters algorithm
	int totE = evt->totEX + evt->totEZ;
	int limsup = 8;
	int irm=1;
	for (int k=0; k<12; k++) {
		if (totE < limsup) {
			break;
		}
		if (irm < 11) irm++;
		limsup *=2;
	}
	if (irm<=MCALElim0) return 0;
	if (irm<=MCALElim1) return 1;
	return 2;
}

int MCVHisto3908::Findig(MCALevt *evt )
{
   Float_t Eilmax ;
   int ilmax ; 
   int i ; // bar number 
   int j ; // place of bar i in the arrays: E[],pdA,pdB ... 

   ilmax=-1 ; 
   Eilmax=0.0 ; 
   
   for (i=0;i<30;i++)
   {    
       // evt->bar[i]<0 means No E into this bar  
       // if bar i is active j=evt->bar[i] is the index of
       // the bar in the arrays E[],pdA,pdB ...
         
       if(evt->bar[i] < 0.0 || evt->bar[i] > 29) continue ; 
       j= evt->bar[i] ; 
       if(evt->rmE[j] >= Eilmax) { Eilmax=evt->rmE[j]; ilmax=i ; } // >= to accept E=0      
   }
   if(ilmax < 0) 
   {
      cerr<< " WARNING ! Events with no hits on MCAL bars " << endl ; 
      return -1 ; 
   }
   
   if( ilmax <  8 ) return 0 ; // from 0 to 7   (8bar zone,upper plane): g=0 
   if( ilmax < 15 ) return 1 ; // from 8 to 14  (7bar zone,upper plane): g=1 
   if( ilmax < 23 ) return 2 ; // from 15 to 22 (8bar zone,lower plane): g=2 
   return 3 ;              // from 23 to 29 (7bar zone,lower plane): g=3

}
void MCVHisto3908::DumpMCALEvent(MCALevt *evt )
{
   // evt->E[],pbs[],pdB etc contengono solo le barre buone; quali sono
   // e' contenuto in evt->bar[30];  
   // evt->bar[30], contiene rispettivamente per le barra numero 0,1,2 ...29 : 
   // -1 se la barra non e' scattata, il numero dell'elemento di E[],pbs[],pdB
   // relativo alla barra ; se la barra e' scattata
   
   int i ,j ; 
   
   cerr << " Event dump: time: "<< evt->time << " mult: " << evt->mult 
        << " Etot: " << evt->Etot
        << " totEX: "<< evt->totEX << " TotEZ: " << evt->totEZ << endl ; 

   cerr << " Bar place  PdA  PdB  pos(cm)   E(Mev) " << endl ;
   
   for (i=0 ; i <30 ; i++ ) 
   {
      if (evt->bar[i] < 0 ) continue ; // -1 se la barra non e' nell'array 
      j=evt->bar[i] ;                  //  altrimenti e' il numero della barra 
                                       // negli array pdA,pdB,etc 
      if(j>29)
      { 
         cerr << " ERROR WRONG BAR NUMBER IN currevt ! " <<j <<endl ;
	 continue ; 
      }	 
      cerr << " " << i << "    " << j << "    "  
           << evt->pdA[j] << "  " << evt->pdB[j]  << " " 
           << evt->pos[j] << "   " << evt->E[j] << endl ;   
   }
}   	   
