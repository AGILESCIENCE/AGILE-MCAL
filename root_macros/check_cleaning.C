{
	// MM 16/11/09 checks if cleaned datasets have non-zero superposition between adjacent contact
	//             based on refine_cleaning.C
	
	gROOT->Reset();
	ifstream fidx;
	//fidx.open("/data/mcal/CLEAN_ARCHIVE.2/clean3908_2.index");
	fidx.open("/home/mcal/data_analysis/AGILE_coordinates/3916.index");
	if (!fidx) {
		cout << "Unable to open index file";
		return 1; // terminate with error
	}
	int contact, outevents, contact0, outevents0, nover=0, nread=1;
	double tini, tfin, tini0, tfin0;
	char ctmp[200];
	
	//fidx >> contact0 >> tini0 >> tfin0 >> outevents0;
	fidx >> ctmp >> contact0 >> tini0 >> tfin0;
	//while (fidx >> contact >> tini >> tfin >> outevents) {
	while (fidx >> ctmp >> contact >> tini >> tfin ) {
		
		if (tini < tfin0) {
			printf("%6d \t%6d \t%20.6f \t%20.6f \t%20.6f\n", nover, contact, tini, tfin0, tfin0 - tini);
			nover++;
		}
		contact0 = contact;
		tini0 = tini;
		tfin0 = tfin;
		outevents0 = outevents;
		nread++;
	}
	cout << "read " << nread << " contacts - found " << nover << " overlapping consecutive contacts" << endl;
	fidx.close();
	return 0;
}
