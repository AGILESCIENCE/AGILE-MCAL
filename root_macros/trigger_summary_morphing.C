{
	// created by MM 22/01/09, based on macro trigger_summary.C 
	
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
	
	int nh = 7;
	TH1F *htrg[nh];
	int ci=7700;
	int cf= 9000;
	int nbins=115;
	for (int i=0; i<nh; i++) {
		TString hname("h");
		hname += i;
		htrg[i] = new TH1F(hname, hname, nbins, ci, cf);
		htrg[i]->GetXaxis()->SetTitle("contact n.");
	}
	htrg[0]->SetTitle("sub-ms only trigger");
	htrg[1]->SetTitle("sub-ms AND 1ms trigger");
	htrg[2]->SetTitle("sub-ms AND 16ms trigger");
	htrg[3]->SetTitle("1ms only trigger");
	htrg[4]->SetTitle("16ms only trigger");
	htrg[5]->SetTitle("all triggers");
	htrg[6]->SetTitle("morphing errors");
	
	
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
	
	// open morphing error file
	ifstream fmo;
	fmo.open("/home/mcal/data_analysis/morphing/Errori_filtro_morphing_tempiobt.csv", ifstream::in);
	int nmor = 0;
	while (!fmo.eof()) {
		fmo >> atrg.runid >> atrg.t0;
		htrg[6]->Fill(atrg.runid);
		nmor++;
	}
	fmo.close();
	nmor--;
	cout << "number of morphing errors = " << nmor << endl;
		
		
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
	c2.cd(6);
	htrg[6]->SetLineColor(kRed);
	htrg[6]->Draw("same");
	
	TCanvas c3("c3", "c3", 800, 600);
	c3.cd();
	htrg[5]->Draw();
	//scale htrg[6] to the pad coordinates
	double rightmax = 1.1*(htrg[6]->GetMaximum());
	double scale = gPad->GetUymax()/rightmax;
	cout << scale << endl;
	//htrg[6]->Scale(scale);
	htrg[6]->Draw("same");
	//TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(), 0,rightmax,510,"+L");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw("same");
	
}
