plot_MCAL_event_4(int contact = 12377, double t0=180166748.086027, double dt=0.000472, char *outpath = "./")         
{
	// MM 26/10/2012 plots 2D representation of all events in a TGF. Works on a single TGF at a time.
	// based on plot_MCAL_event_3.C

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
	
	TH2::AddDirectory(kFALSE);

	double XREF = -24.2;
	double ZREF = -21.6;
	double X1Z = -18.11;
	double Z1X = -18.11;
	double BARSTEP = 2.587;
	double PSIGMA = 1.;
	double BARLEN  =   37.5;

	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	TTimeStamp ts;
	ts.Set((int)  t0, 0, epoch.GetSec(),0);
	
	int nrow = 3; // 3
	int ncol = 4; // 4
	int npad = nrow*ncol;
	int ic=0;
	TCanvas *c4=NULL;
	TH2F *hsX;
	int nevt = 0;
	char cname[200];
	char oname[200];
		
	// load root file
	char fname[100];
	sprintf(fname, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
	TFile fb(fname);

	fb.ls();
	float mcaleth = 0.;
	TTree *tbaux = NULL;
	TTree *tb = NULL;
	tb = (TTree *) fb.Get("tdata");

	int nb = tb->GetEntries();
	cout << "Contact: " << contact << "  " << nb << " entries" << endl;

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
	
	
	// loop on events
	Long64_t i=0;
	while(tb->GetEntry(i++)) if (evtb.time == t0) break;
	i--;

	while(tb->GetEntry(i++)) {
	
		if (evtb.time - t0 > dt) break;
		
		if (nevt%npad == 0) {	// create a new canvas if needed
			
			if (c4) {	// save picture of current canvas, if it exists	
				sprintf(oname, "%s/%s.gif", outpath, cname);
				c4->Print(oname);
			}
			
			sprintf(cname, "%06d_%08d_%06d_map%02d", contact, ts.GetDate(), ts.GetTime(), ic);
			c4 = new TCanvas(cname, cname, 800, 600);
			c4->Divide(ncol, nrow);
			ic++;
			
			cout << "Created canvas " << cname << endl;
		}
	
		c4->cd(nevt%npad + 1);
		cout << "Moving to pad " << nevt%npad + 1 << endl;
		
		TString hname("hsX");
		hname += nevt;
		char htitle[200];
		sprintf(htitle, "E=%8.3f MeV dt = %d us", evtb.Etot, (evtb.time - t0)*1.e6);
		cout << endl << hname.Data() << "    " << htitle << endl;
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

		double xm, zm, X, Z, msize;
			
		TMarker *ma[30];
		TLine *bar[30];
		int mtype = 24; // 21
		int multx = 0;
		for (int j=0; j<15; j++) {
			int id = evtb.bar[j];
			if (id >= 0) {
				mtype = 24;// 21
				if (evtb.pdA[id]==4095 || evtb.pdB[id]==4095) mtype = 5;// 21
				// coordinate in refsys MCAL
				xm = evtb.pos[id];  // NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())
				zm = X1Z + j*BARSTEP;
		
				// passo dal refsys delle barre al refsys Laben
				X = -XREF + xm;
				Z = -ZREF + zm;
				msize = TMath::Max(0., log10(evtb.E[id]));
				cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV  pdA= " << evtb.pdA[id] << "  pdB= " << evtb.pdB[id] << "  msize= " << msize << endl;
				
				if (X<0. || X>50.)  { 
					mtype = 5;
					if (X<0.) X=0.;
					else X=50.;
				}
				
				ma[multx] = new TMarker(X, Z, mtype);
				ma[multx]->SetMarkerColor(4);	// blu = piano X
				ma[multx]->SetMarkerSize(1. + msize); //msize
				ma[multx]->Draw();
				
				bar[multx] = new TLine(0, Z, 50., Z);
				bar[multx]->SetLineColor(4);	// blu = piano X
				bar[multx]->SetLineStyle(2); 
				bar[multx]->Draw();
				multx++;
			}
		}
		for (int j=15; j<30; j++) {
			int id = evtb.bar[j];
			if (id >= 0) {
				mtype = 24;// 21
				if (evtb.pdA[id]==4095 || evtb.pdB[id]==4095) mtype = 5;//21
				// coordinate in refsys MCAL
				xm = Z1X + (j-15)*BARSTEP;
				zm = evtb.pos[id];// NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())
				
				// passo dal refsys delle barre al refsys Laben
				X = -XREF + xm;
				Z = -ZREF +zm;
				msize = TMath::Max(0., log10(evtb.E[id]));
				cout << "bar " << j << " X=" << X << " cm   Z=" << Z << " cm  E=" << evtb.E[id] << " MeV  pdA= " << evtb.pdA[id] << "  pdB= " << evtb.pdB[id] << "  msize= " << msize << endl;
				
				if (Z<0. || Z>50.)  { 
					mtype = 5;
					if (Z<0.) Z=0.;
					else Z=50.;
				}
				
				ma[multx] = new TMarker(X, Z, mtype);
				ma[multx]->SetMarkerColor(2);	// rosso = piano Z
				ma[multx]->SetMarkerSize(1. + msize);
				ma[multx]->Draw();
				
				bar[multx] = new TLine(X, 0., X, 50.);
				bar[multx]->SetLineColor(2);	// rosso = piano Z
				bar[multx]->SetLineStyle(2); 
				bar[multx]->Draw();
				multx++;
			}
		}
		nevt++;	
	}
	
				
	if (c4) {	// save last picture of current canvas
		sprintf(oname, "%s/%s.gif", outpath, cname);
		c4->Print(oname);
	}

	 
	fb.Close(); 
		
	return 0;
}
