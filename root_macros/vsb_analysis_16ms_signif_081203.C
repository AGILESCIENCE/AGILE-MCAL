{
	// MM 03/12/08 include anche info sul background, messe fluence e bkg come float (mod. struttura trigger)
	// MM 19/11/08 modificata a partire da macro vsb_analysis_16ms_signif.C
	//             analisi adatta ai candidati TGF triggerati su 64ms
	// MM 18/11/08 valuta la significativita' degli eventi su diversi tempi scala
	
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
	
	int process_event_data = 1;

	struct Trigger  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			char type[5];
			int sa;
			int lng;
			int subms;
			int nrm;
			int rmid;
			int sitid;
			float lon;
			float lat;
			float h;
			float dt;
			int ntrg;	// number of reconstructed sub-ms triggers
			float mint;	// minimum time (should be ~-3. for datasets with correct pre-burst)
			float dt0;	// time difference of the found trigger with respect to the initial t0 (bstart)
			float tgrbini;
			float t90;
			float tbin;	// added since 15/10/08
			float cbkg;	// added since 15/10/08 
			float counts;
			float cpeak;
			float avgE;
			int nEbins;
			float c[4];
			float b[4];
			int x[15];
			int z[15];
			int cub[8];		// spatial zones (cuboni)
	};
	
	// ----------- read data and fill tree ---------------
	
	int nbin=3;
	int nentries = 0;
	int nc=0;
	double t0prev = 0.;
	int contact[10000];
	int trg[10000];	
	int flag[10000];
	double t0[10000];
	float tbin[5]={1., 2., 4., 8., 16.};		// time bin for labels (in ms)
	float sig[3][10000];
	float t90[3][10000];
	float pflux[3][10000];
	float counts[3][10000];
	float dt0[3][10000];
	float tgrbini[3][10000];
	float avgE[3][10000];
	float hr[3][10000];
	
	TString filenames[3];

	filenames[0]="/data1/mcal/data_analysis/VSB/analysis_08-12-04_16ms_selected/VSB_global_info_10.dat";
	filenames[1]="/data1/mcal/data_analysis/VSB/analysis_08-12-04_16ms_selected/VSB_global_info_20.dat";
	filenames[2]="/data1/mcal/data_analysis/VSB/analysis_08-12-04_16ms_selected/VSB_global_info_40.dat";
	
	Trigger atrg;
	for (int i=0; i<nbin; i++) {
		TTree tvsb;
		tvsb.ReadFile(filenames[i], "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:type/C:sa/I:lng/I:subms/I:nrm/I:rmid/I:sitid/I:lon/F:lat/F:h/F:dt/F:ntrg/I:mint/F:dt0/F:tgrbini/F:t90/F:tbin/F:cbkg/F:counts/F:cpeak/F:avgE/F:nEbins/I:c0/F:c1/F:c2/F:c3/F:b0/F:b1/F:b2/F:b3/F:x1/I:x2/I:x3/I:x4/I:x5/I:x6/I:x7/I:x8/I:x9/I:x10/I:x11/I:x12/I:x13/I:x14/I:x15/I:z1/I:z2/I:z3/I:z4/I:z5/I:z6/I:z7/I:z8/I:z9/I:z10/I:z11/I:z12/I:z13/I:z14/I:z15/I:cub1/I:cub2/I:cub3/I:cub4/I:cub5/I:cub6/I:cub7/I:cub8/I");
		
		// connect tree
		tvsb.SetBranchAddress("runid", &atrg.runid);
		tvsb.SetBranchAddress("trg", &atrg.trg);
		tvsb.SetBranchAddress("t0", &atrg.t0);
		tvsb.SetBranchAddress("tstop", &atrg.tstop);
		tvsb.SetBranchAddress("ttot", &atrg.ttot);
		tvsb.SetBranchAddress("date", &atrg.date);
		tvsb.SetBranchAddress("time", &atrg.time);
		tvsb.SetBranchAddress("type", &atrg.type);
		tvsb.SetBranchAddress("sa", &atrg.sa);
		tvsb.SetBranchAddress("lng", &atrg.lng);
		tvsb.SetBranchAddress("subms", &atrg.subms);
		tvsb.SetBranchAddress("nrm", &atrg.nrm);
		tvsb.SetBranchAddress("rmid", &atrg.rmid);
		tvsb.SetBranchAddress("sitid", &atrg.sitid);
		tvsb.SetBranchAddress("lon", &atrg.lon);
		tvsb.SetBranchAddress("lat", &atrg.lat);
		tvsb.SetBranchAddress("h", &atrg.h);
		tvsb.SetBranchAddress("dt", &atrg.dt);
		tvsb.SetBranchAddress("ntrg", &atrg.ntrg);
		tvsb.SetBranchAddress("mint", &atrg.mint);
		tvsb.SetBranchAddress("dt0", &atrg.dt0);
		tvsb.SetBranchAddress("tgrbini", &atrg.tgrbini);
		tvsb.SetBranchAddress("t90", &atrg.t90);
		tvsb.SetBranchAddress("tbin", &atrg.tbin);
		tvsb.SetBranchAddress("cbkg", &atrg.cbkg);
		tvsb.SetBranchAddress("counts", &atrg.counts);
		tvsb.SetBranchAddress("cpeak", &atrg.cpeak);
		tvsb.SetBranchAddress("avgE", &atrg.avgE);
		tvsb.SetBranchAddress("nEbins", &atrg.nEbins);
		tvsb.SetBranchAddress("c0", &atrg.c[0]);
		tvsb.SetBranchAddress("c1", &atrg.c[1]);
		tvsb.SetBranchAddress("c2", &atrg.c[2]);
		tvsb.SetBranchAddress("c3", &atrg.c[3]);
		tvsb.SetBranchAddress("b0", &atrg.b[0]);
		tvsb.SetBranchAddress("b1", &atrg.b[1]);
		tvsb.SetBranchAddress("b2", &atrg.b[2]);
		tvsb.SetBranchAddress("b3", &atrg.b[3]);
		
		tvsb.SetBranchAddress("x1", &atrg.x[0]);
		tvsb.SetBranchAddress("x2", &atrg.x[1]);
		tvsb.SetBranchAddress("x3", &atrg.x[2]);
		tvsb.SetBranchAddress("x4", &atrg.x[3]);
		tvsb.SetBranchAddress("x5", &atrg.x[4]);
		tvsb.SetBranchAddress("x6", &atrg.x[5]);
		tvsb.SetBranchAddress("x7", &atrg.x[6]);
		tvsb.SetBranchAddress("x8", &atrg.x[7]);
		tvsb.SetBranchAddress("x9", &atrg.x[8]);
		tvsb.SetBranchAddress("x10", &atrg.x[9]);
		tvsb.SetBranchAddress("x11", &atrg.x[10]);
		tvsb.SetBranchAddress("x12", &atrg.x[11]);
		tvsb.SetBranchAddress("x13", &atrg.x[12]);
		tvsb.SetBranchAddress("x14", &atrg.x[13]);
		tvsb.SetBranchAddress("x15", &atrg.x[14]);
		tvsb.SetBranchAddress("z1", &atrg.z[0]);
		tvsb.SetBranchAddress("z2", &atrg.z[1]);
		tvsb.SetBranchAddress("z3", &atrg.z[2]);
		tvsb.SetBranchAddress("z4", &atrg.z[3]);
		tvsb.SetBranchAddress("z5", &atrg.z[4]);
		tvsb.SetBranchAddress("z6", &atrg.z[5]);
		tvsb.SetBranchAddress("z7", &atrg.z[6]);
		tvsb.SetBranchAddress("z8", &atrg.z[7]);
		tvsb.SetBranchAddress("z9", &atrg.z[8]);
		tvsb.SetBranchAddress("z10", &atrg.z[9]);
		tvsb.SetBranchAddress("z11", &atrg.z[10]);
		tvsb.SetBranchAddress("z12", &atrg.z[11]);
		tvsb.SetBranchAddress("z13", &atrg.z[12]);
		tvsb.SetBranchAddress("z14", &atrg.z[13]);
		tvsb.SetBranchAddress("z15", &atrg.z[14]);
		tvsb.SetBranchAddress("cub1", &atrg.cub[0]);
		tvsb.SetBranchAddress("cub2", &atrg.cub[1]);
		tvsb.SetBranchAddress("cub3", &atrg.cub[2]);
		tvsb.SetBranchAddress("cub4", &atrg.cub[3]);
		tvsb.SetBranchAddress("cub5", &atrg.cub[4]);
		tvsb.SetBranchAddress("cub6", &atrg.cub[5]);
		tvsb.SetBranchAddress("cub7", &atrg.cub[6]);
		tvsb.SetBranchAddress("cub8", &atrg.cub[7]);	
		
		nentries = tvsb.GetEntries();
		cout << i << " Total number of entries = " << nentries << endl;
	
		// ----------- process entries & fill arrays ---------------
		nc=0;
		double t0prev = 0.;
		for (int j=0; j<nentries; j++) {
			tvsb.GetEntry(j);
			// main selection
			if ( 1) {	// selection previously done (HR>0.5, GRB, good orbit range...)
			
				if (i==0) {	// to do only for the first dataset (common information)
					contact[nc] = atrg.runid;
					trg[nc]     = atrg.trg;
					t0[nc]      = atrg.t0;
					if(atrg.type[0]=='G') flag[nc]=1;	// 1== GRB; 0== BUS
					else flag[nc]=0;
				}
				sig[i][nc] = 0.;// atrg.cpeak/sqrt(atrg.cbkg);
				t90[i][nc] = atrg.t90;
				dt0[i][nc] = atrg.dt0;
				tgrbini[i][nc] = atrg.tgrbini;
				counts[i][nc] = atrg.counts;
				avgE[i][nc] = atrg.avgE;
				pflux[i][nc] = atrg.cpeak;	// counts/bin
				if (atrg.c[0]+atrg.c[1]) hr[i][nc] = (atrg.c[2]+atrg.c[3])/(atrg.c[0]+atrg.c[1]);
				else hr[i][nc] = 100.;
				nc++;
			}
			//else cout << "*** same as previous trigger " << atrg.runid << "-" << atrg.trg << endl;
			t0prev = atrg.t0;
		}
	}	
	
	// show data and fill graphs
	
	cout << endl << "number of events: " << nc << endl;
	cout << endl << "GRB events" << endl;
	TGraph *gGRB = new TGraph(1);
	int nGRB=0;
	
	printf("\n-------------------------Duration (ms)          peak flux/bin         Fluence (counts)         HR             dt0             tgrbini --------------\norbit   trg     t0    ");
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	for (int k=0; k<nbin; k++) printf("%10.6f  ", tbin[k]);
	for (int k=0; k<nbin; k++) printf("%10.6f  ", tbin[k]);
	printf("\n--------------------------------------------------------------------------------------------------\n");
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("%6d  %3d  %20.6f", contact[j], trg[j], t0[j]);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? 1000.*t90[k][j] : 0.);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? pflux[k][j] : 0. );
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? counts[k][j] : 0.);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? hr[k][j] : 0.);
			for (int k=0; k<nbin; k++) printf("%10.6f  ", fabs(tgrbini[k][j])<0.016 ? dt0[k][j] : 0.);
			for (int k=0; k<nbin; k++) printf("%10.6f  ", fabs(tgrbini[k][j])<0.016 ? tgrbini[k][j] : 0.);
			printf("\n");
			
				
			gGRB->SetPoint(j,  hr[2][j], 1000.*t90[2][j]);
			
		}
	}
	printf("\n--------------------------------------------------------------------------------------------------\n");
	
/*	printf("\n----------------------------------- Peak flux (counts/ms) -------------------------------------\nid    orbit   trg  ");
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	printf("\n--------------------------------------------------------------------------------------------------\n");
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("%4d  %6d  %3d  ", j, contact[j], trg[j]);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? pflux[k][j] : 0. );
			printf("\n");
		}
	}
	printf("\n--------------------------------------------------------------------------------------------------\n");
	
	printf("\n----------------------------------- Fluence (counts) ---------------------------------------------\nid    orbit   trg  ");
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	printf("\n--------------------------------------------------------------------------------------------------\n");
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("%4d  %6d  %3d  ", j, contact[j], trg[j]);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? counts[k][j] : 0.);
			printf("\n");
		}
	}
	printf("\n--------------------------------------------------------------------------------------------------\n");
	
	printf("\n----------------------------------- Average Energy (MeV) -----------------------------------------\nid    orbit   trg  ");
	for (int k=0; k<nbin; k++) printf("%6.2f  ", tbin[k]);
	printf("\n--------------------------------------------------------------------------------------------------\n");
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("%4d  %6d  %3d  ", j, contact[j], trg[j]);
			for (int k=0; k<nbin; k++) printf("%6.2f  ", fabs(tgrbini[k][j])<0.016 ? avgE[k][j] : 0.);
			printf("\n");
		}
	}
	printf("\n--------------------------------------------------------------------------------------------------\n");
	
	printf("\n----------------------------------- t_GRB - t_0 (ms) -----------------------------------------\nid    orbit   trg  ");
	for (int k=0; k<nbin; k++) printf("%8.4f  ", tbin[k]);
	printf("\n--------------------------------------------------------------------------------------------------\n");
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("%4d  %6d  %3d  ", j, contact[j], trg[j]);
			for (int k=0; k<nbin; k++) printf("%8.4f  ", tgrbini[k][j]);
			printf("\n");
		}
	}
	printf("\n--------------------------------------------------------------------------------------------------\n");

	// write single burst info
	
	for (int j=0; j<nc; j++) {
		if (flag[j]==1) {
			printf("\n--------- Trigger %6d-%d ----------------------------------\n", contact[j], trg[j]);
			printf("t_bin    sigma     t90     P_flux     Fluence   avg_E   T_ini\n-------------------------------------------------------------\n");
			for (int k=0; k<nbin; k++) printf("%6.2f   %6.2f   %6.2f   %6.2f   %6.2f   %6.2f   %8.4f\n", tbin[k], sig[k][j], 1000.*t90[k][j], pflux[k][j], counts[k][j], avgE[k][j], tgrbini[k][j]);
			printf("\n-------------------------------------------------------------\n");
		}
	}	
	*/

	
	// ----------- graphic section ---------------
	
	TCanvas c1("c1", "c1", 800, 1000);
	c1.cd(1);
	gGRB->SetMarkerStyle(2);
	gGRB->Draw("AP");

}