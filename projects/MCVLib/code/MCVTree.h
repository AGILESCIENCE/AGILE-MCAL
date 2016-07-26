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
#ifndef MCVLib_MCVTree
#define MCVLib_MCVTree

#include "MCVFileInfo.h"
#include "MCVFile.h"
#include "MCVHisto.h"
#include "MCVSelection.h"

#include <TTree.h>
#include <TString.h>
#include <TEnv.h>
#include <TFile.h>
#include <math.h>

#define NMAX_FILE 100
#define DELTAT 1000.
#define ZSTH 12.

//! Class for selection and operations at ROOT Tree level

/*!
MCVTree 
*/


class MCVTree : public MCVFile, public MCVHisto {

	protected:
		TTree	*indata;	//!< Input dataset
		TTree	*outdata;	//!< Output dataset (selected or modified data)
		TObjArray *inhisto;	//!< Input dataset histogram collection
		TEnv *conf;		//!< Pointer to configuration file
		MCVFileInfo *ininfo;	//!< Pointer to information on input dataset
		MCVFileInfo *outinfo;	//!< Information on output dataset
		MCALevt inevt;		//!< Input event
		int	inevents;	//!< Initial number of events
		int	outevents;	//!< Output number of events
		TString	infoname;
		char	outdataname[100];
		TFile *fp, *fh;
		MCVSelection *sel;
	
		void	Copy3913SciRatemeters(); //!< Copy scientific ratemeters data from input to output histogram collections 
		void	CopyEvt();		//!< Copy input event into output event
		void	CopyEvt(bool *barsel);	//!< Copy input event into output event, single bar selection
		void	Init();			//!< Set branches addresses for input and output trees
		
	
	public:
		MCVTree();
		MCVTree(TTree *t, MCVFileInfo *in, TObjArray *h=NULL);		//!< Standard constructor (shift, selections)
		MCVTree(TTree *t1, MCVFileInfo *in, TObjArray *h, TTree *t2, MCVFileInfo *in2);	//!< Constructor for merging t2 with t1
		~MCVTree();	//virtual 
		void		ApplyCalibration();		//!< Apply calibration parameters stored in outinfo
		void		DoSelection(MCVSelection *s);	//!< Apply selection
		void		DoZeroSuppression();		//!< Perform software zero-suppression on 39.02 data
		TTree		*GetOutputData() {return outdata;};
		TObjArray	*GetOutputHisto() {return rawHisto;};
		MCVFileInfo 	*GetOutputInfo() {return outinfo;};
		void 		MuonTrackSelection();		//!< Apply selection to extract muon tracks
		void		SaveData();			//!< Save new dataset after selection / calculations
		void		Shift(int nevt, int nshift);	//!< Do time shift to cure timing misalignment
		/*!<
		\param nevt is the event number (first is 0) where time shift must start
		\param nshift is the width of the shift: if nshift<0, starting from event nevt, |nshift| timetag 
		are deleted and the time column is shifted backward accordingly (VERY typical case is nshift=-1); 
		if nshift>0, starting from event nevt, nshift data fields are deleted, data columns are shifted back accordingly 
		while leaving intact the timetag column
		*/
		void		UpdateSciRM();		//!< Update Scientific Ratemeters according ONLY to time selection
		ClassDef(MCVTree,1);			//!< Required for ROOT dictionary generation using rootcint
};	

#endif