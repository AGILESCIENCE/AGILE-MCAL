{
	
	// event structure (from MCVLib MCVFile.h)
	gROOT->Reset();
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
	
	// input file
//	TFile fb("/data1/mcalsw_output_COMMISSIONING/RT/RT000201_3909_sel_02.root");
//	TFile fb("~/mcalsw_output_COMMISSIONING/RT/RT000226_3909.root");
	TFile fb("~/mcalsw_output_COMMISSIONING/RT/RT000201_3909_sel_02.root");
	fb.ls();
	float mcaleth = 5.;
	TTree *tb = NULL;
	tb = (TTree *) fb->Get("tdata_sel_0");
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
	
	int istart=3500;
	int ncanvas=10;
	
	for (ic=0; ic<ncanvas; ic++) {
		TString cname("c");
		cname += ic;
		
		TCanvas *c4 = new TCanvas(cname.Data(), cname.Data(), 800, 800);
		c4->Divide(4, 4);
		TH2F *hsX;
				
		for (int k=0; k<16; k++) {
			c4->cd(k+1);
			
			TString hname("hsX");
			hname += (k + istart + ic*16);
			TString htitle("event ");
			htitle += (k + istart + ic*16);
		
			hsX = new TH2F(hname.Data(), htitle.Data(), 100, 0., 50., 100, 0., 50.);
			hsX->SetStats(0);
			hsX->GetXaxis()->SetTitle("X position (cm)");
			hsX->GetXaxis()->SetTitleSize(0.05);
			hsX->GetZaxis()->SetTitle("Counts/s cm");
			hsX->GetZaxis()->CenterTitle();
			hsX->GetZaxis()->SetTitleSize(0.05);
			hsX->GetZaxis()->SetLabelSize(0.05);
			hsX->GetXaxis()->CenterTitle();
			hsX->GetYaxis()->SetTitle("Z position (cm)");
			hsX->GetYaxis()->SetTitleSize(0.05);
			hsX->GetYaxis()->CenterTitle();
			hsX->Draw();
		
			int i=0;
			int c=0;
			int id;
			double pxx[15];
			double pxz[15];
			double xm, zm, X, Z, msize;
		
			tb->GetEntry(k + istart + ic*16);
			//cout << "Multiplicity " << evtb.mult << endl;
			//for (int i=0; i<30; i++) cout << evtb.bar[i] << "  ";
			//cout << endl;
			//for (int i=0; i<evtb.mult; i++) cout << evtb.pos[i] << "  ";
			//cout << endl;
			TMarker *ma[30];
			int multx = 0;
			for (int j=0; j<15; j++) {
				id = evtb.bar[j];
				if (id >= 0) {
					// coordinate in refsys MCAL
					xm = -evtb.pos[id];
					zm = X1Z + j*BARSTEP;
			
					// passo dal refsys delle barre al refsys Laben
					X = -XREF + xm;
					Z = -ZREF + zm;
					//cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV" << endl;
					msize = log10(evtb.E[id]);
					ma[multx] = new TMarker(X, Z, 21);
					ma[multx]->SetMarkerColor(4);
					ma[multx]->SetMarkerSize(msize);
					ma[multx]->Draw();
					multx++;
				}
			}
			for (int j=15; j<30; j++) {
				int id = evtb.bar[j];
				if (id >= 0) {
					// coordinate in refsys MCAL
					xm = Z1X + (j-15)*BARSTEP;
					zm = -evtb.pos[id];
					
					// passo dal refsys delle barre al refsys Laben
					X = -XREF + xm;
					Z = -ZREF +zm;
					cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV" << endl;
					msize = log10(evtb.E[id]);
					ma[multx] = new TMarker(X, Z, 21);
					ma[multx]->SetMarkerColor(2);
					ma[multx]->SetMarkerSize(msize);
					ma[multx]->Draw();
					multx++;
				}
			}
		}
	}
}
