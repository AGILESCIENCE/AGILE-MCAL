{
	// MM 31/01/13 created to compare MCAL triggers to IPN catalog to update AGILE detections in IPN list
	// based on compare_IPN_MCAL_triggers.C
	

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
	int *auxtrg = new int[400000];
	fin.open("/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive", ifstream::in); //mcal_grb.dat.archive
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.mint3908 >> atrg.maxt3908 >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.flg01ms >> atrg.flg16ms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		if (strcmp(atrg.type, "BOOST")) { // atrg.t0 >= 143085852. && atrg.t0 <= 205770462. && 
			auxt03908[ntrg] = atrg.t0;
			auxcont[ntrg] = atrg.runid;
			auxtrg[ntrg] = atrg.trg;
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
	int *trg = new int[ntrg];
	for (int k=0; k<ntrg; k++) {
		t03908[k] = auxt03908[ind3908[k]];
		cont[k] = auxcont[ind3908[k]];
		trg[k] = auxtrg[ind3908[k]];
	}
	delete ind3908;
	delete auxt03908;
	delete auxcont;
	delete auxtrg;
	
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);  // reference date for TT time: 2004/01/01 00:00:01
	char grbname[20];
	char grbdate[20];
	char grbtime[20];
	int yy, mm, dd, hh, min, sec, nsec, sod, doy;
	char month[5];

	int tthr = 10.; // time separation (sec) to accept a trigger
		
	// load IPN burst list without AGILE detection to check for missing triggers for the MCAL catalog			
	
	ifstream fagileipn;
	string line;
	fagileipn.open("/home/mcal/data_analysis/grb/IPN_catalog/IPN_master_list_check_140103/masterli_NEW3.txt", ifstream::in);
	int nipn = 0, idipn;
	nsec = 0;
	double ttipn[10000];
	char stmp[9], stmp1[20];
	while (fagileipn.good()) {
		getline(fagileipn, line);
		
		if (strncmp(&line[184], "YES", 3) && strncmp(&line[184], "NO", 2)&& strncmp(&line[184], "RI", 2)) { // AGILE flag YES / NO / RI is NOT already set 
			sscanf(&line[7],"%d %s %d %d %s", &dd, month, &yy, &doy, grbtime); // 
			strncpy(stmp, &line[0], 6);
			sscanf(stmp, "%d", &idipn);
		
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
		
			TTimeStamp t0ipn(2000+yy, mm, dd, hh, min, sec, nsec, 1, -epoch->GetSec()); 
			ttipn[nipn] = t0ipn.GetSec();
		
			//cout << nipn << "  " << idipn << "  " << ttipn[nipn] << "  " << t0ipn.GetTime() << " *** "  << line << endl;
	
		
			// search for AGILE triggers corresponding to IPN bursts
		
			int pos = TMath::BinarySearch(ntrg, t03908, ttipn[nipn]);
			
			// search for MCAL triggers within tthr seconds from IPN time
			int inlist = 0;
			if ((ttipn[nipn] - t03908[pos] <= tthr) || (t03908[pos+1] - ttipn[nipn] <= tthr)) inlist = 1;
			
			if (inlist) {
				if (ttipn[nipn] - t03908[pos] <= tthr) { 
					printf("%6d %04d-%02d-%02d %s %06d %20.6f\n", idipn, 2000+yy, mm, dd, grbtime, cont[pos], t03908[pos]);
				} else if (t03908[pos+1] - ttipn[nipn] <= tthr) { 
					printf("%6d %04d-%02d-%02d %s %06d %20.6f\n", idipn, 2000+yy, mm, dd, grbtime, cont[pos+1], t03908[pos+1]);
				}
			}
		
			nipn++; 
		}
		
	}
	fagileipn.close();


}