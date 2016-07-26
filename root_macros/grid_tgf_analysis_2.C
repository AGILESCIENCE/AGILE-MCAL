{
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
	int nph = 7;	// 6 is good binning
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
	
	// create histograms for alpha with bins having constant solid angle
/*	double maxOmega = 0.234*2.*TMath::Pi();	// corrisponde a 80 deg
	double dOmega = maxOmega/(nph * 2. * TMath::Pi());
	double *alpha = new double[nph+1];
	alpha[0] = 0.;
	for (int i=1; i<= nph; i++) {
		alpha[i] = TMath::RadToDeg() * 2. * TMath::ACos(1. - i*dOmega);
		cout << i << "  " << alpha[i] << "  " << i*maxOmega/nph << endl;
	} */
	int na = 14;
	TH1F *halpha = new TH1F("halpha", "halpha", na, 0., 21.);

	
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
	gStyle->SetPaperSize(17.8, 4.3);
	TCanvas *c11 = new TCanvas("c11", "c11", 1000,250);	//1000,300
	c11->cd();
	gPad->SetTicks(1,1);
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
	//in.open("/home/mcal/data_analysis/VSB/grid/grid_results_1b.dat");	// lista A (GREEN) good for delta_T plot (fig1 tracker paper)
	// in.open("/home/mcal/data_analysis/VSB/grid/grid_results_2b.dat");	// lista B
	// in.open("/home/mcal/data_analysis/VSB/grid/grid_results_A-B_1.dat");	// lista A+B
	in.open("/home/mcal/data_analysis/VSB/grid/grid_results_reversed_1b.dat");	// lista A reversed
	// in.open("/home/mcal/data_analysis/VSB/grid/grid_bus_1.dat");	// lista BUS (trigger fake)
	grecord gevt;
	GRIDevt eevt;
	char aux[20];
	int  iaux;
	while (in >> aux >> gevt.contact >> gevt.t0 >> gevt.ievt >> gevt.t >> gevt.dt >> iaux >> eevt.energy >> eevt.theta >> eevt.phi >> eevt.RA >> eevt.Dec >> eevt.ph_earth >> eevt.brname03 >> eevt.evstatus >> eevt.lon >> eevt.lat >> eevt.k >> eevt.lon_P >> eevt.lat_P >> eevt.alpha >> eevt.dist_L >> eevt.dist_P) {
	
		// cout << gevt.contact << endl;
	
		hdt->Fill(gevt.dt);
		
		if (iaux > 0 && gevt.dt >= 0. && gevt.dt <= 0.002) { // fabs(gevt.dt) > 0.002
			if (eevt.k > 0.) hph_earth_R->Fill(eevt.ph_earth);
			if (eevt.k < 0.) hph_earth_R->Fill(180. - eevt.ph_earth);
		}
		
		//if (iaux > 0 && fabs(gevt.dt) > 0.001 && fabs(gevt.dt) < 0.003) { //> 0.010
		if (iaux > 0 && gevt.dt > 0. && gevt.dt < 0.002) { //> 0.010
			if (eevt.k > 0.) hph_earth->Fill(eevt.ph_earth);
			if (eevt.k < 0.) hph_earth->Fill(180. - eevt.ph_earth);
			
			printf("%6d \t%.3f \t%.3f \t%.3f \t%.3f \t%.3f \n", gevt.contact, 1000.*gevt.dt, eevt.energy, eevt.k>=0 ? eevt.ph_earth : 180. - eevt.ph_earth, eevt.dist_P, eevt.alpha);
		
			if (eevt.k != 0.) {	// intersection of projection is not null
				
				hE->Fill(eevt.energy);
				halpha->Fill(eevt.alpha);
				
				// draw a marker for the footprint in the geographical distribution
				TMarker *m = new TMarker(eevt.lon, eevt.lat, 8); //8 3
				//m->SetMarkerColor(3);
				c11->cd(1);
				m->Draw();
				
				// draw a marker for the event projection in the geographical distribution
				TMarker *m_P = new TMarker(eevt.lon_P, eevt.lat_P, 4); //8 3
				//m_P->SetMarkerColor(2);
				c11->cd(1);
				m_P->Draw();
				
				TLine *l = new TLine(eevt.lon, eevt.lat, eevt.lon_P, eevt.lat_P);
				//l->SetLineColor(4);
				l->Draw();
			} else {
				
				// draw a marker for the footprint in the geographical distribution
				TMarker *m = new TMarker(eevt.lon, eevt.lat, 22); //8 4
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
	
	c1->cd(2);
	gPad->SetTicks(1,1);
	for (i=1; i<na; i++) {
		cout << i << "  "  << halpha->GetBinLowEdge(i) << "  " << halpha->GetBinLowEdge(i+1) << "  "  << TMath::Cos(TMath::DegToRad()*halpha->GetBinLowEdge(i+2)) << "  "  << 2. * TMath::Pi() * (TMath::Cos(TMath::DegToRad()*halpha->GetBinLowEdge(i)) - TMath::Cos(TMath::DegToRad()*halpha->GetBinLowEdge(i+1))) << endl;
		
		halpha->SetBinContent(i, halpha->GetBinContent(i)/(2. * TMath::Pi() * (TMath::Cos(TMath::DegToRad()*halpha->GetBinLowEdge(i)) - TMath::Cos(TMath::DegToRad()*halpha->GetBinLowEdge(i+1)))));
	}
	//halpha->Sumw2();
	halpha->SetStats(0);
	halpha->Draw();
	
	TCanvas cph("cph", "cph", 800, 600);
	cph.cd();
	
/*	hph_earth_R->SetStats(0);
	hph_earth_R->Sumw2();
	// hph_earth_R->Scale(1./hph_earth_R->GetEntries());
	hph_earth_R->Draw();
	hph_earth->SetLineColor(2); */
	hph_earth->Sumw2();
	// hph_earth->Scale(1./hph_earth->GetEntries());
	hph_earth->SetStats(0);
	hph_earth->Draw(); //"SAME"
	
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

// check consistency with Poisson distribution with KS test

/*
TH1F h("h", "h", 20, 0, 20);
for (int i=1; i<100; i++) h->Fill(hdt->GetBinContent(i));
c1->cd(2);
h.Draw();

TH1F hp("hp", "hp", 20, 0, 20);
for (int i=0; i<20; i++) hp.SetBinContent(i+1, 98.*TMath::Poisson(i,1.18));
hp.SetLineColor(2);
hp.Draw("SAME");
double ki2=0.;
for (int i=1; i<=6; i++) ki2 += ((h.GetBinContent(i) - hp.GetBinContent(i))**2)/hp.GetBinContent(i);


// plot paper Fig.1

gStyle->SetPaperSize(8.6, 4.3)
TCanvas cn("cn", "cn", 800, 400);
gPad->SetTicks(1,1);
hdt->SetStats(0);
hdt->Draw();


// plot the inset in the geographycal distribution

c11.cd();
TPad inpad("inpad", "inpad", 0.12, 0.12, 0.27, 0.8);
inpad.Draw();
TH2F hin("hin", "", 40, -70., -66., 40, -2.5, 1.5);
inpad.cd();
inpad.SetTicks(1,1);
inpad.SetFillStyle(4000);
hin.SetStats(0);
hin.Draw(); 

// draw a marker for the footprint in the geographical distribution
TMarker m0(-66.23, -0.90, 8); //8 3
//m0.SetMarkerColor(3);
m0.Draw();

// draw a marker for the event projection in the geographical distribution
TMarker m1(-68.652, -0.623,4); //8 3
//m1.SetMarkerColor(2);
m1.Draw();
TMarker m2(-68.407, -0.181, 4); //8 3
//m2.SetMarkerColor(2);
m2.Draw();

TLine *lm1 = new TLine(-66.23, -0.90, -68.652, -0.623);
//lm1->SetLineColor(4);
lm1->Draw();
TLine *lm2 = new TLine(-66.23, -0.90, -68.407, -0.181);
//lm2->SetLineColor(4);
lm2->Draw();

TEllipse e1(-68.652, -0.623, 1.13, 0.94, 0., 360.,-6.6);
e1.Draw();
TEllipse e2(-68.407, -0.181, 1.13, 0.94, 0., 360.,-18.4);
e2.Draw();

c11.cd();
TBox b1(-70., -2.5, -66., 1.5);
b1.SetFillStyle(0);
b1.Draw();

TLine *l3 = new TLine(-66., -2.5, -120, -13);
l3.Draw();
TLine *l4 = new TLine(-66., 1.5, -120, 8);
l4.Draw();


// e poi salva con il canvas->SaveAs
*/

}