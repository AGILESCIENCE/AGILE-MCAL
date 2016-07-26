{
	// version 3 by MM 21/09/09
	// read also file tmp3908.dat created with process3908, which includes additional diagnastic info
	
	// version 2 by MM 17/08/09
	// created by MM 07/01/09 
	
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
	int ci=41700;//27000
	int cf=42700;//25850
	int nbins=200;
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
	
	double hwpkt_B = 0.;
	double hwpkt_noB = 0.;
	double swpkt = 0.;
	int hwtrg_B = 0;
	int hwtrg_noB = 0;
	int swtrg = 0;
	
	ifstream fin;
	fin.open("/AGILE_PROC3/DATA/HLT/MCALBURST/mcal_grb.dat.archive", ifstream::in); //mcal_grb.dat.archive
	int ntrg = 0;
	while (!fin.eof()) {
		fin >> atrg.runid >> atrg.trg >> atrg.t0 >> atrg.tstop >> atrg.ttot >> atrg.date >> atrg.time >> atrg.mint3908 >> atrg.maxt3908 >> atrg.type >> atrg.sa >> atrg.lng >> atrg.subms >> atrg.flg01ms >> atrg.flg16ms >> atrg.nrm;
		for (int i=0; i<atrg.nrm; i++) fin >> atrg.rmid[i] >> atrg.sitid[i] >>  atrg.thr[i] >> atrg.ber[i];
		for (int i=0; i<4; i++) fin >> atrg.buffer[i];
		//cout << ntrg << "  " << atrg.runid << endl;
		
		//gmint3908->SetPoint(ntrg, atrg.runid, atrg.mint3908);
		//gmaxt3908->SetPoint(ntrg, atrg.runid, atrg.maxt3908);
		
		ntrg++;
		
		if (atrg.runid>=ci && atrg.runid<=cf && atrg.type[0]=='G') {	// atrg.type[0]=='G' the trigger is flagged GRB
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
			if (atrg.subms == 0 && atrg.nrm == 1 && atrg.sitid[0]== 1) { // 16ms-only GRB
				gtrg[4]->SetPoint(nc[4], atrg.runid, nc[4]++);
				htrg[4]->Fill(atrg.runid);
				//printf("%06d \t%s \t%s \t%20.6f\n", atrg.runid, atrg.date, atrg.time, atrg.t0);
				
			}
			if (atrg.nrm > 1 && atrg.sa == 0 ) {
				gtrg[5]->SetPoint(nc[5], atrg.runid, nc[5]++);
			}
			
			htrg[5]->Fill(atrg.runid);
			
			if (atrg.nrm > 1 && atrg.sa == 0 && !strncmp(atrg.type, "GRB", 3)) { // SW trg classified as GRB
				for (int i=0; i<4; i++) swpkt += atrg.buffer[i];
				printf("%06d \t%s \t%s \t%20.6f \t%2d\n", atrg.runid, atrg.date, atrg.time, atrg.t0, atrg.lng);
				swtrg++;
			}
			if (atrg.nrm <= 1 && atrg.type[0]!='B') {
				for (int i=0; i<4; i++) hwpkt_noB += atrg.buffer[i];
				hwtrg_noB++;
			}
			if (atrg.nrm <= 1 && atrg.type[0]=='B') {
				for (int i=0; i<4; i++) hwpkt_B += atrg.buffer[i];
				hwtrg_B++;
			}
			
		}
	}
	fin.close();
	ntrg--;
	int ncontact = 0;
	for (int i=1; i<nbins; i++) if (h5->GetBinContent(i)>0) ncontact++;
	cout << "number of triggers = " << ntrg << endl;
	cout << "number of contacts with triggers = " << ncontact << endl;
	printf("SW triggers = %10.0f  total packets = %10.0f \n", swtrg, swpkt);
	printf("HW triggers NO BOOST = %10.0f  total packets = %10.0f \n", hwtrg_noB, hwpkt_noB);
	printf("HW triggers  BOOST = %10.0f  total packets = %10.0f \n", hwtrg_B, hwpkt_B);
		
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
	
	TCanvas c2("c2", "c2", 800, 600);
	c2.Divide(3,2);
	for (int i=0; i<6; i++) {
		c2.cd(i+1);
		htrg[i]->Draw();
	}
	
	TCanvas c3("all_triggers", "all_triggers", 800, 600);
	htrg[5]->SetStats(0);
	htrg[5]->Draw();
	
	TLine l1(11802., 0., 11802., 40.);
	l1.SetLineColor(4);
	l1.Draw();
	TLine l2(12021., 0., 12021., 40.);
	l2.SetLineColor(4);
	l2.Draw();
	TLine l3(12338., 0., 12338., 40.);
	l3.SetLineColor(4);
	l3.Draw();
	
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
