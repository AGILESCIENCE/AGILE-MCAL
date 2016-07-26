void GetOrbPhase_file(int cini=-1, int cfin=100000) {

	// *** NON RIESCO A FARLO ANDARE MENTRE GetOrbPhase.C FUNZIONA.
	// *** DA ERRORE QUANDO CHIAMA TFDoubleCol &...

	// MM 01/03/2011 based on GetOrbPhase.C
	// open Fullekrug list, copy only triggers with HR>0.5 and add orbital phase retrieved from logfiles
	
	ifstream infile;
	infile.open("/dischi/gtb/data1/AGILE_MCAL2/mcal_trg_complete.dat", ifstream::in);	// on HP1
	int contact, trg, npeak, nevt;
	char date[100], hour[100];
	float lon, lat, hr, t0;
	//double t0;
	
	while (infile >> contact >> trg >> date >> hour >> lon >> lat >> npeak >> nevt >> hr >> t0) {
		int orbph = -1;
		
		if (contact >= cini && contact <= cfin && hr >= 0.5 && nevt >= 10) {
			char fitsname[100];
			char rootname[100];
			sprintf(fitsname, "/AGILE_PROC3/DATA/LOG/%06d.log.gz", contact);
			
			TFTable* table = TFReadTable(fitsname, "BinTable");
			if(table) {
				int nrows = table->GetNumRows();
				cout << nrows << endl;
				TFDoubleCol &time = table->GetColumn("TIME");
/* 				TFShortCol & phase = table->GetColumn("PHASE");
				
				double *tarr = new double[nrows];
				for (int i=0; i<nrows; i++) tarr[i] = time[i];
				
				int id = TMath::BinarySearch(nrows, tarr, t0);
				cout << id << "  " << time[id] << "  " << phase[id] << endl;
				cout << id+1 << "  " << time[id+1] << "  " << phase[id+1] << endl;
				orbph = phase[id];*/
			}
			else {
				cout << "Log file " << fitsname << " for contact " << contact << " not found" << endl;
			} 
		
			printf("%6d \t%d \t%s \t%s \t%10.2f \t%10.2f \t%d \t%d \t%10.2f \t%20.6f \t%d\n",contact, trg, date, hour, lon, lat, npeak, nevt, hr, t0, orbph);
			
			//delete tarr;
			delete table;
		}
	
	}
	infile.close();
	

}
