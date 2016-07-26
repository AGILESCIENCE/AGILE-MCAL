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

#include "MCVFileTE.h"
#include <fitsio.h>
ClassImp(MCVFileTE);

MCVFileTE::MCVFileTE()
{

}

//
//--------------------------------------------------------------------
//

MCVFileTE::MCVFileTE(const char *name) : MCVFile(name), MCVHisto()
{
	ResizeCountRateHisto(fileInfo.ttot);
	if (fileInfo.gainFactor != 1.) gf=fileInfo.gainFactor;
	
	rootdatapath.Append(getenv("MCALSW"));
	rootdatapath += stdconf->GetValue(".root.data.path.","./");	
	char rootfilename[100];
	sprintf(rootfilename,"RT%06d_%s.root", fileInfo.runid, fileInfo.grid ? "GRID" : "BURST");
	rootdatapath += rootfilename;

}

//
//--------------------------------------------------------------------
//

MCVFileTE::~MCVFileTE()
{

}

//
//--------------------------------------------------------------------
//

void MCVFileTE::CreateRootFile()
{
	if (fp==NULL) {
		
		fp = new TFile(rootdatapath.Data(), "recreate");
		tdata = new TTree("tdata", "MCAL data");
		tdata->Branch("time", &currevt.time, "time/D");
		tdata->Branch("mult", &currevt.mult, "mult/I");
		tdata->Branch("bar", currevt.bar, "bar[30]/S");
		tdata->Branch("pdA", currevt.pdA, "pdA[mult]/S");
		tdata->Branch("pdB", currevt.pdB, "pdB[mult]/S");
		tdata->Branch("pos", currevt.pos, "pos[mult]/F");
		tdata->Branch("E", currevt.E, "E[mult]/F");
		tdata->Branch("Etot", &currevt.Etot, "Etot/F");

		ProcessData();
		
		fileInfo.nevents = tdata->GetEntries();
		
		fileInfo.nlowDT = nlowDT;
		fileInfo.lowDTti = lowDTti;
		fileInfo.lowDTtf = lowDTtf;
		fileInfo.teff = GetEffectiveTime();
		fileInfo.notmonotonic = notmonotonic;
		char infoname[20];
		sprintf(infoname, "run%06d_%s_info", fileInfo.runid, fileInfo.grid ? "GRID" : "BURST");
		fileInfo.SetName(infoname);
		
		tdata->Write();		// write the Root tree to file
		fileInfo.Write(infoname);
		fp->Close();
				
		FillCountRateDistribution();
		FillGlobalHisto(0, fileInfo.teff);
		roothistopath.Append(getenv("MCALSW"));
		roothistopath += stdconf->GetValue(".root.histo.path.","./");	
		char	histofilename[100];
		sprintf(histofilename,"H%06d_%s.root", fileInfo.runid, fileInfo.grid ? "GRID" : "BURST");
		roothistopath += histofilename;
		fh = new TFile(roothistopath.Data(), "recreate");
		rawHisto->Write("rawHisto", TObject::kSingleKey);
		fileInfo.Write(infoname);
		fh->Close();
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFileTE::BuildTimeArray()
{	
	// reads the event TIME column in the TE format fits file and fill an array
	
	timearray = new double[fileInfo.nevents];
	fitsfile *fptr;		// fits file pointer
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if (fits_read_col(fptr, TDOUBLE, 1, 1, 1, fileInfo.nevents, 0, timearray, NULL, &status)) printerror(status);
	fits_close_file(fptr, &status);
}

//
//--------------------------------------------------------------------
//

double *MCVFileTE::GetCountRate(double tini, double tstep, int nstep)
{
	// returns a pointer to an array containing the count rate of MCAL as a single detector
	// this is an auxiliary function needed to calculate FFT of data
	//
	// tini:	initial time (in microseconds) for the calculation (start time = 0.)
	// tstep:	time bin (in microseconds)
	// nstep:	number of steps (length of the returned array)
	
	double	*crate;	// count rate array
	double	ti, dt;
	int nbin;
	
	if (timearray==NULL) BuildTimeArray();
	crate = new double[nstep];
	for (int i=0; i<nstep; i++) crate[i]=0.;
	ti = timearray[0] + tini;
	for (int i=0; i<fileInfo.nevents; i++) {
		dt = timearray[i]-ti;
		if (dt >= 0.) {
			nbin = (int) (dt/tstep);
			if (nbin<nstep) {
				(crate[nbin])++;
			}
			else {
			//	cout << "exit at event " << i << endl;
				break;
			}
		}
	}
	return crate;
}

//
//--------------------------------------------------------------------
//

double *MCVFileTE::GetCountRate(int bar, double tini, double tstep, int nstep)
{
	// returns a pointer to an array containing the count rate on the selected bar
	// this is an auxiliary function needed to calculate FFT of data
	//
	// bar:		the MCAL bar with which data the count rate is calculated by
	// tini:	initial time (in microseconds) for the calculation (start time = 0.)
	// tstep:	time bin (in microseconds)
	// nstep:	number of steps (length of the returned array)
	
	double	*crate;	// count rate array
	double	ti, dt;
	int nbin;
	short *pdA, *pdB;
	
	pdA = new short[fileInfo.nevents];
	pdB = new short[fileInfo.nevents];
	fitsfile *fptr;		// fits file pointer
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if (fits_read_col(fptr, TSHORT, 3+2*bar, 1, 1, fileInfo.nevents, 0, pdA, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 4+2*bar, 1, 1, fileInfo.nevents, 0, pdA, NULL, &status)) printerror(status);
	
	if (timearray==NULL) BuildTimeArray();
	crate = new double[nstep];
	for (int i=0; i<nstep; i++) crate[i]=0.;
	ti = timearray[0] + tini;
	for (int i=0; i<fileInfo.nevents; i++) {
		if (pdA[i]>0 || pdB[i]>0) {
			dt = timearray[i]-ti;
			if (dt >= 0.) {
				nbin = (int) (dt/tstep);
				if (nbin<nstep) {
					(crate[nbin])++;
				}
				else {
				//	cout << "exit at event " << i << endl;
					break;
				}
			}
		}
	}
	delete pdA;
	delete pdB;
	fits_close_file(fptr, &status);
	return crate;
}

//
//--------------------------------------------------------------------
//

void MCVFileTE::ProcessData()
{	
	fitsfile 	*fptr;			// fits file pointer
	int 		istart, nread, nevents, rid=0;
	unsigned long 	timearray[MAXLEN];
	Short_t		pdA[30][MAXLEN];
	Short_t		pdB[30][MAXLEN];
	double		t0;
	double		toffset = 0., dt, tp;
	bool		flag = TRUE;
	Short_t		cmult;
	
	float tfactor = stdconf->GetValue(".burst.time.factor.", 1.);
	
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if ( fits_movnam_hdu(fptr, ANY_HDU, "AGILE_Binary", 0, &status) )  printerror( status );
	
	// get the initial time
	if (fits_read_col(fptr, TULONG, 2, 1, 1, 1, 0, timearray, NULL, &status)) printerror(status);
	t0 = timearray[0];
	tp = t0;
	
	nevents = fileInfo.nevents;
	int ncycle = nevents/MAXLEN;
	cout << "Reading file..." << endl;
	for (istart=1; istart<nevents; istart += MAXLEN) {
		if (nevents-istart>MAXLEN) nread = MAXLEN;
		else nread = nevents-istart+1;
		if (fits_read_col(fptr, TULONG, 2, istart, 1, nread, 0, timearray, NULL, &status)) printerror(status);
		
		// get data block
		for (int i=0; i<30; i++) {
			if (fits_read_col(fptr, TSHORT, 3+2*i, istart, 1, nread, 0, &pdA[i][0], NULL, &status)) printerror(status);
			if (fits_read_col(fptr, TSHORT, 4+2*i, istart, 1, nread, 0, &pdB[i][0], NULL, &status)) printerror(status);
		}

		//main loop		
		for (int i=0; i<nread; i++) {
			rid++;
			if (rid%100000==0) cout << "...processed " << rid << " events..." << endl;
			
			// init new event
			cmult=0;
			for (int j=0; j<30; j++) currevt.bar[j] = -1;
			currevt.Etot = 0.;
			currevt.totEX = 0;
			currevt.totEZ = 0;
			if (timearray[i]-tp < -4.E9) {
				cout << "Internal counter reset at event " << istart+i << endl;
				toffset += 4294967296.;
			}
			currevt.time = (timearray[i]+toffset-t0)*tfactor;
			tp = timearray[i];
			cmult = 0;
			currevt.mult = 0;
			
			for (int j=0; j<30; j++) {
				if (pdA[j][i] > 0) {
					currevt.bar[j] = cmult;
					currevt.pdA[cmult]=pdA[j][i];
					currevt.pdB[cmult]=pdB[j][i];
					CalculateEandPos(cmult, j, pdA[j][i], pdB[j][i]);
					cmult++;
					currevt.mult = cmult;
				}
			}
			tdata->Fill();
			FillHisto(&currevt);
			if (flag && nlowDT==1) {
				cout << "*** first misalignment at event " << tdata->GetEntries() << endl;
				fileInfo.lowDTevt = tdata->GetEntries();
				flag = FALSE;
			}
		}
		
	}
	fits_close_file(fptr, &status);

}
