{
	// MM 04/11/09 find timing / coordinates mismatch between .lst and MCAL file. Does not perform analysis
	//             based on sferics_list_analysis.C
	// MM 26/08/09 created and modified from plot_sferic_list.C
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
	char bstpath[] = "/home/mcal/data_analysis/VSB/sferics_correlation/0909/";
	// char listfile[] = "/home/mcal/data_analysis/VSB/sferics_correlation/wrong_timing/0908/tgf-all.lst";
	char listfile[] = "/home/mcal/data_analysis/VSB/sferics_correlation/0909/tgf_all_0909.lst";
	char mcalfile[] = "/AGILE_MCAL/mcal_trg_complete.dat";
	
	// char rootpath[] = "/data1/mcal/MCAL_ARCHIVE/3908/root_file/";	// old path
	char rootpath[] = "/MCAL_PROC/ARCHIVE/";
	float time, efield;
	FILE *fin;
	//GRB3908 *grb;
		
	// load list of candidates
	ifstream fmcal(mcalfile, ios_base::in);
	int orbit[2000];
	int trg[2000];
	int add[2000], amm[2000], ayy[2000], ah[2000], amin[2000], asec[2000], ausec[2000], apflux[2000], acounts[2000];
	float alon[2000], alat[2000], ahr[2000];
	double at0[2000];
	int n=0;
	char date[100], t[100];
	fmcal.clear();
	while (!fmcal.eof()) {
		fmcal >> orbit[n] >> trg[n] >> date >> t >> alon[n] >> alat[n] >> apflux[n] >> acounts[n] >> ahr[n] >> at0[n];
		sscanf(date, "20%2d-%2d-%2d", &ayy[n], &amm[n], &add[n]);
		sscanf(t, "%2d:%2d:%2d.%6d", &ah[n], &amin[n], &asec[n], &ausec[n]);
		//cout << n << "  " << ayy[n] << "  " << amm[n] << "  " << add[n] << "  " << ah[n] << "  " << amin[n] << "  " << asec[n] << "  " << at0[n] << endl;
		n++;
	}	
	cout << "read " << n << " MCAL triggers" << endl;
	
	// open file for output
	FILE *fout;
	TString foutname(listfile);
	foutname += ".output";
	fout = fopen(foutname.Data(), "w");
	
	// load list of correlated data
	int dd, mm, yy, h, min, sec, usec, pflux;
	float lon, lat;
	ifstream in(listfile, ios_base::in);
	int id = 0;
	while (!in.eof()) {
		dd = 0;
		in >> dd >> mm >> yy >> h >> min >> sec >> usec >> lon >> lat >> pflux;
		if (!dd) continue;	// end of file
		char bname[200];
		sprintf(bname, "tgf-%02d%02d%02d-%02d%02d%02d.bst", dd, mm, yy, h, min, sec);
		char bstname[200];
		sprintf(bstname, "%s%s", bstpath, bname);
		
		// cout << "loading sferic file " << bstname << endl;
		
		// find the corresponding root file
		int k=0;
		float lc;
		TCanvas *c;
		while (k<n && (ayy[k]!=yy || amm[k]!=mm || add[k]!=dd || ah[k]!=h || amin[k]!=min || asec[k]!=sec)) k++;
		if (k<n) {
			if (ausec[k]!=usec || alon[k]!=lon || alat[k]!=lat) {
				cout << "MCAL  " << ayy[k] << "  " << amm[k] << "  " << add[k] << "  " << ah[k] << "  " << amin[k] << "  " << asec[k] << "  " << ausec[k] << "  "  << alon[k] << "  "  << alat[k] << endl;
				cout << "LST   " << yy << "  " << mm << "  " << dd << "  " << h << "  " << min << "  " << sec << "  " << usec << "  "  << lon << "  "  << lat << endl << endl;
			}
			 
			/*char rootname[200];
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
			float xlo = -0.024;
			float xhi =  0.024;
			float tbin = 0.0002;	//0.005
			fclose(fin);
			hs->GetXaxis()->SetRangeUser(-0.010, 0.010);
			float  tmax10 = (hs->GetMaximumBin() - 250000)*0.001;
			double vmax10 = hs->GetMaximum();
			hs->GetXaxis()->SetRangeUser(xlo, xhi);
			float  tmax25 = (hs->GetMaximumBin() - 250000)*0.001;
			double vmax25 = hs->GetMaximum();
				
			// load MCAL light curve
			grb = new GRB3908(rootname);
			grb->DefaultLightCurve();
			grb->CustomLightCurve(at0[k], -0.250, 0.250, 0., 2000., 0x7FFF, 0x7FFF, tbin);
			TH1D *hlc = grb->GetLightCurve();
			
			// graphic section
			TString cname("c");
			cname += id;
			c = new TCanvas(cname.Data(), cname.Data(), 800, 600);
			c->Divide(1,2);
			c->cd(1);
			hs->SetStats(0);
			hs->SetTitle("Electric field");
			hs->GetXaxis()->SetTitle("t-t0 (s)");
			hs->GetYaxis()->SetTitle("E field V/m");
			hs->GetYaxis()->SetTitleSize(0.06);
			hs->GetXaxis()->SetTitleSize(0.06);
			hs->GetXaxis()->CenterTitle();
			hs->GetYaxis()->CenterTitle();
			hs->GetXaxis()->SetTitleOffset(0.8);
			hs->GetYaxis()->SetTitleOffset(0.8);
			hs->Draw();
			TText txt;
			txt->DrawTextNDC(0.15, 0.8, bname);
			char t0[300];
			sprintf(t0, "Max delay = %7.3f ms  Max E = %.1e V/m", tmax25, vmax25);
			txt->DrawTextNDC(0.15, 0.7, t0);
					
			c->cd(2);
			hlc->SetStats(0);
			hlc->SetTitle("MCAL count rate ");
			hlc->GetXaxis()->SetTitle("t-t0 (s)");
			hlc->GetXaxis()->CenterTitle();
			hlc->GetXaxis()->SetRangeUser(xlo, xhi);
			hlc->GetYaxis()->SetTitle("counts/0.2ms");	// 0.5ms
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
			char t6[300];
			sprintf(t6, "HR=%5.2f  Counts=%4d  Peak flux=%4d", ahr[k], acounts[k], apflux[k]);
			txt2->DrawTextNDC(0.15, 0.8, t2);
			txt2->DrawTextNDC(0.15, 0.7, t3);
			txt2->DrawTextNDC(0.15, 0.6, t4);
			txt2->DrawTextNDC(0.15, 0.5, t5);
			txt2->DrawTextNDC(0.15, 0.4, t6);
			
			// save picture	
			char figname[400];
			sprintf(figname, "%simg-%02d%02d%02d-%02d%02d%02d_%06d-%d.ps", bstpath, dd, mm, yy, h, min, sec, orbit[k], trg[k]);
			c->Print(figname, "Landscape");
			
			delete grb;
			
			// output message for correlation
			fprintf(fout, "%06d \t%d %02d %02d %02d %02d %02d %02d.%06d \t%7.1f %7.1f %5.1f \t%5.2f %4d %4d \t%7.3f %.6f \t%7.3f %.6f\n",orbit[k], trg[k], ayy[k], amm[k], add[k], ah[k], amin[k], asec[k], ausec[k], alon[k], alat[k], lc, ahr[k], acounts[k], apflux[k], tmax10, vmax10, tmax25, vmax25);
			*/
		} else cout << "cannot find corresponding MCAL trigger at " << yy << "  " << mm << "  " << dd << "  " << h << "  " << min << "  " << sec << endl;
		id++;
	}
	
	fclose(fout);
}