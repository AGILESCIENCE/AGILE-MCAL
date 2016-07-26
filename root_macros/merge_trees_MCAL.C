{
	// MM 28/03/2012 merge trees in an orbit interval
	// based on merge_trees.C from Alessandro Berra (FIBERSPAD CERN data analysis software)
	
	int cini = 25000;	// initial contact
	int cfin = 25010;	// final contact (included)
	
	TChain ch("tdata");
	
	char cpath[100];
  	int nfile = 0;
	for (int i=cini; i<=cfin; i++) {
	
		sprintf(cpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", i, i);
		cout << "Adding file: " << cpath << endl;
		ch.Add(cpath);
		nfile++;
		
	}

	char rname[100];
	sprintf(rname,"RTmerged_%06d-%06d.root", cini, cfin);
	ch.Merge(rname);
	cout << "Merged " << nfile << " root files" << endl;
	

}