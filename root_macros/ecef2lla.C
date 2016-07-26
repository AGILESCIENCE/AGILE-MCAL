ecef2lla(double X=0., double Y=0., double Z=0.)
{
	X*=0.01;	// nella TM 39.16 e' espresso in cm
	Y*=0.01;
	Z*=0.01;

	// GPS ellipsoid reference (WGS84), data in km
	double a = 6378137.;
	double b = 6356752.31424518;
	double f = 1./298.257223563;
	double e = sqrt((a*a - b*b)/(a*a));
	double e1 = sqrt((a*a - b*b)/(b*b));
	double p, theta;
	double rad2deg = 360./(2.*3.1415); 
	double lon, lat, h, N;

	if (X || Y || Z) {	// single coordinates set	
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
	}
	else {		// if X=Y=Z=0: read a file
		ifstream inFile2;
  		 inFile2.open("/home/mcal/data_analysis/VSB/PKP007568_1_3916_000_t2.txt");
		if (!inFile2) {
			cout << "Unable to open file (2)";
			exit(1); // terminate with error
		}
   		int index = 0;
		double time;
   		while (inFile2 >> X >> Y >> Z >> time) {
       			index++;
			
			if(X) {	// sometimes GPS data are zero!
				X*=0.01;	// nella TM 39.16 e' espresso in cm
				Y*=0.01;
				Z*=0.01;
				
				// auxiliary variables
				p = sqrt(X*X + Y*Y);
				theta = atan((Z*a)/(p*b));
				
				// get LLA geographycal coordinates	
				lon = atan(Y/X);
				lat = atan((Z + e1*e1*b*sin(theta)*sin(theta)*sin(theta)) / (p - e*e*cos(theta)*cos(theta)*cos(theta)));
				N = a/sqrt(1. - e*e*sin(lat)*sin(lat));
				h = p/cos(lat) - N;
				
				// correct for arctangent degeneracy
				lon *= rad2deg;
				lat *= rad2deg;
				if (X<0. && Y>0.) lon += 180.;
				elseif (X<0. && Y<0.) lon -= 180.;
				
				printf("%4d \t%8.3f \t%8.3f \t%10.3f\n", index, lon, lat, h);
			}
   		}
	   	inFile2.close(); 
	}
}
