{
	// MM 11/06/09 modified path for root files
	// MM 25/03/09 modificato a partire da plot_sferics_lc.C. Legge una lista (data da Fullekrug) e produce tutti i grafici
	// MM 23/03/09 corretto baco: millisecondi e NON secondi!!!
	// MM 06/03/09 sovrapposizione segnali onde sferiche di Fullekrug con i dati MCAL
	
	gROOT->Reset();
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
	mcalstyle->SetLabelSize(0.06, "X");
	mcalstyle->SetLabelSize(0.06, "Y");
	gROOT->SetStyle("mcalstyle");

	// input files and path
	
	// 16/3/09 dataset
	// char bstpath[] = "/home/mcal/data_analysis/VSB/analysis_09-03-16/sferics_correlation/";
	// char listfile[] = "/home/mcal/data_analysis/VSB/analysis_09-03-16/sferics_correlation/tgf-130309.lst";
	// char mcalfile[] = "/home/mcal/data_analysis/VSB/analysis_09-03-16/mcal_trg_090313-090316_t0.dat";
	
	// 20/3/09 dataset
	// char bstpath[] = "/home/mcal/data_analysis/VSB/analysis_09-03-20/sferic_correlation/";
	// char listfile[] = "/home/mcal/data_analysis/VSB/analysis_09-03-20/sferic_correlation/tgf-160309_new.lst";
	// char mcalfile[] = "/home/mcal/data_analysis/VSB/analysis_09-03-20/mcal_trg_090316-090320_t0.dat";
	
	// 02-06/06/09 dataset
	char bstpath[] = "/home/mcal/data_analysis/VSB/analysis_090504-090507/sferics/";
	char listfile[] = "/home/mcal/data_analysis/VSB/analysis_090504-090507/sferics/tgf-060509.lst";
	char mcalfile[] = "/home/mcal/data_analysis/VSB/analysis_090504-090507/sferics/mcal_trg_090331-090602_t0.dat";
	
	// 02-06/06/09 dataset
	// char bstpath[] = "/home/mcal/data_analysis/VSB/analysis_090602-090610_orbits010901-010990/sferics/";
	// char listfile[] = "/home/mcal/data_analysis/VSB/analysis_090602-090610_orbits010901-010990/sferics/tgf-040609.lst";
	// char mcalfile[] = "/home/mcal/data_analysis/VSB/analysis_090602-090610_orbits010901-010990/sferics/mcal_trg_090602-090610_t0.dat";
	
	// char rootpath[] = "/data1/mcal/MCAL_ARCHIVE/3908/root_file/";	// old path
	char rootpath[] = "/MCAL_PROC/ARCHIVE/";
	float time, efield;
	FILE *fin;
	GRB3908 *grb;
		
	// load list of candidates
	ifstream fmcal(mcalfile, ios_base::in);
	int orbit[2000];
	int trg[2000];
	int add[2000], amm[2000], ayy[2000], ah[2000], amin[2000], asec[2000], ausec[2000], apflux[2000];
	float alon[2000], alat[2000];
	double at0[2000];
	int n=0;
	char date[100], t[100];
	fmcal.clear();
	while (!fmcal.eof()) {
		fmcal >> orbit[n] >> trg[n] >> date >> t >> alon[n] >> alat[n] >> apflux[n] >> at0[n];
		sscanf(date, "20%2d-%2d-%2d", &ayy[n], &amm[n], &add[n]);
		sscanf(t, "%2d:%2d:%2d.%6d", &ah[n], &amin[n], &asec[n], &ausec[n]);
		cout << n << "  " << ayy[n] << "  " << amm[n] << "  " << add[n] << "  " << ah[n] << "  " << amin[n] << "  " << asec[n] << "  " << at0[n] << endl;
		n++;
	}	
	cout << "read " << n << " MCAL triggers" << endl;
	
	// load list of correlated data
	int dd, mm, yy, h, min, sec, usec, pflux;
	float lon, lat;
	ifstream in(listfile, ios_base::in);
	int id = 0;
	while (!in.eof()) {
		in >> dd >> mm >> yy >> h >> min >> sec >> usec >> lon >> lat >> pflux;
		char bname[200];
		sprintf(bname, "tgf-%02d%02d%02d-%02d%02d%02d.bst", dd, mm, yy, h, min, sec);
		char bstname[200];
		sprintf(bstname, "%s%s", bstpath, bname);
		
		cout << "loading sferic file " << bstname << endl;
		
		// find the corresponding root file
		int k=0;
		float lc;
		while (k<n && (ayy[k]!=yy || amm[k]!=mm || add[k]!=dd || ah[k]!=h || amin[k]!=min || asec[k]!=sec)) k++;
		if (k<n) {
			char rootname[200];
			sprintf(rootname, "%s/%06d/RT%06d_3908.root", rootpath, orbit[k], orbit[k]);
			cout << "load root file " << rootname << " t0 = " << at0[k] << endl;
				
			// calculate local time at trigger
			lc = fmod(ah[k] + amin[k]/60. + asec[k]/3600. + alon[k]*0.06667, 24.);
			
			// load sferics and fill histo
			fin = fopen(bstname, "r");
			TString hname("h");
			hname += id;
			TH1F *hs = new TH1F(hname.Data(), hname.Data(), 500000, -0.250, 0.250);
			for (int i=1; i<=500000; i++) {
				fscanf(fin, "%e %e", &time, &efield);
				hs->SetBinContent(i, efield);
			}
			float xlo = -0.025;
			float xhi =  0.025;
			float tbin = 0.0005;
			fclose(fin);
				
			// load MCAL light curve
			grb = new GRB3908(rootname);
			grb->DefaultLightCurve();
			grb->CustomLightCurve(at0[k], -0.250, 0.250, 0., 2000., 0x7FFF, 0x7FFF, tbin);
			TH1D *hlc = grb->GetLightCurve();
			
			// graphic section
			TString cname("c");
			cname += id;
			TCanvas *c = new TCanvas(cname.Data(), cname.Data(), 800, 600);
			c->Divide(1,2);
			c->cd(1);
			hs->SetStats(0);
			hs->SetTitle("Electric field");
			hs->GetXaxis()->SetTitle("t-t0 (s)");
			hs->GetYaxis()->SetTitle("E field V/m");
			hs->GetYaxis()->SetTitleSize(0.06);
			hs->GetXaxis()->SetTitleSize(0.06);
			hs->GetXaxis()->CenterTitle();
			hs->GetXaxis()->SetRangeUser(xlo, xhi);
			hs->GetYaxis()->CenterTitle();
			hs->GetXaxis()->SetTitleOffset(0.8);
			hs->GetYaxis()->SetTitleOffset(0.8);
			hs->Draw();
			TText txt;
			txt->DrawTextNDC(0.15, 0.8, bname);
					
			c->cd(2);
			hlc->SetStats(0);
			hlc->SetTitle("MCAL count rate ");
			hlc->GetXaxis()->SetTitle("t-t0 (s)");
			hlc->GetXaxis()->CenterTitle();
			hlc->GetXaxis()->SetRangeUser(xlo, xhi);
			hlc->GetYaxis()->SetTitle("counts/0.5ms");
			hlc->GetYaxis()->CenterTitle();
			hlc->GetXaxis()->SetTitleOffset(0.8);
			hlc->GetYaxis()->SetTitleOffset(0.8);
			hlc->Draw();
			TText txt2;
			char t2[300];
			sprintf(t2, "trigger %06d-%d", orbit[k], trg[k]);
			char t3[300];
			sprintf(t3, "t0 = %02d-%02d-%02d %02d:%02d:%02d (UT)", ayy[k], amm[k], add[k], ah[k], amin[k], asec[k]);
			char t4[300];
			sprintf(t4, "(%20.6f TT)", at0[k]);
			char t5[300];
			sprintf(t5, "Lon=%7.1f Lat=%7.1f Loc. time=%5.1f", alon[k], alat[k], lc);
			txt2->DrawTextNDC(0.15, 0.8, t2);
			txt2->DrawTextNDC(0.15, 0.7, t3);
			txt2->DrawTextNDC(0.15, 0.6, t4);
			txt2->DrawTextNDC(0.15, 0.5, t5);
			
			// save picture	
			char figname[400];
			sprintf(figname, "%stgf-%02d%02d%02d-%02d%02d%02d_%06d-%d.ps", bstpath, dd, mm, yy, h, min, sec, orbit[k], trg[k]);
			c->Print(figname, "Landscape");
		} else cout << "cannot find corresponding MCAL trigger" << endl;
		id++;
	}
}