{
	// MM 12/02/09 modified based on dump_trigger_data.C, uses the new format for .out file
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
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int rmid;
			int sitid;
			int ber;
			int bet;
			float lon;
			float lat;
			float h;
			float dt;
	};
	
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
	TTree tvsb;
	Trigger_bus_cleaned atrg;
	 tvsb.ReadFile("/data1/mcal/data_analysis/VSB/TGF_bus_flagged_3001-9190_only_GRB.out",	 "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:mint3908/F:maxt3908/F:type/C:sa/I:lng/I:subms/I:flg01ms/I:flg16ms/I:nrm/I:rmid/I:sitid/I:ber/I:bet/I:lon/F:lat/F:h/F:dt");
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("mint3908", &atrg.mint3908);
	tvsb.SetBranchAddress("maxt3908", &atrg.maxt3908);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("flg01ms", &atrg.flg01ms);
	tvsb.SetBranchAddress("flg16ms", &atrg.flg16ms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("ber", &atrg.ber);
	tvsb.SetBranchAddress("bet", &atrg.bet);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("h", &atrg.h);
	tvsb.SetBranchAddress("dt", &atrg.dt);

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
	// ------------ read tmin - tmax info file (to be modified) ----------------
	/*
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
	*/
	
	
	// ----------- process entries ---------------
	
	double t0prev = 0.;
	int currid=0;
	
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		if(atrg.lon>180.) atrg.lon = atrg.lon-360.;	// longitude correction
		
		// if (atrg.t0 != t0prev && atrg.runid>=8158) {	// condition for sferics correlation
		// if (atrg.t0 != t0prev && atrg.lon>=-15. && atrg.lon<=20. && atrg.lat>+2. && atrg.mint3908>-35.) {	// condition for calibration with activation
		if (atrg.nrm>=2 && atrg.sitid==2 && atrg.sa==0) {	// condition for calibration with activation
		
			// format for sferics correlation
			// printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (fmod(atrg.t0,1.)*1.E6), atrg.lon, atrg.lat);	
		
			// format for calibration with activation
			// printf("%6d \t%d 1. 1. 0 1. \t%10.3f \t%10.3f 0. \t%20.6f \t%10.3f 0. \t%10.3f \n",atrg.runid, atrg.trg, atrg.lon, atrg.lat, atrg.t0, atrg.maxt3908 - atrg.mint3908, atrg.mint3908);
		
			// format for checking GRBs and bright TGFs
			printf("%6d \t%d \t%s \t%s \t%20.6f \t%10.2f \t%10.2f \t%d \t%d\n",atrg.runid, atrg.trg, atrg.date, atrg.time, atrg.t0, atrg.lon, atrg.lat, atrg.nrm, atrg.sitid);	

		}
		t0prev = atrg.t0;
	}
}
