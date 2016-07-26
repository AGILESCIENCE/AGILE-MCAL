{
	// MM 03/02/09 created based on vsb_analysis_16ms_090109.C
	// dumps data to stdout in a format good for sferic cross-correlations

	struct Trigger_bus_cleaned  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			char type[5];
			int sa;
			int lng;
			int subms;
			int nrm;
			int rmid;
			int sitid;
			float lon;
			float lat;
			float h;
			float dt;
	};
	
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
	TTree tvsb;
	Trigger_bus_cleaned atrg;
	 tvsb.ReadFile("/data1/mcal/data_analysis/VSB/TGF_bus_cleaned_3001-8157.out",	 "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:type/C:sa/I:lng/I:subms/I:nrm/I:rmid/I:sitid/I:lon/F:lat/F:h/F:dt/F");
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("h", &atrg.h);
	tvsb.SetBranchAddress("dt", &atrg.dt);

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
	// ------------ read tmin - tmax info file (to be modified) ----------------
	
	ifstream fin;
	fin.open("/data1/mcal/data_analysis/VSB/grb_3001_9190_tmin-tmax.dat", ifstream::in);
	int ntrg;
	fin >> ntrg;
	cout << "Reading tmin/tmax auxiliary file - number of trigger to be processed = " << ntrg << endl;
	int *runid = new int[ntrg];
	int *trg = new int[ntrg];
	float *mint = new float[ntrg];
	float *maxt = new float[ntrg];
	
	for (int i=0; i<ntrg; i++) {
		fin >> runid[i] >> trg[i] >> mint[i] >> maxt[i];
	}
	
	
	
	// ----------- process entries ---------------
	
	double t0prev = 0.;
	int currid=0;
	
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		if(atrg.lon>180.) atrg.lon = atrg.lon-360.;	// longitude correction
		
		if (atrg.t0 != t0prev && atrg.lon>=-30. && atrg.lon<=0. && atrg.lat<-1.5) {
		
			// format for sferics correlation
			//printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (fmod(atrg.t0,1.)*1.E6), atrg.lon, atrg.lat);	
		
			// format for calibration with activation
			
			for (int j=currid; j<ntrg; j++) {
				if (runid[j] == atrg.runid && trg[j] == atrg.trg && mint[j]>-35. && maxt[j]<35.) {
					printf("%6d \t%d 1. 1. 0 1. \t%10.3f \t%10.3f 0. \t%20.6f \t%10.3f 0. \t%10.3f \n",atrg.runid, atrg.trg, atrg.lon, atrg.lat, atrg.t0, maxt[j]-mint[j], mint[j]);
					currid = j;
					break;
				}
				if (runid[j] > atrg.runid) break;
			}

		}
		t0prev = atrg.t0;
	}
	
	delete runid;
	delete trg;
	delete mint;
	delete maxt;
}