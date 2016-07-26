int grid_tgf(int contact =7467, double t0=150112496.460176, double deltat=0.10)
{
	// MM 05/11/10 modified to work only on 3901 data. only time comparison. all subsequent analysis left to successive macros
	// MM 01/02/10 gets info also from the evt files
	// MM 25/01/10 returns all the events within a defined time interval with respect to t0 (and not only the closest)
	//             this macro MUST work in astroroot
	// MM 08/01/10 returns the closest event to the input time

	// t0 += 1.;	// to be uncommented to check with uncorrelated dataset
	
	int getevt = 1;
	
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
	
	struct GRIDevt {
		Double_t	time;
		float		energy;
		float		theta;
		float		phi;
		float		RA;
		float		Dec;
		float		ph_earth; 
		char		brname03;
		char		evstatus;
	};
	
	TTree 		*data;		// GRB events data tree
	MCALevt 	inevt;
	int 		nevents;
	
	char inpath[200];
//	sprintf(inpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3901.root", contact, contact);
	sprintf(inpath, "/home/mcal/data_analysis/VSB/grid/data/rootfiles/3901/RT%06d_3901.root", contact);	
	
	// load root events file
	TFile *fi= new TFile(inpath);
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
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		time[i]   = inevt.time;
	}
	
	// load the evt file as a root tree
/*	char evtfile[200];
	GRIDevt gevt;
	sprintf(evtfile, "/home/mcal/data_analysis/VSB/grid/data/rootfiles/evt/%06d.evt.gz.root", contact);
	TFile *fevt = new TFile(evtfile);
	TTree *tevt = (TTree *) fevt->Get("EVENTS");
	// tevt->Print();
	int nevt = tevt->GetEntries();
	tevt->SetBranchAddress("TIME", &gevt.time);
	tevt->SetBranchAddress("ENERGY", &gevt.energy);
	tevt->SetBranchAddress("THETA", &gevt.theta);
	tevt->SetBranchAddress("PHI", &gevt.phi);
	tevt->SetBranchAddress("RA", &gevt.RA);
	tevt->SetBranchAddress("DEC", &gevt.Dec);
	tevt->SetBranchAddress("PH_EARTH", &gevt.ph_earth);
	tevt->SetBranchAddress("BRNAME03", &gevt.brname03);
	tevt->SetBranchAddress("EVSTATUS", &gevt.evstatus);
	
	double *gtime = new double[nevt];
	for (int i=0; i< nevt; i++) {
		tevt->GetEntry(i);
		gtime[i] = gevt.time;
	}
*/	
	int id = TMath::BinarySearch(nevents, time, t0-deltat);
	int n=id;
	while (time[n] <= t0+deltat) {
		double dt = time[n] - t0;
		if (time[n] >= t0-deltat) {
			printf("GRID %6d \t%20.6f \t%8d \t%20.6f \t%20.6f \n", contact, t0, n, time[n], dt);
	
			// get info from evt file
		/*	int gid = TMath::BinarySearch(nevt, gtime, time[n]);
			if (gtime[gid] == time[n]) {
				tevt->GetEntry(gid);
				printf("\t%8d \t%6.0f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%c \t%c\n", gid, gevt.energy, gevt.theta, gevt.phi, gevt.RA, gevt.Dec, gevt.ph_earth, gevt.brname03, gevt.evstatus);
			} else printf("\t%8d \t%6.0f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%c \t%c\n", -1., -1., -1., -1., -1., -1., -1., 'N', 'N');
			*/	
		}
		
		n++;
	}
	
	//get the closest event
	/*int id = TMath::BinarySearch(nevents, time, t0);
	double dt = time[id] - t0;
	double dt2 = time[id+1] - t0;
	if (dt2 < -dt) {
		dt = dt2;
		id++;
	}
	printf("GRID %6d \t%20.6f \t%8d \t%20.6f \t%20.6f \n", contact, t0, id, time[id], dt);
	*/
	
	
	delete time;
	//delete gtime;
	//delete tevt;
	//fevt->Close();
	fi->Close();
	return 0;
}