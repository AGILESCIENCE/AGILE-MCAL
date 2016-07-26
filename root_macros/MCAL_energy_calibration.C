Calibrate()
{
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "xyz");
	mcalstyle->SetTitleSize(0.05, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	int runid, bar, a1[30], a2[30], a0[30], cnts;
	char tit[100], cc;
	float E, ki2, dev;
	float c0[30], c1[30], c2[30], s1[30], s2[30], s0[30];
	FILE *fp0 = NULL;
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	
	fp0 = fopen("/home/martino/mcalsw/bin/mcalsw_output/misc/peak_3899_1275keV_4.txt", "r");
	if (!fp0) cout << "*** Cannot open file 0" << endl;
	fscanf(fp0, "run	%d\n", &runid);
	fscanf(fp0, "histo	%s\n", &tit);
	fscanf(fp0, "E 	%f\n", &E);
//	fscanf(fp0, "\n");
	fscanf(fp0, "%s\n", &tit);
	for (int i=0; i<12; i++) fscanf(fp0, "%s", &tit);
//	fscanf(fp0, "plane 	bar 	center (MeV) 	sigma (MeV) 	ampl 	counts 	red. Ki^2 	deviation (%%)\n");
	for (int i=0; i<30; i++) fscanf(fp0, "%c %d %f %f %d %d %f %f\n", &cc, &bar, &c0[i], &s0[i], &a0[i], &cnts, &ki2, &dev);
	fclose(fp0);
	
	fp1 = fopen("/home/martino/mcalsw/bin/mcalsw_output/misc/peak_3785_1461keV_8.txt", "r");
	if (!fp1) cout << "*** Cannot open file 1" << endl;
	fscanf(fp1, "run	%d\n", &runid);
	fscanf(fp1, "histo	%s\n", &tit);
	fscanf(fp1, "E 	%f\n", &E);
	fscanf(fp0, "%s\n", &tit);
	for (int i=0; i<12; i++) fscanf(fp0, "%s", &tit);
	for (int i=0; i<30; i++) fscanf(fp1, "%c %d %f %f %d %d %f %f\n", &cc, &bar, &c1[i], &s1[i], &a1[i], &cnts, &ki2, &dev);
	fclose(fp1);
	
	fp2 = fopen("/home/martino/mcalsw/bin/mcalsw_output/misc/peak_3785_2615keV_4.txt", "r");
	if (!fp2) cout << "*** Cannot open file 2" << endl;
	fscanf(fp2, "run	%d\n", &runid);
	fscanf(fp2, "histo	%s\n", &tit);
	fscanf(fp2, "E 	%f\n", &E);
	fscanf(fp0, "%s\n", &tit);
	for (int i=0; i<12; i++) fscanf(fp0, "%s", &tit);
	for (int i=0; i<30; i++) fscanf(fp2, "%c %d %f %f %d %d %f %f\n", &cc, &bar, &c2[i], &s2[i], &a2[i], &cnts, &ki2, &dev);
	fclose(fp2);
	
	TCanvas *ca = new TCanvas("ca", "calibration curve", 800, 600);
	ca->Divide(6, 5);
	TGraph *gaux;
	char gname[100];
	double par[3];
	printf("Linear fit E = a + bE' \nplane \tbar \ta (MeV) \tb \t\tcorr. coeff. \n");
	for (int i=0; i<30; i++) {
		ca->cd(i+1);
		sprintf(gname, "gr_%2d", i);
		gaux = new TGraph(3);
		gaux->SetName(gname);
		TString title = "Bar ";
		title += (i<15 ? 'X' : 'Z');
		title += (i<15 ? i+1 : i-14);
		gaux->SetTitle(title);
		gaux->SetMarkerStyle(2);
		gaux->SetPoint(1, c0[i], 1.275);
		gaux->SetPoint(2, c1[i], 1.461);
		gaux->SetPoint(3, c2[i], 2.615);
		gaux->GetXaxis()->SetTitle("Calc. energy (MeV)");
		gaux->GetYaxis()->SetTitle("True energy (MeV)");
		gaux->Draw("AP");
		TF1 *flin = new TF1("flin", "pol1", 0., 3.);
		gaux->Fit(flin, "QR0+");
		flin->SetLineWidth(2);
		flin->SetLineColor(2);
		flin->Draw("same");
		flin->GetParameters(par);
		printf("%c \t%2d \t%8.4f \t%8.4f \t%8.4f \n", (i<15 ? 'X' : 'Z'), (i<15 ? i+1 : i-14), par[0], par[1], gaux->GetCorrelationFactor());
		ca->Update();
	}
}
