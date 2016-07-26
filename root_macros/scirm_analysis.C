{	
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
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	// open root file
	TFile fresults("results.root", "UPDATE");
	
	struct rm {
		int id;
		float t;
		int xtot;
		int ztot;
		int x[10];
		int z[10];
		float xhr;
		float zhr;
		long int ac[5];
		double lon;
		double lat;
	};
	
	struct coord {
		double t;
		double lon;
		double lat;
	};
	
	// load longitude values
	
	ifstream fcoord("/home/mcal/data_analysis/scirm/3913_lon_lat.dat", ios_base::in);
	int id;
	double auxt, auxlon, auxlat;
	coord acoord[20000];
	fcoord.clear();
	while (!fcoord.eof()) {
		fcoord >> id >> auxt >> auxlon >> auxlat;
		if (id>0 && id<20000) {
			acoord[id].t   = auxt;
			acoord[id].lon = auxlon;
			acoord[id].lat = auxlat;
		}
	}
	fcoord.close();
	
	// load boost-eclipse file
	
	ifstream fboost("/home/mcal/data_analysis/scirm/BOOST_up_to_now_sorted.dat", ios_base::in);
	double boostini[40000];
	double boostfin[40000];
	double auxcont;
	char caux[20];
	fboost.clear();
	int nboost=0;	
	
	while (!fboost.eof() ) {
		fboost >> auxcont >> boostini[nboost] >> boostfin[nboost] >> caux;
		// cout << nboost << "  " << boostini[nboost] << endl;
		nboost++;
	}
	nboost--;
	fboost.close();
	
	// create global histo
	TH3F *h3xtot = new TH3F("h3xtot", "h3xtot", 2000, 0., 20000., 36, 0., 360., 400, 0., 100.);
	TH3F *h3ztot = new TH3F("h3ztot", "h3ztot", 2000, 0., 20000., 36, 0., 360., 400, 0., 100.);
	TH3F *h3xhr = new TH3F("h3xhr", "h3xhr", 2000, 0., 20000., 36, 0., 360., 400, 0., 100.);
	TH3F *h3zhr = new TH3F("h3zhr", "h3zhr", 2000, 0., 20000., 36, 0., 360., 400, 0., 100.);
	
	// loop on orbits
	
	int inicont =  3061;
	int fincont = 3062;
	
	for (int cont=inicont; cont<fincont; cont++) {	// loop on selected contacts
		
		int iboost=0;
	
		char dataname1[200];
		sprintf(dataname1, "/home/mcal/data_analysis/scirm/scirm_%06d.dat", cont);
		ifstream fscirm1(dataname1, ios_base::in);
		
		char dataname2[200];
		sprintf(dataname2, "/home/mcal/data_analysis/scirm/scirm_%06d.dat", cont+1);
		ifstream fscirm2(dataname2, ios_base::in);
		
		double t01, t02;
		int nrows1, nrows2, nrows;
		char saux[10];
		int ns=0;
		int npt=0;	// number of graph points, must be different from ns
		
		fscirm1.clear();
		fscirm1 >> t01 >> nrows1;
		for (int i=0; i<29; i++) fscirm1 >> saux;
		
		fscirm2.clear();
		fscirm2 >> t02 >> nrows2;
		for (int i=0; i<29; i++) fscirm2 >> saux;
		nrows = nrows1+nrows2;
		rm *data = new rm[nrows];
		
		printf("%06d \t%20.3f \t%20.3f \t%10.3f\n%", cont, t01, acoord[cont].t, acoord[cont].lon);
		printf("%06d \t%20.3f \t%20.3f \t%10.3f\n%", cont+1, t02, acoord[cont+1].t, acoord[cont+1].lon);
		
		// interpolate longitude
		double l1, l2, t1, t2, a, b;
		l1 = acoord[cont+1].lon;
		l2 = acoord[cont+2].lon;
		t1 = acoord[cont+1].t - t01;
		t2 = acoord[cont+2].t - t01;
		if (t2-t1 > 10000.) {
			cout << "*** cannot interpolate longitude at orbit " << cont+1 << endl;
			continue;
		}
		l2 += 360.;
		b = (l2-l1)/(t2-t1);
		a = l1 - t1*b;
		
		bool flag;
		
		// create graphs
		TGraph *gxtot = new TGraph(10);
		TGraph *gztot = new TGraph(10);
		TGraph *gxhr = new TGraph(10);
		TGraph *gzhr = new TGraph(10);
		gxtot->SetTitle("X RM E>0.3 MeV");
		gztot->SetTitle("Z RM E>0.3 MeV");
		gxhr->SetTitle("X RM E>1.4 MeV");
		gzhr->SetTitle("Z RM E>1.4 MeV");
		
		while (!fscirm1.eof()) {
			fscirm1 >> data[ns].id >> data[ns].t >> data[ns].xtot >> data[ns].ztot;
			for (int i=0; i<10; i++) fscirm1 >> data[ns].x[i];
			for (int i=0; i<10; i++) fscirm1 >> data[ns].z[i];
			for (int i=0; i<5; i++) fscirm1 >> data[ns].ac[i];
			
			if (data[ns].id == 0 && data[ns].xtot == 0) continue;	// last file line, otherwise segmentation fault
			
			data[ns].xhr = 0.;
			data[ns].zhr = 0.;
			for (int i=3; i<10; i++) data[ns].xhr += data[ns].x[i];
			//if (data[ns].x[0] + data[ns].x[1] > 0.) data[ns].xhr /= (data[ns].x[0] + data[ns].x[1]);
			//else data[ns].xhr = -1.;
			for (int i=3; i<10; i++) data[ns].zhr += data[ns].z[i];
			//if (data[ns].z[0] + data[ns].z[1] > 0.) data[ns].zhr /= (data[ns].z[0] + data[ns].z[1]);
			//else data[ns].zhr = -1.;
			
			auxlon = a + b*data[ns].t;
			//while(auxlon>360.) auxlon -= 360.;
			data[ns].lon = auxlon;
			
			// eliminate idle-obs spikes
			flag = 1;
			if (data[ns].xtot<100.) flag = 0; 
			if (flag) {
				for (int h=ns-1; h>=0 && flag && h>ns-10; h--) {
					if (data[h].xtot == 0) flag = 0;
				} 
			}
			
			// eliminate boost/eclipse
			if (flag) {
				auxt = data[ns].t + t01;
				while (auxt >= boostfin[iboost]) iboost++;
				if (auxt >= boostini[iboost]) flag = 0;
			}
			
			
			if (flag) {
				gxtot->SetPoint(npt, data[ns].lon, data[ns].xtot);
				gztot->SetPoint(npt, data[ns].lon, data[ns].ztot);
				gxhr->SetPoint(npt, data[ns].lon, data[ns].xhr);
				gzhr->SetPoint(npt, data[ns].lon, data[ns].zhr);
				
				
				npt++;
			}
			ns++;
		}
		
		float dt0 = t02-t01;
		while (!fscirm2.eof()) {
			fscirm2 >> data[ns].id >> data[ns].t >> data[ns].xtot >> data[ns].ztot;
			for (int i=0; i<10; i++) fscirm2 >> data[ns].x[i];
			for (int i=0; i<10; i++) fscirm2 >> data[ns].z[i];
			for (int i=0; i<5; i++) fscirm2 >> data[ns].ac[i];
			data[ns].t += dt0;
			
			if (data[ns].id == 0 && data[ns].xtot == 0) continue;	// last file line, otherwise segmentation fault
			
			data[ns].xhr = 0.;
			data[ns].zhr = 0.;
			for (int i=3; i<10; i++) data[ns].xhr += data[ns].x[i];
			//if (data[ns].x[0] + data[ns].x[1] > 0.) data[ns].xhr /= (data[ns].x[0] + data[ns].x[1]);
			//else data[ns].xhr = -1.;
			for (int i=3; i<10; i++) data[ns].zhr += data[ns].z[i];
			//if (data[ns].z[0] + data[ns].z[1] > 0.) data[ns].zhr /= (data[ns].z[0] + data[ns].z[1]);
			//else data[ns].zhr = -1.;
			
			auxlon = a + b*data[ns].t;
			//while(auxlon>360.) auxlon -= 360.;
			data[ns].lon = auxlon;
			
			
			// eliminate idle-obs spikes
			flag = 1;
			if (data[ns].xtot<100.) flag = 0; 
			if (flag) {
				for (int h=ns-1; flag && h>ns-10; h--) {
					if (data[h].xtot == 0) flag = 0;
				} 
			}
			
			// eliminate boost/eclipse
			if (flag) {
				auxt = data[ns].t + t01;
				while (auxt >= boostfin[iboost]) iboost++;
				if (auxt >= boostini[iboost]) flag = 0;
			}
			
			if (flag) {
				gxtot->SetPoint(npt, data[ns].lon, data[ns].xtot);
				gztot->SetPoint(npt, data[ns].lon, data[ns].ztot);
				gxhr->SetPoint(npt, data[ns].lon, data[ns].xhr);
				gzhr->SetPoint(npt, data[ns].lon, data[ns].zhr);
				npt++;
			}
			ns++;
		}
		ns--;
		fscirm1.close();
		fscirm2.close();
		
		// do baseline polinomial fit
		
		TF1 *fxtot = new TF1("fxtot", "pol4", -20., 290.);
		gxtot->Fit(fxtot, "R");
		TF1 *fztot = new TF1("fztot", "pol4", -20., 290.);
		gztot->Fit(fztot, "R");
		TF1 *fxhr = new TF1("fxhr", "pol4", -20., 290.);
		gxhr->Fit(fxhr, "R");
		TF1 *fzhr = new TF1("fzhr", "pol4", -20., 290.);
		gzhr->Fit(fzhr, "R");
		
		double *axtot = gxtot->GetY();
		double *aztot = gztot->GetY();
		double *axhr  = gxhr->GetY();
		double *azhr  = gzhr->GetY();
		double *alon  = gxtot->GetX();
			
		// calculate variance and fill global histo
		
		float aux, ki2;
		TGraph *gxtot_ki2 = new TGraph(10);
		TGraph *gztot_ki2 = new TGraph(10);
		TGraph *gxhr_ki2 = new TGraph(10);
		TGraph *gzhr_ki2 = new TGraph(10);
		gxtot_ki2->SetTitle("Chi^2 X RM E>0.3 MeV");
		gztot_ki2->SetTitle("Chi^2 Z RM E>0.3 MeV");
		gxhr_ki2->SetTitle("Chi^2 X RM E>1.4 MeV");
		gzhr_ki2->SetTitle("Chi^2 Z RM E>1.4 MeV");
		int nki = 0;
		for (int i=0; i<npt; i++) {
			if (alon[i]>=-20. && alon[i]<=290.) {
				aux = axtot[i] - fxtot->Eval(alon[i]);
				if (fxtot->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fxtot->Eval(alon[i]));
				else ki2 = 0.;
				gxtot_ki2->SetPoint(nki, alon[i], ki2);
				h3xtot->Fill(cont, alon[i], ki2);
				
				aux = aztot[i] - fztot->Eval(alon[i]);
				if (fztot->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fztot->Eval(alon[i]));
				else ki2 = 0.;
				gztot_ki2->SetPoint(nki, alon[i], ki2);
				h3ztot->Fill(cont, alon[i], ki2);
				
				aux = axhr[i] - fxhr->Eval(alon[i]);
				if (fxhr->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fxhr->Eval(alon[i]));
				else ki2 = 0.;
				gxhr_ki2->SetPoint(nki, alon[i], ki2);
				h3xhr->Fill(cont, alon[i], ki2);
				
				aux = azhr[i] - fzhr->Eval(alon[i]);
				if (fzhr->Eval(alon[i]) > 0.) ki2 = (aux*aux)/(fzhr->Eval(alon[i]));
				else ki2 = 0.;
				gzhr_ki2->SetPoint(nki, alon[i], ki2);
				h3zhr->Fill(cont, alon[i], ki2);
				
				nki++;
			}
		}
		
		TString cname("c_");
		cname += (cont+1);
		TCanvas *c1 = new TCanvas(cname, cname, 800, 800);
		c1->Divide(2, 4);
		
		c1->cd(1);
		gxtot->Draw("ALP");
		fxtot->SetLineColor(3);
		fxtot->Draw("SAME");
		c1->cd(2);
		gxtot_ki2->Draw("ALP");
		
		c1->cd(3);
		gztot->Draw("ALP");
		fztot->SetLineColor(3);
		fztot->Draw("SAME");
		c1->cd(4);
		gztot_ki2->Draw("ALP");
		
		c1->cd(5);
		gxhr->Draw("ALP");
		fxhr->SetLineColor(3);
		fxhr->Draw("SAME");
		c1->cd(6);
		gxhr_ki2->Draw("ALP");
		
		c1->cd(7);
		gzhr->Draw("ALP");
		fzhr->SetLineColor(3);
		fzhr->Draw("SAME");
		c1->cd(8);
		gzhr_ki2->Draw("ALP");
		
		cname += ".gif";
		c1->Print(cname);
		
		// write results to file
		h3xtot->Write("", TObject::kOverwrite);
		h3ztot->Write("", TObject::kOverwrite);
		h3xhr->Write("", TObject::kOverwrite);
		h3zhr->Write("", TObject::kOverwrite);
		
		// free memory
		delete data;
		delete gxtot;
		delete gztot;
		delete gxhr;
		delete gzhr;
		delete fxtot;
		delete fztot;
		delete fxhr;
		delete fzhr;
		delete gxtot_ki2;
		delete gztot_ki2;
		delete gxhr_ki2;
		delete gzhr_ki2;
		delete c1;
	}
	
	fresults.Close();
	
	// plot global distributions
	
	// TH2D *h2xtot = h3xtot->Project3D("zy");
	// h2xtot->Draw("COLZ");
	
/*	int istart = 200;
	int istep  = 200;
	int istop  = 4000;
	int nstep  = (istop-istart)/istep;
	float fact = 1./sqrt(2. * TMath::Pi());
	
	TCanvas *c2 = new TCanvas("c2", "c2", 1000, 800);
	c2->Divide(5, 4);
	for (int i=istart; i<istop; i+=istep) {
		float avg=0.;
		int nmax = 0;
		int nmin = 10000;
		for (int j=i; j<i+istep; j++) {
			avg += data[j].xtot;
			if (data[j].xtot>nmax) nmax = (int) data[j].xtot;
			if (data[j].xtot<nmin) nmin = (int) data[j].xtot;
		}
		
		TString hname("hpoisson");
		hname += i;
		int dbin=4;
		cout << nmin << "  "  << nmax << "  "  << (nmax-nmin)%dbin << endl; 
		nmax += ((nmax-nmin)%dbin > 0 ? dbin - (nmax-nmin)%dbin : 0);
		cout << nmax << endl;
		TH1F *hpoisson = new TH1F(hname, hname, (nmax-nmin)/dbin+1, nmin, nmax+dbin);
		for (int j=i; j<i+istep; j++) hpoisson->Fill(data[j].xtot);
		//hpoisson->Sumw2();
		//hpoisson->Scale(1./istep);
		avg /= istep;
		
		c2->cd(1 + (i-istart)/istep);
		hpoisson->SetStats(0);
		hpoisson->Draw();
		hpoisson->GetXaxis()->SetRangeUser(nmin-1, nmax+1);
		TString fname("fpoisson");
		fname += i;
		TF1 *fpoisson = new TF1(fname,"gaus(0)", nmin-1, nmax+1); 
		float stdev = sqrt(avg);
		fpoisson->SetParameter(0, dbin*istep*fact/stdev); 
		fpoisson->SetParameter(1, avg); 
		fpoisson->SetParameter(2, stdev); 
		fpoisson->SetLineColor(2);
		//fpoisson->Draw("SAME");
		
		TString hsimname("hsim");
		hsimname += i;
		TH1F *hp = new TH1F(hsimname, hsimname, (nmax-nmin)/dbin+1, nmin, nmax+dbin);
		hp->Eval(fpoisson,"R");
		hp->SetLineColor(3);
		hp->Draw("SAME");
		
		double res[400];
		printf("%8d \t%10.6f\n", i, hpoisson->Chi2Test(hp, "UW", res));
		
		//printf("n \tData \tPoisson\n");
		//for (int i=0; i<nmax; i++) printf("%d \t%.4f \t%4f\n", i, hpoisson->GetBinContent(i+1), fpoisson->Eval(i)); 
		
	}*/
}