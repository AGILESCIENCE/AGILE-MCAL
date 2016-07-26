{
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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");

	//TString *rootfilename[100];
	//double t0[100];
	GRB3908 *grb;
	TH1D *hlc;
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	double tbin=0.001;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	
	// input data
	struct Trigger  {
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
			int rmid;
			int sitid;
			int ber;
			int bet;
			float lon;
			float lat;
			float h;
			float dt;
			int ntrg;	// number of reconstructed sub-ms triggers
			float mint;	// minimum time 
			float dt0;	// time difference of the found trigger with respect to the initial t0 (bstart)
			float tgrbini;
			float t90;
			float tbin;	// added since 15/10/08
			float cbkg;	// added since 15/10/08 
			float counts;
			float cpeak;
			float avgE;
			int nEbins;
			float c[4];
			float b[4];
			int x[15];
			int z[15];
			int cub[8];		// spatial zones (cuboni)
	};
	
	TTree tvsb;
	Trigger atrg;
	 tvsb.ReadFile("/data1/mcal/data_analysis/VSB/TGF_bus_flagged_9747-9780_only_GRB.out",	 "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:mint3908/F:maxt3908/F:type/C:sa/I:lng/I:subms/I:flg01ms/I:flg16ms/I:nrm/I:rmid/I:sitid/I:ber/I:bet/I:lon/F:lat/F:h/F:dt/F");
	
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("mint3908", &atrg.mint3908);
	tvsb.SetBranchAddress("maxt3908", &atrg.maxt3908);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("flg01ms", &atrg.flg01ms);
	tvsb.SetBranchAddress("flg16ms", &atrg.flg16ms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("ber", &atrg.ber);
	tvsb.SetBranchAddress("bet", &atrg.bet);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("h", &atrg.h);
	tvsb.SetBranchAddress("dt", &atrg.dt);
	int nentries = tvsb.GetEntries();
	
	// build light curves (load libMCV.so before)
	TGraph *glc[8];
	int npt[8];
	for (int k=0; k<8; k++) {
		npt[k] = 0;
		glc[k] = new TGraph(600000);	// un grafico per ogni flag
	}
	
	glc[1]->SetMarkerColor(4);	// flag 1 = 16ms only	
	glc[2]->SetMarkerColor(5);	// flag 2 = 1ms only
	glc[3]->SetMarkerColor(7);	// flag 3 = 1ms + 16ms
	glc[4]->SetMarkerColor(1);	// flag 4 = subms only
	glc[5]->SetMarkerColor(2);	// flag 5 = subms + 16ms
	glc[6]->SetMarkerColor(3);	// flag 6 = subms + 1ms
	glc[7]->SetMarkerColor(6);	// flag 7 = subms + 1ms + 16ms
	
	glc[1]->SetFillColor(4);	// flag 1 = 16ms only	
	glc[2]->SetFillColor(5);	// flag 2 = 1ms only
	glc[3]->SetFillColor(7);	// flag 3 = 1ms + 16ms
	glc[4]->SetFillColor(1);	// flag 4 = subms only
	glc[5]->SetFillColor(2);	// flag 5 = subms + 16ms
	glc[6]->SetFillColor(3);	// flag 6 = subms + 1ms
	glc[7]->SetFillColor(6);	// flag 7 = subms + 1ms + 16ms
	
	// work on 3908 data
	// double tzero = 164112361.128144;// first trigger time
	double tzero = 164112398.; // run 9754 start time
	int nplot=0;
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		if (atrg.runid>=9754 && atrg.runid<=9767  && atrg.type[0]=='G') { // 9754  9767
			nplot++;
			char rootfilename[200];
			sprintf(rootfilename, "/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT%06d_3908.root", atrg.runid);
			cout << endl << "+++++++++ " << nplot << ") processing orbit " << atrg.runid << "  trigger " << atrg.trg << "  file " << rootfilename << endl;
			grb = new GRB3908(rootfilename);
			grb->DefaultLightCurve();
			grb->CustomLightCurve(atrg.t0, -35., +35., emin, emax, barsX, barsZ, tbin);
			hlc = grb->GetLightCurve();
			
			// fill graph
			double dt0 = atrg.t0 - tzero;
			int flag = 4*atrg.subms + 2*atrg.flg01ms + atrg.flg16ms;
			for (int j=1; j<=70000; j++) {
				glc[flag]->SetPoint(npt[flag], hlc->GetBinLowEdge(j) + dt0 , hlc->GetBinContent(j));
				(npt[flag])++;
			}
			
			delete grb;
		}
	}

	// graphic section
	
	
	double label_size   = 0.05;
	double text_size    = 0.05;
	double title_offset = 1.0;
	double xpos = 0.15;
	double zpos = 0.8;
	
	
	TCanvas c1("c1", "", 1400, 800);
	c1.Divide(1,2, 0.001, 0.001);  // 3,3
	
	
	c1.cd(1);
	TGraph *g1 = new TGraph(2);
	g1->SetPoint(0, -100., 0.);
	g1->SetPoint(1, 45000., 40.);
	g1->SetTitle("");
	g1->GetYaxis()->SetTitle("Counts/ms");
	g1->GetYaxis()->SetTitleOffset(0.8);
	g1->GetYaxis()->CenterTitle();
	g1->GetXaxis()->SetTitle("Time -t0 (s)");	
	g1->GetXaxis()->SetTitleOffset(0.8);
	g1->GetXaxis()->CenterTitle();
	g1->Draw("AP");
	for (int k=1; k<8; k++) glc[k]->Draw("BP");
	
	TLegend *leg = new TLegend(0.8, 0.6, 0.95, 0.95);
	leg->AddEntry(glc[1], "16ms only", "f");
	// leg->AddEntry(glc[2], "1ms only", "f");
	// leg->AddEntry(glc[3], "1ms + 16ms", "f");
	leg->AddEntry(glc[4], "subms only", "f");
	leg->AddEntry(glc[5], "subms + 16ms", "f");
	leg->AddEntry(glc[6], "subms + 1ms", "f");
	leg->AddEntry(glc[7], "subms + 1ms + 16ms", "f");
	leg->Draw();
	
	// draw orbit boundaries
	for (int k=0; k<8; k++) {
	
		// lines
		double x=k*5795.;
		TLine *l = new TLine(x, 0., x, 40.);
		l->SetLineStyle(2);
		l->Draw();
		
		// text
		if (k<7) {
			char label[10];
			sprintf(label, "%d", 9754+k);
			TText t;
			t.DrawText(x+1500., 40., label);
		}
	}
	
	c1.cd(2);
	TGraph *g2 = new TGraph(2);
	g2->SetPoint(0, 45000., 0.);
	g2->SetPoint(1, 90000., 40.);
	g2->SetTitle("");
	g2->GetYaxis()->SetTitle("Counts/ms");
	g2->GetYaxis()->SetTitleOffset(0.8);
	g2->GetYaxis()->CenterTitle();
	g2->GetXaxis()->SetTitle("Time -t0 (s)");	
	g2->GetXaxis()->SetTitleOffset(0.8);
	g2->GetXaxis()->CenterTitle();
	g2->Draw("AP");
	for (int k=1; k<8; k++) glc[k]->Draw("BP");
	
	// draw orbit boundaries
	for (int k=7; k<15; k++) {
	
		// lines
		double x=k*5795.;
		TLine *l = new TLine(x, 0., x, 40.);
		l->SetLineStyle(2);
		l->Draw();
		
		// text
		if (k<14) {
			char label[10];
			sprintf(label, "%d", 9754+k);
			TText t;
			t.DrawText(x+1500., 40., label);
		}
	}
	
		
/*	for (int i=0; i<3; i++) {// 9
		gPad->SetTicks(1,1);
		
		TString ytitle("Counts / ");
		ytitle += tbin[i]*1000.;
		ytitle += " ms";
		
		hlc[i]->GetXaxis()->SetRangeUser(ctpeak[i] + zbefore[i], ctpeak[i] + zafter[i]);
		hlc[i]->GetYaxis()->SetRangeUser(0., 1.2*hlc[i]->GetMaximum());
		hlc[i]->SetStats(0);
		hlc[i]->SetTitle("");
		hlc[i]->GetXaxis()->SetLabelSize(label_size);
		hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
		hlc[i]->GetXaxis()->SetTitleSize(label_size);
		hlc[i]->GetXaxis()->CenterTitle();
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetYaxis()->CenterTitle();
		if (i=0) {
			hlc[i]->Draw();
		} else {
			hlc[i]->Draw("SAME");
		
		}
			
		TLatex t1;
		t1.SetTextSize(text_size);
		TString str1("T0 = ");
		str1 += ts[i]->AsString("s");
		str1 += " UT";
		t1.DrawTextNDC(xpos, zpos, str1);
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.12, 0.02, label[i]);
	}	*/

}
