{
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
	
	char fname[200];
	sprintf(fname, "/data/mcal/CLEAN_ARCHIVE/RT%06d_3908_clean.root", contact);
	TFile fb(fname);
	double tf = t0 + dt;
	
	fb.ls();
	float mcaleth = 0.;
	TTree *tb = NULL;
	tb = (TTree *) fb->Get("tdata");
	int nb = tb->GetEntries();
	cout << "BURST: " << nb << " entries" << endl;
	
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
	
	int nrow = 4;
	int ncol = 4;
	int npad = nrow*ncol;
	int ic=0;
	TCanvas *c4;
	TH2F *hsX;
	
	Double_t *atime = new Double_t[nb];
	Long64_t i=0;
	while(tb->GetEntry(i)) atime[i++] = evtb.time;
	Long64_t istart = TMath::BinarySearch(nb, atime, t0);
	i=istart;
	cout << "istart = " << istart << endl;

	//int h=0;
	//while(tb->GetEntry(h++)) if (h%10000 == 0) printf("%8d \t%20.6f \t%20.6f\n", h, evtb.time, atime[h]);
	
	int nevt = 0;
	while(tb->GetEntry(i++)) {	// nb
				
		//tb->GetEntry(i);
		
		//if (i%10000 == 0) cout << "processing event " << i << endl;
		
		if (evtb.time<= tf) {	//evtb.time>=t0 && evtb.time<= tf
			//cout << "processing event " << i << endl;
			if (nevt%npad == 0) {	// create a new canvas if needed
				TString cname("c");
				cname += ic;
				c4 = new TCanvas(cname.Data(), cname.Data(), 800, 800);
				c4->Divide(ncol, nrow);
				ic++;
			}
		
			c4->cd(nevt%npad + 1);
			
			TString hname("hsX");
			hname += nevt;
			char htitle[200];
			sprintf(htitle, "evt #%d  E=%8.3f MeV  dt=%6.3f ms", nevt, evtb.Etot, (evtb.time - t0)*1000.);
			cout << endl << htitle << endl;
			hsX = new TH2F(hname.Data(), htitle, 100, 0., 50., 100, 0., 50.);
			hsX->SetStats(0);
			hsX->GetXaxis()->SetTitle("X position (cm)");
			hsX->GetXaxis()->SetTitleSize(0.05);
			//hsX->GetZaxis()->SetTitle("Counts/s cm");
			//hsX->GetZaxis()->CenterTitle();
			//hsX->GetZaxis()->SetTitleSize(0.05);
			hsX->GetZaxis()->SetLabelSize(0.05);
			hsX->GetXaxis()->CenterTitle();
			hsX->GetYaxis()->SetTitle("Z position (cm)");
			hsX->GetYaxis()->SetTitleSize(0.05);
			hsX->GetYaxis()->CenterTitle();
			hsX->Draw();
		
			double xm, zm, X, Z, msize;
		
			//cout << "Multiplicity " << evtb.mult << endl;
			//for (int i=0; i<30; i++) cout << evtb.bar[i] << "  ";
			//cout << endl;
			//for (int i=0; i<evtb.mult; i++) cout << evtb.pos[i] << "  ";
			//cout << endl;
			TMarker *ma[30];
			int mtype = 24; // 21
			int multx = 0;
			for (int j=0; j<15; j++) {
				int id = evtb.bar[j];
				if (id >= 0) {
					mtype = 24;// 21
					if (evtb.pdA[id]==4095 || evtb.pdB[id]==4095) mtype = 21;// 25
					// coordinate in refsys MCAL
					xm = -evtb.pos[id];
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
					zm = -evtb.pos[id];
					
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
			
		} else break;
	}
	delete atime;
	return 0;
}
