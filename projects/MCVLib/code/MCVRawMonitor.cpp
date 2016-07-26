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

#include "MCVRawMonitor.h"

ClassImp(MCVRawMonitor);

MCVRawMonitor::MCVRawMonitor(const TGWindow *p,  TGWindow *main, UInt_t w, UInt_t h) 
    : TGTransientFrame(p, main, w, h, kFitWidth | kFitHeight), rawHisto(NULL), conf(NULL), fileInfo(NULL),
    anBurstOffset(NULL), anBurstOffset2(NULL), anFitPeak(NULL), anFitPeak2(NULL), anFitPeak3(NULL),
    anGridOffset(NULL), anElectricalCalib(NULL), anGridEnergyCal(NULL), anBurstLandauFit(NULL)
{
	// Creates default style
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
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
	
	fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2);
	fL2 = new TGLayoutHints(kLHintsBottom | kLHintsRight, 2, 2, 5, 1);
	fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
	fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
	fL5 = new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5);
	fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
	fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
	
	InitMenu();
	
	fFrame1 = new TGHorizontalFrame(this, 60, 20, kFitWidth | kFitHeight);//kFixedWidth
	
	// Fit histogram frame
	fFitFrame = new TGGroupFrame(fFrame1, "Fit Lab", kHorizontalFrame);      // group frame; entry #0
	fFrame1->AddFrame(fFitFrame, fL5);
	
	fGetLimitsA = new TGTextButton(fFitFrame, "&Get Limits", 100);   // Get current histogram x-axis limits
	fGetLimitsA->Associate(this);
	fFitFrame->AddFrame(fGetLimitsA, fL3);
	
	fMinLabelA = new TGLabel(fFitFrame, "Minimum");  
	fFitFrame->AddFrame(fMinLabelA, fL3);
	
	fMinA = new TGNumberEntry(fFitFrame, 0, 12, 21, (TGNumberFormat::EStyle) 2); // x-axis lower limit for fit, original format: 0
	fMinA->SetLimits(TGNumberFormat::kNELLimitMinMax, 0, 4096);
	fMinA->Associate(this);
	fFitFrame->AddFrame(fMinA, fL3);
	
	fMaxA = new TGNumberEntry(fFitFrame, 0, 12, 21, (TGNumberFormat::EStyle) 2); // x-axis higher limit for fit
	fMaxA->SetLimits(TGNumberFormat::kNELLimitMinMax, 0, 4096);
	fMaxA->Associate(this);
	fFitFrame->AddFrame(fMaxA, fL3);
	
	fMaxLabelA = new TGLabel(fFitFrame, "Maximum");  
	fFitFrame->AddFrame(fMaxLabelA, fL3);
	
	fFitA = new TGTextButton(fFitFrame, "&Fit", 101);   // Do fit button
	fFitA->Associate(this);
	fFitFrame->AddFrame(fFitA, fL3);
	
	// OK and Cancel buttons  
	fOkButton = new TGTextButton(fFrame1, "&Close", CLOSE_WINDOW);
	fOkButton->Associate(this);
	
	
	fFrame1->AddFrame(fOkButton, fL3);	
	fFrame1->Resize(600, fOkButton->GetDefaultHeight()); //150
	AddFrame(fFrame1, fL2); //fL2
	
	fTab = new TGTab(this, 300, 300); //, kFitWidth | kFitHeight);
	
	// Create composite frames
	
	TGCompositeFrame *tf = fTab->AddTab("Global");
	tf->ChangeOptions(kFitWidth | kFitHeight);
	fF0 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc0 = new TRootEmbeddedCanvas("ec0", fF0, 800, 600, kFitWidth | kFitHeight);	//1000, 800)
	TCanvas *c0 = fEc0->GetCanvas();
	c0->Divide(4,4);
	fF0->AddFrame(fEc0, fL4);
	tf->AddFrame(fF0, fL4);//fL3
	
	tf = fTab->AddTab("PD A");
	fF1 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc1 = new TRootEmbeddedCanvas("ec1", fF1, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c1 = fEc1->GetCanvas();
	c1->Divide(6,5);
	fF1->AddFrame(fEc1, fL4);
	tf->AddFrame(fF1, fL4);
	
	tf = fTab->AddTab("PD B");
	fF2 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc2 = new TRootEmbeddedCanvas("ec2", fF2, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c2 = fEc2->GetCanvas();
	c2->Divide(6,5);
	fF2->AddFrame(fEc2, fL4);
	tf->AddFrame(fF2, fL4);
	
	tf = fTab->AddTab("2D histo");
	fF3 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
	fEc3 = new TRootEmbeddedCanvas("ec3", fF3, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c3 = fEc3->GetCanvas();
	c3->Divide(6,5);
	fF3->AddFrame(fEc3, fL4);
	tf->AddFrame(fF3, fL4);
	
	tf = fTab->AddTab("Raw Position");
	fF4 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc4 = new TRootEmbeddedCanvas("ec4", fF4, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c4 = fEc4->GetCanvas();
	c4->Divide(6,5);
	fF4->AddFrame(fEc4, fL4);
	tf->AddFrame(fF4, fL4);
	
	tf = fTab->AddTab("Raw Energy");
	fF5 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc5 = new TRootEmbeddedCanvas("ec5", fF5, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c5 = fEc5->GetCanvas();
	c5->Divide(6,5);
	fF5->AddFrame(fEc5, fL4);
	tf->AddFrame(fF5, fL4);
	
	tf = fTab->AddTab("Count Rate");
	fF6 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc6 = new TRootEmbeddedCanvas("ec6", fF6, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c6 = fEc6->GetCanvas();
	c6->Divide(6,5);
	fF6->AddFrame(fEc6, fL4);
	tf->AddFrame(fF6, fL4);
	
	tf = fTab->AddTab("Count Rate Distr.");
	fF7 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc7 = new TRootEmbeddedCanvas("ec7", fF7, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c7 = fEc7->GetCanvas();
	c7->Divide(6,5);
	fF7->AddFrame(fEc7, fL4);
	tf->AddFrame(fF7, fL4);
	
	tf = fTab->AddTab("Time Separation Distr.");
	fF8 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc8 = new TRootEmbeddedCanvas("ec8", fF8, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c8 = fEc8->GetCanvas();
	c8->Divide(6,5);
	fF8->AddFrame(fEc8, fL4);
	tf->AddFrame(fF8, fL4);
	
	tf = fTab->AddTab("Multiplicity Distr.");
	fF9 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc9 = new TRootEmbeddedCanvas("ec9", fF9, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c9 = fEc9->GetCanvas();
	c9->Divide(6,5);
	fF9->AddFrame(fEc9, fL4);
	tf->AddFrame(fF9, fL4);
	
	tf = fTab->AddTab("Sci. RM.");
	fF10 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fEc10 = new TRootEmbeddedCanvas("ec10", fF10, 800, 600, kFitWidth | kFitHeight);
	TCanvas *c10 = fEc10->GetCanvas();
	c10->Divide(6,4);
	fF10->AddFrame(fEc10, fL4);
	tf->AddFrame(fF10, fL4);
	
	TGLayoutHints *fL5 = new TGLayoutHints(kLHintsBottom | kLHintsExpandX |
						kLHintsExpandY, 2, 2, 5, 1);
	AddFrame(fTab, fL4);//fL5
	
	MapSubwindows();
	Resize();   // resize to default size
	
	// position relative to the parent's window
//	CenterOnParent();
	
	//   SetWindowName("Raw Data Monitor");
	
	dAlo = -10.;
	dAhi = 110.;
	dBlo = -10.;
	dBhi = 110.;
	MapWindow();

}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::InitMenu()
{
	// analysis menu frame
	fMenuDock = new TGDockableFrame(this);
	AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
	
	fMenuAll = new TGPopupMenu(gClient->GetRoot());
	fMenuAll->AddEntry("&Sci. RM", M_ALL_SCI_RM);
	fMenuAll->AddEntry("&Fit Na-22 1.275 MeV peak", M_ALL_FIT_1275KEV);
	fMenuAll->AddEntry("&Fit K-40 1.461 MeV peak", M_ALL_FIT_1461KEV);
	fMenuAll->AddEntry("&Fit Bi-208 2.615 MeV peak", M_ALL_FIT_2615KEV);
	fMenuAll->Associate(this);
	
	fMenuBurst = new TGPopupMenu(gClient->GetRoot());
	fMenuBurst->AddEntry("&Retriggering", M_BURST_RETRIGGERING);
	fMenuBurst->AddEntry("&Global Retriggering", M_BURST_GLOBALRETRIG);
	fMenuBurst->AddEntry("&Energy Threshold", M_BURST_THRESHOLD);
	fMenuBurst->AddEntry("&PD-A offset", M_BURST_OFFSET_A);
	fMenuBurst->AddEntry("&PD-B offset", M_BURST_OFFSET_B);
	fMenuBurst->AddEntry("&Bars Correlation", M_BURST_BARCORR);
	fMenuBurst->AddEntry("&Background Subtraction", M_BURST_BKG_SUBTRACTION);
	fMenuBurst->AddEntry("&MIP Landau Fit", M_BURST_LANDAU_FIT);
	fMenuBurst->Associate(this);
	
	fMenuGrid = new TGPopupMenu(gClient->GetRoot());
	fMenuGrid->AddEntry("&Offset", M_GRID_OFFSET);
	fMenuGrid->Associate(this);
	fMenuGrid->AddEntry("&Update Global Offset", M_GRID_UPDATE_GLOBAL_OFFSET);
	fMenuGrid->Associate(this);
	fMenuGrid->AddEntry("&Energy Calibration", M_GRID_ENERGY_CAL);
	fMenuGrid->Associate(this);
	
	
	fMenuCalib = new TGPopupMenu(gClient->GetRoot());
	fMenuCalib->AddEntry("&Do Electrical Calibration", M_CALIB_ELCAL);
	fMenuCalib->Associate(this);
	
	fMenuEdit = new TGPopupMenu(gClient->GetRoot());
	fMenuEdit->AddEntry("&Update PD-A offset", M_EDIT_OFFSET_A);
	fMenuEdit->AddEntry("&Update PD-B offset", M_EDIT_OFFSET_B);
	fMenuEdit->AddSeparator();
	fMenuEdit->AddEntry("&Update GRID offset", M_EDIT_UPDATE_GRID_OFFSET);
	fMenuEdit->AddEntry("&Update GRID gain", M_EDIT_UPDATE_GRID_GAIN);
	fMenuEdit->AddSeparator();
	fMenuEdit->AddEntry("&Update Na-22 1.275 MeV peak", M_EDIT_UPDATE_1275KEV);
	fMenuEdit->AddEntry("&Update K-40 1.461 MeV peak", M_EDIT_UPDATE_1461KEV);
	fMenuEdit->AddEntry("&Update Bi-208 2.615 MeV peak", M_EDIT_UPDATE_2615KEV);
	fMenuEdit->AddSeparator();
	fMenuEdit->AddEntry("&Create empty canvas", M_EDIT_CREATE_CANVAS);
	fMenuEdit->Associate(this);
	
	fMenuFile = new TGPopupMenu(gClient->GetRoot());
	fMenuFile->AddEntry("&Save Na-22 1.275 MeV peak", M_FILE_SAVE_1275KEV);
	fMenuFile->AddEntry("&Save K-40 1.461 MeV peak", M_FILE_SAVE_1461KEV);
	fMenuFile->AddEntry("&Save Bi-208 2.615 MeV peak", M_FILE_SAVE_2615KEV);
	fMenuFile->Associate(this);
	
	fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
	fMenuBar->AddPopup("&ALL", fMenuAll, fMenuBarItemLayout);
	fMenuBar->AddPopup("&BURST", fMenuBurst, fMenuBarItemLayout);
	fMenuBar->AddPopup("&GRID", fMenuGrid, fMenuBarItemLayout);
	fMenuBar->AddPopup("&CALIB", fMenuCalib, fMenuBarItemLayout);
	fMenuBar->AddPopup("&Edit", fMenuEdit, fMenuBarItemLayout);
	fMenuBar->AddPopup("&File", fMenuFile, fMenuBarItemLayout);
	fMenuDock->AddFrame(fMenuBar, fMenuBarLayout);
	return;
}

//
//--------------------------------------------------------------------
//

MCVRawMonitor::~MCVRawMonitor()
{
	// Delete test dialog widgets.
	fCleanup->Delete();
	delete fCleanup;
	delete fOkButton;
	delete fTab;
	delete fL1; 
	delete fL2; 
	delete fL3; 
	delete fL4;
	delete fEc0; 
	delete fEc1; 
	delete fEc2; 
	delete fEc3; 
	delete fEc4; 
	delete fEc5; 
	delete fEc6; 
	delete fEc7;
	delete fEc8;
	delete fEc9;
	delete fMinA;
	delete fMaxA;
	delete fFitFrame;
	delete fFrame1;
	if(fileInfo) delete fileInfo;
}

//
//--------------------------------------------------------------------
//

Bool_t MCVRawMonitor::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
	// Process messages coming from widgets associated with the dialog.
	switch (GET_MSG(msg)) {
	case kC_COMMAND:
	
		switch (GET_SUBMSG(msg)) {
		case kCM_MENU:
		switch (parm1) {
	
			case M_ALL_SCI_RM:
				//cout << "Scientific ratemeters calculations..." << endl;  
				MCVSciRM *anSciRM;
				anSciRM = new MCVSciRM(rawHisto, fileInfo);
				anSciRM->Calculate();
				anSciRM->PrintResults();
			break;
			case M_ALL_FIT_1275KEV:
				//cout << "Fitting Na22 1.275 MeV peak..." << endl;  
				if (anFitPeak) delete anFitPeak;
				anFitPeak = new MCVFitPeak(1.275, rawHisto, fileInfo);
				anFitPeak->SetCanvases(fEc5->GetCanvas());
				anFitPeak->Calculate();
				anFitPeak->PrintResults();
			break;
			case M_ALL_FIT_1461KEV:
				//cout << "Fitting K40 1.461 MeV peak..." << endl;  
				if (anFitPeak2) delete anFitPeak2;
				anFitPeak2 = new MCVFitPeak(1.461, rawHisto, fileInfo);
				anFitPeak2->SetCanvases(fEc5->GetCanvas());
				anFitPeak2->SetUpperDE(0.4);
				anFitPeak2->SetLowerDE(0.2);
				anFitPeak2->Calculate();
				anFitPeak2->PrintResults();
			break;
			case M_ALL_FIT_2615KEV:
				//cout << "Fitting Bi208 2.615 MeV peak..." << endl;  
				if (anFitPeak3) delete anFitPeak3;
				anFitPeak3 = new MCVFitPeak(2.615, rawHisto, fileInfo);
				anFitPeak3->SetCanvases(fEc5->GetCanvas());
				anFitPeak3->SetUpperDE(0.4);
				anFitPeak3->SetLowerDE(0.2);
				anFitPeak3->Calculate();
				anFitPeak3->PrintResults();
			break;
			case M_GRID_OFFSET:
				//cout << "Grid offset calculations..." << endl;  
				if (anGridOffset) delete anGridOffset;
				anGridOffset = new MCVGridOffset(rawHisto, fileInfo);
				anGridOffset->SetCanvases(fEc1->GetCanvas(), fEc2->GetCanvas());
				anGridOffset->Calculate();
				anGridOffset->PrintResults();
				anGridOffset->SaveToFile();
			break;
			case M_GRID_UPDATE_GLOBAL_OFFSET:
				if (anGridOffset) anGridOffset->UpdateGlobalFile();
			break;
			case M_GRID_ENERGY_CAL:
				//cout << "Grid Energy Calibration with muons..." << endl;  
				if (anGridEnergyCal) delete anGridEnergyCal;
				anGridEnergyCal = new MCVGridEnergyCal(rawHisto, fileInfo);
				anGridEnergyCal->SetCanvases(fEc5->GetCanvas());
				anGridEnergyCal->Calculate();
				anGridEnergyCal->PrintResults();
				anGridEnergyCal->SaveToFile();			
			break;
			case M_BURST_RETRIGGERING:
				//cout << "Burst search for retriggering..." << endl;
				MCVRetriggering *anRetriggering;
				anRetriggering = new MCVRetriggering(rawHisto, fileInfo);
				anRetriggering->SetCanvases(fEc8->GetCanvas());
				anRetriggering->Calculate();
				anRetriggering->PrintResults();
			break;
			case M_BURST_GLOBALRETRIG:
				//cout << "Burst search for retriggering on global histo..." << endl;
				MCVGlobalRetriggering *anGlobalRetrig;
				anGlobalRetrig = new MCVGlobalRetriggering(rawHisto, fileInfo);
				anGlobalRetrig->SetCanvases(fEc0->GetCanvas());
				anGlobalRetrig->Calculate();
				anGlobalRetrig->PrintResults();
			break;
			case M_BURST_THRESHOLD:
				//cout << "Energy threshold evaluation..." << endl;
				MCVEnergyThreshold *anEnergyThreshold;
				anEnergyThreshold = new MCVEnergyThreshold(rawHisto, fileInfo);
				anEnergyThreshold->SetCanvases(fEc5->GetCanvas());
				anEnergyThreshold->Calculate();
				anEnergyThreshold->PrintResults();
				anEnergyThreshold->SaveToFile();
			break;
			case M_BURST_OFFSET_A:
				//cout << "Calculate BURST offset on PD-A..." << endl;
				if (anBurstOffset) delete anBurstOffset;
				anBurstOffset = new MCVBurstOffset(0, rawHisto, fileInfo);
				anBurstOffset->SetCanvases(fEc1->GetCanvas());
				anBurstOffset->Calculate();
				anBurstOffset->PrintResults();
				anBurstOffset->SaveToFile();
			break;
			case M_BURST_OFFSET_B:
				//cout << "Calculate BURST offset on PD-B..." << endl;
				if (anBurstOffset2) delete anBurstOffset2;
				anBurstOffset2 = new MCVBurstOffset(1, rawHisto, fileInfo);
				anBurstOffset2->SetCanvases(fEc2->GetCanvas());
				anBurstOffset2->Calculate();
				anBurstOffset2->PrintResults();
				anBurstOffset2->SaveToFile();
			break;
			case M_BURST_BARCORR:
				//cout << "Calculate BURST offset on PD-B..." << endl;
				MCVBarCorrelation *anBarCorrelation;
				anBarCorrelation = new MCVBarCorrelation(rawHisto, fileInfo);
				anBarCorrelation->SetCanvases(fEc0->GetCanvas());
				anBarCorrelation->Calculate();
				anBarCorrelation->PrintResults();
				//delete anBarCorrelation;
			break;
			case M_BURST_BKG_SUBTRACTION:
				MCVBkgSubtraction *anBkgSubtraction;
				anBkgSubtraction = new MCVBkgSubtraction(rawHisto, fileInfo);
				anBkgSubtraction->SetCanvases(fEc0->GetCanvas(), fEc5->GetCanvas());
				anBkgSubtraction->Calculate();
				anBkgSubtraction->PrintResults();
			break;
			case M_BURST_LANDAU_FIT:
				if (anBurstLandauFit) delete anBurstLandauFit;
				anBurstLandauFit = new MCVBurstLandauFit(rawHisto, fileInfo);
				anBurstLandauFit->SetCanvases(fEc5->GetCanvas(), fEc0->GetCanvas());
				anBurstLandauFit->Calculate();
				anBurstLandauFit->PrintResults();
			break;
			case M_CALIB_ELCAL:
				//cout << "Do Electrical Calibration..." << endl;
				if (anElectricalCalib) delete anElectricalCalib;
				anElectricalCalib = new MCVElectricalCalib(rawHisto, fileInfo);
				anElectricalCalib->SetCanvases(fEc1->GetCanvas(), fEc2->GetCanvas());
				anElectricalCalib->Calculate();
				anElectricalCalib->PrintResults();
				anElectricalCalib->SaveToFile();			
			break;
			case M_EDIT_OFFSET_A:
				cout << "Update BURST PD-A offset on calibration file..." << endl;
				if (anBurstOffset) anBurstOffset->UpdateCalibrationFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_OFFSET_B:
				cout << "Update BURST PD-B offset on calibration file..." << endl;
				if (anBurstOffset2) anBurstOffset2->UpdateCalibrationFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_UPDATE_GRID_OFFSET:
				if (anGridOffset) anGridOffset->UpdateCalibrationFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_UPDATE_GRID_GAIN:
				if (anGridEnergyCal) anGridEnergyCal->UpdateCalibrationFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_UPDATE_1275KEV:
				cout << "Update Na22 1.275 MeV peak for bar " << curbar << endl;
				if (anFitPeak) anFitPeak->UpdateFit(curbar, par[3], par[4], par[2], redKi2);
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_UPDATE_1461KEV:
				cout << "Update K40 1.461 MeV peaks for bar " << curbar << endl;
				if (anFitPeak2) anFitPeak2->UpdateFit(curbar, par[3], par[4], par[2], redKi2);
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_UPDATE_2615KEV:
				cout << "Update Bi208 2.615 MeV peaks for bar " << curbar << endl;
				if (anFitPeak3) anFitPeak3->UpdateFit(curbar, par[3], par[4], par[2], redKi2);
				else cout << "Perform analysis first!" << endl;
			break;
			case M_EDIT_CREATE_CANVAS:
				cout << "Creating empty canvas " << endl;
				TCanvas *cextra;
				cextra = new TCanvas("cextra", "", 800, 600);
			break;
			case M_FILE_SAVE_1275KEV:
				cout << "Saving Na22 1.275 MeV peaks to file..." << endl;
				if (anFitPeak) anFitPeak->SaveToFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_FILE_SAVE_1461KEV:
				cout << "Saving K40 1.461 MeV peaks to file..." << endl;
				if (anFitPeak2) anFitPeak2->SaveToFile();
				else cout << "Perform analysis first!" << endl;
			break;
			case M_FILE_SAVE_2615KEV:
				cout << "Saving Bi208 2.615 MeV peaks to file..." << endl;
				if (anFitPeak3) anFitPeak3->SaveToFile();
				else cout << "Perform analysis first!" << endl;
			break;
			
			
			default:
			break;
		}
			
		case kCM_BUTTON:
		switch(parm1) {
			case CLOSE_WINDOW:
				CloseWindow();
				break;
			case 100:
				GetFitLimits();
			break; 
			case 101:
				DoFit();
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

void MCVRawMonitor::CloseWindow()
{
	// Called when window is closed via the window manager.
	if (GetMain()==NULL) gApplication->Terminate(0);
	else DeleteWindow();
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::FillRaw()
{   
	// fill canvases with standard histograms
	rPDA =(TObjArray *) rawHisto->At(0);
	rPDB =(TObjArray *) rawHisto->At(1);
	r2D  =(TObjArray *) rawHisto->At(2);
	rPos =(TObjArray *) rawHisto->At(3);
	rE   =(TObjArray *) rawHisto->At(4);
	rCR  =(TObjArray *) rawHisto->At(5);
	rDT  =(TObjArray *) rawHisto->At(6);
	rMult  =(TObjArray *) rawHisto->At(7);
	rCRDist  =(TObjArray *) rawHisto->At(8);	
	rSciRM3913 =(TObjArray *) rawHisto->FindObject("rawSciRM3913");
	FillGlobal();
	
	// draw histograms
	TCanvas *c1 = fEc1->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c1->cd(i+1);
		TH1F *aux = (TH1F *) rPDA->At(i);
		aux->SetAxisRange(0., 300., "X");
		if (aux->GetEntries()) aux->Draw();
		c1->Update();
	}
	
	TCanvas *c2 = fEc2->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c2->cd(i+1);
		TH1F *aux = (TH1F *) rPDB->At(i);
		aux->SetAxisRange(0., 300., "X");
		if (aux->GetEntries()) aux->Draw();
		c2->Update();
	}
	
	TCanvas *c3 = fEc3->GetCanvas(); 
	TLine *l1, *l2;
	for (int i=0; i<30; i++) {	
		c3->cd(i+1);
		TH2F *aux = (TH2F *) r2D->At(i);
		if (aux->GetEntries()) {
			gPad->SetLogz(kTRUE);
			gPad->SetRightMargin(0.2);
			aux->Draw("COLZ");
			if (fileInfo) {	// draw offset straight lines
				aux->SetAxisRange((fileInfo->oA)[i]+dAlo, (fileInfo->oA)[i]+dAhi, "X");
				aux->SetAxisRange((fileInfo->oB)[i]+dBlo, (fileInfo->oB)[i]+dBhi, "Y");
				l1 = new TLine((fileInfo->oA)[i], (fileInfo->oB)[i]-10., (fileInfo->oA)[i], (fileInfo->oB)[i]+110.);  
				l1->Draw();
				l2 = new TLine((fileInfo->oA)[i]-10., (fileInfo->oB)[i], (fileInfo->oA)[i]+110., (fileInfo->oB)[i]);  
				l2->Draw();
			}
		}
		c3->Update();
	}

	TCanvas *c4 = fEc4->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c4->cd(i+1);
		TH1F *aux = (TH1F *) rPos->At(i);
		if (aux->GetEntries()) aux->Draw();
		c4->Update();
	}
	
	TCanvas *c5 = fEc5->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c5->cd(i+1);
		TH1F *aux = (TH1F *) rE->At(i);
		aux->SetAxisRange(0., 5., "X");
		if (aux->GetEntries()) aux->Draw();
		c5->Update();
	}
	
	TCanvas *c6 = fEc6->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c6->cd(i+1);
		TH1F *aux = (TH1F *) rCR->At(i);
		if (aux->GetEntries()) aux->Draw();
		c6->Update();
	}
	
	TCanvas *c7 = fEc7->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c7->cd(i+1);
		TH1F *aux = (TH1F *) rCRDist->At(i);
		if (aux->GetEntries()) aux->Draw();
		c7->Update();
	}
	
	TCanvas *c8 = fEc8->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c8->cd(i+1);
		TH1F *aux = (TH1F *) rDT->At(i);
		if (aux->GetEntries()) aux->Draw();
		c8->Update();
	}
	c8->Update();
	
	TCanvas *c9 = fEc9->GetCanvas(); 
	for (int i=0; i<30; i++) {
		c9->cd(i+1);
		TH1F *aux = (TH1F *) rMult->At(i);
		if (aux->GetEntries()) {
			gPad->SetLogy(kTRUE);
			aux->Draw();
		}
	}
	c9->Update();
	
	TCanvas *c10 = fEc10->GetCanvas(); 
	for (int i=0; i<11; i++) {
		c10->cd(i+1);
		TGraph *gaux = (TGraph *) rSciRM3913->At(i);
		if (gaux) {
//			gaux->GetXaxis()->SetLimits(0., fileInfo->teff < 5000. ? 5000. : 1.1*fileInfo->teff);
			int npt = gaux->GetN();
			gaux->GetXaxis()->SetLimits(TMath::MinElement(npt, gaux->GetX()), TMath::MaxElement(npt, gaux->GetX()));
			double maxy = TMath::MaxElement(npt, gaux->GetY());
			gaux->GetHistogram()->SetMaximum(1.1*maxy);
			gaux->GetHistogram()->SetMinimum(0.);
			gaux->Draw("ALP");
		}
	}
	for (int i=11; i<22; i++) {
		c10->cd(i+2);
		TGraph *gaux = (TGraph *) rSciRM3913->At(i);
		if (gaux) {
//			gaux->GetXaxis()->SetLimits(0., fileInfo->teff < 5000. ? 5000. : 1.1*fileInfo->teff);
			int npt = gaux->GetN();
			gaux->GetXaxis()->SetLimits(TMath::MinElement(npt, gaux->GetX()), TMath::MaxElement(npt, gaux->GetX()));
			double maxy = TMath::MaxElement(npt, gaux->GetY());
			gaux->GetHistogram()->SetMaximum(1.1*maxy);
			gaux->GetHistogram()->SetMinimum(0.);
			gaux->Draw("ALP");
		}
	}
	c10->Update();
//	FitCountRateHisto();
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::FillGlobal()
{   
	// draw global dataset information
	TCanvas *c0 = fEc0->GetCanvas();
	
	c0->cd(1);
	TGraphErrors *gCRX = (TGraphErrors *) rawHisto->FindObject("gCRX");
	if (gCRX) {
		// these lines are needed because these TGraph do not autoscale after passage to root 5.14! why???
		// cannot reproduce it with a macro
		// with programs hkmonitor and hkviewer it works!
		gCRX->GetXaxis()->SetLimits(0.,16.);
		double maxy = TMath::MaxElement(15, gCRX->GetY());
		gCRX->GetHistogram()->SetMaximum(1.1*maxy);
		gCRX->GetHistogram()->SetMinimum(0.);
		gCRX->Draw("AP");
	} else cout << "*** Cannot find object gCRX" << endl;
	
	c0->cd(2);
	TGraphErrors *gCRZ = (TGraphErrors *) rawHisto->FindObject("gCRZ");
	if (gCRZ) {
		gCRZ->GetXaxis()->SetLimits(0.,16.);
		double maxy = TMath::MaxElement(15, gCRZ->GetY());
		gCRZ->GetHistogram()->SetMaximum(1.1*maxy);
		gCRZ->GetHistogram()->SetMinimum(0.);
		gCRZ->Draw("AP");
	} else cout << "*** Cannot find object gCRZ" << endl;
	
	c0->cd(3);
	TGraphErrors *gMultX = (TGraphErrors *) rawHisto->FindObject("gMultX");
	if (gMultX) {
		gMultX->GetXaxis()->SetLimits(0.,16.);
		gMultX->GetHistogram()->SetMaximum(1.);
		gMultX->GetHistogram()->SetMinimum(0.);
		gMultX->Draw("AP");
	} else cout << "*** Cannot find object gMultX" << endl;
	
	c0->cd(4);
	TGraphErrors *gMultZ = (TGraphErrors *) rawHisto->FindObject("gMultZ");
	if (gMultZ) {
		gMultZ->GetXaxis()->SetLimits(0.,16.);
		gMultZ->GetHistogram()->SetMaximum(1.);
		gMultZ->GetHistogram()->SetMinimum(0.);
		gMultZ->Draw("AP");
	} else cout << "*** Cannot find object gMultZ" << endl;
	
	// plot events distribution histograms
	c0->cd(5);
   	gPad->SetRightMargin(0.2);
	TH2F *hX = (TH2F *) rawHisto->FindObject("hX");
	if (hX) hX->Draw("COLZ");
	else cout << "*** Cannot find object hX" << endl;

	c0->cd(6);
   	gPad->SetRightMargin(0.2);
	TH2F *hZ = (TH2F *) rawHisto->FindObject("hZ");
	if (hZ) hZ->Draw("COLZ");
	else cout << "*** Cannot find object hZ" << endl;
	
	// plot calculated scientific ratemeters
	c0->cd(7);
	gPad->SetLogy(kTRUE);
	TH1F *sciRMX = (TH1F *) rawHisto->FindObject("sciRMX");
	if (sciRMX) sciRMX->Draw();
	else cout << "*** Cannot find object sciRMX" << endl;
	TH1F *sciRMZ = (TH1F *) rawHisto->FindObject("sciRMZ");
	sciRMZ->SetLineColor(2);
	if (sciRMZ) sciRMZ->Draw("SAME");
	else cout << "*** Cannot find object sciRMZ" << endl;
	TH1F *hRMXR = (TH1F *) rawHisto->FindObject("hRMXR");
	if (hRMXR) {
		hRMXR->SetLineStyle(2);
		hRMXR->Draw("SAME");
	} else cout << "*** Cannot find object hRMZR" << endl;
	TH1F *hRMZR = (TH1F *) rawHisto->FindObject("hRMZR");
	if (hRMZR) {
		hRMZR->SetLineColor(2);
		hRMZR->SetLineStyle(2);
		hRMZR->Draw("SAME");
	} else cout << "*** Cannot find object hRMZR" << endl;
	
	c0->cd(8);
	gPad->SetLogx(kTRUE);
	gPad->SetLogy(kTRUE);
	TGraphErrors *gRMX = NULL;
	if (fileInfo->pkttype != 3913) {
		gRMX = (TGraphErrors *) rawHisto->FindObject("gRMX");
		if (gRMX) {
			gRMX->GetXaxis()->SetLimits(0.1, 2000.);
			double maxy = TMath::MaxElement(11, gRMX->GetY());
			gRMX->GetHistogram()->SetMaximum(1.1*maxy);
			double miny = TMath::MinElement(11, gRMX->GetY());
			gRMX->GetHistogram()->SetMinimum(0.9*miny);
			gRMX->GetXaxis()->SetTitle("Energy (MeV)");
			gRMX->GetXaxis()->SetTitleOffset(1.4);
			gRMX->GetXaxis()->CenterTitle();
			gRMX->GetYaxis()->SetTitle("Counts/s");
			gRMX->GetYaxis()->CenterTitle();
			gRMX->GetYaxis()->SetTitleOffset(1.5);
			gRMX->Draw("AP");
		} else cout << "*** Cannot find object gRMX" << endl;
	}
	TGraphErrors *gRMX3913 = NULL;
	gRMX3913 = (TGraphErrors *) rawHisto->FindObject("gRMX3913");
	if (gRMX3913) {
		if (!gRMX) {
			gRMX3913->GetXaxis()->SetLimits(0.1, 2000.);
			double maxy = TMath::MaxElement(11, gRMX3913->GetY());
			gRMX3913->GetHistogram()->SetMaximum(1.1*maxy);
			double miny = TMath::MinElement(11, gRMX3913->GetY());
			gRMX3913->GetHistogram()->SetMinimum(0.9*miny);
			gRMX3913->GetXaxis()->SetTitle("Energy (MeV)");
			gRMX3913->GetXaxis()->SetTitleOffset(1.4);
			gRMX3913->GetXaxis()->CenterTitle();
			gRMX3913->GetYaxis()->SetTitle("Counts/s");
			gRMX3913->GetYaxis()->CenterTitle();
			gRMX3913->GetYaxis()->SetTitleOffset(1.5);
			gRMX3913->Draw("AP");	
		} else {
			gRMX3913->SetLineColor(2);
			gRMX3913->Draw("PSAME");
		}
	} else cout << "*** Cannot find object gRMX3913" << endl;
	
	c0->cd(9);
	gPad->SetLogx(kTRUE);
	gPad->SetLogy(kTRUE);
	TGraphErrors *gRMZ = NULL;
	if (fileInfo->pkttype != 3913) {
		gRMZ = (TGraphErrors *) rawHisto->FindObject("gRMZ");
		if (gRMZ) {
			gRMZ->GetXaxis()->SetLimits(0.1, 2000.);
			double maxy = TMath::MaxElement(11, gRMZ->GetY());
			gRMZ->GetHistogram()->SetMaximum(1.1*maxy);
			double miny = TMath::MinElement(11, gRMZ->GetY());
			gRMZ->GetHistogram()->SetMinimum(0.9*miny);
			gRMZ->GetXaxis()->SetTitle("Energy (MeV)");
			gRMZ->GetXaxis()->SetTitleOffset(1.4);
			gRMZ->GetXaxis()->CenterTitle();
			gRMZ->GetYaxis()->SetTitle("Counts/s");
			gRMZ->GetYaxis()->CenterTitle();
			gRMZ->GetYaxis()->SetTitleOffset(1.5);
			gRMZ->Draw("AP");
		} else cout << "*** Cannot find object gRMZ" << endl;
	}
	TGraphErrors *gRMZ3913 = NULL;
	gRMZ3913 = (TGraphErrors *) rawHisto->FindObject("gRMZ3913");
	if (gRMZ3913) {
		if (!gRMZ) {
			gRMZ3913->GetXaxis()->SetLimits(0.1, 2000.);
			double maxy = TMath::MaxElement(11, gRMZ3913->GetY());
			gRMZ3913->GetHistogram()->SetMaximum(1.1*maxy);
			double miny = TMath::MinElement(11, gRMZ3913->GetY());
			gRMZ3913->GetHistogram()->SetMinimum(0.9*miny);
			gRMZ3913->GetXaxis()->SetTitle("Energy (MeV)");
			gRMZ3913->GetXaxis()->SetTitleOffset(1.4);
			gRMZ3913->GetXaxis()->CenterTitle();
			gRMZ3913->GetYaxis()->SetTitle("Counts/s");
			gRMZ3913->GetYaxis()->CenterTitle();
			gRMZ3913->GetYaxis()->SetTitleOffset(1.5);
			gRMZ3913->Draw("AP");	
		} else {
			gRMZ3913->SetLineColor(2);
			gRMZ3913->Draw("PSAME");
		}
	} else cout << "*** Cannot find object gRMZ3913" << endl;
	
	// draw global time difference between successive events
	c0->cd(10);	
	TH1F *htotDT = (TH1F *) rawHisto->At(11);
	if (htotDT->GetEntries())htotDT->Draw();
	
	// draw bar correlation histograms
	c0->cd(11);
	TH2F *barcorr = (TH2F *) rawHisto->FindObject("barcorr");
	if (barcorr) barcorr->Draw("COLZ");
	else cout << "*** Cannot find object barcorr" << endl;
	
	c0->cd(12);
	TH2F *barcorrLowDT = (TH2F *) rawHisto->FindObject("barcorrLowDT");
	if (barcorrLowDT) barcorrLowDT->Draw("COLZ");
	else cout << "*** Cannot find object barcorrLowDT" << endl;
	
	// draw total MCAL count rate
	c0->cd(13);		
	TH1F *tCR = (TH1F *) rawHisto->At(14);
	if (tCR->GetEntries()) tCR->Draw();

	// draw total MCAL multiplicity distribution
	c0->cd(14);		
	gPad->SetLogy(kTRUE);
	TH1F *tMult = (TH1F *) rawHisto->At(15);
	if (tMult->GetEntries()) tMult->Draw();

	// draw total MCAL energy spectrum
	c0->cd(15);
	TH1F *tE = (TH1F *) rawHisto->At(16);
	gPad->SetLogx(kTRUE);
	gPad->SetLogy(kTRUE);
	if (tE->GetEntries()){
		tE->Draw();
	}
	
	// draw expected sub-ms trigger distribution
	c0->cd(16);
	TH1F *hSubMS = (TH1F *) rawHisto->FindObject("hSubMS");
	if (hSubMS) {
		if (hSubMS->GetEntries()) hSubMS->Draw();
	} else cout << "*** MCVRawMonitor::FillGlobal() : cannot find object hSubMS" << endl;
	
	c0->Update();
	
	// default count rate analysis
	if (fileInfo->pkttype == 3909 || fileInfo->pkttype == 3902) {
		cout << "Count rate analysis..." << endl;  
		MCVCountRate *anCountRate;
		anCountRate = new MCVCountRate(rawHisto, fileInfo);
		anCountRate->Calculate();
		anCountRate->PrintResults();
	}
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::SetConf(TEnv *c)
{
	conf = c;
	char parameterName[50];
	int ii;
	
	for (UInt_t i=0; i<30; i++) {  
		if (i<15) ii=i;
		else ii=i+1;
/*		sprintf(parameterName, ".Gain.Bar%02d.A", ii);
		u0A[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Gain.Bar%02d.B", ii);
		u0B[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Alpha.Bar%02d.A", ii);
		alphaA[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Alpha.Bar%02d.B", ii);
		alphaB[i] = conf->GetValue(parameterName, 1.);  
*/
		sprintf(parameterName, ".Offset.Bar%02d.A", ii);
		(fileInfo->oA)[i] = conf->GetValue(parameterName, 0.);  
		sprintf(parameterName, ".Offset.Bar%02d.B", ii);
		(fileInfo->oB)[i] = conf->GetValue(parameterName, 0.);   
	}
	
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::SetFileInfo(MCVFileInfo *info)
{
//	if (!fileInfo) fileInfo = new MCVFileInfo();
	fileInfo=info;
//	analyzer->SetFileInfo(info);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::Set2DLimits(float al, float ah, float bl, float bh)
{
	dAlo = al;
	dAhi = ah;
	dBlo = bl;
	dBhi = bh;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::SetHistoCollection(TObjArray *hc) 
{
	rawHisto = hc;
//	analyzer->SetHistoCollection(hc);	
	return;
}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::GetFitLimits()
{
	TVirtualPad *curPad;
	TList *curList;
	Int_t size;
	TH1F *curHisto;
	TString *className;

	curPad  = gROOT->GetSelectedPad();
	curList = curPad->GetListOfPrimitives();
	size = curList->GetSize();
	if (size>=2) className = new TString((curList->At(1))->ClassName());
	if (className->BeginsWith("TH1F")) {
		curHisto = (TH1F *) curList->At(1);
		double xmin  = curHisto->GetBinLowEdge((curHisto->GetXaxis())->GetFirst());// curHisto->GetMinimum();
		double xmax  = curHisto->GetBinLowEdge((curHisto->GetXaxis())->GetLast()); // curHisto->GetMaximum();
		cout << "Working on histogram " << curHisto->GetName() << " between Xmin = " << xmin << "  and Xmax = " << xmax << endl;
		fMinA->SetNumber(xmin);
		fMaxA->SetNumber(xmax);
	}

}

//
//--------------------------------------------------------------------
//

void MCVRawMonitor::DoFit()
{
	TVirtualPad *curPad;
	TList *curList;
	Int_t size;
	TH1F *curHisto;
	TString *className;
	TString *hname;
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;

     	double xmin = fMinA->GetNumber();  // Int_t
     	double xmax = fMaxA->GetNumber();
	curPad  = gROOT->GetSelectedPad();
	int ctab;
	sscanf(curPad->GetName(), "ec%d_%d", &ctab, &curbar);
	curbar--;
	curList = curPad->GetListOfPrimitives();
	size = curList->GetSize();
	if (size>=2) className = new TString((curList->At(1))->ClassName());
	if (className->BeginsWith("TH1F")) {
		curHisto = (TH1F *) curList->At(1);
		cout << "Fitting histogram " << curHisto->GetName() << " between Xmin = " << xmin << "  and Xmax = " << xmax << " on bar " << curbar << endl;
		peak  = new TF1("peak","gaus", xmin + 0.3*(xmax-xmin),  xmax - 0.3*(xmax-xmin)); // (int)
		hname = new TString(curHisto->GetName());
		if (hname->Contains("POS")) {
			cout << "Position histogram: fitting with linear background" << endl;
			bkg   = new TF1("bkg", "pol1", xmin, xmax);
			total = new TF1("total","pol1(0)+gaus(2)", xmin, xmax);
		} else {
			bkg   = new TF1("bkg", "expo", xmin, xmax);
			total = new TF1("total","expo(0)+gaus(2)", xmin, xmax);
		}
		curHisto->Fit("peak","R0+");
		curHisto->Fit("bkg","R0+");
		peak->GetParameters(&par[2]);
		bkg->GetParameters(&par[0]);
		total->SetLineColor(2);
		total->SetParameters(par);
   		curHisto->Fit("total","R0+");
		total->GetParameters(par);
		redKi2 = total->GetChisquare()/(xmax-xmin)*ENERGY_BIN;
		cout << "Reduced Ki^2 = " << redKi2 << endl;
		total->SetLineWidth(2);
		total->SetLineColor(2);
		total->Draw("same");
		
		peak->SetRange(xmin, xmax);
		peak->SetParameters(&par[2]);
		peak->SetLineWidth(1);
		peak->SetLineColor(3);
		peak->Draw("same");
		
		bkg->SetParameters(par);
		bkg->SetLineWidth(1);
		bkg->SetLineColor(4);
		bkg->Draw("same");
		curPad->Update();	
	}
	else
		cout << "This is not a 1D histogram!" << endl; 
}
