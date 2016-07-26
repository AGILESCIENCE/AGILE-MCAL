{

#include <string.h>

	// MM 05/11/09 add also time info and local time
	// MM 03/11/09 add geographic info to trigger file produced with macro burst_search.C
	// libMCV.so MUST be loaded first	
	// checks if trigger is in boost (last flag = 1), eclipse (2) or not (0)
	
	gROOT->Reset();
	ifstream inFile;
	inFile.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trg_005000-012477.dat");	// on HP1
	if (!inFile) {
		cout << "Unable to open trigger file";
		exit(1); // terminate with error
	}
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	
	FILE *fp;
	//ofstream outFile;
	fp = fopen("/home/mcal/data_analysis/VSB/offline_trigger_search/trg_005000-012477_processed_3.dat", "a");	// on HP1
	
	// retrieve boost information if available
	ifstream fb;
	fb.open("/AGILE_PROC3/DATA/HLT/MCALBURST/BOOST_up_to_now.dat", ifstream::in);
	int nrow=30000;
	double *bini = new double[nrow];
	double *bend = new double[nrow];
	double *cont = new double[nrow];
	char saux[12];
	int *label = new int[nrow];
	int nb=0;
	while (fb >> cont[nb] >> bini[nb] >> bend[nb] >> saux) {
		if (strncmp(saux,"BOOST",5) == 0) label[nb] = 1;
		else label[nb] = 2;
		nb++;
	}
	cout << "loaded " << nb << " lines" << endl;

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	gpsevt evt;
        MCVFile3916 *f;
	
	int index = 0;
	int sel = 0;
	char aux[10], path[100], infile[100];
	int trgid, counts, contact, auxtime, auxdate;
	double t0, hr, dt, lc;
	UInt_t yy, mm, dd, hh, min, sec;
	TTimeStamp tzero;
	
	while (inFile >> aux >> path >> trgid >> t0 >> counts >> hr >> dt) {
	
		evt.lon = evt.lat = evt.etheta = evt.ephi = evt.h = 0.;
	
		evt.time = t0;
		sscanf(path, "/MCAL_PROC/ARCHIVE/%06d", &contact);
                
		// retrieve geographic info
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact);
                f = new MCVFile3916(infile);
 		f->GetCoordinates(&evt);
		
		// get time info and calculate local time
		tzero.Set((int) t0, 0, epoch.GetSec(),0);
		auxdate = tzero.GetDate(1,0, &yy, &mm, &dd);
		auxtime = tzero.GetTime(1,0, &hh, &min, &sec);
		lc = fmod(24. + hh + min/60. + sec/3600. + evt.lon*0.06667, 24.);
		
		// check the boost status
		int boost=0;
		for (int j=0; j<nb; j++) {
			if (bini[j] <= evt.time && bend[j] >= evt.time) {
				boost = label[j];
				break;
			}
		}
		
		// write to output file
		fprintf(fp, "%6d \t%4d \t%20.6f \t%6d \t%6.2f \t%10.6f \t%8.2f \t%6.2f \t%10.2f \t%8.2f \t%8.2f \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%06d \t%6.2f  \t%4d\n", contact, trgid, evt.time, counts, hr, dt, evt.lon, evt.lat, evt.h, evt.etheta, evt.ephi, yy, mm, dd, hh, min, sec, (int) (fmod(t0,1.)*1.E6), lc, boost);
                delete f;   
		index++;
	}
	fclose(fp);
	
}
	
