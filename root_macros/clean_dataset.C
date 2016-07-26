int clean_dataset(int contact = 1)
{
	// MM 12/11/09 removes data with the same time, or present in previous files
	// the clean is necessary for univocal trigger search and for exposure generation
	// root file readout based on macro burst_search_2.C
	// help on copying selected entries in tutorial copytree3.C
	
	gROOT->Reset();
  	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
/*	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);  // 2D graph colour version
	//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	int contact = 10000;	// 13149
*/	
	char inpath[200];
	char outpath[200];
	sprintf(inpath,  "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
	sprintf(outpath, "/data/mcal/CLEAN_ARCHIVE/RT%06d_3908_clean.root", contact);

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
	
	TTree 		*data;		// GRB events data tree
	MCALevt 	inevt;
	MCVFileInfo 	*info, *outinfo;
	int 		nevents;

	// load input root events file
	TFile *fi= new TFile(inpath);
	if (fi->IsZombie()) { 	// fi is not a valid root file: exit
		cout << inpath << " is not a valid ROOT file: exiting!" << endl;
		return -1;
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
	data->GetEntry(0);
	double t0 = inevt.time;

	// build index (TTree::BuildIndex seems not to work properly)	
	double *time = new double[nevents];
	//double *energy = new double[nevents];
	int *idx = new int[nevents];
	int *idx2 = new int[nevents];
	cout << "reading... ";
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		time[i]   = inevt.time;
		//energy[i]   = inevt.Etot;
	}
	cout << "sorting... ";
	TMath::Sort(nevents, time, idx, 0);
	cout << "sorted..." << endl;
	FILE *fidx;
	fidx = fopen("/data/mcal/CLEAN_ARCHIVE/clean3908.index", "a");
	
	int n=0;
	idx2[0] = idx[0];
	double t0prev = time[idx[0]];
	for (int i=0; i<nevents; i++) {
		//printf("%6d \t%6d \t%20.6f\n", i, idx[i], time[idx[i]]);
		if (time[idx[i]] == t0prev) {
			if (idx[i] < idx2[n]) idx2[n] = idx[i];
		} else {
			idx2[++n] = idx[i];
			t0prev = time[idx[i]];
		}
	}
	
	//Create a new file + a clone of old tree in new file
	TFile *fo = new TFile(outpath,"recreate");
	TTree *dataout = data->CloneTree(0);
	
	int istart=0;
	for (int i=1; i<=n; i++) {
		if (idx2[i] != idx2[i-1]+1) {
			//printf("%6d \t%6d \t%20.6f\n", i, idx2[i], time[idx2[i]]);
			for (int j=istart; j<i; j++) {
				data->GetEntry(idx2[j]);
				dataout->Fill();
			}
			istart = i;
		}
	}
	for (int j=istart; j<=n; j++) {
		data->GetEntry(idx2[j]);
		dataout->Fill();
	}
	
	// questo algoritmo e' lentissimo!
/*	for (int i=0; i<nevents; i++) {
		data->GetEntry(idx[i]);	// idx[i]
		if (i%10000 == 0) cout << i << endl;
		if (inevt.time > t0prev) {
			dataout->Fill();
			t0prev = inevt.time;
		} else continue;
	}*/
	
	int outevents = dataout->GetEntries();
	cout << "contact " << contact << "  -  old tree entries = " << nevents << "  -  cleaned tree entries = " << outevents << endl;
	dataout->GetEntry(0);
	double tini = inevt.time;
	dataout->GetEntry(outevents-1);
	double tfin = inevt.time;
	fprintf(fidx, "%6d \t%20.6f \t%20.6f \t%10d\n", contact, tini, tfin, outevents);
	outinfo->nevents = outevents;		
	outinfo->tstart = tini;	 
	outinfo->tstop = tfin;	
	outinfo->ttot = tfin - tini;	
	
	// draw section (debug)
/*	TCanvas c1;
	data->Draw("time:Entry$", "Entry$ % 10 == 0");
	TCanvas c2;
	dataout->Draw("time:Entry$", "Entry$ % 10 == 0");
*/	
	//dataout->Print();
	
	outinfo->Write();
	dataout->Write();
	delete fi;
	delete fo;
	delete time;
	delete idx;
	delete idx2;
	fclose(fidx);
	return 0;
}
	
