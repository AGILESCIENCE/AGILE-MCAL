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
#ifndef MCVLib_MCVSelection
#define MCVLib_MCVSelection

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <TROOT.h>
#include <TNamed.h>
#include <TString.h>

#include "MCVFile.h"

#define MINPHA -1000
#define MAXPHA 4095
#define MINPOS -100.
#define MAXPOS  100.
#define MINE  0.
#define MAXE  10000.
#define MINTIME  0.
#define MAXTIME  20000.

//! A class to represent a conditional selection on data 

/*! This class contains information on a selection operated on a certain dataset. 
It is derived from ROOT class TNamed so it can be written in a ROOT. 
*/

class MCVSelection : public TNamed {

	public:
		Int_t 		nbars;    // number of selected bars
		Int_t 		bars[30]; // bar numbers 
		Int_t 		minA, maxA, minB, maxB;  	// minimum and maximum PDs PHA limits
		Int_t 		minMult, maxMult;		// minimum and maximum bar multiplicity limits
		Double_t 	minPos, maxPos, minE, maxE;  // minimum and maximum position and energy boundaries
		Double_t 	minTime, maxTime;
		Bool_t 		saveTracksFlg;
		Bool_t 		pdAEnable;
		Bool_t 		pdBEnable;
		Bool_t 		multEnable;
		Bool_t 		posEnable;
		Bool_t 		energyEnable;
		Bool_t 		timeEnable;
		Bool_t 		relToOffset;
		Bool_t 		isBarStrong;		//!< TRUE if exclusive condition on bars selected has to be applied
		Bool_t 		isPosStrong;		//!< TRUE if exclusive condition on position has to be applied
		Bool_t 		isEStrong;		//!< TRUE if exclusive condition on energy has to be applied
		MCVSelection();
		~MCVSelection();
		void PrintSelection();
		void SetDefault();
		ClassDef(MCVSelection,1);
};

#endif