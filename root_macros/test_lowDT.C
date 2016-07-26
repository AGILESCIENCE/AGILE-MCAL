// test macro to analyze the origin of events with low time separation
// library libMCV.so must be loaded first


struct MCALevt {	// copiata da MCVFile.h
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

void CopyEvt(MCALevt *inevt, MCALevt *old);

void run()
{
float X1Z = -18.11;
float Z1X = -18.11;
float BARSTEP = 2.587;
float BARLEN  =   37.5;
float XREF = 24.2;
float ZREF = 21.6;
	
	// load data from file
	gROOT->Reset();
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT011828_3909.root");
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT006746_3909.root");	// T5  @CGS, bkg
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT006797_3909.root");	// T14 @CGS, bkg
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT013209_3909.root");	// T14 @IABG in TV, bkg
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT013210_3909.root");	// T14 @IABG in TV, Cs137
//	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT013301_3909.root");	// T14 @IABG in TV, bkg
	TFile *fp = new TFile("/data1/mcalsw_output/RT/RT013303_3909.root");	// T14 @IABG in TV, Cs137
	fp->ls();
	TTree *t2 = (TTree *) fp->Get("tdata");
	t2->Print();
	
	// create histograms
	TH1D *hdt = new TH1D("hdt", "hdt", 5000, 0., 5000.);
	TH1D *hdtS = new TH1D("hdtS", "hdtS", 5000, 0., 5000.);
	TH2F *hedt = new TH2F("hedt", "hedt", 20, 0., 20., 2000, 0., 200.); // energy vs DT
	TH1F *hdposX = new TH1F("hdposX", "hdposX", 400, -40., 40.);
	TH1F *hdposZ = new TH1F("hdposZ", "hdposZ", 400, -40., 40.);
	TH2F *hdposX2 = new TH2F("hdposX2", "hdposX2", 200, -40., 40., 30, -15., 15.);
	TH2F *hdposXZ2 = new TH2F("hdposXZ2", "hdposXZ2", 40, -40., 40., 200, -40., 40.);
	TH2F *hdposZ2 = new TH2F("hdposZ2", "hdposZ2", 40, -40., 40., 30, -15., 15.);
	TH2F *hdposZX2 = new TH2F("hdposZX2", "hdposZX2", 200, -40., 40., 200, -40., 40.);
	TH1F *hdposXhighDT = new TH1F("hdposXhighDT", "hdposXhighDT", 400, -40., 40.);
	TH1F *hdposZhighDT = new TH1F("hdposZhighDT", "hdposZhighDT", 400, -40., 40.);
	TH2F *hdposXhighDT2 = new TH2F("hdposXhighDT2", "hdposXhighDT2", 400, -40., 40., 30, -15., 15.);
	TH2F *hdposZhighDT2 = new TH2F("hdposZhighDT2", "hdposZhighDT2", 400, -40., 40., 30, -15., 15.);
	TH1F *hE1 = new TH1F("hE1", "hE1", 2000, 0., 2000.);		// first event total energy
	TH1F *hE2 = new TH1F("hE2", "hE2", 2000, 0., 2000.);		// second event total energy
	TH1F *hEtot = new TH1F("hEtot", "hEtot", 2000, 0., 2000.);	// events total energy
	TH1F *hE12 = new TH1F("hE12", "hE12", 2000, 0., 2000.);	// E1+E2 total energy
	TH1F *hm1 = new TH1F("hm1", "hm1", 31, 0, 31);		// first event multiplicity
	TH1F *hm2 = new TH1F("hm2", "hm2", 31, 0, 31);		// second event multiplicity
	TH1F *hmtot = new TH1F("hmtot", "hmtot", 31, 0, 31);	// events total multiplicity
	TH1F *hcons = new TH1F("hcons", "hcons", 100, 0, 100);	// number of consecutive low DT events
	
	// connect input tree
	MCALevt inevt;
	MCALevt oldevt;
	t2->SetBranchAddress("time", &inevt.time);
	t2->SetBranchAddress("mult", &inevt.mult);
	t2->SetBranchAddress("bar", inevt.bar);
	t2->SetBranchAddress("pdA", inevt.pdA);
	t2->SetBranchAddress("pdB", inevt.pdB);
	t2->SetBranchAddress("pos", inevt.pos);
	t2->SetBranchAddress("E", inevt.E);
	t2->SetBranchAddress("Etot", &inevt.Etot);

	int nevents = t2->GetEntries();
	cout << "Number of events to process = " << nevents << endl;
		
	// main loop
	int nlowdt = 0;
	int inbar, oldbar;
	float oldXpos, oldZpos, inXpos, inZpos;
	t2->GetEntry(0);
	CopyEvt(&inevt, &oldevt);
	int ncons = 0;	// number of consecutive low DT events
	for (int i=1; i<nevents; i++) { 
		if (i%100000 == 0) cout << i << " events processed..." << endl;
		t2->GetEntry(i);
		double dt = inevt.time - oldevt.time;
		hdt->Fill(dt);
		hEtot->Fill(inevt.Etot);
		hmtot->Fill(inevt.mult);
		
		if (dt<=12.) { // main analysis on low dt events
			hedt->Fill(dt, inevt.Etot);
			hE1->Fill(oldevt.Etot);
			hE2->Fill(inevt.Etot);
			hE12->Fill(oldevt.Etot + inevt.Etot);
			hm1->Fill(oldevt.mult);
			hm2->Fill(inevt.mult);
			nlowdt++;
			ncons++;
			
			if (inevt.mult==1 && oldevt.mult==1) {
				int oldX=0;
				int inX=0;
				for (int j=0; j<30; j++) {
					if (inevt.bar[j] >= 0) {
						inbar = j;
						if (inbar<15) inX = 1;
						break;
					}
				}
				for (int j=0; j<30; j++) {
					if (oldevt.bar[j] >= 0) {
						oldbar = j;
						if (oldbar<15) oldX = 1;
						break;
					}
				}
				if (inX && oldX) {	// both events on plane X
					hdposX->Fill(inevt.pos[0] - oldevt.pos[0]);
					hdposX2->Fill(inevt.pos[0] - oldevt.pos[0], inbar - oldbar);
				}
				if (inX==0 && oldX==0) {	// both events on plane Z
					hdposZ->Fill(inevt.pos[0] - oldevt.pos[0]);
					hdposZ2->Fill(inevt.pos[0] - oldevt.pos[0], inbar - oldbar);
				}
				if (inX==0 && oldX==1) {	// old on plane X, new on plane Z
					oldXpos = -XREF -oldevt.pos[0];
					oldZpos = -ZREF + X1Z + oldbar*BARSTEP;
					inXpos  = -XREF + Z1X + (inbar-15)*BARSTEP;
					inZpos  = -ZREF -inevt.pos[0];
					hdposXZ2->Fill(inXpos-oldXpos, inZpos-oldZpos);
				}
				if (inX==1 && oldX==0) {	// old on plane Z, new on plane X
					inXpos = -XREF -inevt.pos[0];
					inZpos = -ZREF + X1Z + inbar*BARSTEP;
					oldXpos  = -XREF + Z1X + (oldbar-15)*BARSTEP;
					oldZpos  = -ZREF -oldevt.pos[0];
					hdposZX2->Fill(inXpos-oldXpos, inZpos-oldZpos);
				}
/*	
	if (plane == 0) { // X plane track
			xm = -inevt->pos[id];
			zm = X1Z + ibar*BARSTEP;
	
			// passo dal refsys delle barre al refsys Laben
			X = -XREF + xm;
			Z = -ZREF + zm;
			eX = PSIGMA;
			eZ = 0.5*BARSTEP;
			
			// passo dal refsys Laben al refsys Kalman
			setKalmanCoordinate();
			
	} else {	  // Z plane track
			xm = Z1X + (ibar-15)*BARSTEP;
			zm = -inevt->pos[id];
			
			// passo dal refsys delle barre al refsys Laben
			X = -XREF + xm;
			Z = -ZREF +zm;
			eX = 0.5*BARSTEP;
			eZ = PSIGMA;
			
			// passo dal refsys Laben al refsys Kalman
			setKalmanCoordinate();
	}*/	
					
				
				if ((inbar - oldbar<-1) || (inbar - oldbar>1)) hdtS->Fill(dt);
			}
		} else { // create control histogram
		
			if (ncons>0) {
				hcons->Fill(ncons);
				ncons=0;
			}
		
			if (inevt.mult==1 && oldevt.mult==1) { //  && inevt.Etot>1. && oldevt.Etot>1.
				int oldX=0;
				int inX=0;
				for (int j=0; j<30; j++) {
					if (inevt.bar[j] >= 0) {
						inbar = j;
						if (inbar<15) inX = 1;
						break;
					}
				}
				for (int j=0; j<30; j++) {
					if (oldevt.bar[j] >= 0) {
						oldbar = j;
						if (oldbar<15) oldX = 1;
						break;
					}
				}
				if (inX && oldX) {
					hdposXhighDT->Fill(inevt.pos[0] - oldevt.pos[0]);
					hdposXhighDT2->Fill(inevt.pos[0] - oldevt.pos[0], inbar - oldbar);
				}
				if (inX==0 && oldX==0) {
					hdposZhighDT->Fill(inevt.pos[0] - oldevt.pos[0]);
					hdposZhighDT2->Fill(inevt.pos[0] - oldevt.pos[0], inbar - oldbar);
				}
			}
			hdtS->Fill(dt);
		}
		
		CopyEvt(&inevt, &oldevt);
	}
	cout << "Total events with DT<=12 us: " << nlowdt << endl;
	
	// graphic section
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.04, "xyz");
	mcalstyle->SetTitleSize(0.04, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	TCanvas *c1 = new TCanvas("lowDT_data", "low DT data", 800, 600);
	c1->Divide(4, 3);
	
	c1->cd(1);
	hdt->Draw();
	hdtS->SetLineColor(2);
	hdtS->Draw("SAME");
	
	c1->cd(2);
	//hedt->Draw("COLZ");
	TLegend *leg1 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<13; i++) {
		TString pname("DT_");
		pname += i+1;
		TH1D *aux = hedt->ProjectionY(pname.Data(), i+1, i+1, "");
		aux->SetLineColor(i+1);
		float tot = aux->Integral();
		if (tot) aux->Scale(1./tot);
		cout << "DT = " << i+1 << "   -   entries = " << tot << endl;
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Normalized counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg1->AddEntry(aux, pname.Data(), "lp");
	}
	leg1->Draw();

	c1->cd(3);
	hdposX->Draw();
	hdposZ->SetLineColor(2);
	hdposZ->Draw("SAME");

	c1->cd(4);
	hdposXhighDT->Draw();
	hdposZhighDT->SetLineColor(2);
	hdposZhighDT->Draw("SAME");

	c1->cd(5);
	hdposX2->Draw("COLZ");

	c1->cd(6);
	hdposZ2->Draw("COLZ");

	c1->cd(7);
	hdposXhighDT2->Draw("COLZ");

	c1->cd(8);
	hdposZhighDT2->Draw("COLZ");
	
	c1->cd(9);
	hdposXZ2->Draw("COLZ");
	
	c1->cd(10);
	hdposZX2->Draw("COLZ");
		
	TCanvas *c2 = new TCanvas("lowDT_data_2", "low DT data 2", 1000, 300);
	c2->Divide(3,1);
	
	c2->cd(1);
	gPad->SetLogx(kTRUE);
	gPad->SetLogy(kTRUE);
	double s = hE1->Integral();
	hE1->Sumw2();
	hE1->Scale(1./s);
	hE1->GetXaxis()->SetTitle("Energy (MeV)");
	hE1->GetYaxis()->SetTitle("Normalized counts/MeV");
	hE1->Draw("");
	s = hEtot->Integral();
	hEtot->Sumw2();
	hEtot->Scale(1./s);
	hEtot->SetLineColor(2);
	hEtot->Draw("SAME");
	s = hE2->Integral();
	hE2->Sumw2();
	hE2->Scale(1./s);
	hE2->SetLineColor(3);
	hE2->Draw("SAME");
	s = hE12->Integral();
	hE12->Sumw2();
	hE12->Scale(1./s);
	hE12->SetLineColor(4);
	hE12->Draw("SAME");
	
	cout << "E>3 MeV events fraction ALL   " << hEtot->Integral(4, 2000) << endl;
	cout << "E>3 MeV events fraction evt 1 " << hE1->Integral(4, 2000) << endl;
	cout << "E>3 MeV events fraction evt 2 " << hE2->Integral(4, 2000) << endl;
	
	c2->cd(2);
	gPad->SetLogy(kTRUE);
	s = hm1->Integral();
	hm1->Scale(1./s);
	hm1->GetXaxis()->SetTitle("Multiplicity");
	hm1->GetYaxis()->SetTitle("Normalized counts");
	hm1->Draw("");
	s = hmtot->Integral();
	hmtot->Scale(1./s);
	hmtot->SetLineColor(2);
	hmtot->Draw("SAME");
	s = hm2->Integral();
	hm2->Scale(1./s);
	hm2->SetLineColor(3);
	hm2->Draw("SAME");
	
	cout << "multiple events fraction ALL   " << hmtot->Integral(3, 30) << endl;
	cout << "multiple events fraction evt 1 " << hm1->Integral(3, 30) << endl;
	cout << "multiple events fraction evt 2 " << hm2->Integral(3, 30) << endl;
	
	c2->cd(3);
	gPad->SetLogy(kTRUE);
	hcons->GetXaxis()->SetTitle("Consecutive DT<12us events");
	hcons->GetYaxis()->SetTitle("Normalized counts");
	s = hcons->Integral();
	hcons->Scale(1./s);	
	hcons->Draw();
}
//
//--------------------------------------------------------------------
//

void CopyEvt(MCALevt *inevt, MCALevt *oldevt)		// riadattata da MCVTree.cpp
{
	int auxmult;
	oldevt->time = inevt->time;
	auxmult = inevt->mult;
	oldevt->mult = auxmult;
	oldevt->totEX = 0;
	oldevt->totEZ = 0;
	for (int i=0; i<30; i++) oldevt->bar[i] = inevt->bar[i];
	for (int i=0; i<auxmult; i++) {
		oldevt->pdA[i] = inevt->pdA[i];
		oldevt->pdB[i] = inevt->pdB[i];
		oldevt->pos[i] = inevt->pos[i];
		oldevt->E[i] = inevt->E[i];
	}
	oldevt->Etot = inevt->Etot;
	return;
}
