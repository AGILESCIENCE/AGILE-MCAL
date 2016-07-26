{
	// MM 22/02/10 reads the trigger file for Steve Cummer and calculate lon, lat	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	ifstream in;
	// in.open("/home/mcal/data_analysis/VSB/offline_trigger_search/t0_list_Jun08-Sep09.dat");
	//in.open("/home/mcal/data_analysis/VSB/analysis_0909/tgf_list_Jun08-Nov09.txt");
	//in.open("/home/mcal/AGILE_IDL_Tools/final_list_14829_14845_MM_format.dat");
//	in.open("/home/mcal/AGILE_IDL_Tools/final_list_13974_14419_MM_format.dat");
	in.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/tgf_list_Jun08-Apr10.txt");


	struct gpsevt {
		Double_t	time;
		Double_t	lon;
		Double_t	lat;
		Double_t	h;
		Double_t	etheta;
		Double_t	ephi;
	};
	gpsevt evt[10000];
        
	struct scevt {
		int runid;
		int trg;
		char date[20];
		char time[40];
		float lon;
		float lat;
		float cpeak;
		float counts;
		float hr;
		double t0;
	};
	scevt e[10000];
	
	char infile[200];
	int contact;
	
	int id=0;
	while(in >> e[id].runid >> e[id].trg >> e[id].date >> e[id].time >> e[id].lon >> e[id].lat >> e[id].cpeak >> e[id].counts >> e[id].hr >> e[id].t0) {
		// retrieve geographic info
		evt[id].time = e[id].t0;
		sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", e[id].runid);
                f = new MCVFile3916(infile, 1);
 		int coorok = f->GetCoordinates(&evt[id]);
                delete f;   
		
		if (coorok == 1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", e[id].runid+1);
			f = new MCVFile3916(infile, 1);
			f->GetCoordinates(&evt[id]);
			delete f;   
		}
		
		if (coorok == -1) {
			sprintf(infile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", e[id].runid-1);
			f = new MCVFile3916(infile, 1);
			f->GetCoordinates(&evt[id]);
			delete f;   
		}
		
		
		
		id++;
	}
	
	in.close();
	
	//for (int i=0; i<id; i++) printf("%6d \t%20.6f \t%10.2f \t%6.2f \t%10.2f \t%6.2f  \t%6.2f  \t%6.2f \n", e[i].runid, e[i].t0, e[i].lon, e[i].lat, evt[i].lon, evt[i].lat, evt[i].lon - e[i].lon, evt[i].lat - e[i].lat);
	
	for (int i=0; i<id; i++) printf("%6d \t%3d \t%s \t%s \t%8.2f \t%8.2f \t%3d \t%3d \t%6.2f \t%20.6f \n", e[i].runid, e[i].trg, e[i].date, e[i].time, evt[i].lon, evt[i].lat, e[i].cpeak, e[i].counts, e[i].hr, e[i].t0);
}
