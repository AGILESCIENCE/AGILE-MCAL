/********************************************************************
* code/MCVDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error code/MCVDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableMCVDict();
extern void G__cpp_setup_inheritanceMCVDict();
extern void G__cpp_setup_typetableMCVDict();
extern void G__cpp_setup_memvarMCVDict();
extern void G__cpp_setup_globalMCVDict();
extern void G__cpp_setup_memfuncMCVDict();
extern void G__cpp_setup_funcMCVDict();
extern void G__set_cpp_environmentMCVDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "MCVFile.h"
#include "MCVFileTE.h"
#include "MCVFilePKP.h"
#include "MCVHisto.h"
#include "MCVRawMonitor.h"
#include "MCVFileInfo.h"
#include "MCVAnalysis.h"
#include "MCVStdAnalysis.h"
#include "MCVGridOffset.h"
#include "MCVCountRate.h"
#include "MCVRetriggering.h"
#include "MCVGridEnergyCal.h"
#include "MCVSciRM.h"
#include "MCVEnergyThreshold.h"
#include "MCVBarMonitor.h"
#include "MCVTree.h"
#include "MCVSelection.h"
#include "MCVSelectionGUI.h"
#include "MCVFitPeak.h"
#include "MCVBurstOffset.h"
#include "MCVElectricalCalib.h"
#include "MCVHisto3908.h"
#include "MCVFilePKP3908.h"
#include "MCVbsrMonitor.h"
#include "MCVbsrMonitor_ric.h"
#include "MCVBurst.h"
#include "MCVAntiCoincidence.h"
#include "MCVCalibration.h"
#include "MCVSubMS.h"
#include "MCVGlobalRetriggering.h"
#include "MCVBarCorrelation.h"
#include "MCVBkgSubtraction.h"
#include "MCVBurstLandauFit.h"
#include "GRB.h"
#include "GRB3908.h"
#include "GRB3913.h"
#include "GRB3913XZ.h"
#include "MCVFile3916.h"
#include "MCVFile3201.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__MCVDictLN_TClass;
extern G__linked_taginfo G__MCVDictLN_TBuffer;
extern G__linked_taginfo G__MCVDictLN_TMemberInspector;
extern G__linked_taginfo G__MCVDictLN_TObject;
extern G__linked_taginfo G__MCVDictLN_TNamed;
extern G__linked_taginfo G__MCVDictLN_TString;
extern G__linked_taginfo G__MCVDictLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__MCVDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__MCVDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__MCVDictLN_TList;
extern G__linked_taginfo G__MCVDictLN_TObjArray;
extern G__linked_taginfo G__MCVDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__MCVDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__MCVDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__MCVDictLN_TFile;
extern G__linked_taginfo G__MCVDictLN_TCanvas;
extern G__linked_taginfo G__MCVDictLN_TStyle;
extern G__linked_taginfo G__MCVDictLN_TEnv;
extern G__linked_taginfo G__MCVDictLN_TTree;
extern G__linked_taginfo G__MCVDictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR;
extern G__linked_taginfo G__MCVDictLN_TF1;
extern G__linked_taginfo G__MCVDictLN_TRandom3;
extern G__linked_taginfo G__MCVDictLN_MCVFileInfo;
extern G__linked_taginfo G__MCVDictLN_MCVbsrInfo;
extern G__linked_taginfo G__MCVDictLN_MCALpkt;
extern G__linked_taginfo G__MCVDictLN_MCALbsr;
extern G__linked_taginfo G__MCVDictLN_MCALevt;
extern G__linked_taginfo G__MCVDictLN_MCVFile;
extern G__linked_taginfo G__MCVDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__MCVDictLN_TH1D;
extern G__linked_taginfo G__MCVDictLN_TH1I;
extern G__linked_taginfo G__MCVDictLN_TH1F;
extern G__linked_taginfo G__MCVDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__MCVDictLN_TH2F;
extern G__linked_taginfo G__MCVDictLN_TH2D;
extern G__linked_taginfo G__MCVDictLN_TGraph;
extern G__linked_taginfo G__MCVDictLN_TGraphErrors;
extern G__linked_taginfo G__MCVDictLN_MCVHisto;
extern G__linked_taginfo G__MCVDictLN_MCVFileTE;
extern G__linked_taginfo G__MCVDictLN_MCVFilePKP;
extern G__linked_taginfo G__MCVDictLN_TQObject;
extern G__linked_taginfo G__MCVDictLN_pairlEunsignedsPintcOintgR;
extern G__linked_taginfo G__MCVDictLN_vectorlEpairlEunsignedsPintcOintgRcOallocatorlEpairlEunsignedsPintcOintgRsPgRsPgR;
extern G__linked_taginfo G__MCVDictLN_reverse_iteratorlEvectorlEpairlEunsignedsPintcOintgRcOallocatorlEpairlEunsignedsPintcOintgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__MCVDictLN_TGObject;
extern G__linked_taginfo G__MCVDictLN_TGWindow;
extern G__linked_taginfo G__MCVDictLN_TGFrame;
extern G__linked_taginfo G__MCVDictLN_TGCompositeFrame;
extern G__linked_taginfo G__MCVDictLN_TGLayoutHints;
extern G__linked_taginfo G__MCVDictLN_TGTextButton;
extern G__linked_taginfo G__MCVDictLN_TGMainFrame;
extern G__linked_taginfo G__MCVDictLN_TGTransientFrame;
extern G__linked_taginfo G__MCVDictLN_TGGroupFrame;
extern G__linked_taginfo G__MCVDictLN_TGPopupMenu;
extern G__linked_taginfo G__MCVDictLN_TGButton;
extern G__linked_taginfo G__MCVDictLN_TGCheckButton;
extern G__linked_taginfo G__MCVDictLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR;
extern G__linked_taginfo G__MCVDictLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__MCVDictLN_TGLabel;
extern G__linked_taginfo G__MCVDictLN_TGNumberEntry;
extern G__linked_taginfo G__MCVDictLN_TGMenuBar;
extern G__linked_taginfo G__MCVDictLN_TGTab;
extern G__linked_taginfo G__MCVDictLN_TRootEmbeddedCanvas;
extern G__linked_taginfo G__MCVDictLN_TPad;
extern G__linked_taginfo G__MCVDictLN_vectorlETStringcOallocatorlETStringgRsPgR;
extern G__linked_taginfo G__MCVDictLN_reverse_iteratorlEvectorlETStringcOallocatorlETStringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__MCVDictLN_TGDockableFrame;
extern G__linked_taginfo G__MCVDictLN_MCVStdAnalysis;
extern G__linked_taginfo G__MCVDictLN_MCVGridOffset;
extern G__linked_taginfo G__MCVDictLN_MCVCountRate;
extern G__linked_taginfo G__MCVDictLN_MCVRetriggering;
extern G__linked_taginfo G__MCVDictLN_MCVGlobalRetriggering;
extern G__linked_taginfo G__MCVDictLN_MCVSciRM;
extern G__linked_taginfo G__MCVDictLN_MCVFitPeak;
extern G__linked_taginfo G__MCVDictLN_MCVBurstOffset;
extern G__linked_taginfo G__MCVDictLN_MCVElectricalCalib;
extern G__linked_taginfo G__MCVDictLN_MCVEnergyThreshold;
extern G__linked_taginfo G__MCVDictLN_MCVGridEnergyCal;
extern G__linked_taginfo G__MCVDictLN_MCVBarCorrelation;
extern G__linked_taginfo G__MCVDictLN_MCVBkgSubtraction;
extern G__linked_taginfo G__MCVDictLN_MCVBurstLandauFit;
extern G__linked_taginfo G__MCVDictLN_MCVRawMonitor;
extern G__linked_taginfo G__MCVDictLN_MCVAnalysis;
extern G__linked_taginfo G__MCVDictLN_MCVBarMonitor;
extern G__linked_taginfo G__MCVDictLN_MCVSelection;
extern G__linked_taginfo G__MCVDictLN_MCVTree;
extern G__linked_taginfo G__MCVDictLN_MCVSelectionGUI;
extern G__linked_taginfo G__MCVDictLN_MCVHisto3908;
extern G__linked_taginfo G__MCVDictLN_MCVBurst;
extern G__linked_taginfo G__MCVDictLN_MCVFilePKP3908;
extern G__linked_taginfo G__MCVDictLN_MCVbsrMonitor;
extern G__linked_taginfo G__MCVDictLN_MCVbsrMonitor_ric;
extern G__linked_taginfo G__MCVDictLN_MCVAntiCoincidence;
extern G__linked_taginfo G__MCVDictLN_MCVCalibration;
extern G__linked_taginfo G__MCVDictLN_subwin;
extern G__linked_taginfo G__MCVDictLN_MCVSubMS;
extern G__linked_taginfo G__MCVDictLN_TH3F;
extern G__linked_taginfo G__MCVDictLN_TTimeStamp;
extern G__linked_taginfo G__MCVDictLN_GRB;
extern G__linked_taginfo G__MCVDictLN_GRB3908;
extern G__linked_taginfo G__MCVDictLN_GRB3913;
extern G__linked_taginfo G__MCVDictLN_GRB3913XZ;
extern G__linked_taginfo G__MCVDictLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__MCVDictLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__MCVDictLN_gpsevt;
extern G__linked_taginfo G__MCVDictLN_MCVFile3916;
extern G__linked_taginfo G__MCVDictLN_hkevt;
extern G__linked_taginfo G__MCVDictLN_MCVFile3201;

/* STUB derived class for protected member access */
