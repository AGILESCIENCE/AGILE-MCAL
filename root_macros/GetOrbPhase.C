int GetOrbPhase(int contact, double t0) {

	// MM 01/03/2011 ritorna la fase orbitale per un certo T0 dal logfile
	// serve per scremare e classificare i trigger di TGF post processamento
	
	int orbph = -1;
	char fitsname[100];
	char rootname[100];
	sprintf(fitsname, "/AGILE_PROC3/DATA/LOG/%06d.log.gz", contact);
	sprintf(rootname, "./tmp%06d.log.root", contact);
	
	TFTable* table = TFReadTable(fitsname, "BinTable");
	if(table) {
		int nrows = table->GetNumRows();
		TFDoubleCol & time = table->GetColumn("TIME");
		TFShortCol & phase = table->GetColumn("PHASE");
		
		double *tarr = new double[nrows];
		for (int i=0; i<nrows; i++) tarr[i] = time[i];
		
		int id = TMath::BinarySearch(nrows, tarr, t0);
		//cout << id << "  " << time[id] << "  " << phase[id] << endl;
		//cout << id+1 << "  " << time[id+1] << "  " << phase[id+1] << endl;
		orbph = phase[id];
	}
	else
		cout << "File not found" << endl;
	
	FILE *fp;
	fp = fopen("./orbphase.dat", "w");	// on HP1
	fprintf(fp, "%d", orbph);
	fclose(fp);
	
	return orbph;
}
