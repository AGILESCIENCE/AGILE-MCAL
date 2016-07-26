int burst_search(char *inpath="./tmp.root", char *outpath="./output.dat")
{

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
	
	double dt = 0.001;
	double dt2 = 0.0003;
	int thr   = 9;

	// load root events file
	// cout << "GRB3908::GRB3908() Opening rootfile " << inpath << endl;
	fi= new TFile(inpath);
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << inpath << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {
	
		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
			//if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
		}
		//if (info) info->PrintFileInfo();
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
	
	for (int i=0; i<nevents; i++) {		// get a subset of the time array around t0
		data->GetEntry(i);
		time[i]   = inevt.time;
		energy[i] = inevt.Etot;	
	}
	
	int ntrg=0;
	int n, nle, nhe;
	double duration;
	float hr;
	for (int i=0; i<nevents-thr; i++) {	// scan reduced time array to find event clusters above threshold
		if (time[i+thr-1] > time[i] && time[i+thr-1] - time[i] <= dt) {
			n = nle = nhe = 0;
			while (time[i+n+1]-time[i+n] <= dt2) {	// modifica rispetto alla logica subms standard
				if (energy[i+n] >= 1.4) nhe++;
				else nle++;
				n++;
			}
			duration = time[i+n] - time[i];
			hr = (nle > 0 ? ((float) nhe)/nle : +100.);
			if (n>=thr && hr>=0.2) {
				printf("TRG %s \t%4d \t%20.6f \t%6d \t%6.2f \t%10.6f\n", inpath, ++ntrg, time[i], n, hr, duration);
				i += n;
			}
		} 
	}
	delete time;
	delete energy;
	return 0;
}