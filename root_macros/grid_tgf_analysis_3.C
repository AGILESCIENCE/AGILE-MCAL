{
	// MM 10/03/10 read file with both forward and reverse data, chose the point to plot appropriately
	// MM 17/02/10 includes counts vs solid angle plot
	// MM 12/02/10 v2 read the output from find_grid_times_2.rb
	// MM 01/02/10 modified to include evt data
	// MM 11/01/10 plot histogram for GRID/TGF waiting time
	
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
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	struct grecord {
		int contact;	
		double t0;	// TGF time 0
		int ievt;	// GRID 3901 event index
		double t;	// GRID event time
		double dt;	// t-t0
	};
	
	struct GRIDevt {
		Double_t	time;
		float		energy;
		float		theta;
		float		phi;
		float		RA;
		float		Dec;
		float		ph_earth; 
		char		brname03;
		char		evstatus;
		double		lon;		// longitude footprint satellite
		double		lat;		// latitude footprint satellite
		double		k;		// magnitudine (km) vettore intersezione con il geoide WGS84 (0 se non c'e' intersezione)
		double		lon_P;		// longitudine del vettore intersezione con il geoide WGS84
		double		lat_P;		// latitudine del vettore intersezione con il geoide WGS84
		double		alpha;		// angolo (deg) footprint - centro della terra - proiezione
		double		dist_L;		// distanza lineare (km) footprint - proiezione
		double		dist_P;		// arco di circonferenza (km) footprint - proiezione
	};

	TH1F *hdt = new TH1F("hdt", "hdt", 100,-0.1, 0.1);
	TH1F *hE = new TH1F("hE", "hE", 1000, 0.1, 10000.1);
	
	// create histograms for ph_earth with bins having constant solid angle
/*	int nph = 6;	// 6 is good binning
	double maxOmega = 0.234*2.*TMath::Pi();	// corrisponde a 80 deg
	double dOmega = maxOmega/(nph * 2. * TMath::Pi());
	double *alpha = new double[nph+1];
	alpha[0] = 0.;
	for (int i=1; i<= nph; i++) {
		alpha[i] = TMath::RadToDeg() * 2. * TMath::ACos(1. - i*dOmega);
		cout << i << "  " << alpha[i] << "  " << i*maxOmega/nph << endl;
	}
	TH1F *hph_earth = new TH1F("hph_earth", "hph_earth", nph, alpha);
	TH1F *hph_earth_R = new TH1F("hph_earth_R", "hph_earth_R", nph, alpha);	// reversed: |ph_earth - 180|
*/

	// create histograms for cos(ph_earth) with constant bin, which marcks the solid angle distribution
	int nbins = 20;
	TH1F *hph_earth = new TH1F("hph_earth", "hph_earth", nbins, -1., 1.);
	TH1F *hph_earth_R = new TH1F("hph_earth_R", "hph_earth_R", nbins, -1., 1.);	// reversed: |ph_earth - 180|
	
	// variables for the KS test
	int na = nb = 0;
	double a[1000];
	double b[1000];
	
	
	/*int nbins = 20;
	double t1 = 1.E-7;
	double t2 = 10.;
	float fact = (t2/t1)**(1./nbins);
	cout << "fact = " << fact << endl;
	float *bins = new float[nbins];
	bins[0] = t1;
	for (int i=1; i<nbins; i++) bins[i] = bins[i-1]*fact;
	TH1F *hdt = new TH1F("hdt", "hdt", nbins-1, bins);
	*/
	
	// canvas for geographical distribution
	TCanvas *c11 = new TCanvas("c11", "c11", 1000,400);	//1000,300
	c11->cd();
	TH2F *htmp = new TH2F("htmp", "htmp", 360, -180., 180., 30, -15., 15.); // 60, -3., 3.
	// TH2F *htmp2 = new TH2F("htmp2", "htmp2", 100, -20., 80., 80, -40., 40.); // 60, -3., 3.
	// TH2F *htmp3 = new TH2F("htmp3", "htmp3", 100, 70., 170., 80, -40., 40.); // 60, -3., 3.
	
	// fill with Earth contour (from example earth.C)
	ifstream in;
	in.open("earth.dat");
	Float_t x,y;
	while (1) {
	in >> x >> y;
	if (!in.good()) break;
		htmp->Fill(x,y, 1);
// 		htmp2->Fill(x,y, 1);
// 		htmp3->Fill(x,y, 1);
	}
	in.close();
	htmp->SetStats(0);
	htmp->SetTitle("");
	htmp->GetXaxis()->SetTitle("Longitude (deg)");
	htmp->GetXaxis()->CenterTitle();
	htmp->GetXaxis()->SetTitleOffset(0.8);
	htmp->GetYaxis()->SetTitle("Latitude (deg)");
	htmp->GetYaxis()->CenterTitle();
	htmp->GetYaxis()->SetTitleOffset(0.8);
	htmp->SetContour(2);
	htmp->Draw("cont3"); // mercator
	
	TLine l1(-180., 2.5, 180., 2.5);
	l1.SetLineStyle(2);
	l1.Draw();
	TLine l2(-180., -2.5, 180., -2.5);
	l2.SetLineStyle(2);
	l2.Draw();

	
	// read GRID data file
		
	ifstream in;
	// in.open("/home/mcal/data_analysis/VSB/grid/grid_results_1b.dat");	// lista GREEN
	// in.open("/home/mcal/data_analysis/VSB/grid/grid_results_2b.dat");	// lista B
	in.open("/home/mcal/data_analysis/VSB/grid/grid_results_complete_1.dat");	// completo (entrambe le liste affiancate)
	grecord gevt, hevt;
	GRIDevt eevt, fevt;
	char aux[20];
	int  iaux, iaux2;
	while (in >> aux >> gevt.contact >> gevt.t0 >> gevt.ievt >> gevt.t >> gevt.dt >> iaux >> eevt.energy >> eevt.theta >> eevt.phi >> eevt.RA >> eevt.Dec >> eevt.ph_earth >> eevt.brname03 >> eevt.evstatus >> eevt.lon >> eevt.lat >> eevt.k >> eevt.lon_P >> eevt.lat_P >> eevt.alpha >> eevt.dist_L >> eevt.dist_P >> hevt.contact >> hevt.t0 >> hevt.ievt >> hevt.t >> hevt.dt >> iaux2 >> fevt.energy >> fevt.theta >> fevt.phi >> fevt.RA >> fevt.Dec >> fevt.ph_earth >> fevt.brname03 >> fevt.evstatus >> fevt.lon >> fevt.lat >> fevt.k >> fevt.lon_P >> fevt.lat_P >> fevt.alpha >> fevt.dist_L >> fevt.dist_P) {
	
		cout << gevt.contact << endl;
		hdt->Fill(gevt.dt);
		
		if (iaux > 0 && fabs(gevt.dt) > 0.010) { // && fabs(gevt.dt) > 0.010
			//if (eevt.k > 0.) hph_earth_R->Fill(eevt.ph_earth);
			//if (eevt.k < 0.) hph_earth_R->Fill(180. - fevt.ph_earth);
			if (eevt.k >= 0.) {
				hph_earth_R->Fill(TMath::Cos(eevt.ph_earth * TMath::DegToRad()));
				b[nb++] = TMath::Cos(eevt.ph_earth * TMath::DegToRad());
			}
			if (eevt.k < 0.) {
				hph_earth_R->Fill(TMath::Cos((180. - fevt.ph_earth) * TMath::DegToRad()));
				b[nb++] = TMath::Cos((180. - fevt.ph_earth) * TMath::DegToRad());	
			}
		}
		
		if (iaux > 0 && gevt.dt > 0.0 && gevt.dt < 0.002) { //&& gevt.dt > 0.0 && gevt.dt < 0.002
			//if (eevt.k > 0.) hph_earth->Fill(eevt.ph_earth);
			//if (eevt.k < 0.) hph_earth->Fill(180. - fevt.ph_earth);
			if (eevt.k >= 0.) {
				hph_earth->Fill(TMath::Cos(eevt.ph_earth * TMath::DegToRad()));
				a[na++] = TMath::Cos(eevt.ph_earth * TMath::DegToRad());
			}
			if (eevt.k < 0.) {
				hph_earth->Fill(TMath::Cos((180. - fevt.ph_earth) * TMath::DegToRad()));
				a[na++] = TMath::Cos((180. - fevt.ph_earth) * TMath::DegToRad());
			}
		
			if (eevt.k != 0.) {	// intersection of projection is not null
				
				hE->Fill(eevt.energy);
				
				// draw a marker for the footprint in the geographical distribution
				TMarker *m = new TMarker(eevt.lon, eevt.lat, 3); //8
				m->SetMarkerColor(3);
				c11->cd(1);
				m->Draw();
				
				// draw a marker for the event projection in the geographical distribution
				TMarker *m_P = new TMarker(eevt.lon_P, eevt.lat_P, 3); //8
				m_P->SetMarkerColor(2);
				c11->cd(1);
				m_P->Draw();
				
				// draw a marker for the REVERSED event projection in the geographical distribution
				TMarker *m_R = new TMarker(fevt.lon_P, fevt.lat_P, 4); //8
				m_R->SetMarkerColor(2);
				c11->cd(1);
				m_R->Draw();
				
				TLine *l = new TLine(eevt.lon, eevt.lat, eevt.lon_P, eevt.lat_P);
				l->SetLineColor(4);
				l->Draw();
				
				TLine *lR = new TLine(eevt.lon, eevt.lat, fevt.lon_P, fevt.lat_P);
				lR->SetLineColor(4);
				lR->Draw();
			} else {
				
				// draw a marker for the footprint in the geographical distribution
				TMarker *m = new TMarker(eevt.lon, eevt.lat, 4); //8
				c11->cd(1);
				m->Draw();
			
			}
		}
	}
	in.close();
	
	// graphic section
	
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	c1->Divide(2,2);
	c1->cd(1);
	hdt->Draw();
	
	TCanvas cph("cph", "cph", 800, 600);
	cph.cd();
	
	hph_earth_R->SetStats(0);
	hph_earth_R->Sumw2();
	// hph_earth_R->Scale(1./180.);	// 180 ms
	hph_earth_R->Draw();
	hph_earth->SetLineColor(2);
	hph_earth->Sumw2();
	// hph_earth->Scale(1./2.);	//2 ms
	hph_earth->Draw("SAME");
	
	c1->cd(3);
	gPad->SetLogx();
	gPad->SetLogy();
	hE->Draw();
	
/*	c1->cd(4);
	TGraph gp(10);
	TGraph gp_R(10);
	double omega_prev = 0.;	// solid angle of the previous bin
	for (int i=1; i<=nph; i++) {
		double omega = 2*TMath::Pi() * (1. - cos(0.5 * TMath::DegToRad() * hph_earth->GetBinLowEdge(i+1)));
		cout << i << "  " << omega << "  " << omega - omega_prev << endl;
		gp.SetPoint(i-1, 0.5*(hph_earth->GetBinLowEdge(i+1) + hph_earth->GetBinLowEdge(i)), hph_earth->GetBinContent(i) / (omega - omega_prev));
		gp_R.SetPoint(i-1, 0.5*(hph_earth_R->GetBinLowEdge(i+1) + hph_earth_R->GetBinLowEdge(i)), hph_earth_R->GetBinContent(i) / (omega - omega_prev));
		omega_prev = omega;
	}
	gp.SetMarkerStyle(2);
	gp_R.SetMarkerStyle(2);
	gp_R.SetMarkerColor(2);
	gp_R.Draw("AP");
	gp.Draw("PSAME");
*/	

	// KS test on [t0, t0+2ms] e |t-t0|>10ms distributions
	int ida[1000];
	int idb[1000];
	TMath::Sort(na, a, ida, 0); // sort in ascending order
	TMath::Sort(nb, b, idb, 0); // sort in ascending order
	double ao[1000];
	double bo[1000];
	for (int i=0; i<na; i++) ao[i] = a[ida[i]];
	for (int i=0; i<nb; i++) bo[i] = b[idb[i]];
	
	for (int i=0; i<na; i++) cout << ao[i] << " ";
	cout << endl << endl;
	for (int i=0; i<nb; i++) cout << bo[i] << " ";
	cout << endl << endl;
	TMath::KolmogorovTest(na, ao, nb, bo, "D");


}