process_histo_2(int contact, int nbefore=15, int nafter=15)
{
	// MM 10/04/2012 based on process_histo.C, tailored to high-energy event nalysis / calibration
	// position estimate based on plot_MCAL_event_3.C
	// does not save histogram file. Use process_histo.C instead
	
	// MM 28/03/2012 
	// builds a histogram collection summing data from different contacts
	// needed to exploit MCVlib analysis routines for offset and energy calibration
	
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
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
	int cini = contact - nbefore;	// initial contact
	int cfin = contact + nafter;	// final contact (included)
	char cpath[100];
	
	// data needed for position estimate
	double XREF = -24.2;
	double ZREF = -21.6;
	double X1Z = -18.11;
	double Z1X = -18.11;
	double BARSTEP = 2.587;
	double PSIGMA = 1.;
	double BARLEN  =   37.5;
	
	// get file info from first file
	
	MCVFileInfo *info = NULL;
 	sprintf(cpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
	
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


	// debug for simulations
//	data.Add("~/BUILD_MCALSW.BUILD14-devel/root_macros/sim1000MeV.root");
		
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

//	MCVHisto *rh = new MCVHisto();
	
	int i=0;
	int thr1 = 40;
	int thr2 = 10;
	
	TH2F *hb = new TH2F("hb", "hb", 31, 0., 29., 31, 0., 29.);
	TH1F *hs = new TH1F("hs", "hs", 10000, 0., 1000.);
	TH2F *he = new TH2F("he", "he", 200, 0., 200., 200, 0., 200.);
	TH2F *hd = new TH2F("hd", "hd", 100, -10., 10., 100, -10., 10.);
	
	double xm1, zm1, X1, Z1, dist;
	double xm2, zm2, X2, Z2;
	while (data.GetEntry(i++)) {
		if (inevt.mult == 2) { 
		
			// get bar address
			int ibar1 = 0;
			while (inevt.bar[ibar1] != 0 && ibar1 <30) ibar1++;
			int ibar2 = 0;
			while (inevt.bar[ibar2] != 1 && ibar2 <30) ibar2++;
		
			// selection for offset correction
			// get bar address
			//int ibar = 0;
			//while (inevt.bar[ibar] < 0 && ibar <30) ibar++;
			//if (ibar<30 && ((inevt.pdA[0]>(info->oA[ibar] + thr1) && inevt.pdB[0]<(info->oB[ibar] + thr2)) || (inevt.pdB[0]>(info->oB[ibar] + thr1) && inevt.pdA[0]<(info->oA[ibar] + thr2)))) rh->FillHisto(&inevt);
			
			if (inevt.E[0] > 5. && inevt.E[1] > 5. && ibar1<15 && ibar2>15) {
				
				// posizione piano X
				xm1 = inevt.pos[0];  // NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())
				zm1 = X1Z + ibar1*BARSTEP;	
				// passo dal refsys delle barre al refsys Laben
				X1 = -XREF + xm1;
				Z1 = -ZREF + zm1;
				
				// posizione piano Z
				xm2 = Z1X + (ibar2-15)*BARSTEP;
				zm2 = inevt.pos[1];// NB: ci va il segno + (vedi commento in MCVFile::CalculateEandPos())	
				// passo dal refsys delle barre al refsys Laben
				X2 = -XREF + xm2;
				Z2 = -ZREF +zm2;
				
				dist = TMath::Sqrt((X2-X1)*(X2-X1) + (Z2-Z1)*(Z2-Z1));
				
			//	if (dist<2.) {
					hd->Fill(X2-X1, Z2-Z1);
					hb->Fill(ibar1, ibar2);
					hs->Fill(inevt.Etot);
					he->Fill(inevt.E[0], inevt.E[1]);
			//	}

				
				// rh->FillHisto(&inevt);
			}
		}
	}
	
	TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
	c1->Divide(2,2);
	c1->cd(1);
	hb->Draw("COLZ");
	c1->cd(2);
	hs->Draw();
	c1->cd(3);
	he->Draw("COLZ");
	c1->cd(4);
	hd->Draw("COLZ");
	
/*	char hname[100];
	sprintf(hname,"Hmerged_%06d_%03d-%03d.root", contact, nbefore, nafter);
	TFile *fh = new TFile(hname, "recreate");
	TObjArray *rawHisto = rh->GetRawHisto();
	rawHisto->Write("rawHisto", TObject::kSingleKey);
	info->Write();
	fh->Close(); 
*/

}