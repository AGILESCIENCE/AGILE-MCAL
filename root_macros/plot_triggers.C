{
	// MM 07/10/09 plots the trigger population detected with the offline trigger search
	
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
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	ifstream inFile;
	inFile.open("/home/mcal/BUILD_MCALSW.BUILD14-devel/root_macros/trg_010000-012477.dat");	// on HP1
//	inFile.open("/home/martino/BUILD_MCALSW.BUILD14-devel/root_macros/trg_010000-012477.dat");	// on laptop
	if (!inFile) {
	cout << "Unable to open trigger file";
	exit(1); // terminate with error
	}
	
	TGraph *g1 = new TGraph(20000);
	TH3F *topology = new TH3F("topology", "topology", 100, 0., 100.,  50, 0., 0.010, 50, 0., 5.); // counts, dt, hr
	
	float tini = 6.;
	float tfin = 6000.;
	int nbins = 30;
	float fact = (tfin/tini)**(1./nbins);
	float abin[42];
	abin[0] = 0.01;
	abin[1] = tini;
	for (int i=2; i<=40; i++) abin[i] = abin[i-1]*fact;
	for (int i=0; i<=40; i++) cout << abin[i] << endl;
	
	//TH1F *deltat = new TH1F("deltat","deltat", 130, 0., 13000);	
	TH1F *deltat = new TH1F("deltat","deltat", 40, abin);	
	
	int index = 0;
	int sel = 0;
	char aux[10], path[100];
	int trgid, counts[10000], contact[10000];
	double t0[10000], hr[10000], dt[10000];
	while (inFile >> aux >> path >> trgid >> t0[sel] >> counts[sel] >> hr[sel] >> dt[sel]) {
		//g1->SetPoint(index, hr, counts);
		topology->Fill(counts[sel], dt[sel], hr[sel]);
		//if (counts[sel]>=50. || dt[sel]>=0.010 || hr[sel]>=5.) cout << path << "  " << t0 << "  " << counts[sel] << "  " << hr[sel] << "  " << dt[sel] << endl;
		
		// do main selection
		if (hr[sel]>=0.5 && counts[sel]>=10) {
			sscanf(path, "/MCAL_PROC/ARCHIVE/%06d", &contact[sel]);
			printf("%4d \t%06d \t%.6f \t%d \t%.2f \t%.6f\n", sel, contact[sel], t0[sel], counts[sel], hr[sel], dt[sel]);
			if (sel) deltat->Fill(t0[sel]-t0[sel-1]);
			sel++;
		}
		
		index++;
	}
	cout << "Processed " << index << " triggers" << endl;
	inFile.close();
	
	TCanvas *c9 = new TCanvas ("c9", "c9", 800, 600);
	//g1->Draw("AP");
	c9->Divide(3,2);
	
	c9->cd(1);
	gPad->SetLogz(1);
	TH2F *zyt = topology->Project3D("zy");
	zyt->SetStats(0);
	zyt->SetTitle("HR vs. dt");
	zyt->GetXaxis()->SetTitle("dt");
	zyt->GetXaxis()->CenterTitle();
	zyt->GetYaxis()->SetTitle("HR");
	zyt->GetYaxis()->CenterTitle();
	zyt->GetYaxis()->SetTitleOffset(2.);
	zyt->DrawClone("colZ");
	
	c9->cd(2);
	gPad->SetLogz(1);
	TH2F *xyt = topology->Project3D("yx");
	xyt->SetStats(0);
	xyt->SetTitle("dt vs. counts");
	xyt->GetXaxis()->SetTitle("counts");
	xyt->GetXaxis()->CenterTitle();
	xyt->GetYaxis()->SetTitle("dt");
	xyt->GetYaxis()->CenterTitle();
	xyt->DrawClone("colZ");
	
	c9->cd(3);
	gPad->SetLogz(1);
	TH2F *zxt = topology->Project3D("zx");
	zxt->SetStats(0);
	zxt->SetTitle("HR vs. counts");
	zxt->GetXaxis()->SetTitle("counts");
	zxt->GetXaxis()->CenterTitle();
	zxt->GetYaxis()->SetTitle("HR");
	zxt->GetYaxis()->CenterTitle();
	zxt->GetYaxis()->SetTitleOffset(2.);
	zxt->DrawClone("colZ");	
	
	// plot slices
	
	c9->cd(4);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(0, 5);
	TH2F *xyt_HR1 = topology->Project3D("yx");
	xyt_HR1->SetStats(0);
	xyt_HR1->SetTitle("dt vs. counts - HR < 0.5");
	xyt_HR1->GetXaxis()->SetTitle("counts");
	xyt_HR1->GetXaxis()->CenterTitle();
	xyt_HR1->GetYaxis()->SetTitle("dt");
	xyt_HR1->GetYaxis()->CenterTitle();
	xyt_HR1->DrawClone("colZ");
	
	c9->cd(5);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(6, 10);
	TH2F *xyt_HR2 = topology->Project3D("yx");
	xyt_HR2->SetStats(0);
	xyt_HR2->SetTitle("dt vs. counts - 0.5 < HR < 1.");
	xyt_HR2->GetXaxis()->SetTitle("counts");
	xyt_HR2->GetXaxis()->CenterTitle();
	xyt_HR2->GetYaxis()->SetTitle("dt");
	xyt_HR2->GetYaxis()->CenterTitle();
	xyt_HR2->DrawClone("colZ");
	
	c9->cd(6);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(11, 50);
	TH2F *xyt_HR3 = topology->Project3D("yx");
	xyt_HR3->SetStats(0);
	xyt_HR3->SetTitle("dt vs. counts - HR > 1.");
	xyt_HR3->GetXaxis()->SetTitle("counts");
	xyt_HR3->GetXaxis()->CenterTitle();
	xyt_HR3->GetYaxis()->SetTitle("dt");
	xyt_HR3->GetYaxis()->CenterTitle();
	xyt_HR3->DrawClone("colZ");
	
	
	
	TCanvas *c10 = new TCanvas ("c10", "c10", 800, 600);
	//topology->Draw("BOX");
	c10->Divide(2,2);
	
	c10->cd(1);
	topology->GetZaxis()->SetRange(1, 50);
	topology->GetXaxis()->SetRange(15, 100);
	TH1F *x = topology->Project3D("x");
	//x->SetStats(0);
	x->SetTitle("counts");
	x->GetXaxis()->SetTitle("counts");
	x->GetXaxis()->CenterTitle();
	x->GetXaxis()->SetLabelSize(0.06);
	x->GetXaxis()->SetTitleSize(0.06);
	x->GetXaxis()->SetTitleOffset(0.8);
	x->GetYaxis()->SetTitle("n.");
	x->GetYaxis()->CenterTitle();
	x->GetYaxis()->SetLabelSize(0.06);
	x->GetYaxis()->SetTitleSize(0.06);
	x->GetYaxis()->SetTitleOffset(0.8);
	x->DrawClone();

	c10->cd(2);
	//topology->GetZaxis()->SetRange(6, 50);
	TH1F *y = topology->Project3D("y");
	//y->SetStats(0);
	y->SetTitle("duration");
	y->GetXaxis()->SetTitle("duration");
	y->GetXaxis()->CenterTitle();
	y->GetXaxis()->SetLabelSize(0.06);
	y->GetXaxis()->SetTitleSize(0.06);
	y->GetXaxis()->SetTitleOffset(0.8);
	y->GetYaxis()->SetTitle("n.");
	y->GetYaxis()->CenterTitle();
	y->GetYaxis()->SetLabelSize(0.06);
	y->GetYaxis()->SetTitleSize(0.06);
	y->GetYaxis()->SetTitleOffset(0.8);
	y->DrawClone();

	c10->cd(3);
	//topology->GetZaxis()->SetRange(6, 50);
	TH1F *z = topology->Project3D("z");
	//z->SetStats(0);
	z->SetTitle("HR");
	z->GetXaxis()->SetTitle("HR");
	z->GetXaxis()->CenterTitle();
	z->GetXaxis()->SetLabelSize(0.06);
	z->GetXaxis()->SetTitleSize(0.06);
	z->GetXaxis()->SetTitleOffset(0.8);
	z->GetYaxis()->SetTitle("n.");
	z->GetYaxis()->CenterTitle();
	z->GetYaxis()->SetLabelSize(0.06);
	z->GetYaxis()->SetTitleSize(0.06);
	z->GetYaxis()->SetTitleOffset(0.8);
	z->DrawClone();

	c10->cd(4);
	deltat->Draw();
}
