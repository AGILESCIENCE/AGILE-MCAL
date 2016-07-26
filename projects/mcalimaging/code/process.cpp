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

#include "mcalimaging.h"
#include "mcalguess.h"
#include <fitsio.h>
//extern float rbars[NBINS][NBARSBINX][NBARSBINZ];
//extern float renergy[NBINS][NEBINX][NEBINZ];
extern double Estep[];
extern float mcalEbin[];

void processEvents(int ifile, selection *sel, TTree *indata, TObjArray *histo)
{
	float *ETagged=NULL, *EK=NULL, eX, eZ;
	MCALevt inevt;		// Input event
	TH2F *hTracksCorr, *hEXEZ, *hEREtot, *hEtotEtag, *hEratioEtag, *hEdev;
	TH1F *hNBarsX, *hNBarsZ, *hNTracksX, *hNTracksZ, *hERatio, *hERatioSel, *hEX, *hEZ;
	TH1F *hETagged;
	TH2F *hEtotEtag_sel, *hEratioEtag_sel;
	TGraph *gEMEK;
	TH2F *hTracks1, *hTracks2, *hTracks3, *hTracks4;
	TH2F *hBars1, *hBars2, *hBars3, *hBars4;
	int nbars, nbarsX, nbarsZ, id, nevents, ntrX, ntrZ, nsel=0;
	
	fitsfile *fptr, *kptr;		// fits file pointer
	int status=0;
	unsigned long *pktnumb;
	Short_t *evtnumb;
	track **trackarrX;
	track **trackarrZ;
	
	nevents = indata->GetEntries();
	trackarrX = new track *[nevents];
	trackarrZ = new track *[nevents];
	cout << "Processing " << nevents << " events" << endl;
	
	// connect input tree
	indata->SetBranchAddress("time", &inevt.time);
	indata->SetBranchAddress("mult", &inevt.mult);
	indata->SetBranchAddress("bar", inevt.bar);
	indata->SetBranchAddress("pdA", inevt.pdA);
	indata->SetBranchAddress("pdB", inevt.pdB);
	indata->SetBranchAddress("pos", inevt.pos);
	indata->SetBranchAddress("E", inevt.E);
	indata->SetBranchAddress("Etot", &inevt.Etot);
	
	hNBarsX = (TH1F *) histo->FindObject("hNBarsX");
	hNBarsZ = (TH1F *) histo->FindObject("hNBarsZ");
	hNTracksX = (TH1F *) histo->FindObject("hNTracksX");
	hNTracksZ = (TH1F *) histo->FindObject("hNTracksZ");
	hEX = (TH1F *) histo->FindObject("hEX");
	hEZ = (TH1F *) histo->FindObject("hEZ");
	hTracksCorr = (TH2F *) histo->FindObject("hTracksCorr");
	hERatio = (TH1F *) histo->FindObject("hERatio");
	hERatioSel = (TH1F *) histo->FindObject("hERatioSel");
	hEXEZ = (TH2F *) histo->FindObject("hEXEZ");
	hEREtot = (TH2F *) histo->FindObject("hEREtot");
	
	if (sel->isTrigTrack) {
		hETagged = (TH1F *) histo->FindObject("hETagged");
		hEtotEtag = (TH2F *) histo->FindObject("hEtotEtag");
		hEratioEtag = (TH2F *) histo->FindObject("hEratioEtag");
		
		hTracks1 = (TH2F *) histo->FindObject("hTracks1");
		hTracks2 = (TH2F *) histo->FindObject("hTracks2");
		hTracks3 = (TH2F *) histo->FindObject("hTracks3");
		hTracks4 = (TH2F *) histo->FindObject("hTracks4");
		
		hBars1 = (TH2F *) histo->FindObject("hBars1");
		hBars2 = (TH2F *) histo->FindObject("hBars2");
		hBars3 = (TH2F *) histo->FindObject("hBars3");
		hBars4 = (TH2F *) histo->FindObject("hBars4");
		
		// opens lv1 fits file and get tagged photon energy
		ETagged = new float[nevents];
		cout << "Reading fits file " << (sel->lv1file[ifile]).Data() << endl;
		if ( fits_open_file(&fptr, (sel->lv1file[ifile]).Data(), READONLY, &status) )  printerror( status );
		if ( fits_movabs_hdu(fptr, 3, 0, &status) )  printerror( status );
		if (fits_read_col(fptr, TFLOAT, 10, 1, 1, nevents, 0, ETagged, NULL, &status)) printerror(status);
		fits_close_file(fptr, &status);
	}
	
	if (sel->doKalmanCorr) {
		hEdev = (TH2F *) histo->FindObject("hEdev");
		hEtotEtag_sel = (TH2F *) histo->FindObject("hEtotEtag_sel");
		hEratioEtag_sel = (TH2F *) histo->FindObject("hEratioEtag_sel");
		gEMEK = (TGraph *) histo->FindObject("gEMEK");
	
		// opens Kalman fits file and get reconstructed photon energy
		EK = new float[nevents];
		cout << "Reading fits file " << (sel->kalmanfile[ifile]).Data() << endl;
		if ( fits_open_file(&kptr, (sel->kalmanfile[ifile]), READONLY, &status) )  printerror( status );
		if ( fits_movabs_hdu(kptr, 2, 0, &status) )  printerror( status );
		if (fits_read_col(kptr, TFLOAT, 29, 1, 1, nevents, 0, EK, NULL, &status)) printerror(status);
		fits_close_file(kptr, &status);
	}
	
	// main loop
	for (int i=0; i<nevents; i++) {
		indata->GetEntry(i);
/*		if (i < 100) {
			for (int j=0; j<30; j++) cout << inevt.bar[j] << " ";
			cout << endl;
		
		}*/
		nbars=0;
		nbarsX=0;
		nbarsZ=0;
		ntrX=0;		// numero di tracce indipendenti sul piano X
		ntrZ=0;		// numero di tracce indipendenti sul piano Z
		eX = eZ = 0.;
		int prbarX=0, prbarZ=15;
		for (int k=0; k<30; k++) {
			
			id = inevt.bar[k];
			
			if (id>=0 && inevt.E[id] >0.) k<15 ? eX += inevt.E[id] : eZ += inevt.E[id];
			
			if (id>=0 && inevt.E[id] >= sel->minE) {
				nbars++;
				if (k<15) {
					nbarsX++;
					if (ntrX==0) {
						ntrX++;
						prbarX=k;
						trackarrX[i] = new track(&inevt, k, sel->minE);
						trackarrX[i]->evt = i;
						trackarrX[i]->trid = 1;
						trackarrX[i]->print();
					} else {
						if (prbarX<k-1) {
							ntrX++;
							track *auxtr = new track(&inevt, k, sel->minE);
							auxtr->prev = trackarrX[i];
							trackarrX[i] = auxtr;
							trackarrX[i]->evt = i;
							trackarrX[i]->trid = ntrX;
							trackarrX[i]->print();
						}
						prbarX=k;
					}  
				} else {
					nbarsZ++;
					if (ntrZ==0) {
						ntrZ++;
						prbarZ=k;
						trackarrZ[i] = new track(&inevt, k, sel->minE);
						trackarrZ[i]->evt = i;
						trackarrZ[i]->trid = 1;
						trackarrZ[i]->print();
					} else {
						if (prbarZ<k-1) {
							ntrZ++;
							track *auxtr = new track(&inevt, k, sel->minE);
							auxtr->prev = trackarrZ[i];
							trackarrZ[i] = auxtr;
							trackarrZ[i]->evt = i;
							trackarrZ[i]->trid = ntrX;
							trackarrZ[i]->print();
						}
						prbarZ=k;
					}
				}
			}
			//if (i==657) printf("evt= %d\t k=%d\t nbars= %d\t nbX= %d\t ntX= %d\t pbX= %d\t nbZ= %d\t ntZ= %d\t pbZ= %d\n", i, k, nbars, nbarsX, ntrX, prbarX, nbarsZ, ntrZ, prbarZ);
		}
		hNBarsX->Fill(nbarsX);
		hNBarsZ->Fill(nbarsZ);
		hNTracksX->Fill(ntrX);
		hNTracksZ->Fill(ntrZ);
		hTracksCorr->Fill(ntrX, ntrZ);
		if (ntrX>=sel->minTrX && ntrX<=sel->maxTrX && ntrZ>=sel->minTrZ && ntrZ<=sel->maxTrZ && nbarsX>=sel->minBarX && nbarsX<=sel->maxBarX && nbarsZ>=sel->minBarZ && nbarsZ<=sel->maxBarZ) {	//ntrX>0 && ntrZ>0 && eX>0. && eZ>0.
			nsel++;
			hEX->Fill(eX);
			hEZ->Fill(eZ);
			if (nsel<=100) printf("%d: evt %d - EX= %6.2f\t - EZ= %6.2f\t - EZ/EX= %6.2f\t - Etot= %6.2f\n", nsel, i, eX, eZ, eX>0. ? eZ/eX : -111., eX+eZ);
			hEXEZ->Fill(eX, eZ);
			if (eX>0) {
				hERatio->Fill(eZ/eX);
				hERatioSel->Fill(eZ/eX);
				hEREtot->Fill(eZ+eX, eZ/eX);
			}
		}
		
/*		if (sel->isTrigTrack) {
			if (ETagged[i]<50.) hTracks1->Fill(ntrX, ntrZ);
			else if (ETagged[i]<100.) hTracks2->Fill(ntrX, ntrZ);
				else if (ETagged[i]<200.) hTracks3->Fill(ntrX, ntrZ);
					else hTracks4->Fill(ntrX, ntrZ);
		}
*/		
		if (sel->isTrigTrack && ntrX>=sel->minTrX && ntrX<=sel->maxTrX) {
			hETagged->Fill(ETagged[i]);
			hEratioEtag->Fill(ETagged[i], eX/eZ);
			hEtotEtag->Fill(ETagged[i], eX+eZ);
			
			
			if (sel->doKalmanCorr) {
				if (EK[i] > 0.) hEdev->Fill(ETagged[i], EK[i]/ETagged[i] - 1.);
				if (ETagged[i]> 200 && ETagged[i]< 500) {
					cout << i << "  " << ETagged[i] << "  " << EK[i] << "  " << eX+eZ << endl;
					gEMEK->SetPoint(i, EK[i], eX+eZ);
				}
			}
		}
		
		if (sel->isTrigTrack && sel->doKalmanCorr && fabs(EK[i]/ETagged[i] - 1.)<=0.5) {
			hEtotEtag_sel->Fill(ETagged[i], eX+eZ);
			hEratioEtag_sel->Fill(ETagged[i], eX/eZ);
			
			if (ETagged[i]<50.) hTracks1->Fill(ntrX, ntrZ);
			else if (ETagged[i]<100.) hTracks2->Fill(ntrX, ntrZ);
				else if (ETagged[i]<200.) hTracks3->Fill(ntrX, ntrZ);
					else hTracks4->Fill(ntrX, ntrZ);
					
			if (ntrX==1 && ntrZ==1) {
				if (ETagged[i]<50.) hBars1->Fill(nbarsX, nbarsZ);
				else if (ETagged[i]<100.) hBars2->Fill(nbarsX, nbarsZ);
					else if (ETagged[i]<200.) hBars3->Fill(nbarsX, nbarsZ);
						else hBars4->Fill(nbarsX, nbarsZ);			
			}

		}
	}
	if(ETagged) delete ETagged;
	if(EK) delete EK;

	return;
}

//
//--------------------------------------------------------------------
//

void processEvents2(int ifile, selection *sel, TTree *indata, TObjArray *histo, int nEtag, int runid)
{
	float *ETagged=NULL, *EK1=NULL, *EK2=NULL, *EK=NULL, eX, eZ;
	MCALevt inevt;		// Input event
	TH2F *hTracksCorr, *hBarCorr, *hEXEZ, *hEREtot, *hEtotEtag, *hEratioEtag, *hEdev, *hEK, *hEKcorr;
	TH1F *hNBarsX, *hNBarsZ, *hNTracksX, *hNTracksZ, *hERatio, *hERatioSel, *hEX, *hEZ, *hEtot;
	TH1F *hETagged, *hZDevP1X, *hXDevP2X, *hZDevP2X, *hEMcorr;
	TH2F *hEtotEtag_sel, *hEratioEtag_sel, *hPosDevP1X, *hPosDevP2X;
	TH2F *hXDevP1X;
	TH2F *hXDevP1X_bar;
	TGraph *gEMEK;
	TH2F *hTracks1, *hTracks2, *hTracks3, *hTracks4;
	TH2F *hBars1, *hBars2, *hBars3, *hBars4;
	TH2F *hEMEK, *hEMEtag, *hEMavgEtag;
	int nbars, nbarsX, nbarsZ, id, nevents, ntrX, ntrZ, nsel=0;
	
//	double Estep2[] = {1., 50., 100., 200., 400., 1000., 3000.};
	
	fitsfile *fptr, *kptr;		// fits file pointer
	int status=0;
	unsigned long *pktnumb;
	Short_t *evtnumb;
	long *nevt1x, *offset1x, *nevt2x, *offset2x, *nevt1y, *offset1y, *nevt2y, *offset2y;
	float *p1x, *p2x, *p1y, *p2y;
	Short_t *primopX, *primopY;
	float x1, y1, x2, y2, ykp1X, xkp1X, ykp2X, xkp2X, emX, emZ;
	int iplaneX, iplaneY, ipx1, ipy1, ipx2, ipy2;
	
	int n1=0, nEK=0, nEMcorr=0;
	int nP1eqP2 = 0;
	int nP1noteqP2 = 0;
	
	TFile *fp=NULL;
	mcalguess *mresp=NULL;
	
	TGraph *gEMEK_sel[10];
	
	
	// load mcal guess response file
	if (sel->buildResponseMatrix || sel->doMcalGuess) {
		fp = new TFile((sel->parname)->Data(), "update");
		mresp = (mcalguess *) fp->Get("mcal_response");
		if (mresp) {
			cout << "Loaded mcal_response.  N events/bin: ";
			for (int i=0; i<NBINS; i++) cout << mresp->GetNEvents(i) << "  ";
		} else {
			cout << "*** Cannot find MCAL response data mcal_response in root file " << (sel->parname)->Data() << endl;
			cout << "*** Creating a new response matrix" << endl; 
			mresp = new mcalguess();
			mresp->SetEnergyArray(Estep, mcalEbin, mcalEbin);
		}
	}
	int ibin = ifile;
	
	nevents = indata->GetEntries();
	cout << "Processing " << nevents << " events" << endl;
	pktnumb = new unsigned long[nevents];
	evtnumb = new Short_t[nevents];
	
	// connect input tree
	indata->SetBranchAddress("time", &inevt.time);
	indata->SetBranchAddress("mult", &inevt.mult);
	indata->SetBranchAddress("bar", inevt.bar);
	indata->SetBranchAddress("pdA", inevt.pdA);
	indata->SetBranchAddress("pdB", inevt.pdB);
	indata->SetBranchAddress("pos", inevt.pos);
	indata->SetBranchAddress("E", inevt.E);
	indata->SetBranchAddress("Etot", &inevt.Etot);
	
	hNBarsX = (TH1F *) histo->FindObject("hNBarsX");
	hNBarsZ = (TH1F *) histo->FindObject("hNBarsZ");
	hNTracksX = (TH1F *) histo->FindObject("hNTracksX");
	hNTracksZ = (TH1F *) histo->FindObject("hNTracksZ");
	hEX = (TH1F *) histo->FindObject("hEX");
	hEZ = (TH1F *) histo->FindObject("hEZ");
	hEtot = (TH1F *) histo->FindObject("hEtot");
	hTracksCorr = (TH2F *) histo->FindObject("hTracksCorr");
	hBarCorr = (TH2F *) histo->FindObject("hBarCorr");
	hERatio = (TH1F *) histo->FindObject("hERatio");
	hERatioSel = (TH1F *) histo->FindObject("hERatioSel");
	hEXEZ = (TH2F *) histo->FindObject("hEXEZ");
	hEREtot = (TH2F *) histo->FindObject("hEREtot");
	
	hETagged = (TH1F *) histo->FindObject("hETagged");
	hEtotEtag = (TH2F *) histo->FindObject("hEtotEtag");
	hEratioEtag = (TH2F *) histo->FindObject("hEratioEtag");
	
	hTracks1 = (TH2F *) histo->FindObject("hTracks1");
	hTracks2 = (TH2F *) histo->FindObject("hTracks2");
	hTracks3 = (TH2F *) histo->FindObject("hTracks3");
	hTracks4 = (TH2F *) histo->FindObject("hTracks4");
	
	hBars1 = (TH2F *) histo->FindObject("hBars1");
	hBars2 = (TH2F *) histo->FindObject("hBars2");
	hBars3 = (TH2F *) histo->FindObject("hBars3");
	hBars4 = (TH2F *) histo->FindObject("hBars4");
	
	hEMEtag = (TH2F *) histo->FindObject("hEMEtag");
	hEMavgEtag = (TH2F *) histo->FindObject("hEMavgEtag");
	
	// opens lv1 fits file and get tagged photon energy
	ETagged = new float[nevents];
	cout << "Reading fits file " << (sel->lv1file)[ifile].Data() << endl;
	if ( fits_open_file(&fptr, (sel->lv1file)[ifile].Data(), READONLY, &status) )  printerror( status );
	if ( fits_movabs_hdu(fptr, 3, 0, &status) )  printerror( status );
	if (sel->isTrigTrack) {
		if (fits_read_col(fptr, TFLOAT, 10, 1, 1, nevents, 0, ETagged, NULL, &status)) printerror(status);
	} else for (int ii=0; ii<nevents; ii++) ETagged[ii] = -1.;
	if (fits_read_col(fptr, TINT32BIT, 1, 1, 1, nevents, 0, pktnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 2, 1, 1, nevents, 0, evtnumb, NULL, &status)) printerror(status);
	fits_close_file(fptr, &status);
	
	if (sel->doKalmanCorr) {
		hEdev = (TH2F *) histo->FindObject("hEdev");
		hEK = (TH2F *) histo->FindObject("hEK");
		hEtotEtag_sel = (TH2F *) histo->FindObject("hEtotEtag_sel");
		hEratioEtag_sel = (TH2F *) histo->FindObject("hEratioEtag_sel");
		gEMEK = (TGraph *) histo->FindObject("gEMEK");
		hPosDevP1X = (TH2F *) histo->FindObject("hPosDevP1X");
		hXDevP1X = (TH2F *) histo->FindObject("hXDevP1X");
		hZDevP1X = (TH1F *) histo->FindObject("hZDevP1X");
		hPosDevP2X = (TH2F *) histo->FindObject("hPosDevP2X");
		hXDevP2X = (TH1F *) histo->FindObject("hXDevP2X");
		hZDevP2X = (TH1F *) histo->FindObject("hZDevP2X");
		hXDevP1X_bar = (TH2F *) histo->FindObject("hXDevP1X_bar");
		if (!hPosDevP1X) cout << "Cannot find hPosDevP1X" << endl;
		if (!hXDevP1X) cout << "Cannot find hXDevP1X" << endl;
		if (!hZDevP1X) cout << "Cannot find hZDevP1X" << endl;
		if (!hXDevP1X_bar) cout << "Cannot find hXDevP1X_bar" << endl;
		hEMEK = (TH2F *) histo->FindObject("hEMEK");
		
		for (int i=0; i<10; i++) gEMEK_sel[i] = NULL;
		for (int i=0; i<nEtag; i++) {
			TString gname("gEMEK_");
			gname += i;
			if (i<10) gEMEK_sel[i] = (TGraph *) histo->FindObject(gname.Data());
		}
		hEMcorr = (TH1F *) histo->FindObject("hEMcorr");
		hEKcorr = (TH2F *) histo->FindObject("hEKcorr");

		// opens Kalman fits file and get reconstructed photon energy
		EK1 = new float[nevents];
		EK2 = new float[nevents];
		EK = new float[nevents];
		primopX = new Short_t[nevents];
		primopY = new Short_t[nevents];
		cout << "Reading fits file " << (sel->kalmanfile)[ifile].Data() << endl;
		if ( fits_open_file(&kptr, (sel->kalmanfile)[ifile].Data(), READONLY, &status) )  printerror( status );
		if ( fits_movabs_hdu(kptr, 2, 0, &status) )  printerror( status );
		if (fits_read_col(kptr, TSHORT, 25, 1, 1, nevents, 0, primopX, NULL, &status)) printerror(status);
		if (fits_read_col(kptr, TSHORT, 26, 1, 1, nevents, 0, primopY, NULL, &status)) printerror(status);
		if (fits_read_col(kptr, TFLOAT, 27, 1, 1, nevents, 0, EK1, NULL, &status)) printerror(status);
		if (fits_read_col(kptr, TFLOAT, 28, 1, 1, nevents, 0, EK2, NULL, &status)) printerror(status);
		if (fits_read_col(kptr, TFLOAT, 29, 1, 1, nevents, 0, EK, NULL, &status)) printerror(status);
		
		// read data from variable length arrays
		nevt1x = new long[nevents];
		offset1x = new long[nevents];
		nevt2x = new long[nevents];
		offset2x = new long[nevents];
		nevt1y = new long[nevents];
		offset1y = new long[nevents];
		nevt2y = new long[nevents];
		offset2y = new long[nevents];
		p1x = new float[nevents*NMAX];
		p2x = new float[nevents*NMAX];
		p1y = new float[nevents*NMAX];
		p2y = new float[nevents*NMAX];
		if ( fits_read_descripts(kptr, 1, 1, nevents, nevt1x, offset1x, &status)) printerror(status);
		if ( fits_read_descripts(kptr, 2, 1, nevents, nevt2x, offset2x, &status)) printerror(status);
		if ( fits_read_descripts(kptr, 3, 1, nevents, nevt1y, offset1y, &status)) printerror(status);
		if ( fits_read_descripts(kptr, 4, 1, nevents, nevt2y, offset2y, &status)) printerror(status);
		for (long i=0; i<nevents; i++) {
			if ( fits_read_col(kptr, TFLOAT, 1, i+1, 1, nevt1x[i], 0, &p1x[i*NMAX], NULL, &status)) printerror(status);
			if ( fits_read_col(kptr, TFLOAT, 2, i+1, 1, nevt2x[i], 0, &p2x[i*NMAX], NULL, &status)) printerror(status);
			if ( fits_read_col(kptr, TFLOAT, 3, i+1, 1, nevt1y[i], 0, &p1y[i*NMAX], NULL, &status)) printerror(status);
			if ( fits_read_col(kptr, TFLOAT, 4, i+1, 1, nevt2y[i], 0, &p2y[i*NMAX], NULL, &status)) printerror(status);
		}
		fits_close_file(kptr, &status);
	}
	
	// main loop
	for (int i=0; i<nevents; i++) {
		indata->GetEntry(i);
/*		if (i < 100) {
			for (int j=0; j<30; j++) cout << inevt.bar[j] << " ";
			cout << endl;
		
		}*/
		nbars=0;
		nbarsX=0;
		nbarsZ=0;
		ntrX=0;		// numero di tracce indipendenti sul piano X
		ntrZ=0;		// numero di tracce indipendenti sul piano Z
		eX = eZ = 0.;
		int prbarX=0, prbarZ=15;
		
		point *Xpoints[15];
		point *Zpoints[15];
		for (int ii=0; ii<15; ii++) Xpoints[ii] = Zpoints[ii] = NULL;
		
		for (int k=0; k<30; k++) {
			
			id = inevt.bar[k];
			
			if (id>=0 && inevt.E[id] >0.) k<15 ? eX += inevt.E[id] : eZ += inevt.E[id];
			
			if (id>=0 && inevt.E[id] >= sel->minE) {
				nbars++;
				if (k<15) {
					Xpoints[nbarsX] = new point(&inevt, k);
					nbarsX++;
					if (ntrX==0) {
						ntrX++;
						prbarX=k;
					} else {
						if (prbarX<k-1) {
							ntrX++;
						}
						prbarX=k;
					}  
				} else {
					Zpoints[nbarsZ] = new point(&inevt, k);
					nbarsZ++;
					if (ntrZ==0) {
						ntrZ++;
						prbarZ=k;
					} else {
						if (prbarZ<k-1) {
							ntrZ++;
						}
						prbarZ=k;
					}
				}
			}
		}
		
		// get point distance matrix for points in X and Z plane
		float pdist[15][15];
		for (int k=0; k<nbarsX; k++) {
			for (int j=0; j<nbarsZ; j++) {
				pdist[k][j] = getPointDistance(Xpoints[k], Zpoints[j]);
			}
		}
		
		// calculate best points (figure of merit of points)
		float pmerit[225];
		int bestx, bestz;
		float bestfom = pointFigureOfMerit1(nbarsX, Xpoints, nbarsZ, Zpoints, pmerit, &bestx, &bestz);
		
		// calculate center of energy for plane X, plane Z and total MCAL
		point *cenX = getEnergyCenterPlane(nbarsX, Xpoints);
		point *cenZ = getEnergyCenterPlane(nbarsZ, Zpoints);
		emX = getEnergyMoment(cenX, nbarsX, Xpoints);
		emZ = getEnergyMoment(cenZ, nbarsZ, Zpoints);
		
		ipx1 = nevt1x[i]/2;
		ipy1 = nevt1y[i]/2;
		//if (primopX[i]>=6 && (ipx1 == 13-primopX[i])) {
		if (ipx1 >= 13-primopX[i] -1 && EK[i]>0. && nbarsX>0 ) {
		
			// project Kalman trajectories on MCAL
			
			// P1 on plane X
			iplaneX = primopX[i] + ipx1-1;
			xkp1X = p1x[i*NMAX+ipx1-1] + p1x[i*NMAX+2*ipx1-1]*(Y_ST_P1 - (iplaneX-1)*ST_DY - Y_PLANEX);
			iplaneY = primopY[i] + ipy1;
			ykp1X = p1y[i*NMAX+ipy1-1] + p1y[i*NMAX+2*ipy1-1]*(Y_ST_P1 - (iplaneY-1)*ST_DY - Y_PLANEX);
			
			// P2 on plane X
			ipx2 = nevt2x[i]/2;
			ipy2 = nevt2y[i]/2;
			iplaneX = primopX[i] + ipx2-1;
			xkp2X = p2x[i*NMAX+ipx2-1] + p2x[i*NMAX+2*ipx2-1]*(Y_ST_P1 - (iplaneX-1)*ST_DY - Y_PLANEX);
			iplaneY = primopY[i] + ipy2;
			ykp2X = p2y[i*NMAX+ipy2-1] + p2y[i*NMAX+2*ipy2-1]*(Y_ST_P1 - (iplaneY-1)*ST_DY - Y_PLANEX);
			
			point *p1X, *p2X;
			
			// EITHER P1 is the point with higher Kalman energy
			/*if (EK1[i] > EK2[i]) {
				p1X = new point(0, xkp1X, ykp1X, EK1[i]); 
				p2X = new point(0, xkp2X, ykp2X, EK2[i]); 
			} else {
				p2X = new point(0, xkp1X, ykp1X, EK1[i]); 
				p1X = new point(0, xkp2X, ykp2X, EK2[i]); 
			}*/
			
			// OR P1 is the standard P1 point  in the Kalman file
			p1X = new point(0, xkp1X, ykp1X, EK1[i]); 
			p2X = new point(0, xkp2X, ykp2X, EK2[i]);
			
			// update distance histograms for P1
			int iminP1 = getClosestPoint(nbarsX, Xpoints, p1X);
			//int iminP1 = getClosestPointX(nbarsX, Xpoints, p1X);
			
			float dxP1 = 1000., dzP1 = 1000.;
			if (iminP1 >= 0) dxP1 = p1X->X - Xpoints[iminP1]->X;
			if (iminP1 >= 0) dzP1 = p1X->Z - Xpoints[iminP1]->Z;
			//float dxP1 = p1X->X - Xpoints[bestx]->X;
			//float dzP1 = p1X->Z - Xpoints[bestx]->Z;
			
			if (fabs(EK[i]/ETagged[i] - 1.)>0.5) {		// tmp
			hPosDevP1X->Fill(dxP1, dzP1);
			hXDevP1X->Fill(ETagged[i], dxP1);
			//hXDevP1X->Fill(EK[i], dxP1);
			if (iminP1 >= 0) hXDevP1X_bar->Fill(Xpoints[iminP1]->bar, dxP1);
			hZDevP1X->Fill(dzP1);
			}
			
			// update distance histograms for P2
			int iminP2 = getClosestPoint(nbarsX, Xpoints, p2X);
			//int iminP2 = getClosestPointX(nbarsX, Xpoints, p2X);
			
			float dxP2 = 1000., dzP2 = 1000.;
			if (iminP2 >= 0) dxP2 = p2X->X - Xpoints[iminP2]->X;
			if (iminP2 >= 0) dzP2 = p2X->Z - Xpoints[iminP2]->Z;
			if (fabs(EK[i]/ETagged[i] - 1.)>0.5) {		// tmp
			hPosDevP2X->Fill(dxP2, dzP2);
			hXDevP2X->Fill(ETagged[i], dxP2);
			hZDevP2X->Fill(dzP2);
			}
			
			// verify if P1 and P2 differ
			if (iminP1 != 0 && iminP1 == iminP2) nP1eqP2++;
			else nP1noteqP2++;
			
			hETagged->Fill(ETagged[i]);
			
			float auxE = 0.;
			if (sel->doMcalGuess) {
				int mguess = mresp->GetBestBin(nbarsX, nbarsZ, eX, eZ);
				auxE = 0.5*(Estep[mguess] + Estep[mguess+1]);  
				hEMEK->Fill(EK[i], auxE);
			}
			
			// print event info in tabular form (standard analysis validation)
			if (0) {
				printf("%6d \t%6d \t%6d \t%6d \t%6d \t", runid, pktnumb[i], evtnumb[i], nbarsX, nbarsZ);
				for (int k=0; k<nbarsX; k++) printf("%6d \t%5.2f \t%5.2f \t%5.2f ", Xpoints[k]->bar, Xpoints[k]->X, Xpoints[k]->Z, Xpoints[k]->E);
				for (int k=0; k<nbarsZ; k++) printf("%6d \t%5.2f \t%5.2f \t%5.2f ", Zpoints[k]->bar, Zpoints[k]->X, Zpoints[k]->Z, Zpoints[k]->E);
				cout << endl;
			}
			
			// print event info for selected events
			//if (1) {
			if (EK[i] > 400.) {
				// print point distance info
				printf("\nRun %d - id %d - pkt %d - evt %d - E_PTS = %6.2f - EK = %6.2f - nbarsX = %d - nbarsZ = %d\n\t\t", runid, i, pktnumb[i], evtnumb[i], ETagged[i], EK[i], nbarsX, nbarsZ);
				//printf("primopX %d - primopY %d - n. planes %d\n\t\t", primopX[i], primopY[i], nevt1x[i]/2);
				printf("\n\tPoint distance info:\n\t\t");
				for (int j=0; j<nbarsZ; j++) printf("\t   Z%d", Zpoints[j]->bar - 14);
				printf("\n\t\t");
				for (int j=0; j<nbarsZ; j++) printf("\t%5.1f", Zpoints[j]->E);
				printf("\n-----------------------------------------------\n");
				for (int k=0; k<nbarsX; k++) {
					printf("\t   X%d\t%5.1f |", Xpoints[k]->bar + 1, Xpoints[k]->E);
					for (int j=0; j<nbarsZ; j++) printf("\t%6.2f", pdist[k][j]);
					printf("\n");
				}
				
				// print extended point info
				cout << endl;
				for (int k=0; k<nbarsX; k++) {
					printf("\t   X%d : ", Xpoints[k]->bar + 1);
					Xpoints[k]->show();
				}
				cout << endl;
				for (int k=0; k<nbarsZ; k++) {
					printf("\t   Z%d : ", Zpoints[k]->bar - 14);
					Zpoints[k]->show();
				}
				
				// print figure of merit info
				printf("\n\tPoint figure of merit info:\n\t\t");
				for (int j=0; j<nbarsZ; j++) printf("\t   Z%d", Zpoints[j]->bar - 14);
				printf("\n\t\t");
				for (int j=0; j<nbarsZ; j++) printf("\t%5.1f", Zpoints[j]->E);
				printf("\n-----------------------------------------------\n");
				for (int k=0; k<nbarsX; k++) {
					printf("\t   X%d\t%5.1f |", Xpoints[k]->bar + 1, Xpoints[k]->E);
					for (int j=0; j<nbarsZ; j++) printf("\t%6.2f", pmerit[k*nbarsX + j]);
					printf("\n");
				}
				
				// print center of energy info
	/*			if (cenX) {
					printf("\n\tPlane X center of energy: X = %6.2f; Z = %6.2f\n", cenX->X, cenX->Z);
					printf("\tPlane X energy moment: %6.2f MeV cm2 ; normalized: %6.2f cm2\n", emX, eX>0. ? emX/eX : -1.);
				}
				if (cenZ) {
					printf("\tPlane Z center of energy: X = %6.2f; Z = %6.2f\n", cenZ->X, cenZ->Z);
					printf("\tPlane Z energy moment: %6.2f MeV cm2 ; normalized: %6.2f cm2\n", emZ, eZ>0. ? emZ/eZ : -1.);
				}
				printf("\n\tPlane X points distance from center of energy:\n\t\tbar\tE (MeV)\tX (cm)\tZ (cm)\t D (cm)\n");
				for (int k=0; k<nbarsX; k++) printf("\t\tX%d\t%5.1f\t%6.2f\t%6.2f \t%6.2f\n", Xpoints[k]->bar + 1, Xpoints[k]->E, Xpoints[k]->X, Xpoints[k]->Z, getPointDistance(Xpoints[k], cenX));
				printf("\n\tPlane Z points distance from center of energy:\n\t\tbar\tE (MeV)\tX (cm)\tZ (cm)\t D (cm)\n");
				for (int k=0; k<nbarsZ; k++) printf("\t\tZ%d\t%5.1f\t%6.2f\t%6.2f \t%6.2f \n", Zpoints[k]->bar -14, Zpoints[k]->E, Zpoints[k]->X, Zpoints[k]->Z, getPointDistance(Zpoints[k], cenZ));
	*/			
				
				printf("\n\tKalman P1: ");
				p1X->show();
				printf("\tPlane X points distance from Kalman track P1:\n\t\tbar\tE (MeV)\tX (cm)\tZ (cm)\t D (cm)\n");
				for (int k=0; k<nbarsX; k++) printf("\t\tX%d\t%5.1f\t%6.2f\t%6.2f \t%6.2f\n", Xpoints[k]->bar + 1, Xpoints[k]->E, Xpoints[k]->X, Xpoints[k]->Z, getPointDistance(Xpoints[k], p1X));
				
				printf("\tClosest point to P1 on plane X is point %d: ", iminP1);
				Xpoints[iminP1]->show();
				/*if (bestx > -1) {
					printf("\tBest point on plane X is point %d: ", bestx);
					Xpoints[bestx]->show();
				}*/
				if (fabs(dxP1)>20.) printf("\tWARNING P1 X DEVIATION > 20 cm\n");
				
				printf("\n\tKalman P2: ");
				p2X->show();
				printf("\tPlane X points distance from Kalman track P2:\n\t\tbar\tE (MeV)\tX (cm)\tZ (cm)\t D (cm)\n");
				for (int k=0; k<nbarsX; k++) printf("\t\tX%d\t%5.1f\t%6.2f\t%6.2f \t%6.2f\n", Xpoints[k]->bar + 1, Xpoints[k]->E, Xpoints[k]->X, Xpoints[k]->Z, getPointDistance(Xpoints[k], p2X));
				printf("\tClosest point to P2 on plane X is point %d: ", iminP2);
				Xpoints[iminP2]->show();
				if (fabs(dxP2)>20.) printf("\tWARNING P2 X DEVIATION > 20 cm\n");
				
				//int mguess = mresp->GetBestBin(nbarsX, nbarsZ, eX, eZ, 1);

			}
			
			delete p1X;
			delete p2X;
		}
		
		if (sel->doMcalGuess) {
			int mguess = mresp->GetBestBin(nbarsX, nbarsZ, eX, eZ);
			float auxE = 0.5*(Estep[mguess] + Estep[mguess+1]);  
			float avgE = mresp->GetBestEnergy();
			if (sel->isTrigTrack) {
				hEMEtag->Fill(ETagged[i], auxE);
				hEMavgEtag->Fill(ETagged[i], avgE);
			}
		}
		
		hNBarsX->Fill(nbarsX);
		hNBarsZ->Fill(nbarsZ);
		hNTracksX->Fill(ntrX);
		hNTracksZ->Fill(ntrZ);
		hTracksCorr->Fill(ntrX, ntrZ);
		hBarCorr->Fill(nbarsX, nbarsZ);
		nsel++;
		hEX->Fill(eX);
		hEZ->Fill(eZ);
		hEtot->Fill(eX + eZ);
//			if (nsel<=100) printf("%d: evt %d - EX= %6.2f\t - EZ= %6.2f\t - EZ/EX= %6.2f\t - Etot= %6.2f\n", nsel, i, eX, eZ, eX>0. ? eZ/eX : -111., eX+eZ);
		hEXEZ->Fill(eX, eZ);
		if (eX>0) {
			hERatio->Fill(eZ/eX);
			hERatioSel->Fill(eZ/eX);
			hEREtot->Fill(eZ+eX, eZ/eX);
		}
		
		// fill MCAL response matrix
		if (sel->buildResponseMatrix) mresp->Fill(ibin, nbarsX, nbarsZ, eX, eZ);
		
		// MCAL guess for event energy
	/*	if (sel->doMcalGuess) {
			printf("\nMCAL guess: Run %d - id %d - pkt %d - evt %d - E tag. = %.2f MeV\n", runid, i, pktnumb[i], evtnumb[i], sel->isTrigTrack ? ETagged[i]: -1.);
			int mguess = mresp->GetBestBin(nbarsX, nbarsZ, eX, eZ, 1);
			if (EK[i]>0) int kbin = mresp->GetKalmanBin(EK[i], 1);
		}*/
				
/*		if (sel->isTrigTrack) {
			if (ETagged[i]<50.) hTracks1->Fill(ntrX, ntrZ);
			else if (ETagged[i]<100.) hTracks2->Fill(ntrX, ntrZ);
				else if (ETagged[i]<200.) hTracks3->Fill(ntrX, ntrZ);
					else hTracks4->Fill(ntrX, ntrZ);
		}
*/		
		if (sel->isTrigTrack && ntrX>=sel->minTrX && ntrX<=sel->maxTrX) {
			//hETagged->Fill(ETagged[i]);
			hEratioEtag->Fill(ETagged[i], eX/eZ);
			hEtotEtag->Fill(ETagged[i], eX+eZ);
		}
		
		if (sel->isTrigTrack && sel->doKalmanCorr) {
			if (EK[i] > 0.) {
				nEK++;
				hEdev->Fill(ETagged[i], EK[i]/ETagged[i] - 1.);
				hEK->Fill(ETagged[i], EK[i]);
				gEMEK->SetPoint(i, EK[i], eX+eZ);
				
				for (int j=0; j<nEtag-1; j++) {
					if (ETagged[i]<Estep[j+1]) {
						gEMEK_sel[j]->SetPoint(i, EK[i], eX+eZ);
						break;
					}
				}
				if (eX+eZ > EK[i]) {
					hEMcorr->Fill(ETagged[i]);
					hEKcorr->Fill(ETagged[i], eX+eZ);
					nEMcorr++;
				} else hEKcorr->Fill(ETagged[i], EK[i]);
			}
		}
		
		if (sel->isTrigTrack && sel->doKalmanCorr && fabs(EK[i]/ETagged[i] - 1.)<=0.5) {
			hEtotEtag_sel->Fill(ETagged[i], eX+eZ);
			hEratioEtag_sel->Fill(ETagged[i], eX/eZ);
			
			if (ETagged[i]<50.) hTracks1->Fill(ntrX, ntrZ);
			else if (ETagged[i]<100.) hTracks2->Fill(ntrX, ntrZ);
				else if (ETagged[i]<200.) hTracks3->Fill(ntrX, ntrZ);
					else hTracks4->Fill(ntrX, ntrZ);
					
			if (ntrX==1 && ntrZ==1) {
				if (ETagged[i]<50.) hBars1->Fill(nbarsX, nbarsZ);
				else if (ETagged[i]<100.) hBars2->Fill(nbarsX, nbarsZ);
					else if (ETagged[i]<200.) hBars3->Fill(nbarsX, nbarsZ);
						else hBars4->Fill(nbarsX, nbarsZ);			
			}

		}
	for (int i=0; i<nbarsX; i++) if (Xpoints[i]) delete Xpoints[i];
	for (int i=0; i<nbarsZ; i++) if (Zpoints[i]) delete Zpoints[i];		
	if (cenX) delete cenX;
	if (cenZ) delete cenZ;	
	}
	
	if(ETagged) delete ETagged;
	if(EK1) delete EK1;
	if(EK2) delete EK2;
	if(EK) delete EK;
	if(nevt1x) delete nevt1x;
	if(offset1x) delete offset1x;
	if(primopX) delete primopX;
	if(primopY) delete primopY;
	
	if (sel->buildResponseMatrix) {
		cout << "*** Updated N events/bin: ";
		for (int i=0; i<NBINS; i++) cout << mresp->GetNEvents(i) << "  ";
		cout << endl;
		mresp->Normalize(ibin);
		mresp->Write("mcal_response", TObject::kSingleKey);
	}
	if (fp) fp->Close();
	
	// check on the response matrix
	if (sel->doMcalGuess) {
		for (int i=0; i<NBINS; i++) {
			TH2F *aux = mresp->GetBarCorr(i, 1);
			histo->AddLast(aux);
			TH2F *aux2 = mresp->GetEnergyCorr(i, 1);
			histo->AddLast(aux2);
		}
	}
	
	cout << "*** events with E<50 MeV = " << n1 << endl;
	cout << "Events with P1 == P2: " << nP1eqP2 << endl;
	cout << "Events with P1 != P2: " << nP1noteqP2 << endl;
	cout << endl << "Total number of events: " << nevents << endl;
	cout << "Events with EK>0:  " << nEK << "(" << 100.*nEK/nevents << "%)" << endl;
	cout << "Events with EM>EK: " << nEMcorr << "(" << 100.*nEMcorr/nEK << "% of nEK)" << endl;
	return;
}

//
//--------------------------------------------------------------------
//

int getClosestPoint(int nx, point **px, point *p0)
{
	// returns the index of the point in array px closest to point p0

	if (nx==0 || p0==NULL) return -1;
	float d;
	int imin = 0;
	float dmin = getPointDistance(px[0], p0);
	for (int i=1; i<nx; i++) {
		d = getPointDistance(px[i], p0);
		if (d < dmin) {
			imin = i;
			dmin = d;
		}
	}
	return imin;
}

//
//--------------------------------------------------------------------
//

int getClosestPointX(int nx, point **px, point *p0)
{
	// returns the index of the point in array px closest to point p0

	float d;
	int imin = 0;
	float dmin = fabs(px[0]->X - p0->X);
	for (int i=1; i<nx; i++) {
		d = fabs(px[i]->X - p0->X);
		if (d < dmin) {
			imin = i;
			dmin = d;
		}
	}
	return imin;
}

//
//--------------------------------------------------------------------
//

float getPointDistance(point *px, point *pz)
{
	float dx, dz;
//	dx = px->X - pz->X;
//	dz = px->Z - pz->Z;
	dx = px->xk - pz->xk;
	dz = px->yk - pz->yk;
	return sqrt(dx*dx + dz*dz);
}

//
//--------------------------------------------------------------------
//

point *getEnergyCenterPlane(int n, point **p)
{
	if (n==0) return NULL;
	float Etot = 0.;
	point *pc = new point();
	if (p[0]->plane == 0) pc->plane = 0;
	else pc->plane = 1;
	for (int i=0; i<n; i++) Etot += p[i]->E;
	for (int i=0; i<n; i++) {
		pc->X += p[i]->E * p[i]->X;
		pc->Z += p[i]->E * p[i]->Z;
	}
	pc->E = Etot;
	pc->X /= Etot;
	pc->Z /= Etot;
	pc->setKalmanCoordinate();
	return pc;
}

//
//--------------------------------------------------------------------
//

point *getEnergyCenterMCAL(int nx, int nz, point *px, point **pz)
{

}

//
//--------------------------------------------------------------------
//

float getEnergyMoment(point *cen, int n, point **p)
{
	if (!cen) return -100.;
	float em=0.;
	float dx, dz;
	for (int i=0; i<n; i++) {
		dx = p[i]->X - cen->X;
		dz = p[i]->Z - cen->Z;
		em += p[i]->E * (dx*dx + dz*dz);
	}
	return em;
}

//
//--------------------------------------------------------------------
//

float pointFigureOfMerit1(int nbarsX, point **Xpoints, int nbarsZ, point **Zpoints, float *fom, int *bestx, int *bestz)
{
	// calculate best points (figure of merit of points)
	// returns the highest fom value
	
	int ix=-1, iz=-1;
	float bestfom = -1., dist, cfom;
	
	if (nbarsX==0 && nbarsZ==0) {
		*bestx = ix;
		*bestz = iz;	
		return -1.;
	}
	
	if (nbarsX==0) { // get the most energetic point on plane Z
		iz = 0;
		bestfom = Zpoints[0]->E;
		for (int j=1; j<nbarsZ; j++) {
			if (Zpoints[j]->E > bestfom) {
				bestfom = Zpoints[j]->E;
				iz = j;
			}
		}
		*bestx = ix;
		*bestz = iz;	
		return bestfom;
	}
	
	if (nbarsZ==0) { // get the most energetic point on plane X
		ix = 0;
		bestfom = Xpoints[0]->E;
		for (int j=1; j<nbarsX; j++) {
			if (Xpoints[j]->E > bestfom) {
				bestfom = Xpoints[j]->E;
				ix = j;
			}
		}
		*bestx = ix;
		*bestz = iz;	
		return bestfom;
	}
	
	// points on both planes: compute figure of merit
	
		// initial values
	dist = getPointDistance(Xpoints[0], Zpoints[0]);
	if (dist>0) bestfom = (Xpoints[0]->E + Zpoints[0]->E)/dist;
	else bestfom = (Xpoints[0]->E + Zpoints[0]->E)*1000.;
	
		//main cycle
	for (int k=0; k<nbarsX; k++) {	
		for (int j=0; j<nbarsZ; j++) {
			dist = getPointDistance(Xpoints[k], Zpoints[j]);
			if (dist>0) cfom = (Xpoints[k]->E + Zpoints[j]->E)/dist;
			else cfom = (Xpoints[k]->E + Zpoints[j]->E)*1000.;
			
			fom[k*nbarsX + j] = cfom;
			if (cfom > bestfom) {
				bestfom = cfom;
				ix = k;
				iz = j;
			}
		} 
	}		
	*bestx = ix;
	*bestz = iz;	
	return bestfom;
}

