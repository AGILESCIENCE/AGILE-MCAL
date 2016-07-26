#include "mcalanalyzer.h"

using namespace std ; 

void printerror( int );

int main(int argc, char *argv[])
{
	TObjArray *rh;
	MCVFileInfo *info, *newinfo;
	MCVTree *newdata;
	TFile *fh;	// pointer to histogram ROOT file
	TFile *fd;	// pointer to data ROOT file
	TFile *fg;	// pointer to global info ROOT file
	TTree *tdata;
	TTree *gdata = NULL;	
	TKey *key;
	int nlowDTini = 0;
	int notmonotonic = 0;
	TString roothistopath;
	bool pdhuformat;
	int 	status;
	char 	comment[100];
	
	char ini;
	char histoname[100];
	char rootname[100];
	TH1::AddDirectory(kFALSE);
	
	bool updateGlobal = FALSE;	// if TRUE update global file
	bool process = TRUE;		// if TRUE process the file, otherwise do ancillary analysis
	
	TString grbfilename;
	
	// load calibration parameters from standard configuration file
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	TEnv *stdconf = new TEnv(confname);
	TString boostfile(getenv("MCALSW"));
	boostfile.Append(stdconf->GetValue(".boost.file.", "none"));
	
	cout << endl << "mcalanalizer version " << VERSION << endl;
	
	// parse command line data
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
		// input grb trigger file
		if (cmd.Contains("-grb", TString::kExact)) grbfilename += argv[++iopt];

		// update global file flag
		if (cmd.Contains("-u", TString::kExact)) updateGlobal=TRUE;

		// no-process flag
		if (cmd.Contains("-noproc", TString::kExact)) process=FALSE;
		iopt++;
	}
	if (grbfilename.IsNull()) grbfilename += "grboutput.txt";
	
	// is argv[1] a valid ROOT file?
	fh = new TFile(argv[1], "READ");
	if (fh->IsZombie()) {
		// I suppose it is a fits file
		fh->Close();
		
		// work on fits file 
		// check whether the file is in DFE-TE or PDHU format
		cout << "Trying to open fits file: " << argv[1] << endl;
		fitsfile *fptr;		
		char aux[200];
		status = 0;
		sprintf(aux, "%s[0]",  argv[1]);  
		if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
		char	opmode[16];	// operative mode (grid / burst / PDHU)
//		if ( fits_read_key(fptr, TSTRING, "INSTRUME", &opmode, comment, &status) )  printerror( status ); 
		 
		// modifica per trattare anche i dati TRIG&TRACK
		fits_read_key(fptr, TSTRING, "INSTRUME", &opmode, comment, &status);		
		if (status == 202 || !strncmp(opmode, "PDHU", 4) || !strncmp(opmode, "GRID", 4)) pdhuformat=TRUE;
		else pdhuformat=FALSE;
		
		// gestione rapida dei 3908
		int pkttype = 0;
		int pktstype = 0;
		int obsid = 0;
		fits_read_key(fptr, TINT, "PKTTYPE", &pkttype, comment, &status);
		fits_read_key(fptr, TINT, "PKTSTYPE", &pktstype, comment, &status);
		fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status);
		
		if (pkttype==39 && pktstype==8) process3908(fptr, &boostfile, &grbfilename);
		
		
		
//		pdhuformat=TRUE;		// SOLO FORMATI LEVEL 1!
//		pdhuformat=FALSE;		// SOLO FORMATI TE!
		
		if (process && pdhuformat) {
			// work on a PDHU-style file (use class MCVFilePKP)
			cout << "Processing a PDHU-style file (use libMCV class MCVFilePKP)" << endl;
			MCVFilePKP *fp;
			fp = new MCVFilePKP(argv[1]);
			fp->PrintCalibrationParameters();
			fp->CreateRootFile();
			info = fp->GetFileInfo();
			nlowDTini = info->nlowDT;
			notmonotonic = info->notmonotonic;
			
			// if a time misalignment is evident: shift time column and process file again
			if (info->nlowDT > 0 && info->pkttype != 3910 && info->pkttype != 1111 && info->pkttype != 2222) {
				printf("\nWARNING: evidence for timing misalignment.\n");
				printf("Anomalous events:\t%d\n", info->nlowDT);
				printf("First anomalous event:\t %d at time:\t%f\n", info->lowDTevt, info->lowDTti);
				printf("Trying to correct via timing shift...\n");
				sprintf(rootname, "RT%06d_%d.root", info->runid, info->pkttype);
				TString rootdatapath;
				rootdatapath += fp->GetDataPath();
				fd = new TFile(rootdatapath.Data());
				TIter next0 = fd->GetListOfKeys();
				while ((key = (TKey *)next0())) {
					if (strcmp(key->GetClassName(),"TTree") == 0) tdata = (TTree *) fd->Get(key->GetName());
					if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fd->Get(key->GetName());
				}
				sprintf(histoname, "H%06d_%d.root", info->runid, info->pkttype);
				TString oldhistopath;
				oldhistopath += fp->GetHistoPath();
				fh = new TFile(oldhistopath.Data());
				rh = (TObjArray *) fh->Get("rawHisto");
				if (!tdata) cout << "*** no tdata!" << endl;
				if (!info) cout << "*** no info!" << endl;
				newdata = new MCVTree(tdata, info, rh);
				newdata->Shift(info->lowDTevt-1, -1);	// shift from first misalignment
				newdata->SaveData();
				newinfo = newdata->GetOutputInfo();
				fh->Close();
				if (newinfo->nlowDT) {
					printf("\nWARNING: still evidence for timing misalignment.\n");
					printf("Anomalous events:\t%d\n", newinfo->nlowDT);
					printf("First anomalous event:\t %d at time:\t%f\n", newinfo->lowDTevt, newinfo->lowDTti);
				} else printf("Timing misalignment corrected\n");
				roothistopath += newdata->GetHistoPath();
			} else roothistopath += fp->GetHistoPath(); 
			delete fp;
		 } else {
		 	cout << "Nothing to process (either non-PDHU style file or -noproc flag set)!" << endl;
			return 0;
		 }
		 cout << "Opening histo file: " << roothistopath.Data() << endl;
		fh = new TFile(roothistopath.Data());
	}
	
	// work on ROOT histogram files
	rh = (TObjArray *) fh->Get("rawHisto");
	TIter next = fh->GetListOfKeys();
	while ((key = (TKey *)next())) {
//		printf("key: %s \t\t\tclass name: %s\n", key->GetName(), key->GetClassName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fh->Get(key->GetName());
	}
	
	// print out detailed results
	printf("\n--------------------- TIMING ----------------------\n\n");
	if (nlowDTini) {
		printf("WARNING: evidence for timing misalignment.(%d impossible events)\n", nlowDTini);
		printf("After correction: %d impossible events\n", info->nlowDT);
	} else if (info->nlowDT) printf("WARNING: evidence for timing misalignment.(%d impossible events)\n", info->nlowDT);
	else printf("No timing misalignement\n");
	if (notmonotonic || info->notmonotonic) 
		printf("WARNING: orbit %d Timing not monotonic in %d events.\n", info->runid, info->notmonotonic);
	if (info) info->PrintFileInfo();
				
	MCVCountRate 	*countrate = NULL;
	MCVGridOffset 	*gridoffset = NULL;
	MCVRetriggering *retriggering = NULL;
	MCVSciRM 	*sciRM = NULL;
	MCVElectricalCalib *electricalcalib = NULL;
	MCVEnergyThreshold *energythreshold = NULL;
	MCVGridEnergyCal *gridenergycal = NULL;
	MCVAntiCoincidence *anticoincidence = NULL;
	MCVGlobalRetriggering *globalretrig = NULL;
	
	if (info->pdhuformat) {		// PDHU data
		switch (info->pkttype) {
			case 3901:
				countrate = new MCVCountRate(rh, info);
				countrate->Calculate();
				sciRM = new MCVSciRM(rh, info);
				sciRM->Calculate();
			break;
			case 3902:
				countrate = new MCVCountRate(rh, info);
				countrate->Calculate();
				sciRM = new MCVSciRM(rh, info);
				sciRM->Calculate();
				gridoffset = new MCVGridOffset(rh, info);
				gridoffset->Calculate();
				gridenergycal = new MCVGridEnergyCal(rh, info);
				gridenergycal->Calculate();
			break;
			case 3909:
				countrate = new MCVCountRate(rh, info);
				countrate->Calculate();
				sciRM = new MCVSciRM(rh, info);
				sciRM->Calculate();
				retriggering = new MCVRetriggering(rh, info);
				retriggering->Calculate();
				energythreshold = new MCVEnergyThreshold(rh, info);
				energythreshold->Calculate();
				anticoincidence = new MCVAntiCoincidence(rh, info);
				anticoincidence->Calculate();
				globalretrig = new MCVGlobalRetriggering(rh, info);
				globalretrig->Calculate();
			break;
			case 3910:
				electricalcalib = new MCVElectricalCalib(rh, info);
				electricalcalib->Calculate();
			break;
			case 3913:
				sciRM = new MCVSciRM(rh, info);
				sciRM->Calculate();
			break;
			default:
			break;
		}
	} else {			// MCAL DFE-TE data
		countrate = new MCVCountRate(rh, info);
		countrate->Calculate();
	}

	switch (info->pkttype) {
		case 3901:
			countrate->PrintResults();
			//countrate->SaveToFile();
			sciRM->PrintResults();
			//sciRM->SaveToFile();
			if (updateGlobal && info->runid != -1) sciRM->UpdateGlobalFile();
		break;
		case 3902:
			countrate->PrintResults();
			//countrate->SaveToFile();
			sciRM->PrintResults();
			//sciRM->SaveToFile();
			gridoffset->PrintResults();
			//gridoffset->SaveToFile();
			if (updateGlobal && info->runid != -1) {
				sciRM->UpdateGlobalFile();
				//gridoffset->UpdateGlobalFile();
			}
			gridenergycal->PrintResults();
			//gridenergycal->SaveToFile();
		break;
		case 3909:
			countrate->PrintResults();
			//countrate->SaveToFile();
			sciRM->PrintResults();
			//sciRM->SaveToFile();
			if (updateGlobal && info->runid != -1) sciRM->UpdateGlobalFile();
			retriggering->PrintResults();
			energythreshold->PrintResults();
			//energythreshold->SaveToFile();
			anticoincidence->PrintResults();
			globalretrig->PrintResults();
		break;
		case 3910:
			electricalcalib->PrintResults();
			//electricalcalib->SaveToFile();
		break;
		case 3913:
			sciRM->PrintResults();
			//sciRM->SaveToFile();
			if (updateGlobal && info->runid != -1) sciRM->UpdateGlobalFile();
		break;
		default:
		break;
	}
	
	// print out sintetic comment
	if (info) info->PrintFileInfo();
	printf("\n--------------------- COMMENTS ----------------------\n\n");
	
	printf("Timing:\t\t\t\t\t");
	if (nlowDTini || notmonotonic || info->nlowDT || info->notmonotonic) printf(" Out of specifications. See detailed results.\n");
	else printf (" OK\n");
	
	switch (info->pkttype) {
		case 3901:
			printf("Count rate analysis: \t\t\t");
			countrate->PrintComment();
			printf("Scientific ratemeters analysis: \t");
			sciRM->PrintComment();
			delete countrate;
			delete sciRM;
		break;
		case 3902:
			printf("Count rate analysis: \t\t\t");
			countrate->PrintComment();
			printf("Scientific ratemeters analysis: \t");
			sciRM->PrintComment();
			printf("Grid offset analysis: \t\t\t");
			gridoffset->PrintComment();
			printf("Grid energy calibration with muons: \t");
			gridenergycal->PrintComment();
			delete countrate;
			delete sciRM;
			delete gridoffset;
			delete gridenergycal;
		break;
		case 3909:
			printf("Count rate analysis: \t\t\t");
			countrate->PrintComment();
			printf("Scientific ratemeters analysis: \t");
			sciRM->PrintComment();
			printf("Retriggering search: \t\t\t");
			retriggering->PrintComment();
			printf("Global retriggering: \t\t\t");
			globalretrig->PrintComment();
			printf("Energy Threshold: \t\t\t");
			energythreshold->PrintComment();
			printf("Anticoincidence check: \t\t\t");
			anticoincidence->PrintComment();
			delete countrate;
			delete sciRM;
			delete retriggering;
			delete globalretrig;
			delete energythreshold;
			delete anticoincidence;
		break;
		case 3910:
			printf("Electrical calibration: \t\t\t");
			electricalcalib->PrintComment();
			delete electricalcalib;
		break;
		case 3913:
			printf("Scientific ratemeters analysis: \t");
			sciRM->PrintComment();
			delete countrate;
			delete sciRM;
		break;
		default:
		break;
	} 
	printf("-----------------------------------------------------\n\n");
	
	// save file info to global file
	if (updateGlobal && info->runid != -1) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += stdconf->GetValue(".global.history.file.", "./global.root");
		fg = new TFile(outname.Data(), "UPDATE");  
		gdata = (TTree *) fg->Get("runInfo");
		if (gdata == NULL) {
			gdata = new TTree("runInfo", "run information");
			gdata->Branch("run_info", "MCVFileInfo", &info, 16000, 0);
		}
		TBranch *br = gdata->GetBranch("run_info");
		br->SetAddress(&info);
		gdata->Fill();
		gdata->Write();
		fg->Close();
	} 
	if (info->runid == -1) cout << "*** WARNING: this run was not properly closed and has run id -1. Global file was not updated" << endl;
	
	fh->Close();

	return 0;
}

//
//--------------------------------------------------------------------
//

void printerror( int status)
{   // Print out cfitsio error messages and exit program
	if (status)  {
		fits_report_error(stderr, status);  // print error report
		exit( status );                     // terminate the program, returning error status
	}
	return;
}
