//
//
// Description: class to view burst rameters
//
// Authors: marcello galli, martino marisaldi 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MCVbsrMonitor.h"

ClassImp(MCVbsrMonitor);

MCVbsrMonitor::MCVbsrMonitor(const TGWindow *p,  TGWindow *main, UInt_t w, UInt_t h) 
    : TGTransientFrame(p, main, w, h, kFitWidth | kFitHeight), 
      rawbsr(NULL), conf(NULL), fileInfo(NULL) 
{ 
	sitwinttime[0]=1    ;         
        sitwinttime[1]=16   ;    
	sitwinttime[2]=64   ;    
	sitwinttime[3]=256  ;    
	sitwinttime[4]=1024 ;    
	sitwinttime[5]=8192 ;    
	

	// Creates default style 
	
	TStyle * mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "xyz");
	mcalstyle->SetTitleSize(0.05, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
   
       char titolo[50] ;
       strncpy(titolo,"bsr summary",50) ; 	
	 
       allmcal = new TCanvas("allmcal",titolo,0,10,800,600) ; 
       allmcal->Divide(4,3) ;    // 4 cols, 3 rows
       
       // canvas layout: 
       //  1msec  ; 16 msec  ; E2 64+256  ; E2 1024+8192
       //  E1 G0  ; E1 G1    ; E1 G2      ; E1 G3 all sw sits
       //  E0 G0  ; E1 G1    ; E1 G2      ; E1 G3 all sw sits
       

}

MCVbsrMonitor::~MCVbsrMonitor()
{
}

void MCVbsrMonitor::SetHistoCollection(TObjArray *hc) 
{
	rawbsr = hc;
//	analyzer->SetHistoCollection(hc);	
	return;
}

TObjArray   * MCVbsrMonitor::GetHistoCollection(){return rawbsr;}
MCVFileInfo * MCVbsrMonitor::GetFileInfo()       {return fileInfo;}


void MCVbsrMonitor::SetFileInfo(MCVFileInfo *info)
{

	fileInfo=info  ;
	if (!fileInfo) 
	{
	   cerr << " warning void MCVFileInfo given to MCVBsrMonitor " << endl ; 
	   //fileInfo = new MCVFileInfo(); 
	}   	
//	analyzer->SetFileInfo(info);
	return;

}	

void MCVbsrMonitor::FillRaw()
{   
	 char colname[30];
	 
        // ---- retrieving graph from TobjArray in file 

        for (int it=firstsoftsitnum; it<nsitsw; it++) 
	{
	   sprintf(colname, "DR_E2_T%d",sitwinttime[it]);  

	   gdr_E2_mcal[it] = (TGraph *) rawbsr->FindObject(colname) ;

	   if( ! gdr_E2_mcal[it] )
	   {
	      cerr << " Error TGraph for " << colname <<" missing in file "<<endl ;  	   
           }

           for (int ie=0; ie<nEsegm; ie++) {
           for (int ig=0; ig<ngeom ; ig++) {     
 
              sprintf(colname, "MCAL_DR_E%d_G%d_T%d",ie,ig,sitwinttime[it]);  
	  
	      gdr_mcal[ie][ig][it] = (TGraph *) rawbsr->FindObject(colname) ;  

	     if( ! gdr_mcal[ie][ig][it] )
	     {
	        cerr << " Error TGraph for " << colname <<" missing in file "<<endl ;  	   
             }

           }}
	}   

}
void MCVbsrMonitor::PlotAll() 
{  
    
    TGraph * grafico ; 

    cerr << " Plotting begins ..." <<endl ; 

    // graph*->SetLineColor(1) ; // background norm, nero
    // graph*->SetMarkerStyle(2) ; // marker: cross 
    //
    //    colori : 1 nero, 2=red , 3=green , 4=blue, 5=giallo, 6=pink, 7=azzurro
    //             8=verdone, 9=bluscuro, 10=bianco 12-19 = grigi  
    //    markers : 1= punto , 2=cross, 3= * , 4= tondo , 5 =x, 7 = quadratini
    //              20 = tondo scuro, 21 quadrato, 22,23 triangoli, 29=star 
    //


   // -------------- first line 
   
   // allmcal->cd(1) ;   //  1 msec .... still to be done 
   // allmcal->cd(2) ;   // 16 msec .... still to be done 
  
   allmcal->cd(3) ; // E2 64+256 msec 
   
     grafico=findymax(gdr_E2_mcal[firstsoftsitnum    ],
                      gdr_E2_mcal[firstsoftsitnum +1 ]) ;
		      
     grafico->GetXaxis()->SetTitle("Time (ms)");		      
     grafico->GetXaxis()->CenterTitle();
     grafico->GetYaxis()->SetTitle("Counts");
     grafico->GetYaxis()->CenterTitle();
    
     grafico->Draw("A*") ; // quello con Ymax fa assi etc. etc. 
 
     gdr_E2_mcal[firstsoftsitnum    ]->Draw("*") ; // gli altri solo dei punti
     gdr_E2_mcal[firstsoftsitnum +1 ]->Draw("*") ; 

    /****  
    
      gdr_E2_mcal[it] = new TGraph(nummsec) ; 
      gdr_E2_mcal[it]->SetName(colname);
      gdr_E2_mcal[it]->SetTitle(colname);
      gdr_E2_mcal[it]->GetXaxis()->SetTitle("Time (ms)");
//    gdr_E2_mcal[it]->GetXaxis()->SetTitleOffset(1.4);
      gdr_E2_mcal[it]->GetXaxis()->CenterTitle();
      gdr_E2_mcal[it]->GetYaxis()->SetTitle("Counts");
      gdr_E2_mcal[it]->GetYaxis()->CenterTitle();
//    gdr_E2_mcal[it]->GetYaxis()->SetTitleOffset(1.5);

     grafico = findymax(gdr_E2_mcal[firstsoftsitnum    ],
                        gdr_E2_mcal[firstsoftsitnum +1 ]) ;    
     cerr << " TGRAPH con Ymax : "<< grafico ;
     cerr << " fra: " << gdr_E2_mcal[firstsoftsitnum    ] << " " 
          << gdr_E2_mcal[firstsoftsitnum +1 ] << endl ; 
     *****/


   allmcal->cd(4) ; // E2 1024+8192 msec 

     /****
     grafico = findymax(gdr_E2_mcal[firstsoftsitnum +2   ],
                        gdr_E2_mcal[firstsoftsitnum +3 ]) ;    
     cerr << " TGRAPH con Ymax : "<< grafico ;
     cerr << " fra: " << gdr_E2_mcal[firstsoftsitnum +2   ] << " " 
          << gdr_E2_mcal[firstsoftsitnum +3 ] << endl ; 
      *****/

     findymax(gdr_E2_mcal[firstsoftsitnum +2   ],
              gdr_E2_mcal[firstsoftsitnum +3 ]) ->Draw("A*") ; 

     gdr_E2_mcal[firstsoftsitnum +2 ]->Draw("*") ; 
     gdr_E2_mcal[firstsoftsitnum +3 ]->Draw("*") ; 

   // -------------- second line 

   allmcal->cd(5) ; // E1,G0 : all sw sits 
   
    findymax(gdr_mcal[1][0][firstsoftsitnum    ],
             gdr_mcal[1][0][firstsoftsitnum +1 ],
             gdr_mcal[1][0][firstsoftsitnum +2 ],
             gdr_mcal[1][0][firstsoftsitnum +3 ])->Draw("A*") ;   

             gdr_mcal[1][0][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[1][0][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[1][0][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[1][0][firstsoftsitnum +3 ]->Draw("*") ;   

   allmcal->cd(6) ; // E1,G1 : all sw sits 

    findymax(gdr_mcal[1][1][firstsoftsitnum    ],
             gdr_mcal[1][1][firstsoftsitnum +1 ],
             gdr_mcal[1][1][firstsoftsitnum +2 ],
             gdr_mcal[1][1][firstsoftsitnum +3 ])->Draw("A*") ;   

             gdr_mcal[1][1][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[1][1][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[1][1][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[1][1][firstsoftsitnum +3 ]->Draw("*") ;   

   allmcal->cd(7) ; // E1,G2 : all sw sits 

    findymax(gdr_mcal[1][2][firstsoftsitnum    ],
             gdr_mcal[1][2][firstsoftsitnum +1 ],
             gdr_mcal[1][2][firstsoftsitnum +2 ],
             gdr_mcal[1][2][firstsoftsitnum +3 ])->Draw("A*") ;   

             gdr_mcal[1][2][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[1][2][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[1][2][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[1][2][firstsoftsitnum +3 ]->Draw("*") ;   

   allmcal->cd(8) ; // E1,G3 : all sw sits 
 
    findymax(gdr_mcal[1][3][firstsoftsitnum    ],
             gdr_mcal[1][3][firstsoftsitnum +1 ],
             gdr_mcal[1][3][firstsoftsitnum +2 ],
             gdr_mcal[1][3][firstsoftsitnum +3 ])->Draw("A*") ;   

             gdr_mcal[1][3][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[1][3][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[1][3][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[1][3][firstsoftsitnum +3 ]->Draw("*") ;   
    
   // -------------- third line 

   allmcal->cd(9) ; // E0,G0 : all sw sits 

    findymax(gdr_mcal[0][0][firstsoftsitnum    ],
             gdr_mcal[0][0][firstsoftsitnum +1 ],
             gdr_mcal[0][0][firstsoftsitnum +2 ],
             gdr_mcal[0][0][firstsoftsitnum +3 ])->Draw("A*") ;   
  
             gdr_mcal[0][0][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[0][0][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[0][0][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[0][0][firstsoftsitnum +3 ]->Draw("*") ;   
    
   allmcal->cd(10) ; // E0,G1 : all sw sits

    findymax(gdr_mcal[0][1][firstsoftsitnum    ],
             gdr_mcal[0][1][firstsoftsitnum +1 ],
             gdr_mcal[0][1][firstsoftsitnum +2 ],
             gdr_mcal[0][1][firstsoftsitnum +3 ])->Draw("A*") ;   
    
             gdr_mcal[0][1][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[0][1][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[0][1][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[0][1][firstsoftsitnum +3 ]->Draw("*") ;   
    
   allmcal->cd(11) ; // E0,G2 : all sw sits 

    findymax(gdr_mcal[0][2][firstsoftsitnum    ],
             gdr_mcal[0][2][firstsoftsitnum +1 ],
             gdr_mcal[0][2][firstsoftsitnum +2 ],
             gdr_mcal[0][2][firstsoftsitnum +3 ])->Draw("A*") ;   
   
             gdr_mcal[0][2][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[0][2][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[0][2][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[0][2][firstsoftsitnum +3 ]->Draw("*") ; 
      
   allmcal->cd(12) ; // E0,G3 : all sw sits 
   
    findymax(gdr_mcal[0][3][firstsoftsitnum    ],
             gdr_mcal[0][3][firstsoftsitnum +1 ],
             gdr_mcal[0][3][firstsoftsitnum +2 ],
             gdr_mcal[0][3][firstsoftsitnum +3 ])->Draw("A*") ;   

             gdr_mcal[0][3][firstsoftsitnum    ]->Draw("*") ;   
             gdr_mcal[0][3][firstsoftsitnum +1 ]->Draw("*") ;   
             gdr_mcal[0][3][firstsoftsitnum +2 ]->Draw("*") ;   
             gdr_mcal[0][3][firstsoftsitnum +3 ]->Draw("*") ;   
    
   cerr << " Plotting ended ..." <<endl ; 
 

} 
TGraph * MCVbsrMonitor::findymax(TGraph * a0 , TGraph * a1)
{
   // brutta routine per trovare l'Y max di 2 TGraphs
      
    double ymax0,ymax1 ;

    ymax0= a0->GetYaxis()->GetXmax() ; 
    ymax1= a1->GetYaxis()->GetXmax() ; 
    
    cerr << " max Y dei grafici: "<< ymax0 << " " << ymax1 <<endl ; 
    return ymax0 > ymax1?  a0 : a1 ;
 
}

TGraph * MCVbsrMonitor::findymax(TGraph * a0 , TGraph * a1, TGraph * a2, TGraph * a3)
{
   // routine ancor piu' brutta per trovare l'Y max di 4 TGraphs
   
    TGraph *  yilmax ;
   
    yilmax = a0 ;
   
    if(a1->GetYaxis()->GetXmax() > yilmax->GetYaxis()->GetXmax()) yilmax=a1 ; 
    if(a2->GetYaxis()->GetXmax() > yilmax->GetYaxis()->GetXmax()) yilmax=a2 ; 
    if(a3->GetYaxis()->GetXmax() > yilmax->GetYaxis()->GetXmax()) yilmax=a3 ; 
    
    return yilmax ;

}


