// MM 01/06/2010
// for DAFNE simulations by INFN-Pv: check whether are there simulation data for MCAL only

{
	ifstream in, in2, in3;
	Int_t plane, bar, c1, c2, c6;
	Float_t energy, eA, eB, c3, c4, c5;
		
	// read MCAL simulation data
	cout << "reading MCAL data" << endl;
	in.open("/home/mcal/data_analysis/calibration/frascati/2345/calo_2345.dat");
	int nmcal = 100000;
	int *evtm = new int[nmcal];
	int im=0;
	while (in >> evtm[im++] >> plane >> bar >> energy >> eA >> eB);
	in.close();
	
	// read stripx simulation data
	cout << "reading stripx data" << endl;
	in2.open("/home/mcal/data_analysis/calibration/frascati/2345/stripx_2345.dat");
	int nstripx = 900000;
	int *evtsx = new int[nstripx];
	int isx=0;
	while (in2 >> evtsx[isx++] >> c1 >> c2 >> c3 >> c4 >> c5 >> c6);
	in2.close();
	
	// read stripy simulation data
	cout << "reading stripy data" << endl;
	in3.open("/home/mcal/data_analysis/calibration/frascati/2345/stripy_2345.dat");
	int nstripy = 900000;
	int *evtsy = new int[nstripy];
	int isy=0;
	while (in3 >> evtsy[isy++] >> c1 >> c2 >> c3 >> c4 >> c5 >> c6);
	in3.close();

	cout << "n lines MCAL = " << im << "   n lines stripx = " << isx <<  "   n lines stripy = " << isy << endl;
	
	// main search
	int evt0 = -1;
	int nevt=0;
	int idx, idy;
	for (int i=0; i<im; i++) {
		//if (i%1000 == 0) cout << "...processing evt " << i << endl;
		if (evtm[i] != evt0) {
			nevt++;
			evt0 = evtm[i];		
			idx = TMath::BinarySearch(isx, evtsx, evt0);
			idy = TMath::BinarySearch(isy, evtsy, evt0);
			if (evtsx[idx] != evt0 && evtsy[idy] != evt0) cout << "*** MCAL only event " << evt0 << endl;
		}
	
	}
	cout << "total number of different events in MCAL = " << nevt << endl;
	
	delete evtm;
	delete evtsx; 
	delete evtsy; 
}