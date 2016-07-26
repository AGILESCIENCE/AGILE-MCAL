{
	// MM 12/10/09 created from tgf_candidates_analysis_4.C
	// reads the RHESSI TGF catalog and dumps info needed for correlation with ATDnet data
	
	struct RHESSI_tgf {	// based on the RHESSI TGF catalog available here: http://scipp.ucsc.edu/~dsmith/tgflib_public/
		char date[20];
		char start[20];
		char path[30];
		float lat;
		float lon;
		float alt;	// km
		int decim;
		int decim_chan;
		float loctime;
		float earthangle;
	};

	
	RHESSI_tgf rtgf[2000];
 // 	ifstream frhessi("/home/mcal/data_analysis/VSB/RHESSI-TGF-catalog/tgflist_before-Jan-06.txt", ios_base::in);
  	ifstream frhessi("/home/mcal/data_analysis/VSB/RHESSI-TGF-catalog/tgflist.txt", ios_base::in);
	int nrh=0;
	int nsel=0;
	frhessi.clear();
	while (!frhessi.eof()) {
		frhessi >> rtgf[nrh].date >> rtgf[nrh].start >> rtgf[nrh].path >> rtgf[nrh].lat >> rtgf[nrh].lon >> rtgf[nrh].alt >> rtgf[nrh].decim >> rtgf[nrh].decim_chan >> rtgf[nrh].loctime >> rtgf[nrh].earthangle;
		if(rtgf[nrh].lon>180.) rtgf[nrh].lon = rtgf[nrh].lon-360.;	// longitude correction
		// cout << nrh << "  " << rtgf[nrh].date << "  "  << rtgf[nrh].lat << "  " << rtgf[nrh].lon << endl;
		
		int yy, mm, dd;
		char stmp[30];
		sscanf(rtgf[nrh].path, "%2d%2d%2d%s", &yy, &mm, &dd, stmp);

		//if (rtgf[nrh].lat<=2.5 && rtgf[nrh].lat>=-2.5 ) {	// && (mm<=3 || mm>=6)
		if (yy>=8 ) {	// && (mm<=3 || mm>=6)
			nsel++;
			
			// format for sferics (ATDnet) correlation
			printf("%6d \t%d \t%4d-%02d-%02d \t%s \t%10.2f \t%10.2f \t%10.0f \t%10.0f \t%10.2f \t%20.6f\n", nsel, 1, 2000+yy, mm, dd, rtgf[nrh].start, rtgf[nrh].lon, rtgf[nrh].lat, 1, 1, 1., 1.);
				
		}
		
		nrh++;
	}
	
			
}
