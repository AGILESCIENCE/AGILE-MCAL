{
	// MM 13/11/2012 apply K-S test on WWLLN distributions for HE and LE TGFs
	
	int itest = 1;  // baseline: 1
	
	// read HE TGF data and sort the array
	
	ifstream fhe;
	//fhe.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/HE_TGF_Mar09-Feb12_BEST-WWLLN_NO-SAA.dat");
	fhe.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/RANDOM-TEST-WWLLN.dat");
	float ahe[1000];
	char saux[20];
	int cont, nw[9];
	float t0, dtmin, dist;
	int nhe = 0;
	while (fhe >> saux >> cont >> t0 >> dtmin >> dist >> nw[0] >> nw[1] >> nw[2] >> nw[3] >> nw[4] >> nw[5] >> nw[6] >> nw[7] >> nw[8]) {
		if (nw[itest]>=0.) {
			ahe[nhe] = nw[itest];
			nhe++;
		}
	}
	//for (int i=0; i<nhe; i++) printf("%3d  %8.0f\n", i, ahe[i]);

	
	// read LE TGF data and sort the array
	
	ifstream fle;
	fle.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/LE_TGF_Mar09-Feb12_BEST-WWLLN_1.dat");
	//fle.open("/home/mcal/data_analysis/VSB/tgf_candidates_complete/RANDOM-TEST-WWLLN.dat");
	float ale[1000];
	int nle = 0;
	while (fle >> saux >> cont >> t0 >> dtmin >> dist >> nw[0] >> nw[1] >> nw[2] >> nw[3] >> nw[4] >> nw[5] >> nw[6] >> nw[7] >> nw[8]) {
		if (nw[itest]>=0.) {
			ale[nle] = nw[itest];
			nle++;
		}
	}
	//for (int i=0; i<nle; i++) printf("%3d  %8.0f\n", i, ale[i]);
	
	
	// sort data and do Kolmogorov test
	
	if (nle<nhe) nhe = nle;
	if (nhe<nle) nle = nhe;
	
	
	int *idxhe = new int[nhe];
	TMath::Sort(nhe, ahe, idxhe, 0);
	double *ahes = new double[nhe+1];
	for (int i=0; i<nhe; i++) ahes[i+1] = ahe[idxhe[i]];
	ahes[0] = 0.;
	for (int i=0; i<nhe; i++) printf("%3d  %8.0f\n", i, ahes[i]);
	
	int *idxle = new int[nle];
	TMath::Sort(nle, ale, idxle, 0);
	double *ales = new double[nle+1]; // NB: faccio così per avere già i bin pronti per il TH1F
	for (int i=0; i<nle; i++) ales[i+1] = ale[idxle[i]];
	ales[0] = 0.;
	//for (int i=0; i<nle; i++) printf("%3d  %8.0f\n", i, ales[i]);
	
	double p = TMath::KolmogorovTest(nhe, &ahes[1], nle, &ales[1], "D");
	
	
	// build empirical distribution functions
	
	TH1F hle("hle", "hle", nle, ales);
	for (int i=1; i<=nle; i++) hle.SetBinContent(i, ((float) i)/nle);
	
	TH1F hhe("hhe", "hhe", nhe, ahes);
	for (int i=1; i<=nhe; i++) hhe.SetBinContent(i, ((float) i)/nhe);
	hhe.SetLineColor(2);
	
	TCanvas c1("c1", "c1", 800, 600);
	
	double xmax = ales[nle];
	if (ahes[nhe] > xmax) xmax = ahes[nhe];
	
	TH2F haux("haux", "haux", 100, 0., xmax, 100, 0., 1.2);
	haux.SetStats(0);
	haux.Draw();
	hle.Draw("same");
	hhe.Draw("same");
	
}
