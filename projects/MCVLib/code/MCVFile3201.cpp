//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MCVFile3201.h"
#include <TMath.h>
#include <fitsio.h>
ClassImp(MCVFile3201);

MCVFile3201::MCVFile3201(const char *name, int v):fi(NULL),fo(NULL),tdata(NULL)
{
	filename += name; 
	isRoot = 0;
	isFits = 0;
	contact = 0;
	nevents = 0;
	
	verbose = v;
	time     = NULL;
	orphase  = NULL;
	plopmode = NULL;
	digitbit = NULL;
	rtmbit   = NULL;
	zstatus  = NULL;
	zvalue   = NULL;
	sentgnd  = NULL;

	
	// check if it is a root file
	if (verbose) cout << "MCVFile3201::MCVFile3201() Opening file " << filename.Data() << endl;
	fi= new TFile(filename.Data());
	if (fi->IsZombie()) { // fi is not a valid root file
		cout << filename.Data() << " is not a valid ROOT file" << endl;
		fi->Close();
	} else {
		isRoot = 1;
		LoadRootFile();
	}
	
	// check if it is a fits file
	if (!isRoot) {
		LoadFitsFile();
		FillTree();
	}
}

//
//--------------------------------------------------------------------
//

MCVFile3201::~MCVFile3201()
{
//	cout << "Closing fits file" << endl;
//	if (fits_close_file(fptr, &status)) printerror(status);
	if (time) delete time;
	if (orphase) delete orphase;
	if (plopmode) delete plopmode;
	if (digitbit) delete digitbit;
	if (rtmbit) delete rtmbit;
	if (zstatus) delete zstatus;
	if (zvalue) delete zvalue;
	if (sentgnd) delete sentgnd;
	
	if (fi) fi->Close();

}

//
//--------------------------------------------------------------------
//

void MCVFile3201::printerror( int status)
{   // Print out cfitsio error messages and exit program
	if (status)  {
		fits_report_error(stderr, status);  // print error report
		//exit( status );                     // terminate the program, returning error status
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile3201::LoadFitsFile()
{
	status = 0;
	fitsfile *fptr;		// fits file pointer
	char aux[200];
	
	if (verbose) cout << "MCVFile3201::LoadFitsFile() Opening fits file " << filename.Data() << endl;
	sprintf(aux, "%s[0]",  filename.Data());  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  {
		printerror( status );
		return;
	}
	isFits = 1;
	fits_read_key(fptr, TINT, "OBS_ID", &contact, comment, &status);
	if (status == 204 && verbose) cout << "*** WARNING: FITS file " << filename.Data() << " corrupted: no contact info! Set contact to 0" << endl;
	else if (verbose) cout << "Loading data for contact " << contact << endl;
	
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
	else if (verbose) cout << "Processing " << nevents << " lines" << endl;
	
	int icol=0;
	char colname[10];
	time     = new Double_t[nevents] ;
	orphase  = new char[nevents];
	plopmode = new char[nevents];
	digitbit = new char[nevents];
	rtmbit   = new char[nevents];
	zstatus  = new char[nevents];
	zvalue   = new Int_t   [nevents] ;
	sentgnd  = new Int_t   [nevents] ;
	
	if (fits_get_colname(fptr, CASESEN, "TIME", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, time,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "M6299a_1_PDHU_Orbit_Phase", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, orphase,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "PLOPMODE", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, plopmode,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "M6394_2_HK_MCAL_DIGIT_BIT", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, digitbit,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "M6393_1_HK_MCAL_RTM_BIT", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, rtmbit,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "M6746_7_Zombie_Mode_Status", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, zstatus,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "M6749_2_Zombie_Event_Counter_Value", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, zvalue,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "M6320_0_PDHU_GRID_events_sent_to_ground", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, sentgnd,   NULL,&status)) printerror(status);	
	
	fits_close_file(fptr, &status);	
	
	
}

//
//--------------------------------------------------------------------
//

void MCVFile3201::PrintResults()
{
/*	for (int i=0; i<nevents; i++) {
		if (1) {
			printf("%4d \t%8.3f \t%8.3f \t%10.3f\n", i, evt.lon, evt.lat, evt.h);
		}
	}*/
}

//
//--------------------------------------------------------------------
//

void MCVFile3201::FillTree()
{
	if (!isFits) return;
	
	// init root tree
	tdata = new TTree("tdata3201", "HK data");
	tdata->Branch("time", &evt.time, "time/D");
	tdata->Branch("orphase", &evt.orphase, "orphase/B");  
	tdata->Branch("plopmode", &evt.plopmode, "plopmode/B");  
	tdata->Branch("digitbit", &evt.digitbit, "digitbit/B");  
	tdata->Branch("rtmbit", &evt.rtmbit, "rtmbit/B");  
	tdata->Branch("zstatus", &evt.zstatus, "zstatus/B");  
	tdata->Branch("zvalue", &evt.zvalue, "zvalue/I");  
	tdata->Branch("sentgnd", &evt.sentgnd, "sentgnd/I");  
	
	// loop on data and fill tree
	if (verbose) cout << "Filling the root tree... " << endl;
	double tprev = time[0] - 1.;
	for (int i=0; i<nevents; i++) {
		if (1) {	
			evt.time = time[i];
			evt.orphase = orphase[i];
			evt.plopmode = plopmode[i];
			evt.digitbit = digitbit[i];
			evt.rtmbit = rtmbit[i];
			evt.zstatus = zstatus[i];
			evt.zvalue = zvalue[i];
			evt.sentgnd = sentgnd[i];
			tdata->Fill();
			
			// check whether time is monotonic
			if (time[i] < tprev) {
				if (verbose) cout << "*** gps time for contact " << contact << " not monotonic at row " << i << endl;
			}
			tprev = time[i];
		}
	}
	mintime = tdata->GetMinimum("time");
	maxtime = tdata->GetMaximum("time");
	nentries = tdata->GetEntries();
	if (verbose) printf("%6d entries recorded. Min time = %20.6f  Max time = %20.6f\n", nentries, mintime, maxtime);
	printf("INDEX %6d \t%20.6f  \t%20.6f\n", contact, mintime, maxtime);
}

//
//--------------------------------------------------------------------
//

void MCVFile3201::WriteRootFile(const char *name)
{
	if (isFits && tdata) {
		outname += name;
		cout << "Writing tree to root file " << outname.Data() << endl;
		fo = new TFile(outname.Data(), "recreate");
		tdata->Write();		// write the Root tree to file
		fo->Close();
	} else cout << "*** cannot write to file " << outname.Data() << endl;
}

//
//--------------------------------------------------------------------
//

void MCVFile3201::LoadRootFile()
{
	tdata = (TTree *) fi->Get("tdata3201");
	if (!tdata) {
		cerr << "*** cannot find tree tdata3201" << endl;
		return;
	}
/*	tdata->SetBranchAddress("time", &evt.time);
	tdata->SetBranchAddress("lon", &evt.lon);
	tdata->SetBranchAddress("lat", &evt.lat);
	tdata->SetBranchAddress("h", &evt.h);
	tdata->SetBranchAddress("etheta", &evt.etheta);
	tdata->SetBranchAddress("ephi", &evt.ephi);
	mintime = tdata->GetMinimum("time");
	maxtime = tdata->GetMaximum("time");
	nentries = tdata->GetEntries();
	if (verbose) printf("%6d entries recorded. Min time = %20.6f  Max time = %20.6f\n", nentries, mintime, maxtime);
*/
}
