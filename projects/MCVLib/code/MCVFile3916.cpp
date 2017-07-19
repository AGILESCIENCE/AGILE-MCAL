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
#include "MCVFile3916.h"
#include <TMath.h>
#include <fitsio.h>
ClassImp(MCVFile3916);

MCVFile3916::MCVFile3916(const char *name, int v, int e):fi(NULL),fo(NULL),tdata(NULL)
{
	filename += name; 
	isRoot = 0;
	isFits = 0;
	contact = 0;
	nevents = 0;
	
	verbose = v;
	eci = e;
	
	iX = iY = iZ = NULL;
	time = ephi = etheta = NULL;
	posvali = NULL;	
	
	// GPS ellipsoid reference (WGS84), data in km
	a = 6378137.;
	b = 6356752.31424518;
	f = 1./298.257223563;
	e = sqrt((a*a - b*b)/(a*a));
	e1 = sqrt((a*a - b*b)/(b*b));
	rad2deg = TMath::RadToDeg(); 

	// data for calculating the Greenwich Mean Sidereal Time
	o1 = 18.697374558;
	m  = 24.06570982441908;
	o2 = 1460.5; 
	
	// check if it is a root file
	if (verbose) cout << "MCVFile3916::MCVFile3916() Opening file " << filename.Data() << endl;
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

MCVFile3916::~MCVFile3916()
{
//	cout << "Closing fits file" << endl;
//	if (fits_close_file(fptr, &status)) printerror(status);
	if (iX) delete iX;
	if (iY) delete iY;
	if (iZ) delete iZ;
	if (time) delete time;
	if (posvali) delete posvali;
	if (ephi) delete ephi;
	if (etheta) delete etheta;
	if (fi) fi->Close();

}

//
//--------------------------------------------------------------------
//

void MCVFile3916::printerror( int status)
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

void MCVFile3916::LoadFitsFile()
{
	status = 0;
	fitsfile *fptr;		// fits file pointer
	char aux[200];
	
	if (verbose) cout << "MCVFile3916::LoadFitsFile() Opening fits file " << filename.Data() << endl;
	sprintf(aux, "%s[0]",  filename.Data());  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  {
		printerror( status );
		return;
	}
	isFits = 1;
	fits_read_key(fptr, TINT, "OBS_ID", &contact, comment, &status);
	if (status == 204 && verbose) cout << "*** WARNING: FITS file " << filename.Data() << " corrupted: no contact info! Set contact to 0" << endl;
	else if (verbose) cout << "Loading data for contact " << contact << endl;
	
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
	else if (verbose) cout << "Processing " << nevents << " lines" << endl;
	
	int icol=0;
	char colname[10];
	iX = new Long_t   [nevents] ;
	iY = new Long_t   [nevents] ;
	iZ = new Long_t   [nevents] ;
	time   = new Double_t[nevents] ;
	posvali  = new char[nevents];
	ephi   = new Double_t[nevents] ;
	etheta   = new Double_t[nevents] ;
	if (fits_get_colname(fptr, CASESEN, "GPSXPOS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, iX,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "GPSYPOS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, iY,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "GPSZPOS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, iZ,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "TIME", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, time,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "POSVALI", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, posvali,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "EARTH_PHI", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, ephi,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "EARTH_THETA", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, etheta,   NULL,&status)) printerror(status);
	
	fits_close_file(fptr, &status);	
	
	
}

//
//--------------------------------------------------------------------
//

void MCVFile3916::Eci2lla(double X, double Y, double Z, double time)
{
	// calculate Greenwich Mean Sidereal Time
	double gmst = o1 + m*(time/86400. + o2); // in hours
	double agmst = (gmst / 24. - TMath::Floor(gmst/24.)) * TMath::TwoPi(); // in radiants
 
	// convert from ECI to ECEF: do rotation of GMST
	TVector3 veci(X, Y, Z);
	veci.RotateZ(-agmst);

	// convert from ECEF to lon,lat
	Ecef2lla(veci.X(), veci.Y(), veci.Z());
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFile3916::Ecef2lla(double X, double Y, double Z)
{
	if(X) {	// sometimes GPS data are zero!
		p = TMath::Sqrt(X*X + Y*Y);
		theta = TMath::ATan((Z*a)/(p*b));
		
		// get LLA geographycal coordinates	
		evt.lon = TMath::ATan(Y/X);
		evt.lat = TMath::ATan((Z + e1*e1*b*TMath::Sin(theta)*TMath::Sin(theta)*TMath::Sin(theta)) / (p - e*e*TMath::Cos(theta)*TMath::Cos(theta)*TMath::Cos(theta)));
		N = a/TMath::Sqrt(1. - e*e*TMath::Sin(evt.lat)*TMath::Sin(evt.lat));
		evt.h = p/TMath::Cos(evt.lat) - N;
		
		// correct for arctangent degeneracy
		evt.lon *= rad2deg;
		evt.lat *= rad2deg;
		if (X<0. && Y>0.) evt.lon += 180.;
		else if (X<0. && Y<0.) evt.lon -= 180.;
	
	} else evt.h = -1.;

}

//
//--------------------------------------------------------------------
//

void MCVFile3916::PrintResults()
{
	for (int i=0; i<nevents; i++) {
		if (posvali[i]) {
			if (eci) Eci2lla(iX[i]*0.01, iY[i]*0.01, iZ[i]*0.01,time[i]);
			else Ecef2lla(iX[i]*0.01, iY[i]*0.01, iZ[i]*0.01);
			printf("%4d \t%8.3f \t%8.3f \t%10.3f\n", i, evt.lon, evt.lat, evt.h);
		}
	}
}

//
//--------------------------------------------------------------------
//

void MCVFile3916::FillTree()
{
	if (!isFits) return;
	
	// init root tree
	tdata = new TTree("tdata3916", "orbital data");
	tdata->Branch("gpsxpos", &evt.gpsxpos, "gpsxpos/D");
	tdata->Branch("gpsypos", &evt.gpsypos, "gpsypos/D");
	tdata->Branch("gpszpos", &evt.gpszpos, "gpszpos/D");
	tdata->Branch("posvali", &evt.posvali, "posvali/I");
	tdata->Branch("time", &evt.time, "time/D");
	tdata->Branch("lon", &evt.lon, "lon/D");
	tdata->Branch("lat", &evt.lat, "lat/D");
	tdata->Branch("h", &evt.h, "h/D");
	tdata->Branch("etheta", &evt.etheta, "etheta/D");
	tdata->Branch("ephi", &evt.ephi, "ephi/D");
	
	// loop on data and fill tree
	if (verbose) cout << "Filling the root tree... " << endl;
	double tprev = time[0] - 1.;
	for (int i=0; i<nevents; i++) {
		if (posvali[i]) {
		
			// do ECEF or ECI -> LLA coordinate conversion
			if (eci) Eci2lla(iX[i]*0.01, iY[i]*0.01, iZ[i]*0.01,time[i]);
			else Ecef2lla(iX[i]*0.01, iY[i]*0.01, iZ[i]*0.01);
		} else {
		
			// no valid position: put fake numbers
			evt.lon = -100.;
			evt.lat = -100.;
			evt.h = -100.;
		}
	
		// check whether time is monotonic
		if (time[i] < tprev) {
			if (verbose) cout << "*** gps time for contact " << contact << " not monotonic at row " << i << endl;
		}
		tprev = time[i];
		
		evt.gpsxpos = iX[i];
		evt.gpsypos = iY[i];
		evt.gpszpos = iZ[i];
		evt.posvali = posvali[i];
		evt.etheta = etheta[i];
		evt.ephi = ephi[i];
		evt.time = time[i];
		tdata->Fill();
			
	}
	//mintime = tdata->GetMinimum("time");
	//maxtime = tdata->GetMaximum("time");
	mintime = time[0]; // like reading keywords in the fits header
	maxtime = time[nevents-1];
	nentries = tdata->GetEntries();
	if (verbose) printf("%6d entries recorded. Min time = %20.6f  Max time = %20.6f\n", nentries, mintime, maxtime);
	printf("INDEX %6d \t%20.6f  \t%20.6f\n", contact, mintime, maxtime);
}

//
//--------------------------------------------------------------------
//

void MCVFile3916::WriteRootFile(const char *name)
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

int MCVFile3916::GetCoordinates(gpsevt *aevt)
{
cout << aevt->gpsxpos << "  " << aevt->gpsypos << "  " << aevt->gpszpos << "  " << aevt->posvali << " " << aevt->time << "  " << aevt->lon << "  " << aevt->lat << "  " << aevt->h << "  " << aevt->etheta << "  " << aevt->ephi << endl; 
	int returnvalue = 0;
	if (!tdata) return -2;
	
	// interpolate coordinates at aevt.time
	if (aevt->time < mintime) {
		if (verbose) printf("MCVFile3916::GetCoordinates() t0 %20.6f < minimum time %20.6f\n", aevt->time, mintime);
		returnvalue = -1;
	}
	if (aevt->time > maxtime) {
		if (verbose) printf("MCVFile3916::GetCoordinates() t0 %20.6f > maximum time %20.6f\n", aevt->time, maxtime);
		returnvalue = 1;
	}
	
	int id=0;
	do {
		tdata->GetEntry(id++);
	} while(evt.time < aevt->time && id<nentries);
	
	if (id == 1) tdata->GetEntry(1);
	double t2 = evt.time;
	double lon2 = evt.lon;
	double lat2 = evt.lat;
	double h2 = evt.h;
	double etheta2 = evt.etheta;
	double ephi2 = evt.ephi;
	
	if (id == 1) tdata->GetEntry(0);
	else tdata->GetEntry(id-2);
	double t1 = evt.time;
	double lon1 = evt.lon;
	double lat1 = evt.lat;
	double h1 = evt.h;
	double etheta1 = evt.etheta;
	double ephi1 = evt.ephi;
	
	// adjust for 2Pi periodicity
	if (lon2 < lon1) lon2 += 360.;
	
	// do linear interpolation
	if (verbose) cout << "Interpolate between t0 - " << aevt->time -t1 << " and t0 + " << t2 - aevt->time << endl;
	double m = (aevt->time -t1)/(t2-t1);
	aevt->lon = lon1 + m*(lon2-lon1);
	aevt->lat = lat1 + m*(lat2-lat1);
	aevt->h = h1 + m*(h2-h1);
	aevt->ephi = ephi1 + m*(ephi2-ephi1);
	aevt->etheta = etheta1 + m*(etheta2-etheta1);
	
	if (aevt->lon > 180.) aevt->lon -= 360.;
	if (verbose) cout << "Lon = " << aevt->lon << "  Lat = " << aevt->lat << "  return flag = " << returnvalue << endl;
	
	// added MM 02/04/2012 to check whether interpolation between large times is performed
	if (t2 - t1 <=0. ) returnvalue = -3;
	if (t2 - t1 > 33.) returnvalue = -4;
	
	return returnvalue;
}


//
//--------------------------------------------------------------------
//

void MCVFile3916::LoadRootFile()
{
	tdata = (TTree *) fi->Get("tdata3916");
	if (!tdata) {
		cerr << "*** cannot find tree tdata3916" << endl;
		return;
	}
	tdata->SetBranchAddress("time", &evt.time);
	tdata->SetBranchAddress("lon", &evt.lon);
	tdata->SetBranchAddress("lat", &evt.lat);
	tdata->SetBranchAddress("h", &evt.h);
	tdata->SetBranchAddress("etheta", &evt.etheta);
	tdata->SetBranchAddress("ephi", &evt.ephi);
	mintime = tdata->GetMinimum("time");
	maxtime = tdata->GetMaximum("time");
	nentries = tdata->GetEntries();
	if (verbose) printf("%6d entries recorded. Min time = %20.6f  Max time = %20.6f\n", nentries, mintime, maxtime);
}
