{
	// MM 30/01/2012 plots 2D MCAL events from an input file
	// created to plot high energy TGF events
	// based on plot_MCAL_event_2.C

	// MM 3/12/09 plots 2D successive MCAL events
	// based on plot_MCAL_event.C created during May 2007 commissioning!	
	// NB MUST work onrdered root file placed in /data/mcal/CLEAN_ARCHIVE/
	
	// event structure (from MCVLib MCVFile.h)
	/*
	12123	178606890.947791	0.000424
	*/
	gROOT->Reset();
	int contact =10250	; //10035
	double t0   =167150762.697472; //165830024.913240;
	double dt   =	0.003;
	float energy = 0.;
	char label[20];
	
	TTree tEventList;
	tEventList.ReadFile("/data1/mcal/data_analysis/VSB/tgf_candidates_complete/7/HE/photon_list/sp7_HE_event_list.dat", "label/C:contact/I:t0/D:energy/F");
	tEventList.SetBranchAddress("label", &label);
	tEventList.SetBranchAddress("contact", &contact);
	tEventList.SetBranchAddress("t0", &t0);
	tEventList.SetBranchAddress("energy", &energy);

	int nentries = tEventList.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
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
	
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");

	double XREF = -24.2;
	double ZREF = -21.6;
	double X1Z = -18.11;
	double Z1X = -18.11;
	double BARSTEP = 2.587;
	double PSIGMA = 1.;
	double BARLEN  =   37.5;

	int nrow = 3;
	int ncol = 4;
	int npad = nrow*ncol;
	int ic=0;
	TCanvas *c4;
	TH2F *hsX;
	int nevt = 0;

		
	// loop on events
	
	int ievt=0;
	while (tEventList.GetEntry(ievt++)) {
	
		if (energy < 30.) continue;
			
		if (nevt%npad == 0) {	// create a new canvas if needed
			TString cname("c");
			cname += ic;
			c4 = new TCanvas(cname.Data(), cname.Data(), 800, 600);
			c4->Divide(ncol, nrow);
			ic++;
		}
		
		c4->cd(nevt%npad + 1);
			
		TString hname("hsX");
		hname += nevt;
		char htitle[200];
		sprintf(htitle, "Contact #%06d  E=%8.3f MeV", contact, energy);
		cout << endl << htitle << endl;
		hsX = new TH2F(hname.Data(), htitle, 100, 0., 50., 100, 0., 50.);
		hsX->SetStats(0);
		hsX->GetXaxis()->SetTitle("X position (cm)");
		hsX->GetXaxis()->SetTitleSize(0.05);
		hsX->GetZaxis()->SetLabelSize(0.05);
		hsX->GetXaxis()->CenterTitle();
		hsX->GetYaxis()->SetTitle("Z position (cm)");
		hsX->GetYaxis()->SetTitleSize(0.05);
		hsX->GetYaxis()->CenterTitle();
		hsX->Draw();
		char fname[200];
		sprintf(fname, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
		TFile fb(fname);
	
		fb.ls();
		float mcaleth = 0.;
		TTree *tb = NULL;
		tb = (TTree *) fb->Get("tdata");
		int nb = tb->GetEntries();
		cout << "contact: " << contact << "  " << nb << " entries" << endl;
	
		// connect input trees
		MCALevt evtb;
		tb->SetBranchAddress("time", &evtb.time);
		tb->SetBranchAddress("mult", &evtb.mult);
		tb->SetBranchAddress("bar", evtb.bar);
		tb->SetBranchAddress("pdA", evtb.pdA);
		tb->SetBranchAddress("pdB", evtb.pdB);
		tb->SetBranchAddress("pos", evtb.pos);
		tb->SetBranchAddress("E", evtb.E);
		tb->SetBranchAddress("Etot", &evtb.Etot);
		
		Long64_t i=0;
		while(tb->GetEntry(i++)) if (evtb.time == t0) break;
		fb.Close();
	
		double xm, zm, X, Z, msize;
			
		TMarker *ma[30];
		int mtype = 24; // 21
		int multx = 0;
		for (int j=0; j<15; j++) {
			int id = evtb.bar[j];
			if (id >= 0) {
				mtype = 24;// 21
				if (evtb.pdA[id]==4095 || evtb.pdB[id]==4095) mtype = 21;// 25
				// coordinate in refsys MCAL
				xm = evtb.pos[id];  // NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())
				zm = X1Z + j*BARSTEP;
		
				// passo dal refsys delle barre al refsys Laben
				X = -XREF + xm;
				Z = -ZREF + zm;
				cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV  pdA= " << evtb.pdA[id] << "  pdB= " << evtb.pdB[id] << endl;
				msize = log10(evtb.E[id]);
				ma[multx] = new TMarker(X, Z, mtype);
				ma[multx]->SetMarkerColor(4);	// blu = piano X
				ma[multx]->SetMarkerSize(1. + msize); //msize
				ma[multx]->Draw();
				multx++;
			}
		}
		for (int j=15; j<30; j++) {
			int id = evtb.bar[j];
			if (id >= 0) {
				mtype = 24;// 21
				if (evtb.pdA[id]==4095 || evtb.pdB[id]==4095) mtype = 21;//25
				// coordinate in refsys MCAL
				xm = Z1X + (j-15)*BARSTEP;
				zm = evtb.pos[id];// NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())
				
				// passo dal refsys delle barre al refsys Laben
				X = -XREF + xm;
				Z = -ZREF +zm;
				cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV  pdA= " << evtb.pdA[id] << "  pdB= " << evtb.pdB[id] << endl;
				msize = log10(evtb.E[id]);
				ma[multx] = new TMarker(X, Z, mtype);
				ma[multx]->SetMarkerColor(2);	// rosso = piano Z
				ma[multx]->SetMarkerSize(1. + msize);
				ma[multx]->Draw();
				multx++;
			}
		}
		nevt++;	
	} 

	return 0;
}
