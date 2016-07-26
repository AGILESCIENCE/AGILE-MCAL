{
	// MM 26/08/09 created to show the output results produced with sferic_list_analysis.C
	
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
	mcalstyle->SetLabelSize(0.06, "X");
	mcalstyle->SetLabelSize(0.06, "Y");
	gROOT->SetStyle("mcalstyle");

	// input files and path
	char inpath[] = "/home/mcal/data_analysis/VSB/sferics_correlation/complete.output";
	
	int nbin = 10;
	TH1F *dt10 = new TH1F("dt10", "dt10", 50, -25., 25.);
	TH1F *dt25 = new TH1F("dt25", "dt25", 50, -25., 25.);
	
	ifstream fmcal(inpath, ios_base::in);
	int orbit[2000];
	int trg[2000];
	int add[2000], amm[2000], ayy[2000], ah[2000], amin[2000], apflux[2000], acounts[2000];
	float alon[2000], alat[2000], ahr[2000], alc[2000], asec[2000], tmax10[2000], vmax10[2000], tmax25[2000], vmax25[2000];
	int n=0;
	fmcal.clear();
	while (!fmcal.eof()) {
		orbit[n] = 0;
		fmcal >> orbit[n] >> trg[n] >> ayy[n] >> amm[n] >> add[n] >> ah[n] >> amin[n] >> asec[n] >> alon[n] >> alat[n] >> alc[n]  >> ahr[n] >> acounts[n] >> apflux[n] >> tmax10[n] >> vmax10[n] >> tmax25[n] >> vmax25[n];
		if (!orbit[n]) continue;
		
		if (ahr[n] < 0.5) {	// alon[n] >= -30. && alon[n] <= 60. && ahr[n] >= 0.5
			dt10->Fill(tmax10[n]);
			dt25->Fill(tmax25[n]);
		}
		n++;
	}
	n--;
	cout << "read " << n << " events" << endl;
	
	// graphic section
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	c1->Divide(2,2);
	c1->cd(1);
	dt10->Draw();
	c1->cd(2);
	dt25->Draw();

}
