#include <fitsio.h>

#include <iostream.h>
#include <stdlib.h>
#include <math.h>

#include <TROOT.h>
#include <TFile.h>
#include <TEnv.h>
#include <TString.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TGraph.h>
#include "version.h"

#define HKMONITOR_CONF "/share/hkmonitor_conf/hkmonitor_configuration.conf"
#define HKREFERENCE_CONF "/share/hkmonitor_conf/32_01.ql_file2.PPS"

void printerror(int);

int main(int argc, char *argv[])
{
	int 	status = 0;
	int 	nevents;
	int 	parnum;
	int 	colnum;
	int 	typecode;
	int 	obsid;
	long	repeat, width;
	char 	comment[100];
	char 	colname[100];
	char 	strtstart[50]; 
	
	unsigned char	*databyte;
	short 	*datashort;
	double	tstart, tobs, auxtime;
	double	*time;
	float	*data;
	int	isrelative, usephysical;
		
	fitsfile *fptr;
	
	// load configuration file
	TString   calFile;		
	calFile.Append(getenv("MCALSW"));
	calFile += HKMONITOR_CONF;
	cout << "hkmonitor version " << VERSION << endl << "opening configuration file " << calFile.Data() << endl;
	TEnv *conf = new TEnv(calFile.Data());
	if (!conf) {
		cout << "*** cannot find configuration file " << calFile.Data() << endl;
		return 0;
	}
	isrelative  = conf->GetValue(".relative.time.", 1);
	usephysical = conf->GetValue(".use.physical.", 0);
	
	// load A.B. physical units configuration file
	TString unitFile;
	unitFile.Append(getenv("MCALSW"));
	unitFile += HKREFERENCE_CONF;
	TEnv *units = NULL;
	if (usephysical) {
		units = new TEnv(unitFile.Data());
		if (!units) {
			cout << "*** cannot find required physical units file " << unitFile.Data() << endl;
			return 0;
		}
	}
	
	// open ROOT data file
	TString outname;
	if (argc==2) {
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".hkmonitor.file.", "./hkm.root");
	} else outname += argv[2];
	TFile *fh = new TFile(outname.Data(), "UPDATE");  // "RECREATE" "UPDATE"
	
	// load fits file and get start time
	if ( fits_open_file(&fptr, argv[1], READONLY, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status) )  printerror( status );
	if ( fits_movabs_hdu(fptr, 1, NULL, &status) )  printerror( status );
	
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
	
	// allocate dinamic memory
	data = new float[nevents];
	time = new double[nevents];
	databyte = new unsigned char[nevents];
	datashort = new short[nevents];
	
	// get time column and set start time
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )  {
		cout << "could not find column TIME" << endl;
		return 0;
	}
	if (fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevents, 0, time, NULL, &status)) printerror( status );
	cout << "working on file " << argv[1] << "    observation start time " << time[0]*1.E-6 << " s" << endl;
	if (isrelative) tstart = time[0];
	else tstart = 0.;
	
	parnum = conf->GetValue(".parameter.number.", 0);
	
	cout << "HKMON " << obsid << "  " << time[0];
	
	// cycle on each parameter
	double avgval, stdev;
	for (int i=1; i<=parnum; i++) {
		TString parname = ".par.";
		parname += i;
		parname += ".";
		sprintf(colname, "%s", conf->GetValue(parname.Data(), "ERR"));
		if (strcmp(colname,"none")==0) continue;
		if ( fits_get_colnum(fptr, CASEINSEN, colname, &colnum, &status) )  {
			cout << "could not find column " << colname << endl;
			continue;
		}
		//cout << "retrieving column " << i << ": " << colname << endl;
		if ( fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status) )  printerror( status );
		switch (typecode) {
			case TBYTE:
				if (fits_read_col(fptr, typecode, colnum, 1, 1, nevents, 0, databyte, NULL, &status)) printerror(status);
				for (int j=0; j<nevents; j++) data[j] = (float) databyte[j];
			break;
			case TSHORT:
				if (fits_read_col(fptr, typecode, colnum, 1, 1, nevents, 0, datashort, NULL, &status)) printerror(status);
				for (int j=0; j<nevents; j++) data[j] = (float) datashort[j];
			break;
		}
		
		// retrieve information from A.B. physical units configuration file
		// 	token  6: Y-axis unit
		// 	token 11: lower alarm limit
		// 	token 12: upper alarm limit
		//	token 19: calibration row (subtokens divided by ',')
		double aa, bb=0.; // calibration coefficients: y = a + bx
		if (usephysical) {
			TString saux;
			double par[4];
			saux +=  units->GetValue(colname, "wrong string");
			TObjArray *aaux = saux.Tokenize("|");
			TObjArray *aaux2 = (((TObjString *) aaux->At(19))->GetString()).Tokenize(",");
			if ((((TObjString *) aaux2->At(0))->GetString()).IsDigit()) {
				int npt = (((TObjString *) aaux2->At(0))->GetString()).Atoi();
				if (npt==2) for (int k=0; k<4; k++) par[k] = (((TObjString *) aaux2->At(k+1))->GetString()).Atof();
				if (par[2] - par[0] != 0) {
					aa = (par[1]*par[2] - par[3]*par[0])/(par[2] - par[0]);		// intercept
					bb = (par[3] - par[1])/(par[2] - par[0]);			// angular coefficient
				}
			}
			delete aaux2;
			delete aaux;
		}
		
		// retrieve or create graph  
		TGraph	*aux = NULL;
		int nold = 0;
		TString gname = colname;
		fh->GetObject(gname.Data(), aux);
		if (aux == NULL) {
			//cout << "*** there is no graph!" << endl;
			aux = new TGraph(nevents);
			aux->SetName(colname);
			aux->SetTitle(colname);
		} else {
			nold = aux->GetN();
			aux->Set(nold+nevents);
		}
		
		// fill in the graph and save it to file
		avgval = stdev = 0.;
		int nge0 = 0;
		for (int i=0; i<nevents; i++) {
			auxtime = time[i] - tstart;
			if (auxtime>0) {
				if (bb) {
					double atmp = aa + bb*data[i];
					aux->SetPoint(nold+i, auxtime, atmp);		// physical units output
					avgval += atmp;
					stdev  += atmp*atmp;
				} else {
				    	aux->SetPoint(nold+i, auxtime, data[i]);		// ADC units output
					avgval += data[i];
					stdev  += data[i]*data[i];
				}
				nge0++;
			}
		}
		aux->Write(gname.Data(), TObject::kOverwrite);
		avgval /= nge0;
		stdev = sqrt(stdev/nge0 - avgval*avgval);
		cout << "  " << avgval << "  " << stdev;
	}
	cout << endl;
	
	fits_close_file(fptr, &status);
	fh->Close();
	
	delete data;
	delete time;
	delete databyte;
	delete datashort;
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
