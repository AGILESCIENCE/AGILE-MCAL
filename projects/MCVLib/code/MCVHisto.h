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

#ifndef MCVLib_MCVHisto
#define MCVLib_MCVHisto

#include "MCVFileInfo.h"
#include "MCVFile.h"
#include <TObjArray.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>

//! A class for creating / handling basic MCAL histograms

/*! This class creates the main histograms needed for basic MCAL data analysis, such as energy and position distributions, count rate and multiplicity histograms, PD-A vs. PD-B distributions and so on. 
The histograms are created in the constructor and filled up after each event is processed (method MCVHisto::FillHisto) and at the end of processing (for global histograms, method MCVHisto::FillGlobalHisto). 
It is a stand-alone class so that both classes working either on PDHU or TE data format can inherit from it and build the same science products.
*/

class MCVHisto {
	
	protected:
	
		// baseline engineering & science products
		TObjArray *rawPDA;	//!< Container class for PD-A histograms of all bars
		TObjArray *rawPDB;	//!< Container class for PD-B histograms of all bars
		TObjArray *raw2D;	//!< Container class for 2D (PD-A vs. PD-B) histograms of all bar
		TObjArray *rawPos;	//!< Container class for reconstructed position histograms of all bar
		TObjArray *rawE;	//!< Container class for reconstructed energy histograms of all bar
		TObjArray *rawDTime;	//!< Container class for time separation histograms of all bar
		TObjArray *rawCRate;	//!< Container class for count rate histograms of all bar
		TObjArray *rawCRDist;	//!< Container class for count rate distribution histograms of all bar
		TObjArray *rawMult;	//!< Container class for multiplicity histograms of all bar
		TObjArray *rawSciRM3913;//!< Container class for MCAL scientific ratemeters (39.13) vs. time
		TObjArray *rawACRM3913;//!< Container class for AC scientific ratemeters (39.13) vs. time
		TH1F	*sciRMX;	//!< scientific ratemeters on plane X as derived from data
		TH1F	*sciRMZ;	//!< scientific ratemeters on plane Z as derived from data
		TH1F	*hRMXR;		//!< scientific ratemeters on plane X as derived from reconstructed energy
		TH1F	*hRMZR;		//!< scientific ratemeters on plane Z as derived from reconstructed energy
		TGraphErrors	*gRMX;	//!< scientific ratemeters on plane X as derived from data (normalized graph)
		TGraphErrors	*gRMZ;	//!< scientific ratemeters on plane Z as derived from data (normalized graph)
		TGraphErrors	*gRMX3913;//!< scientific ratemeters on plane X as derived from 39.13 packets (normalized graph)
		TGraphErrors	*gRMZ3913;//!< scientific ratemeters on plane Z as derived from 39.13 packets(normalized graph)
		TGraphErrors	*gRMXR;	//!< scientific ratemeters on plane X as derived from reconstructed energy 
		TGraphErrors	*gRMZR;	//!< scientific ratemeters on plane Z as derived from  reconstructed energy 
		TH1F	*totDT;		//!< global time difference between every successive event
		TH2F	*barcorr;	//!< bar[i],bar[i+1] 2D histogram (for spurious correlations)
		TH2F	*barcorrLowDT;	//!< bar[i],bar[i+1] 2D histogram for delta_t<20 us (for spurious correlations)
		TH1F	*totCR;		//!< total MCAL count rate
		TH1F	*totMult;	//!< total MCAL multiplicity
		TH1F	*totE;		//!< total MCAL energy spectrum
		TH2F	*hX;		//!< X plane events distribution
		TH2F	*hZ;		//!< Z plane events distribution
		TH1F	*hSubMS;	//!< Sub-millisecond trigger distribution according to data
		TGraphErrors	*gCRX;	//!< count rate on plane X
		TGraphErrors	*gCRZ;	//!< count rate on plane Z
		TGraphErrors	*gMultX;	//!< multiplicity ratio distribution on plane X
		TGraphErrors	*gMultZ;	//!< multiplicity ratio distribution on plane Z
		
		TObjArray 	*rawHisto;    	//!< main collection of raw histograms
		TFile 		*fh;		//!< root histograms file pointer
		TString	roothistopath;		//!< Path to ROOT histogram file
		Double_t	prtime[30];	//!< previous events time (for each bar) for delta_t calculations
		Double_t	prt;		//!< previous event time for global delta_t calculations
		Int_t		prmult;		//!< previous event multiplicity
		Int_t		prbar;		//!< previous event bar address (meaningful only if prmult==1)
		
		Int_t		nlowDT;		//!< Number of consecutive events on the same bar with DT<10 us
		Int_t		firstlowDT;	//!< Event number of the first "impossible" event (needed for shift correction)
		Double_t	lowDTti;	//!< Time (s) of the first "impossible" event
		Double_t	lowDTtf;	//!< Time (s) of the last "impossible" event
		
		Float_t Eband[11];		//!< Default center of scientific ratemeters energy bands	
		double  EbandUL[11];		//!< Default upper limit of scientific ratemeters energy bands	
		Float_t Eerr[11];		//!< Default half width of scientific ratemeters energy bands
		Float_t gf;			//!< Gain factor for GRID scientific ratemeters calculation
		
		void FillCountRateDistribution();	//!< Fill count rate distribution histograms for each bar 
		void FillGlobalHisto(int pkttype, Double_t te);		//!< Fill global histograms
		/*!< Fill global histograms after all events have been processed 
		\param pkttype is the packet type required to process correctly the count rate for Grid observation data
		\param te is the measure effective time for histograms normalization */
		
		void InitSciRMEnergyBand();
		
		void ResizeCountRateHisto(int time);	//!< Resize count rate histograms according to measurement duration

	public:
		MCVHisto();
		MCVHisto(TObjArray *inh);
		~MCVHisto();
		Double_t	GetEffectiveTime(Int_t ibar);	//!< Returns effective time calculated on bar ibar
		Double_t	GetEffectiveTime();		//!< Returns effective time calculated on global CR histo
		const char 	*GetHistoPath() {return roothistopath.Data();};
		
		void FillHisto(MCALevt *evt);	//!< Fill histograms for the current event (0=OK; 1=timing misalignment)
		/*!< \param evt is a pointer to the current event */
		
		TObjArray *GetRawHisto() {return rawHisto;};
		
		ClassDef(MCVHisto,1);				//!< required for ROOT dictionary generation using rootcint
};

#endif
