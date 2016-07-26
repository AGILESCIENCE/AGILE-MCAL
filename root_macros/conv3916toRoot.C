{
	// MM 02/08/12 load the MCV library directly
	
	// MM 03/11/09 creates an archive of geographyc coordinates for the AGILE contacts
	// must load libMCV.so first
	
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	int inicont = 36021;
	int fincont = 36627; // 27 May 2014
	
	MCVFile3916 *f;
	for (int i=inicont; i<fincont; i++) {
		char infile[100], outfile[100];
		sprintf(infile, "/AGILE_PROC3/DATA/COR/%06d/PKP%06d_1_3916_000.cor.gz", i, i);
		sprintf(outfile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", i);
		
		f = new MCVFile3916(infile);
		f->WriteRootFile(outfile);
		delete f;		 
	}
}
