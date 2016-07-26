{
	// MM 17/09/12 created to compare MCAL triggers to GBM catalog to update AGILE detections in IPN list
	

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

	// load trigger file mcal_grb.dat
	
	ifstream fin;
	TriggerBasic atrg;
	double *auxt03908 = new double[400000];
	int *auxcont = new int[400000];
	fin.open("/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive", ifstream::in); //mcal_grb.dat
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.mint3908 >> atrg.maxt3908 >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.flg01ms >> atrg.flg16ms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		if (atrg.t0 >= 284083200. && atrg.t0 <= 315619200. && strcmp(atrg.type, "BOOST")) {
			auxt03908[ntrg] = atrg.t0;
			auxcont[ntrg] = atrg.runid;
			ntrg++;
		}
	}
	fin.close();
	//ntrg--;
	cout << "sorting times..." << endl;
	int *ind3908 = new int[ntrg];
	TMath::Sort(ntrg, auxt03908, ind3908, 0);
	cout << "number of on-board triggers = " << ntrg << endl;
	double *t03908 = new double[ntrg];
	int *cont = new int[ntrg];
	for (int k=0; k<ntrg; k++) {
		t03908[k] = auxt03908[ind3908[k]];
		cont[k] = auxcont[ind3908[k]];
	}
	delete ind3908;
	delete auxt03908;
	delete auxcont;
	
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);  // reference date for TT time: 2004/01/01 00:00:01
	char grbname[20];
	char grbdate[20];
	char grbtime[20];
	int yy, mm, dd, hh, min, sec, nsec;
	char month[5];
		
	// load AGILE IPN master list
	
	ifstream fagileipn;
	fagileipn.open("/home/mcal/data_analysis/grb/IPN-supplement-GBM-catalog/master_list_120810.txt", ifstream::in);
	int nipn = 0;
	nsec = 0;
	double ttipn[10000];
	while (fagileipn >> grbid >> month >> yy >> grbtime) {
		if (strcmp(month, "JAN") == 0) mm = 1;
		else if (strcmp(month, "FEB") == 0) mm = 2;
		else if (strcmp(month, "MAR") == 0) mm = 3;
		else if (strcmp(month, "APR") == 0) mm = 4;
		else if (strcmp(month, "MAY") == 0) mm = 5;
		else if (strcmp(month, "JUN") == 0) mm = 6;
		else if (strcmp(month, "JUL") == 0) mm = 7;
		else if (strcmp(month, "AUG") == 0) mm = 8;
		else if (strcmp(month, "SEP") == 0) mm = 9;
		else if (strcmp(month, "OCT") == 0) mm = 10;
		else if (strcmp(month, "NOV") == 0) mm = 11;
		else if (strcmp(month, "DEC") == 0) mm = 12;
		sscanf(grbtime, "%d:%2d:%2d", &hh, &min, &sec);
	
		TTimeStamp t0ipn(yy, mm, dd, hh, min, sec, nsec, 1, -epoch->GetSec()); 
		ttipn[nipn] = t0ipn.GetSec();
		cout << nipn << "  " << ttipn[nipn] << endl;
		nipn++;
	}
	
	
	
	// load GBM catalog

	
	ifstream fgbm;
	fgbm.open("/home/mcal/data_analysis/grb/IPN-supplement-GBM-catalog/gbmcat.txt", ifstream::in);
	int ngbm = 0;
	int tthr = 10.;
	
	cout << endl << "GBM AND MCAL triggers" << endl << endl;
	
	while (fgbm >> grbname >> grbdate >> grbtime) {
		
		sscanf(grbdate, "%4d-%2d-%2d", &yy, &mm, &dd);
		sscanf(grbtime, "%2d:%2d:%2d.%3d", &hh, &min, &sec, &nsec);
		nsec *= 1000000;
		
		TTimeStamp t0gbm(yy, mm, dd, hh, min, sec, nsec, 1, -epoch->GetSec()); // epoch->GetSec()
		
		int pos = TMath::BinarySearch(ntrg, t03908, (double) t0gbm.GetSec());
		
		// search for MCAL triggers within tthr seconds from trigger time
		int inlist = -1;
		if ((t0gbm.GetSec() - t03908[pos] <= tthr) || (t03908[pos+1] - t0gbm.GetSec() <= tthr)) {
			int ipnpos = TMath::BinarySearch(nipn, ttipn, (double) t0gbm.GetSec());
			if ((t0gbm.GetSec() - ttipn[ipnpos] <= tthr) || (ttipn[ipnpos+1] - t0gbm.GetSec() <= tthr)) inlist = 1;
			else inlist = 0;
		}
		
		// print ONLY events NOT in the IPN master list
		
		if (inlist == 0) { // == 0
			if (t0gbm.GetSec() - t03908[pos] <= tthr) { 
				printf("%s %s %s %06d %20.6f\n", grbname, grbdate, grbtime, cont[pos], t03908[pos]);
			} else if (t03908[pos+1] - t0gbm.GetSec() <= tthr) { 
				printf("%s %s %s %06d %20.6f\n", grbname, grbdate, grbtime, cont[pos+1], t03908[pos+1]);
			}
		}
	}
	fgbm.close();

}