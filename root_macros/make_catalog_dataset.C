{
// MM 23/04/13 build reduced dataset for TGF analysis

	int contact, nentries, id;
	double t0, time, dt = 0.1;
	float Etot;
	char fname[100], fnameNew[100];
	TFile *f, *fNew;
	TTree *t, *tNew;
	
	//TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_new.root"); // class A
	//TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_classB_new.root");
	//TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/HE_TGF_Mar09-Dec13.root");
	//TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_newconf_PAPER_2.root");
	//TFile ftrg("./trg_proc4_D_TLE_LF.root");
	//TTree *ttrg = (TTree *) ftrg.Get("triggers");	
	TChain *ttrg = new TChain("triggers");
	//ttrg->AddFile("/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/LE_TGF_newconf4_42220-43188.root"); // dataset B
	//ttrg->AddFile("/home/mcal/BUILD_MCALSW.BUILD15-devel/scripts/LE_TGF_newconf4_43614-44614.root"); // dataset D+
	ttrg->AddFile("/home/mcal/data_analysis/tgf_candidates_no-E-cut.root"); // dataset D+
	ttrg->SetBranchAddress("contact",&contact);
	ttrg->SetBranchAddress("t0",&t0);
	ttrg->SetBranchAddress("id",&id);
	
	int i=0;

	while(ttrg->GetEntry(i++)) {
		//if (contact < 44357) continue;

		printf("%3d \t%06d \t%.6f\n", i, contact, t0);
		sprintf(fname, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
		//sprintf(fname, "/home/mcal/data_analysis/MCAL_PROC2/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
		f = new TFile(fname);
		if (!f) continue;
	
		t = (TTree *) f.Get("tdata");
		t->SetBranchAddress("time", &time);
		t->SetBranchAddress("Etot", &Etot);
		int j=0;
		
		sprintf(fname, "./RT%06d_%d_3908_new.root", contact, id);
		fNew = new TFile(fname, "recreate");
		tNew = new TTree("tdata_new", "tdata_new");
		tNew->Branch("time", &time, "time/D");
		tNew->Branch("Etot", &Etot, "Etot/F");
		while (t->GetEntry(j++)) {
			if (fabs(time - t0) <= dt) {
				//printf("\t%6d \t%.6f \t%.3f\n", j, time - t0, Etot);
				tNew->Fill();
			}
		}
		fNew->Write();
		fNew->ls();
		fNew->Close();
		f->Close();
	}
	
	ftrg.Close();
}
