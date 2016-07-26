
// Last update: 3-july-2006
//
// Description: class to view burst rameters
//
// Authors: marcello galli, martino marisaldi 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MCVbsrMonitor_ric.h"

ClassImp(MCVbsrMonitor_ric);

MCVbsrMonitor_ric::MCVbsrMonitor_ric(const TGWindow *p,  TGWindow *main, UInt_t w, UInt_t h) 
    : TGTransientFrame(p, main, w, h, kFitWidth | kFitHeight), 
      rawbsr_ric(NULL), conf(NULL), fileInfo(NULL), burstarray(NULL)
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
       
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	conf = new TEnv(confname);
	nsigma = conf->GetValue(".burst.sigma.", 0);
}

MCVbsrMonitor_ric::~MCVbsrMonitor_ric()
{
}

void MCVbsrMonitor_ric::SetHistoCollection(TObjArray *hc) 
{
	rawbsr_ric = hc;
//	analyzer->SetHistoCollection(hc);	
	return;
}

void MCVbsrMonitor_ric::SetBurstCollection(TObjArray *hb) 
{
	burstarray = hb;
	return;
}

TObjArray   * MCVbsrMonitor_ric::GetHistoCollection(){return rawbsr_ric;}
MCVFileInfo * MCVbsrMonitor_ric::GetFileInfo()       {return fileInfo;}


void MCVbsrMonitor_ric::SetFileInfo(MCVFileInfo *info)
{
	fileInfo=info  ;
	if (!fileInfo) 
	{
	   cerr << " warning void MCVFileInfo given to MCVbsrMonitor_ric "<< endl ; 
	   //fileInfo = new MCVFileInfo(); 
	}   	
//	analyzer->SetFileInfo(info);
	return;
}	

void MCVbsrMonitor_ric::FillRaw()
{   
	 char colname[30];
	 
        // ---- retrieving graph from TobjArray in file 

        for (int it=firstsoftsitnum; it<nsit; it++) 
	{
	   sprintf(colname, "DR_E2_T%d_rec",sitwinttime[it]);  

	   gdr_E2_mcal[it] = (TH1I *) rawbsr_ric->FindObject(colname) ;

	   if( ! gdr_E2_mcal[it] )
	   {
	      cerr << " Error TH1I for " << colname <<" missing in file "<<endl ;  	   
           }

           for (int ie=0; ie<nEsegm; ie++) {
           for (int ig=0; ig<ngeom ; ig++) {     
 
              sprintf(colname, "MCAL_DR_E%d_G%d_T%d_rec",ie,ig,sitwinttime[it]);  
	  
	      gdr_mcal[ie][ig][it] = (TH1I *) rawbsr_ric->FindObject(colname) ;  

	     if( ! gdr_mcal[ie][ig][it] )
	     {
	        cerr << " Error TH1I for " << colname <<" missing in file "<<endl ;  	   
             }

           }}
	}   

}
void MCVbsrMonitor_ric::PlotAll() 
{  
    
    TH1I * grafico ; 

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
   
    grafico=findhmax( gdr_E2_mcal[firstsoftsitnum    ],
                      gdr_E2_mcal[firstsoftsitnum +1 ]) ;
		      
    grafico->GetXaxis()->SetTitle("Time (ms)");		      
    grafico->GetXaxis()->CenterTitle();
    grafico->GetYaxis()->SetTitle("Counts");
    grafico->GetYaxis()->CenterTitle();
    
    grafico->Draw("AXIS") ; // quello con Ymax fa assi etc. etc. 
    
    gdr_E2_mcal[firstsoftsitnum +1 ]->SetLineColor(8) ; //green
    gdr_E2_mcal[firstsoftsitnum    ]->SetLineColor(2) ; //red
    
    gdr_E2_mcal[firstsoftsitnum +1 ]->Draw("SAME") ; // gli altri solo dei punti
    gdr_E2_mcal[firstsoftsitnum    ]->Draw("SAME") ; 

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
      gdr_E2_mcal[it]->SetLineColor(num colore) ;

     grafico = findhmax(gdr_E2_mcal[firstsoftsitnum    ],
                        gdr_E2_mcal[firstsoftsitnum +1 ]) ;    
     cerr << " TGRAPH con Ymax : "<< grafico ;
     cerr << " fra: " << gdr_E2_mcal[firstsoftsitnum    ] << " " 
          << gdr_E2_mcal[firstsoftsitnum +1 ] << endl ; 
     *****/

     

   allmcal->cd(4) ; // E2 1024+8192 msec 

     /****
     grafico = findhmax(gdr_E2_mcal[firstsoftsitnum +2   ],
                        gdr_E2_mcal[firstsoftsitnum +3 ]) ;    
     cerr << " TGRAPH con Ymax : "<< grafico ;
     cerr << " fra: " << gdr_E2_mcal[firstsoftsitnum +2   ] << " " 
          << gdr_E2_mcal[firstsoftsitnum +3 ] << endl ; 
      *****/


     findhmax(gdr_E2_mcal[firstsoftsitnum +2   ],
              gdr_E2_mcal[firstsoftsitnum +3 ]) ->Draw("AXIS") ; 
    
     gdr_E2_mcal[firstsoftsitnum +3 ]->SetLineColor(1) ; //black
     gdr_E2_mcal[firstsoftsitnum +2 ]->SetLineColor(9) ; //blue

     gdr_E2_mcal[firstsoftsitnum +3 ]->Draw("SAME") ; // gli altri solo dei punti
     gdr_E2_mcal[firstsoftsitnum +2 ]->Draw("SAME") ; 



   // -------------- second line 

   allmcal->cd(5) ; // E1,G0 : all sw sits 
   
    findhmax(gdr_mcal[1][0][firstsoftsitnum    ],
             gdr_mcal[1][0][firstsoftsitnum +1 ],
             gdr_mcal[1][0][firstsoftsitnum +2 ],
             gdr_mcal[1][0][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[1][0][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[1][0][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[1][0][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[1][0][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   


             gdr_mcal[1][0][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[1][0][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[1][0][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[1][0][firstsoftsitnum    ]->Draw("SAME") ;   

   allmcal->cd(6) ; // E1,G1 : all sw sits 

    findhmax(gdr_mcal[1][1][firstsoftsitnum    ],
             gdr_mcal[1][1][firstsoftsitnum +1 ],
             gdr_mcal[1][1][firstsoftsitnum +2 ],
             gdr_mcal[1][1][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[1][1][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[1][1][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[1][1][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[1][1][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   


             gdr_mcal[1][1][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[1][1][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[1][1][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[1][1][firstsoftsitnum    ]->Draw("SAME") ;   

   allmcal->cd(7) ; // E1,G2 : all sw sits 

    findhmax(gdr_mcal[1][2][firstsoftsitnum    ],
             gdr_mcal[1][2][firstsoftsitnum +1 ],
             gdr_mcal[1][2][firstsoftsitnum +2 ],
             gdr_mcal[1][2][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[1][2][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[1][2][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[1][2][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[1][2][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   

             gdr_mcal[1][2][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[1][2][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[1][2][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[1][2][firstsoftsitnum    ]->Draw("SAME") ;   

   allmcal->cd(8) ; // E1,G3 : all sw sits 
 
    findhmax(gdr_mcal[1][3][firstsoftsitnum    ],
             gdr_mcal[1][3][firstsoftsitnum +1 ],
             gdr_mcal[1][3][firstsoftsitnum +2 ],
             gdr_mcal[1][3][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[1][3][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[1][3][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[1][3][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[1][3][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   

             gdr_mcal[1][3][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[1][3][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[1][3][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[1][3][firstsoftsitnum    ]->Draw("SAME") ;   
    
   // -------------- third line 

   allmcal->cd(9) ; // E0,G0 : all sw sits 

    findhmax(gdr_mcal[0][0][firstsoftsitnum    ],
             gdr_mcal[0][0][firstsoftsitnum +1 ],
             gdr_mcal[0][0][firstsoftsitnum +2 ],
             gdr_mcal[0][0][firstsoftsitnum +3 ])->Draw("AXIS") ;   
  
             gdr_mcal[0][0][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[0][0][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[0][0][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[0][0][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   

             gdr_mcal[0][0][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[0][0][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[0][0][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[0][0][firstsoftsitnum    ]->Draw("SAME") ;   
    
   allmcal->cd(10) ; // E0,G1 : all sw sits

    findhmax(gdr_mcal[0][1][firstsoftsitnum    ],
             gdr_mcal[0][1][firstsoftsitnum +1 ],
             gdr_mcal[0][1][firstsoftsitnum +2 ],
             gdr_mcal[0][1][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[0][1][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[0][1][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[0][1][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[0][1][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   
    
             gdr_mcal[0][1][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[0][1][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[0][1][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[0][1][firstsoftsitnum    ]->Draw("SAME") ;   
    
   allmcal->cd(11) ; // E0,G2 : all sw sits 

    findhmax(gdr_mcal[0][2][firstsoftsitnum    ],
             gdr_mcal[0][2][firstsoftsitnum +1 ],
             gdr_mcal[0][2][firstsoftsitnum +2 ],
             gdr_mcal[0][2][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[0][2][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[0][2][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[0][2][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[0][2][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   

             gdr_mcal[0][2][firstsoftsitnum +3 ]->Draw("SAME") ;   
             gdr_mcal[0][2][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[0][2][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[0][2][firstsoftsitnum    ]->Draw("SAME") ; 
      
   allmcal->cd(12) ; // E0,G3 : all sw sits 
   
    findhmax(gdr_mcal[0][3][firstsoftsitnum    ],
             gdr_mcal[0][3][firstsoftsitnum +1 ],
             gdr_mcal[0][3][firstsoftsitnum +2 ],
             gdr_mcal[0][3][firstsoftsitnum +3 ])->Draw("AXIS") ;   

             gdr_mcal[0][3][firstsoftsitnum +3 ]->SetLineColor(1) ; //black  
             gdr_mcal[0][3][firstsoftsitnum +2 ]->SetLineColor(9) ; //blue  
             gdr_mcal[0][3][firstsoftsitnum +1 ]->SetLineColor(8) ; //green ;   
             gdr_mcal[0][3][firstsoftsitnum    ]->SetLineColor(2) ; //red ;   

             gdr_mcal[0][3][firstsoftsitnum +3   ]->Draw("SAME") ;   
             gdr_mcal[0][3][firstsoftsitnum +2 ]->Draw("SAME") ;   
             gdr_mcal[0][3][firstsoftsitnum +1 ]->Draw("SAME") ;   
             gdr_mcal[0][3][firstsoftsitnum    ]->Draw("SAME") ;   
  
   cerr << " Adding burst info ..." <<endl ;
   AddBurstInfo();
   cerr << " Plotting ended ..." <<endl ; 
 

} 
TH1I * MCVbsrMonitor_ric::findhmax(TH1I * a0 , TH1I * a1)
{
   // brutta routine per trovare l'Y max di 2 TH1Is
      
    double ymax0,ymax1 ;

    ymax0= a0->GetMaximum() ; 
    ymax1= a1->GetMaximum() ; 
    
    cerr << " max Y dei grafici: "<< ymax0 << " " << ymax1 <<endl ; 
    return ymax0 > ymax1?  a0 : a1 ;
 
}

TH1I * MCVbsrMonitor_ric::findhmax(TH1I * a0 , TH1I * a1, TH1I * a2, TH1I * a3)
{
   // routine ancor piu' brutta per trovare l'Y max di 4 TH1Is
   
    TH1I *  yilmax ;
   
    yilmax = a0 ;
   
    if(a1->GetMaximum() > yilmax->GetMaximum()) yilmax=a1 ; 
    if(a2->GetMaximum() > yilmax->GetMaximum()) yilmax=a2 ; 
    if(a3->GetMaximum() > yilmax->GetMaximum()) yilmax=a3 ; 
    
    return yilmax ;

}


void MCVbsrMonitor_ric::AddBurstInfo()
{
	int nburst;
	TLine *l;
	MCVBurst *currburst;
	
	if (burstarray == NULL) return;
	
	nburst = burstarray->GetEntries();
	cout << "adding info for " << nburst << " bursts" << endl;
	
	for (int i=0; i<nburst; i++) {
		currburst = (MCVBurst *) burstarray->At(i);
		currburst->PrintInfo();
		if (currburst->nume < 2) {	// trigger on low or medium energy bands
			int id = 9 - 4*currburst->nume + currburst->numg;
			cout << " burst " << currburst->burstid << ": changing to pad " << id << endl;
			allmcal->cd(id);
		} else if (currburst->bsrsit < 2) {
			allmcal->cd(3);
		} else allmcal->cd(3);
		
		double len = 1.E-3*(currburst->bstop - currburst->bstart);
		double t1 = 1.E-3*(currburst->bstart - currburst->inittime);
		double t2 = 1.E-3*(currburst->bstop  - currburst->inittime);
		
		l = new TLine(t1 - 0.2*len, currburst->ber, t2 + 0.2*len, currburst->ber);
		l->Draw();
		cout << "drawing line at " << t1 - 0.2*len << " - " 
		     << currburst->ber << " - " 
		     << t2 + 0.2*len << " - " << currburst->ber << endl;
		
		l = new TLine(t1 - 0.2*len, currburst->ber + nsigma*sqrt(currburst->ber), t2 + 0.2*len, currburst->ber + nsigma*sqrt(currburst->ber));
		l->SetLineStyle(2);
		l->Draw();
		
		l = new TLine(t1, 0, t1, 1.2*currburst->bsr);
		l->SetLineStyle(3);
		l->Draw();
		
		l = new TLine(t2, 0, t2, 1.2*currburst->bsr);
		l->SetLineStyle(3);
		l->Draw();
		
	}
	
	return;
}

void MCVbsrMonitor_ric::PrintRMTable(double tini, double tend)
{
	double rmtable[4][3];	//[geometry][energy band]
	double rateLE=0., rateME=0., rateHE, rate;
	int bin1, bin2;
	
	bin1 = (int) (tini*1000./1024.);
	bin2 = (int) (tend*1000./1024.);
	
	// get the mean RM from the 1024 ms SIT
	for (int geom=0; geom<4; geom++) {	// loop on geometry
		for (int eband = 0; eband<2; eband++) {	// loop on energy bands
			rmtable[geom][eband] = gdr_mcal[eband][geom][firstsoftsitnum +2 ]->Integral(bin1, bin2) / (bin2-bin1);
		}
	}
	rmtable[0][2] = gdr_E2_mcal[firstsoftsitnum +2 ]->Integral(bin1, bin2) / (bin2-bin1);
	
	// print table
	printf("\nRatemeters between t1=%.2f s and t2=%.2f s.\n", tini, tend);
	printf("G0 = bars X1-X8\nG1 = bars X9-X15\nG2 = bars Z1-Z8\nG3 = bars Z9-Z15\n\n");
	printf("1024 ms SIT:\n\t\tLE \t\tME \t\tHE\n");
	for (int geom=0; geom<4; geom++) {	// loop on geometry
		printf("G%d\t\t", geom);
		for (int eband = 0; eband<2; eband++) {	// loop on energy bands
			printf("%8.2f\t", rmtable[geom][eband]);
		}
		if (geom==0) printf("%8.2f\t", rmtable[0][2]);
		printf("\n");
	}
	
	printf("\nRMTABLE \t%d \t", fileInfo->runid);
	for (int eband = 0; eband<2; eband++) {	// loop on geometry
		for (int geom=0; geom<4; geom++) printf("%8.2f \t", rmtable[geom][eband]);
	}
	printf("%8.2f\t\n", rmtable[0][2]);
	
	// calculate rates
	for(int geom=0; geom<4; geom++) {
		rateLE += rmtable[geom][0];
		rateME += rmtable[geom][1];
	}
	rateLE /= 1.024;	// from 1024 ms to 1 s
	rateME /= 1.024;
	rateHE = rmtable[0][2]/1.024;
	rate = rateLE + rateME + rateHE;
	printf("\nLE rate  = %8.2f Hz \nME rate  = %8.2f Hz \nHE rate  = %8.2f Hz \ntot rate = %8.2f Hz\n", rateLE, rateME, rateHE, rate);
		
	return;
}
