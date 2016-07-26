// MM 15/03/13 retrieve time passages of AGILE above South America for meteo studies with Ferran @UPC

{
	ifstream findex;
	findex.open("/home/mcal/data_analysis/AGILE_coordinates/3916.index", ifstream::in);
	int contact;
	char aux[20], fname[100];
	double tini, tfin;
	int cini = 30209; 
	int cfin = 34535; 

	TFile *f;
	TTree *t;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	TTimeStamp ts;
	UInt_t year, month, day, date;
	UInt_t hour, min, sec, time;

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	gpsevt evt;

	while (findex >> aux >> contact >> tini >> tfin) { // begin loop on orbits
		
		if (contact >= cini && contact <= cfin) { // begin if on orbit range
			sprintf(fname, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact); 
			f = new TFile(fname);
			t = (TTree *) f->Get("tdata3916");
			
			//cout << fname << "  " << t->GetEntries() << endl;

			t->SetBranchAddress("time",&evt.time);
			t->SetBranchAddress("lon",&evt.lon);
			t->SetBranchAddress("lat",&evt.lat);
			t->SetBranchAddress("h",&evt.h);
			t->SetBranchAddress("etheta",&evt.etheta);
			t->SetBranchAddress("ephi",&evt.ephi);
			//t->Scan("time:lon:lat", "lon > -90. && lon < -45.");

			int i=1;
			t->GetEntry(0);
			double lonprev = evt.lon;
			double tin = -1., tout = -1.;
			double lonin, lonout, latin, latout;
			
			while (t->GetEntry(i++)) { // begin loop on tree entries
				if (lonprev < -90. && evt.lon > -90.) {
					tin = evt.time;
					lonin = evt.lon;
					latin = evt.lat;
				}
				if (lonprev < -45. && evt.lon > -45.) {
					tout = evt.time;
					lonout = evt.lon;
					latout = evt.lat;
				}
				lonprev = evt.lon;
				if (tin > 0. && tout > 0.) {
					if (tout - tin > 745. && tout - tin < 775.) { 
						ts.Set((int) tin , 0, epoch.GetSec(),0);	// calculate UT time
						date = ts.GetDate(kTRUE, 0, &year, &month, &day);
						time = ts.GetTime(kTRUE, 0, &hour, &min, &sec);
						printf("%6d  %4d %2d %2d %2d %2d %2d %8.3f %8.3f    ", contact, year, month, day, hour, min, sec, lonin, latin);
                                        
						ts.Set((int) tout , 0, epoch.GetSec(),0);        // calculate UT time
                                        	date = ts.GetDate(kTRUE, 0, &year, &month, &day);
                                        	time = ts.GetTime(kTRUE, 0, &hour, &min, &sec);
                                        	printf("%4d %2d %2d %2d %2d %2d %8.3f %8.3f\n", year, month, day, hour, min, sec, lonout, latout);
					}

				//	printf("%6d  %20.6f %8.3f  %20.6f %8.3f  %10.3f\n", contact, tin, lonin, tout, lonout, tout - tin);
					tin = tout = -1.;
				}
			} // end loop on tree entries
			
			f->Close();
		} // end if on orbit range

	} // end loop on orbits
	findex.close();
}
