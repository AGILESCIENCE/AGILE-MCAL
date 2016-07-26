{
	// MM 19/10/09 to analize MCAL front-end threshold scan on days 13-16/10/09
	// based on trigger_summary_3.C
	
	gROOT->Reset();
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
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int rmid[9];
			int sitid[9];
			int thr[9];
			int ber[9];
			int buffer[4];
	};

	struct TriggerNew  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int npkt;	// effective number of packets
			int totpkt;	// expected total number of packets
			int buf1;
			int buf2;
			int buf3;
			int buf4;
			int notgood;
	};
	
	// AGILE pointings
	int npt = 17;
	float pointing[] = {5266, 5407, 5831, 5915, 6127, 6480, 6565, 6776, 7002, 7143, 7568, 7666, 7863, 8281, 8569, 8897, 8996};
	
	// coarse periods
	int ncoarse = 25;
	float cini[] = {5092,5393,5500,5695,5768,5773,5912,6153,6435,7297,7797,7808,7845,7976,7808,7845,7976,8029,8120,8135,8215,8261,8313,8437,8443};
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
	
	TGraph *gmint3908 = new TGraph(1);
	TGraph *gmaxt3908 = new TGraph(1);
	
	int nh = 6;
	TH1I *htrg[nh];
	int ci=10000;//10000
	int cf=14100;//10000
	int nbins=4100;
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
	
	TH1I   *hbad = new TH1I("hbad", "hbad", nbins, ci, cf);
	TH1I   *hpkt = new TH1I("hpkt", "hpkt", nbins, ci, cf);
	
	ifstream fin;
	fin.open("/AGILE_DATA/HLT/MCALBURST/mcal_grb.dat", ifstream::in); //mcal_grb.dat.archive
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.mint3908 >> atrg.maxt3908 >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.flg01ms >> atrg.flg16ms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		//cout << ntrg << "  " << atrg.runid << endl;
		
		//gmint3908->SetPoint(ntrg, atrg.runid, atrg.mint3908);
		//gmaxt3908->SetPoint(ntrg, atrg.runid, atrg.maxt3908);
		
		ntrg++;
		
		if (1) {	// atrg.type[0]=='G' the trigger is flagged GRB
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
		
	// read the file produced with process3908
	ifstream fin2;
	TriggerNew atrg2;
	//fin2.open("/home/mcal/BUILD_MCALSW.BUILD14-devel/scripts/tmp3908.dat", ifstream::in); //mcal_grb.dat.archive
	fin2.open("/home/mcal/BUILD_MCALSW.BUILD14-devel/root_macros/tmp3908_selected.dat", ifstream::in); //mcal_grb.dat.archive
	int ntrg2 = 0;
	while (!fin2.eof()) {
		fin2 >> atrg2.runid >> atrg2.trg >> atrg2.t0 >> atrg2.tstop >> atrg2.ttot >> atrg2.date >> atrg2.time >> atrg2.mint3908 >> atrg2.maxt3908 >> atrg2.type >> atrg2.sa >> atrg2.lng >> atrg2.subms >> atrg2.flg01ms >> atrg2.flg16ms >> atrg2.nrm >> atrg2.npkt >> atrg2.totpkt >> atrg2.buf1 >> atrg2.buf2 >> atrg2.buf3 >> atrg2.buf4 >> atrg2.notgood;
		ntrg2++;
		
		if (atrg2.notgood) hbad->Fill(atrg2.runid);
		if (atrg2.npkt < atrg2.totpkt) hpkt->Fill(atrg2.runid);
		
	}
	fin2.close();
	ntrg2--;
	cout << "number of triggers 2 = " << ntrg2 << endl;	
	
	// graphic section
	TCanvas ctrg("ctrg", "ctrg", 800, 600);
	ctrg.Divide(3,2);
	for (int i=0; i<6; i++) {
		ctrg.cd(i+1);
		gtrg[i]->Draw("AP");
		gtrg[i]->GetHistogram()->GetYaxis()->SetTitle("n. of trigger");
		gtrg[i]->GetHistogram()->GetXaxis()->SetTitle("Orbit number");
	}
	
	TCanvas c3("all_triggers", "all_triggers", 800, 600);
	htrg[5]->SetStats(0);
	htrg[5]->Draw();
	htrg[5]->GetXaxis()->SetRangeUser(12726, 12850);
	
	// plot contact boundaries for MCAL thr scan
	TLine l0(12745., 0., 12745., 40.);	// P/L switch off and PDHU restart
	l0.SetLineColor(4);
	l0.Draw();
	TLine l1(12760., 0., 12760., 40.);	// start DAC_Ref 252
	l1.SetLineColor(4);
	l1.Draw();
	TLine l2(12775., 0., 12775., 40.);	// start DAC_Ref 251
	l2.SetLineColor(4);
	l2.Draw();
	TLine l3(12790., 0., 12790., 40.);	// start DAC_Ref 249
	l3.SetLineColor(4);
	l3.Draw();
	TLine l4(12805., 0., 12805., 40.);	// ripristino DAC_Ref 254
	l4.SetLineColor(4);
	l4.Draw();
	
	// get number of triggers
	cout << endl << "total triggers: " << endl;
	cout << "step 0 = " << htrg[5]->Integral(12730-ci+1, 12744-ci+1) << endl;
	cout << "step 1 = " << htrg[5]->Integral(12761-ci+1, 12775-ci+1) << endl;
	cout << "step 2 = " << htrg[5]->Integral(12776-ci+1, 12790-ci+1) << endl;
	cout << "step 3 = " << htrg[5]->Integral(12791-ci+1, 12805-ci+1) << endl;
	cout << "step 4 = " << htrg[5]->Integral(12806-ci+1, 12820-ci+1) << endl;
	
	// draw triggers separated by trigger type
	
	TCanvas c2("c2", "c2", 800, 600);
	c2.Divide(3,2);
	for (int i=0; i<6; i++) {
		c2.cd(i+1);
		htrg[i]->SetStats(0);
		htrg[i]->Draw();
		htrg[i]->GetXaxis()->SetRangeUser(12726, 12820);
		htrg[i]->GetXaxis()->SetNdivisions(508);
		htrg[i]->GetXaxis()->SetTitleOffset(1.);
		
		// draw step boundaries
		l0.Draw();
		l1.Draw();
		l2.Draw();
		l3.Draw();
		l4.Draw();	
		
		// get number of triggers
		cout << endl << "n. of triggers type: " << i << endl;
		cout << "step 0 = " << htrg[i]->Integral(12730-ci+1, 12744-ci+1) << endl;
		cout << "step 1 = " << htrg[i]->Integral(12761-ci+1, 12775-ci+1) << endl;
		cout << "step 2 = " << htrg[i]->Integral(12776-ci+1, 12790-ci+1) << endl;
		cout << "step 3 = " << htrg[i]->Integral(12791-ci+1, 12805-ci+1) << endl;
		cout << "step 4 = " << htrg[i]->Integral(12806-ci+1, 12820-ci+1) << endl;
	}
	
	TCanvas c4("t3908", "t3908", 800, 600);
	//gmint3908->Draw("AP");
	//gmaxt3908->SetMarkerColor(2);
	//gmaxt3908->Draw("PSAME");
	hbad->SetStats(0);
	hbad->Draw();
	hpkt->SetStats(0);
	hpkt->SetLineColor(2);
	hpkt->Draw("SAME");
	l1.Draw();
	l2.Draw();
	l3.Draw();
	
	
/*	for (int i=0; i<npt; i++) {	// draw pointing boundaries
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
	*/
}
