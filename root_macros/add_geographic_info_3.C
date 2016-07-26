{
#include <string.h>
	// 17/10/2012 updated data path (high energy trigger search #4)
	// 03/08/2012 updated data path
	// MM 09/11/09 corrected bug in the binary search on arrays
	// MM 07/11/09 add_geographic_info_3.C - based on add_geographic_info_2.C

	// MM 06/11/09 add_geographic_info_2.C - based on add_geographic_info.C
	//             set boost flag=-1 if no boost info are available for that contact
	//             add a flag to check if the offline trg corresponds to an onboard trg
	//             add a flag to check if the offline trg corresponds to a selected TGF candidate
	//             if coordinates are not found in the current file check contact +/- 1
	
	// MM 05/11/09 add also time info and local time
	// MM 03/11/09 add geographic info to trigger file produced with macro burst_search.C
	// libMCV.so MUST be loaded first	
	// checks if trigger is in boost (last flag = 1), eclipse (2) or not (0)
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

	struct TriggerBasic  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int rmid[9];
			int sitid[9];
			int thr[9];
			int ber[9];
			int buffer[4];
	};

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};

	// load offline trigger file
	ifstream inFile;
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_005000-012585.dat");	// on HP1
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/orbits_012586-015154_Mar2010/trg_2_012586-015154.dat");	// on HP1
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/orbits_009564-19872_Feb2011/trg_2_005000-019872.dat");	// on HP1
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_019872-027208.dat");	// on HP1, update to 31/7/2012
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_4/trg_4_005000-027208.dat");	// on HP1, high energy events selected with macro mcal_burst_search_4.sh
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_027209-033247.dat");	// on HP1, update to 30/9/2013
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_033248-034543.dat");	// on HP1, update to 31/12/2013
	inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_040418-041012.dat");	// on HP1, update to 31/03/2015
	//inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/tmp.dat");	// test on HP1
	if (!inFile) {
		cout << "Unable to open trigger file";
		exit(1); // terminate with error
	}
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01

	// open output file
	FILE *fp;
	//fp = fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_005000-012585_processed.dat", "a");	// on HP1
	fp = //fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/orbits_012586-015154_Mar2010/trg_2_012586-015154_processed.dat", "a");	// on HP1
	//fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_019872-027208_processed.dat", "a");	// on HP1
	//fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_4/trg_4_005000-027208_processed.dat", "a");	// on HP1
	//fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_027209-033247_processed.dat", "a");	// on HP1
	//fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_033248-034543_processed.dat", "a");	// on HP1
	fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_040418-041012_processed.dat", "a");	// on HP1
	//fp = fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/tmp_processed.dat", "a");	// test
	// retrieve boost information if available

	ifstream fb;
	fb.open("/AGILE_PROC3/DATA/HLT/MCALBURST/BOOST_up_to_now.dat", ifstream::in);
	int nrow=100000;
	double *bini = new double[nrow];
	double *bend = new double[nrow];
	int *cont = new int[nrow];
	double auxcont;
	char saux[12];
	int *label = new int[nrow];
	int nb=0;
	while (fb >> auxcont >> bini[nb] >> bend[nb] >> saux) {
		cont[nb] = auxcont;
		if (strncmp(saux,"BOOST",5) == 0) label[nb] = 1;
		else label[nb] = 2;
		nb++;
	}
	cout << "loaded " << nb << " BOOST lines" << endl;

	// load trigger file mcal_grb.dat
	ifstream fin;
	TriggerBasic atrg;
	double *auxt03908 = new double[500000];
	fin.open("/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive", ifstream::in); //mcal_grb.dat
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.mint3908 >> atrg.maxt3908 >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.flg01ms >> atrg.flg16ms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		auxt03908[ntrg] = atrg.t0;
		ntrg++;
	}
	fin.close();
	ntrg--;
	cout << "sorting times..." << endl;
	int *ind3908 = new int[ntrg];
	TMath::Sort(ntrg, auxt03908, ind3908, 0);
	cout << "number of on-board triggers = " << ntrg << endl;
	double *t03908 = new double[ntrg];
	for (int k=0; k<ntrg; k++) t03908[k] = auxt03908[ind3908[k]];
	delete ind3908;
	delete auxt03908;
	
	gpsevt evt;
        MCVFile3916 *f;
	
	int index = 0;
	int sel = 0;
	char aux[10], path[100], infile[100];
	int trgid, counts, contact, auxtime, auxdate;
	int c[8];
	double t0, hr, dt, lc, eavg, emax;
	UInt_t yy, mm, dd, hh, min, sec;
	TTimeStamp tzero;
	
	// read t0 list for selected TGFs
	ifstream ftgf;
	double t0tgf[400];
	// ftgf.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Jun08-Sep09.dat", ifstream::in);
	//ftgf.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Jun08-Mar10.dat", ifstream::in);
	//ftgf.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Mar09-Feb11.dat", ifstream::in);
	ftgf.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_trg2_Mar09-Jul12.dat", ifstream::in);
	int ntgf = 0;
	int ctgf;
	while (ftgf >> ctgf >> t0tgf[ntgf]) ntgf++;
	cout << "number of read tgf = " << ntgf << endl;
	
	// main loop: read offline triggers and add info
	
	while (inFile >> aux >> path >> trgid >> t0 >> counts >> hr >> dt >> eavg >> emax >> c[0] >> c[1] >> c[2] >> c[3] >> c[4] >> c[5] >> c[6] >> c[7]) {
	
		evt.lon = evt.lat = evt.etheta = evt.ephi = evt.h = 0.;
	
		evt.time = t0;
		sscanf(path, "/MCAL_PROC/ARCHIVE/%06d", &contact);
 	
		//if (contact<9564 || (contact==9989 && trgid<97) || contact > 19872) continue;
              
		// retrieve geographic info
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact);
                f = new MCVFile3916(infile);
 		int coorok = f->GetCoordinates(&evt);
                delete f;   
		
		if (coorok == 1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact+1);
			f = new MCVFile3916(infile);
			f->GetCoordinates(&evt);
			delete f;   
		}
		
		if (coorok == -1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact-1);
			f = new MCVFile3916(infile);
			f->GetCoordinates(&evt);
			delete f;   
		}
		
		// get time info and calculate local time
		tzero.Set((int) t0, 0, epoch.GetSec(),0);
		auxdate = tzero.GetDate(1,0, &yy, &mm, &dd);
		auxtime = tzero.GetTime(1,0, &hh, &min, &sec);
		lc = fmod(24. + hh + min/60. + sec/3600. + evt.lon*0.06667, 24.);
		
		// check the boost status
		int boost=-1;	// default -1: no boost info available for this contact
		for (int j=0; j<nb; j++) {
			if (contact == cont[j]) {
				boost=0;	// boost info is available
				if (bini[j] <= evt.time && bend[j] >= evt.time) {
					boost = label[j];	// trigger time is inside a boost/eclipse time window
					break;
				}
			}
		}
		
		// check if offline trigger is close to an onboard trigger
		int flag = 0; 	// defult 0: farther than 20ms from an onboard trg
		int id = TMath::BinarySearch(ntrg, t03908, evt.time);
		if (id>=0 && id<ntrg-1 && (evt.time < t03908[id]+0.020 || evt.time > t03908[id+1]-0.020)) flag = 1;
		if (id==ntrg-1 && evt.time < t03908[id]+0.020) flag = 1;
		
		// check if offline trigger is close to an onboard trigger
		int tgflag = 0; 	// defult 0: farther than 20ms from an onboard trg
		int tgfid = TMath::BinarySearch(ntgf, t0tgf, evt.time);
		if (tgfid>=0 && tgfid<ntgf-1 && (evt.time < t0tgf[tgfid]+0.020 || evt.time > t0tgf[tgfid+1]-0.020)) tgflag = 1;
		if (tgfid==ntgf-1 && evt.time < t0tgf[tgfid]+0.020) tgflag = 1;
		
		// write to output file
		fprintf(fp, "%6d \t%4d \t%20.6f \t%6d \t%6.2f \t%10.6f \t%8.2f \t%6.2f \t%10.2f \t%8.2f \t%8.2f \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%06d \t%6.2f \t%6.2f \t%6.2f \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d\n", contact, trgid, evt.time, counts, hr, dt, evt.lon, evt.lat, evt.h, evt.etheta, evt.ephi, yy, mm, dd, hh, min, sec, (int) (fmod(t0,1.)*1.E6), lc, eavg, emax, boost, flag, tgflag, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
		index++;
	}
	fclose(fp);
	
}
	
