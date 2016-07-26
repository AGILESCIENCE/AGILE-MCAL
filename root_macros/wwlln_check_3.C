wwlln_check_3(int contact = 9682, double t0=163673775.205678, double tgflon = -6.65, double tgflat = 1.89, double height = 546.64, double dt = 3600., char *outpath = "/home/mcal/data_analysis/VSB/tgf_candidates_complete/light_curves_cat1_HE/")        
{
	// MM 25/02/2013 use simple geometrical calculation for distance (do not use geodetic distance)
	// MM 21/02/2013 better calculates distances and apply propagation to satellite altitude
	// MM 27/12/2012 does not save pictures (to work without graphycs display)
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
	printf("TGF SOD = %12.6f\n", tgfsod);
	
	//TGraph *gwwlln = new TGraph(10000); // all day sferics
	//TGraph *gwwllnred = new TGraph(); // only in +/- Dt time sferics
	//TH1F *hwloc = new TH1F("hwloc", "hwloc", 100, -dt, dt); // time histogram of strokes in the TGF time/geo region
	//TGraph *gwloc = new TGraph(); // (distance, dt) for the strokes in the TGF time/geo region
	
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
	//TH1F *disttest = new TH1F("disttest", "disttest", 2000, -100., 100.);
	
	
	// load the Earth map (from example earth.C)
	
	/*TH2F *htmp = new TH2F("htmp", "htmp", 360, -180., 180., 180, -90., 90.); 
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
	*/
	
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
	float sec, lat, lon, ipeak, dist, tdist;
	double tdist2, sdist, sod, dtcorr;
	double timecorr = 0., timecorr2 = 0., cloudh = 15.;
	double dtmin300 = dt, dtmin600 = dt, dtmin2400 = dt;
	double dtmindist300 = -1., dtmindist600 = -1., dtmindist2400 = -1.;
	double oneoverc = 3.33564e-6; // 1/c (s/km)
	while (fwwlln.getline(wline, 56)) { //&& i < 100000
		sscanf(wline, "%10c,%2d:%2d:%9f,%8f,%9f,%5f,%3d", &date, &hh, &min, &sec, &lat, &lon, &ipeak, &nstat);
		//cout << date << "  " << hh << "  " << min << "  " << sec << "  " << lat << "  " << lon << "  " << ipeak << "  " << nstat << endl;
		//gwwlln->SetPoint(i, lon, lat);
		sod = hh*3600 + min*60 + sec;

		if (fabs(sod - tgfsod) <= dt) { // first time check
			//gwwllnred->SetPoint(ired, lon, lat);
			ired++;
			
			// distance calculation

			dist = 111. * sqrt((lon-tgflon)*(lon-tgflon) + (lat-tgflat)*(lat-tgflat)); // approximate calculation
			tdist = 0.001 * vdist(tgflat, tgflon, lat, lon); // geodetic calculation using WGS84
			tdist2 = vdist2(tgflat, tgflon, lat, lon); // use simple vector geometry assuming a sphere with Earth equatorial radius 
			if (tdist2 <= 2400.) {

				// propagation time correction (assuming a 15km production altitude)
				sdist = vdist3(tgflat, tgflon, height, lat, lon, cloudh); // use simple vector geometry assuming a sphere with Earth equatorial radius
				timecorr = oneoverc * TMath::Sqrt((height - cloudh)*(height - cloudh) + tdist2*tdist2); 
				timecorr2 = oneoverc * sdist;

				//printf("%02d  %02d  %10.6f  %12.6f  %8.3f  %8.3f  %8.3f  %8.3f  %8.3f  %8.3f  %10.6f  %10.6f\n", hh, min, sec, sod, lon, lat, dist, tdist, tdist2, sdist, timecorr, timecorr2);  // debug printout

				//disttest->Fill(TMath::Sqrt((height - cloudh)*(height - cloudh) + tdist2*tdist2)-sdist); // tdist-dist
				//hwloc->Fill(sod - tgfsod);
				//gwloc->SetPoint(iloc, sod - tgfsod, tdist2);
				dtcorr = sod - tgfsod + timecorr2; // sferics TGF time difference corrected for light propagation at the source
				hflash->Fill(dtcorr, tdist2);
				iloc++;
			
				if (tdist2 <= 300. && fabs(dtcorr) <= fabs(dtmin300)) {
					//printf("%8.3f  %10.6f\n", tdist2, dtcorr); // debug print
					dtmin300 = dtcorr;
					dtmindist300 = tdist2;
				}
                                else if (tdist2 > 300. && tdist2 <= 600. && fabs(dtcorr) <= fabs(dtmin600)) {
                                        dtmin600 = dtcorr;
                                        dtmindist600 = tdist2;
                                }
                                else if (tdist2 > 600. && fabs(dtcorr) <= fabs(dtmin2400)) {
                                        dtmin2400 = dtcorr;
                                        dtmindist2400 = tdist2;
                                }

			}
			
		}
		
		i++;
	}
	
	printf("BESTWWLLN %6d  %20.6f  %11.6f  %8.3f  %11.6f  %8.3f  %11.6f  %8.3f  ", contact, t0, dtmin300, dtmindist300, dtmin600, dtmindist600, dtmin2400, dtmindist2400);
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
	/*sprintf(cname, "%06d_%08d_%06d_wwlln01", contact, ts.GetDate(), ts.GetTime());
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
	*/

/*	float aux;
	for (int i=0; i<ntime; i++) { // flash density in unità di flash/(h*10.e6 km^2)
		for (int j=0; j<ndist; j++) {
			aux = hflash->GetBinContent(i+1,j+1);
			hflash->SetBinContent(i+1,j+1, aux*anorm[i*ntime + j]);
		}
	}*/
	
	/*hflash->SetStats(0);
	hflash->SetTitle("");
	hflash->GetXaxis()->SetTitle("Time - t0 (sec)");
	hflash->GetXaxis()->CenterTitle();
	hflash->GetXaxis()->SetTitleOffset(0.8);
	hflash->GetYaxis()->SetTitle("Distance from footprint (km)");
	hflash->GetYaxis()->CenterTitle();
	hflash->GetYaxis()->SetTitleOffset(0.8);
	hflash->Draw("text");
*/
	//printf("TEST  %06d  %10.6f  %10.6f\n", contact, disttest->GetMean(), disttest->GetRMS());

	return 0;
}


// --------------------------------

double vdist(double lat1, double lon1, double lat2, double lon2)
{
// MM 08/01/2013, based on matlab routine at http://www.mathworks.it/matlabcentral/fileexchange/5379-geodetic-distance-on-wgs84-earth-ellipsoid/content/vdist.m

// VDIST - compute distance between points on the WGS-84 ellipsoidal Earth
//         to within a few millimeters of accuracy using Vincenty's algorithm
//
// s = vdist(lat1,lon1,lat2,lon2)
//
// s = distance in meters
// lat1 = GEODETIC latitude of first point (degrees)
// lon1 = longitude of first point (degrees)
// lat2, lon2 = second point (degrees)
//
//  Original algorithm source:
//  T. Vincenty, "Direct and Inverse Solutions of Geodesics on the Ellipsoid
//  with Application of Nested Equations", Survey Review, vol. 23, no. 176,
//  April 1975, pp 88-93
//
// Notes: (1) Error correcting code, convergence failure traps, antipodal corrections,
//            polar error corrections, WGS84 ellipsoid parameters, testing, and comments
//            written by Michael Kleder, 2004.
//        (2) Vincenty describes his original algorithm as precise to within
//            0.01 millimeters, subject to the ellipsoidal model.
//        (3) Essentially antipodal points are treated as exactly antipodal,
//            potentially reducing accuracy by a small amount.
//        (4) Failures for points exactly at the poles are eliminated by
//            moving the points by 0.6 millimeters.
//        (5) Vincenty's azimuth formulas are not implemented in this
//            version, but are available as comments in the code.
//        (6) The Vincenty procedure was transcribed verbatim by Peter Cederholm,
//            August 12, 2003. It was modified and translated to English by Michael Kleder.
//            Mr. Cederholm's website is http://www.plan.aau.dk/~pce/
//        (7) Code to test the disagreement between this algorithm and the
//            Mapping Toolbox spherical earth distance function is provided
//            as comments in the code. The maximum differences are:
//            Max absolute difference: 38 kilometers
//            Max fractional difference: 0.56 percent

// Input check:
if (fabs(lat1)>90. || fabs(lat2)>90.) {
	cerr << "Input latitudes must be between -90 and 90 degrees, inclusive." << endl;
	return -1.;
}

// Supply WGS84 earth ellipsoid axis lengths in meters:
double a = 6378137; // definitionally
double b = 6356752.31424518; // computed from WGS84 earth flattening coefficient definition
// convert inputs in degrees to radians:
double lat1 = lat1 * 0.0174532925199433;
double lon1 = lon1 * 0.0174532925199433;
double lat2 = lat2 * 0.0174532925199433;
double lon2 = lon2 * 0.0174532925199433;
double pi = TMath::Pi();

// correct for errors at exact poles by adjusting 0.6 millimeters: MM: CAN BE NEGLECTED NOW
// if abs(pi/2-abs(lat1)) < 1e-10;
//     lat1 = sign(lat1)*(pi/2-(1e-10));
// end
// if abs(pi/2-abs(lat2)) < 1e-10;
//     lat2 = sign(lat2)*(pi/2-(1e-10));
// end

double f = (a-b)/a;
double U1 = TMath::ATan((1-f)*TMath::Tan(lat1));
double U2 = TMath::ATan((1-f)*TMath::Tan(lat2));
lon1 = fmod(lon1,2.*pi);
lon2 = fmod(lon2,2.*pi);
double L = fabs(lon2-lon1);
if (L > pi) L = 2.*pi - L;

double lambda = L;
double lambdaold = 0.;
double itercount = 0.;
while (!itercount || fabs(lambda-lambdaold) > 1.e-12) {  // force at least one execution
    itercount = itercount+1;
    if (itercount > 50) {
        cerr << "Points are essentially antipodal. Precision may be reduced slightly." << endl;
        lambda = pi;
        break;
    }
    lambdaold = lambda;
    double sinsigma = TMath::Sqrt(pow(TMath::Cos(U2)*TMath::Sin(lambda),2.) + pow(TMath::Cos(U1)*TMath::Sin(U2) - TMath::Sin(U1)*TMath::Cos(U2)*TMath::Cos(lambda),2.));
    double cossigma = TMath::Sin(U1)*TMath::Sin(U2) + TMath::Cos(U1)*TMath::Cos(U2)*TMath::Cos(lambda);
    double sigma = TMath::ATan2(sinsigma,cossigma);
    double alpha = TMath::ASin(TMath::Cos(U1)*TMath::Cos(U2)*TMath::Sin(lambda)/TMath::Sin(sigma));
    double cos2sigmam = TMath::Cos(sigma)-2*TMath::Sin(U1)*TMath::Sin(U2)/pow(TMath::Cos(alpha), 2.);
    double C = f/16*pow(TMath::Cos(alpha), 2.)*(4+f*(4-3*pow(TMath::Cos(alpha), 2.)));
    lambda = L+(1-C)*f*TMath::Sin(alpha)*(sigma+C*TMath::Sin(sigma)*(cos2sigmam+C*TMath::Cos(sigma)*(-1+2*pow(cos2sigmam, 2.))));
    // correct for convergence failure in the case of essentially antipodal points
    if (lambda > pi) {
        cerr << "Points are essentially antipodal. Precision may be reduced slightly." << endl;
        lambda = pi;
        break;
    }
}
double u2 = pow(TMath::Cos(alpha), 2.)*(a*a-b*b)/(b*b);
double A = 1+u2/16384*(4096+u2*(-768+u2*(320-175*u2)));
double B = u2/1024*(256+u2*(-128+u2*(74-47*u2)));
double deltasigma = B*TMath::Sin(sigma)*(cos2sigmam+B/4*(TMath::Cos(sigma)*(-1+2*cos2sigmam*cos2sigmam) - B/6*cos2sigmam*(-3+4*sin(sigma)*sin(sigma))*(-3+4*cos2sigmam*cos2sigmam)));
double s = b*A*(sigma-deltasigma);

return s;
}

// --------------------------------

double vdist2(double lat1, double lon1, double lat2, double lon2)
{
// MM 25/02/2013 use geometric distance and NOT geodetic distance as in vdist.

TVector3 p1(1);
p1.SetMagThetaPhi(1., TMath::PiOver2()*(1. - lat1/90.), TMath::Pi()*lon1/180.);
TVector3 p2(1);
p2.SetMagThetaPhi(1., TMath::PiOver2()*(1. - lat2/90.), TMath::Pi()*lon2/180.);
double dist2 = 6384.*2.*TMath::Sin(0.5*p2.Angle(p1));
return dist2;
}

// --------------------------------

double vdist3(double lat1, double lon1, double h1, double lat2, double lon2, double h2)
{
// MM 25/02/2013 use geometric distance and NOT geodetic distance as in vdist. Calculate distance between satellite and cloud (includes heights

TVector3 p1(1);
p1.SetMagThetaPhi(6384. + h1, TMath::PiOver2()*(1. - lat1/90.), TMath::Pi()*lon1/180.);
TVector3 p2(1);
p2.SetMagThetaPhi(6384. + h2, TMath::PiOver2()*(1. - lat2/90.), TMath::Pi()*lon2/180.);
TVector3 p3(1);
p3 = p1 - p2; 
return p3.Mag();
}

