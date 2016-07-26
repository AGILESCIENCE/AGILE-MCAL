#include "root2fits.h"
#include "version.h"
// #include "phlistFile.h" 

using namespace std ; 

void printerror( int );

int main(int argc, char *argv[])
{
	TFile *fi;
	TObjArray *rh;
	MCVFileInfo *info=NULL;
	TTree *data=NULL;
	MCALevt inevt;
	
	int status = 0;
        fitsfile *fptr=NULL; 
	int nevents;
	int colnum;
	char nometemp[200];	
	char	comment[100];
	int runid = 0;
	
	cout << "root2fits version " << VERSION << endl; 
	if (argc != 3) {
		cout << "Usage: root2fits <original root file name> <destination fits file name>" << endl;
		return 0;
	}

 	string phlisttemplate = getenv("MCALSW");
	phlisttemplate += "/share/mcalfits_conf/phlisttemplate.txt" ;
	// phlistFile *phlist = new phlistFile(argv[2],phlisttemplate); 

	
	
	
	// -----------------------------
	// open input file and load data
	// -----------------------------	
	
	// load root events file
	cout << "Opening rootfile " << argv[1] << endl;
	fi= new TFile(argv[1]);
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << argv[1] << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {
	
		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
			if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
		}
		if (info) {
			info->PrintFileInfo();
			runid = info->runid;
		}
		nevents = data->GetEntries();
		
		if (!info) {
			cout << "Cannot find info file: exiting!" << endl;
			return 1;
		}
		
		
		if (!data) {
			cout << "Cannot find data tree: exiting!" << endl;
			return 1;
		}
	}
	
	// connect the input tree
	data->SetBranchAddress("time", &inevt.time);
	data->SetBranchAddress("mult", &inevt.mult);
	data->SetBranchAddress("bar", inevt.bar);
	data->SetBranchAddress("pdA", inevt.pdA);
	data->SetBranchAddress("pdB", inevt.pdB);
	data->SetBranchAddress("pos", inevt.pos);
	data->SetBranchAddress("E", inevt.E);
	data->SetBranchAddress("Etot", &inevt.Etot);
	
	// allocate arrays for PHOTLIST HDU
	float  		*Etot   = new float[nevents];
	float  		*posmax = new float[nevents];
	short int 	*ibar   = new short int[nevents];
	short int 	*mult   = new short int[nevents];
	
	// allocate arrays for EVENTS HDU
	double 		*time = new double[nevents];
	float  		*energy[30];
	float  		*pos[30];	
	for (int j=0; j<30; j++) {
		energy[j] = new float[nevents];
		pos[j]    = new float[nevents];
		for (int k=0; k<nevents; k++) {
			(energy[j])[k] = 0.;
			(pos[j])[k] = 0.;
		}
	}
	
	cout << "Processing " << nevents << " events." << endl;
	for (int i=0; i<nevents; i++) {
		if (i % 20000 == 0) cout << "...processed " << i << " events..." << endl;
		data->GetEntry(i);
		
		// get the bar with highest energy deposit
		double auxemax = -1.;
		float auxposmax = -100.;
		short int auxibar = -1;
		for (int k=0; k<30; k++) {
			int id = inevt.bar[k];
			if (id>=0) { 
				
				(energy[k])[i] = (inevt.E)[id];
				(pos[k])[i] = (inevt.pos)[id];
			
				if ((inevt.E)[id] > auxemax) {
					auxemax = (inevt.E)[id];
					auxposmax = (inevt.pos)[id];
					auxibar = k;
				}
			}
			if (id == inevt.mult - 1) break;
		}
		
		time[i]   = inevt.time;
		Etot[i]   = inevt.Etot;
		mult[i]   = inevt.mult;
		posmax[i] = auxposmax;
		ibar[i]   = auxibar;
	}
	
	// -----------------------------
	// create output fits file from template 
	// -----------------------------
	
     	char errtext[40] ; // fitsio error text : max per fitsio  30 char + '/0'
	int esiste_fits ; 
	int status_fits=0;         // fitsio error flag 
        fitsfile *fout; 
    
	cout << "creating output file " << argv[2] << " from template file " <<  phlisttemplate << endl;
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
	
	sprintf(nometemp, "%s", phlisttemplate.data());
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
	
	cout << "writing data to output file: PHOTLIST HDU" << endl;
	if ( fits_movnam_hdu(fout, ANY_HDU, "PHOTLIST", 0, &status_fits) )  printerror( status_fits );
	
	if ( fits_update_key(fout, TINT, "OBSID", &runid, comment, &status_fits) )  printerror( status_fits );
	char ver[20];
	sprintf(ver, "%s", VERSION);
	if ( fits_update_key(fout, TSTRING, "VERSION", ver, comment, &status_fits) )  printerror( status_fits );	
	
	if ( fits_write_col(fout, TDOUBLE, 1, 1, 1, nevents, time, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TFLOAT,  2, 1, 1, nevents, Etot, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TSHORT,  3, 1, 1, nevents, mult, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TSHORT,  4, 1, 1, nevents, ibar, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TFLOAT,  5, 1, 1, nevents, posmax, &status_fits)  )  printerror( status_fits );
	
	cout << "writing data to output file: EVENTS HDU" << endl;
	if ( fits_movnam_hdu(fout, ANY_HDU, "EVENTS", 0, &status_fits) )  printerror( status_fits );
	if ( fits_write_col(fout, TDOUBLE, 1, 1, 1, nevents, time, &status_fits)  )  printerror( status_fits );
	if ( fits_write_col(fout, TSHORT,  2, 1, 1, nevents, mult, &status_fits)  )  printerror( status_fits );
	for (int j=0; j<30; j++) {
		if ( fits_write_col(fout, TFLOAT, 3+2*j, 1, 1, nevents, energy[j], &status_fits)  )  printerror( status_fits );
		if ( fits_write_col(fout, TFLOAT, 4+2*j, 1, 1, nevents, pos[j], &status_fits)  )  printerror( status_fits );	
	}
	cout << endl;
   	fits_close_file(fout, &status_fits) ;
	
	// -----------------------------
	// free memory
	// -----------------------------
	
	delete time;
	delete Etot;
	delete posmax;
	delete mult;
	delete ibar;
	for (int j=0; j<30; j++) {
		delete energy[j];
		delete pos[j];
	}
	
	
/*	// writing PHOTLIST HDU
	
	cout << "Processing " << nevents << " events." << endl;
	cout << "Writing PHOTLIST HDU:" << endl;
	for (int i=0; i<nevents; i++) {
		if (i % 20000 == 0) cout << "...processed " << i << " events..." << endl;
		data->GetEntry(i);
		
		// get the bar with highest energy deposit
		double emax = -1.;
		float posmax = -100.;
		short int ibar = -1;
		for (int k=0; k<30; k++) {
			int id = inevt.bar[k];
			if (id>=0 && (inevt.E)[id] > emax) {
				emax = (inevt.E)[id];
				posmax = (inevt.pos)[id];
				ibar = k;
			}
			if (id == inevt.mult - 1) break;
		}
		
		// write row to fits file
		phlist->outLine(inevt.time, inevt.Etot, inevt.mult, ibar, posmax);	
	}
	phlist->setIntKey("OBSID", info->runid, "PHOTLIST");
	char str[20];
	sprintf(str, "%s", VERSION);
	phlist->setStringKey("VERSION", str, "PHOTLIST");
	
	// writing EVENTS HDU
  	// NB: need to separate the routines working on different HDU because the HDU shift is the most time consuming operation!!!
	
	cout << "Writing EVENTS HDU:" << endl;
	for (int i=0; i<nevents; i++) {
		if (i % 20000 == 0) cout << "...processed " << i << " events..." << endl;
		data->GetEntry(i);
		
		// write row to fits file
		phlist->outArrayLine(inevt.time, inevt.mult, inevt.bar, inevt.E, inevt.pos);	
	}
	
	phlist->chiudi() ;
*/
	
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
