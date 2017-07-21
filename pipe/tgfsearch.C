// $ root -b -q 'tgfsearch.C(50315, "/data01/ASDC_PROC2/DATA2/COR", 0.001, 0.0003, 9)'

int tgfsearch(int rootname, char *rootpath, double dt, double dt2, int thr)
{

printf("\n\n\nSTAMPA\n\n\n%d %s\n\n\n", rootname, rootpath);

FILE *list;

	// open and read 3908 root file
	gROOT->Reset();
	gSystem->Load("/opt/prod/mcalsw-new/lib/libMCV.so");

	char infile3908[100];

	sprintf(infile3908, "%s/RT%06d_3908.root", rootpath, rootname);

	struct MCALevt {
		Double_t	time3908;
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
	
	TTree 		*data3908;
	MCALevt 	inevt;
	int 		nevents3908;
	int 		cuboni[8];

	// load root events file
	fi= new TFile(infile3908);
	if (fi->IsZombie()) { 	// fi is not a valid root file: exit
		cout << infile3908 << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data3908 = (TTree *) fi->Get(key->GetName());
		}
		nevents3908 = data3908->GetEntries();
	}
	
	// connect the input tree
	data3908->SetBranchAddress("time", &inevt.time3908);
	data3908->SetBranchAddress("mult", &inevt.mult);
	data3908->SetBranchAddress("bar", inevt.bar);
	data3908->SetBranchAddress("pdA", inevt.pdA);
	data3908->SetBranchAddress("pdB", inevt.pdB);
	data3908->SetBranchAddress("pos", inevt.pos);
	data3908->SetBranchAddress("E", inevt.E);
	data3908->SetBranchAddress("Etot", &inevt.Etot);

	// cout << nevents << " events" << endl;
	double *time3908 = new double[nevents3908];
	double *energy = new double[nevents3908];

	// get time and energy arrays
	for (int i=0; i<nevents3908; i++) {
		data3908->GetEntry(i);
		time3908[i]   = inevt.time3908;
		energy[i] = inevt.Etot;
	}
	
	int ntrg=0;
	int n, nle, nhe;
	double duration, emax, eavg;
	float hr;

	for (int i=0; i<nevents3908-thr; i++) {	
	
		// scan reduced time array to find event clusters above threshold
		if (time3908[i+thr-1] > time3908[i] && time3908[i+thr-1] - time3908[i] <= dt) {
			
			// a cluster is found: initialize
			n = nle = nhe = 0;
			emax = eavg = 0.;
			for (int j=0; j<8; j++)  cuboni[j] = 0;
			
			// go on until consecutive events are closer than dt2
			while (time3908[i+n+1]-time3908[i+n] <= dt2) {	// modifica rispetto alla logica subms standard
				if (energy[i+n] >= 1.4) nhe++;
				else nle++;
				
				// get average and max energy
				eavg += energy[i+n];
				if (energy[i+n] > emax) emax = energy[i+n];
				
				// fill the "cuboni" (big cubes) array: topology info
				data3908->GetEntry(i+n);
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

			duration = time3908[i+n-1] - time3908[i];

			if (n) eavg /= n;
			hr = (nle > 0 ? ((float) nhe)/nle : +100.);

			if (n>thr && hr > 0.5 && emax <= 30. && (cuboni[0] + cuboni[2]) > 0 && (cuboni[1] + cuboni[3]) > 0 && (cuboni[4] + cuboni[6]) > 0 && (cuboni[5] + cuboni[7]) > 0) {

				// open and read 3916 root file
				gSystem->Load("/opt/prod/mcalsw-new/lib/libMCV.so");

				char infile3916[100];

				struct gpsevt {
					Double_t	time3916;
					Double_t	lon;
					Double_t	lat;
					Double_t	h;
					Double_t	etheta;
					Double_t	ephi;
				};

				gpsevt evt;

				TTree 		*data3916;
				int 		nevents3916;
				
				// load root events file
				sprintf(infile3916, "%s/RT%06d_3916.root", rootpath, rootname);
				fi= new TFile(infile3916);
					if (fi->IsZombie()) { 	// fi is not a valid root file: exit
							cout << infile3916 << " is not a valid ROOT file: exiting!" << endl;
							return 1;
					} else {		// get data and file info
							TIter next = fi->GetListOfKeys();
							TKey *key;
					
				while ((key = (TKey *)next())) {
				if (strncmp(key->GetName(),"tdata", 5) == 0) data3916 = (TTree *) fi->Get(key->GetName());
				}
				nevents3916 = data3916->GetEntries();
				}

				// connect the input tree
				data3916->SetBranchAddress("time", &evt.time3916);
				data3916->SetBranchAddress("lon", &evt.lon);
				data3916->SetBranchAddress("lat", &evt.lat);
				data3916->SetBranchAddress("h", &evt.h);
				data3916->SetBranchAddress("etheta", &evt.etheta);
				data3916->SetBranchAddress("ephi", &evt.ephi);

				double *time3916 = new double[nevents3916];
				double *lon  = new double[nevents3916];
				double *lat  = new double[nevents3916];
				double LON, LAT;

				double t0, lc;
				UInt_t YYYY, MM, DD, hh, mm, ss;
				TTimeStamp tzero;
				int auxtime, auxdate;

				// get geographic coordinates
				for (int k=0; k<nevents3916; k++) {
					data3916->GetEntry(k);
					time3916[k] = evt.time3916;
					lon[k]  = evt.lon;
					lat[k]  = evt.lat;
						
					if (time3916[k]-time3908[i] <= 8. && time3916[k]-time3908[i] >= 0.) {
								
								LON = lon[k];
								LAT = lat[k];
								// get time info and calculate local time
								TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);
								tzero.Set((int) time3908[i], 0, epoch.GetSec(),0);
								auxdate = tzero.GetDate(1,0, &YYYY, &MM, &DD);
								auxtime = tzero.GetTime(1,0, &hh, &mm, &ss);
								lc = fmod(24. + hh + mm/60. + ss/3600. + LON*0.06667, 24.);

								}
				
				}

				printf("TGF %06d\t %9.6f\t %04d-%02d-%02d %02d:%02d:%02d\t %3.2f\t %6d\t %6.2f\t %10.6f\t %6.2f\t %6.2f\t %3.2f\t %3.2f\n", rootname, time3908[i], YYYY, MM, DD, hh, mm, ss, lc, n, hr, duration, emax, eavg, LON, LAT, lc);

				i += n;
			}
		} 
	}

	return 0;

}
