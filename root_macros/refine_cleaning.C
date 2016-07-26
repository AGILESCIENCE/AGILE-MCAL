//int refine_cleaning()
{
	// MM 16/11/09 checks if cleaned datasets have non-zero superposition between adjacent contact
	//             in case, removes duplicated entries
	//             Works on datasets cleaned using macro clean_dataset
	
	gROOT->Reset();
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
	
	char inpath[200];
	char outpath[200];
	
	// input index file
	ifstream fidx;
	// fidx.open("/data/mcal/CLEAN_ARCHIVE/clean3908.index");
	fidx.open("/data/mcal/CLEAN_ARCHIVE/tmp.index");
	if (!fidx) {
		cout << "Unable to open index file";
		return 1; // terminate with error
	}
	
	// output index file
	FILE *fidx2;
	fidx2 = fopen("/data/mcal/CLEAN_ARCHIVE_2/clean3908_2.index", "a");
	
	int contact, outevents, contact0, outevents0, nover=0, nread=1;
	double tini, tfin, tini0, tfin0=-1.;
	TTree 		*data;		// GRB events data tree
	MCVFileInfo 	*info, *outinfo;
	MCALevt 	inevt;
	int 		nevents;
	
	while (fidx >> contact >> tini >> tfin >> outevents) {
		
		sprintf(inpath,  "/data/mcal/CLEAN_ARCHIVE/RT%06d_3908_clean.root", contact);
		sprintf(outpath, "/data/mcal/CLEAN_ARCHIVE_2/RT%06d_3908_clean_2.root", contact);
		
		// load input file
		TFile *fi= new TFile(inpath);
		if (fi->IsZombie()) { 	// fi is not a valid root file: exit
			cout << inpath << " is not a valid ROOT file: exiting!" << endl;
			continue;
		} else {		// get data and file info
			TIter next = fi->GetListOfKeys();
			TKey *key;
			while ((key = (TKey *)next())) {
				if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
				if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
			}
			nevents = data->GetEntries();
		}
		outinfo = (MCVFileInfo *) info->Clone();
	
		// connect the input tree
		data->SetBranchAddress("time", &inevt.time);
		data->SetBranchAddress("mult", &inevt.mult);
		data->SetBranchAddress("bar", inevt.bar);
		data->SetBranchAddress("pdA", inevt.pdA);
		data->SetBranchAddress("pdB", inevt.pdB);
		data->SetBranchAddress("pos", inevt.pos);
		data->SetBranchAddress("E", inevt.E);
		data->SetBranchAddress("Etot", &inevt.Etot);
		
		
		//Create a new file + a clone of old tree in new file
		TFile *fo = new TFile(outpath,"recreate");
		TTree *dataout = data->CopyTree("time>0.");
		
		int outevents = dataout->GetEntries();
		dataout->GetEntry(0);
		double outtini = inevt.time;
		dataout->GetEntry(outevents-1);
		double outtfin = inevt.time;
		outinfo->nevents = outevents;		
		outinfo->tstart = outtini;	 
		outinfo->tstop = outtfin;	
		outinfo->ttot = outtfin - outtini;	
		
		fprintf(fidx2, "%6d \t%20.6f \t%20.6f \t%10d\n", contact, outtini, outtfin, outevents);
		
		if (tini < tfin0) {
			printf("%6d \t%6d \t%20.6f \t%20.6f \t%20.6f\n", nover, contact, tini, tfin0, tfin0 - tini);
			nover++;
		}
		
		outinfo->Write();
		dataout->Write();
		delete fi;
		delete fo;
		
		contact0 = contact;
		tini0 = tini;
		tfin0 = tfin;
		outevents0 = outevents;
		nread++;
	}
	
	cout << "read " << nread << " contacts - found " << nover << " overlapping consecutive contacts" << endl;
	
	fidx.close();
	fclose(fidx2);
	
	return 0;
}
