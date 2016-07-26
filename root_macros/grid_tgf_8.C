int grid_tgf_8(int contact =7249, double t0=148782321.371584, double deltat=0.002)
{
	// MM 20/07/10 print also phi angle in routine vdist
	
	// MM 13/07/10 print also interpolated Earth Ra,Dec for position check (see README_GRID.txt)
	//	       added function vdist based on macro vdist.C to calculate East North distance based only on RA,Dec info
	//             based on grid_tgf_6.C

	// MM 22/06/10 uses new logfiles (after 3916 3914 corrector modification)
	// MM 18/06/10 include effects of 15-20 km production height
	
	// MM 17/06/10 print out point coordinates and north/east distance from nadir (after Cohen et al. 2006, Fig.1)
	//             limits analysis to [t0, t0+2ms] interval (publication events)
	//             based on grid_tgf_3.C

	// MM 09/03/10 read reversed evt from a single file (could be optimized)
	//             based on grid_tgf_2.C
	
	// MM 19/02/10 changed repository for 3901 root files (good for class B triggers)
	// MM 03/02/10 based on grid_tgf.C
	//             get sat ECEF coordinates from log file
	//             project photon to Earth based on macro projectVector.C
	
	// MM 01/02/10 gets info also from the evt files
	// MM 25/01/10 returns all the events within a defined time interval with respect to t0 (and not only the closest)
	//             this macro MUST work in astroroot
	// MM 08/01/10 returns the closest event to the input time

	// t0 += 1.;	// to be uncommented to check with uncorrelated dataset
	
	int getevt = 1;
	
	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
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
	};
	
	struct LOGevt {
		Double_t	time;
		double		x;
		double		y;
		double		z;
		double		earth_ra;
		double		earth_dec;
		double		earth_theta;
		double		earth_phi;
	};
	
	TTree 		*data;		// GRB events data tree
	MCALevt 	inevt;
	int 		nevents;
	
	char inpath[200];
	//sprintf(inpath, "/MCAL_PROC/ARCHIVE/%06d/RT%06d_3901.root", contact, contact);	
	sprintf(inpath, "/home/mcal/data_analysis/VSB/grid/data/rootfiles/3901/RT%06d_3901.root", contact);	
	
	// load root events file
	TFile *fi= new TFile(inpath);
	if (fi->IsZombie()) { 	// fi is not a valid root file: exit
		cout << inpath << " is not a valid ROOT file: exiting!" << endl;
		return 1;
	} else {		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
		}
		nevents = data->GetEntries();
	}
	
	// connect the input tree
	data->SetBranchAddress("time", &inevt.time);
	data->SetBranchAddress("mult", &inevt.mult);
	data->SetBranchAddress("bar", inevt.bar);
	data->SetBranchAddress("pdA", inevt.pdA);
	data->SetBranchAddress("pdB", inevt.pdB);
	data->SetBranchAddress("pos", inevt.pos);
	data->SetBranchAddress("E", inevt.E);
	data->SetBranchAddress("Etot", &inevt.Etot);

	// cout << nevents << " events" << endl;
	double *time = new double[nevents];
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		time[i]   = inevt.time;
	}
	
	
	// load the evt file as a root tree
	
	char evtfile[200];
	GRIDevt gevt;
	sprintf(evtfile, "/home/mcal/data_analysis/VSB/grid/data/rootfiles/evt/%06d.evt.gz.root", contact);
	TFile *fevt = new TFile(evtfile);
	TTree *tevt = (TTree *) fevt->Get("EVENTS");
	// tevt->Print();
	int nevt = tevt->GetEntries();
	tevt->SetBranchAddress("TIME", &gevt.time);
	tevt->SetBranchAddress("ENERGY", &gevt.energy);
	tevt->SetBranchAddress("THETA", &gevt.theta);
	tevt->SetBranchAddress("PHI", &gevt.phi);
	tevt->SetBranchAddress("RA", &gevt.RA);
	tevt->SetBranchAddress("DEC", &gevt.Dec);
	tevt->SetBranchAddress("PH_EARTH", &gevt.ph_earth);
	tevt->SetBranchAddress("BRNAME03", &gevt.brname03);
	tevt->SetBranchAddress("EVSTATUS", &gevt.evstatus);
	
	double *gtime = new double[nevt];
	for (int i=0; i< nevt; i++) {
		tevt->GetEntry(i);
		gtime[i] = gevt.time;
	}
	
	
	// load the log file as a root tree
	
	char logfile[200];
	LOGevt levt;
	// sprintf(logfile, "/home/mcal/data_analysis/VSB/grid/data/new_proc/rootfiles/log/%06d.log.gz.root", contact);
	sprintf(logfile, "/home/mcal/data_analysis/VSB/grid/data/rootfiles/log/%06d.log.gz.root", contact);
	TFile *flog = new TFile(logfile);
	TTree *tlog = (TTree *) flog->Get("BinTable");
	// tlog->Print();
	int nlog = tlog->GetEntries();
	tlog->SetBranchAddress("TIME", &levt.time);
	tlog->SetBranchAddress("POSITION_X", &levt.x);
	tlog->SetBranchAddress("POSITION_Y", &levt.y);
	tlog->SetBranchAddress("POSITION_Z", &levt.z);
	tlog->SetBranchAddress("EARTH_RA", &levt.earth_ra);
	tlog->SetBranchAddress("EARTH_DEC", &levt.earth_dec);
	tlog->SetBranchAddress("EARTH_THETA", &levt.earth_theta);
	tlog->SetBranchAddress("EARTH_PHI", &levt.earth_phi);
	
	double *ltime = new double[nlog];
	for (int i=0; i< nlog; i++) {
		tlog->GetEntry(i);
		ltime[i] = levt.time;
	}
	
	LOGevt levt1, levt2, levt0;

	// look for GRID events within deltat from TGF t0
	
	int id = TMath::BinarySearch(nevents, time, t0);
	int n=id;
	while (time[n] <= t0+deltat) {
		double dt = time[n] - t0;
		if (time[n] >= t0) {
			printf("GRID %6d \t%20.6f \t%8d \t%20.6f \t%20.6f ", contact, t0, n, time[n], dt);
	
			
			// get info from evt file
			
			int gid = TMath::BinarySearch(nevt, gtime, time[n]);
			if (gtime[gid] == time[n]) {
				tevt->GetEntry(gid);
				printf("\t%8d \t%6.0f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%c \t%c", gid, gevt.energy, gevt.theta, gevt.phi, gevt.RA, gevt.Dec, gevt.ph_earth, gevt.brname03, gevt.evstatus);
			} else printf("\t%8d \t%6.0f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%c \t%c", -1., -1., -1., -1., -1., -1., -1., 'N', 'N');
			
			
			// get info from the log file
			
			int lid = TMath::BinarySearch(nlog, ltime, time[n]);
			
			tlog->GetEntry(lid);
			levt1.time = levt.time;
			levt1.x = levt.x;
			levt1.y = levt.y;
			levt1.z = levt.z;
			levt1.earth_ra = levt.earth_ra;
			levt1.earth_dec = levt.earth_dec;
			
			tlog->GetEntry(lid+1);
			levt2.time = levt.time;
			levt2.x = levt.x;
			levt2.y = levt.y;
			levt2.z = levt.z;
			levt2.earth_ra = levt.earth_ra;
			levt2.earth_dec = levt.earth_dec;
			
			levt0.time = gtime[gid];
			levt0.x = interpolate(levt0.time, levt1.time, levt1.x, levt2.time, levt2.x);
			levt0.y = interpolate(levt0.time, levt1.time, levt1.y, levt2.time, levt2.y);
			levt0.z = interpolate(levt0.time, levt1.time, levt1.z, levt2.time, levt2.z);
			levt0.earth_ra = interpolate(levt0.time, levt1.time, levt1.earth_ra, levt2.time, levt2.earth_ra);
			levt0.earth_dec = interpolate(levt0.time, levt1.time, levt1.earth_dec, levt2.time, levt2.earth_dec);
			
			// printf("P1 %20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \n", levt1.time, levt1.x, levt1.y, levt1.z, levt1.earth_ra, levt1.earth_dec);
			// printf("P0 %20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \n", levt0.time, levt0.x, levt0.y, levt0.z, levt0.earth_ra, levt0.earth_dec);
			// printf("P2 %20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \t%20.6f \n", levt2.time, levt2.x, levt2.y, levt2.z, levt2.earth_ra, levt2.earth_dec);
			
			printf("\t%20.6f \t%20.6f", levt0.earth_ra, levt0.earth_dec);		
		
			// project vector to the Earth geoid
			// algorithm check using levt0.earth_ra, levt0.earth_dec
			
			// test version: input Earth center coordinates: difference must be almost 0. OK!
			// projectVector(levt0.earth_ra, levt0.earth_dec, levt0.x, levt0.y, levt0.z, levt0.time);
			
			projectVector(gevt.RA, gevt.Dec, levt0.x, levt0.y, levt0.z, levt0.time);
			
			// convert satellite coordinates in ECEF to lla 
			double par[3]; // lon, lat, h;
			ecef2lla(levt0.x, levt0.y, levt0.z, par);
			
			// calculate E,N linear distance with only RA,Dec info (approximation)
			vdist(levt0.earth_ra, levt0.earth_dec, gevt.RA, gevt.Dec, par[2]*0.001);
		
			printf("\n");
		}
		
		n++;
	}


		
	delete time;
	delete gtime;
	delete ltime;
	delete tevt;
	fevt->Close();
	fi->Close();
	flog->Close();
	return 0;
}

double interpolate(double x, double x1, double y1, double x2, double y2) {

	// return linear interpolation between points (x1,y1) (x2,y2) at abscissa x
	
	double m = (y2 - y1) / (x2 - x1);
	return m*x + y1 - m*x1;
}

// --------------------------------

projectVector(double ra = 106.6149, double dec = -43.99318, double x0 = 6717.70960710422, double y0 = 1632.85447954786, double z0 = -147.114275690763, double time = 159242835.342130) 	// default evt @ contact 8958
{
	int verbose = 0;	// if 1: print debug info, if 0: print output
	
	// convert satellite coordinates in ECEF to lla 
	
	double par[3]; // lon, lat, h;
	ecef2lla(x0, y0, z0, par);
	if (verbose == 0) printf(" \t%8.3f \t%8.3f", par[0], par[1]);
	
	// calculate Local Sidereal Time
	// LST = 100.46 + 0.985647 * d + long + 15*UT   come riportato in http://www.stargazing.net/kepler/altaz.html

	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	UInt_t telapsed = (UInt_t) time;
	TTimeStamp ts;
	ts.Set((int) telapsed , 0, epoch.GetSec(),0);
	TString stime(ts.AsString("c"));
	int hour, min, sec;
	char aux[20];
	sscanf(stime.Data(), "%s %2d:%2d:%2d", &aux, &hour, &min, &sec);
	double ut = hour + min/60. + sec/3600.;
	int dd = (int) time/86400. + 1459.5;	// days between J2000 and Epoch
	double lst = 100.46 + 0.985647*dd + 15.*ut;	// LST at the Greenwich meridian i.e. X axis of ECEF system. NON dovrebbe essere necessario  + par[0]
	if (verbose) cout << endl << "LST = " << lst << "  "  << lst/360.  << "  "   << fmod(lst, 360.) << "  " << fmod(lst, 360.)*24./360. << endl;
	lst = fmod(lst, 360.);

	double deg2rad = TMath::Pi()/180.;
	double rad2deg = 180./TMath::Pi();
	double ha = lst - ra;	// hour angle HA = LST - RA  If HA negative, then add 360 to bring in range 0 to 360
	if (ha<0.) ha += 360.;
	// cout << ha << endl;

	// convert RA,Dec to ECEF, spheric coordinates, using Greenwich LST as Z-axis rotation
	
	TVector3 v0(1);
	v0.SetTheta((90. - dec)*deg2rad);
	v0.SetPhi(-ha*deg2rad);	// NB: FUNZIONA col segno meno (verificato con RA,Dec del centro della terra
	TVector3 u0 = v0.Unit();
	
	// cout << u0[0] << "  "  << u0[1]  << "  "   << u0[2] << endl;

	
	// intersect with the Geoid: solve 2nd order equation 
	
	double a = 6378.137;
	double b = 6356.75231424518;
	
	// MM 18/06/10 include a 15-20km height above mean sea level
	
	double hp = 0.;
	a += hp;
	b += hp;
	
	double c = (a/b)*(a/b);
	
	double A = u0[0]*u0[0] + u0[1]*u0[1] + u0[2]*u0[2]*c;
	double B = (x0*u0[0] + y0*u0[1] + z0*u0[2]*c);
	double C = x0*x0 + y0*y0 + z0*z0*c -a*a;
	double discr2 = B*B - A*C;
	
	if (discr2 > 0.) {
		double discr  = sqrt(discr2);
		double k1 = (-B - discr)/A; 
		double k2 = (-B + discr)/A; 
		double k = k2;
		if (fabs(k1)<fabs(k2)) k=k1;
		
		// calcola le coordinate dell'intersezione
		
		double par1[3]; // lon, lat, h;
		double x1 = x0 + k*u0[0];
		double y1 = y0 + k*u0[1];
		double z1 = z0 + k*u0[2];
		ecef2lla(x1, y1, z1, par1);
		
		if (verbose) {
			cout << "A = " << A << endl;
			cout << "B = " << B << endl;
			cout << "C = " << C << endl;
			cout << "discr = " << discr << endl;
			cout << "k1 = " << k1 << endl;
			cout << "k2 = " << k2 << endl;
			cout << "x1 = " << x1 << endl;
			cout << "y1 = " << y1 << endl;
			cout << "z1 = " << z1 << endl;
			
		} else printf(" \t%8.3f \t%8.3f \t%8.3f", k, par1[0], par1[1]);
	} else {
		if (verbose) cout << "No intersection!" << endl;
		else printf(" \t%8.3f \t%8.3f \t%8.3f", 0., 0., 0.);
	}
	
	// get satellite footprint coordinates: intersect (x0,y0,z0) with the Geoid
	
	double k0 = a / sqrt(x0*x0 + y0*y0 + z0*z0*c);
	double xF = k0*x0;
	double yF = k0*y0;
	double zF = k0*z0;
	
	if (verbose) {
		cout << "k0 = " << k0 << endl;
		cout << "xF = " << xF << endl;
		cout << "yF = " << yF << endl;
		cout << "zF = " << zF << endl;
	}
	
	ecef2lla(xF, yF, zF, par);
	
	
	// get linear distance between footprint and projected event
	if (discr2 > 0.) {
		double lindist = sqrt((x1-xF)*(x1-xF) + (y1-yF)*(y1-yF) + (z1-zF)*(z1-zF));
		
		
		// get distance between footprint and projected event as sphere arc
		
		TVector3 vS(x0, y0, z0);
		TVector3 v1(x1, y1, z1);
		double angle = vS.Angle(v1);
		double arcdist = a*angle;
		
		if (verbose) {
			cout << "angular distance (deg) = " << angle*rad2deg << endl;
			cout << "linear distance (km) = " << lindist << endl;
			cout << "spheric arc distance (km) = " << arcdist << endl;
			
		} else printf(" \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f", angle*rad2deg, lindist, arcdist, (par1[0]-par[0])*TMath::DegToRad()*a,  (par1[1]-par[1])*TMath::DegToRad()*a);
		
	} else {
		if (verbose) cout << "No intersection = no distance!" << endl;
		else printf(" \t%8.3f \t%8.3f \t%8.3f", 0., 0., 0.);
	}
	
}

// --------------------------------

void ecef2lla(double X=0., double Y=0., double Z=0., double *par)
{

	X*=1000.;	// nella TM 39.16 e' espresso in cm, nel log e' espresso in km
	Y*=1000.;
	Z*=1000.;

	int verbose = 0;
	
	// GPS ellipsoid reference (WGS84), data in km
	double a = 6378137.;
	double b = 6356752.31424518;
	double f = 1./298.257223563;
	double e = sqrt((a*a - b*b)/(a*a));
	double e1 = sqrt((a*a - b*b)/(b*b));
	double p, theta;
	double rad2deg = TMath::RadToDeg();
//	double rad2deg = 180./3.1415;
	double lon, lat, h, N, h2;
	
	if(X) {	// sometimes GPS data are zero!
	
		// auxiliary variables
		p = TMath::Sqrt(X*X + Y*Y);
		theta = TMath::ATan((Z*a)/(p*b));
		
		// get LLA geographycal coordinates	
		lon = TMath::ATan(Y/X);
		lat = TMath::ATan((Z + e1*e1*b*TMath::Sin(theta)*TMath::Sin(theta)*TMath::Sin(theta)) / (p - e*e*TMath::Cos(theta)*TMath::Cos(theta)*TMath::Cos(theta)));
		N = a/TMath::Sqrt(1. - e*e*TMath::Sin(lat)*TMath::Sin(lat));
		h = p/TMath::Cos(lat) - N;
		
		// test rough estimation of altitude (propagation errors?)
		h2 = 1000.*TMath::Sqrt((X*0.001)*(X*0.001) + (Y*0.001)*(Y*0.001) + (Z*0.001)*(Z*0.001)) - a;
		
		// correct for arctangent degeneracy
		lon *= rad2deg;
		lat *= rad2deg;
		if (X<0. && Y>0.) lon += 180.;
		else if (X<0. && Y<0.) lon -= 180.;
	
	} else lon = lat = h = -1.;

	// auxiliary variables
/*	p = sqrt(X*X + Y*Y);
	theta = atan((Z*a)/(p*b));
	
	// get LLA geographycal coordinates	
	lon = atan(Y/X);
	lat = atan((Z + e1*e1*b*sin(theta)*sin(theta)*sin(theta)) / (p - e*e*cos(theta)*cos(theta)*cos(theta)));
	N = a/sqrt(1. - e*e*sin(lat)*sin(lat));
	h = p/cos(lat) - N;
*/	
	if (verbose) {
		printf("\tp = %f \tcos(p) = %f \tp/cos(lat) = %f \tN = %f\n", p, cos(lat), p/cos(lat), N);
		printf("Radians: \tlon = %8.3f \tlat = %8.3f \th = %f \n", lon, lat, h);
		printf("Degrees: \tlon = %8.3f \tlat = %8.3f \th = %10.3f\n", lon*rad2deg, lat*rad2deg, h);
	}
	
	par[0] = lon;
	par[1] = lat;
	par[2] = h;
//	par[2] = h2;
	
	return;
}

// --------------------------------

void vdist(double era = 102.323566, double edec = -0.527237, double pra = 293.386, double pdec = -26.607, double h = 553.)
{
	// MM 13/07/10 alternative check of GRID event / Earth intersection using only RA,Dec info
	//	era, edec Earth RA,Dec (degrees)
	//	pra, pdec GRID event RA,Dec (degrees)
	//	h orbital height (km)
	
	// 	TVector3 Phi   = RA
	// 	TVector3 Theta = PI/2 - Dec
	
	// default data for #7467 (reverse event)
	
	// Earth center unit vector
	TVector3 ve(1);
	ve.SetPhi(era*TMath::DegToRad());
	ve.SetTheta((90. - edec)*TMath::DegToRad());
	ve.SetMag(1.);
	
	// GRID event unit vector
	TVector3 vp(1);
	vp.SetPhi(pra*TMath::DegToRad());
	vp.SetTheta((90. - pdec)*TMath::DegToRad());
	vp.SetMag(1.);
	
	double phi = vp.Angle(ve)*TMath::RadToDeg();
	//cout << "phi (deg) = " << phi << endl;
	
	if (phi > 90.) {	// NB: pra,pdec si riferiscono all'evento con verso nel campo di vista GRID (anche se reversed)
		vp = -vp;	
	}
	phi = vp.Angle(ve)*TMath::RadToDeg();
	//cout << "phi reversed (deg) = " << phi << endl;
	
	// create a local reference system (North, East)
	TVector3 vn(1);		// Earth north pole direction
	vn.SetPhi(0.);
	vn.SetTheta(0.);
	vn.SetMag(1.);
	
	TVector3 vE(1);		// local East unit vector
	vE = ve.Cross(vn);
	
	TVector3 vN(1);		// local North unit vector
	vN = vE.Cross(ve);
	
	// calculate North, East distances from footprint
	double dE = h / TMath::Cos(vp.Angle(ve)) * (vp.Dot(vE));	// GRID event projection East  distance from footprint
	double dN = h / TMath::Cos(vp.Angle(ve)) * (vp.Dot(vN));	// GRID event projection North distance from footprint
	double d  = TMath::Sqrt(dE*dE + dN*dN);	// distance from footprint
	double dphi = h * TMath::Tan(vp.Angle(ve));	// distance calculated based on phi_Earth angle (should be correct!)
	
	//cout << "East  distance (km) = " << dE << endl;
	//cout << "North distance (km) = " << dN << endl;
	//cout << "Total distance (km) = " << d << endl;
	//cout << "Exp.  distance (km) = " << dphi << endl;
	
	printf(" \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f", dE, dN, d, h, phi);
	return;

}
