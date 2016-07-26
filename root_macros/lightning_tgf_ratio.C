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
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	
	// load annual flash rate
	
	TH2F *h2f = new TH2F("h2f", "h2f", 144, -180., 180., 5, -2.5, 2.5); 

	ifstream inflash;
	inflash.open("/home/mcal/data_analysis/VSB/meteo/flashes_per_square_km_per_year.dat");
	
	float aux;
	float f[144][5];
	for (int j=1; j<=5; j++) {
		for (int i=1; i<=144; i++) {
			inflash >> aux;
			h2f->SetBinContent(i, j, aux);
			f[i-1][j-1] = aux;
		}
	}
	inflash.close();

	TCanvas *c1 = new TCanvas();
	c1->Divide(1,4);
	c1->cd(1);
	gPad->SetLogz(1);
	h2f->Draw("colz");

	
	// load exposure
	
	TH2F *h2e = new TH2F("h2e", "h2e", 144, -180., 180., 5, -2.5, 2.5); 

	ifstream inexpo;
	inexpo.open("/home/mcal/data_analysis/VSB/meteo/Exposure_map_sec_per_bin.dat");
	
	float e[144][5];
	for (int j=1; j<=5; j++) {
		for (int i=1; i<=144; i++) {
			inexpo >> aux;
			h2e->SetBinContent(i, j, aux);
			e[i-1][j-1] = aux;
		}
	}
	inexpo.close();

	c1->cd(2);
	gPad->SetLogz(1);
	h2e->Draw("colz");
	
	
	// calculate lightning x exposure maps product
	
	TH2F *h2p = new TH2F("h2p", "h2p", 144, -180., 180., 5, -2.5, 2.5); 
	
	float p[144][5];
	for (int j=1; j<=5; j++) {
		for (int i=1; i<=144; i++) {
			p[i-1][j-1] = f[i-1][j-1] * e[i-1][j-1];
			h2p->SetBinContent(i, j, p[i-1][j-1]);
		}
	}

	c1->cd(3);
	gPad->SetLogz(1);
	h2p->Draw("colz");

	float sec2y = 1./(365.*86400.);
	float binsize = 2.5 * 1. * 111.70 * 111.70;
	
	// all the World
	float avge=0., totf=0.;
	for (int j=1; j<=5; j++) {
		for (int i=1; i<=144; i++) {
			avge += e[i-1][j-1];
			totf += p[i-1][j-1];
		}
	}
	avge /= (144.*5.);
	cout << "average exposure per bin = " << avge << endl;
	
	totf *= (sec2y * binsize);
	cout << "binsize = " << binsize << endl;
	cout << "total flash number = " << totf << endl;
	cout << "TGF per flash = " << 118. / totf << endl;
	cout << "flash per TGF= " << totf / 118. << endl;
	
	// Central America
	float avgeCA=0., totfCA=0.;
	for (int j=1; j<=5; j++) {
		for (int i=33; i<=52; i++) {
			avgeCA += e[i-1][j-1];
			totfCA += p[i-1][j-1];
		}
	}
	avgeCA /= (20.*5.);
	cout << endl << "Central America average exposure per bin = " << avgeCA << endl;
	
	totfCA *= (sec2y * binsize);
	cout << "Central America total flash number = " << totfCA << endl;
	cout << "Central America TGF per flash = " << 22. / totfCA << endl;
	cout << "Central America flash per TGF= " << totfCA / 22. << endl;
	
	// Africa
	float avgeA=0., totfA=0.;
	for (int j=1; j<=5; j++) {
		for (int i=61; i<=92; i++) {
			avgeA += e[i-1][j-1];
			totfA += p[i-1][j-1];
		}
	}
	avgeA /= (32.*5.);
	cout << endl << "Africa average exposure per bin = " << avgeA << endl;
	
	totfA *= (sec2y * binsize);
	cout << "Africa total flash number = " << totfA << endl;
	cout << "Africa TGF per flash = " << 60. / totfA << endl;
	cout << "Africa flash per TGF= " << totfA / 60. << endl;
	
	// South East Asia
	float avgeSA=0., totfSA=0.;
	for (int j=1; j<=5; j++) {
		for (int i=105; i<=132; i++) {
			avgeSA += e[i-1][j-1];
			totfSA += p[i-1][j-1];
		}
	}
	avgeSA /= (28.*5.);
	cout << endl << "South East Asia average exposure per bin = " << avgeSA << endl;
	
	totfSA *= (sec2y * binsize);
	cout << "South East Asia total flash number = " << totfSA << endl;
	cout << "South East Asia TGF per flash = " << 28. / totfSA << endl;
	cout << "South East Asia flash per TGF= " << totfSA / 28. << endl;
	

	
	// load TGFs list (Mar 2009 - Feb 2010)

	ifstream intgf;
	//intgf.open("/home/mcal/data_analysis/VSB/meteo/list_Mar09-Feb10.dat");
	intgf.open("/home/mcal/data_analysis/VSB/meteo/list_Mar09-Feb10_FF.dat");
	
	float tlon, tlat, lt, tt;
	int cont, trg, cpeak, counts;
	char date[20], time[20];
	
	float alat[]={-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};
	
//	float alat[]={-2.5, -1.5, 1.5, 2.5};
	
	// full version
	float alon[]={-180, -177.5, -175, -172.5, -170, -167.5, -165, -162.5, -160, -157.5, -155, -152.5, -150, -147.5, -145, -142.5, -140, -137.5, -135, -132.5, -130, -127.5, -125, -122.5, -120, -117.5, -115, -112.5, -110, -107.5, -105, -102.5, -100, -97.5, -95, -92.5, -90, -87.5, -85, -82.5, -80, -77.5, -75, -72.5, -70, -67.5, -65, -62.5, -60, -57.5, -55, -52.5, -50, -47.5, -45, -42.5, -40, -37.5, -35, -32.5, -30, -27.5, -25, -22.5, -20, -17.5, -15, -12.5, -10, -7.5, -5, -2.5, 0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 22.5, 25, 27.5, 30, 32.5, 35, 37.5, 40, 42.5, 45, 47.5, 50, 52.5, 55, 57.5, 60, 62.5, 65, 67.5, 70, 72.5, 75, 77.5, 80, 82.5, 85, 87.5, 90, 92.5, 95, 97.5, 100, 102.5, 105, 107.5, 110, 112.5, 115, 117.5, 120, 122.5, 125, 127.5, 130, 132.5, 135, 137.5, 140, 142.5, 145, 147.5, 150, 152.5, 155, 157.5, 160, 162.5, 165, 167.5, 170, 172.5, 175, 177.5, 180};
		
//	float alon[]={-180, -82.5, -77.5, -72.5, -67.5, -62.5, -57.5, -12.5, -7.5, -2.5, 2.5, 7.5, 12.5, 17.5, 22.5,  27.5, 32.5, 87.5, 92.5, 97.5, 102.5, 107.5, 112.5, 117.5, 122.5, 127.5, 132.5, 137.5, 142.5, 180};
	
	TH2F *h2t = new TH2F("h2t", "h2t", 144, alon, 5, alat); 
	
//	while (intgf >> cont >> trg >> date >> time >> tlon >> tlat >> cpeak >> counts >> lt >> tt) {
	while (intgf >> cont >> trg >> date >> time >> tlon >> tlat) {
		h2t->Fill(tlon, tlat);
	}
	
	intgf.close();

	c1->cd(4);
	//gPad->SetLogz(1);
	h2t->Draw("colz");

	
}