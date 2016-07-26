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

	TString *rootfilename[9];
	double t0[9];
	double tbin[9];
	TTimeStamp ts[9];
	GRB3908 *grb[9];
	GRB3913XZ *grb3913[9];
	TH1D *hlc[9];
	double zbefore[9];
	double zafter[9];
	
	double emin=0.;
	double emax=2000.;
	int barsX=0x7FFF;
	int barsZ=0x7FFF;
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time
	char *label[] = {"(a)", "(b)", "(c)", "(d)", "(e)", "(f)", "(g)", "(h)", "(k)"};
	
	// input data
	
	rootfilename[0] = new TString("/data2/FLIGHT_DATA/1746_1746_MCAL_global_data.root");
	t0[0] = 115091741.696;
	tbin[0] = 1.024;
	zbefore[0] = -50.;
	zafter[0] = 150.;
	
	rootfilename[1] = new TString("/data2/FLIGHT_DATA/3051_3051_MCAL_global_data.root");
	t0[1] = 123083801.634;
	tbin[1] = 1.024;
	zbefore[1] = -50.;
	zafter[1] = 150.;
	
	rootfilename[2] = new TString("/data2/FLIGHT_DATA/3758_3758_MCAL_global_data.root");
	t0[2] = 127411769.583;
	tbin[2] = 1.024;
	zbefore[2] = -50.;
	zafter[2] = 150.;
	
	rootfilename[3] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT003174_3908.root");
	t0[3] = 123832709.400;
	tbin[3] = 0.032;
	zbefore[3] = -4.;
	zafter[3] = 6.;
	
	rootfilename[4] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT003507_3908.root");
	t0[4] = 125871227.170;
	tbin[4] = 0.032;
	zbefore[4] = -4.;
	zafter[4] = 6.;
	
	rootfilename[5] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004172_3908.root");
	t0[5] = 129942289.420;
	tbin[5] = 0.032;
	zbefore[5] = -4.;
	zafter[5] = 6.;
	
	rootfilename[6] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004453_3908.root");
	t0[6] = 131664877.824;
	tbin[6] = 0.256;
	zbefore[6] = -10.;
	zafter[6] = 30.;

	rootfilename[7] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004673_3908.root");
	t0[7] = 133014356.656;
	tbin[7] = 0.256 ;
	zbefore[7] = -10.;
	zafter[7] = 30.;
	
	rootfilename[8] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT004798_3908.root");
	t0[8] = 133776179.856;
	tbin[8] = 0.256 ;
	zbefore[8] = -10.;
	zafter[8] = 30.;
		
/* 	// template
	rootfilename[] = new TString("/data2/mcalsw_output_FLIGHT/RT/RT00_3908.root");
	t0[] = ;
	tbin[] = ;
	zbefore[] = ;
	zafter[] = ;
*/
	
	// build light curves (load libMCV.so before)
	
	// work on 3913 data
	for (int i=0; i<3; i++) {
		grb3913[i] = new GRB3913XZ(rootfilename[i]->Data());
		grb3913[i]->LightCurve(t0[i], -200., +200.);
		hlc[i] = grb3913[i]->GetLightCurve(0);
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
		
	}
	
	// work on 3908 data
	for (int i=3; i<9; i++) {
		grb[i] = new GRB3908(rootfilename[i]->Data());
		grb[i]->DefaultLightCurve();
		grb[i]->CustomLightCurve(t0[i], -80., +50., emin, emax, barsX, barsZ, tbin[i]);
		hlc[i] = grb[i]->GetLightCurve();
		ts[i].Set((int)  t0[i], 0, epoch.GetSec(),0);
		
	}
		
	// graphic section
	
	double label_size   = 0.05;
	double text_size    = 0.05;
	double title_offset = 1.0;
	double xpos = 0.15;
	double zpos = 0.8;
	
	TCanvas c1("c1", "", 1400, 800);
	c1.Divide(3,3, 0.001, 0.001);
	
	int nrow = 3;
	int ncol = 3;
	for (int i=0; i<9; i++) {
		int j = (i%nrow)*ncol + i/nrow + 1;
		cout << i << " --> " << j << endl;
		c1.cd(j);	// simmetrizza l'indice
		gPad->SetTicks(1,1);
		
		TString ytitle("Counts / ");
		ytitle += tbin[i]*1000.;
		ytitle += " ms";
		
		hlc[i]->GetXaxis()->SetRangeUser(zbefore[i], zafter[i]);
		hlc[i]->GetYaxis()->SetRangeUser(0., 1.2*hlc[i]->GetMaximum());
		hlc[i]->SetStats(0);
		hlc[i]->SetTitle("");
		hlc[i]->GetXaxis()->SetLabelSize(label_size);
		hlc[i]->GetXaxis()->SetTitle("Time-T0 (s)");
		hlc[i]->GetXaxis()->SetTitleSize(label_size);
		hlc[i]->GetXaxis()->CenterTitle();
		hlc[i]->GetYaxis()->SetLabelSize(label_size);
		hlc[i]->GetYaxis()->SetTitle(ytitle);
		hlc[i]->GetYaxis()->SetTitleSize(label_size);
		hlc[i]->GetYaxis()->SetTitleOffset(title_offset);
		hlc[i]->GetYaxis()->CenterTitle();
		hlc[i]->Draw();
			
		TLatex t1;
		t1.SetTextSize(text_size);
		TString str1("T0 = ");
		str1 += ts[i]->AsString("s");
		str1 += " UT";
		t1.DrawTextNDC(xpos, zpos, str1);
		
		TLatex t2;
		t2.SetTextSize(text_size);
		t2.DrawTextNDC(0.12, 0.02, label[i]);		
		
	}	

}
