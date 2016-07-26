{
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	ifstream in;
//	in.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Jun08-Sep09.dat");
	in.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Oct09-Mar10.dat");

	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	gpsevt evt;
        
	char infile[200];
	int contact;
	
	while(in >> contact >> evt.time) {
		// retrieve geographic info
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact);
                f = new MCVFile3916(infile, 1);
 		int coorok = f->GetCoordinates(&evt);
                delete f;   
		
		if (coorok == 1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact+1);
			f = new MCVFile3916(infile, 1);
			f->GetCoordinates(&evt);
			delete f;   
		}
		
		if (coorok == -1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", contact-1);
			f = new MCVFile3916(infile, 1);
			f->GetCoordinates(&evt);
			delete f;   
		}
		
		printf("%6d \t%20.6f \t%10.2f \t%6.2f \t%10.2f \t%6.2f \t%6.2f\n", contact, evt.time, evt.lon, evt.lat, evt.h, evt.etheta, evt.ephi);
	}
	
	in.close();
}