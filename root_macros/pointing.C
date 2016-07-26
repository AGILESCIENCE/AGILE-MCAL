{
  double sr = TMath::Pi()/180.;
  TVector3 pointing(0.,0.,1.);
  
  TVector3 earth(1.,0.,0.);
  earth.SetTheta(39.231*sr);
  earth.SetPhi(135.677*sr);
  earth.SetMag(1.);
  
  TVector3 north(1.,0.,0.);
  north.SetTheta(52.557*sr);
  north.SetPhi(325.497*sr);
  north.SetMag(1.);
  
  TVector3 south;
  south = -north;
  
  TVector3 east;
  east = earth.Cross(north);
  
  TVector3 west;
  west = -east;
  
  TVector3 limb(1.,0.,0.);
  double limb_extension = 60.;
  limb.SetTheta((39.231 + limb_extension)*sr);
  limb.SetPhi(135.677*sr);
  limb.SetMag(1.);
  
  int deg_bin = 1;
  double rad_bin = deg_bin*sr;
  for (int i=0; i<360; i+=deg_bin) {
  	limb.Rotate(rad_bin, earth);
	double theta = limb.Theta()/sr;
	double phi   = limb.Phi()/sr;
	double north_comp = limb.Dot(north);
	double east_comp  = limb.Dot(east);
	double local_angle = 0.;
	if (east_comp != 0.) local_angle = TMath::ATan(north_comp/east_comp)/sr;
	
	cout << i << "  " << theta << "  " << phi << "  " << north_comp << "  " << east_comp << "  " << local_angle << endl;
  }
  
//  double theta = earth.Angle(south);
 }