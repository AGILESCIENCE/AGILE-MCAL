{
// MM 11/12/12 remove SAA triggers from root file used for TGF analysis

int nbad = 18; // number of TGFs in SAA to be removed
double badguys[] = {177376065.709123, 182308850.927842, 182810695.148692, 185228045.468579, 202613255.589766, 212005147.634817, 214183117.953267, 214274871.495314, 219095767.673059, 222215585.297793, 239260906.711805, 244140133.162791, 245662293.095846, 248639172.181184, 253160596.256642, 248651448.678909, 245980317.013353, 241303225.000115};
	
	struct offline_trg {
		int contact;
		int id;
		double t0;
		int counts;
		float hr;
		float dt;
		float lon;
		float lat;
		float h;
		float etheta;
		float ephi;
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int usec;
		float loctime;
		float eavg;
		float emax;
		int boost;
		int flag;
		int tgflag;
		int c[8];
	};
	offline_trg atrg;

//	TFile ftrg("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_HE.root");
	TFile *ftrg = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection.root");
	TTree *ttrg = (TTree *) ftrg->Get("triggers");
	
	ttrg->SetBranchAddress("contact",&atrg.contact);
	ttrg->SetBranchAddress("id",&atrg.id);
	ttrg->SetBranchAddress("t0",&atrg.t0);
	ttrg->SetBranchAddress("counts",&atrg.counts);
	ttrg->SetBranchAddress("hr",&atrg.hr);
	ttrg->SetBranchAddress("dt",&atrg.dt);
	ttrg->SetBranchAddress("lon",&atrg.lon);
	ttrg->SetBranchAddress("lat",&atrg.lat);
	ttrg->SetBranchAddress("h",&atrg.h);
	ttrg->SetBranchAddress("etheta",&atrg.etheta);
	ttrg->SetBranchAddress("ephi",&atrg.ephi);
	ttrg->SetBranchAddress("year",&atrg.year);
	ttrg->SetBranchAddress("month",&atrg.month);
	ttrg->SetBranchAddress("day",&atrg.day);
	ttrg->SetBranchAddress("hour",&atrg.hour);
	ttrg->SetBranchAddress("min",&atrg.min);
	ttrg->SetBranchAddress("sec",&atrg.sec);
	ttrg->SetBranchAddress("usec",&atrg.usec);
	ttrg->SetBranchAddress("loctime",&atrg.loctime);
	ttrg->SetBranchAddress("eavg",&atrg.eavg);
	ttrg->SetBranchAddress("emax",&atrg.emax);
	ttrg->SetBranchAddress("boost",&atrg.boost);
	ttrg->SetBranchAddress("flag",&atrg.flag);
	ttrg->SetBranchAddress("tgflag",&atrg.tgflag);
	ttrg->SetBranchAddress("c0",&atrg.c[0]);
	ttrg->SetBranchAddress("c1",&atrg.c[1]);
	ttrg->SetBranchAddress("c2",&atrg.c[2]);
	ttrg->SetBranchAddress("c3",&atrg.c[3]);
	ttrg->SetBranchAddress("c4",&atrg.c[4]);
	ttrg->SetBranchAddress("c5",&atrg.c[5]);
	ttrg->SetBranchAddress("c6",&atrg.c[6]);
	ttrg->SetBranchAddress("c7",&atrg.c[7]);
	int nentries = ttrg->GetEntries();
	ttrg->Print();

	TFile *fnew = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection_new.root","recreate");
	TTree *tnew = ttrg->CloneTree(0);
	
	for (Int_t i=0;i<nentries; i++) {
		ttrg->GetEntry(i);
		cout << "Event " << atrg.contact;
		int isBad = 0;
		for (int j=0; j<nbad; j++) if (atrg.t0 == badguys[j]) isBad = 1;
		if (!isBad) {
			tnew->Fill();
			cout << "  is good" << endl;
		} else cout << "  is BAD" << endl;
		
	}
	tnew->SetName("triggers");
	tnew->Print();
	tnew->AutoSave();
	delete ftrg;
	delete fnew;

}