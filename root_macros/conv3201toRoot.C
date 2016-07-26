{
	// MM 23/11/11 creates an archive of HK info for exposure calculations for the AGILE contacts
	// based on macro conv3916toRoot.C

	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	int inicont = 30483; //9565
	int fincont = 33247;
	
	MCVFile3201 *f;
	for (int i=inicont; i<fincont; i++) {
		char infile[100], outfile[100];
		sprintf(infile, "/AGILE_PROC3/DATA/COR/%06d/PKP%06d_1_3201_000.cor.gz", i, i);
		sprintf(outfile, "/home/mcal/data_analysis/AGILE_HK/RT%06d_3201.root", i);
		
		f = new MCVFile3201(infile);
		f->WriteRootFile(outfile);
		delete f;		 
	}
}
