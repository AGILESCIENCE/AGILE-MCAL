{
	// MM 22/4/11 removes the SAA events in offline triggers based on the exposure calculation by FF
	// to be updated with 0.5 deg data
		
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

	
	// load exposure
	
	TH2F *h2e = new TH2F("h2e", "h2e", 144, -180., 180., 5, -2.5, 2.5); 

	ifstream inexpo;
	inexpo.open("/home/mcal/data_analysis/VSB/meteo/Exposure_map_sec_per_bin.dat");
	
	float e[144][5], aux;
	for (int j=1; j<=5; j++) {
		for (int i=1; i<=144; i++) {
			inexpo >> aux;
			h2e->SetBinContent(i, j, aux);
			e[i-1][j-1] = aux;
		}
	}
	inexpo.close();

	
	// load trigger list
	
	
	TH2F *h2t = new TH2F("h2t", "h2t", 144, -180., 180., 5, -2.5, 2.5); 
	TH2F *h2c = new TH2F("h2c", "h2c", 144, -180., 180., 5, -2.5, 2.5); 
	
	ifstream intrg;
	//intrg.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/orbits_009564-19872_Feb2011/cut3.dat");
	intrg.open("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/orbits_009564-19872_Feb2011/cut3_no-onboard-trg.dat");
	
	int cont, nph, nlon, nlat;
	float dt, lon, lat, lc;
	double t0;
	while(intrg >> cont >> t0 >> dt >> lon >> lat >> nph >> lc) {
		h2t->Fill(lon, lat);
		nlon = (int) ((lon + 180.)/2.5) + 1;
		nlat = (int) (lat + 2.5) + 1;
		//cout << lon << "  " << lat << "  " << nlon << "  " << nlat << endl;
		
		if (h2e->GetBinContent(nlon, nlat) > 20000.) {
			h2c->Fill(lon, lat);
			printf("%6d \t%20.6f \t%10.6f \t%10.2f \t%10.2f \t%4d \t%8.2f\n", cont, t0, dt, lon, lat, nph, lc);	
		}
	}

	TCanvas *c1 = new TCanvas();
	c1->Divide(1,3);
	c1->cd(1);
	h2e->Draw("colz");	
	c1->cd(2);
	h2t->Draw("colz");	
	c1->cd(3);
	h2c->Draw("colz");	 
}