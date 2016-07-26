wwlln_check(int contact = 24428, double t0=253880563.593489, double tgflon = -116.15, double tgflat = -1.48, double dt = 3600., char *outpath = "/home/mcal/data_analysis/VSB/tgf_candidates_complete/light_curves_cat1_HE/")        
{
	// MM 30/10/2012 reads and analyze WWLLN data provided by Colin Price
	
	gROOT->Reset();
	
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");
	
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	TTimeStamp ts;
	ts.Set((int)  t0, 0, epoch.GetSec(),0);
	UInt_t tgfhh, tgfmin, tgfsec;
	ts.GetTime(kTRUE, 0, &tgfhh, &tgfmin, &tgfsec);
	double tgfsod = tgfhh*3600 + tgfmin*60 + tgfsec + (t0 - TMath::Floor(t0));
	
	TGraph *gwwlln = new TGraph(10000); // all day sferics
	TGraph *gwwllnred = new TGraph(); // only in +/- Dt time sferics
	TH1F *hwloc = new TH1F("hwloc", "hwloc", 100, -dt, dt); // time histogram of strokes in the TGF time/geo region
	TGraph *gwloc = new TGraph(); // (distance, dt) for the strokes in the TGF time/geo region
	
	// 2D histo to collect flash density
	int ntime = 3;
	float atime[] = {-3600., -600., 600., 3600.};
	int ndist = 3;
	float adist[] = {0., 300., 600., 2400.};
	float *anorm;
	anorm = new float[ntime*ndist]; 
	for (int i=0; i<ntime; i++) {
		for (int j=0; j<ndist; j++) {
			anorm[i*ntime + j] = 3600.*1.e6/((atime[i+1] - atime[i])*((adist[j+1])**2 - (adist[j])**2)*TMath::Pi());
			//printf("%2d  %2d  %10.3e\n", i, j, anorm[i*ntime + j]);
		}
	}
	TH2F *hflash = new TH2F("hflash", "hflash", ntime, atime, ndist, adist);
	
	
	// load the Earth map (from example earth.C)
	
	TH2F *htmp = new TH2F("htmp", "htmp", 360, -180., 180., 180, -90., 90.); 
	TH2F *htmploc = new TH2F("htmploc", "htmploc", 60, tgflon-30., tgflon+30., 60, tgflat-30., tgflat+30.); 
	ifstream in;
	in.open("earth.dat");
	Float_t x,y;
	while (1) {
	in >> x >> y;
	if (!in.good()) break;
		htmp->Fill(x,y, 1);
		htmploc->Fill(x,y, 1);
	}
	in.close();
	
	
	// load WWLLN data file
	
	UInt_t year, month, day;
	char wname[200];
	sprintf(wname, "/home/mcal/data_analysis/WWLLN/TGFs/A%8d.loc", ts.GetDate(kTRUE, 0, &year, &month, &day));
	cout << "Loading WWLLN data file " << wname << endl;
	
	//ifstream fwwlln;
	//fwwlln.open(wname);
	
	ifstream fwwlln(wname);
	if (fwwlln.fail()) return 1;
	
	//fwwlln.open();
	char wline[100];
	
	int i=0, ired=0, iloc=0; 
	char date[10];
	int hh, min, nstat;
	float sec, lat, lon, ipeak, dist, dtmindist = 10000., sod;
	double dtmin = dt;
	while (fwwlln.getline(wline, 56)) { //&& i < 100000
		sscanf(wline, "%10c,%2d:%2d:%9f,%8f,%9f,%5f,%3d", &date, &hh, &min, &sec, &lat, &lon, &ipeak, &nstat);
		//cout << date << "  " << hh << "  " << min << "  " << sec << "  " << lat << "  " << lon << "  " << ipeak << "  " << nstat << endl;
		gwwlln->SetPoint(i, lon, lat);
		sod = hh*3600 + min*60 + sec;
		
		if (fabs(sod - tgfsod) <= dt) {
			gwwllnred->SetPoint(ired, lon, lat);
			ired++;
			
			dist = 111. * sqrt((lon-tgflon)*(lon-tgflon) + (lat-tgflat)*(lat-tgflat));
			if (dist <= 2400.) {
				hwloc->Fill(sod - tgfsod);
				gwloc->SetPoint(iloc, sod - tgfsod, dist);
				hflash->Fill(sod - tgfsod, dist);
				iloc++;
			
				if (fabs(sod - tgfsod) <= fabs(dtmin)) {
					dtmin = sod - tgfsod;
					dtmindist = dist;
				}
			}
			
		}
		
		i++;
	}
	
	printf("BESTWWLLN %6d  %20.6f  %11.6f  %8.3f  ", contact, t0, dtmin, dtmindist);
	for (int j=0; j<ndist; j++) { 
		for (int i=0; i<ntime; i++) {
			printf(" %6d", hflash->GetBinContent(i+1,j+1));
		}
	}
	printf("\n");
	
	// ----- graphics section -----
	
	char cname[200];
	char oname[200];
	
	
	// create canvas and plot world map
	sprintf(cname, "%06d_%08d_%06d_wwlln01", contact, ts.GetDate(), ts.GetTime());
	TCanvas *c0 = new TCanvas(cname, cname, 800,600);	//1000,300
	htmp->SetStats(0);
	htmp->SetTitle(cname);
	htmp->GetXaxis()->SetTitle("Longitude (deg)");
	htmp->GetXaxis()->CenterTitle();
	htmp->GetXaxis()->SetTitleOffset(0.8);
	htmp->GetYaxis()->SetTitle("Latitude (deg)");
	htmp->GetYaxis()->CenterTitle();
	htmp->GetYaxis()->SetTitleOffset(0.8);
	htmp->SetContour(2);
	htmp->Draw("cont3"); // mercator
	
	TLine *l1 = new TLine(-180., 2.5, 180., 2.5);
	l1->SetLineStyle(2);
	l1->Draw();
	TLine *l2 = new TLine(-180., -2.5, 180., -2.5);
	l2->SetLineStyle(2);
	l2->Draw();
	
	// plot WWLLN data (all day)
	gwwlln->SetMarkerStyle(6); // 7
	gwwlln->SetMarkerColor(4);
	gwwlln->Draw("PSAME");
	
	// plot WWLLN data (events within dt)
	gwwllnred->SetMarkerStyle(6); // 7
	gwwllnred->SetMarkerColor(3);
	gwwllnred->Draw("PSAME");
	
	// plot TGF ccordinates
	TMarker *mtgf = new TMarker(tgflon, tgflat, 2); 
	mtgf->SetMarkerColor(3);
	mtgf->SetMarkerSize(1);
	mtgf->Draw();
	TEllipse *cs = new TEllipse(tgflon, tgflat, 5.4); //5.4° radius is 600km at the equator
	cs->SetLineColor(3);
	cs->Draw();
	TEllipse *cl = new TEllipse(tgflon, tgflat, 21.6); //5.4° radius is 600km at the equator
	cl->SetLineColor(3);
	cl->SetLineStyle(2);
	cl->Draw();
	
	c0->Update();
	sprintf(oname, "%s/%s.gif", outpath, cname);
	c0->Print(oname);
	
	
	// create canvas and plot world map (zoom on TGF region)
	sprintf(cname, "%06d_%08d_%06d_wwlln02", contact, ts.GetDate(), ts.GetTime());
	TCanvas *c1 = new TCanvas(cname, cname, 800,600);	//1000,300
	htmploc->SetStats(0);
	htmploc->SetTitle(cname);
	htmploc->GetXaxis()->SetTitle("Longitude (deg)");
	htmploc->GetXaxis()->CenterTitle();
	htmploc->GetXaxis()->SetTitleOffset(0.8);
	htmploc->GetYaxis()->SetTitle("Latitude (deg)");
	htmploc->GetYaxis()->CenterTitle();
	htmploc->GetYaxis()->SetTitleOffset(0.8);
	htmploc->SetContour(2);
	htmploc->Draw("cont3"); // mercator
	
	l1->Draw();
	l2->Draw();
	gwwlln->Draw("PSAME");
	gwwllnred->Draw("PSAME");
	mtgf->Draw();
	cs->Draw();
	cl->Draw();
	
	sprintf(oname, "%s/%s.gif", outpath, cname);
	c1->Print(oname);
	
	
	// create time and distance canvas
	sprintf(cname, "%06d_%08d_%06d_wwlln03", contact, ts.GetDate(), ts.GetTime());
	TCanvas *c2 = new TCanvas(cname, cname, 800,600);	//1000,300
	c2->Divide(1,2);
	
	c2->cd(1);
	hwloc->SetStats(0);
	hwloc->SetTitle(cname);
	hwloc->GetXaxis()->SetTitle("Time - t0 (sec)");
	hwloc->GetXaxis()->CenterTitle();
	hwloc->GetXaxis()->SetTitleOffset(0.8);
	hwloc->GetYaxis()->SetTitle("Number of WWLLN strokes");
	hwloc->GetYaxis()->CenterTitle();
	hwloc->GetYaxis()->SetTitleOffset(0.8);
	hwloc->Draw();
	
	c2->cd(2);
	TH2F *htmp2 = new TH2F("htmp2", "htmp2", 100, -dt, dt, 100, 0., 2400.); // 60, -3., 3.
	htmp2->SetStats(0);
	htmp2->SetTitle("");
	htmp2->GetXaxis()->SetTitle("Time - t0 (sec)");
	htmp2->GetXaxis()->CenterTitle();
	htmp2->GetXaxis()->SetTitleOffset(0.8);
	htmp2->GetYaxis()->SetTitle("Distance from footprint (km)");
	htmp2->GetYaxis()->CenterTitle();
	htmp2->GetYaxis()->SetTitleOffset(0.8);
	htmp2->Draw();
	gwloc->SetMarkerStyle(7);
	gwloc->SetMarkerColor(4);
	gwloc->Draw("PSAME");
	
	sprintf(oname, "%s/%s.gif", outpath, cname); 
	c2->Print(oname);
	
	
	// create flash density canvas
	sprintf(cname, "%06d_%08d_%06d_wwlln04", contact, ts.GetDate(), ts.GetTime());
	TCanvas *c3 = new TCanvas(cname, cname, 800,600);	//1000,300
	
/*	float aux;
	for (int i=0; i<ntime; i++) { // flash density in unità di flash/(h*10.e6 km^2)
		for (int j=0; j<ndist; j++) {
			aux = hflash->GetBinContent(i+1,j+1);
			hflash->SetBinContent(i+1,j+1, aux*anorm[i*ntime + j]);
		}
	}*/
	
	hflash->SetStats(0);
	hflash->SetTitle("");
	hflash->GetXaxis()->SetTitle("Time - t0 (sec)");
	hflash->GetXaxis()->CenterTitle();
	hflash->GetXaxis()->SetTitleOffset(0.8);
	hflash->GetYaxis()->SetTitle("Distance from footprint (km)");
	hflash->GetYaxis()->CenterTitle();
	hflash->GetYaxis()->SetTitleOffset(0.8);
	hflash->Draw("text");

	return 0;
}