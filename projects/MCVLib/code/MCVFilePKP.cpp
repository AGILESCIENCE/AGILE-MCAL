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
#include "MCVFilePKP.h"
#include "MCVSubMS.h"
#include <fitsio.h>

ClassImp(MCVFilePKP);

MCVFilePKP::MCVFilePKP()
{

}

//
//--------------------------------------------------------------------
//

MCVFilePKP::MCVFilePKP(const char *name) : MCVFile(name), MCVHisto()
{
	ResizeCountRateHisto(fileInfo.ttot);
	if (fileInfo.gainFactor != 1.) gf=fileInfo.gainFactor;
}

//
//--------------------------------------------------------------------
//

MCVFilePKP::~MCVFilePKP()
{
//	if (tdata) delete tdata;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP::CreateRootFile()
{
	if (fp==NULL) {
		
		rootdatapath.Append(getenv("MCALSW"));
		rootdatapath += stdconf->GetValue(".root.data.path.","./");	
		char rootfilename[100];
		if (fileInfo.pkttype == 2222) {
			sprintf(rootfilename,"RT%s.root", fileInfo.filename);
		} else sprintf(rootfilename,"RT%06d_%4d.root", fileInfo.runid, fileInfo.pkttype);
		rootdatapath += rootfilename;
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

		switch (fileInfo.pkttype) {
			case 3901:
				Process3902Data();
				break;			
			case 3902:
				Process3902Data();
				break;			
			case 3908:
				Process3909Data();
				break;
			case 3909:
				Process3909Data();
				break;
			case 3910:
				Process3909Data();
				break;
			case 3917:
				Process3909Data();
				break;
			case 1111:
				ProcessTrigTrackData();
				break;
			case 2222:
				Process3902Data();
				break;			
			default:
				break;
		}
		fileInfo.nevents = tdata->GetEntries();
		
		fileInfo.nlowDT = nlowDT;
		fileInfo.lowDTti = lowDTti;
		fileInfo.lowDTtf = lowDTtf;
		if (fileInfo.pkttype!=3913) {
		
			tdata->GetEntry(0);
			double ti = currevt.time;
			tdata->GetEntry(fileInfo.nevents-1);
			double tf = currevt.time;
			fileInfo.teff = tf-ti;
		
/*			if (fileInfo.pkttype==3901 || fileInfo.pkttype==1111) {	// to account for 39.01 low data rate
				tdata->GetEntry(0);
				double ti = currevt.time;
				tdata->GetEntry(fileInfo.nevents-1);
				double tf = currevt.time;
				fileInfo.teff = (tf-ti)*1.E-6;
			} else fileInfo.teff = GetEffectiveTime(); */
		}
		fileInfo.notmonotonic = notmonotonic;
		char infoname[20];
		Add3913SciRatemeters();
		sprintf(infoname, "run%06d_%4d_info", fileInfo.runid, fileInfo.pkttype);
		fileInfo.SetName(infoname);
		tdata->Write();		// write the Root tree to file
		fileInfo.Write(infoname);
		fp->Close();
				
		FillCountRateDistribution();
		FillGlobalHisto(fileInfo.pkttype, fileInfo.teff);
		roothistopath.Append(getenv("MCALSW"));
		roothistopath += stdconf->GetValue(".root.histo.path.","./");	
		char	histofilename[100];
		if (fileInfo.pkttype == 2222) {
			sprintf(histofilename,"H%s.root", fileInfo.filename);
		} else sprintf(histofilename,"H%06d_%4d.root", fileInfo.runid, fileInfo.pkttype);
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

void MCVFilePKP::Process3902Data()
{
	Double_t 	t0;//, tprev;
	Short_t 	*pdA;
	Short_t 	*pdB;
	Short_t		cmult;
	Short_t		cbar;
	char		*nzerosup;
	char		*flg;
	char		*barid;
	int		k;
	int		res;
	int		notvalid = 0;
	bool		flag = TRUE;
	
	Int_t nevents = fileInfo.nevents;
	timearray = new Double_t[nevents];
	nzerosup = new char[nevents];
	flg = new char[nevents];
	barid = new char[nevents*30];
	pdA = new Short_t[nevents*30];
	pdB = new Short_t[nevents*30];

	fitsfile *fptr;		// fits file pointer
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	
	cout << "Reading file..." << endl;
	if (fits_read_col(fptr, TDOUBLE, 51, 1, 1, nevents, 0, timearray, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 6, 1, 1, nevents, 0, nzerosup, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 3, 1, 1, nevents, 0, flg, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 45, 1, 1, nevents*30, 0, barid, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 46, 1, 1, nevents*30, 0, pdA, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 47, 1, 1, nevents*30, 0, pdB, NULL, &status)) printerror(status);
	
	t0 = timearray[0];
	//main loop
	
	fileInfo.tstart = t0;			// tini is the time of the first event (sec)
	fileInfo.tstop  = timearray[nevents-1];	// tend is the time of the last event (sec)
	
	
	for (int i=0; i<nevents; i++) {
		if (i%100000==0) cout << "...processed " << i << " events..." << endl;
		if (flg[i]==1) {	// MCAL valid event
			// init new event
			for (int j=0; j<30; j++) currevt.bar[j] = -1;
			currevt.Etot = 0.;
			currevt.totEX = 0;
			currevt.totEZ = 0;
//			tprev = timearray[i];
			currevt.time = timearray[i];	// No more -t0. Save in the events file the corrected TT time!!!
			cmult = nzerosup[i];
			currevt.mult = cmult;
			
			for (int j=0; j<cmult; j++) {
				k = i*30+j;
				cbar = barid[k];
				if (cbar>=0 && cbar<30) {
					if (cbar<15) cbar += 15;
					else cbar -= 15;
					currevt.bar[cbar]=j;
					currevt.pdA[j]=pdA[k];
					currevt.pdB[j]=pdB[k];
					CalculateEandPos(j, cbar, pdA[k], pdB[k]);
				} else {
					// something is wrong
					cout << "*** bar " << cbar << " detected at event " << i << endl;
				}
			
			}
			
			tdata->Fill();
			currevt.time -= t0;	// now subtract t0 for filling basic histogram 
			FillHisto(&currevt);
			if (flag && nlowDT==1) {
				cout << "*** first misalignment at event " << tdata->GetEntries() << endl;
				fileInfo.lowDTevt = tdata->GetEntries();
				flag = FALSE;
			}
		} else notvalid++;

	}
	fits_close_file(fptr, &status);

	cout << "Total number of valid entries: " << tdata->GetEntries() << endl;
	if (notvalid) cout << "*** WARNING: run " << fileInfo.runid << "  " << notvalid << " not valid MCAL events." << endl;
	
	delete timearray;
	delete pdA;
	delete pdB;
	delete flg;
	delete nzerosup;
	delete barid;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP::ProcessTrigTrackData()
{
	Double_t 	t0;//, tprev;
	Short_t 	*pdA;
	Short_t 	*pdB;
	Short_t		cmult;
	Short_t		cbar;
	char		*nzerosup;
	char		*flg;
	char		*barid;
	int		k;
	int		res;
	int		notvalid = 0;
	bool		flag = TRUE;
	
	Int_t nevents = fileInfo.nevents;
	timearray = new Double_t[nevents];
	nzerosup = new char[nevents];
	flg = new char[nevents];
	barid = new char[nevents*30];
	pdA = new Short_t[nevents*30];
	pdB = new Short_t[nevents*30];

	fitsfile *fptr;		// fits file pointer
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
//	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_movabs_hdu(fptr, 3, NULL, &status) )  printerror( status );	
	
	cout << "Reading file..." << endl;
	if (fits_read_col(fptr, TDOUBLE, 51, 1, 1, nevents, 0, timearray, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 6, 1, 1, nevents, 0, nzerosup, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 3, 1, 1, nevents, 0, flg, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 45, 1, 1, nevents*30, 0, barid, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 46, 1, 1, nevents*30, 0, pdA, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 47, 1, 1, nevents*30, 0, pdB, NULL, &status)) printerror(status);
	
	t0 = timearray[0];
	//main loop
	
	for (int i=0; i<nevents; i++) {
		if (i%100000==0) cout << "...processed " << i << " events..." << endl;
		if (flg[i]==1) {	// MCAL valid event
			// init new event
			for (int j=0; j<30; j++) currevt.bar[j] = -1;
			currevt.Etot = 0.;
			currevt.totEX = 0;
			currevt.totEZ = 0;
//			tprev = timearray[i];
			currevt.time = timearray[i]-t0;
			cmult = nzerosup[i];
			currevt.mult = cmult;
			
			for (int j=0; j<cmult; j++) {
				k = i*30+j;
				cbar = barid[k];
				if (cbar>=0 && cbar<30) {
					if (cbar<15) cbar += 15;
					else cbar -= 15;
					currevt.bar[cbar]=j;
					currevt.pdA[j]=pdA[k];
					currevt.pdB[j]=pdB[k];
					CalculateEandPos(j, cbar, pdA[k], pdB[k]);
				} else {
					// something is wrong
					cout << "*** bar " << cbar << " detected at event " << i << endl;
				}
			
			}
			
			tdata->Fill();
			FillHisto(&currevt);
			if (flag && nlowDT==1) {
				cout << "*** first misalignment at event " << tdata->GetEntries() << endl;
				fileInfo.lowDTevt = tdata->GetEntries();
				flag = FALSE;
			}
		} else notvalid++;

	}
	fits_close_file(fptr, &status);

	cout << "Total number of valid entries: " << tdata->GetEntries() << endl;
	if (notvalid) cout << "*** WARNING: run " << fileInfo.runid << "  " << notvalid << " not valid MCAL events." << endl;
	
	delete timearray;
	delete pdA;
	delete pdB;
	delete flg;
	delete nzerosup;
	delete barid;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP::Process3909Data()
{
	Double_t 	t0, tprev;
	Short_t		*pd1A;
	Short_t		*pd1B;
	Short_t		*pd2A;
	Short_t		*pd2B;
	char		*flg;
	char		*bar1;
	char		*bar2;
	Short_t		cmult;
	Short_t		cbar;
	bool		flag = TRUE;
	
	unsigned long 	*pktnumb;
	unsigned long 	*sec;
	unsigned long 	*micsec;
	Short_t		*evtnumb;
	int 		colnum;
	
	Int_t nevents = fileInfo.nevents;
	timearray = new Double_t[nevents];
	pd1A = new Short_t[nevents];
	pd1B = new Short_t[nevents];
	flg = new char[nevents];
	pd2A = new Short_t[nevents];
	pd2B = new Short_t[nevents];
	bar1 = new char[nevents];
	bar2 = new char[nevents];
	
	pktnumb = new unsigned long[nevents];
	evtnumb = new Short_t[nevents];
	sec = new unsigned long[nevents];
	micsec = new unsigned long[nevents];
	
//	MCVSubMS *subms = new MCVSubMS(hSubMS);

	fitsfile *fptr;		// fits file pointer
	if ( fits_open_file(&fptr, fitsname.Data(), READONLY, &status) )  printerror( status );
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );

	cout << "Reading file..." << endl;
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )  {
		cout << "could not find column TIME" << endl;
		return;
	}
	if (fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevents, 0, timearray, NULL, &status)) printerror( status );
	if (fits_read_col(fptr, TSHORT, 5, 1, 1, nevents, 0, pd1A, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 6, 1, 1, nevents, 0, pd1B, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 7, 1, 1, nevents, 0, flg, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 8, 1, 1, nevents, 0, pd2A, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 10, 1, 1, nevents, 0, pd2B, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 11, 1, 1, nevents, 0, bar1, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TBYTE, 13, 1, 1, nevents, 0, bar2, NULL, &status)) printerror(status);
	
	// only for txt file dump if timing error occurs
	if (fits_read_col(fptr, TINT32BIT, 1, 1, 1, nevents, 0, pktnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 2, 1, 1, nevents, 0, evtnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TINT32BIT, 3, 1, 1, nevents, 0, sec, NULL, &status)) printerror(status);
	//if (fits_read_col(fptr, TINT32BIT, 4, 1, 1, nevents, 0, micsec, NULL, &status)) printerror(status);
	
	// MM: 29/05/2012 put time in seconds (rather than microsec), needed for calibration measure analysis with standard flight SW tools
	//for (int k=0; k<nevents; k++) timearray[k] *= 1.E-6;
	
	t0 = timearray[0];
	double toffset = 0.;
	fileInfo.tstart = t0;			// tini is the time of the first event (sec)
	fileInfo.tstop  = timearray[nevents-1];	// tend is the time of the last event (sec)
	bool go = TRUE;
	if (fileInfo.pkttype == 3910) go = FALSE;

	//main loop
	int i=0;
	tprev=0.;
	do {
		if (i%100000==0) {
			cout << "...processed " << i << " events... "; // << endl;
			printf(" Current time (us) = %.1f\n", timearray[i]);
		}
		
		// init new event
		for (int j=0; j<30; j++) currevt.bar[j] = -1;
		currevt.Etot = 0.;
		currevt.totEX = 0;
		currevt.totEZ = 0;
		if (timearray[i] < tprev) {
			if (notmonotonic==0) cout << "*** First time not monotonic at event " << i << endl;
			notmonotonic++;
			if (ferr==NULL) OpenErrorFile();
			//if (evtnumb[i]>1) fprintf(ferr,"%d \t%d \t%d \t%X \t%X \t%c\n\t%d \t%d \t%X \t%X\n\n", i-1, pktnumb[i-1], evtnumb[i-1], sec[i-1], micsec[i-1], sec[i]==sec[i-1] ? 'M' : 'S', pktnumb[i], evtnumb[i], sec[i], micsec[i]);
		}
		tprev = timearray[i];
		currevt.time = timearray[i];	// No more -t0. Save in the events file the corrected TT time!!!
		cmult=0;
		do {
			cbar = bar1[i];
			if (cbar!=15 && cbar!=31) {
				// process first bar
				if (cbar>15) cbar--;
				currevt.bar[cbar]=cmult;
				currevt.pdA[cmult]=pd1A[i];
				currevt.pdB[cmult]=pd1B[i];
				CalculateEandPos(cmult, cbar, pd1A[i], pd1B[i]);
				cmult++;
			} else {
				// something is wrong
				if (go) cout << "*** bar " << cbar << " detected at line " << i << endl;
			}
			if(flg[i]==15) {	
				cbar = bar2[i];
				if (cbar!=15 && cbar!=31) {
					// process second bar
					if (cbar>15) cbar--;
					currevt.bar[cbar]=cmult;
					currevt.pdA[cmult]=pd2A[i];
					currevt.pdB[cmult]=pd2B[i];
					CalculateEandPos(cmult, cbar, pd2A[i], pd2B[i]);
					cmult++;				
				} else {
					// something is wrong
					if (go) cout << "*** bar " << cbar << " detected at line " << i << endl;
				}				
			}
			i++;
		} while (i<nevents && timearray[i]==tprev);
		
		// save current event
		currevt.mult = cmult;
		tdata->Fill();
		currevt.time -= t0;	// now subtract t0 for filling basic histogram 
		FillHisto(&currevt);
		if (flag && nlowDT==1) {
			cout << "*** first misalignment at event " << tdata->GetEntries() << " line " << i-1 << endl;
			fileInfo.lowDTevt = tdata->GetEntries();
			flag = FALSE;
		}
//		subms->ProcessEvent(&currevt);
	} while (i<nevents);
	fits_close_file(fptr, &status);
	if (ferr) fclose(ferr);
	
	cout << "Total number of entries: " << tdata->GetEntries() << endl;
	
//	delete subms;
	
	delete timearray;
	delete pd1A;
	delete pd1B;
	delete flg;
	delete pd2A;
	delete pd2B;
	delete bar1;
	delete bar2;
	
	delete pktnumb;
	delete evtnumb;
	delete sec;
	delete micsec;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFilePKP::PrintEvent(int nevt)
{
	if (tdata==NULL) {
		cout << "Open Root Tree first!" << endl;
		return;
	}
	tdata->Show(nevt);
}

//
//--------------------------------------------------------------------
//

Bool_t MCVFilePKP::Add3913SciRatemeters()
{
    	fitsfile *fptr;
	int nevts, colnum;
	unsigned short *colarray;
	double *tarray;
	double tottime;		// total time in seconds
	int i=-1, k=-1;
	float sum=0;
	float rmX[11], rmZ[11]; 
	TGraph *gX, *gZ, *gAC;
	TString n3913;
	
	n3913 += fileInfo.filename;
	int ps = n3913.Index("_39");
	n3913.Replace(ps, 5, "_3913", 5);
	
	cout << "Opening 39.13 file: " << n3913.Data() << endl;
    	status=0;
	FILE *ftmp=NULL;
	ftmp = fopen(n3913.Data(),"r");
	if (!ftmp) {
		cout << "*** Could not open 39.13 file" << endl;
		return kFALSE;
	} else fclose(ftmp);
	
	n3913 += "[1]";
	if ( fits_open_file(&fptr, n3913.Data(), READONLY, &status) ) printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevts, comment, &status) )  printerror( status );
	for (int i=0; i<22; i++) {
		gX = (TGraph *) rawSciRM3913->At(i);
		gX->Set(nevts*8);
	}
	tarray = new double[nevts];
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )  {
		cout << "could not find column TIME" << endl;
		return 0;
	}
	if (fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevts, 0, tarray, NULL, &status)) printerror( status );
	double tottime2;
	tottime2 = (tarray[nevts-1] - tarray[0])+8.;
	cout << "39.13 total time = " << tottime2 << " s " << endl;
	if (tottime2 <= 0.) cout << "*** WARNING: run " << fileInfo.runid << " 39.13 total time <= 0!" << endl;
	
	colarray = new unsigned short[nevts];
	
	// calculate total data time
	tottime = 0.;
	int n0 = 0;
	if (fits_read_col(fptr, TUSHORT, 164, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);
	for (int j=0; j<nevts; j++) {
		if(colarray[j]>0) tottime += 8.192;
		else n0++;
	}
	cout << "Calculated 39.13 total time = " << tottime << " s " << endl;
	if (n0>0) cout << "*** WARNING: run " << fileInfo.runid << "  " << n0 << "/" << nevts << " packets with 0 events on RM X1!" << endl;
	if (tottime == 0) { // this is necessary otherwise division by zero!
		cout << "Using 39.13 total time." << endl;
		tottime = (tottime2 > 0. ? tottime2 : 1.);
	}
	if (fileInfo.pkttype == 3913) fileInfo.teff = tottime;
	
	// get MCAL scientific ratemeters
	for (i=0; i<11; i++) {
		rmX[i]=0.;
		rmZ[i]=0.;
		gX = (TGraph *) rawSciRM3913->At(i);
		gZ = (TGraph *) rawSciRM3913->At(i+11);
		for (k=0; k<8; k++) {
			// work on RM-Xi
			sum=0;
 			if (fits_read_col(fptr, TUSHORT, 162+22*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) {
//				gX->SetPoint(j*8+k, j*8+k, colarray[j]);
//				gX->SetPoint(j*8+k, (tarray[j]-tarray[0])*1.E-6 +k, colarray[j]);
				gX->SetPoint(j*8+k, tarray[j]+k*1.024, colarray[j]);
/*				if (colarray[j]<1000) {
					sum += colarray[j];
				} else cout << "*** high RM value at line " << j+1 << " , RM " << i+1 << " , second " << k+1 << endl;*/
				sum += colarray[j];	// add anyway the data despite the high value
			}
			rmX[i] += sum;
			// work on RM-Zi
			sum=0;
 			if (fits_read_col(fptr, TUSHORT, 162+22*k+i+11, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) {
//				gZ->SetPoint(j*8+k, j*8+k, colarray[j]);
//				gZ->SetPoint(j*8+k, (tarray[j]-tarray[0])*1.E-6 +k, colarray[j]);
				gZ->SetPoint(j*8+k, tarray[j]+k*1.024, colarray[j]);
/*				if (colarray[j]<1000) {
					sum += colarray[j];
				} else cout << "*** high RM value at line " << j+1 << " , RM " << i+1 << " , second " << k+1 << endl;*/
				sum += colarray[j];	// add anyway the data despite the high value
			}
			rmZ[i] += sum;
		}
		if (i>0) {
			gRMX3913->SetPoint(i, Eband[i], rmX[i]/tottime);
			gRMX3913->SetPointError(i, Eerr[i], sqrt(rmX[i])/tottime);
			gRMZ3913->SetPoint(i, Eband[i], rmZ[i]/tottime);
			gRMZ3913->SetPointError(i, Eerr[i], sqrt(rmZ[i])/tottime);
		}
	}
	
	// get AC scientific ratemeters
	for (i=0; i<5; i++) {
		gAC = (TGraph *) rawACRM3913->At(i);
		for (k=0; k<8; k++) {
 			if (fits_read_col(fptr, TUSHORT, 122+5*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
//			for (int j=0; j<nevts; j++) gAC->SetPoint(j*8+k, (tarray[j]-tarray[0])*1.E-6 +k, colarray[j]);
			for (int j=0; j<nevts; j++) gAC->SetPoint(j*8+k, tarray[j]+k*1.024, colarray[j]);
		}
	}
	
	delete colarray;
	delete tarray;
	fits_close_file(fptr, &status);	
	return kTRUE;
}
