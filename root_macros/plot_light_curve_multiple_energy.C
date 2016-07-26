{
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
	mcalstyle->SetLabelSize(0.05, "X");
	mcalstyle->SetLabelSize(0.05, "Y");
	gROOT->SetStyle("mcalstyle");

	TString *rootfilename[2];
	double t0[2];
	double tbin[2];
	TTimeStamp ts[2];
	GRB3908 *grb[2];
	GRB3913XZ *grb3913[2];
	TH2D *h2lc[2];
	double zbefore[2];
	double zafter[2];
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	//char *label[] = {"(a)", "(b)", "(c)", "(d)", "(e)", "(f)", "(g)", "(h)", "(k)"};
	
	// input data
	
	// NB: SE SEGMENTA CARICANDO I DATI VUOL DIRE CHE E' STATA MODIFICATA LA LIBRERIA E
	// DEVONO ESSERE RIPROCESSATI I FILE!!! OGNI RESISTENZA E' INUTILE!!!
	
	rootfilename[0] = new TString("/data2/FLIGHT_DATA/1746_1746_MCAL_global_data.root");
	t0[0] = 115091741.696;
	tbin[0] = 1.024;
	zbefore[0] = -50.;
	zafter[0] = 150.;
	
	rootfilename[1] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004946_3908.root");
	t0[1] = 134685725.108208;
	tbin[1] = 0.256;
	zbefore[1] = -20.;
	zafter[1] = 60.;
	
/*	rootfilename[2] = new TString("/data2/FLIGHT_DATA/3758_3758_MCAL_global_data.root");
	t0[2] = 127411769.583;
	tbin[2] = 1.024;
	zbefore[2] = -50.;
	zafter[2] = 150.;
*/
	
/*	rootfilename[8] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004798_3908.root");
	t0[8] = 133776179.856;
	tbin[8] = 0.256 ;
	zbefore[8] = -10.;
	zafter[8] = 30.;
*/		
/* 	// template
	rootfilename[] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT00_3908.root");
	t0[] = ;
	tbin[] = ;
	zbefore[] = ;
	zafter[] = ;
*/
	
	// build light curves (load libMCV.so before)
	
	// work on 3913 data
	for (int i=0; i<1; i++) {
		grb3913[i] = new GRB3913XZ(rootfilename[i]->Data());
		grb3913[i]->LightCurve(t0[i], -200., +200.);
		h2lc[i] = grb3913[i]->GetLightCurve2D(0);
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
		
	}
	
	// work on 3908 data
	for (int i=1; i<2; i++) {
		grb[i] = new GRB3908(rootfilename[i]->Data());
		grb[i]->DefaultLightCurve();
		grb[i]->CustomLightCurve(t0[i], -80., +60., emin, emax, barsX, barsZ, tbin[i]);
		h2lc[i] = grb[i]->GetLightCurve2D();
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
		
	}
		
	// graphic section
	
	double label_size   = 0.05;
	double text_size    = 0.05;
	double title_offset = 1.0;
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 1400, 800);
	c1.Divide(2,4, 0.001, 0.001);
	
	int nrow = 4;
	int ncol = 2;
	for (int i=0; i<ncol; i++) {
		for (int j=0; j<nrow; j++) {
			int k = 1 + j*ncol + i;
			cout << i << " , " << j  << " --> " << k << endl;
			
			c1.cd(k);
			gPad->SetTicks(1,1);
			
			TString ytitle("Counts / ");
			ytitle += tbin[i]*1000.;
			ytitle += " ms";
			
			TString pname("hE");
			pname += j+1;
			pname += "_";
			pname += i+1;
			TH1D *haux = h2lc[i]->ProjectionX(pname.Data(), j+1, j+1, "");
			
			haux->SetStats(0);
			haux->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
			haux->GetYaxis()->SetRangeUser(0., 1.2*haux->GetMaximum());
			haux->SetStats(0);
			haux->SetTitle("");
			haux->GetXaxis()->SetLabelSize(label_size);
			haux->GetXaxis()->SetTitle("Time-T0 (s)");
			haux->GetXaxis()->SetTitleSize(label_size);
			haux->GetXaxis()->CenterTitle();
			haux->GetYaxis()->SetLabelSize(label_size);
			haux->GetYaxis()->SetTitle(ytitle);
			haux->GetYaxis()->SetTitleSize(label_size);
			haux->GetYaxis()->SetTitleOffset(title_offset);
			haux->GetYaxis()->CenterTitle();
			haux->Draw();
				
/*			TLatex t1;
			t1.SetTextSize(text_size);
			TString str1("T0 = ");
			str1 += ts[i]->AsString("s");
			str1 += " UT";
			t1.DrawTextNDC(xpos, zpos, str1);
			
			TLatex t2;
			t2.SetTextSize(text_size);
			t2.DrawTextNDC(0.12, 0.02, label[i]);	
*/	
		}
	}
	
	// plot spectra
	
	TCanvas csp("csp", "csp", 800, 600);
//	TPad *p2 = new TPad("p2", "p2",0.01,0.51,0.99,0.99);
//	p2->Draw();	
//	TPad *p = new TPad("p", "p",0.01,0.01,0.99,0.49);
//	p->Draw();
	
	// 3913 spectrum
	
//	p2->cd();
//	p2->SetTicks(1,1);
//	p2->SetLogx(1);
//	p2->SetLogy(1);
	gPad->SetTicks(1,1);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	int nb2 = 10;
	double spbins2[] = {0.173, 0.350, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 44.8, 89.6, 1433.6};
	float tbkgini2 = -60.;
	float tbkgfin2 = -10.;
	float tgrbini2 = 0.;
	float tgrbfin2 = 20.;
	
	// float dtgrb2 = 30.7384;	// ********* data for contact 1746 (25/8/2007)
	// float dtbkg2 = 51.2276;
	// float spbkg2[] = {1003., 4036., 1857., 1097., 689., 582., 504., 314., 116., 26.};
	// float spgrb2[] = {1016., 5338., 2394., 1264., 827., 602., 376., 197., 69., 13.};
	// grb data obtained running: 
	//plotmcaldata -f 1746_1746_MCAL_global_data.root -z 115091741.696 -nb 50 -na 150 -sp -60 -10 0 30
	
	float dtgrb2 = 27.6664;		// ********* data for contact 4946 (7/4/2008)
//	float dtbkg2 = 51.2276;
	float dtbkg2 = 102.455;
//	float spbkg2[] = {1111, 4565, 2163, 1288, 806, 702, 603, 299, 126, 30}; // bkg fra [-60, -10]
	float spbkg2[] = {2208, 9234, 4327, 2565, 1643, 1352, 1190, 653, 274, 60}; // bkg fra [-110, -10]
	float spgrb2[] = {1485, 6156, 2128, 794, 449, 362, 283, 167, 79, 17};
	// grb data obtained running: 
	// plotmcaldata -f 4946_4946_MCAL_global_data.root -z 134685725.1 -nb 50 -na 150 -sp -60 -10 0.624 27.248
	
	
	printf("i \tE_lo \t\tE_hi \t\tbkg \t\tgrb \t\tgrb counts/s\n");
	for (int i=0; i<nb2; i++) printf("%3d \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f\n", i, spbins2[i],
	spbins2[i+1], spbkg2[i], spgrb2[i], spgrb2[i]/(dtgrb2) - spbkg2[i]/(dtbkg2)); 
	
	TGraphErrors *gsp2 = new TGraphErrors(nb2);
	for (int j=0; j<nb2; j++) {
		double de2 = spbins2[j+1] - spbins2[j];
		double y2 = (spgrb2[j]/dtgrb2 - spbkg2[j]/dtbkg2)/de2;  
		gsp2->SetPoint(j, 0.5*(spbins2[j]+spbins2[j+1]), y2);
		gsp2->SetPointError(j, 0.5*(spbins2[j+1]-spbins2[j]), sqrt(spgrb2[j]/(dtgrb2*dtgrb2)+ spbkg2[j]/(dtbkg2*dtbkg2))/de2); // 
	}
	
	TH1F *hsp2 = gsp2->GetHistogram();
	hsp2->GetXaxis()->SetTitle("Energy (MeV)");
	hsp2->GetXaxis()->SetTitleOffset(1.4);
	hsp2->GetXaxis()->CenterTitle();
	hsp2->GetXaxis()->SetRangeUser(0.1, 10.);
	hsp2->GetYaxis()->SetTitle("Counts/(s MeV)");
	hsp2->GetYaxis()->CenterTitle();
	hsp2->GetYaxis()->SetTitleOffset(1.5);
	hsp2->GetYaxis()->SetRangeUser(0.01, 10000.);
	hsp2->SetTitle("");
	hsp2->GetXaxis()->SetTitleSize(0.04);
	hsp2->GetXaxis()->SetTitleOffset(1.);
	hsp2->GetXaxis()->SetLabelSize(0.04);
	hsp2->GetYaxis()->SetTitleSize(0.04);
	hsp2->GetYaxis()->SetTitleOffset(1.);
	hsp2->GetYaxis()->SetLabelSize(0.04);
	hsp2->Draw();
	gsp2->Draw("PSAME");
	
	// 3908 spectrum
	
	
	// canali_KW.dat
//	int nb = 17;
//	double spbins[] = {0.173, 0.223, 0.277, 0.345, 0.444, 0.607, 0.822, 1.038, 1.308, 1.739, 2.182, 2.863, 3.544, 4.452, 5.815, 11.605, 20.900, 1433.6};

	// canali SRM divisi per 3 (canali SRM moltiplicati per 2^(1/3) = 1.2599 )
	int nb = 20;
	double spbins[] = {0.173, 0.218, 0.275, 0.350, 0.441, 0.556, 0.700, 0.882, 1.111, 1.400, 1.764, 2.222, 2.8, 3.528, 4.444, 5.6, 7.055, 8.889, 11.2, 22.4, 1433.6};
	float tbkgini = -110.;
	float tbkgfin = -10.;
	float tgrbini = 0.624;
	float tgrbfin = 27.248;
	float dtgrb = tgrbfin-tgrbini;
	float dtbkg = tbkgfin-tbkgini;
	float *spbkg = grb[1]->GetPha(tbkgini, tbkgfin, nb, spbins);
	float *spgrb = grb[1]->GetPha(tgrbini, tgrbfin, nb, spbins);
	
	printf("i \tE_lo \t\tE_hi \t\tbkg \t\tgrb \t\tgrb counts/s\n");
	for (int i=0; i<nb; i++) printf("%3d \t%8.3f \t%8.3f \t%8.3f \t%8.3f \t%8.3f\n", i, spbins[i],
	spbins[i+1], spbkg[i], spgrb[i], spgrb[i]/(tgrbfin-tgrbini) - spbkg[i]/(tbkgfin-tbkgini)); 
	
	TGraphErrors *gsp = new TGraphErrors(nb);
	for (int j=0; j<nb; j++) {
		double de = spbins[j+1] - spbins[j];
		double y = (spgrb[j]/dtgrb - spbkg[j]/dtbkg)/de;  
		gsp->SetPoint(j, 0.5*(spbins[j]+spbins[j+1]), y);
		gsp->SetPointError(j, 0.5*(spbins[j+1]-spbins[j]), sqrt(spgrb[j]/(dtgrb*dtgrb) + spbkg[j]/(dtbkg*dtbkg))/de);
	}
	
//	p->cd();
//	p->SetTicks(1,1);
//	p->SetLogx(1);
//	p->SetLogy(1);
/*	TH1F *hsp = gsp->GetHistogram();
	hsp->GetXaxis()->SetTitle("Energy (MeV)");
	hsp->GetXaxis()->SetTitleOffset(1.4);
	hsp->GetXaxis()->CenterTitle();
	hsp->GetXaxis()->SetRangeUser(0.1, 10.);
	hsp->GetYaxis()->SetTitle("Counts/(s MeV)");
	hsp->GetYaxis()->CenterTitle();
	hsp->GetYaxis()->SetTitleOffset(1.5);
	hsp->GetYaxis()->SetRangeUser(0.01, 10000.);
	hsp->SetTitle("");
	hsp->GetXaxis()->SetTitleSize(0.04);
	hsp->GetXaxis()->SetTitleOffset(1.);
	hsp->GetXaxis()->SetLabelSize(0.04);
	hsp->GetYaxis()->SetTitleSize(0.04);
	hsp->GetYaxis()->SetTitleOffset(1.);
	hsp->GetYaxis()->SetLabelSize(0.04);
	hsp->Draw(); */
	gsp->Draw("PSAME");
}
