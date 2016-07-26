//
// Last update: 26-june-2006
// Description: class to view burst rameters
//
//
// Authors: marcello galli, martino marisaldi june 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef MCVLib_MCVbsrMonitor_ric
#define MCVLib_MCVbsrMonitor_ric

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstring>

#include <TROOT.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TGResourcePool.h>
#include <TGListBox.h>
#include <TGListTree.h>
#include <TGFSContainer.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGDoubleSlider.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TGShutter.h>
#include <TGProgressBar.h>
#include <TGColorSelect.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TEnv.h>
#include <TFile.h>
#include <TKey.h>
#include <TGDockableFrame.h>
#include <TGFontDialog.h>
#include <TLine.h>
#include <TObjArray.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TList.h>
#include <TVirtualPad.h>
#include <TSpectrum.h>
#include <TStyle.h>

#include "MCVFileInfo.h"
#include "MCVBurst.h"
#include "MCVFile.h"

//#define NSIGMA 10

//! A standard viewer for basic bsr analysis histograms 
//! from MCVRawMonitor by Martino, but to see burst ratemeters 

class MCVbsrMonitor_ric : public TGTransientFrame {

   protected:

	MCVFileInfo * fileInfo ; //!<  Pointer to run info
        TEnv	    *  conf    ; //!<  Current configuration file
        TObjArray   * rawbsr_ric   ; //!<  bsr graphs
	TObjArray   * burstarray; //!< burst info collection

   public:

        MCVbsrMonitor_ric(const TGWindow *p, TGWindow *main, UInt_t w, UInt_t h);  
	virtual 	~MCVbsrMonitor_ric();

	void         AddBurstInfo();
	void 	     SetBurstCollection(TObjArray *hb);	//!< Set pointer to the burst info collection
	void 	     SetHistoCollection(TObjArray *hc);	//!< Set pointer to the main histogram collection
	TObjArray *  GetHistoCollection(); //!< Return pointer to the main histogram collection

        void FillRaw();  	//!< Fill canvases with standard histograms
	void PlotAll() ;        //!< Graph all the Mcal bsr
	void PrintRMTable(double tini, double tend);	//!< print a table with RM values

        void SetFileInfo(MCVFileInfo *info);	       //!< Set pointer to run info
        MCVFileInfo  *GetFileInfo() ;//!< Return pointer to run info
	
	ClassDef(MCVbsrMonitor_ric,1);	//!< required for ROOT dictionary generation using rootcint
	//ClassDef(MCVbsrMonitor_ric,0); //!<  0 means streamer ??? The root manual is not clear :(

   private: 
 
	
	char titolo[50] ;    //! canvas title 
        TCanvas * allmcal ;  //! canvas pointer 
	
	int sitwinttime[6]  ;                  //! number of  sits = 2 hard+4 soft
	static const int nsit = 6 ;            //! number of  sits = 2 hard+4 soft
	static const int firstsoftsitnum = 2 ; //! number of the first soft SIT 
        static const int ngeom  = 4 ;          //! num of geom.  parts of the detector   
        static const int nEsegm = 2 ;          //! num of geom.  segmented energy ranges 
	int nsigma;

	
	TH1I* gdr_E2_mcal[6]  ;     //!< MCAL histo for E2: unsegmented x 4 sits  
	TH1I* gdr_mcal[2][4][6]  ;  //!< MCAL histo for E0,1 x 4 geom.segm. x 4 sits 
	TH1I* gdr_sa  [2][4][6]  ;  //!< SA   histo for E0,1 x 4 geom.segm. x 4 sits 
	     
//	TH2F      * Et_mcal ;  //!< 2 dim E-t histograms for bars 
        
	TH1I * findhmax(TH1I * a0 , TH1I * a1) ; //trova il TH1I con ymax 
	TH1I * findhmax(TH1I * a0 , TH1I * a1, TH1I * a2, TH1I * a3) ;  
};

#endif




