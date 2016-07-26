int conv3916toRoot_2(int inicont, int fincont, int eci=0)
{
	// MM 27/10/15 add flag for ECI to lla conversion (to be used for data after Jul 2015)
	// MM 14/04/14 created based on conv3916toRoot.C
        // initial and final contacts as input parameters. Convert within interval (last contact included)

        // MM 02/08/12 load the MCV library directly
	
	// MM 03/11/09 creates an archive of geographyc coordinates for the AGILE contacts
	// must load libMCV.so first
	
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	
	MCVFile3916 *f;
	for (int i=inicont; i<=fincont; i++) {
		char infile[100], outfile[100];
		sprintf(infile, "/AGILE_PROC3/DATA/COR/%06d/PKP%06d_1_3916_000.cor.gz", i, i);
		sprintf(outfile, "/home/mcal/data_analysis/AGILE_coordinates/RT%06d_3916.root", i);
		
		f = new MCVFile3916(infile, 1, eci);
		f->WriteRootFile(outfile);
		delete f;		 
	}
        return 0;
}
