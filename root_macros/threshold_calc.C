{
	gROOT->Reset();
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");
	
	// input files
	TFile fg("/data2/FLIGHT_DATA/calibration/RT003016_3020_3908_sel_01.root");
	TString calFile("/home/martino/mcalsw/share/MCVLib_conf/MCALBurstTest_601.conf");
	
	struct MCALevt {
		Double_t	time;
		Int_t		mult;
		Short_t		bar[30];
		Short_t 	pdA[30];
		Short_t 	pdB[30];
		Float_t	 	pos[30];
		Float_t	 	E[30];
		Float_t		Etot;
		Int_t		totEX;
		Int_t		totEZ;
		Int_t		rmE[30];
	};

	double u0A[30];
	float u0B[30], alphaA[30], alphaB[30], oA[30], oB[30];
	float alphaMean[30], alphaDelta[30], cx1[30], cx2[30], cE1[30];
	float range;
	cout << "Loading default calibration parameter file: " << calFile.Data() << endl;
	
	TEnv *conf = new TEnv(calFile);
	double barLen = conf->GetValue(".BarLen", 37.5);
	double gainFactor = conf->GetValue(".GainFactor", 1.);
	char parameterName[50];
	int i, ii;
	
	// load parameters
	for (i=0; i<30; i++) {  
		if (i<15) ii=i;
		else ii=i+1;
		sprintf(parameterName, ".Gain.Bar%02d.A", ii);
		u0A[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Gain.Bar%02d.B", ii);
		u0B[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Alpha.Bar%02d.A", ii);
		alphaA[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Alpha.Bar%02d.B", ii);
		alphaB[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Offset.Bar%02d.A", ii);
		oA[i] = conf->GetValue(parameterName, 0.);  
		//soA[i] = (Int_t) oA[i];
		sprintf(parameterName, ".Offset.Bar%02d.B", ii);
		oB[i] = conf->GetValue(parameterName, 0.);   
		//soB[i] = (Int_t) oB[i];
	}
	
	// calculate auxiliary parameters for energy/position calculations
	for (i=0; i<30; i++) {
		alphaMean[i]  = (alphaA[i] + alphaB[i])*0.5; 
		alphaDelta[i] = (alphaA[i] - alphaB[i])*0.5;  
		cx1[i] = 0.5 * (log(u0A[i]/u0B[i]) - alphaDelta[i]*barLen)/alphaMean[i];
		cx2[i] = 0.5/alphaMean[i];
		cE1[i] = exp(0.5 * barLen * alphaMean[i]) / sqrt(u0A[i] * u0B[i]);
	}
	
	cout << endl << "Current calibration parameter file: " << calFile.Data() << endl << endl;
	printf("i\t bar\t oA (ch)\t oB (ch)\t u0A (ch/MeV)\t u0B (ch/MeV)\t alphaA (cm^-1)\t alphaB (cm^-1)\n\n");
	for (int i=0; i<30; i++) {
		if (i<15) printf("%d\t X%2d\t ", i, i+1);
		else printf("%d\t Z%2d\t ", i, i-14);
		printf("%8.2f\t %8.2f\t %8.3f\t %8.3f\t %8.4f\t %8.4f\n", oA[i], oB[i], u0A[i], u0B[i], alphaA[i], alphaB[i]);
	}
	cout << endl;
	
	// retrieve data trees
	TTree *tg = NULL;
	fg.ls();
	tg = (TTree *) fg->Get("tdata_sel_0");
	int ng = tg->GetEntries();
	
	// init histo
	TH1F *ha[30];
	for (int i=0; i<30; i++) {
		TString hname("ha_");
		hname += i;
		ha[i] = new TH1F(hname, hname, 10000, 0, 10000);	// 1 ADC channel bin
	}
	TH1F *he[30];
	for (int i=0; i<30; i++) {
		TString hname("he_");
		hname += i;
		he[i] = new TH1F(hname, hname, 5000, 0., 200.);	// 20 keV energy bin
	}
	
	// connect input trees
	MCALevt evtg;
	int nbarsg, nbarsb, id;
	double tmpe;
	tg->SetBranchAddress("time", &evtg.time);
	tg->SetBranchAddress("mult", &evtg.mult);
	tg->SetBranchAddress("bar", evtg.bar);
	tg->SetBranchAddress("pdA", evtg.pdA);
	tg->SetBranchAddress("pdB", evtg.pdB);
	tg->SetBranchAddress("pos", evtg.pos);
	tg->SetBranchAddress("E", evtg.E);
	tg->SetBranchAddress("Etot", &evtg.Etot);
	
	// process data tree
	cout << "Processing " << ng << " entries" << endl;
	len05 = 0.5*barLen;
	for (int i=0; i<ng; i++) {
		tg->GetEntry(i);
		nbarsg=0;
		for (int k=0; k<30; k++) { 
			id = evtg.bar[k];
			if (id>=0) {
				nbarsg++;
				ha[k]->Fill(evtg.pdA[id] + evtg.pdB[id]);
				
				tmpe = (evtg.pdA[id] - oA[k])/(u0A[k]*exp(-alphaMean[k]*(evtg.pos[id] + len05))) + (evtg.pdB[id] - oB[k])/(u0B[k]*exp(-alphaMean[k]*(len05 - evtg.pos[id])));
				he[k]->Fill(tmpe);
			}
			
			if (nbarsg == evtg.mult) break;
		}
	}
	
	// graphic section
	TCanvas *c3 = new TCanvas("calib_data", "pdA + pdB distribution", 800, 600);
	c3->Divide(6, 5);
	for (int i=0; i<30; i++) {
		c3->cd(i+1);
		ha[i]->GetXaxis()->SetRangeUser(400., 500.);
		ha[i]->Draw();
	}
	
	TCanvas *c4 = new TCanvas("calib_data2", "EA + EB distributions", 800, 600);
	c4->Divide(6, 5);
	for (int i=0; i<30; i++) {
		c4->cd(i+1);
		he[i]->GetXaxis()->SetRangeUser(0., 2.);
		he[i]->Draw();
	}
}