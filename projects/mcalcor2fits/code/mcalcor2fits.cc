#include "mcalcor2fits.h"
#include "version.h"

using namespace std ; 

void printerror( int );

int main(int argc, char *argv[])
{
	int status = 0;
        fitsfile *fptr=NULL; 
	int nevents;
	int colnum;
	char nometemp[200];	
	double		*timearray;
	short int	*pd1A;
	short int	*pd1B;
	short int	*pd2A;
	short int	*pd2B;
	char		*flg;
	char		*bar1;
	char		*bar2;
	char		comment[100];
	
	cout << "Program mcalcor2fits version " << VERSION << endl;
	
	unsigned int seed = (unsigned int)time( NULL );	// calculate seed for this session
	srand48(seed);					// init seed for random number generator
	
	// -----------------------------
	// open input file and load data
	// -----------------------------
	
	cout << "opening file " << argv[1] << endl;
        if ( fits_open_file(&fptr, argv[1], READONLY, &status) )  printerror( status );
         if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) ) printerror( status );
	cout << "number of lines = " << nevents << endl;
	
	timearray = new double[nevents];
	pd1A = new short int[nevents];
	pd1B = new short int[nevents];
	flg = new char[nevents];
	pd2A = new short int[nevents];
	pd2B = new short int[nevents];
	bar1 = new char[nevents];
	bar2 = new char[nevents];
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )  printerror( status );
	if (fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevents, 0, timearray, NULL, &status)) printerror( status );
        if (fits_read_col(fptr, TSHORT, 5, 1, 1, nevents, 0, pd1A, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 6, 1, 1, nevents, 0, pd1B, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 7, 1, 1, nevents, 0, flg, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 8, 1, 1, nevents, 0, pd2A, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TSHORT, 10, 1, 1, nevents, 0, pd2B, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 11, 1, 1, nevents, 0, bar1, NULL, &status)) printerror(status);
        if (fits_read_col(fptr, TBYTE, 13, 1, 1, nevents, 0, bar2, NULL, &status)) printerror(status);
	
        fits_close_file(fptr, &status);
	
	// -----------------------------
        // load calibration parameters
	// -----------------------------
	
	double barLen = 37.5;		// Bar length (default = 37.5 cm)
	double gainFactor;	// Gain multiplicative coefficient for GRID energy calculations (~1.2)
	double u0A[30];		// PD-A gain near PD (ch/MeV)
	double u0B[30];		// PD-B gain near PD (ch/MeV)
	double alphaA[30];	// PD-A attenuation coefficient (cm^-1)
	double alphaB[30];	// PD-B attenuation coefficient (cm^-1)
	double oA[30];		// Offset of PD-A (ADC channels)
	double oB[30];		// Offset of PD-B (ADC channels)
	double alphaMean[30];   // Mean attenuation coefficient = 0.5*(alphaA+alphaB)
	double alphaDelta[30];  // Attenuation coefficient semidispersion = 0.5*(alphaA-alphaB)
	double cx1[30];   	// Auxiliary coefficient for position calculation
	double cx2[30];   	// Auxiliary coefficient for position calculation
	double cE1[30];   	// Auxiliary coefficient for position calculation
	
	// load calibration data file
	ifstream fin;
	fin.open(CALFILE, ifstream::in);
	cout << "Reading calibration data file " << CALFILE << endl;
	if (VERBOSE) printf("i\t bar\t oA (ch)\t oB (ch)\t u0A (ch/MeV)\t u0B (ch/MeV)\t alphaA (cm^-1)\t alphaB (cm^-1)\n\n");

	int id;
	for (int i=0; i<30; i++) {
		fin >> id >> oA[i] >> oB[i] >> u0A[i] >> u0B[i] >> alphaA[i] >> alphaB[i];
		
		// build auxiliary parameters
		alphaMean[i]  = (alphaA[i] + alphaB[i])*0.5; 
		alphaDelta[i] = (alphaA[i] - alphaB[i])*0.5;  
		cx1[i] = 0.5 * (log(u0A[i]/u0B[i]) - alphaDelta[i]*barLen)/alphaMean[i];
		cx2[i] = 0.5/alphaMean[i];
		cE1[i] = exp(0.5 * barLen * alphaMean[i]) / sqrt(u0A[i] * u0B[i]);
	
		if (VERBOSE) {	// print calibration parameters to stdout
			if (i<15) printf("%d\t X%2d\t ", i, i+1);
			else printf("%d\t Z%2d\t ", i, i-14);
			printf("%8.2f\t %8.2f\t %8.3f\t %8.3f\t %8.4f\t %8.4f\n", oA[i], oB[i], u0A[i], u0B[i], alphaA[i], alphaB[i]);
		}
	}
	
	// -----------------------------
        // main conversion loop
	// -----------------------------
        
	int i=0;
        double tprev=0.;
	short int cmult;	// multiplicity of the current event
	int cbar;		// bar index of the current event

	int nevt=0;
	
	// create and init data array
	double *time   = new double[nevents];
	short int    *mult   = new short int[nevents];
	float  *energy[30];
	float  *pos[30];	
	for (int j=0; j<30; j++) {
		energy[j] = new float[nevents];
		pos[j]    = new float[nevents];
		for (int k=0; k<nevents; k++) {
			(energy[j])[k] = 0.;
			(pos[j])[k] = 0.;
		}
	}
	
        do {
                if (VERBOSE && i%100000==0) cout << "...processed " << i << " events..." << endl;
                if (timearray[i] < tprev) cout << "*** Time not monotonic at line " << i << " time " << timearray[i] << endl;
                tprev = timearray[i];
                cmult=0;
                do {
                        cbar = bar1[i];
                        if (cbar!=15 && cbar!=31) {
                                // process first bar
                                if (cbar>15) cbar--;
                                CalculateEandPos(nevt, cbar, pd1A[i], pd1B[i], oA, oB, cx1, cx2, cE1, energy[cbar], pos[cbar]);
                                cmult++;
                        } else {
                                // something is wrong
                                cout << "*** bar " << cbar << " detected at event " << i << endl;
                        }
                        if(flg[i]==15) {        
                                cbar = bar2[i];
                                if (cbar!=15 && cbar!=31) {
                                        // process second bar, if any
                                        if (cbar>15) cbar--;
                                        CalculateEandPos(nevt, cbar, pd2A[i], pd2B[i], oA, oB, cx1, cx2, cE1, energy[cbar], pos[cbar]);
                                        cmult++;                                
                                } else {
                                        // something is wrong
                                        cout << "*** bar " << cbar << " detected at event " << i << endl;
                                }                               
                        }
                        i++;
                } while (i<nevents && timearray[i]==tprev);
		
		mult[nevt] = cmult;
		time[nevt] = tprev;
		nevt++;
		
	} while (i<nevents);
	cout << "number of events = " << nevt << endl;
	
	
	delete timearray;
	delete pd1A;
	delete pd1B;
	delete flg;
	delete pd2A;
	delete pd2B;
	delete bar1;
	delete bar2;
	
	// -----------------------------
	// create output fits file from template 
	// -----------------------------
	
     	char errtext[40] ; // fitsio error text : max per fitsio  30 char + '/0'
	int esiste_fits ; 
	int status_fits=0;         // fitsio error flag 
        fitsfile *fout; 
    
	cout << "creating output file " << argv[2] << " from template file " <<  STDTEMPLATE << endl;
	if( fits_file_exists(argv[2],&esiste_fits, &status_fits) )
	{  
		fits_get_errstatus( status_fits, errtext);
		std::cerr << "fitsFile::fitsFile Errore in test esistenza, fitsio number: " 
			<< status_fits << std::endl ; 
		std::cerr << errtext << std::endl  ;
	}
	
	if(esiste_fits != 0 )
	{
		std::cerr << "fitsFile::fitsFile Attenzione, il file: " 
			<< argv[2] << " esiste gia' ..." << std::endl ; 
		return 1;
	}   
	
	sprintf(nometemp, "%s", STDTEMPLATE);
	if( fits_create_template(&fout , argv[2] ,nometemp, &status_fits) )
	{  
		fits_get_errstatus(status_fits, errtext);
		std::cerr << " Errore in create template, fitsio number: " << status_fits << std::endl ; 
		std::cerr << errtext << std::endl  ;
		std::cerr << "template name: " << nometemp << std::endl  ;
		std::cerr << "fits name: " << argv[2] << std::endl  ;
		return 1 ; 
	}
	
	// -----------------------------
	// write data to output fits file
	// -----------------------------
	
	cout << "writing data to output file " << endl;
	if ( fits_movnam_hdu(fout, ANY_HDU, "EVENTS", 0, &status_fits) )  printerror( status_fits );
	if ( fits_write_col(fout, TDOUBLE, 1, 1, 1, nevt, time, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TSHORT,  2, 1, 1, nevt, mult, &status_fits)  )  printerror( status_fits );
	for (int j=0; j<30; j++) {
		if ( fits_write_col(fout, TFLOAT, 3+2*j, 1, 1, nevt, energy[j], &status_fits)  )  printerror( status_fits );
		if ( fits_write_col(fout, TFLOAT, 4+2*j, 1, 1, nevt, pos[j], &status_fits)  )  printerror( status_fits );	
	}
	cout << endl;
   	fits_close_file(fout, &status_fits) ;
	
	// -----------------------------
	// free memory
	// -----------------------------
	
	delete time;
	for (int j=0; j<30; j++) {
		delete energy[j];
		delete pos[j];
	}
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

//
//--------------------------------------------------------------------
//

void CalculateEandPos(int nevt, int cbar, short int pdA, short int pdB, double *oA, double *oB, double *cx1, double *cx2, double *cE1, float *e, float *p )
{
	float auxE, auxE2, auxPos, auxA, auxB;
	
	auxA = ((float) pdA)-oA[cbar] + drand48()-0.5; // random term to smooth binning effects
	auxB = ((float) pdB) -oB[cbar] + drand48()-0.5;
	if (auxA>0. && auxB>0.) {
		auxPos = cx1[cbar] + cx2[cbar]*log(auxB/auxA);
		auxE   = cE1[cbar]  * sqrt(auxB*auxA);  // * exp(alphaDelta[cbar]*auxPos) 
	} else {
		auxPos = -100.;
		auxE = 0.;
	}
	e[nevt] = auxE;
	p[nevt] = -auxPos;
	
	return;
}

