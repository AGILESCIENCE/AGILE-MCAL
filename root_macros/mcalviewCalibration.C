// Mainframe macro generated from application: ./mcalview2
// By ROOT version 5.01/01 on 2005-07-21 15:53:42

#if !defined( __CINT__) || defined (__MAKECINT__)

#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_mcalviewGUI
#include "mcalviewGUI.h"
#endif
#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_mcalviewDetailed
#include "mcalviewDetailed.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TRootContextMenu
#include "TRootContextMenu.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootCanvas
#include "TRootCanvas.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGuiBldHintsButton
#include "TGuiBldHintsButton.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGuiBldQuickHandler
#include "TGuiBldQuickHandler.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif

#endif

void mcalviewCalibration()
{

   // main frame
   TGMainFrame *frame1832 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   frame1832->SetLayoutBroken(kTRUE);
   TGLabel *lBarId = new TGLabel(frame1832,"Bar id");
   frame1832->AddFrame(lBarId, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lBarId->MoveResize(190,10,58,16);
   TGNumberEntry *fBarId = new TGNumberEntry(frame1832, (Double_t) -141540,5,-1,(TGNumberFormat::EStyle) 5);
   frame1832->AddFrame(fBarId, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fBarId->MoveResize(194,38,60,30);
   TGCheckButton *fChkPDA = new TGCheckButton(frame1832,"PD-A");
   frame1832->AddFrame(fChkPDA, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fChkPDA->MoveResize(200,80,60,17);
   TGCheckButton *fChkPDB = new TGCheckButton(frame1832,"PD-B");
   frame1832->AddFrame(fChkPDB, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fChkPDB->MoveResize(200,120,60,17);

   // embedded canvas
   TRootEmbeddedCanvas *fDisplay = new TRootEmbeddedCanvas(0,frame1832,510,390);
   Int_t wfDisplay = fDisplay->GetCanvasWindowId();
   TCanvas *c134 = new TCanvas("c134", 10, 10, wfDisplay);
   fDisplay->AdoptCanvas(c134);
   frame1832->AddFrame(fDisplay, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fDisplay->MoveResize(270,30,510,390);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valAPrev;
   valAPrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valAPrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valAPrev.fBackground);
   valAPrev.fFillStyle = kFillSolid;
   valAPrev.fFont = ufont->GetFontHandle();
   valAPrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valAPrev, kTRUE);
   TGTextEntry *fGainAPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fGainAPrev->SetMaxLength(255);
   fGainAPrev->SetAlignment(kTextLeft);
   fGainAPrev->SetText("frame1313");
   fGainAPrev->Resize(66,fGainAPrev->GetDefaultHeight());
   frame1832->AddFrame(fGainAPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fGainAPrev->MoveResize(250,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valACurr;
   valACurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valACurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valACurr.fBackground);
   valACurr.fFillStyle = kFillSolid;
   valACurr.fFont = ufont->GetFontHandle();
   valACurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valACurr, kTRUE);
   TGTextEntry *fGainACurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fGainACurr->SetMaxLength(255);
   fGainACurr->SetAlignment(kTextLeft);
   fGainACurr->SetText("frame1314");
   fGainACurr->Resize(66,fGainACurr->GetDefaultHeight());
   frame1832->AddFrame(fGainACurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fGainACurr->MoveResize(330,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valPrev;
   valPrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valPrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valPrev.fBackground);
   valPrev.fFillStyle = kFillSolid;
   valPrev.fFont = ufont->GetFontHandle();
   valPrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valPrev, kTRUE);
   TGTextEntry *fAttAPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fAttAPrev->SetMaxLength(255);
   fAttAPrev->SetAlignment(kTextLeft);
   fAttAPrev->SetText("frame1315");
   fAttAPrev->Resize(66,fAttAPrev->GetDefaultHeight());
   frame1832->AddFrame(fAttAPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fAttAPrev->MoveResize(430,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valCurr;
   valCurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valCurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valCurr.fBackground);
   valCurr.fFillStyle = kFillSolid;
   valCurr.fFont = ufont->GetFontHandle();
   valCurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valCurr, kTRUE);
   TGTextEntry *fAttACurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fAttACurr->SetMaxLength(255);
   fAttACurr->SetAlignment(kTextLeft);
   fAttACurr->SetText("frame1316");
   fAttACurr->Resize(66,fAttACurr->GetDefaultHeight());
   frame1832->AddFrame(fAttACurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fAttACurr->MoveResize(510,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valBPrev;
   valBPrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valBPrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valBPrev.fBackground);
   valBPrev.fFillStyle = kFillSolid;
   valBPrev.fFont = ufont->GetFontHandle();
   valBPrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valBPrev, kTRUE);
   TGTextEntry *fGainBPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fGainBPrev->SetMaxLength(255);
   fGainBPrev->SetAlignment(kTextLeft);
   fGainBPrev->SetText("frame1317");
   fGainBPrev->Resize(66,fGainBPrev->GetDefaultHeight());
   frame1832->AddFrame(fGainBPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fGainBPrev->MoveResize(250,500,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valBCurr;
   valBCurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valBCurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valBCurr.fBackground);
   valBCurr.fFillStyle = kFillSolid;
   valBCurr.fFont = ufont->GetFontHandle();
   valBCurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valBCurr, kTRUE);
   TGTextEntry *fGainBCurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fGainBCurr->SetMaxLength(255);
   fGainBCurr->SetAlignment(kTextLeft);
   fGainBCurr->SetText("frame1318");
   fGainBCurr->Resize(66,fGainBCurr->GetDefaultHeight());
   frame1832->AddFrame(fGainBCurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fGainBCurr->MoveResize(330,500,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valPrev;
   valPrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valPrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valPrev.fBackground);
   valPrev.fFillStyle = kFillSolid;
   valPrev.fFont = ufont->GetFontHandle();
   valPrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valPrev, kTRUE);
   TGTextEntry *fAttBPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fAttBPrev->SetMaxLength(255);
   fAttBPrev->SetAlignment(kTextLeft);
   fAttBPrev->SetText("frame1319");
   fAttBPrev->Resize(66,fAttBPrev->GetDefaultHeight());
   frame1832->AddFrame(fAttBPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fAttBPrev->MoveResize(430,500,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valCurr;
   valCurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valCurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valCurr.fBackground);
   valCurr.fFillStyle = kFillSolid;
   valCurr.fFont = ufont->GetFontHandle();
   valCurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valCurr, kTRUE);
   TGTextEntry *fAttBCurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fAttBCurr->SetMaxLength(255);
   fAttBCurr->SetAlignment(kTextLeft);
   fAttBCurr->SetText("frame1320");
   fAttBCurr->Resize(66,fAttBCurr->GetDefaultHeight());
   frame1832->AddFrame(fAttBCurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fAttBCurr->MoveResize(510,500,66,22);
   TGLabel *lGain = new TGLabel(frame1832,"Gain (ch/MeV)");
   frame1832->AddFrame(lGain, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lGain->MoveResize(280,430,90,16);
   TGLabel *lPDA = new TGLabel(frame1832,"PD-A");
   frame1832->AddFrame(lPDA, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lPDA->MoveResize(180,470,58,16);
   TGLabel *frame1341 = new TGLabel(frame1832,"PD-B");
   frame1832->AddFrame(frame1341, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   frame1341->MoveResize(180,500,58,16);
   TGLabel *l1 = new TGLabel(frame1832,"previous");
   frame1832->AddFrame(l1, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l1->MoveResize(260,450,58,16);
   TGLabel *l2 = new TGLabel(frame1832,"current");
   frame1832->AddFrame(l2, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l2->MoveResize(330,450,58,16);
   TGLabel *lAtt = new TGLabel(frame1832,"Attenuation (1/cm)");
   frame1832->AddFrame(lAtt, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lAtt->MoveResize(450,430,110,16);
   TGLabel *l3 = new TGLabel(frame1832,"previous");
   frame1832->AddFrame(l3, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l3->MoveResize(440,450,58,16);
   TGLabel *l4 = new TGLabel(frame1832,"current");
   frame1832->AddFrame(l4, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l4->MoveResize(510,450,58,16);
   TGTextButton *fFitButton = new TGTextButton(frame1832,"Fit");
   fFitButton->Resize(66,22);
   frame1832->AddFrame(fFitButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fFitButton->MoveResize(190,150,66,22);
   TGTextButton *frame1582 = new TGTextButton(frame1832,"Save Calibration");
   frame1582->Resize(110,22);
   frame1832->AddFrame(frame1582, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   frame1582->MoveResize(180,550,110,22);
   TGTextButton *fOKButton = new TGTextButton(frame1832,"OK");
   fOKButton->Resize(66,22);
   frame1832->AddFrame(fOKButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fOKButton->MoveResize(650,550,66,22);
   TGTextButton *fCancelButton = new TGTextButton(frame1832,"Cancel");
   fCancelButton->Resize(66,22);
   frame1832->AddFrame(fCancelButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fCancelButton->MoveResize(730,550,66,22);
   TGLabel *lLY = new TGLabel(frame1832,"LightYield (e-/keV)");
   frame1832->AddFrame(lLY, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lLY->MoveResize(640,430,130,16);
   TGLabel *l5 = new TGLabel(frame1832,"previous");
   frame1832->AddFrame(l5, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l5->MoveResize(610,450,58,16);
   TGLabel *l6 = new TGLabel(frame1832,"current");
   frame1832->AddFrame(l6, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l6->MoveResize(690,450,58,16);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valrev;
   valrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valrev.fBackground);
   valrev.fFillStyle = kFillSolid;
   valrev.fFont = ufont->GetFontHandle();
   valrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valrev, kTRUE);
   TGTextEntry *fLYAPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fLYAPrev->SetMaxLength(255);
   fLYAPrev->SetAlignment(kTextLeft);
   fLYAPrev->SetText("frame1624");
   fLYAPrev->Resize(66,fLYAPrev->GetDefaultHeight());
   frame1832->AddFrame(fLYAPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fLYAPrev->MoveResize(610,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valrev;
   valrev.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valrev.fForeground);
   gClient->GetColorByName("#c6c3c6",valrev.fBackground);
   valrev.fFillStyle = kFillSolid;
   valrev.fFont = ufont->GetFontHandle();
   valrev.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valrev, kTRUE);
   TGTextEntry *fLYBPrev = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fLYBPrev->SetMaxLength(255);
   fLYBPrev->SetAlignment(kTextLeft);
   fLYBPrev->SetText("frame1721");
   fLYBPrev->Resize(66,fLYBPrev->GetDefaultHeight());
   frame1832->AddFrame(fLYBPrev, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fLYBPrev->MoveResize(610,500,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valurr;
   valurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valurr.fBackground);
   valurr.fFillStyle = kFillSolid;
   valurr.fFont = ufont->GetFontHandle();
   valurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valurr, kTRUE);
   TGTextEntry *fLYACurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fLYACurr->SetMaxLength(255);
   fLYACurr->SetAlignment(kTextLeft);
   fLYACurr->SetText("frame1722");
   fLYACurr->Resize(66,fLYACurr->GetDefaultHeight());
   frame1832->AddFrame(fLYACurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fLYACurr->MoveResize(690,470,66,22);

   ufont = gClient->GetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valurr;
   valurr.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valurr.fForeground);
   gClient->GetColorByName("#c6c3c6",valurr.fBackground);
   valurr.fFillStyle = kFillSolid;
   valurr.fFont = ufont->GetFontHandle();
   valurr.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valurr, kTRUE);
   TGTextEntry *fLYBCurr = new TGTextEntry(frame1832, new TGTextBuffer(15),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fLYBCurr->SetMaxLength(255);
   fLYBCurr->SetAlignment(kTextLeft);
   fLYBCurr->SetText("frame1723");
   fLYBCurr->Resize(66,fLYBCurr->GetDefaultHeight());
   frame1832->AddFrame(fLYBCurr, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fLYBCurr->MoveResize(690,500,66,22);

   // embedded canvas
   TRootEmbeddedCanvas *fXPlane = new TRootEmbeddedCanvas(0,frame1832,160,160);
   Int_t wfXPlane = fXPlane->GetCanvasWindowId();
   TCanvas *c135 = new TCanvas("c135", 10, 10, wfXPlane);
   fXPlane->AdoptCanvas(c135);
   frame1832->AddFrame(fXPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fXPlane->MoveResize(10,60,160,160);
   TGLabel *lXPlane = new TGLabel(frame1832,"X Plane");
   frame1832->AddFrame(lXPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lXPlane->MoveResize(60,40,58,16);
   TGLabel *l7 = new TGLabel(frame1832,"Measurements Status");
   frame1832->AddFrame(l7, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   l7->MoveResize(20,10,130,16);
   TGLabel *lZPlane = new TGLabel(frame1832,"Z Plane");
   frame1832->AddFrame(lZPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   lZPlane->MoveResize(60,240,58,16);

   // embedded canvas
   TRootEmbeddedCanvas *fZPlane = new TRootEmbeddedCanvas(0,frame1832,162,160);
   Int_t wfZPlane = fZPlane->GetCanvasWindowId();
   TCanvas *c136 = new TCanvas("c136", 10, 10, wfZPlane);
   fZPlane->AdoptCanvas(c136);
   frame1832->AddFrame(fZPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fZPlane->MoveResize(10,260,162,160);

   frame1832->MapSubwindows();

   frame1832->Resize(frame1832->GetDefaultSize());
   frame1832->MapWindow();
   frame1832->Resize(803,584);
}  
