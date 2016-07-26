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
#include "process3908.h"

using namespace std ; 

int main(int argc, char *argv[])
{
	TApplication *theApp;
	bool batch=TRUE;	// FALSE
	int 	status = 0;
	char 	comment[100];
	int pkttype = 0;
	int pktstype = 0;
	int obsid = 0;
	int nevents=0;
	TString grbfilename("./tmp3908.dat");

	cout << endl << "process3908 version " << VERSION << endl;
	
	cout << "Trying to open fits file: " << argv[1] << endl;
	fitsfile *fptr;		
	char aux[200];
	status = 0;
	sprintf(aux, "%s[0]",  argv[1]);  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	
	fits_read_key(fptr, TINT, "PKTTYPE", &pkttype, comment, &status);
	fits_read_key(fptr, TINT, "PKTSTYPE", &pktstype, comment, &status);
	fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status);
	
	if (pkttype!=39 || pktstype!=8) {
		cout << "This is not a 3908 file. Exiting!" << endl;
		return 1;
	}
		
	// read extension PACKETS
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevents, comment, &status) )  printerror( status );
	int icol=0;
	char colname[10];
	Int_t *bstarthi = new Int_t   [nevents] ;
	Int_t *bstartlo = new Int_t   [nevents] ;
	Int_t *bstophi  = new Int_t   [nevents] ;
	Int_t *bstoplo  = new Int_t   [nevents] ;
	Double_t *bstart   = new Double_t[nevents] ;
	Double_t *bstop    = new Double_t[nevents] ;
	short *brstpkts  = new short[nevents];
	char *fbebchip  = new char[nevents];
	char *mcsmsflg  = new char[nevents];
	char *mcal16ms  = new char[nevents];
	char *mcal1ms  = new char[nevents];
	char *longbrfl  = new char[nevents];
	char *sabtrfl  = new char[nevents];
	Int_t *pktid  = new Int_t   [nevents] ;
	short *packetid  = new short   [nevents] ;
	if (fits_get_colname(fptr, CASESEN, "BSTARTHI", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstarthi,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BSTARTLO", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstartlo,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BSTOPHI", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstophi,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BSTOPLO", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, bstoplo,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "FBEBCHIP", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TBYTE, icol, 1,1,nevents,0, fbebchip,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "BRSTPKTS", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TSHORT, icol, 1,1,nevents,0, brstpkts,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "PACKETID", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TSHORT, icol, 1,1,nevents,0, packetid,   NULL,&status)) printerror(status);
	if (fits_get_colname(fptr, CASESEN, "PAKTNUMB", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevents,0, pktid,   NULL,&status)) printerror(status);	
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
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nrtmtrs,0, paktnumb,   NULL,&status)) printerror(status);
	
	// read extension EVENTS
	int nevt;
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevt, comment, &status) )  printerror( status );
	Int_t *pktid2  = new Int_t   [nevt] ;
	Double_t *time   = new Double_t[nevt] ;
	if (fits_get_colname(fptr, CASESEN, "PAKTNUMB", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TINT32BIT, icol, 1,1,nevt,0, pktid2,   NULL,&status)) printerror(status);	
	if (fits_get_colname(fptr, CASESEN, "TIME", colname, &icol, &status)) printerror(status);	
	if (fits_read_col(fptr,TDOUBLE, icol, 1,1,nevt,0, time,   NULL,&status)) printerror(status);
	
	
	FILE *fp;
	fp = fopen(grbfilename.Data(), "a");	// "w"
	int ngrb=0;
	double tpr = -1.;
//	TTimeStamp epoch(2003, 12, 31, 23, 0, 0, 0, 1, 0);	// WRONG for legal/solar time change
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	int nchip[4];
	int evtid=0;
	//int totpkts = -packetid[0]+1;
	int totpkts = 0;
	double mint, maxt;
	
	if (!batch)  {
		theApp = new TApplication("App", &argc, argv);
		cout << "Work in interactive mode" << endl;
	} else cout << "Work in batch mode" << endl;
	
	int npt=0;
	
	// set the graphic environment
	
	TGraph *gt = new TGraph(1000000);
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);  // 2D graph colour version
	//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.04, "XYZ");
	mcalstyle->SetTitleSize(0.04, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	TLine *al[100];
	
	// build the light curve histo
	TH1F *hlc = new TH1F("hlc", "hlc", 20000, -5000., 15000.);
	double t0 = 99792000. + bstarthi[0] + bstartlo[0]*1.E-6;
	for (int j=0; j<nevt; j++) hlc->Fill(time[j] - t0);
	
	// main loop
	int inievt;
	TLine *l;
	
	tpr = -1.;
	int notgood;
	int bint;
	for (int i=0; i<nevents; i++) {
		bstarthi[i] += 99792000.;	// do on the fly TT correction
		bstophi[i]  += 99792000.;
		if (bstarthi[i] != tpr) {		// it means we have a new trigger, in origin it was >
		
			for (int j=0; j<4; j++) nchip[j] = 0;
			inievt = evtid;
			ngrb++;
			tpr = bstarthi[i];
			
			bint = 5000 + (int) (bstarthi[i]+bstartlo[i]*1.E-6 - t0);
			if (hlc->GetBinContent(bint) == 0. || hlc->GetBinContent(bint+1) == 0. || hlc->GetBinContent(bint+2) == 0.) notgood = 1;
			else notgood = 0;
			
			
			TTimeStamp tzero;
			//tzero.Set((int) bstarthi[i], 1, epoch.GetSec(),0);  // WRONG for legal/solar time change
			tzero.Set((int) bstarthi[i], 0, epoch.GetSec(),0);
			fprintf(fp, "%06d %3d %20.6f %20.6f %10.3f %s\t", obsid, ngrb, bstarthi[i]+bstartlo[i]*1.E-6, bstophi[i]+bstoplo[i]*1.E-6, bstophi[i]-bstarthi[i]+(bstoplo[i]-bstartlo[i])*1.E-6, tzero.AsString("s"));
			
			// get mint and maxt info
			
			// find true final packet
			int npkts = 1;
			while (i+npkts-1<nevents && packetid[i+npkts-1]<brstpkts[i] && (packetid[i+npkts-1]<packetid[i+npkts] || i+npkts== nevents-1)) npkts++;
			if (npkts<brstpkts[i]) cout << "*** orbit " << obsid << "  trigger " << ngrb << " less packets (" << npkts << ") than expected (" << brstpkts[i] << ")" << endl;
			totpkts += npkts;
			mint = time[evtid] - (bstarthi[i]+bstartlo[i]*1.E-6);
			while (evtid<nevt && pktid2[evtid] <= totpkts) {
				if (evtid%10 == 0) gt->SetPoint(evtid, evtid, time[evtid] - (bstarthi[0]+bstartlo[0]*1.E-6));	// i invece che 0
				evtid++;
			}
			maxt = time[evtid-1] - (bstarthi[i]+bstartlo[i]*1.E-6);
			fprintf(fp, " %12.6f %12.6f\t", mint, maxt);
			// cout << i << "  " << totpkts << "  " << evtid-1 << endl;
			
			fprintf(fp, "UNK\t");
			fprintf(fp, " %2d %2d %2d %2d %2d\t", sabtrfl[i], longbrfl[i], mcsmsflg[i], mcal1ms[i], mcal16ms[i]);
			
			// print trigger RM and SIT info
			int startid = -1;
			int nrm = 0;		// number of triggered RM (should be 2 with current trigger conf)
			while (paktnumb[++startid] < pktid[i]);			// find corresponding packet
			while (paktnumb[startid + nrm] == pktid[i]) nrm++;	// find number of RM
			fprintf(fp, "%3d \t", nrm);
			fprintf(fp, " %d\t %d\t", npkts, brstpkts[i]);
			//for (int j=0; j<nrm; j++) fprintf(fp, "  %3d %3d %6d %6d \t", rtmtidch[startid + j], rmtidsit[startid + j], bscirtmt[startid + j], bkgertmt[startid + j]);
			
		}
		nchip[fbebchip[i]]++;
		if (i==nevents-1 || (bstarthi[i+1] + 99792000.) != tpr) {
			// print CHIP info
			fprintf(fp, " %d\t %d\t %d\t %d \t%2d\n", nchip[0], nchip[1], nchip[2], nchip[3], notgood);
			float auxt = (bstarthi[i]+bstartlo[i]*1.E-6 - bstarthi[0] - bstartlo[0]*1.E-6);
			// cout << inievt << "  " << auxt << "  " << evtid << endl;
			printf("%2d \t%20.6f \t%8d \t%8d\n", ngrb, bstarthi[i]+bstartlo[i]*1.E-6, inievt, evtid-1);
			al[ngrb-1] = new TLine(inievt, auxt, evtid, auxt);
			al[ngrb-1]->SetLineColor(2);
		}
		
	}
	gt->GetHistogram()->SetFillStyle(4000);
	gt->Draw("AP");
	for (int i=0; i<ngrb; i++) al[i]->Draw();
	if (!batch) theApp->Run();	

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

