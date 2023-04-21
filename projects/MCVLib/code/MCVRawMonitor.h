//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef MCVLib_MCVRawMonitor
#define MCVLib_MCVRawMonitor

#include <stdlib.h>
#include <math.h>
#include <iostream>

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
#include "MCVGridOffset.h"
#include "MCVCountRate.h"
#include "MCVRetriggering.h"
#include "MCVGlobalRetriggering.h"
#include "MCVSciRM.h"
#include "MCVFitPeak.h"
#include "MCVBurstOffset.h"
#include "MCVElectricalCalib.h"
#include "MCVEnergyThreshold.h"
#include "MCVGridEnergyCal.h"
#include "MCVBarCorrelation.h"
#include "MCVBkgSubtraction.h"
#include "MCVBurstLandauFit.h"

#define ENERGY_BIN 0.020

//! A standard viewer for basic analysis histograms 

/*! This class provides a standard visualization and analysis environment for MCAL data.
Data are passed by means of a histogram collection containing standard histograms, previously created using class MCVFilePKP and possibly loaded from a ROOT file. 
The graphic interface is derived from program mcalview2 and inherited from base class MCVRawMonitor. 
In addition to the base class, basic analysis capabilities are provided both for GRID and BURST data. 
*/

enum MCVCommandIdentifiers {
	M_ALL_SCI_RM,
	M_ALL_FIT_1275KEV,
	M_ALL_FIT_1461KEV,
	M_ALL_FIT_2615KEV,
	M_GRID_OFFSET,
	M_GRID_UPDATE_GLOBAL_OFFSET,
	M_GRID_ENERGY_CAL,
	M_BURST_RETRIGGERING,
	M_BURST_GLOBALRETRIG,
	M_BURST_THRESHOLD,
	M_BURST_OFFSET_A,
	M_BURST_OFFSET_B,
	M_BURST_BARCORR,
	M_BURST_BKG_SUBTRACTION,
	M_BURST_LANDAU_FIT,
	M_CALIB_ELCAL,
	M_EDIT_OFFSET_A,
	M_EDIT_OFFSET_B,
	M_EDIT_UPDATE_GRID_OFFSET,
	M_EDIT_UPDATE_GRID_GAIN,
	M_EDIT_UPDATE_1275KEV,
	M_EDIT_UPDATE_1461KEV,
	M_EDIT_UPDATE_2615KEV,
	M_EDIT_CREATE_CANVAS,
	M_FILE_SAVE_1275KEV,
	M_FILE_SAVE_1461KEV,
	M_FILE_SAVE_2615KEV,
	CLOSE_WINDOW
};

class MCVRawMonitor : public TGTransientFrame {
	
	protected:
		TGCompositeFrame    *fFrame1, *fF0, *fF1, *fF2, *fF3, *fF4, *fF5, *fF6, *fF7, *fF8, *fF9, *fF10;
		TGButton            *fOkButton; 
		TGTab               *fTab;
		TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fL5, *fMenuBarItemLayout, *fMenuBarLayout;
		TList               *fCleanup;
		TRootEmbeddedCanvas *fEc0, *fEc1, *fEc2, *fEc3, *fEc4, *fEc5, *fEc6, *fEc7, *fEc8, *fEc9, *fEc10;
		TObjArray *rPDA, *rPDB, *r2D, *rPos, *rE, *rCR, *rCRDist, *rDT, *rMult, *rSciRM3913, *rawHisto;
		TGNumberEntry *fMaxA;
		TGNumberEntry *fMinA;
		TGGroupFrame *fFitFrame;
		TGTextButton *fGetLimitsA;
		TGLabel *fMinLabelA;
		TGLabel *fMaxLabelA;
		TGTextButton *fFitA;		
		TGDockableFrame	*fMenuDock;
		TGPopupMenu	*fMenuAll;
		TGPopupMenu	*fMenuBurst;
		TGPopupMenu	*fMenuGrid;
		TGPopupMenu	*fMenuCalib;
		TGPopupMenu	*fMenuEdit;
		TGPopupMenu	*fMenuFile;
		TGMenuBar	*fMenuBar;
		TEnv		*conf;		//!<  Current configuration file
		
		MCVFileInfo	*fileInfo;	//!<  Pointer to run info
		float	dAlo, dAhi, dBlo, dBhi;	//!< boundaries for 2D graph plot
		
  	   	// data for peak fitting
		Double_t par[6];
		int curbar;
		double redKi2;
		
		// pointers to analysis routines
		MCVBurstOffset *anBurstOffset;
		MCVBurstOffset *anBurstOffset2;
		MCVFitPeak *anFitPeak;
		MCVFitPeak *anFitPeak2;
		MCVFitPeak *anFitPeak3;
		MCVGridOffset *anGridOffset;
		MCVElectricalCalib *anElectricalCalib;
		MCVEnergyThreshold *anEnergyThreshold;
		MCVGridEnergyCal *anGridEnergyCal;
		MCVBurstLandauFit *anBurstLandauFit;
		
		void FillGlobal();	//!< Draw global dataset information
		void GetFitLimits();
		void DoFit();
	
	public:
		MCVRawMonitor(const TGWindow *p, TGWindow *main, UInt_t w, UInt_t h);  
		virtual 	~MCVRawMonitor();
		virtual void 	CloseWindow();
		void 		FillRaw();  	//!< Fill canvases with standard histograms
		void		InitMenu();	//!< Init the menu entries
		virtual Bool_t 	ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);	//!< Handles menu entries
		void		Set2DLimits(float, float, float, float);//!< Set default boundaries to 2D histograms
		void		SetConf(TEnv *c); 			//!< Set current configuration
		void		SetFileInfo(MCVFileInfo *info);		//!< Set pointer to run info
		void 		SetHistoCollection(TObjArray *hc);	//!< Set pointer to the main histogram collection
		TObjArray	*GetHistoCollection() {return rawHisto;};	//!< Return pointer to the main histogram collection
		MCVFileInfo 	*GetFileInfo() {return fileInfo;};		//!< Return pointer to run info
		ClassDef(MCVRawMonitor,0);	//!< required for ROOT dictionary generation using rootcint
};

#endif
