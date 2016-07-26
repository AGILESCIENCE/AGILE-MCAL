int burst_search_4(char *inpath="./tmp.root", char *outpath="./output.dat")
{
	// MM 15/10/12 offline trigger search, work on 3908 archive root files
	// based on burst_search_2.C
	// explanations in mcal@agilehp1:~/data_analysis/VSB/offline_trigger_search/README_offline.txt
	// changes: look for high-energy events only! E > 20 MeV, in 300us and 1ms time window, threshold = 3 counts
	// improvement with respect to burst_search_3.C: consider all dataset and NOT ONLY high energy events, so that count statistics is complete
	
	// MM 06/11/09 offline trigger search, work on 3908 archive root files
	// based on burst_search.C
	// explanations in mcal@agilehp1:~/data_analysis/VSB/offline_trigger_search/README_offline.txt
	// changes: lowered thr to >=6 evt/ms, calculates average energy, max energy, topology info (cuboni)

	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
	};
	
	TTree 		*data;		// GRB events data tree
	MCALevt 	inevt;
	int 		nevents;
	int 		cuboni[8];
	
	// offline logic settings
	double dt = 0.001;	// main search time window
	double dt2 = 0.00034;	// max time separation to define a cluster of events
	int thr   = 3;	// 9	// events threshold in dt1
	double ethr = 20.;	// energy threshold in MeV

	// load root events file
	fi= new TFile(inpath);
	if (fi->IsZombie()) { 	// fi is not a valid root file: exit
		cout << inpath << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
		}
		nevents = data->GetEntries();
	}
	
	// connect the input tree
	data->SetBranchAddress("time", &inevt.time);
	data->SetBranchAddress("mult", &inevt.mult);
	data->SetBranchAddress("bar", inevt.bar);
	data->SetBranchAddress("pdA", inevt.pdA);
	data->SetBranchAddress("pdB", inevt.pdB);
	data->SetBranchAddress("pos", inevt.pos);
	data->SetBranchAddress("E", inevt.E);
	data->SetBranchAddress("Etot", &inevt.Etot);

	// cout << nevents << " events" << endl;
	double *time = new double[nevents];
	double *energy = new double[nevents];
	
	// get time and energy arrays
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		time[i]   = inevt.time;
		energy[i] = inevt.Etot;	
	}
	
	int ntrg=0;
	int n, nle, nhe;
	double duration, emax, eavg;
	float hr;
	for (int i=0; i<nevents-thr; i++) {	
	
		// scan reduced time array to find event clusters above threshold
		if (time[i+thr-1] > time[i] && time[i+thr-1] - time[i] <= dt) {
			
			// a cluster is found: initialize
			n = nle = nhe = 0;
			emax = eavg = 0.;
			for (int j=0; j<8; j++)  cuboni[j] = 0;
			
			// go on until consecutive events are closer than dt2
			while (time[i+n+1]-time[i+n] <= dt2) {	// modifica rispetto alla logica subms standard
				if (energy[i+n] >= ethr) nhe++;
				else nle++;
				
				// get average and max energy
				eavg += energy[i+n];
				if (energy[i+n] > emax) emax = energy[i+n];
				
				// fill the "cuboni" (big cubes) array: topology info
				data->GetEntry(i+n);
				for (int k=0; k<30; k++) {
					int id = inevt.bar[k];
					if (id>=0) {
						if (k<8) {	// fill up the "cuboni" (big cubes) histogram
							if (inevt.pos[id]<=0.) cuboni[0]++;
							else cuboni[1]++;
						}
						if (k>=8 && k<15) {
							if (inevt.pos[id]<=0.) cuboni[2]++;
							else cuboni[3]++;
						}
						if (k>=15 && k<23) {
							if (inevt.pos[id]<=0.) cuboni[4]++;
							else cuboni[5]++;
						}
						if (k>=23) {
							if (inevt.pos[id]<=0.) cuboni[6]++;
							else cuboni[7]++;
						}
					}
					if (id == inevt.mult - 1) break;
				}
				
				n++;
			}
			duration = time[i+n-1] - time[i];
			if (n) eavg /= n;
			//hr = (nle > 0 ? ((float) nhe)/nle : +100.);
			hr = nhe; // don't care about HR; look only for the number of HE events
			if (n>=thr && hr>=thr) {
				printf("TRG %s \t%4d \t%20.6f \t%6d \t%6.2f \t%10.6f \t%6.2f \t%6.2f \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d \t%4d\n", inpath, ++ntrg, time[i], n, hr, duration, eavg, emax, cuboni[0], cuboni[1], cuboni[2], cuboni[3], cuboni[4], cuboni[5], cuboni[6], cuboni[7]);
				i += n-1;
			}
		} 
	}
	delete time;
	delete energy;
	return 0;
}