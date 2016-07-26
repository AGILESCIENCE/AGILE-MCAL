{
	//gROOT->Reset();
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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");
	
	struct duration {
		int TrigNo;
		float T50;
		float e_T50;
		float T50start;
		float T90;
		float e_T90;
		float T90start;
	};
	
	struct main4Br {
		int TrigNo;
		char c4B[3];
		char name[12];
		double JD;
		double Time;
		double RAdeg;
		double DEdeg;
		double GLON;
		double GLAT;
		double PosErr;
		double Angle;
		char PrevFlag;
		char NextFlag;
	};
	
	struct grb3913 {
		int orbit;
		int pkttype;
		float t0;
		char date[11];
		char time[9];
		int alert;
		float tbin;
		float t90X;
		float hrX;
		int nX;
		float bsX;
		float b0X;
		float b1X;
		float b2X;
		float b3X;
		float psX;
		float p0X;
		float p1X;
		float p2X;
		float p3X;
		float fsX;
		float f0X;
		float f1X;
		float f2X;
		float f3X;
		
		float t90Z;
		float hrZ;
		int nZ;
		float bsZ;
		float b0Z;
		float b1Z;
		float b2Z;
		float b3Z;
		float psZ;
		float p0Z;
		float p1Z;
		float p2Z;
		float p3Z;
		float fsZ;
		float f0Z;
		float f1Z;
		float f2Z;
		float f3Z;	
	};
	
		
	struct grb3908 {
		int orbit;
		int pkttype;
		float t0;
		char date[11];
		char time[9];
		int alert;
		float tbin;
		float t90X;
		float hrX;
		int nX;
		float bsX;
		float b0X;
		float b1X;
		float b2X;
		float b3X;
		float psX;
		float p0X;
		float p1X;
		float p2X;
		float p3X;
		float fsX;
		float f0X;
		float f1X;
		float f2X;
		float f3X;
	};
	
	int nbins1 = 30; // 30
	int nbins2 = 18; // 30
	float x0 = 1.E-3;
	float xN = 1.E+3;
	
	float fact1 = pow(xN/x0, 1./nbins1); 
	float *bins1 = new float[nbins1+1];
	bins1[0] = x0;
	for (int i=1; i<=nbins1; i++) bins1[i] = fact1*bins1[i-1];
	
	float fact2 = pow(xN/x0, 1./nbins2); 
	float *bins2 = new float[nbins2+1];
	bins2[0] = x0;
	for (int i=1; i<=nbins2; i++) bins2[i] = fact2*bins2[i-1];
	
	TH1F t90B("t90B", "T90 distribution", nbins1, bins1);
	TH1F t90M3913("t90M3913", "MCAL T90 distribution", nbins2, bins2);
	TH1F t90M3908("t90M3908", "MCAL T90 distribution", nbins2, bins2);
	
	int nbins3 = 15; // 30
	float f0 = 1.;
	float fN = 1.E+5;
	float fact3 = pow(fN/f0, 1./nbins3); 
	float *bins3 = new float[nbins3+1];
	bins3[0] = f0;
	for (int i=1; i<=nbins3; i++) bins3[i] = fact3*bins3[i-1];
	TH1F fM3908("fM3908", "MCAL fluence distribution", nbins3, bins3);
	TH1F pM3908("pM3908", "MCAL peak flux distribution", nbins3, bins3);
	
	string str;

	// read the 3913 MCAL GRB DB
	ifstream f3913;
	f3913.open("/home/martino/Documents/home/martino/AGILE/science/GRB/MCALBURST/MCAL_GRB_3913/grb3913_global_info.dat", ifstream::in);
	int nrowMC=0;	
	while ( f3913.good() && !f3913.eof() ) {
		getline(f3913, str);
		nrowMC++;
	}
	nrowMC--;
	f3913.close();
	f3913.clear();
	grb3913 *aMC = new grb3913[nrowMC];
	
	f3913.open("/home/martino/Documents/home/martino/AGILE/science/GRB/MCALBURST/MCAL_GRB_3913/grb3913_global_info.dat", ifstream::in);
	for (int i=0; i<nrowMC; i++) {
		f3913 >> aMC[i].orbit >> aMC[i].pkttype >> aMC[i].t0 >> aMC[i].date >> aMC[i].time >> aMC[i].alert >> aMC[i].tbin >> aMC[i].t90X >> aMC[i].hrX >> aMC[i].nX >> aMC[i].bsX >> aMC[i].b0X >> aMC[i].b1X >> aMC[i].b2X >> aMC[i].b3X >> aMC[i].psX >> aMC[i].p0X >> aMC[i].p1X >> aMC[i].p2X >> aMC[i].p3X >> aMC[i].fsX >> aMC[i].f0X >> aMC[i].f1X >> aMC[i].f2X >> aMC[i].f3X >> aMC[i].t90Z >> aMC[i].hrZ >> aMC[i].nZ >> aMC[i].bsZ >> aMC[i].b0Z >> aMC[i].b1Z >> aMC[i].b2Z >> aMC[i].b3Z >> aMC[i].psZ  >> aMC[i].p0Z >> aMC[i].p1Z >> aMC[i].p2Z >> aMC[i].p3Z >> aMC[i].fsZ >> aMC[i].f0Z >> aMC[i].f1Z >> aMC[i].f2Z >> aMC[i].f3Z;
		//cout << aMC[i].orbit << "  " << aMC[i].t90Z << endl;
		t90M3913.Fill(aMC[i].t90X);
	}

	ifstream f3908;
	f3908.open("/home/martino/Documents/home/martino/AGILE/science/GRB/MCALBURST/MCAL_GRB/grb_global_info.dat", ifstream::in);
	int nrowMC2=0;	
	while ( f3908.good() && !f3908.eof() ) {
		getline(f3908, str);
		nrowMC2++;
	}
	nrowMC2--;
	f3908.close();
	f3908.clear();
	grb3908 *aMC2 = new grb3908[nrowMC2];
	
	f3908.open("/home/martino/Documents/home/martino/AGILE/science/GRB/MCALBURST/MCAL_GRB/grb_global_info.dat", ifstream::in);
	for (int i=0; i<nrowMC2; i++) {
		f3908 >> aMC2[i].orbit >> aMC2[i].pkttype >> aMC2[i].t0 >> aMC2[i].date >> aMC2[i].time >> aMC2[i].alert >> aMC2[i].tbin >> aMC2[i].t90X >> aMC2[i].hrX >> aMC2[i].nX >> aMC2[i].bsX >> aMC2[i].b0X >> aMC2[i].b1X >> aMC2[i].b2X >> aMC2[i].b3X >> aMC2[i].psX >> aMC2[i].p0X >> aMC2[i].p1X >> aMC2[i].p2X >> aMC2[i].p3X >> aMC2[i].fsX >> aMC2[i].f0X >> aMC2[i].f1X >> aMC2[i].f2X >> aMC2[i].f3X;
		float fluence = aMC2[i].f0X + aMC2[i].f1X + aMC2[i].f2X + aMC2[i].f3X;
		float fpeak = aMC2[i].psX / aMC2[i].tbin;
		cout << i << "  " << aMC2[i].orbit << "  " << aMC2[i].t90X  << "  " << fluence  << "  " << fpeak << endl;
		t90M3908.Fill(aMC2[i].t90X);
		fM3908.Fill(fluence);
		pM3908.Fill(fpeak);
	}
		
	// read the 4Br duration catlaog
	ifstream fduration4B;
	fduration4B.open("/home/martino/Documents/4Br/duration.dat", ifstream::in);
	int nrow=0;
	while ( fduration4B.good() && !fduration4B.eof() ) {
		getline(fduration4B, str);
		nrow++;
	}
	fduration4B.close();
	fduration4B.clear();
	duration *ad = new duration[nrow];
	
	fduration4B.open("/home/martino/Documents/4Br/duration.dat", ifstream::in);
	for (int i=0; i<nrow; i++) {
		fduration4B >> ad[i].TrigNo >> ad[i].T50 >> ad[i].e_T50 >> ad[i].T50start >> ad[i].T90 >> ad[i].e_T90 >> ad[i].T90start;
	}

	// read the main 4Br catalog
	ifstream f4B;
	f4B.open("/home/martino/Documents/4Br/4br_nostar.dat", ifstream::in);
	int nrow4B=0;
	while ( f4B.good() && !f4B.eof() ) {
		getline(f4B, str);
		nrow4B++;
	}
	f4B.close();
	f4B.clear();
	main4Br *a4B = new main4Br[nrow4B];
	
	f4B.open("/home/martino/Documents/4Br/4br_nostar.dat", ifstream::in); 
	for (int i=0; i<nrow4B; i++) {
		f4B >> a4B[i].TrigNo >> a4B[i].c4B  >> a4B[i].name >> a4B[i].JD >> a4B[i].Time >> a4B[i].RAdeg >> a4B[i].DEdeg >> a4B[i].GLON >> a4B[i].GLAT >> a4B[i].PosErr >> a4B[i].Angle >> a4B[i].PrevFlag >> a4B[i].NextFlag;
		//cout << a4B[i].TrigNo << "  " << a4B[i].c4B << "  " << a4B[i].JD << "  " << a4B[i].Time << "  " << a4B[i].PrevFlag << "  " << a4B[i].NextFlag << endl;
	}

	int j=0;
	for (int i=0; i<nrow4B; i++) {
		if (a4B[i].PrevFlag == 'Y' || a4B[i].NextFlag == 'Y') continue;
		while (ad[j].TrigNo < a4B[i].TrigNo) j++;
		if (ad[j].TrigNo == a4B[i].TrigNo) t90B.Fill(ad[j].T90);
	}
	
	// normalize t90 histograms
	t90B.Scale(2./t90B.GetEntries());
	t90M3913.Scale(1./t90M3913.GetEntries());
	t90M3908.Scale(1./t90M3908.GetEntries());
	
	// graphic section
	t90B.SetStats(0);
	t90B.Draw();
	t90M3913.SetLineColor(2);
	t90M3913.Draw("SAME");	
	t90M3908.SetLineColor(3);
	t90M3908.Draw("SAME");	
	gPad->SetLogx();
	
	TCanvas c2;
	gPad->SetLogx();
	fM3908->Draw();
	
	TCanvas c3;
	gPad->SetLogx();
	pM3908->Draw();
	
}