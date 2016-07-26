#ifndef MCVLib_MCVFile
#define MCVLib_MCVFile

//#include <fitsio.h>

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <TROOT.h>
#include <TString.h>
#include <TObjArray.h>
#include <TEnv.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TRandom3.h>
#include <math.h>

#include "MCVFileInfo.h"

#define STANDARD_CONF "/share/MCVLib_conf/MCVLib_configuration.conf"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// file : MCVbsrInfo.h 
// last update: 28-mar-2006
//
// ----------------------------------------------------------

#ifndef MCVLib_MCVbsrInfo
#define MCVLib_MCVbsrInfo

struct MCVbsrInfo  
{                       //! description of  L1 3908, extname: PACKETS and RATEMTRS
    int numpkt ;        //! num of packets records 
    int numbsr ;        //! num of bsr (ratemeter) records) 
    
};

struct MCALpkt 
{                              //! description of  L1 3908, extname: PACKETS 

    int paktnumb  ;      //!  packet keyword name: PAKTNUMB ( packet  number)
    int pakseq   ;       //! packet keyword name: PKTSEQCN  ( packet sequence number)
    int pakid ;          //! packet keyword name: PACKETID ( packet identifier in the conctact)
    int numpackets;      //! packet keyword name: BSRTPKYS (tot num of packets)
    int numbsr ;         //! packet keyword name: BSRBLCKS (num of bsr blocks)
    int bebnum ;         //!packet keyword name: BEBNUM (num of burst blocks)
    bool mcal ;
    bool sa ; 
    bool mcalsubm ;     //!  packet keyword name: MCSMSFLG , type: 1B 
    bool sasubm   ;     //!  packet keyword name: SASMSFLG , type: 1B 
    bool mcal16ms ;     //!  packet keyword name:  MCAL16MS
    bool mcal1ms ;      //!  packet keyword name: MCAL1MS
    bool sa16ms ;       //!  packet keyword name: SA16MS
    bool sa1ms ;        //!  packet keyword name:  SA1MS 
    double bstart ;     //!    packet keywords: BSTARTHI + BSTARTLO 
    double bstop ;      //!    packet keywords: BSTOPHI + BSTOPLO  
    
    
} ;   
    
struct MCALbsr
{                                   //!   L1 3908, extname:  RATEMTRS
    int pktnum  ;         //!  packet keyword name: PAKTNUMB ( packet  number)
    int evnum ;           //!   packet keyword name:EVNUMBER 
    int saormcal ;        // 0=sa = false ; 1 = mcal = true 
    
    unsigned int nume ;    //!  packet keyword name:RTMTIDCH
    unsigned int numg ;    //!  packet keyword name:RTMTIDCH
    unsigned int numt ;    //!  packet keyword name:RMTIDSIT
    unsigned int dr ;      //!  packet keyword name:BSCIRTMT
    unsigned int br ;      //!  packet keyword name:BKGERTMT
    
    double time ;          //!  packet keyword name:TIME

    unsigned int numch ;   //! num ratemeter channel 

} ;

#endif
 
struct MCALevt {
	Double_t	time;
	Int_t		mult;
	Short_t		bar[30];
	Short_t 	pdA[30];
	Short_t 	pdB[30];
	Float_t	 	pos[30];
	Float_t	 	E[30];
	Float_t		Etot;
	Int_t		totEX;
	Int_t		totEZ;
	Int_t		rmE[30];
};

//#define CONF_FILENAME_BURST "MCALBurstTest.conf"
//#define CONF_FILENAME_GRID  "MCALGridTest.conf"
//#define CONF_DIR "/conf/calibration/"

//! Basic structure for MCAL events representation

/*!
One object of structure MCALevt represents an event occurring in MCAL.

*/


//! Base class for fits file elaboration

/*!
MCVFile provides a standard interface to AGILE level-1 fits files.
Configuration files are required and must be located in $(MCALSW)/share/MCVLib_conf/
*/

class MCVFile {
	
	protected:
		// fits file and general parameters
//		fitsfile *fptr;		//!< fits file pointer
		int 	status;
		int 	nfields;
		char 	comment[100];
		char 	path[200], fname[200];
		Double_t	*timearray;	//!< array containing the TIME column of the lv1 fits file
		MCVFileInfo	fileInfo;	//!< object containing all the relevant file information
		FILE 	*ferr;			//!< pointer to txt file for data error info storage
		TRandom3 rg;
		double enoise;
		
		// parameters for energy and position reconstruction using exponential model (default)
		TString   calFile;		
		TString   fitsname;		
		TEnv *conf;
		TEnv *stdconf;		//!< Standard configuration file
		double barLen;		//!< Bar length (default = 37.5 cm)
		double gainFactor;	//!< Gain multiplicative coefficient for GRID energy calculations (~1.2)
		double u0A[30];		//!< PD-A gain near PD (ch/MeV)
		double u0B[30];		//!< PD-B gain near PD (ch/MeV)
		double alphaA[30];	//!< PD-A attenuation coefficient (cm^-1)
		double alphaB[30];	//!< PD-B attenuation coefficient (cm^-1)
		double oA[30];		//!< Offset of PD-A (ADC channels)
		double oB[30];		//!< Offset of PD-B (ADC channels)
		double alphaMean[30];   //!< Mean attenuation coefficient = 0.5*(alphaA+alphaB)
		double alphaDelta[30];  //!< Attenuation coefficient semidispersion = 0.5*(alphaA-alphaB)
		double cx1[30];   	//!< Auxiliary coefficient for position calculation
		double cx2[30];   	//!< Auxiliary coefficient for position calculation
		double cE1[30];   	//!< Auxiliary coefficient for position calculation
		
		float Ea[30];		//!< Energy linear correction offset parameter (to be read from file)  
		float Eb[30];		//!< Energy linear correction angular coefficient (to be read from file)
		bool	corrFlag;	//!< Linear correction flag: TRUE = lin. corr. ON; FALSE = lin. corr. OFF
		
		// parameters for energy and position reconstruction using 5th grade polinomial model
		TObjArray *barpol;	//!< Collection of polinomial bar models for E and pos. reconstruction
		TObjArray *fcA;		//!< Collection of polinomial bar models for E and pos. reconstruction (side A)
		TObjArray *fcB;		//!< Collection of polinomial bar models for E and pos. reconstruction (side B)
		TObjArray *fcBonA;	//!< Collection of polinomial bar models for E and pos. reconstruction (ln(B/A))
		int	recmodel;	//!< Model for energy/position reconstruction (0: expo, 1: pol5 (text file), 2: pol3 (root file))
		float	minX[30];
		float	minAB[30];
		float	maxX[30];
		float	maxAB[30];
		
		// parameters for scientific ratemeters calculation
		Int_t	soA[30];	//!< PD-A offset as stored in PDHU for scientific RM calculation	
		Int_t	soB[30];	//!< PD-B offset as stored in PDHU for scientific RM calculation	
		
		TFile 	*fp;		//!< root file pointer
		TTree	*tdata;		//!< data tree pointer
		TString	rootdatapath;	//!< Path to ROOT data file
		MCALevt currevt;	//!< structure containing all the information concerning the current event
		unsigned int	seed;	//!< initial seed for random number generator
		int	notmonotonic;	//!< Number of events with not monotonic time tag
		bool	simulation;	//!< TRUE if data set is GAMS + DHSim simulated data, FALSE otherwise
		bool	trigtrack;	//!< TRUE if data set contains tagged DAFNE photons, FALSE otherwise
		
		void printerror(int);	//!< Print out cfitsio error messages and exit program
		
		void BuildPolinomialFunctions();	//!< Creates approximated polinomial functions for E and pos. recon.
		void GetPolinomialFunctions();		//!< Retrieve polinomial functions for E and pos. recon. (recmodel==2)
		void CalculateEandPos(int cmult, int cbar, Short_t pdA, Short_t pdB);	//!< Calculate energy (MeV) and position (cm)
		/*!< calculates energy and position using current calibration parameters
		and updates scientific ratemeters counters.
		The data member in MCVFile::currevt are updated.
		\param cmult is the event current multiplicity
		\param cbar is the current bar where the event took place
		\param pdA and \param pdB are the two PDs readouts in ADC channels*/
		
		unsigned long GetTime(char *timestring);//!< Return time in seconds since 1st january of selected year.
		/*!< does NOT work for measurements crossing two different years
		 does NOT take care of bisestile years (feb IS 28 days) yet!!! 
		 \param timestring is the time string as recovered from keywords DATE-OBS and DATE-END in lv1 fits files*/
		
		void Initialize();	//!< Class initialization
		/*!< reads the appropriate fits keywords and sets class flags */
		
		void InitializeSimulation();	//!< Class initialization for GAMS+DHSim lv1 data
		/*!< reads the appropriate fits keywords and sets class flags */
		
		void InitializeTrigTrack();	//!< Class initialization for Trig & Track data
		/*!< reads the appropriate fits keywords and sets class flags */
		
		float LinearCorrection(int ibar, float Ein);

		void LoadCalibrationParameters(); //!< Load calibration parameters from configuration file
		/*!< Load calibration parameters from configuration file and 
		calculate auxiliary parameters for energy/position calculations */
		
		void LoadLinCorrParameters();		//!< Load parameters for Energy linear correction
		
		void OpenErrorFile();
		
		void UpdateFileInfo();	//!< Writes file information in object of class MCVFileInfo
	
	public:
		MCVFile();
		MCVFile(const char *name);
		virtual void CreateRootFile();	//!< virtual function for ROOT file creation
		/*!< Manages ROOT file creation depending on the structure of the input fits file (DFE-TE or PDHU lv1).
		Must be overloaded in inherited classes */
		
		const char *GetDataPath() {return rootdatapath.Data();};
		MCVFileInfo *GetFileInfo() {return &fileInfo;};
		bool IsGrid() {return fileInfo.grid;}; 	//!< returns TRUE if data are from the GRID branch
		bool IsPDHUData() {return fileInfo.pdhudata;};	//!< returns TRUE if data come from PDHU (OBSOLETE)
		bool IsPDHUFormat() {return fileInfo.pdhuformat;}; //!< returns TRUE if fits file is in PDHU lv1 format (OBSOLETE)
		void PrintCalibrationParameters(); 	//!< print out the calibration parameters used for energy calculation
		void PrintFileInfo() {fileInfo.PrintFileInfo();};	//!< print out basic informations on the current file
		void SetCalibrationFile(char *);	//!< set a new calibration file and reload calibration parameters
		void SetSeed(unsigned int s);		//!< set a new seed for the random number generator
		void SetRootFileName(char *s) {rootdatapath = s;};		//!< set the name of the output root file
		~MCVFile();	 
		ClassDef(MCVFile,1);	//!< required for ROOT dictionary generation using rootcint
};

#endif
