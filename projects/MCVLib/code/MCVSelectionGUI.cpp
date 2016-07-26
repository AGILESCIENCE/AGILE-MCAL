//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "MCVSelectionGUI.h"

ClassImp(MCVSelectionGUI);

const char *const MCVSelectionGUI::groupName[6] = {"PD A", "PD B", "Position  (cm)", "Energy (MeV)", "Time", "Multiplicity"};
const double MCVSelectionGUI::minval[6] = {MINPHA, MINPHA, -100., 0., 0., 1.};
const double MCVSelectionGUI::maxval[6] = {MAXPHA, MAXPHA, 100., 500., 20000., 30.};  
const int MCVSelectionGUI::format[6] = {0, 0, 2, 2, 3, 0};
const int MCVSelectionGUI::groupButtonNumber = 7;
const int MCVSelectionGUI::nobjects = 6;

MCVSelectionGUI::MCVSelectionGUI(const TGWindow *p, TGWindow *main, UInt_t w, UInt_t h, TTree *ind, MCVFileInfo *in, TObjArray *rh) : TGTransientFrame(p, main, w, h), indata(NULL), ininfo(NULL), selector(NULL), inhisto(NULL)
{
	if (ind) indata = ind;
	if (in)  ininfo  = in;
	if (rh)  inhisto = rh;
	nsel = 0;

	fFrame1 = new TGHorizontalFrame(this, 60, 20, kFixedWidth);
	
	fOkButton = new TGTextButton(fFrame1, "&Close", 1);
	fOkButton->Associate(this);
	
	fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2);
	fL2 = new TGLayoutHints(kLHintsBottom | kLHintsRight, 2, 2, 5, 1);
	
	fFrame1->AddFrame(fOkButton, fL1);
	fFrame1->Resize(150, fOkButton->GetDefaultHeight());
	AddFrame(fFrame1, fL2);
	
	fTab = new TGTab(this, 300, 300);
	fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
	fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
	fL5 = new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 15, 5, 15);  // 5 --> 15
	AddFrame(fTab, fL3);
	
	// Create selection frame
	TGCompositeFrame *tf = fTab->AddTab("Selection");
	tf->SetLayoutManager(new TGHorizontalLayout(tf));
	
	fF0 = new TGGroupFrame(tf, "Bars", kHorizontalFrame);      
	tf->AddFrame(fF0, fL3);
	fChkArr = new TObjArray(30,0);
	char chkName[12];
	fF1 = new TGCompositeFrame(fF0, 60, 20, kVerticalFrame);  
	TGTextButton *SelAllBars;
	SelAllBars = new TGTextButton(fF1, "&Select All", 10);
	SelAllBars->Associate(this);
	fF1->AddFrame(SelAllBars,fL3);
	
	TGCheckButton *fChk1;   
	for (int i=1; i<=15; i++) {
		sprintf(chkName, "Bar X%d", i);
		fChk1 = new TGCheckButton(fF1, chkName, 0);
		fChkArr->AddLast(fChk1);
		fF1->AddFrame(fChk1, fL3);
	}
	fF0->AddFrame(fF1, fL3);  
	
	fF2 = new TGCompositeFrame(fF0, 60, 20, kVerticalFrame);  
	TGTextButton *SelNoBars;
	SelNoBars = new TGTextButton(fF2, "&Select None", 11);
	SelNoBars->Associate(this);
	fF2->AddFrame(SelNoBars,fL3);
	
	for (int i=1; i<=15; i++) {
		sprintf(chkName, "Bar Z%d", i);
		fChk1 = new TGCheckButton(fF2, chkName, 0);
		fChkArr->AddLast(fChk1);
		fF2->AddFrame(fChk1, fL3);
	}
	fF0->AddFrame(fF2, fL5);  
	/*
	// Coincidence pattern selection frame
	fFCoinc = new TGGroupFrame(tf, "Coincidence Pattern", kHorizontalFrame);      
	tf->AddFrame(fFCoinc, fL3);
	fChkCoinc = new TObjArray(30,0);
	fFCoinc1 = new TGCompositeFrame(fFCoinc, 60, 20, kVerticalFrame); 
	fFCoinc1->AddFrame(new TGTextButton(fFCoinc1, "&Select All", 0), fL3); 
	for (int i=1; i<=15; i++) {
	sprintf(chkName, "Bar X%d", i);
	fChk1 = new TGCheckButton(fFCoinc1, chkName, 0);
	fChkCoinc->AddLast(fChk1);
	fFCoinc1->AddFrame(fChk1, fL3);
	}
	fFCoinc->AddFrame(fFCoinc1, fL3);  
	
	fFCoinc2 = new TGCompositeFrame(fFCoinc, 60, 20, kVerticalFrame);  
	fFCoinc2->AddFrame(new TGTextButton(fFCoinc2, "&Select None", 0), fL3); 
	for (int i=1; i<=15; i++) {
	sprintf(chkName, "Bar Z%d", i);
	fChk1 = new TGCheckButton(fFCoinc2, chkName, 0);
	fChkCoinc->AddLast(fChk1);
	fFCoinc2->AddFrame(fChk1, fL3);
	}
	fFCoinc->AddFrame(fFCoinc2, fL5); 
	*/
	// other selections container frame
	fFButtons = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	tf->AddFrame(fFButtons, fL3);
	
	// init selection frames
	fGroupArr   = new TObjArray(groupButtonNumber*nobjects,0);
	
	TGGroupFrame *fFGroupFrame;
	TGCheckButton *fGroupChk;
	TGTextButton *fGroupDefault;
	TGLabel *fGroupLabel1, *fGroupLabel2;
	TGNumberEntry *fGroupEntryMin, *fGroupEntryMax; 
	
	for (int i=0; i<nobjects; i++) {  
		fFGroupFrame = new TGGroupFrame(fFButtons, groupName[i], kHorizontalFrame);      // group frame; entry #0
		fFButtons->AddFrame(fFGroupFrame, fL3);
		fGroupArr->AddLast(fFGroupFrame);
		
		fGroupChk = new TGCheckButton(fFGroupFrame, "Enable", 10 + 10*i); // enable check button; entry #1
		sprintf(command, "EnableSelection(=%d)", 10 + 10*i);
		fGroupChk->Connect("Clicked()", "MCVSelectionGUI", this, command);
		fFGroupFrame->AddFrame(fGroupChk, fL3);
		fGroupArr->AddLast(fGroupChk);
		
		fGroupDefault = new TGTextButton(fFGroupFrame, "&Default", 11 + 10*i);   // set default button; entry #2
		fGroupDefault->Associate(this);
		sprintf(command, "SetDefault(=%d)", i);
		fGroupDefault->Connect("Clicked()", "MCVSelectionGUI", this, command);
		fFGroupFrame->AddFrame(fGroupDefault, fL3);
		fGroupArr->AddLast(fGroupDefault);
		
		fGroupLabel1 = new TGLabel(fFGroupFrame, "Minimum");  // label 1; entry #3
		fFGroupFrame->AddFrame(fGroupLabel1, fL3);
		fGroupArr->AddLast(fGroupLabel1);
		
		fGroupEntryMin = new TGNumberEntry(fFGroupFrame, minval[i], 12, 21, (TGNumberFormat::EStyle) format[i]); // minimum value number entry box; entry #4
		fGroupEntryMin->SetLimits(TGNumberFormat::kNELLimitMinMax, minval[i], maxval[i]);
		fGroupEntryMin->Associate(this);
		fFGroupFrame->AddFrame(fGroupEntryMin, fL3);
		fGroupArr->AddLast(fGroupEntryMin);
		
		fGroupEntryMax = new TGNumberEntry(fFGroupFrame, maxval[i], 12, 21, (TGNumberFormat::EStyle) format[i]); // maximum value number entry box; entry #5
		fGroupEntryMax->SetLimits(TGNumberFormat::kNELLimitMinMax, minval[i], maxval[i]);
		fGroupEntryMax->Associate(this);
		fFGroupFrame->AddFrame(fGroupEntryMax, fL3);
		fGroupArr->AddLast(fGroupEntryMax);
		
		fGroupLabel2 = new TGLabel(fFGroupFrame, "Maximum");  // label 2; entry #6
		fFGroupFrame->AddFrame(fGroupLabel2, fL3);
		fGroupArr->AddLast(fGroupLabel2);
		
		fGroupEntryMin->SetState(kFALSE);  
		fGroupEntryMax->SetState(kFALSE);
		fGroupDefault->SetState(kButtonDisabled);
	}

// command button frame
	fFCommands = new TGCompositeFrame(fFButtons, 60, 20, kHorizontalFrame);
	fFButtons->AddFrame(fFCommands, fL3);
	
/*	fLoadButton = new TGTextButton(fFCommands, "&Load", 100);   // load selection configuration button
	fLoadButton->Associate(this);
	fFCommands->AddFrame(fLoadButton, fL3);
	
	fSaveButton = new TGTextButton(fFCommands, "&Save", 101);   // save selection configuration button
	fSaveButton->Associate(this);
	fFCommands->AddFrame(fSaveButton, fL3); */
	
	fApplyButton = new TGTextButton(fFCommands, "&Apply", 102);   // apply selection configuration button
	fApplyButton->Associate(this);
	fFCommands->AddFrame(fApplyButton, fL3);
	
	fZeroSupButton = new TGTextButton(fFCommands, "&Zero Sup.", 107);   // apply selection configuration button
	fZeroSupButton->Associate(this);
	fFCommands->AddFrame(fZeroSupButton, fL3);
	
	fRelToOffset = new TGCheckButton(fFCommands, "Relative to offset", 103); // enable check button; entry #1
	fRelToOffset->SetState(kButtonDown);
	fFCommands->AddFrame(fRelToOffset, fL3);
	
	// frame to enable/disable strong (exclusive) condition on bar address, position and energy
	fFStrongFrame = new TGGroupFrame(fFCommands, "Strong Condition on", kHorizontalFrame); 
	fStrongBar = new TGCheckButton(fFStrongFrame, "Bars", 104); 
	fStrongBar->SetState(kButtonUp);
	fFStrongFrame->AddFrame(fStrongBar, fL3);
	fStrongPos = new TGCheckButton(fFStrongFrame, "Position", 105); 
	fStrongPos->SetState(kButtonUp);
	fFStrongFrame->AddFrame(fStrongPos, fL3);
	fStrongE = new TGCheckButton(fFStrongFrame, "Energy", 106); 
	fStrongE->SetState(kButtonUp);
	fFStrongFrame->AddFrame(fStrongE, fL3);
	fFCommands->AddFrame(fFStrongFrame, fL3);
	
/*	fSaveTracksChk = new TGCheckButton(fFCommands, "Save Tracks", 103); // enable check button; entry #1
	fSaveTracksChk->SetState(kButtonUp);
	fFCommands->AddFrame(fSaveTracksChk, fL3);
 */ 
	// ------------ common comands --------------
		
	MapSubwindows();
	Resize();   // resize to default size
	
	// position relative to the parent's window
	CenterOnParent();
	
	SetWindowName("Selection GUI");
	
	MapWindow();
}

//
//--------------------------------------------------------------------
//

MCVSelectionGUI::~MCVSelectionGUI()
{

	if (selector) delete selector;
}

//
//--------------------------------------------------------------------
//

void MCVSelectionGUI::CloseWindow()
{
	// Called when window is closed via the window manager.
	if (GetMain()==NULL) gApplication->Terminate(0);
	else DeleteWindow();
}

//
//--------------------------------------------------------------------
//

Bool_t MCVSelectionGUI::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Process messages coming from widgets associated with the dialog.
   switch (GET_MSG(msg)) {
      case kC_COMMAND:

         switch (GET_SUBMSG(msg)) {
            case kCM_BUTTON:
               switch(parm1) {
                  case 1:
                  case 2:
		      CloseWindow();
                     break;
		  case 10:
			for (int i=0; i<30; i++) {
				TGCheckButton *curChk;
				curChk = (TGCheckButton *) fChkArr->At(i);
				curChk->SetState(kButtonDown);
			}
		  break;
		  case 11:
			for (int i=0; i<30; i++) {
				TGCheckButton *curChk;
				curChk = (TGCheckButton *) fChkArr->At(i);
				curChk->SetState(kButtonUp);
			}
		  break;
  		  case 102:
		  	ApplySelection();
			sel.PrintSelection();
			if (indata && ininfo) {
				selector = new MCVTree(indata, ininfo, inhisto);
				selector->DoSelection(&sel);
				selector->SaveData();
				delete selector;
				cout << "All data processed" << endl;
			}
		  break;
   		  case 107:
			if (indata && ininfo) {
				selector = new MCVTree(indata, ininfo, inhisto);
				selector->DoZeroSuppression();
				selector->SaveData();
				delete selector;
				cout << "All data processed" << endl;
			}
		  break;
              default:
                     break;
               }
           default:
               break;
         }
         break;

      default:
         break;
   }
   return kTRUE;
}

//
//--------------------------------------------------------------------
//

void MCVSelectionGUI::EnableSelection(int id)
{
// enable button group selection

	int i = id/10 - 1;
	if (((TGCheckButton *) fGroupArr->At(i*groupButtonNumber+1))->GetState() == kButtonDown) {
	//	cout << "Detailed monitor - Selection Tab: enable selection #"<< i << endl;
		((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+4))->SetState(kTRUE);  
		((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+5))->SetState(kTRUE);
		((TGTextButton *) fGroupArr->At(i*groupButtonNumber+2))->SetState(kButtonUp);
	} else {
	//	cout << "Detailed monitor - Selection Tab: disable selection #"<< i << endl;
		((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+4))->SetState(kFALSE);
		((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+5))->SetState(kFALSE);
		((TGTextButton *) fGroupArr->At(i*groupButtonNumber+2))->SetState(kButtonDisabled);
	}
}

//
//--------------------------------------------------------------------
//

void MCVSelectionGUI::SetDefault(int i)
{
// set default values (lower and upper limits) to button group number entry widjets

	//    cout << "Detailed monitor - Selection Tab: set defaults in frame #"<< i << endl;
	((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+4))->SetNumber(minval[i]);  
	((TGNumberEntry *) fGroupArr->At(i*groupButtonNumber+5))->SetNumber(maxval[i]);
}

//
//--------------------------------------------------------------------
//

void MCVSelectionGUI::ApplySelection()
{
	sel.SetDefault();
	if (fRelToOffset->GetState() == kButtonDown) sel.relToOffset = TRUE;
	else sel.relToOffset = FALSE;
	if (fStrongE->GetState() == kButtonDown) sel.isEStrong = TRUE;
	else sel.isEStrong = FALSE;
	if (fStrongPos->GetState() == kButtonDown) sel.isPosStrong = TRUE;
	else sel.isPosStrong = FALSE;
	if (fStrongBar->GetState() == kButtonDown) sel.isBarStrong = TRUE;
	else sel.isBarStrong = FALSE;
	
	
	// get bars
	for (int i=0; i<30; i++) {
		TGCheckButton *curChk;
		curChk = (TGCheckButton *) fChkArr->At(i);
		if (curChk->GetState() == kButtonDown) {
			(sel.bars)[i] = 1;
			(sel.nbars)++;
		}
	}
	
	// get PD A limits
	if (((TGCheckButton *) fGroupArr->At(1))->GetState() == kButtonDown) {
		sel.pdAEnable = TRUE;
		sel.minA = ((TGNumberEntry *) fGroupArr->At(4))->GetNumber();
		sel.maxA = ((TGNumberEntry *) fGroupArr->At(5))->GetNumber();
	}
	
	// get PD B limits
	if (((TGCheckButton *) fGroupArr->At(8))->GetState() == kButtonDown) {
		sel.pdBEnable = TRUE;	
		sel.minB = ((TGNumberEntry *) fGroupArr->At(11))->GetNumber();
		sel.maxB = ((TGNumberEntry *) fGroupArr->At(12))->GetNumber();
	}
	
	// get Position limits
	if (((TGCheckButton *) fGroupArr->At(15))->GetState() == kButtonDown) {
		sel.posEnable = TRUE;		
		sel.minPos = ((TGNumberEntry *) fGroupArr->At(18))->GetNumber();
		sel.maxPos = ((TGNumberEntry *) fGroupArr->At(19))->GetNumber();
	}
	
	// get Energy limits
	if (((TGCheckButton *) fGroupArr->At(22))->GetState() == kButtonDown) {
		sel.energyEnable = TRUE;		
		sel.minE = ((TGNumberEntry *) fGroupArr->At(25))->GetNumber();
		sel.maxE = ((TGNumberEntry *) fGroupArr->At(26))->GetNumber();
	}
	
	// get Time limits 
	if (((TGCheckButton *) fGroupArr->At(29))->GetState() == kButtonDown) {
		sel.timeEnable = TRUE;		
		sel.minTime = ((TGNumberEntry *) fGroupArr->At(32))->GetNumber();
		sel.maxTime = ((TGNumberEntry *) fGroupArr->At(33))->GetNumber();
	}
	
	// get Multiplicity limits
	if (((TGCheckButton *) fGroupArr->At(36))->GetState() == kButtonDown) {
		sel.multEnable = TRUE;		
		sel.minMult = ((TGNumberEntry *) fGroupArr->At(39))->GetNumber();
		sel.maxMult = ((TGNumberEntry *) fGroupArr->At(40))->GetNumber();
	}
	
	TString selname = "sel_";
	selname += nsel++;
	sel.SetName(selname.Data());
	return;
}
