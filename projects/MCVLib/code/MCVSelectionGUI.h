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

#ifndef MCVLib_MCVSelectionGUI
#define MCVLib_MCVSelectionGUI

#include <stdlib.h>
#include <stdio.h>
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
#include <TGButtonGroup.h>
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
#include <TString.h>
#include <TFile.h>
#include <TKey.h>
#include <TGDockableFrame.h>
#include <TGFontDialog.h>

#include <TObjArray.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TGraph.h>
#include <TLine.h>

#include "MCVSelection.h"
#include "MCVFileInfo.h"
#include "MCVTree.h"

//#define CALFILE "MCALBurstCalibration.cal"

class MCVSelectionGUI : public TGTransientFrame {

private:
	TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fL5;
	TObjArray           *fChkArr, *fChkCoinc, *fGroupArr;
	TObjArray		*detHisto, *rawHisto, *selHisto;
	TH1F		*hPDA, *hPDB, *hPos, *hE, *hCR;
	TH2F		*h2D;
    
    	// selection tab
	TGGroupFrame        *fF0, *fFCoinc, *fFPDA, *fFStrongFrame;
	TGCompositeFrame    *fFrame1, *fF1, *fF2, *fFCoinc1, *fFCoinc2, *fFButtons, *fFCommands, *fFData;
	TGButton            *fOkButton; // *fLoadButton, *fSaveButton; 
	TGButton            *fZeroSupButton;
	TGButton            *fApplyButton;
	TGTab               *fTab;
	TGCheckButton	    *fRelToOffset, *fStrongBar, *fStrongPos, *fStrongE;
   
	static const char *const groupName[6];
	static const double minval[6];
	static const double maxval[6];
	static const int format[6];
	static const int groupButtonNumber;
	static const int nobjects;
	char command[50];
 
	TTree	 *indata;	//!< Input data
	TObjArray *inhisto;	//!< Input dataset histogram collection
	MCVFileInfo *ininfo;	//!< Pointer to information on input dataset
	MCVSelection sel;	//!< Selection
	MCVTree	*selector;	//!< Pointer to object that performs selection
	int	nsel;		//!< Number of current selection
    
	void 	ApplySelection();

public:
	MCVSelectionGUI(const TGWindow *p, TGWindow *main, UInt_t w, UInt_t h, TTree *ind=NULL, MCVFileInfo *in=NULL, TObjArray *rh=NULL);
	~MCVSelectionGUI();
	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
	void 	EnableSelection(int id);
	void 	SetDefault(int id);
	ClassDef(MCVSelectionGUI,0);
};

#endif
