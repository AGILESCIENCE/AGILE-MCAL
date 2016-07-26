int make_exposure(int cini=1, int cfin=1, float dt=1., char *outpath = "/data/mcal/CLEAN_ARCHIVE/expo_1sec.root")
{
	// MM 14/11/09 macro make exposure
	// builds exposure dataset/histograms for analysis of 3908 data
	// based on macros burst_search_2.C, plot_triggers_4.C, add_geographic_info_3.C
	// work between contacts cini-cfin
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	struct expo {
		int contact;
		int id;
		double time;	// t_start of the time bin
		float rate;	// counts/sec in the dt interval 
		float dt;	// delta_t chosen for exposure calculation
		float lon;	// longitude at center of time interval t = t0+0.5*dt
		float lat;	// latitude at center of time interval t = t0+0.5*dt
		float h;	// height at center of time interval t = t0+0.5*dt
		float etheta;	// Earth theta at center of time interval t = t0+0.5*dt
		float ephi;	// Earth phi at center of time interval t = t0+0.5*dt
		float loctime;	// local time at center of time interval t = t0+0.5*dt
	};

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

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	
	MCALevt inevt;
	gpsevt  evt;
 	expo    xevt;
	MCVFile3916 *f1, *f2, *f3;
	
	int nevents, nbins, coorok, auxtime, id=0;
	double tini, tfin;
	char inpath[200];
	char infile1[200], infile2[200], infile3[200];
	
	TKey *key;
	TTree *data;		// 3908 events data tree
	UInt_t hh, min, sec;
	TTimeStamp tzero;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	
	// create exposure tree and connect structure
	TTree texpo;
	texpo.Branch("contact",&xevt.contact, "contact/I");
	texpo.Branch("id",&xevt.id, "id/I");
	texpo.Branch("time",&xevt.time, "time/D");
	texpo.Branch("rate",&xevt.rate, "rate/F");
	texpo.Branch("dt",&xevt.dt, "dt/F");
	texpo.Branch("lon",&xevt.lon, "lon/F");
	texpo.Branch("lat",&xevt.lat, "lat/F");
	texpo.Branch("h",&xevt.h, "h/F");
	texpo.Branch("etheta",&xevt.etheta, "etheta/F");
	texpo.Branch("ephi",&xevt.ephi, "ephi/F");
	texpo.Branch("loctime",&xevt.loctime, "loctime/F");
	
	// main loop: loop over contact range
	for (int contact=cini; contact<=cfin; contact++) {

		// load root clean events file
		sprintf(inpath, "/data/mcal/CLEAN_ARCHIVE/RT%06d_3908_clean.root", contact);
		fi= new TFile(inpath);
		if (fi->IsZombie()) { 	// fi is not a valid root file: exit
			cout << inpath << " is not a valid ROOT file: exiting!" << endl;
			continue;
		} else {		// get data and file info
			TIter next = fi->GetListOfKeys();
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
	
		data->GetEntry(0);
		tini = inevt.time;
		data->GetEntry(nevents-1);
		tfin = inevt.time;
		nbins = (int) ((tfin-tini)/dt + 1.);
		TH1F htime("htime", "htime", nbins, tini, tini+nbins*dt); 
		
		printf("Making exposure for contact %6d - tini = \t%20.6f - tfin = \t%20.6f - nbins = \t%8d - nevents = \t%10d\n", contact, tini, tfin, nbins, nevents);
		
		// fill in the time histogram
		for (int i=0; i<nevents; i++) {
			data->GetEntry(i);
			htime.Fill(inevt.time);
		}

		// open GPS files
		sprintf(infile1, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact-1);
		f1 = new MCVFile3916(infile1);
		sprintf(infile2, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact);
		f2 = new MCVFile3916(infile2);
		sprintf(infile3, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact+1);
		f3 = new MCVFile3916(infile3);
		
		// scan the time hist and fill the exposure tree
		for (int j=1; j<=nbins; j++) {
			
			xevt.rate = htime.GetBinContent(j) / dt;
			
			if (xevt.rate > 0.) {	// work only if the time bin is not empty
			
				xevt.contact = i;
				xevt.id = id;
				xevt.time = htime.GetBinLowEdge(j) + 0.5*dt;
				xevt.dt = dt;
					
				// retrieve geographic info
				evt.time = xevt.time;
				coorok = f2->GetCoordinates(&evt);
				if (!coorok) {
					if (coorok == 1)  f3->GetCoordinates(&evt);
					else if (coorok == -1) f1->GetCoordinates(&evt);
				}
				xevt.lon = evt.lon;
				xevt.lat = evt.lat;
				xevt.h = evt.h;
				xevt.etheta = evt.etheta;
				xevt.ephi = evt.ephi;
				
				// get time info and calculate local time
				tzero.Set((int) xevt.time, 0, epoch.GetSec(),0);
				auxtime = tzero.GetTime(1,0, &hh, &min, &sec);
				xevt.loctime = fmod(24. + hh + min/60. + sec/3600. + evt.lon*0.06667, 24.);
			
				if (id%1 == 0) printf("\t%6d \t%10d \t%20.6f \t%7.2d \t%6.2f \t%8.2f \t%6.2f \t%10.2f \t%8.2f \t%8.2f \t%6.2f\n", xevt.contact, xevt.id, xevt.time, xevt.rate, xevt.dt, xevt.lon, xevt.lat, xevt.h, xevt.etheta, xevt.ephi, xevt.loctime);				
				
				// fill the exposure tree
				texpo.Fill();
				id++;
			}
		}
		
		// clean up memory
		delete data;
		delete fi;
		delete f1;
		delete f2;
		delete f3;
		cout << "... memory cleaned" << endl;
	}
	
	// save exposure tree to file
	cout << "Exposure tree: number of entries = " << texpo.GetEntries() << endl;
	TFile fout(outpath,"recreate");
	texpo.Write();
	// delete fout;
	return 0;
}
