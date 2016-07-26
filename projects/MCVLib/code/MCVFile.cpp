#include "MCVFile.h"
#include <fitsio.h>
ClassImp(MCVFile);

MCVFile::MCVFile():timearray(NULL),fp(NULL),tdata(NULL), barpol(NULL), fcA(NULL), fcB(NULL), fcBonA(NULL), ferr(NULL)
{
	// load calibration parameters from standard configuration file
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	stdconf = new TEnv(confname);
	LoadLinCorrParameters();
}

//
//--------------------------------------------------------------------
//

MCVFile::MCVFile(const char *name):timearray(NULL),fp(NULL),tdata(NULL), barpol(NULL), fcA(NULL), fcB(NULL), fcBonA(NULL), ferr(NULL)
{
	status = 0; // important otherwise status control will not work!
	TString confname;
	
	int i=0, npath=0, ndot=0;	// extract path from filename
	while (name[i]!='\0') if (name[i++]=='/') npath =i;
	i=npath;
	while (name[i]!='\0') if (name[i++]=='.') ndot =i;
	strncpy(path, name, npath);
	path[npath] = '\0';
	cout << "File path: " << path << endl;
	
	strncpy(fname, &name[npath], ndot-npath-1);
	fname[ndot-npath-1] = '\0';
	cout << "File name: " << fname << endl;
	sprintf(fileInfo.filename, "%s",  name); 
	fitsname += name; 
	
	// checks whether file contains DAFNE tagged photons data (TRIG & TRACK)
	if (fitsname.Contains("tagged")) trigtrack = TRUE;
	else trigtrack = FALSE;
	
	// checks whether file contains GAMS + DHSim simulated data
	if (fitsname.Contains("SIM")) simulation = TRUE;
	else simulation = FALSE;

//	sprintf(aux, "%s[0]",  name);  
//	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	
	fileInfo.seed = (unsigned int)time( NULL );	// calculate seed for this session
	srand48(fileInfo.seed);				// init seed for random number generator
	
	// load calibration parameters from standard configuration file
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	stdconf = new TEnv(confname);
	
	if (trigtrack) InitializeTrigTrack();
	else if (simulation) InitializeSimulation();
	else Initialize();		// MANDATORY call: retrieve informations on fits file
	LoadCalibrationParameters();	// MANDATORY call: load the default parameters for energy and position calculation
	LoadLinCorrParameters();
	recmodel = stdconf->GetValue(".energy.reconstruction.model.", 0);
	switch (recmodel) {
		case 0:
			cout << "Using full exponential model for position and energy reconstruction" << endl;
		break;
		case 1:
			cout << "Using polinomial model for position and energy reconstruction (parameters from tex file)" << endl;
			BuildPolinomialFunctions();
		break;
		case 2:
			cout << "Using polinomial model for position and energy reconstruction (functions from ROOT file)" << endl;
			GetPolinomialFunctions();
		break;
		default:
		break;
	}
	notmonotonic = 0;
	enoise = stdconf->GetValue(".electronic.noise.adc.", 0.);
}

//
//--------------------------------------------------------------------
//

MCVFile::~MCVFile()
{
//	cout << "Closing fits file" << endl;
//	if (fits_close_file(fptr, &status)) printerror(status);
}

//
//--------------------------------------------------------------------
//

void MCVFile::CreateRootFile()
{
	// virtual function to be overloaded in inherited classes
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::printerror( int status)
{   // Print out cfitsio error messages and exit program
	if (status)  {
		fits_report_error(stderr, status);  // print error report
		exit( status );                     // terminate the program, returning error status
	}
	return;
}

//
//--------------------------------------------------------------------
//

unsigned long MCVFile::GetTime(char *timestring)
{
// return time in seconds since 1st january of selected year.
// does NOT work for measurements crossing two different years
// does NOT take care of bisestile years (feb IS 28 days) yet!!!
    int monthdays[12]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int year, month, day, hour, min, sec;
    unsigned long time=0;
    sscanf(timestring, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec);
    time = (monthdays[month-1] + day - 1)*86400 + hour*3600 + min*60 + sec;
    return time;
}

//
//--------------------------------------------------------------------
//

void MCVFile::Initialize() 
{
	// class initialization: reads the appropriate fits keywords and sets class flags

	fitsfile *fptr;		// fits file pointer
	char aux[200];
	sprintf(aux, "%s[0]",  fitsname.Data());  
	fileInfo.tstart = 0.;
	fileInfo.tstop = 0.;
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	// if ( fits_read_key(fptr, TDOUBLE, "TSTART", &(fileInfo.tstart), comment, &status) )  printerror( status );
	// if ( fits_read_key(fptr, TDOUBLE, "TSTOP", &(fileInfo.tstop), comment, &status) )  printerror( status );
	fits_read_key(fptr, TDOUBLE, "TSTART", &(fileInfo.tstart), comment, &status);
	fits_read_key(fptr, TDOUBLE, "TSTOP", &(fileInfo.tstop), comment, &status);
	fileInfo.ttot   =  (fileInfo.tstop - fileInfo.tstart);
	if (status == 204) {
		cout << "*** WARNING: FITS file " << fitsname.Data() << " with no TSTART or TSTOP info" << endl;
		fileInfo.runid = -1;
		status = 0;
	} else printf("TSTART=%.2f  TSTOP=%.2f  TTOT=%.2f\n", fileInfo.tstart, fileInfo.tstop, fileInfo.ttot);
	
	fits_read_key(fptr, TINT, "OBS_ID", &fileInfo.runid, comment, &status);
	if (status == 204) {
		cout << "*** WARNING: FITS file " << fitsname.Data() << " corrupted: no run-id info! Set run-id to -1" << endl;
		fileInfo.runid = -1;
		status = 0;
	}
	
	char	opmode[16];	// operative mode (grid / burst / PDHU)
	if ( fits_read_key(fptr, TSTRING, "INSTRUME", &opmode, comment, &status) )  printerror( status ); 
	if (strncmp(opmode, "PDHU", 4)==0 || strncmp(opmode, "GRID", 4)==0) {
		fileInfo.pdhuformat=kTRUE;
		fileInfo.pdhudata=kTRUE;
		
		Short_t pkttype, pktstype;
		if ( fits_read_key(fptr, TSHORT, "PKTTYPE", &pkttype, comment, &status) )  printerror( status );
		if ( fits_read_key(fptr, TSHORT, "PKTSTYPE", &pktstype, comment, &status) )  printerror( status );
		fileInfo.pkttype = pkttype*100 + pktstype;
		cout << "packet type: " << fileInfo.pkttype << endl;
		
		if (fileInfo.pkttype==3901 || fileInfo.pkttype==3902) fileInfo.grid = kTRUE;
		else fileInfo.grid = kFALSE;
		if (fileInfo.pkttype!=3913) {
			if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
			if ( fits_read_key(fptr, TINT, "NAXIS2", &fileInfo.nevents, comment, &status) )  printerror( status );
			if ( fits_read_key(fptr, TINT, "TFIELDS", &nfields, comment, &status) )  printerror( status );
		} else fileInfo.nevents=0;
	} 
	else {
		fileInfo.pdhuformat=kFALSE;
		cout << "*** ERROR: file is not in PDHU format!" << endl;
	}
	fits_close_file(fptr, &status);
}

//
//--------------------------------------------------------------------
//

void MCVFile::InitializeTrigTrack() 
{
	cout << "Basic initialization for TRIG & TRACK data" << endl;
	
	// class initialization: reads the appropriate fits keywords and sets class flags

	char strtstart[50]; 
	char strtstop[50];
	fitsfile *fptr;		// fits file pointer
	char aux[200];
	sprintf(aux, "%s[2]",  fitsname.Data());  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &fileInfo.nevents, comment, &status) )  printerror( status );
	fileInfo.pdhuformat=kTRUE;
	fileInfo.pdhudata=kTRUE;
	fileInfo.grid = kTRUE;
	fileInfo.pkttype=1111;
	
	timearray = new Double_t[fileInfo.nevents];
	if (fits_read_col(fptr, TDOUBLE, 51, 1, 1, fileInfo.nevents, 0, timearray, NULL, &status)) printerror(status);
	fileInfo.ttot   =  (timearray[fileInfo.nevents-1] - timearray[0])/1.E6;
	fileInfo.tstart = 0.;
	fileInfo.tstop  = fileInfo.ttot;	
	delete timearray;
	
	sscanf(fitsname.Data(), "PKP%6d", &fileInfo.runid);
	fits_close_file(fptr, &status);
}

//
//--------------------------------------------------------------------
//

void MCVFile::InitializeSimulation() 
{
	cout << "Basic initialization for GAMS + DHSim lv1 data" << endl;
	
	// class initialization: reads the appropriate fits keywords and sets class flags

	char strtstart[50]; 
	char strtstop[50];
	fitsfile *fptr;		// fits file pointer
	char aux[200];
	sprintf(aux, "%s",  fitsname.Data());  
	cout << "trying to open file " << aux << endl;
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	//if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_movabs_hdu(fptr, 2, NULL, &status) )  printerror( status );
 	
	if ( fits_read_key(fptr, TINT, "NAXIS2", &fileInfo.nevents, comment, &status) )  printerror( status );
	fileInfo.pdhuformat=kTRUE;
	fileInfo.pdhudata=kTRUE;
	fileInfo.grid = kTRUE;
	fileInfo.pkttype=2222;
	timearray = new Double_t[fileInfo.nevents];
	if (fits_read_col(fptr, TDOUBLE, 51, 1, 1, fileInfo.nevents, 0, timearray, NULL, &status)) printerror(status);
	fileInfo.ttot   =  (timearray[fileInfo.nevents-1] - timearray[0])/1.E6;
	fileInfo.tstart = 0.;
	fileInfo.tstop  = fileInfo.ttot;	
	delete timearray;
	
	//sscanf(fitsname.Data(), "PKP%6d", &fileInfo.runid);
	fits_close_file(fptr, &status);
}

//
//--------------------------------------------------------------------
//

void MCVFile::LoadCalibrationParameters()
{
	// load calibration parameters from configuration file
	calFile.Append(getenv("MCALSW"));
	if (fileInfo.grid) calFile += stdconf->GetValue(".grid.calibration.file.","grid.conf");
	else calFile += stdconf->GetValue(".burst.calibration.file.","burst.conf");
	cout << "Loading default calibration parameter file: " << calFile.Data() << endl;
	
	conf = new TEnv(calFile);
	barLen = conf->GetValue(".BarLen", 37.5);
	gainFactor = conf->GetValue(".GainFactor", 1.);
	char parameterName[50];
	int ii;
	for (UInt_t i=0; i<30; i++) {  
		if (i<15) ii=i;
		else ii=i+1;
		sprintf(parameterName, ".Gain.Bar%02d.A", ii);
		u0A[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Gain.Bar%02d.B", ii);
		u0B[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Alpha.Bar%02d.A", ii);
		alphaA[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Alpha.Bar%02d.B", ii);
		alphaB[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Offset.Bar%02d.A", ii);
		oA[i] = conf->GetValue(parameterName, 0.);  
		//soA[i] = (Int_t) oA[i];
		sprintf(parameterName, ".Offset.Bar%02d.B", ii);
		oB[i] = conf->GetValue(parameterName, 0.);   
		//soB[i] = (Int_t) oB[i];
	}
	
	// calculate auxiliary parameters for energy/position calculations
	if (gainFactor != 1.) {		// to account for different gain of the grid chains
		for (UInt_t i=0; i<30; i++) {
			u0A[i] *= gainFactor;
			u0B[i] *= gainFactor;
		}
	}
	for (UInt_t i=0; i<30; i++) {
		alphaMean[i]  = (alphaA[i] + alphaB[i])*0.5; 
		alphaDelta[i] = (alphaA[i] - alphaB[i])*0.5;  
		cx1[i] = 0.5 * (log(u0A[i]/u0B[i]) - alphaDelta[i]*barLen)/alphaMean[i];
		cx2[i] = 0.5/alphaMean[i];
		cE1[i] = exp(0.5 * barLen * alphaMean[i]) / sqrt(u0A[i] * u0B[i]);
	}	
	
	// load offset stored in MCAL for scientific ratemeters calculations
	TString offsetFile;
	offsetFile.Append(getenv("MCALSW"));
	offsetFile += stdconf->GetValue(".burst.offset.file.","offset.conf");
	cout << "Loading offset file for Scientific Ratemeters calculations: " << offsetFile.Data() << endl;
	TEnv *offconf = new TEnv(offsetFile);
	for (UInt_t i=0; i<30; i++) {  
		if (i<15) ii=i;
		else ii=i+1;
		sprintf(parameterName, ".Offset.Bar%02d.A", ii);
		soA[i] = (Int_t) offconf->GetValue(parameterName, 0.);  
		sprintf(parameterName, ".Offset.Bar%02d.B", ii);
		soB[i] = (Int_t) offconf->GetValue(parameterName, 0.);   
	}
	delete offconf;
		
	UpdateFileInfo();
}

//
//--------------------------------------------------------------------
//

void MCVFile::SetCalibrationFile(char *newFile) 
{
	// set a new calibration file and reload calibration parameters
	delete conf;
	calFile = newFile;
	LoadCalibrationParameters();
}

//
//--------------------------------------------------------------------
//

void MCVFile::PrintCalibrationParameters()
{
	cout << endl << "Current calibration parameter file: " << calFile.Data() << endl << endl;
	printf("i\t bar\t oA (ch)\t oB (ch)\t u0A (ch/MeV)\t u0B (ch/MeV)\t alphaA (cm^-1)\t alphaB (cm^-1)\n\n");
	for (int i=0; i<30; i++) {
		if (i<15) printf("%d\t X%2d\t ", i, i+1);
		else printf("%d\t Z%2d\t ", i, i-14);
		printf("%8.2f\t %8.2f\t %8.3f\t %8.3f\t %8.4f\t %8.4f\n", oA[i], oB[i], u0A[i], u0B[i], alphaA[i], alphaB[i]);
	}
	cout << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::CalculateEandPos(int cmult, int cbar, Short_t pdA, Short_t pdB)
{
	// calculates energy and position using current calibration parameters
	// and updates scientific ratemeters counters
	float auxE, auxE2, auxPos, auxA, auxB;
	static TF1 *faux;
	
	auxA = ((float) pdA)-oA[cbar] + drand48()-0.5; // random term to smooth binning effects
	auxB = ((float) pdB) -oB[cbar] + drand48()-0.5;
	if (enoise) {
		auxA += rg.Gaus(0., enoise);
		auxB += rg.Gaus(0., enoise);
	}
	if (auxA>0. && auxB>0.) {
		switch (recmodel) {
		
		// default exponential model
		case 0:
			auxPos = cx1[cbar] + cx2[cbar]*log(auxB/auxA);
			auxE   = cE1[cbar]  * sqrt(auxB*auxA);  // * exp(alphaDelta[cbar]*auxPos) 
		break;
		
		// polinomial model (text file, old version)
		case 1:
			faux = (TF1 *) barpol->At(cbar*3+2);
			auxE = auxA/auxB;
			if (auxE >= minAB[cbar] && auxE <= maxAB[cbar]) {
				auxPos = faux->GetX(auxE, minX[cbar], maxX[cbar]);
				if (auxA>auxB) {
					faux = (TF1 *) barpol->At(cbar*3);
					auxE = auxA/faux->Eval(auxPos);
				} else {
					faux = (TF1 *) barpol->At(cbar*3+1);
					auxE = auxB/faux->Eval(auxPos);
				}
			} else {
				auxPos = -100.;
				auxE = 0.;
			}
		break;
		
		// polinomial model (ROOT file, new version, MM 19/3/2007)
		case 2:
			// no cut in ADC
			auxPos = ((TF1 *) fcBonA->At(cbar))->GetX(log(auxB/auxA), -19., 19.);
			if (auxB>auxA) auxE = 1.275*auxB/((TF1 *) fcB->At(cbar))->Eval(auxPos);
			else auxE = 1.275*auxA/((TF1 *) fcA->At(cbar))->Eval(auxPos);
			if (cbar>14) auxPos = -auxPos;	//change versus for Z bars (look at simulations)
			
			// 2 with cut in ADC
			/*if (auxA>10 && auxB>10) {
				auxPos = ((TF1 *) fcBonA->At(cbar))->GetX(log(auxB/auxA), -18.75, 18.75);
				if (auxB>auxA) auxE = 1.275*auxB/((TF1 *) fcB->At(cbar))->Eval(auxPos);
				else auxE = 1.275*auxA/((TF1 *) fcA->At(cbar))->Eval(auxPos);
				if (auxPos == -18.75 || auxPos == 18.75) auxPos = -100.;
			} else {
				auxPos = -100.;
				auxE = 0.;
			}*/
		break;
		}
	} else {
		auxPos = -100.;
		auxE = 0.;
	}
	
	int auxSciRME = pdA + pdB - soA[cbar] - soB[cbar];
	if (cbar<15) currevt.totEX += auxSciRME;
	else currevt.totEZ += auxSciRME;

	if (corrFlag) auxE = Ea[cbar] + Eb[cbar]*auxE;
		
	currevt.E[cmult] = auxE;
	currevt.pos[cmult] = -auxPos;	// segno - per allinearsi al ref sys giusto, verificato con GRB 080721
	currevt.Etot += auxE;
	currevt.rmE[cmult] = auxSciRME;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::UpdateFileInfo()
{
	for (int i=0; i<30; i++) {
		(fileInfo.u0A)[i] = u0A[i];
		(fileInfo.u0B)[i] = u0B[i];
		(fileInfo.alphaA)[i] = alphaA[i];
		(fileInfo.alphaB)[i] = alphaB[i];
		(fileInfo.oA)[i] = oA[i];
		(fileInfo.oB)[i] = oB[i];
		(fileInfo.soA)[i] = soA[i];
		(fileInfo.soB)[i] = soB[i];
	}
	fileInfo.calFile = calFile;
	fileInfo.gainFactor = gainFactor;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::SetSeed(unsigned int s) 
{
	fileInfo.seed = s;
	srand48(fileInfo.seed);				// init seed for random number generator	
};

//
//--------------------------------------------------------------------
//

float MCVFile::LinearCorrection(int ibar, float Ein)
{
	return Ea[ibar] + Eb[ibar]*Ein;
}

//
//--------------------------------------------------------------------
//

void MCVFile::LoadLinCorrParameters()
{
	FILE *flin;
	char plane;
	int bar;
	float cc;
	
	corrFlag = stdconf->GetValue(".use.linear.correction.", FALSE);
	if (corrFlag) {
		TString parname;
		parname.Append(getenv("MCALSW"));
		parname += stdconf->GetValue(".linear.correction.file.", "lin_corr.txt");
		flin = fopen(parname.Data(), "r");
		if (flin) {
			printf("Coefficients for linear correction from file %s:\n", parname.Data());
			printf("Linear fit E = a + bE' \nplane \tbar \ta (MeV) \tb \t\tcorr. coeff. \n");
			fscanf(flin, "plane 	bar 	a (MeV) 	b 		corr. coeff.\n");
			for (int i=0; i<30; i++) {
				fscanf(flin, "%c %d %f %f %f\n", &plane, &bar, &Ea[i], &Eb[i], &cc);
				printf("%c \t%2d \t%8.4f \t%8.4f \t%8.4f \n", (i<15 ? 'X' : 'Z'), (i<15 ? i+1 : i-14), Ea[i], Eb[i], cc);
			}	
			fclose(flin);
		} else {
			cout << "*** Cannot open file " << parname.Data() << endl;
			for (int i=0; i<30; i++) {
				Ea[i] = 0.;
				Eb[i] = 1.;
			}
		}
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::GetPolinomialFunctions()
{
	TString polFile;
	polFile.Append(getenv("MCALSW"));
	polFile += stdconf->GetValue(".burst.polinomial.functions.","pol.root");
	TFile fp(polFile.Data());
	fcA = (TObjArray *) fp.Get("fcA");
	fcB = (TObjArray *) fp.Get("fcB");
	fcBonA = (TObjArray *) fp.Get("fcBonA");
	if (!fcA || !fcB || !fcBonA) {
		cout << "Cannot load polinomial calibration functions from file " << polFile.Data() << endl;
		return;
	}
}

//
//--------------------------------------------------------------------
//

void MCVFile::BuildPolinomialFunctions()
{
	TString polFile;
	FILE *fpol=NULL;
	float par[12];
	double param[12];
	int barid;
	char fname[10];
	TF1 *fptr;
	
	// load polinomial parameters from configuration file
	polFile.Append(getenv("MCALSW"));
	polFile += stdconf->GetValue(".burst.polinomial.parameters.","pol.conf");
	fpol = fopen(polFile.Data(), "r");
	if (!fpol) {
		cout << "*** Cannot open file " << polFile.Data() << endl;
		return;
	}
	barpol = new TObjArray(90, 0);
	printf("bar id \tX-min \tmin \tX_max \tmax \n");
	for (int i=0; i<30; i++) {
		fscanf(fpol, "%d", &barid);
		for (int j=0; j<12; j++) {
			fscanf(fpol, "%f", &par[j]);
			param[j] = par[j]/1.275;	// correct for calibration energy at Na22 1.275 MeV peak
		}
		
		// side A polinomial approximation
		sprintf(fname, "pbar_%d_A", barid);
		fptr = new TF1(fname, "pol5", -25., 25.);
		fptr->SetParameters(param);
		barpol->AddLast(fptr);
		
		// side B polinomial approximation
		sprintf(fname, "pbar_%d_B", barid);
		fptr = new TF1(fname, "pol5", -25., 25.);
		fptr->SetParameters(&param[6]);
		barpol->AddLast(fptr);
		
		// A/B polinomial approximation
		sprintf(fname, "pbar_%d_AB", barid);
		fptr = new TF1(fname, "pol5(0)/pol5(6)", -25., 25.);
		fptr->SetParameters(param);
		barpol->AddLast(fptr);
		
		minX[barid] = fptr->GetMinimumX(-18.75, 18.75);
		minAB[barid] = fptr->GetMinimum(-18.75, 18.75);
		maxX[barid] = fptr->GetMaximumX(-18.75, 18.75);
		maxAB[barid] = fptr->GetMaximum(-18.75, 18.75);
		float aux;
		if (minX[barid] > maxX[barid]) {
			aux = maxX[barid];
			maxX[barid] = minX[barid];
			minX[barid] = aux;
		}
		printf("%d \t%6.2f \t%6.2f \t%6.2f \t%6.2f\n", barid, minX[barid], minAB[barid], maxX[barid], maxAB[barid]);
	}
	fclose(fpol);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile::OpenErrorFile()
{
	TString errname;
	errname += getenv("MCALSW");
	errname += stdconf->GetValue(".error.data.path.", "error");
	errname += fileInfo.runid;
	errname += "_";
	errname += fileInfo.pkttype;
	errname += "_data_error.txt";
	cout << "Open error file: " << errname.Data() << endl;
	ferr = fopen(errname.Data(), "w");
}
