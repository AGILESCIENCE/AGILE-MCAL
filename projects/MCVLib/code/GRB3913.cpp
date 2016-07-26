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

#include "GRB3913.h"

ClassImp(GRB3913);

GRB3913::GRB3913() : GRB()
{
	// NB: this constructor MUST be implemented even is useless, otherwise you will get 
	// /home/martino/mcalsw/lib/libMCV.so: undefined reference to `GRB::GRB[in-charge]()'
}


//
//--------------------------------------------------------------------
//

GRB3913::GRB3913(TString rootfilename, int planeL) : GRB(), data(NULL)
{
	// setting the MCAL plane
	if (planeL == 1) {
		plane = 1;
		barsX = 0;
		barsZ = 0X7FFF;
	} else {
		plane = 0;
		barsZ = 0;
		barsX = 0X7FFF;		
	}
	burstid = plane;
	emin = 0.170;
	emax = 2000.;

	// load root events file
	//cout << "GRB3913::GRB3913() Opening rootfile " << rootfilename.Data() << endl;
	fi= new TFile(rootfilename.Data());
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << rootfilename.Data() << " is not a valid ROOT file: exiting!" << endl;
		return;
	} else {
	
		// get file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
		}
		if (info) info->PrintFileInfo();
	}
	
	TGraph *gaux = (TGraph *) fi->Get("grmX1");
	ns = gaux->GetN();
	time = gaux->GetX();
	data = new double[22*ns];
	double *dataAC = new double[5*ns];
	
	// account for 8.192 s offset in time array
	for (int i=0; i<ns; i++) time[i] -= 8.192;
	
	// loads data from a root file created with mcalanalyzer

	// retrieve MCAL sci. RM data
	char gname[10];
	for (int i=0; i<22; i++) {
		if (i<11) sprintf(gname, "grmX%d", i+1);
		else sprintf(gname, "grmZ%d", i-10);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
		}
	}

	// retrieve AC sci. RM data
	for (int i=0; i<5; i++) {
		sprintf(gname, "gAC%d", i);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) dataAC[j*5 + i] = y[j];
		}
	}
	nbins = (int) ((time[ns-1] - time[0])/1.024) + 1;
	
	t0=0.;
	tbin = 1.024;
	
	// default spectral boundaries (Sci. RM like)
	double stddataEbin[] = {0., 0.17, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 44.8, 89.6, 1433.6};
	dataEbin = new double[12];	
	for (int i=0; i<12; i++) dataEbin[i] = stddataEbin[i];
		
	nEbins = 4;
	double stdEbin[] = {0.35, 0.7, 1.4, 2.8, 1433.6};
	mcalEbin = new double[nEbins+1];
	for (int i=0; i<nEbins+1; i++) mcalEbin[i] = stdEbin[i];
//	bkg = new double[nEbins];
//	for (int i=0; i<nEbins; i++) bkg[i] = grb[i] = tot[i] = 0.;
//	cbkg = 0.;
	
}

//
//--------------------------------------------------------------------
//

GRB3913::~GRB3913()
{
	fi->Close();
}

//
//--------------------------------------------------------------------
//

int GRB3913::LightCurve(double t0L, double tpre, double tpost)
{
	//cout << endl << "GRB3913::LightCurve() building light curves for plane " << (plane == 0 ? 'X' : 'Z') << endl;
	
	// set the time boundaries and create histogram
	if (t0L > 0.) {
		tstart = tpre;
		tstop = tpost;
		// find time bin closest to t0L
		double dt = fabs(t0L);
		for (int i=0; i<ns; i++) {
			if (fabs(time[i] - t0L) <= dt) {
				t0=time[i];
				dt = fabs(t0 - t0L);
			}
		}
	}
	t0 -= 0.512;	// to account for TM packet generation and Sci RM quantization
	int nbins1 = (int) (-tstart/tbin)+1;
	tstart = -nbins1*tbin;
	int nbins2 = (int) (tstop/tbin)+1;
	tstop = nbins2*tbin;
	nbins = nbins1 + nbins2;
	TString hname("hc_");
	hname += (plane == 0 ? 'X' : 'Z');
	hclc = new TH1D(hname, "Custom light curve", nbins, tstart, tstop);
	TString hname2D("h2D_");
	hname2D += (plane == 0 ? 'X' : 'Z');
	h2clc = new TH2D(hname2D, "Energy binned light curve", nbins, tstart, tstop, nEbins, mcalEbin);
	tzero = new TTimeStamp();
	tzero->Set((int) t0 , 0, epoch->GetSec(),0);
	hLE = new TH1F("hLE", "low energy histogram", nbins, tstart, tstop);	// for HR calculations
	hHE = new TH1F("hLE", "low energy histogram", nbins, tstart, tstop);
	ghr = new TGraphErrors();
	//printf("Time0 TT %18.6f    UT %s\n", t0, tzero->AsString("s"));
	
	// get time0 information
	char caux[10];
	sprintf(caux, "%d", tzero->GetDate(1));
	sscanf(caux, "%4d%2d%2d", &year, &month, &day);
	sprintf(caux, "%06d", tzero->GetTime(1));
	sscanf(caux, "%2d%2d%2d", &hour, &min, &sec);
	
	// process data: fill custom light curve
	double tl, ntot;
	for (int j=0; j<ns; j++) {
		tl = time[j]-t0;
		if (tl>= tstart && tl<=tstop) {
			ntot = 0.;
			for (int i=1; i<11; i++) {
				ntot += data[j*22 + i + plane*11];
				h2clc->Fill(tl, dataEbin[i], data[j*22 + i + plane*11]);
			}
			hclc->Fill(tl, ntot);
		}
	}
	return 0;
}


