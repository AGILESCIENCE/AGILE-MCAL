vdist(double era = 102.323566, double edec = -0.527237, double pra = 293.386, double pdec = -26.607, double h = 553.)
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
	
	printf(" \t%8.3f \t%8.3f \t%8.3f \t%8.3f", dE, dN, d, h);

}
