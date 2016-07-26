
//
// Last update: 3-july-2006
//
// C++ Implementation: MCVFIlePKP3908
//
// Description: 
//
//
// Authors : Martino Marisaldi and marcello galli,,+39 51 6098312 <marcello.galli@bologna.enea.it>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
// file : MCVFilePKP3908.cpp , m.galli, june 2006 
//
// ----------------------------------------------------------

#include "MCVFilePKP3908.h"
#include <fitsio.h>

#define MAXLEN 100000

ClassImp(MCVFilePKP3908) ;

MCVFilePKP3908::MCVFilePKP3908()
{

}

//
//--------------------------------------------------------------------
//

MCVFilePKP3908::MCVFilePKP3908(const char *name) : MCVFilePKP(name), MCVHisto3908()
{
        // MCVFilePKP initilizes MCVFIle(name), MCVHisto()
        
        //ResizeCountRateHisto(fileInfo.ttot);
        //if (fileInfo.gainFactor != 1.) gf=fileInfo.gainFactor;
	
	cerr << " fits file name in MCVFilePKP3908: " << fileInfo.filename << endl ; 
	cerr << " fits file name as TString fitsname: "   << fitsname.Data() << endl ; 
	cerr << " fits file name as argument " << name << endl ; 
	
	burstarray = new TObjArray(MAX_BURST);
	
}

//
//--------------------------------------------------------------------
//

MCVFilePKP3908::~MCVFilePKP3908()
{
//      if (tdata) delete tdata;
//	delete burstarray;
}

void MCVFilePKP3908::CreateRootFile3908()
{	

	cerr << " fits file name in CreateRootFile3908: " << fileInfo.filename << endl ; 
	cerr << " fits file name as TString fitsname: "   << fitsname.Data() << endl ; 
	
        //           override's parent method , case 3908 added to the packet type switch
        //           tdatabsr, rawEt, rawbsr written to output files 
        if (fp==NULL) {
                
                rootdatapath.Append(getenv("MCALSW"));
                rootdatapath += stdconf->GetValue(".root.data.path.","./");     
                char rootfilename[100];
                sprintf(rootfilename,"RT%06d_%04d.root", fileInfo.runid, fileInfo.pkttype);
                rootdatapath += rootfilename;
                std::cerr << " Creating root file : " << rootdatapath.Data() << endl ; 
                fp = new TFile(rootdatapath.Data(), "recreate");
                tdata = new TTree("tdata", "MCAL data");
                tdata->Branch("time", &currevt.time, "time/D");
                tdata->Branch("mult", &currevt.mult, "mult/I");
                tdata->Branch("bar",   currevt.bar,  "bar[30]/S");
                tdata->Branch("pdA",   currevt.pdA,  "pdA[mult]/S");
                tdata->Branch("pdB",   currevt.pdB,  "pdB[mult]/S");
                tdata->Branch("pos",   currevt.pos,  "pos[mult]/F");
                tdata->Branch("E",     currevt.E,    "E[mult]/F");
                tdata->Branch("Etot", &currevt.Etot, "Etot/F");

                switch (fileInfo.pkttype) {
                        case 3901:
                                std::cerr << " calling Process3902Data for 3901 pkts" << endl ;                                 
                                Process3902Data();
                                break;                  
                        case 3902:                                                      
                                std::cerr << " calling Process3902Data " << endl ; 
                                Process3902Data();
                                break;                  
                        case 3908:
				Resize_bsr_ric();
                                // opens hdu named EVENTS reading bar data 
                                std::cerr << " calling Process3908Data " << endl ;      
                                Process3908Data();
                                // opens hdu named PACKETS; with packet data and subm.bsr 
                                std::cerr << " calling Process3908pktData " << endl ;   
                                Process3908pktData(); 
                                                // opens hdu named RATEMTRS; with bsr data 
                                std::cerr << " calling Process3908bsrData " << endl ; 
                               // Process3908bsrData(); 
                                std::cerr << " calling PrintBurstInfo " << endl ; 
 				// PrintBurstInfo();    
                                break;
                        case 3909:
                                std::cerr << " calling Process3909Data " << endl ; 
                                //Process3909Data();  //  opend PDHu number ....  
                                Process3908Data(); 
                                break;
			case 3910:
				Process3909Data();
				break;	
 			case 0:
                                std::cerr << " calling ProcessTEData " << endl ; 
				ProcessTEData();
				break;	
                       default:
                                break;
                }
                fileInfo.nevents = tdata->GetEntries();
                
                fileInfo.nlowDT = nlowDT;
                fileInfo.lowDTti = lowDTti;
                fileInfo.lowDTtf = lowDTtf;
                if (fileInfo.pkttype==3901) {   // to account for 39.01 low data rate
                        tdata->GetEntry(0);
                        double ti = currevt.time;
                        tdata->GetEntry(fileInfo.nevents-1);
                        double tf = currevt.time;
                        fileInfo.teff = (tf-ti)*1.E-6;
                } else fileInfo.teff = GetEffectiveTime();
                fileInfo.notmonotonic = notmonotonic;
                char infoname[20];
                Add3913SciRatemeters();
                sprintf(infoname, "run%06d_%04d_info", fileInfo.runid, fileInfo.pkttype);
                fileInfo.SetName(infoname);
                
                std::cerr << " Writing bar data to the root file ... " << endl ; 

                tdata->Write();         // write the Root tree to file
                fileInfo.Write(infoname);
                
                if(tdatabsr) 
                {
                  std::cerr << " Writing bsr data to the root file ... " << endl ;
                  tdatabsr->Write(); // bsr data, if read, are written
                }
                else
                {
                  std::cerr << "  bsr data not found ... " << endl ;
                } 
                
                fp->Close();
                                
                FillCountRateDistribution();
                FillGlobalHisto(fileInfo.pkttype, fileInfo.teff);

		cerr << " Histo path: " << roothistopath.Data() << endl ; 
		cerr << " Histo path: via gethistopath " << GetHistoPath() << endl ; 
		
                roothistopath.Append(getenv("MCALSW"));

		cerr << " Histo path: " << roothistopath.Data() << endl ; 
		cerr << " Histo path: via gethistopath " << GetHistoPath() << endl ; 
				
                roothistopath += stdconf->GetValue(".root.histo.path.","./");   
		
		cerr << " Histo path: " << roothistopath.Data() << endl ; 
		cerr << " Histo path: via gethistopath " << GetHistoPath() << endl ; 


                char  histofilename[100];
                sprintf(histofilename,"H%06d_%04d.root", fileInfo.runid, fileInfo.pkttype);
                roothistopath += histofilename;
                
		cerr << " Histo path: " << roothistopath.Data() << endl ; 
		cerr << " Histo path: via gethistopath " << GetHistoPath() << endl ; 

                std::cerr << " Creating histo root file: " <<  roothistopath.Data() << endl ;
                
                fh = new TFile(roothistopath.Data(), "recreate");
                
                std::cerr << " Writing raw histo to the root file: " <<  roothistopath.Data() << endl ;
                rawHisto->Write("rawHisto", TObject::kSingleKey);
		burstarray->Write("burstArray", TObject::kSingleKey);

                fileInfo.Write(infoname);
                 
                // also E-t histo is written, if filled
                // also bsr graphs are written, if filled, 
		// also bsr_ric histo are written, if filled, 
                // a single key with name "rawbsr" is used to store all the bsr Graphs  
                    
                if(rawEt && ! rawEt->IsEmpty() ) 
                {
                   std::cerr << " Writing raw Et data to the root file ... " << endl ;
                   rawEt->Write("rawEt") ;  // a key is made for rawEt     
                }
                else
                {
                  std::cerr << " Raw Et data not found ... " << endl ; 
                }    

                if(rawbsr && ! rawbsr->IsEmpty()   )
                {
                 std::cerr << " Writing raw bsr data to the root file ... "<< endl ;
		 //rawbsr->Write("rawbsr") ; // each bsr with its key 
                 rawbsr->Write("rawbsr",TObject::kSingleKey);//unique key for all bsr
                }
                else
                {
                  std::cerr << " Raw bsr data not found ... " << endl ; 
                }

                if(rawbsr_ric && ! rawbsr_ric->IsEmpty() )
                {
                 std::cerr<<" Writing raw bsr rec data to the root file ..."<<endl;
		 //rawbsr->Write("rawbsr") ; // each bsr with its key 
                 rawbsr_ric->Write("rawbsr_ric", TObject::kSingleKey) ; 
                }
                else
                {
                  std::cerr << " Raw bsr rec. data not found ... " << endl ; 
                }
                
                fh->Close();
        } 
        return; 
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::Process3908bsrData()
{ 

	cerr << " fits file name in Process3908bsrData: " << fileInfo.filename << endl ; 
	cerr << " fits file name as TString fitsname:   " << fitsname.Data() << endl ; 

                 // ( Int_t is a root type =32 bit signed int =TINT32BIT, code: J in fitsio) 
                 //  (Short_t (root type) = 16 bin signed int = TSHORT, code: I in fitsio) 
                 // (Uchar_t = 8 bit unsigned = TBYTE, code : B in fitsio ) 
                 // ( Double_t = 64 bit double=TDOUBLE, code: D in fitsio) 
        
        int i ; 
                                    
        Int_t   *  pktnum   ;    //! packet keyword name: PAKTNUMB
        Short_t *  evnum    ;    //! packet keyword name: EVNUMBER
        char *     saormcal ;    //! 0=sa, 1=mcal : packet keyword name:  RATEMTID 
        char *     bsrid    ;    //! packet keyword name:  RTMTIDCH
        char *     bsrsit   ;    //!  packet keyword name:  RMTIDSIT      
        Int_t *    bsr      ;    //!  packet keyword name:  BSCIRTMT  ( ratemeters )
        Int_t *    bck      ;    //!  packet keyword name:  BSCIRTMT  ( backgr. ratemeters )
        Double_t * time     ;    //!  packet keyword name:  TIME 
        int nevents         ;    //! num bsr 

        char    comment[100];  // temporary array to store fits comments  
        
	cerr << " Process3908bsrData opening file: " << fileInfo.filename << endl ; 
        
        fitsfile *fptr;         // fits file pointer

        //if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );	
        if ( fits_open_file(&fptr, fileInfo.filename, READONLY, &status) )  printerror( status );
        if ( fits_movnam_hdu(fptr, ANY_HDU, "RATEMTRS", 0, &status) )  printerror( status );

        if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
        bsrinfo.numbsr=nevents ; 
        
        pktnum   = new Int_t   [nevents] ;     
        evnum    = new Short_t [nevents] ;      
        saormcal = new char    [nevents] ;
        bsrid    = new char    [nevents] ;
        bsrsit   = new char    [nevents] ;       
        bsr      = new Int_t   [nevents] ;   
        bck      = new Int_t   [nevents] ;  
        time     = new Double_t[nevents] ;
       
        cout << "Reading file..." << endl; 
        
        if (fits_read_col(fptr,TINT32BIT, 1, 1,1,nevents,0, pktnum,   NULL,&status)) printerror(status);
        if (fits_read_col(fptr,TSHORT,    2, 1,1,nevents,0, evnum,    NULL,&status)) printerror(status);        
        if (fits_read_col(fptr,TBYTE,     3, 1,1,nevents,0, saormcal, NULL,&status)) printerror(status);
        if (fits_read_col(fptr,TBYTE,     4, 1,1,nevents,0, bsrid,    NULL,&status)) printerror(status);        
        if (fits_read_col(fptr,TBYTE,     5, 1,1,nevents,0, bsrsit,   NULL,&status)) printerror(status);  
	if (nevents > 1) {	// to account for sub ms triggers with BSR=0
		if (fits_read_col(fptr,TINT32BIT, 6, 1,1,nevents,0, bsr,      NULL,&status)) printerror(status);
		if (fits_read_col(fptr,TINT32BIT, 7, 1,1,nevents,0, bck,      NULL,&status)) printerror(status);
	} else bsr[0] = bck[0] = 0;
        if (fits_read_col(fptr,TDOUBLE  , 8, 1,1,nevents,0, time,     NULL,&status)) printerror(status);

        tdatabsr = new TTree("tdatabsr", "MCAL bsr data"); 
        
        
        tdatabsr->Branch("pktnum",   &currbsr.pktnum,    "pktnum/I");  // Int 32 bit signed 
        tdatabsr->Branch("evnum" ,   &currbsr.evnum,     "evnum/I");   // tutti come int nel ttree
        tdatabsr->Branch("saormcal", &currbsr.saormcal,  "saormcal/I"); 
        tdatabsr->Branch("nume",     &currbsr.nume,      "nume/I"); 
        tdatabsr->Branch("numg",     &currbsr.numg,      "numg/I"); 
        tdatabsr->Branch("numt",     &currbsr.numt,      "numt/I"); 
        tdatabsr->Branch("dr",       &currbsr.dr,        "dr/I"); 
        tdatabsr->Branch("br",       &currbsr.br,        "br/I"); 
        tdatabsr->Branch("time",     &currbsr.time,      "time/D"); 
        tdatabsr->Branch("numch",    &currbsr.numch,     "numch/I"); 
 
	int currburstid = -1;
        for (i=0 ; i <nevents ; i++ )
        {
        
            if (i%10000==0) cout << "...processed " << i << " events..." << endl; 
            
            currbsr.pktnum   =   pktnum[i] ; 
            currbsr.evnum    =   evnum[i] ; 
            currbsr.saormcal =   saormcal[i]; 
            currbsr.nume     =   bsrid[i]/4  ;       // suppongo ordine: E0:g0,g1,g2,g3, E1:g0,g1, ... 
            currbsr.numg     =   bsrid[i] - currbsr.nume*4  ; 
            currbsr.numt     =   bsrsit[i]; 
            currbsr.dr       =   bsr[i]; 
            currbsr.br       =   bck[i] ; 
            currbsr.time     =   time[i]; 
        
            currbsr.numch    =  bsrid[i] ; 
        
            tdatabsr->Fill();
            FillGraph3908(i,&currbsr);
           // FillHisto3908(&currbsr); 
	    if (burstid[i] >  currburstid) {
	    	UpdateBurst(burstid[i], &currbsr);
	    	currburstid = burstid[i];
	    }
        }       
        cout << "Total number of entries: " << tdatabsr->GetEntries() << endl;
        fits_close_file(fptr, &status);         

             
        delete  pktnum   ;     
        delete  evnum    ;      
        delete  saormcal ;
        delete  bsrid    ;
        delete  bsrsit   ;       
        delete  bsr      ;   
        delete  bck      ;  
        delete  time     ;
             
            
}
void MCVFilePKP3908::PrintEvent3908(int nevt)
{
        if (tdatabsr==NULL) {
                cout << "Open Root Tree first!" << endl;
                return;
        }
        tdatabsr->Show(nevt);
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::Process3908pktData()
{
	cerr << " fits file name in : Process3908pktData" << fileInfo.filename << endl ; 
	cerr << " fits file name as TString fitsname: " << fitsname.Data() << endl ; 

        int i ; 
        int nevents         ;    //! num bsr 
	Int_t *bstarthi;
	Int_t *bstartlo;
	Int_t *bstophi;
	Int_t *bstoplo;
	Double_t *bstart;
	Double_t *bstop;

        char    comment[100];  // temporary array to store fits comments  
	Double_t t0 = -1.;
	nburst = 0;
	
	MCVBurst *currentburst = NULL;
        
	cerr << " Process3908bsrData opening file: " << fileInfo.filename << endl ; 
        
        fitsfile *fptr;         // fits file pointer

        //if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );	
        if ( fits_open_file(&fptr, fileInfo.filename, READONLY, &status) )  printerror( status );
        if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );

        if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
        bsrinfo.numbsr=nevents ; 
        
	bstarthi = new Int_t   [nevents] ;
	bstartlo = new Int_t   [nevents] ;
	bstophi  = new Int_t   [nevents] ;
	bstoplo  = new Int_t   [nevents] ;
	bstart   = new Double_t[nevents] ;
	bstop    = new Double_t[nevents] ;
	burstid  = new int[nevents];
       
        cout << "Reading file..." << endl; 
	
	int icol=0;
	char colname[10];
	if (fits_get_colname(fptr, CASESEN, "BSTARTHI", colname, &icol, &status)) printerror(status);	
        if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstarthi,   NULL,&status)) printerror(status);
 	if (fits_get_colname(fptr, CASESEN, "BSTARTLO", colname, &icol, &status)) printerror(status);	
        if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstartlo,   NULL,&status)) printerror(status);
 	if (fits_get_colname(fptr, CASESEN, "BSTOPHI", colname, &icol, &status)) printerror(status);	
        if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstophi,   NULL,&status)) printerror(status);
 	if (fits_get_colname(fptr, CASESEN, "BSTOPLO", colname, &icol, &status)) printerror(status);	
        if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstoplo,   NULL,&status)) printerror(status);
       
/*        if (fits_read_col(fptr,TINT32BIT, 52, 1,1,nevents,0, bstarthi,   NULL,&status)) printerror(status);
        if (fits_read_col(fptr,TINT32BIT, 53, 1,1,nevents,0, bstartlo,   NULL,&status)) printerror(status);
        if (fits_read_col(fptr,TINT32BIT, 56, 1,1,nevents,0, bstophi,   NULL,&status)) printerror(status);
        if (fits_read_col(fptr,TINT32BIT, 57, 1,1,nevents,0, bstoplo,   NULL,&status)) printerror(status);
*/	
	for (int i=0; i<nevents; i++) {
		bstart[i] = bstartlo[i] + bstarthi[i]*1.E6;
		bstop[i]  = bstoplo[i]  + bstophi[i]*1.E6;
		if (bstart[i] > t0) {	// a new burst has been found
			t0 = bstart[i];
			TString burstname("burst_");
			burstname += nburst;
			currentburst = new MCVBurst(burstname.Data());
			currentburst->bstart = bstart[i];
			currentburst->bstop  = bstop[i];
			currentburst->burstid = nburst;
			currentburst->inittime = inittime;
			burstarray->AddLast(currentburst);
			nburst++;
		}
		burstid[i] = nburst-1;
	}
        fits_close_file(fptr, &status);
	
	delete bstarthi;
	delete bstartlo;
	delete bstophi;
	delete bstoplo;
	delete bstart;
	delete bstop;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::Process3908Data()
{ 

	cerr << " fits file name in Process3908Data : " << fileInfo.filename << endl ; 
	cerr << " fits file name as TString fitsname: " << fitsname.Data() << endl ; 


//              MCVFilePKP::Process3909Data() with minor changes 

        Double_t        t0, tprev;
        Short_t         *pd1A;
        Short_t         *pd1B;
        Short_t         *pd2A;
        Short_t         *pd2B;
        char            *flg;
        char            *bar1;
        char            *bar2;
        Short_t         cmult;
        Short_t         cbar;
        bool            flag = TRUE;
        
	unsigned long 	*pktnumb;
	unsigned long 	*sec;
	unsigned long 	*micsec;
	Short_t		*evtnumb;
	
	double auxtime;			// for on-the-fly timing shift
	int jj, colnum;

        Int_t nevents = fileInfo.nevents;
        timearray = new Double_t[nevents];
        pd1A = new Short_t[nevents];
        pd1B = new Short_t[nevents];
        flg = new char[nevents];
        pd2A = new Short_t[nevents];
        pd2B = new Short_t[nevents];
        bar1 = new char[nevents];
        bar2 = new char[nevents];

	pktnumb = new unsigned long[nevents];
	evtnumb = new Short_t[nevents];
	sec = new unsigned long[nevents];
	micsec = new unsigned long[nevents];

        fitsfile *fptr;         // fits file pointer
        //if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );	
        if ( fits_open_file(&fptr, fileInfo.filename, READONLY, &status) )  printerror( status );
        if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );

        cout << "Reading file..." << " events to be read: " << nevents << endl;
	
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )  {
		cout << "could not find column TIME" << endl;
		return;
	}
	if (fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevents, 0, timearray, NULL, &status)) printerror( status );
        if (fits_read_col(fptr, TSHORT, 5, 1, 1, nevents, 0, pd1A, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 6, 1, 1, nevents, 0, pd1B, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 7, 1, 1, nevents, 0, flg, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 8, 1, 1, nevents, 0, pd2A, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 10, 1, 1, nevents, 0, pd2B, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 11, 1, 1, nevents, 0, bar1, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 13, 1, 1, nevents, 0, bar2, NULL, &status)) printerror(status);
        	
	// only for txt file dump if timing error occurs
	if (fits_read_col(fptr, TINT32BIT, 1, 1, 1, nevents, 0, pktnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 2, 1, 1, nevents, 0, evtnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TINT32BIT, 3, 1, 1, nevents, 0, sec, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TINT32BIT, 4, 1, 1, nevents, 0, micsec, NULL, &status)) printerror(status);
	 
	// DO AUTOMATIC TIMING SHIFT?
	bool burstTimeShift = stdconf->GetValue(".burst.time.shift.", 0);

       t0 = timearray[0];
 
//	inittime = t0;
	double toffset = 0.;
	bool go = TRUE;
	if (fileInfo.pkttype == 3910) go = FALSE;
 
        //main loop
        int i=0;
        tprev=0.;
        do {
                if (i%100000==0) {
		        cout << "...processed " << i << " events..." << endl;
			printf(" Current time (us) = %.1f\n", timearray[i]);
		}
                
                // init new event
                for (int j=0; j<30; j++) currevt.bar[j] = -1;
                currevt.Etot = 0.;
                currevt.totEX = 0;
                currevt.totEZ = 0;
                if (timearray[i] < tprev) 
		{
                     cout << "*** Time not monotonic at event " << i << endl;
                     notmonotonic++;
		     if (ferr==NULL) OpenErrorFile();
		     if (evtnumb[i]>1) 
		     fprintf(ferr,"%d \t%d \t%d \t%X \t%X \t%c\n\t%d \t%d \t%X \t%X\n\n", 
			     i-1, pktnumb[i-1], evtnumb[i-1], sec[i-1], micsec[i-1], 
			     sec[i]==sec[i-1] ? 'M' : 'S', pktnumb[i], evtnumb[i], sec[i], micsec[i]);
		}
                tprev = timearray[i];
 //               currevt.time = timearray[i]-t0;        // start from time 0.0 
 
 		auxtime = timearray[i];
		if (burstTimeShift) {	// on-the-fly timing shift if necessary
			jj=i+1;
			do {
				auxtime = timearray[jj++];
			} while (auxtime==timearray[i] && jj<nevents);
		}
		currevt.time = auxtime-t0;
		
                cmult=0;
                do {
                        cbar = bar1[i];
                        if (cbar!=15 && cbar!=31) {
                                // process first bar
                                if (cbar>15) cbar--;
                                currevt.bar[cbar]=cmult;
                                currevt.pdA[cmult]=pd1A[i];
                                currevt.pdB[cmult]=pd1B[i];
                                CalculateEandPos(cmult, cbar, pd1A[i], pd1B[i]);
                                cmult++;
                        } else {
                                // something is wrong
                                cout << "*** bar " << cbar << " detected at event " << i << endl;
                        }
                        if(flg[i]==15) {        
                                cbar = bar2[i];
                                if (cbar!=15 && cbar!=31) {
                                        // process second bar
                                        if (cbar>15) cbar--;
                                        currevt.bar[cbar]=cmult;
                                        currevt.pdA[cmult]=pd2A[i];
                                        currevt.pdB[cmult]=pd2B[i];
                                        CalculateEandPos(cmult, cbar, pd2A[i], pd2B[i]);                                        
                                        cmult++;                                
                                } else {
                                        // something is wrong
                                        cout << "*** bar " << cbar << " detected at event " << i << endl;
                                }                               
                        }
                        i++;
                } while (i<nevents && timearray[i]==tprev);
        
                // save current event
                currevt.mult = cmult;
                tdata->Fill();
                FillHisto(&currevt); 
               
	        //cout << " Into MCVFilePKP3908 : " ;
		//DumpMCALEvent(MCALevt *evt ) ;
	        
                FillHisto3908(&currevt) ;       // Aggiunto rispetto a Process3909Data
                FillHisto3908_ric(&currevt);    // Aggiunto rispetto a Process3909Data 
		
                
                if (flag && nlowDT==1) {
                        cout << "*** first misalignment at event " << tdata->GetEntries() << endl;
                        fileInfo.lowDTevt = tdata->GetEntries();
                        flag = FALSE;
                }
        } while (i<nevents);
        fits_close_file(fptr, &status);
	if (ferr) fclose(ferr);

        cout << "Total number of entries: " << tdata->GetEntries() << endl;
        
        delete timearray;
        delete pd1A;
        delete pd1B;
        delete flg;
        delete pd2A;
        delete pd2B;
        delete bar1;
        delete bar2;
	
	delete pktnumb;
	delete evtnumb;
	delete sec;
	delete micsec;

        return;

}


/*void MCVFilePKP3908::Resize_bsr_ric()
{
        int ie,ig,it ;
	int numpunti ; 
	unsigned long  tempo1, tempo2, tinterval ;
	Double_t t1double, t2double , tintervaldouble, tempo0  ; 
	
	// Setting a different histo size, based on the fits keywords:
	// DATE-OBS, DATE-END, ( tstart and tsyop in fileInfo ) 
	// The real number of time points (timearray[fileInfo.nevents]
	// is known only after the fits is read annd the histo are filled
		
	// this overrides the values in MCVHisto3908 constructor
	// which are: numpunti=(int)(maxsec_ric-minsec_ric)/sitmsecs[it] 
	
	// Here the histo steps depends on the sit integ. time 
	// Times begins from t=0 
	

    tempo1= fileInfo.tstart ;  // seconds 
    tempo2= fileInfo.tstop  ; 
    tinterval = tempo2-tempo1 ;
    
    //t1double = (Double_t) (tempo1) * 1000 ; // msec in histo 
    //t2double = (Double_t) (tempo2) * 1000 ; // usec from L1 are set to msec 
                                              // before: histo->Fill() 
					          
    tempo0 = 0.0 ; 
    tintervaldouble = (Double_t) tinterval * 1000 ; 
     
    cerr << endl ; 
    cerr << " MCVFilePKP3908::Resize_bsr_ric resize histo, from time (sec) : " 
         << tempo1 << " to " << tempo2 << " interval " << tinterval << endl ; 

    for ( it=0; it<nsit; it++) 
    {                      
    
          // num of sits in tinterval, bin size from 1 msec to 8.192 sec
	  // ( num of bins is computed on msec, axis are secs )
    
        numpunti=(int)( (tinterval*1000)/sitmsecs[it] ); 
	
        cerr << " Sit: " << sitmsecs[it] 
	     << " Hist points: " << numpunti << " range (msec): " 
             << tempo0 << " " << tintervaldouble  << endl ; 
	
	gdr_E2_mcal_ric[it]->SetBins( numpunti,tempo0,tintervaldouble); 

        for (int ie=0; ie<nEsegm; ie++) {
        for (int ig=0; ig<ngeom ; ig++) {     
           gdr_mcal_ric[ie][ig][it]->SetBins( numpunti,tempo0,tintervaldouble); 	   
        }}
    }		   
}*/

void MCVFilePKP3908::Resize_bsr_ric()
{
        int ie,ig,it ;
	int numpunti ; 
	int bstophi, bstoplo, nevents;
	Double_t t1double, t2double , tinterval, tempo0, tempo2; 
		
	if (fileInfo.pkttype == 3908) {
		fitsfile *fptr;         // fits file pointer
		//if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );	
		if ( fits_open_file(&fptr, fileInfo.filename, READONLY, &status) )  printerror( status );
		if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
		if (fits_read_col(fptr, TDOUBLE, 14, 1, 1, 1, 0, &inittime, NULL, &status)) printerror(status);
		
		if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
		if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
		
		// allineo le SIT al primo BSTART
//		if (fits_read_col(fptr,TINT32BIT, 52, 1,1,1,0, &bstophi,   NULL,&status)) printerror(status);
//		if (fits_read_col(fptr,TINT32BIT, 53, 1,1,1,0, &bstoplo,   NULL,&status)) printerror(status);
		
		// allineo le SIT all'ultimo BSTOP		
//		if (fits_read_col(fptr,TINT32BIT, 56, nevents-1,1,1,0, &bstophi,   NULL,&status)) printerror(status);
//		if (fits_read_col(fptr,TINT32BIT, 57, nevents-1,1,1,0, &bstoplo,   NULL,&status)) printerror(status);
		
		// test: allineo le SIT all'ultimo BSTART
		if (fits_read_col(fptr,TINT32BIT, 52, nevents-1,1,1,0, &bstophi,   NULL,&status)) printerror(status);
		if (fits_read_col(fptr,TINT32BIT, 53, nevents-1,1,1,0, &bstoplo,   NULL,&status)) printerror(status);
		
		t1double = inittime*1.E-3;			// first event time in ms
		t2double = bstophi*1.E3 + bstoplo*1.E-3;	// first BSTOP time in ms (synchronization mark)
		cout << " initial time: " << t1double << " ms" << endl;
		cout << " last BSTOP time: " << t2double << " ms" << endl;
	
		int n8192 = (int)((t2double - t1double)/8192) + 1;
		tempo0 = t2double - t1double - n8192*8192.;
		n8192 = (int) ((fileInfo.tstop - fileInfo.tstart)/8.192) + 1;
		tempo2 = tempo0 + n8192*8192.;
		tinterval =  tempo2 - tempo0;
	
	cerr << endl ; 
	cerr << " MCVFilePKP3908::Resize_bsr_ric resize histo, from time (sec) : " 
		<< tempo0 << " to " << tempo2 << " num. 8192 ms steps " << n8192 << " tinterval (ms) " << tinterval << endl ; 
	
	for ( it=0; it<nsit; it++) 
	{                      
	
		// num of sits in tinterval, bin size from 1 msec to 8.192 sec
		// ( num of bins is computed on msec, axis are secs )
			
		numpunti=(int)( tinterval/sitmsecs[it] ); 
		
		cerr << " Sit: " << sitmsecs[it] 
		<< " Hist points: " << numpunti << " range (msec): " 
		<< tempo0 << " " << tempo2  << endl ; 
		
		gdr_E2_mcal_ric[it]->SetBins( numpunti,tempo0,tempo2); 
	
		for (int ie=0; ie<nEsegm; ie++) {
		for (int ig=0; ig<ngeom ; ig++) {     
		gdr_mcal_ric[ie][ig][it]->SetBins( numpunti,tempo0,tempo2); 	   
		}}
	}
    } else {
    	// 3909 packet: resize histograms according to DATE_OBS and DATE_END header keywords
		
		// Setting a different histo size, based on the fits keywords:
		// DATE-OBS, DATE-END, ( tstart and tsyop in fileInfo ) 
		// The real number of time points (timearray[fileInfo.nevents]
		// is known only after the fits is read annd the histo are filled
			
		// this overrides the values in MCVHisto3908 constructor
		// which are: numpunti=(int)(maxsec_ric-minsec_ric)/sitmsecs[it] 
		
		// Here the histo steps depends on the sit integ. time 
		// Times begins from t=0 
		
	unsigned long  tempo1, tempo2, tinterval ;
	Double_t t1double, t2double , tintervaldouble, tempo0  ; 
	
	tempo1= fileInfo.tstart ;  // seconds 
	tempo2= fileInfo.tstop  ; 
	tinterval = tempo2-tempo1 ;
	
	//t1double = (Double_t) (tempo1) * 1000 ; // msec in histo 
	//t2double = (Double_t) (tempo2) * 1000 ; // usec from L1 are set to msec 
						// before: histo->Fill() 
							
	tempo0 = 0.0 ; 
	tintervaldouble = (Double_t) tinterval * 1000 ; 
	
	cerr << endl ; 
	cerr << " MCVFilePKP3908::Resize_bsr_ric resize histo, from time (sec) : " 
		<< tempo1 << " to " << tempo2 << " interval " << tinterval << endl ; 
	
	for ( it=0; it<nsit; it++) 
	{                      
	
		// num of sits in tinterval, bin size from 1 msec to 8.192 sec
		// ( num of bins is computed on msec, axis are secs )
	
		numpunti=(int)( (tinterval*1000)/sitmsecs[it] ); 
		
		cerr << " Sit: " << sitmsecs[it] 
		<< " Hist points: " << numpunti << " range (msec): " 
		<< tempo0 << " " << tintervaldouble  << endl ; 
		
		gdr_E2_mcal_ric[it]->SetBins( numpunti,tempo0,tintervaldouble); 
	
		for (int ie=0; ie<nEsegm; ie++) {
		for (int ig=0; ig<ngeom ; ig++) {     
		gdr_mcal_ric[ie][ig][it]->SetBins( numpunti,tempo0,tintervaldouble); 	   
		}}
	}		 	
    }   
}

void MCVFilePKP3908::Resize_bsr_ric(double tinizio, double tfine)
{
       // ############# routine da provare #########
        int ie,ig,it ;
	int numpunti ; 
	Double_t  tempo0,tempo1,tintervaldouble  ; 
	
	// Setting a different histo size, based on input data (sec):
	// in sec, relative to the initial time of the histo
		
	// this overrides the values in MCVHisto3908 constructor
	// which are: numpunti=(int)(maxsec_ric-minsec_ric)/sitmsecs[it] 
	
	// Here the histo steps depends on the sit integ. time 
	// Times begins from t= tinizio, and stop at t=tfine
	// The histo begin from t=0, so
	// tinizio,tfine are: sec from histo beginning
	

    tintervaldouble = tfine-tinizio ;
     
    cerr << endl ; 
    cerr << " MCVFilePKP3908::Resize_bsr_ric resize histo, from time (sec) : " 
         << tinizio << " to " << tfine << " interval " << tintervaldouble << endl ;     tempo0 = tinizio * 1000000.0 ;

    tempo0 = tinizio * 1000000.0 ;
    tempo1 = tfine   * 1000000.0 ; 

    for ( it=0; it<nsit; it++) 
    {                      
    
          // num of sits in tinterval, bin size from 1 msec to 8.192 sec
	  // ( num of bins is computed on msec, axis are secs )
	  // THI->Fill() is in msec units
    
        numpunti=(int)( (tintervaldouble*1000000.0)/sitmsecs[it] ); 
	
        cerr << " Sit(msec): " << sitmsecs[it] 
	     << " Hist points: " << numpunti << " range (msec): " 
             << tempo0 << " " << tempo1  << endl ; 
	
	gdr_E2_mcal_ric[it]->SetBins( numpunti,tempo0,tempo1); 

        for (int ie=0; ie<nEsegm; ie++) {
        for (int ig=0; ig<ngeom ; ig++) {     
           gdr_mcal_ric[ie][ig][it]->SetBins( numpunti,tempo0,tempo1); 	   
        }}
    }		   
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::PrintBurstInfo()
{
	// print info for the bursts recorded in the file
	for (int i=0; i<nburst; i++) {
		MCVBurst *aux = (MCVBurst *) burstarray->At(i);
		aux->PrintInfo();
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::UpdateBurst(int id, MCALbsr *bsr)
{
	MCVBurst *currburst = (MCVBurst *) burstarray->At(id);
	currburst->nume   = bsr->nume;
	currburst->numg   = bsr->numg;
	currburst->bsrsit = bsr->numt;
	currburst->bsr    = bsr->dr;
	currburst->ber    = bsr->br;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP3908::ProcessTEData()
{
	// copiata da MCVFileTE::ProcessData() con modifiche per calcolare i ratemeters di burst

	fitsfile 	*fptr;			// fits file pointer
	int 		istart, nread, nevents, rid=0;
	unsigned long 	timearray[MAXLEN];
	Short_t		pdA[30][MAXLEN];
	Short_t		pdB[30][MAXLEN];
	double		t0;
	double		toffset = 0., dt, tp;
	bool		flag = TRUE;
	Short_t		cmult;
	
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if ( fits_movnam_hdu(fptr, ANY_HDU, "AGILE_Binary", 0, &status) )  printerror( status );
	
	// get the initial time
	if (fits_read_col(fptr, TULONG, 2, 1, 1, 1, 0, timearray, NULL, &status)) printerror(status);
	t0 = timearray[0];
	tp = t0;
	
	nevents = fileInfo.nevents;
	int ncycle = nevents/MAXLEN;
	cout << "Reading file..." << endl;
	for (istart=1; istart<nevents; istart += MAXLEN) {
		if (nevents-istart>MAXLEN) nread = MAXLEN;
		else nread = nevents-istart+1;
		if (fits_read_col(fptr, TULONG, 2, istart, 1, nread, 0, timearray, NULL, &status)) printerror(status);
		
		// get data block
		for (int i=0; i<30; i++) {
			if (fits_read_col(fptr, TSHORT, 3+2*i, istart, 1, nread, 0, &pdA[i][0], NULL, &status)) printerror(status);
			if (fits_read_col(fptr, TSHORT, 4+2*i, istart, 1, nread, 0, &pdB[i][0], NULL, &status)) printerror(status);
		}

		//main loop		
		for (int i=0; i<nread; i++) {
			rid++;
			if (rid%100000==0) cout << "...processed " << rid << " events..." << endl;
			
			// init new event
			cmult=0;
			for (int j=0; j<30; j++) currevt.bar[j] = -1;
			currevt.Etot = 0.;
			currevt.totEX = 0;
			currevt.totEZ = 0;
			if (timearray[i]-tp < -4.E9) {
				cout << "Internal counter reset at event " << istart+i << endl;
				toffset += 4294967296.;
			}
			currevt.time = (timearray[i]+toffset-t0); // multiply * 1.396 for real DFE-TE data.
			tp = timearray[i];
			cmult = 0;
			currevt.mult = 0;
			
			for (int j=0; j<30; j++) {
				if (pdA[j][i] > 0) {
					currevt.bar[j] = cmult;
					currevt.pdA[cmult]=pdA[j][i];
					currevt.pdB[cmult]=pdB[j][i];
					CalculateEandPos(cmult, j, pdA[j][i], pdB[j][i]);
					cmult++;
					currevt.mult = cmult;
				}
			}
			tdata->Fill();
			FillHisto(&currevt);
	        
	                FillHisto3908(&currevt) ;       // Aggiunto rispetto a Process3909Data
	                FillHisto3908_ric(&currevt);    // Aggiunto rispetto a Process3909Data 
			
			if (flag && nlowDT==1) {
				cout << "*** first misalignment at event " << tdata->GetEntries() << endl;
				fileInfo.lowDTevt = tdata->GetEntries();
				flag = FALSE;
			}
		}
		
	}
	fits_close_file(fptr, &status);
	return;
}
