{
	// created by MM 07/01/09 
	
	// gROOT->Reset();
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
	mcalstyle->SetLabelSize(0.04, "XYZ");
	mcalstyle->SetTitleSize(0.04, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	struct TriggerBasic  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			char type[5];
			int sa;
			int lng;
			int subms;
			int nrm;
			int rmid[9];
			int sitid[9];
			int thr[9];
			int ber[9];
			int buffer[4];
	};
	
	// AGILE pointings
	int npt = 17;
	float pointing[] = {5266, 5407, 5831, 5915, 6127, 6480, 6565, 6776, 7002, 7143, 7568, 7666, 7863, 8281, 8569, 8897, 8996};
	
	// coarse periods
	int ncoarse = 25;
	float cini[] = {5092,5393,5500,5695,5768,5773,5912,6153,6435,7297,7797,7808,7845,7976,7808,7845,7976,8029,8120,8135,8215,8261,8313,8437,8443,};
	float cend[] = {5096,5394,5504,5701,5769,5775,5913,6155,6438,7300,7800,7810,7848,7977,7810,7848,7977,8032,8121,8136,8220,8262,8316,8441,8445};
	
	// spinning periods
	int nspinning = 7;
	float sini[] = {5910,7969,8132,8208,8259,8308,8434};
	float send[] = {5913,7976,8135,8214,8262,8311,8436};
	
	TriggerBasic atrg;
	int ngr = 6;
	int nc[ngr];
	TGraph *gtrg[ngr];
	for (int i=0; i<ngr; i++) {
		gtrg[i] = new TGraph(1);
		nc[i] = 0;
	}
	gtrg[0]->SetTitle("sub-ms only trigger");
	gtrg[1]->SetTitle("sub-ms AND 1ms trigger");
	gtrg[2]->SetTitle("sub-ms AND 16ms trigger");
	gtrg[3]->SetTitle("1ms only trigger");
	gtrg[4]->SetTitle("16ms only trigger");
	gtrg[5]->SetTitle("SW trigger");
	
	int nh = 6;
	TH1I *htrg[nh];
	int ci=5000;
	int cf= 9100;
	int nbins=205;
	for (int i=0; i<nh; i++) {
		TString hname("h");
		hname += i;
		htrg[i] = new TH1I(hname, hname, nbins, ci, cf);
		htrg[i]->GetXaxis()->SetTitle("contact n.");
	}
	htrg[0]->SetTitle("sub-ms only trigger");
	htrg[1]->SetTitle("sub-ms AND 1ms trigger");
	htrg[2]->SetTitle("sub-ms AND 16ms trigger");
	htrg[3]->SetTitle("1ms only trigger");
	htrg[4]->SetTitle("16ms only trigger");
	htrg[5]->SetTitle("all triggers");
	
	
	ifstream fin;
	fin.open("/AGILE_DATA/HLT/MCALBURST/mcal_grb.dat", ifstream::in);
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		//cout << ntrg << "  " << atrg.runid << endl;
		ntrg++;
		
		if (atrg.type[0]=='G') {	// the trigger is flagged GRB
			if (atrg.subms == 1 && atrg.nrm == 0) {
				gtrg[0]->SetPoint(nc[0], atrg.runid, nc[0]++);
				htrg[0]->Fill(atrg.runid);	
			}
			if (atrg.subms == 1 && atrg.nrm == 1 && atrg.sitid[0]== 0) {
				gtrg[1]->SetPoint(nc[1], atrg.runid, nc[1]++);
				htrg[1]->Fill(atrg.runid);
			}
			if (atrg.subms == 1 && atrg.nrm == 1 && atrg.sitid[0]== 1) {
				gtrg[2]->SetPoint(nc[2], atrg.runid, nc[2]++);
				htrg[2]->Fill(atrg.runid);
			}
			if (atrg.subms == 0 && atrg.nrm == 1 && atrg.sitid[0]== 0) {
				gtrg[3]->SetPoint(nc[3], atrg.runid, nc[3]++);
				htrg[3]->Fill(atrg.runid);
			}
			if (atrg.subms == 0 && atrg.nrm == 1 && atrg.sitid[0]== 1) {
				gtrg[4]->SetPoint(nc[4], atrg.runid, nc[4]++);
				htrg[4]->Fill(atrg.runid);
			}
			if (atrg.nrm > 1 && atrg.sa == 0) {
				gtrg[5]->SetPoint(nc[5], atrg.runid, nc[5]++);
			}
			
			htrg[5]->Fill(atrg.runid);
		}
	}
	fin.close();
	ntrg--;
	cout << "number of triggers = " << ntrg << endl;
	
	// graphic section
	TCanvas ctrg("ctrg", "ctrg", 800, 600);
	ctrg.Divide(3,2);
	for (int i=0; i<6; i++) {
		ctrg.cd(i+1);
		gtrg[i]->Draw("AP");
		gtrg[i]->GetHistogram()->GetYaxis()->SetTitle("n. of trigger");
		gtrg[i]->GetHistogram()->GetXaxis()->SetTitle("Orbit number");
	}
	
	TCanvas c2("c2", "c2", 800, 600);
	c2.Divide(3,2);
	for (int i=0; i<6; i++) {
		c2.cd(i+1);
		htrg[i]->Draw();
	}
	
	TCanvas c3("all_triggers", "all_triggers", 800, 600);
	htrg[5]->SetStats(0);
	htrg[5]->Draw();
	
	for (int i=0; i<npt; i++) {	// draw pointing boundaries
		TLine *l = new TLine(pointing[i], 0., pointing[i], 220.);
		l->SetLineColor(2);
		l->Draw();
	}
	
	for (int i=0; i<ncoarse; i++) {	// draw coarse period boundaries
		TBox *bc = new TBox(cini[i], 0., cend[i], 200.);
		bc->SetFillStyle(0);
		bc->SetLineColor(4);
		bc->Draw();
	}
	
	for (int i=0; i<nspinning; i++) {// draw spinning period boundaries
		TBox *bs = new TBox(sini[i], 0., send[i], 200.);
		bs->SetFillStyle(0);
		bs->SetLineColor(3);
		bs->Draw();
	}
}
