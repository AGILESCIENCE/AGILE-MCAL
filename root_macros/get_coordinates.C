int get_coordinates(double t0=0., int contact=1000.)
{
	
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
	
	cout << "lon = " << evt.lon << "  lat = " << evt.lat << endl;
}