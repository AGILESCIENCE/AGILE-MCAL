int get_coordinates_2(double t0=0., int contact=1000.)
{
	// MM 09/03/2011, based on get_coordinates.C
	// added control on orbit like in add_geographic_info_3.C
	// saves data to file like in GetOrbPhase.C
	// to be used by macro ..... to correct wrong coordinates in file green_candidates_ALL.txt
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	
	gpsevt evt;
        MCVFile3916 *f;

	evt.time = t0;
              
	// retrieve geographic info
	char infile[200];
	sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact);
	f = new MCVFile3916(infile);
	int coorok = f->GetCoordinates(&evt);
	delete f;   
		
	if (coorok == 1) {
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact+1);
		f = new MCVFile3916(infile);
		f->GetCoordinates(&evt);
		delete f;   
	}
	
	if (coorok == -1) {
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact-1);
		f = new MCVFile3916(infile);
		f->GetCoordinates(&evt);
		delete f;   
	}
	
	// cout << "lon = " << evt.lon << "  lat = " << evt.lat << endl;
	
	FILE *fp;
	fp = fopen("./coor.dat", "w");	// on HP1
	fprintf(fp, "%.2f  %.2f", evt.lon, evt.lat);
	fclose(fp);
	
	return 0;
}