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
#include "mcalanalyzer.h"

using namespace std ; 

int process3908(fitsfile *fptr, TString *boostfile, TString *grbfilename)
{
	int status = 0;
	char 	comment[100];
	int nevents=0;
	int obsid = 0;
	fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status);

	// MM 27/07/16 NB: NON USARE MAI TINT32BIT!!!!!
	
	// read extension PACKETS
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
	int icol=0;
	char colname[10];
	//Int_t *bstarthi = new Int_t   [nevents] ;
	//Int_t *bstartlo = new Int_t   [nevents] ;
	//Int_t *bstophi  = new Int_t   [nevents] ;
	//Int_t *bstoplo  = new Int_t   [nevents] ;
	Double_t *bstart   = new Double_t[nevents] ;
	Double_t *bstop    = new Double_t[nevents] ;
	Short_t *brstpkts  = new Short_t[nevents];
	char *fbebchip  = new char[nevents];
	unsigned char *mcsmsflg  = new unsigned char[nevents];
	char *mcal16ms  = new char[nevents];
	char *mcal1ms  = new char[nevents];
	char *longbrfl  = new char[nevents];
	char *sabtrfl  = new char[nevents];
	Int_t *pktid  = new Int_t   [nevents] ;
	Short_t *packetid  = new Short_t[nevents] ;
	//if (fits_get_colname(fptr, CASESEN, "BSTARTHI", colname, &icol, &status)) printerror(status);	
	//if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstarthi,   NULL,&status)) printerror(status);
	//if (fits_get_colname(fptr, CASESEN, "BSTARTLO", colname, &icol, &status)) printerror(status);	
	//if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstartlo,   NULL,&status)) printerror(status);
	//if (fits_get_colname(fptr, CASESEN, "BSTOPHI", colname, &icol, &status)) printerror(status);	
	//if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstophi,   NULL,&status)) printerror(status);
	//if (fits_get_colname(fptr, CASESEN, "BSTOPLO", colname, &icol, &status)) printerror(status);	
	//if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstoplo,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "PACKETID", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TSHORT, icol, 1,1,nevents,0, packetid,   NULL,&status)) printerror(status); // TSHORT
	if (fits_get_colname(fptr, CASESEN, "FBEBCHIP", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, fbebchip,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BRSTPKTS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TSHORT, icol, 1,1,nevents,0, brstpkts,   NULL,&status)) printerror(status);
	
	if (fits_get_colname(fptr, CASESEN, "PAKTNUMB", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT, icol, 1,1,nevents,0, pktid,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "MCSMSFLG", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, mcsmsflg,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "LONGBRFL", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, longbrfl,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "SABTRFL", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, sabtrfl,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "MCAL16MS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, mcal16ms,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "MCAL1MS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, mcal1ms,   NULL,&status)) printerror(status);	
	
	if (fits_get_colname(fptr, CASESEN, "BSTART", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, bstart,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BSTOP", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevents,0, bstop,   NULL,&status)) printerror(status);


	// read extension RATEMTRS
	int nrtmtrs;
	if ( fits_movnam_hdu(fptr, ANY_HDU, "RATEMTRS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nrtmtrs, comment, &status) )  printerror( status );
	char *rtmtidch  = new char[nrtmtrs];
	char *rmtidsit  = new char[nrtmtrs];
	long int *bscirtmt = new long  int [nrtmtrs] ; //Int_t
	long int  *bkgertmt = new long int [nrtmtrs] ;
	Int_t *paktnumb = new Int_t   [nrtmtrs] ;
	if (fits_get_colname(fptr, CASESEN, "RTMTIDCH", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nrtmtrs,0, rtmtidch,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "RMTIDSIT", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nrtmtrs,0, rmtidsit,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BSCIRTMT", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TULONG, icol, 1,1,nrtmtrs,0, bscirtmt,   NULL,&status)) printerror(status);//TINT32BIT
	if (fits_get_colname(fptr, CASESEN, "BKGERTMT", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TULONG, icol, 1,1,nrtmtrs,0, bkgertmt,   NULL,&status)) printerror(status);//TINT32BIT
	if (fits_get_colname(fptr, CASESEN, "PAKTNUMB", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT, icol, 1,1,nrtmtrs,0, paktnumb,   NULL,&status)) printerror(status);
	
	// read extension EVENTS
	int nevt;
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevt, comment, &status) )  printerror( status );
	Int_t *pktid2  = new Int_t   [nevt] ;
	Double_t *time   = new Double_t[nevt] ;
	if (fits_get_colname(fptr, CASESEN, "PAKTNUMB", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT, icol, 1,1,nevt,0, pktid2,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "TIME", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevt,0, time,   NULL,&status)) printerror(status);
	
	// retrieve boost information if available
	ifstream fb;
	int nrow=0;
	double *bini=NULL, *bend=NULL, *cont=NULL;
	string *label=NULL;
	if (!boostfile->Contains("none", TString::kExact)) {
		cout << "Opening BOOST file " << boostfile->Data() << endl;
		fb.open(boostfile->Data(), ifstream::in);
		string str;
		while ( fb.good() && !fb.eof() ) {
			getline(fb, str);
			nrow++;
		}
		nrow--; 	// to overcome the final line problem found with GRB on orbit 4657!
		// cout << "Number of rows = " << nrow << endl;
		if (nrow>0) {
			fb.close();
			fb.clear();	// <-- FONDAMENTALE! altrimenti non resetta lo status EOF del file
			bini = new double[nrow];
			bend = new double[nrow];
			cont = new double[nrow];
			label = new string[nrow];
			fb.open(boostfile->Data());
			for (int i=0; i<nrow; i++) {
				fb >> cont[i] >> bini[i] >> bend[i] >> label[i];
				//cout << cont[i] << "  " << bini[i] << "  " << bend[i] << "  " << label[i] << endl;
			}
		} else cout << "BOOST file does not exist: 'UNK' flag will be used." << endl;
	} else cout << "No BOOST file specified: 'UNK' flag will be used." << endl;
	
	FILE *fp;
	fp = fopen(grbfilename->Data(), "a");
	int ngrb=0;
	double tpr = -1.;
//	TTimeStamp epoch(2003, 12, 31, 23, 0, 0, 0, 1, 0);	// WRONG for legal/solar time change
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	int nchip[4];
	int evtid=0;
	//int totpkts = -packetid[0]+1;
	int totpkts = 0;
	Double_t mint, maxt;
	
	// main loop
	for (int i=0; i<nevents; i++) {
		//bstarthi[i] += 99792000.;	// do on the fly TT correction
		//bstophi[i]  += 99792000.;
		//bstart[i] -= 366.; // MM 16/02/10: offset empirico basato su GRB150811B, contatto 42920. da capire e correggere!
		if (bstart[i]!=tpr) {	// bstarthi[i] 
			for (int j=0; j<4; j++) nchip[j] = 0;
			ngrb++;
			tpr = bstart[i];
			TTimeStamp tzero;
			//tzero.Set((int) bstarthi[i], 1, epoch.GetSec(),0);  // WRONG for legal/solar time change
			tzero.Set((int) bstart[i], 0, epoch.GetSec(),0);
			//fprintf(fp, "%06d %3d %20.6f %20.6f %10.3f %s\t", obsid, ngrb, bstarthi[i]+bstartlo[i]*1.E-6, bstophi[i]+bstoplo[i]*1.E-6, bstophi[i]-bstarthi[i]+(bstoplo[i]-bstartlo[i])*1.E-6, tzero.AsString("s"));
			fprintf(fp, "%06d %3d %20.6f %20.6f %10.3f %s\t", obsid, ngrb, bstart[i], bstop[i], bstop[i]-bstart[i], tzero.AsString("s"));
			
			// get mint and maxt info
			
			// find true final packet
			int npkts = 1;
			while (i+npkts-1<nevents && packetid[i+npkts-1]<brstpkts[i] && (packetid[i+npkts-1]<packetid[i+npkts] || i+npkts== nevents-1)) npkts++;
			if (npkts<brstpkts[i]) cout << "*** orbit " << obsid << "  trigger " << ngrb << " less packets (" << npkts << ") than expected (" << brstpkts[i] << ")" << endl;
			totpkts += npkts;
			//mint = time[evtid] - (bstarthi[i]+bstartlo[i]*1.E-6);
			mint = time[evtid] - bstart[i];
			while (evtid<nevt && pktid2[evtid] <= totpkts) evtid++;
			//maxt = time[evtid-1] - (bstarthi[i]+bstartlo[i]*1.E-6);
			maxt = time[evtid-1] - bstart[i];
			fprintf(fp, " %12.6f %12.6f\t", mint, maxt);
			// cout << i << "  " << totpkts << "  " << evtid-1 << endl;
			
			// is it a boost?
			int iboost=-1;
		//	while(bini[iboost] < bstarthi[i] && iboost<nrow) iboost++;
			if (nrow>0) {
				for (int j=0; j<nrow; j++) {
					if (bini[j] <= bstart[i] && bend[j] >= bstart[i]) {
						iboost = j;
						break;
					}
				}
				if (iboost == -1) {
					if (bstart[i] < TMath::MinElement(nrow, bini) || bstart[i] > TMath::MaxElement(nrow, bini)) {
						fprintf(fp, "UNK\t");
					} else fprintf(fp, "GRB\t");
				} else fprintf(fp, "%s\t", label[iboost].data());
			} else fprintf(fp, "UNK\t");
			fprintf(fp, " %2d %2d %2d %2d %2d\t", sabtrfl[i], longbrfl[i], mcsmsflg[i], mcal1ms[i], mcal16ms[i]);
			
			// print trigger RM and SIT info
			int startid = -1;
			int nrm = 0;		// number of triggered RM (should be 2 with current trigger conf)
			while (paktnumb[++startid] < pktid[i]);			// find corresponding packet
			while (paktnumb[startid + nrm] == pktid[i]) nrm++;	// find number of RM
			fprintf(fp, "%3d \t", nrm);
			for (int j=0; j<nrm; j++) fprintf(fp, "  %3d %3d %6d %6d \t", 
				rtmtidch[startid + j], rmtidsit[startid + j], bscirtmt[startid + j], bkgertmt[startid + j]);
		}
		nchip[fbebchip[i]]++;
		if (i==nevents-1 || bstart[i+1] != tpr) {	// >
			// print CHIP info
			fprintf(fp, " %d\t %d\t %d\t %d\n", nchip[0], nchip[1], nchip[2], nchip[3]);
		}
	}
	if (bini) delete bini;
	if (bend) delete bend;
	if (cont) delete cont;

	return 0;
}

