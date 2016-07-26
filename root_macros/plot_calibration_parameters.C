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

	double u0A[30];
	float u0B[30], alphaA[30], alphaB[30], oA[30], oB[30];
	float alphaMean[30], alphaDelta[30], cx1[30], cx2[30], cE1[30];
	float range;
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += "/share/MCVLib_conf/MCVLib_configuration.conf";
	stdconf = new TEnv(confname);
	
	TString calFile;
	calFile.Append(getenv("MCALSW"));
//	calFile += stdconf->GetValue(".burst.calibration.file.","burst.conf");
	calFile += stdconf->GetValue(".grid.calibration.file.","burst.conf");
	cout << "Loading default calibration parameter file: " << calFile.Data() << endl;
	
	TEnv *conf = new TEnv(calFile);
	double barLen = conf->GetValue(".BarLen", 37.5);
	double gainFactor = conf->GetValue(".GainFactor", 1.);
	char parameterName[50];
	int i, ii;
	
	TH1F *hu0A = new TH1F("hu0A", "gain side A", 100, 0., 50.); 
	TH1F *hu0B = new TH1F("hu0B", "gain side B", 100, 0., 50.); 
	TH1F *halphaA = new TH1F("halphaA", "alpha side A", 40, 0.01, 0.05); 
	TH1F *halphaB = new TH1F("halphaB", "alpha side B", 40, 0.01, 0.05); 
	TH1F *hr = new TH1F("hr", "bar center energy range", 50, 0., 250.); 
	TH1F *hrb = new TH1F("hrb", "PD energy range", 50, 0., 250.); 
	
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
	if (gainFactor != 1.) {		// to account for different gain of the grid chains
		for (i=0; i<30; i++) {
			u0A[i] *= gainFactor;
			u0B[i] *= gainFactor;
		}
	}
	for (i=0; i<30; i++) {
		alphaMean[i]  = (alphaA[i] + alphaB[i])*0.5; 
		alphaDelta[i] = (alphaA[i] - alphaB[i])*0.5;  
		cx1[i] = 0.5 * (log(u0A[i]/u0B[i]) - alphaDelta[i]*barLen)/alphaMean[i];
		cx2[i] = 0.5/alphaMean[i];
		cE1[i] = exp(0.5 * barLen * alphaMean[i]) / sqrt(u0A[i] * u0B[i]);
	}	
	
	// fill histos
	for (i=0; i<30; i++) {
		hu0A->Fill(u0A[i]);
		hu0B->Fill(u0B[i]);
		halphaA->Fill(alphaA[i]);
		halphaB->Fill(alphaB[i]);
		range = cE1[i]*sqrt((4095-oA[i])*(4095-oB[i]));
		hr->Fill(range);
		float auxB = (4095-oA[i])/u0A[i]*exp(-alphaB[i]*barLen)*u0B[i] + oB[i];
		range = cE1[i]*sqrt((4095-oA[i])*auxB);
//		hrb->Fill(range);
		hrb->Fill((4095-oA[i])/u0A[i]);
		float auxA = (4095-oB[i])/u0B[i]*exp(-alphaA[i]*barLen)*u0A[i] + oA[i];
		range = cE1[i]*sqrt((4095-oB[i])*auxA);
//		hrb->Fill(range);
		hrb->Fill((4095-oB[i])/u0B[i]);
		
	}
	
	// graphics section
	TCanvas *c1;
	c1 = new TCanvas("mcal_data", "MCAL calibration parameter", 1200, 600);
//	c1 = new TCanvas("mcal_data", "MCAL calibration parameter", 800, 600);
	c1->Divide(3, 2);
	c1->cd(1);
	hu0A->Draw();
	c1->cd(2);
	hu0B->Draw();
	c1->cd(3);
	halphaA->Draw();
	c1->cd(4);
	halphaB->Draw();
	c1->cd(5);
	hr->Draw();
	c1->cd(6);
	hrb->Draw();
	
	// text output
	cout << endl << "Current calibration parameter file: " << calFile.Data() << endl << endl;
	printf("i\t bar\t oA (ch)\t oB (ch)\t u0A (ch/MeV)\t u0B (ch/MeV)\t alphaA (cm^-1)\t alphaB (cm^-1)\n\n");
	for (int i=0; i<30; i++) {
		if (i<15) printf("%d\t X%2d\t ", i, i+1);
		else printf("%d\t Z%2d\t ", i, i-14);
		printf("%8.2f\t %8.2f\t %8.3f\t %8.3f\t %8.4f\t %8.4f\n", oA[i], oB[i], u0A[i], u0B[i], alphaA[i], alphaB[i]);
	}
	cout << endl;
	

}
