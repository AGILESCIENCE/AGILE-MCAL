#include "scirm_analysis.h"
#include "version.h"

using namespace std ; 

void printerror( int );
int doConvertFits(TString *, TString *, double t0, double tgrbini, double tgrbfin, double tbkgini, double tbkgfin);
int doConvertFits2(TString *, TString *);
int doAnalysis(int, int);
	
struct rm {
	int id;
	double t; // it was float
	int xtot;
	int ztot;
	int x[10];
	int z[10];
	float xhr;
	float zhr;
	long int ac[5];
	double lon;
	double lat;
	
	double h;
	double etheta;
	double ephi;
	float lt; // local time
	int flag;
};

struct coord {
	double t;
	double lon;
	double lat;
};

int main(int argc, char *argv[])
{
	TString infile;
	TString outfile;
	int inicont=1000;
	int fincont=1000;
	
	int convertfits = 0;
	int convertfits2 = 0;
	int doanalysis  = 0;
	
	double t0=0.;
	double tbkgini=0.;
	double tbkgfin=0.;
	double tgrbini=0.;
	double tgrbfin=0.;
	
	cout << endl << "scirm_analysis version " << VERSION << endl;
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-f <fits_file_name> <out_file_name> : opens <fits_file_name> and writes output to <out_file_name>" << endl;
		cout << "-a <ini_cont> <fin_cont> : do analysis between contacts <ini_cont> <fin_cont>" << endl;
		cout << "-t0 <time_zero> set T0 time (seconds TT)" << endl;
		cout << "-bkg <t_bkg_ini> <t_bkg_fin>: set background time interval, time referred to t0" << endl;
		cout << "-grb <t_grb_ini> <t_grb_fin>: set GRB time interval, time referred to t0" << endl;
		return 0;
	}
	
	// parse command line data
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		if (cmd.Contains("-f", TString::kExact)) {
			infile += argv[++iopt];
			outfile += argv[++iopt];
			convertfits2 = 1;
		}
		if (cmd.Contains("-a")) {
			TString saux(argv[++iopt]);
			inicont = saux.Atoi();
			TString saux2(argv[++iopt]);
			fincont = saux2.Atoi();
			doanalysis  = 1;
		}
		if (cmd.Contains("-t0")) {	// input t0
			TString saux1(argv[++iopt]);
			t0 = saux1.Atof();
			printf("Set T0 (TT) = %20.6f\n", t0);
			convertfits = 1;
		}
		if (cmd.Contains("-bkg", TString::kExact)) {// set background time interval
			TString saux1(argv[++iopt]);
			tbkgini = saux1.Atof();
			TString saux2(argv[++iopt]);
			double tmptbkgfin = saux2.Atof();
			if (tmptbkgfin <= tbkgini) {
				cout << "Wrong t_bkg_fin assignment. Must be > t_bkg_ini" << endl;
				tbkgfin = tbkgini + 1.;
			} else tbkgfin = tmptbkgfin;
			printf("Set background to the following time interval: [%.4f, %.4f]\n", tbkgini, tbkgfin);
		}
		if (cmd.Contains("-grb", TString::kExact)) {// set GRB time interval
			TString saux3(argv[++iopt]);
			tgrbini = saux3.Atof();
			TString saux4(argv[++iopt]);
			double tmptgrbfin = saux4.Atof();
			if (tmptgrbfin < tgrbini) {
				cout << "Wrong t_grb_fin assignment. Must be >= t_grb_ini" << endl;
				tgrbfin = tgrbini + 1.;
			} else tgrbfin = tmptgrbfin;
			printf("Set GRB to the following time interval: [%.4f, %.4f]\n", tgrbini, tgrbfin);
		}
		iopt++;
	}

	if (convertfits && convertfits2) doConvertFits(&infile, &outfile, t0, tgrbini, tgrbfin, tbkgini, tbkgfin);
	if (convertfits2 && !convertfits) doConvertFits2(&infile, &outfile);
	if (doanalysis) doAnalysis(inicont, fincont);

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

//
//--------------------------------------------------------------------
//

int doConvertFits(TString *infile, TString *outfile, double t0, double tgrbini, double tgrbfin, double tbkgini, double tbkgfin)
{
	char aux[200];
	int status = 0;
	int nevts, colnum;
	char 	comment[100];
	double *tarray;
	double *time;
	int *totx, *totz;
	unsigned short *colarray;
	fitsfile *fptr;		
		
	cout << "opening fits file " << infile->Data() << endl;
	sprintf(aux, "%s[0]",  infile->Data());  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	int pkttype = 0;
	int pktstype = 0;
	int obsid = 0;
	fits_read_key(fptr, TINT, "PKTTYPE", &pkttype, comment, &status);
	fits_read_key(fptr, TINT, "PKTSTYPE", &pktstype, comment, &status);
	fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status);
	cout << "packet type: " << pkttype  << " - packet sub type: " << pktstype  << " - observation-id: " << obsid << endl;
	
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevts, comment, &status) )  printerror( status );
	cout << "number of packets: " << nevts << endl;
	if (nevts*8>20000) {
		cout << "*** error: file " << infile->Data() << " has more than 20000 lines. Exiting!" << endl;
		return 1;
	}
	
	// get time
	tarray = new double[nevts];
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )   printerror( status );
	if ( fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevts, 0, tarray, NULL, &status)) printerror( status );
	
	// get MCAL scientific ratemeters
	time = new double[nevts*8];
	colarray = new unsigned short[nevts];
	int data[22][20000];
	long int dataAC[5][20000];
	for (int i=0; i<11; i++) {
		for (int k=0; k<8; k++) {
			// work on RM-Xi
			if (fits_read_col(fptr, TUSHORT, 162+22*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) {
				data[i][j*8+k] = (int) colarray[j];
				time[j*8+k]    = tarray[j]+k*1.024 - 8.192;  // MM 01/02/2012: added -8.192 to synchronize with 3908 data
			}
			// work on RM-Zi
			if (fits_read_col(fptr, TUSHORT, 162+22*k+i+11, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) data[i+11][j*8+k] = colarray[j];
		}
	}
	
	// get AC scientific ratemeters
	for (int i=0; i<5; i++) {
		for (int k=0; k<8; k++) {
			if (fits_read_col(fptr, TUSHORT, 122+5*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) dataAC[i][j*8+k] = (long int) colarray[j];
		}
	}
	fits_close_file(fptr, &status);
	
	// build total ratemeters
	totx = new int[nevts*8];
	totz = new int[nevts*8];
	for (int j=0; j<nevts*8; j++) {
		totx[j] = 0;
		totz[j] = 0;
		for (int i=1; i<11; i++) {
			totx[j] += data[i][j];
			totz[j] += data[i+11][j];
		}
	}
	
	// write sci-RM to file
	FILE *frm;
	frm = fopen(outfile->Data(), "w");
	fprintf(frm, "%20.6f \t%d \nid \tt-t0 \t\tXtot \tZtot \t", time[0], nevts*8);
	for (int i=1; i<11; i++) fprintf(frm, "X%02d \t", i);
	for (int i=1; i<11; i++) fprintf(frm, "Z%02d \t", i);
	for (int i=0; i<5; i++) fprintf(frm, "AC%02d \t", i);
	fprintf(frm, "\n");
	for (int j=0; j<nevts*8; j++) {
		fprintf(frm, "%5d \t%10.3f \t%6d \t%6d \t", j, time[j]-time[0], totx[j], totz[j]);
		for (int i=1; i<11; i++)  fprintf(frm, "%6d \t", data[i][j]);
		for (int i=12; i<22; i++) fprintf(frm, "%6d \t", data[i][j]);
		for (int i=0; i<5; i++)   fprintf(frm, "%6d \t", dataAC[i][j]);
		fprintf(frm, "\n");
	}
	fclose(frm);	
	
	// calculate dead time
	if (t0) {
		cout << "Dead time estimations:" << endl;
		double tbi = t0 + tbkgini;
		double tbf = t0 + tbkgfin;
		double tgi = t0 + tgrbini;
		double tgf = t0 + tgrbfin;
		double ratebkg = 0.;
		double rategrb = 0.;
		double maxrategrb = 0.;
		double dtbkg = 0.;
		double dtgrb = 0.;
		double acwin = 5.4e-6;	// AC blanking window
		for (int j=0; j<nevts*8; j++) {
		
			// bkg dead time
			if (time[j] >= tbi && time[j] < tbf) {
				for (int i=0; i<5; i++) ratebkg += dataAC[i][j];
				dtbkg += 1.024;
			}
		
			// grb dead time
			if (time[j] >= tgi && time[j] < tgf) {
				double aux = 0.;
				for (int i=0; i<5; i++) aux += dataAC[i][j];
				rategrb += aux;
				if (aux > maxrategrb) maxrategrb = aux;
				dtgrb += 1.024;
			}
		}
		double ltbkg = 1. - ratebkg*acwin/dtbkg;	// BKG live time (fraction)
		double ltgrb = 1. - rategrb*acwin/dtgrb;	// GRB live time (fraction)	
		double minltgrb = 1. - maxrategrb*acwin/1.024;	// minimum GRB live time (worst case)	
		cout << "BKG: counts = " << ratebkg << "  total time (s) = " << dtbkg << "  live time (fraction) = " << ltbkg << "  live time (s) = " << ltbkg*(tbkgfin - tbkgini) << endl;
		cout << "GRB: counts = " << rategrb << "  total time (s) = " << dtgrb << "  live time (fraction) = " << ltgrb << "  live time (s) = " << ltgrb*(tgrbfin - tgrbini) << endl;
		cout << "Worst case: maximum AC rate i.e. minimum live time:" << endl;
		cout << "GRB: counts = " << maxrategrb << "  live time (fraction) = " << minltgrb << "  live time (s) = " << minltgrb*(tgrbfin - tgrbini) << endl;
		printf("Summary live time fraction:\nlabel \t\tcont. \tBKG \tGRB \tWORST\nDEADTIME \t%06d \t%5.3f \t%5.3f \t%5.3f\n", obsid, ltbkg, ltgrb, minltgrb);  
	}
		
		
	delete colarray;
	delete tarray;
	delete time;
	delete totx;
	delete totz;
	
	return 0;
}

//
//--------------------------------------------------------------------
//

int doConvertFits2(TString *infile, TString *outfile)
{
	char aux[200];
	int status = 0;
	int nevts, colnum;
	char 	comment[100];
	double *tarray;
	double *time;
	int *totx, *totz;
	unsigned short *colarray;
	fitsfile *fptr;		
	rm srm;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	TTimeStamp tzero;
		
	cout << "opening fits file " << infile->Data() << endl;
	sprintf(aux, "%s[0]",  infile->Data());  
	if ( fits_open_file(&fptr, aux, READONLY, &status) )  printerror( status );
	int pkttype = 0;
	int pktstype = 0;
	int obsid = 0;
	fits_read_key(fptr, TINT, "PKTTYPE", &pkttype, comment, &status);
	fits_read_key(fptr, TINT, "PKTSTYPE", &pktstype, comment, &status);
	fits_read_key(fptr, TINT, "OBS_ID", &obsid, comment, &status);
	cout << "packet type: " << pkttype  << " - packet sub type: " << pktstype  << " - observation-id: " << obsid << endl;
	
	if ( fits_movnam_hdu(fptr, ANY_HDU, "PACKETS", 0, &status) )  printerror( status );
	if ( fits_read_key(fptr, TINT, "NAXIS2", &nevts, comment, &status) )  printerror( status );
	cout << "number of packets: " << nevts << endl;
	if (nevts*8>40000) {
		cout << "*** error: file " << infile->Data() << " has more than 20000 lines. Exiting!" << endl;
		return 1;
	}
	
	// get time
	tarray = new double[nevts];
	if ( fits_get_colnum(fptr, CASEINSEN, "TIME", &colnum, &status) )   printerror( status );
	if ( fits_read_col(fptr, TDOUBLE, colnum, 1, 1, nevts, 0, tarray, NULL, &status)) printerror( status );
	
	// get MCAL scientific ratemeters
	time = new double[nevts*8];
	colarray = new unsigned short[nevts];
	int data[22][40000];
	long int dataAC[5][40000];
	for (int i=0; i<11; i++) {
		for (int k=0; k<8; k++) {
			// work on RM-Xi
			if (fits_read_col(fptr, TUSHORT, 162+22*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) {
				data[i][j*8+k] = (int) colarray[j];
				time[j*8+k]    = tarray[j]+k*1.024 - 8.192;  // MM 01/02/2012: added -8.192 to synchronize with 3908 data
			}
			// work on RM-Zi
			if (fits_read_col(fptr, TUSHORT, 162+22*k+i+11, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) data[i+11][j*8+k] = colarray[j];
		}
	}
	
	// get AC scientific ratemeters
	for (int i=0; i<5; i++) {
		for (int k=0; k<8; k++) {
			if (fits_read_col(fptr, TUSHORT, 122+5*k+i, 1, 1, nevts, 0, colarray, NULL, &status)) printerror(status);	//TSHORT
			for (int j=0; j<nevts; j++) dataAC[i][j*8+k] = (long int) colarray[j];
		}
	}
	fits_close_file(fptr, &status);
	
	// build total ratemeters
	totx = new int[nevts*8];
	totz = new int[nevts*8];
	for (int j=0; j<nevts*8; j++) {
		totx[j] = 0;
		totz[j] = 0;
		for (int i=1; i<11; i++) {
			totx[j] += data[i][j];
			totz[j] += data[i+11][j];
		}
	}
	
	// get coordinates (from macro add_geographic_info_3.C)
	
	gpsevt evt;
        MCVFile3916 *f,*fprev, *fpost;
	char infile3916[100]; 
	sprintf(infile3916, "%s/RT%06d_3916.root", COORPATH, obsid);
        f = new MCVFile3916(infile3916);
	sprintf(infile3916, "%s/RT%06d_3916.root", COORPATH, obsid-1);
        fprev = new MCVFile3916(infile3916);
	sprintf(infile3916, "%s/RT%06d_3916.root", COORPATH, obsid+1);
        fpost = new MCVFile3916(infile3916);
		
	UInt_t hh, min, sec;
	int coorok, auxtime;
	
	// write sci-RM to file
	
	TFile *fout = new TFile(outfile->Data(),"RECREATE");
	TTree *t = new TTree("scirm", "sci. RM and additional info");
	t->Branch("scirm", &srm.id, "id/I:t/D:xtot/I:ztot:x[10]:z[10]:xhr/F:zhr:ac[5]/I:lon/D:lat:h:etheta:ephi:lt/F");
	
	for (int j=0; j<nevts*8; j++) {
		srm.id = obsid;
		srm.t = time[j];
		srm.xtot = totx[j];
		srm.ztot = totz[j];

		for (int i=1; i<11; i++) srm.x[i-1] = data[i][j]; //fprintf(frm, "%6d \t", data[i][j]);
		for (int i=12; i<22; i++) srm.z[i-12] = data[i][j]; //fprintf(frm, "%6d \t", data[i][j]);
		for (int i=0; i<5; i++) srm.ac[i] = dataAC[i][j];  // fprintf(frm, "%6d \t", dataAC[i][j]);
		
		evt.time = srm.t;
		coorok = f->GetCoordinates(&evt);
		if (coorok == 1) coorok = fpost->GetCoordinates(&evt);
		else if (coorok == -1) coorok = fprev->GetCoordinates(&evt);
		srm.xhr = coorok;
		srm.zhr = 0.;
		
		if (coorok==0) {
			srm.lon = evt.lon;
			srm.lat = evt.lat;
			srm.h = evt.h;
			srm.etheta = evt.etheta;
			srm.ephi = evt.ephi;
			
			// calculate local time
			tzero.Set((int) srm.t, 0, epoch.GetSec(),0);
			auxtime = tzero.GetTime(1,0, &hh, &min, &sec);
			srm.lt = fmod(24. + hh + min/60. + sec/3600. + evt.lon*0.06667, 24.);
		}
		else {
			srm.lon = -100.;
			srm.lat = -100.;
			srm.h = -100.;
			srm.etheta = -100.;
			srm.ephi = -100.;
			srm.lt = -100.;
		}
		

		t->Fill();
	}
        
	delete f;   
	delete fprev;   
	delete fpost;   
	
	t->Write();
	fout->Close();
	
	delete colarray;
	delete tarray;
	delete time;
	delete totx;
	delete totz;
	
	return 0;
}


//
//--------------------------------------------------------------------
//

int doAnalysis(int inicont, int fincont)
{
	// incorporates root macro scirm_analysis.C

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
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	// open root file
	TFile fresults(ROOTFILE, "UPDATE");
	
	// load longitude values
	
	cout << "loading longitude file " << LONFILE << endl;
	ifstream fcoord(LONFILE, ios_base::in);
	int id;
	double auxt, auxlon, auxlat;
	coord acoord[20000];
	for (int i=0; i<20000; i++) acoord[i].lon = -1.;
	fcoord.clear();
	while (!fcoord.eof()) {
		fcoord >> id >> auxt >> auxlon >> auxlat;
		if (id>0 && id<20000) {
			acoord[id].t   = auxt;
			acoord[id].lon = auxlon;
			acoord[id].lat = auxlat;
		}
	}
	fcoord.close();
	
	// load boost-eclipse file
	
	cout << "loading boost file " << BOOSTFILE << endl;
	ifstream fboost(BOOSTFILE, ios_base::in);
	double boostini[40000];
	double boostfin[40000];
	double auxcont;
	char caux[20];
	fboost.clear();
	int nboost=0;	
	
	while (!fboost.eof() ) {
		fboost >> auxcont >> boostini[nboost] >> boostfin[nboost] >> caux;
		// cout << nboost << "  " << boostini[nboost] << endl;
		nboost++;
	}
	nboost--;
	fboost.close();
	
	// get (or create) global histo
	TH3F *h3xtot = NULL;
	TH3F *h3ztot = NULL;
	TH3F *h3xhr = NULL;
	TH3F *h3zhr = NULL;
	fresults.GetObject("h3xtot", h3xtot);
	fresults.GetObject("h3ztot", h3ztot);
	fresults.GetObject("h3xhr", h3xhr);
	fresults.GetObject("h3zhr", h3zhr);
	if (!h3xtot) h3xtot = new TH3F("h3xtot", "h3xtot", 2000, 0., 20000., 36, -20., 340., 400, 0., 100.);
	if (!h3ztot) h3ztot = new TH3F("h3ztot", "h3ztot", 2000, 0., 20000., 36, -20., 340., 400, 0., 100.);
	if (!h3xhr) h3xhr = new TH3F("h3xhr", "h3xhr", 2000, 0., 20000., 36, -20., 340., 400, 0., 100.);
	if (!h3zhr) h3zhr = new TH3F("h3zhr", "h3zhr", 2000, 0., 20000., 36, -20., 340., 400, 0., 100.);

	for (int cont=inicont; cont<fincont; cont++) {	// loop on selected contacts
		cout << "processing contact  " << cont << endl;		
		int iboost=0;
	
  	        if (acoord[cont+1].lon == -1. ) {
                        cout << "*** no longitude info for contact " << cont+1 << endl;
                        continue;
                }
                if (acoord[cont+2].lon == -1. ) {
                        cout << "*** no longitude info for contact " << cont+2 << endl;
                        continue;
                }
                if (acoord[cont+1].lon == acoord[cont+2].lon ) {
                        cout << "*** same longitude info for contacts " << cont+1 << " and " << cont+2 << endl;
                        continue;
                }
                double degsec = (360.+acoord[cont+2].lon - acoord[cont+1].lon)/(acoord[cont+2].t - acoord[cont+1].t);
                if (degsec<0.0586 || degsec> 0.0590) {
                        cout << "*** longitude reconstruction problem " << cont+1 << " and " << cont+2 << endl;
                        continue;
                }
	cout << "1" << endl;

		char dataname1[200];
		sprintf(dataname1, "/home/mcal/data_analysis/scirm/scirm_%06d.dat", cont);
		ifstream fscirm1(dataname1, ios_base::in);
		if (!fscirm1) {
			cout << "*** file " << dataname1 << " does not exist" << endl;
			continue;
		}
		
		char dataname2[200];
		sprintf(dataname2, "/home/mcal/data_analysis/scirm/scirm_%06d.dat", cont+1);
		ifstream fscirm2(dataname2, ios_base::in);
		if (!fscirm2) {
			cout << "*** file " << dataname2 << " does not exist" << endl;
			continue;
		}
	cout << "2" << endl;	
		double t01, t02;
		int nrows1, nrows2, nrows;
		float minlon = 290.;
		float maxlon = -20.;
		char saux[10];
		int ns=0;
		int npt=0;	// number of graph points, must be different from ns
		
		fscirm1.clear();
		fscirm1 >> t01 >> nrows1;
		for (int i=0; i<29; i++) fscirm1 >> saux;
	cout << "2.1" << endl;	
		fscirm2.clear();
		fscirm2 >> t02 >> nrows2;
		for (int i=0; i<29; i++) fscirm2 >> saux;
		nrows = nrows1+nrows2;
	cout << "2.2" << endl;
		rm *data = new rm[nrows];
	cout << "3" << endl;	
		printf("%06d \t%20.3f \t%6d \t%20.3f \t%10.3f\n%", cont, t01, nrows1, acoord[cont].t, acoord[cont].lon);
		printf("%06d \t%20.3f \t%6d \t%20.3f \t%10.3f\n%", cont+1, t02, nrows2, acoord[cont+1].t, acoord[cont+1].lon);
		
		// interpolate longitude
		double l1, l2, t1, t2, a, b;
		l1 = acoord[cont+1].lon;
		l2 = acoord[cont+2].lon;
		t1 = acoord[cont+1].t - t01;
		t2 = acoord[cont+2].t - t01;
		if (t2-t1 > 10000.) {
			cout << "*** cannot interpolate longitude at orbit " << cont+1 << endl;
			fscirm1.close();
			fscirm2.close();
			delete data;
			continue;
		}
		l2 += 360.;
		b = (l2-l1)/(t2-t1);
		a = l1 - t1*b;
		
		bool flag;
	cout << "4" << endl;	
		// create graphs
		TGraph *gxtot = new TGraph(10);
		TGraph *gztot = new TGraph(10);
		TGraph *gxhr = new TGraph(10);
		TGraph *gzhr = new TGraph(10);
		gxtot->SetTitle("X RM E>0.3 MeV");
		gztot->SetTitle("Z RM E>0.3 MeV");
		gxhr->SetTitle("X RM E>1.4 MeV");
		gzhr->SetTitle("Z RM E>1.4 MeV");
	cout << "5" << endl;
		while (!fscirm1.eof()) {
			fscirm1 >> data[ns].id >> data[ns].t >> data[ns].xtot >> data[ns].ztot;
			for (int i=0; i<10; i++) fscirm1 >> data[ns].x[i];
			for (int i=0; i<10; i++) fscirm1 >> data[ns].z[i];
			for (int i=0; i<5; i++) fscirm1 >> data[ns].ac[i];
			
			if (data[ns].id == 0 && data[ns].xtot == 0) continue;	// last file line, otherwise segmentation fault
			
			data[ns].xhr = 0.;
			data[ns].zhr = 0.;
			for (int i=3; i<10; i++) data[ns].xhr += data[ns].x[i];
			//if (data[ns].x[0] + data[ns].x[1] > 0.) data[ns].xhr /= (data[ns].x[0] + data[ns].x[1]);
			//else data[ns].xhr = -1.;
			for (int i=3; i<10; i++) data[ns].zhr += data[ns].z[i];
			//if (data[ns].z[0] + data[ns].z[1] > 0.) data[ns].zhr /= (data[ns].z[0] + data[ns].z[1]);
			//else data[ns].zhr = -1.;
			
			auxlon = a + b*data[ns].t;
			//while(auxlon>360.) auxlon -= 360.;
			data[ns].lon = auxlon;
			
			// eliminate idle-obs spikes and high (spurious) events
			flag = 1;
			if (data[ns].xtot<100. || data[ns].xtot>500. || data[ns].ztot>500.) flag = 0; 
			if (flag) {
				for (int h=ns-1; h>=0 && flag && h>ns-10; h--) {
					if (data[h].xtot == 0) flag = 0;
				} 
			}
			
			// eliminate boost/eclipse
			if (flag) {
				auxt = data[ns].t + t01;
				if (ns>0 && data[ns].t <= data[ns-1].t) iboost = 0;	// reset boost index in case of non monotonic times
				while (auxt >= boostfin[iboost]) iboost++;
				if (auxt >= boostini[iboost]) flag = 0;
			}
			
			
			if (flag && data[ns].lon>-20. && data[ns].lon<290.) {
			
				if (data[ns].lon<minlon) minlon =  data[ns].lon;
				if (data[ns].lon>maxlon) maxlon =  data[ns].lon;
				
				gxtot->SetPoint(npt, data[ns].lon, data[ns].xtot);
				gztot->SetPoint(npt, data[ns].lon, data[ns].ztot);
				gxhr->SetPoint(npt, data[ns].lon, data[ns].xhr);
				gzhr->SetPoint(npt, data[ns].lon, data[ns].zhr);
				
				
				npt++;
			}
			ns++;
		}
	cout << "6" << endl;	
		float dt0 = t02-t01;
		while (!fscirm2.eof()) {
			fscirm2 >> data[ns].id >> data[ns].t >> data[ns].xtot >> data[ns].ztot;
			for (int i=0; i<10; i++) fscirm2 >> data[ns].x[i];
			for (int i=0; i<10; i++) fscirm2 >> data[ns].z[i];
			for (int i=0; i<5; i++) fscirm2 >> data[ns].ac[i];
			data[ns].t += dt0;
			
			if (data[ns].id == 0 && data[ns].xtot == 0) continue;	// last file line, otherwise segmentation fault
			
			data[ns].xhr = 0.;
			data[ns].zhr = 0.;
			for (int i=3; i<10; i++) data[ns].xhr += data[ns].x[i];
			//if (data[ns].x[0] + data[ns].x[1] > 0.) data[ns].xhr /= (data[ns].x[0] + data[ns].x[1]);
			//else data[ns].xhr = -1.;
			for (int i=3; i<10; i++) data[ns].zhr += data[ns].z[i];
			//if (data[ns].z[0] + data[ns].z[1] > 0.) data[ns].zhr /= (data[ns].z[0] + data[ns].z[1]);
			//else data[ns].zhr = -1.;
			
			auxlon = a + b*data[ns].t;
			//while(auxlon>360.) auxlon -= 360.;
			data[ns].lon = auxlon;
			
			
			// eliminate idle-obs spikes and high (spurious) events
			flag = 1;
			if (data[ns].xtot<100. || data[ns].xtot>500. || data[ns].ztot>500.) flag = 0; 
			if (flag) {
				for (int h=ns-1; flag && h>ns-10; h--) {
					if (data[h].xtot == 0) flag = 0;
				} 
			}
			
			// eliminate boost/eclipse
			if (flag) {
				auxt = data[ns].t + t01;
				if (ns>0 && data[ns].t <= data[ns-1].t) iboost = 0;	// reset boost index in case of non monotonic times
				while (auxt >= boostfin[iboost]) iboost++;
				if (auxt >= boostini[iboost]) flag = 0;
			}
			
			if (flag && data[ns].lon>-20. && data[ns].lon<290.) {
			
				if (data[ns].lon<minlon) minlon =  data[ns].lon;
				if (data[ns].lon>maxlon) maxlon =  data[ns].lon;
				
				gxtot->SetPoint(npt, data[ns].lon, data[ns].xtot);
				gztot->SetPoint(npt, data[ns].lon, data[ns].ztot);
				gxhr->SetPoint(npt, data[ns].lon, data[ns].xhr);
				gzhr->SetPoint(npt, data[ns].lon, data[ns].zhr);
				
				npt++;
			}
			ns++;
		}
		ns--;
		fscirm1.close();
		fscirm2.close();
	cout << "7" << endl;	
		// do baseline polinomial fit
		
		if (npt<100) {
			cout << "*** too few events at orbit " << cont+1 << endl;
			delete data;
			delete gxtot;
			delete gztot;
			delete gxhr;
			delete gzhr;
			cout << "continue" << endl;
			continue;
		}
		
		TF1 *fxtot = new TF1("fxtot", "pol4", -20., 290.);
		gxtot->Fit(fxtot, "R");
		TF1 *fztot = new TF1("fztot", "pol4", -20., 290.);
		gztot->Fit(fztot, "R");
		TF1 *fxhr = new TF1("fxhr", "pol4", -20., 290.);
		gxhr->Fit(fxhr, "R");
		TF1 *fzhr = new TF1("fzhr", "pol4", -20., 290.);
		gzhr->Fit(fzhr, "R");
	cout << "8" << endl;	
		double *axtot = gxtot->GetY();
		double *aztot = gztot->GetY();
		double *axhr  = gxhr->GetY();
		double *azhr  = gzhr->GetY();
		double *alon  = gxtot->GetX();
			
		// calculate variance and fill global histo
	cout << "9" << endl;	
		float aux, ki2;
	/*	TGraph *gxtot_ki2 = new TGraph(10);
		TGraph *gztot_ki2 = new TGraph(10);
		TGraph *gxhr_ki2 = new TGraph(10);
		TGraph *gzhr_ki2 = new TGraph(10);
		gxtot_ki2->SetTitle("Chi^2 X RM E>0.3 MeV");
		gztot_ki2->SetTitle("Chi^2 Z RM E>0.3 MeV");
		gxhr_ki2->SetTitle("Chi^2 X RM E>1.4 MeV");
		gzhr_ki2->SetTitle("Chi^2 Z RM E>1.4 MeV");
	*/
		int nki = 0;
		for (int i=0; i<npt; i++) {
			if (alon[i]>=-20. && alon[i]<=290.) {
				aux = axtot[i] - fxtot->Eval(alon[i]);
				if (fxtot->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fxtot->Eval(alon[i]));
				else ki2 = 0.;
				//gxtot_ki2->SetPoint(nki, alon[i], ki2);
				h3xtot->Fill(cont, alon[i], ki2);
				
				aux = aztot[i] - fztot->Eval(alon[i]);
				if (fztot->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fztot->Eval(alon[i]));
				else ki2 = 0.;
				//gztot_ki2->SetPoint(nki, alon[i], ki2);
				h3ztot->Fill(cont, alon[i], ki2);
				
				aux = axhr[i] - fxhr->Eval(alon[i]);
				if (fxhr->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fxhr->Eval(alon[i]));
				else ki2 = 0.;
				//gxhr_ki2->SetPoint(nki, alon[i], ki2);
				h3xhr->Fill(cont, alon[i], ki2);
				
				aux = azhr[i] - fzhr->Eval(alon[i]);
				if (fzhr->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fzhr->Eval(alon[i]));
				else ki2 = 0.;
				//gzhr_ki2->SetPoint(nki, alon[i], ki2);
				h3zhr->Fill(cont, alon[i], ki2);
				
				nki++;
			}
		}
	cout << "10" << endl;
		
	/*	TString cname("c_");
		cname += (cont+1);
		TCanvas *c1 = new TCanvas(cname, cname, 800, 800);
		c1->Divide(2, 4);
		
		c1->cd(1);
		gxtot->Draw("ALP");
		fxtot->SetLineColor(3);
		fxtot->Draw("SAME");
		c1->cd(2);
		gxtot_ki2->Draw("ALP");
		
		c1->cd(3);
		gztot->Draw("ALP");
		fztot->SetLineColor(3);
		fztot->Draw("SAME");
		c1->cd(4);
		gztot_ki2->Draw("ALP");
		
		c1->cd(5);
		gxhr->Draw("ALP");
		fxhr->SetLineColor(3);
		fxhr->Draw("SAME");
		c1->cd(6);
		gxhr_ki2->Draw("ALP");
		
		c1->cd(7);
		gzhr->Draw("ALP");
		fzhr->SetLineColor(3);
		fzhr->Draw("SAME");
		c1->cd(8);
		gzhr_ki2->Draw("ALP");
		
		cname += ".gif";
		c1->Print(cname);
	
		// write results to file
		if (cont%100 == 0) {
			h3xtot->Write("", TObject::kOverwrite);
			h3ztot->Write("", TObject::kOverwrite);
			h3xhr->Write("", TObject::kOverwrite);
			h3zhr->Write("", TObject::kOverwrite);
		}*/
		
		// free memory
		delete data;
		delete gxtot;
		delete gztot;
		delete gxhr;
		delete gzhr;
		delete fxtot;
		delete fztot;
		delete fxhr;
		delete fzhr;
	/*	delete gxtot_ki2;
		delete gztot_ki2;
		delete gxhr_ki2;
		delete gzhr_ki2;
		delete c1;
	*/
	}
	cout << "11" << endl;
	 h3xtot->Write("", TObject::kOverwrite);
	 h3ztot->Write("", TObject::kOverwrite);
	 h3xhr->Write("", TObject::kOverwrite);
	 h3zhr->Write("", TObject::kOverwrite);
	cout << "12" << endl;
	fresults.Close();
	return 0;
}

