projectVector(double ra = 106.6149, double dec = -43.99318, double x0 = 6717.70960710422, double y0 = 1632.85447954786, double z0 = -147.114275690763, double time = 159242835.342130) 	// default evt @ contact 8958
{
	// convert satellite coordinates in ECEF to lla 
	
	double par[3]; // lon, lat, h;
	ecef2lla(x0, y0, z0, par);
	
	
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
	double lst = 100.46 + 0.985647*dd + 15.*ut;	// LST at the Greenwich meridian i.e. X axis of ECEF system
	cout << lst << "  "  << lst/360.  << "  "   << fmod(lst, 360.) << endl;
	lst = fmod(lst, 360.);

	double deg2rad = TMath::Pi()/180.;
	double rad2deg = 180./TMath::Pi();
	double ha = lst - ra;	// hour angle HA = LST - RA  If HA negative, then add 360 to bring in range 0 to 360
	if (ha<0.) ha += 360.;
	cout << ha << endl;

	// convert RA,Dec to ECEF, spheric coordinates, using Greenwich LST as Z-axis rotation
	
	TVector3 v0(1);
	v0.SetTheta((90. - dec)*deg2rad);
	v0.SetPhi(ha*deg2rad);
	TVector3 u0 = v0.Unit();
	
	cout << u0[0] << "  "  << u0[1]  << "  "   << u0[2] << endl;

	
	// intersect with the Geoid: solve 2nd order equation 
	
	double a = 6378.137;
	double b = 6356.75231424518;
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
		
		cout << "A = " << A << endl;
		cout << "B = " << B << endl;
		cout << "C = " << C << endl;
		cout << "discr = " << discr << endl;
		cout << "k1 = " << k1 << endl;
		cout << "k2 = " << k2 << endl;
		
		
		// calcola le coordinate dell'intersezione
		
		double par1[3]; // lon, lat, h;
		double x1 = x0 + k*u0[0];
		double y1 = y0 + k*u0[1];
		double z1 = z0 + k*u0[2];
		ecef2lla(x1, y1, z1, par1);
		cout << "x1 = " << x1 << endl;
		cout << "y1 = " << y1 << endl;
		cout << "z1 = " << z1 << endl;
	} else {
		cout << "No intersection!" << endl;
	}
	
	// get satellite footprint coordinates: intersect (x0,y0,z0) with the Geoid
	
	double k0 = a / sqrt(x0*x0 + y0*y0 + z0*z0*c);
	cout << "k0 = " << k0 << endl;
	double xF = k0*x0;
	double yF = k0*y0;
	double zF = k0*z0;
	cout << "xF = " << xF << endl;
	cout << "yF = " << yF << endl;
	cout << "zF = " << zF << endl;
	ecef2lla(xF, yF, zF, par);
	
	
	// get linear distance between footprint and projected event
	if (discr2 > 0.) {
	double lindist = sqrt((x1-xF)*(x1-xF) + (y1-yF)*(y1-yF) + (z1-zF)*(z1-zF));
	cout << "linear distance (km) = " << lindist << endl;
	
	
	// get distance between footprint and projected event as sphere arc
	
	TVector3 vS(x0, y0, z0);
	TVector3 v1(x1, y1, z1);
	double angle = vS.Angle(v1);
	cout << "angular distance (deg) = " << angle*rad2deg << endl;
	double arcdist = a*angle;
	cout << "spheric arc distance (km) = " << arcdist << endl;
	} else {
		cout << "No intersection = no distance!" << endl;		
	}
	
}

void ecef2lla(double X=0., double Y=0., double Z=0., double *par)
{
	X*=1000.;	// nella TM 39.16 e' espresso in cm, nel log e' espresso in km
	Y*=1000.;
	Z*=1000.;

	// GPS ellipsoid reference (WGS84), data in km
	double a = 6378137.;
	double b = 6356752.31424518;
	double f = 1./298.257223563;
	double e = sqrt((a*a - b*b)/(a*a));
	double e1 = sqrt((a*a - b*b)/(b*b));
	double p, theta;
	double rad2deg = 360./(2.*3.1415); 
	double lon, lat, h, N;

	// auxiliary variables
	p = sqrt(X*X + Y*Y);
	theta = atan((Z*a)/(p*b));
	
	// get LLA geographycal coordinates	
	lon = atan(Y/X);
	lat = atan((Z + e1*e1*b*sin(theta)*sin(theta)*sin(theta)) / (p - e*e*cos(theta)*cos(theta)*cos(theta)));
	N = a/sqrt(1. - e*e*sin(lat)*sin(lat));
	h = p/cos(lat) - N;
	
	printf("\tp = %f \tcos(p) = %f \tp/cos(lat) = %f \tN = %f\n", p, cos(lat), p/cos(lat), N);
	printf("Radians: \tlon = %8.3f \tlat = %8.3f \th = %f \n", lon, lat, h);
	printf("Degrees: \tlon = %8.3f \tlat = %8.3f \th = %10.3f\n", lon*rad2deg, lat*rad2deg, h);
	
	par[0] = lon*rad2deg;
	par[1] = lat*rad2deg;
	par[2] = h;
	
	return;
}
