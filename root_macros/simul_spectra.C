{
	double eph;
	int nsp = 100;
	
	float hbin[] = {277., 344.54, 362.61, 389.73, 416.84, 443.95, 471.06, 498.17, 552.71, 606.63, 660.55, 714.47, 822.32, 930.16, 1038.01, 1145.85, 1307.62, 1415.47, 1523.31, 1631.16, 1739., 1954.69, 2181.52, 2351.82, 2522.13, 2692.44, 3033.06, 3203.37, 3543.99, 3771.07, 4225.22, 4452.3, 4792.92, 5133.54, 5474.16, 6155.39, 6950.17, 7404.32, 8312.64, 8766.8, 9902.19, 11037.58, 13876.07, 17168.71, 27577.6, 48015.5, 67904.1, 156003., 200000.};
	for (int i=0; i<49; i++) hbin[i] *= 0.001;	// use MeV instead of keV

	
	TF1 *cp = new TF1("cutoffpl", "[0]*pow(x,[1])*exp(-x/[2])", 0.3, 200.);
	cp->SetNpx(1000);
	
	TF1 *bp = new TF1("bknpwrl", "(x<[1])*[3]*pow(x, -[0]) + (x>[1])*[3]*pow([1],[2]-[0])*pow(x,-[2])", 0.3, 200.);
	bp->SetNpx(1000);
	bp->SetParameters(0.5, 7.1, 2.7, 1000.);
	
	TH1F *hsp = new TH1F("hsp", "hsp", 48, hbin);
	TH1F *hsp2 = new TH1F("hsp2", "hsp2", 48, hbin);
	TRandom *rndm = new TRandom();
	
	for (int i=0; i<nsp; i++) {
		//float ecut = rndm->Uniform(6., 15.);
		float ecut = rndm->Landau(6.6, 0.3);
		cp->SetParameters(1., -0.4, ecut);
		cout << i << "  " << ecut << endl;
		
		int nevt = 20;
		for (int j=0; j<nevt; j++) {
			eph = cp->GetRandom(0.3, 200.);
			//cout << j << "  " << eph << endl;
			hsp->Fill(eph);	
			eph = bp->GetRandom(0.3, 200.);
			hsp2->Fill(eph);	
		}
		
	
	}
	
	// normalizza i bin: counts/MeV
//	hsp->Sumw2();
//	hsp2->Sumw2();
	for (int i=1; i<50; i++) {
		float aux = hsp->GetBinContent(i);
		hsp->SetBinContent(i, aux/(hbin[i]-hbin[i-1]));

		float aux2 = hsp2->GetBinContent(i);
		hsp2->SetBinContent(i, aux2/(hbin[i]-hbin[i-1]));
	}

	TCanvas *c1 = new TCanvas();
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	hsp->Draw();
	hsp->GetYaxis()->SetRangeUser(0.01, 1000.);
	
	hsp2->SetLineColor(2);
	hsp2->Draw("SAME");
	
/*	cp->SetParLimits(1, 0.4, 0.4);
	cp->SetParLimits(2, 6.6, 6.6);
	hsp->Fit(cp);
	cp->Draw("SAME");
	
	TF1 *bp = new TF1("bknpwrl", bknpwrl, 0.5, 7.1, 2.7, 1.);
	bp->SetParLimits(1, 6., 10.);
	bp->SetLineColor(2);
	hsp2->Fit(bp);
	bp->Draw("SAME");
*/	
	
/*	// show the Landau function
	TF1 *ln = new TF1("l", "landau", 0.3, 100.)
	ln->SetParameters(1., 6.6, 0.4);
	ln->SetNpx(1000);
	ln->Draw();      
*/
	
}
 