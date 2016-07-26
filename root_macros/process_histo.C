process_histo(int contact)
{
	// MM 28/03/2012 
	// builds a histogram collection summing data from different contacts
	// needed to exploit MCVlib analysis routines for offset and energy calibration
	
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");


	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
	};
	
	MCALevt 	inevt;
	int 		nevents;
	int nbefore = 15; // number of cantacts before to include
	int nafter  = 15; // number of cantacts after to include
	int cini = contact - nbefore;	// initial contact
	int cfin = contact + nafter;	// final contact (included)
	
	
	// get file info from first file
	
	MCVFileInfo *info = NULL;
	char cpath[100];
 	sprintf(cpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
	
	// ONLY for validation with contact 11137
	// sprintf(cpath, "~/data_analysis/calibration/GBM-MCAL/offset_test/RT/RT%06d_3908.root", contact);
	
	TFile *fini = new TFile(cpath);
	TIter next = fini->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next())) {
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fini->Get(key->GetName());
	}
	
	// create the chain of input trees
	
	TChain data("tdata");	
 	int nfile = 0;
	for (int i=cini; i<=cfin; i++) {
	
		sprintf(cpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", i, i);
		cout << "Adding file: " << cpath << endl;
		data.Add(cpath);
		nfile++;
	}
		
	// connect the input tree
	data.SetBranchAddress("time", &inevt.time);
	data.SetBranchAddress("mult", &inevt.mult);
	data.SetBranchAddress("bar", inevt.bar);
	data.SetBranchAddress("pdA", inevt.pdA);
	data.SetBranchAddress("pdB", inevt.pdB);
	data.SetBranchAddress("pos", inevt.pos);
	data.SetBranchAddress("E", inevt.E);
	data.SetBranchAddress("Etot", &inevt.Etot);
	nevents = data.GetEntries();
	cout << nevents << " events to be processed in " << nfile << " files" << endl;

	MCVHisto *rh = new MCVHisto();
	
	int i=0;
	int thr1 = 40;
	int thr2 = 10;
	while (data.GetEntry(i++)) {
		if (inevt.mult == 2) { 
		
			// get bar address
			// int ibar1 = 0;
			// while (inevt.bar[ibar] != 0 && ibar <30) ibar1++;
			// int ibar2 = 0;
			// while (inevt.bar[ibar] != 1 && ibar <30) ibar2++;
		
			// selection for offset correction
			// get bar address
			//int ibar = 0;
			//while (inevt.bar[ibar] < 0 && ibar <30) ibar++;
			//if (ibar<30 && ((inevt.pdA[0]>(info->oA[ibar] + thr1) && inevt.pdB[0]<(info->oB[ibar] + thr2)) || (inevt.pdB[0]>(info->oB[ibar] + thr1) && inevt.pdA[0]<(info->oA[ibar] + thr2)))) rh->FillHisto(&inevt);
			
			if (inevt.E[0] > 5. && inevt.E[1] > 5. ) rh->FillHisto(&inevt);
		
		}
	}
	
	char hname[100];
	sprintf(hname,"Hmerged_%06d_%03d-%03d.root", contact, nbefore, nafter);
	TFile *fh = new TFile(hname, "recreate");
	TObjArray *rawHisto = rh->GetRawHisto();
	rawHisto->Write("rawHisto", TObject::kSingleKey);
	info->Write();
	fh->Close(); 


}