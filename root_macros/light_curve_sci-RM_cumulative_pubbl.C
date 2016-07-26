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

int norbit = 4;	//14
TString *fname[4];
fname[0] = new TString("/data2/data_analysis/5518_5518_MCAL_global_data.root");	// fino a 5531
//fname[1] = new TString("/data2/data_analysis/5519_5519_MCAL_global_data.root");
//fname[2] = new TString("/data2/data_analysis/5520_5520_MCAL_global_data.root");
fname[1] = new TString("/data2/data_analysis/5521_5521_MCAL_global_data.root");
fname[2] = new TString("/data2/data_analysis/5525_5525_MCAL_global_data.root");
fname[3] = new TString("/data2/data_analysis/5528_5528_MCAL_global_data.root");

// data for boost rejection in orbits 5518-5531
int nboost = 5; //15

// original boost info for orbits 5518-5531
// double bini[15] = {138185687.994, 138191403.882, 138197120.261, 138202836.124, 138208552.523, 138214268.323, 138219984.706, 138225700.440, 138231416.801, 138237132.475, 138242848.823, 138248564.452, 138254280.842, 138259996.499, 138265712.961};
// double bfin[15] = {138185847.994, 138191563.882, 138197280.261, 138202996.124, 138208712.523, 138214428.323, 138220144.706, 138225860.440, 138231576.801, 138237292.475, 138243008.823, 138248724.452, 138254440.842, 138260156.499, 138265872.961};

double bini[5] = {138185687.994, 138202836.124, 138225700.440, 138242848.823, 138248564.452};	// 2 boost nell'orbita 5528!
double bfin[5] = {138185847.994, 138202996.124, 138225860.440, 138243008.823, 138248724.452};

// data for enter idle/enter obs rejection
int nidle = 4;
double idini[4] = {4862., 4913., 4965., 4930.}; 
double idfin[4] = {4882., 4942., 4995., 4960.};

TGraphErrors *grX[14];	//nb
TGraphErrors *grZ[14];
TGraphErrors *grAC4[14];

TGraph *grXn[14][9];
TGraph *grZn[14][9];

for (int h=0; h<norbit; h++) {
	cout << "operating on file " << fname[h]->Data() << endl;
	TFile *fi= new TFile(fname[h]->Data());
	fi->ls();

	TGraph *gx1=NULL;
	fi->GetObject("grmX1", gx1);
	int ns = gx1->GetN();
	double *data = new double[22*ns];
	double *dataAC = new double[5*ns];
	double *x=NULL;
	char gname[10];
	
	// retrieve MCAL sci. RM data
	for (int i=0; i<22; i++) {
		if (i<11) sprintf(gname, "grmX%d", i+1);
		else sprintf(gname, "grmZ%d", i-10);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			if (!x) x = gaux->GetX();
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) data[j*22 + i] = y[j];
		}
	}
	
	// retrieve AC sci. RM data
	for (int i=0; i<5; i++) {
		sprintf(gname, "gAC%d", i);
		TGraph *gaux=NULL;
		fi->GetObject(gname, gaux);
		if (gaux) {
			double *y = gaux->GetY();
			for (int j=0; j<ns; j++) dataAC[j*5 + i] = y[j];
		}
	}
	
	// refer time to t0
	double t0 = x[0];
	for (int j=0; j<ns; j++) x[j] -= t0;
	double binitmp[nboost];	
	double bfintmp[nboost];	
	for (int j=0; j<nboost; j++) {
		binitmp[j] = bini[j]-t0;
		bfintmp[j] = bfin[j]-t0;
	}
	
	// create graphs and arrays
	int bin = 16;	// rebin factor 16
	int nb = ns/bin+1;
	int ib=0;
	int ibAC=0;
	double yauX=0.;
	double yauZ=0.;
	double yauAC4=0.;
	double yauXn[9];
	double yauZn[9];

	TString grXname("grX_");
	grXname += h;
	grX[h] = new TGraphErrors(100);	//nb
	grX[h]->SetName(grXname);
	TString grZname("grZ_");
	grZname += h;
	grZ[h] = new TGraphErrors(100);
	grZ[h]->SetName(grZname);
	TString grAC4name("grAC4_");
	grAC4name += h;
	grAC4[h] = new TGraphErrors(100);
	grAC4[h]->SetName(grAC4name);

	for (int i=0; i<9; i++) {
		grXn[h][i] = new TGraph(100);
		grZn[h][i] = new TGraph(100);
		yauXn[i] = 0.;
		yauZn[i] = 0.;
	}
	
	// main loop: build graphs
	for (int j=1; j<ns; j++) {
		
		for (int i=1; i<11; i++) yauX += data[j*22 + i];
		for (int i=12; i<22; i++) yauZ += data[j*22 + i];
		yauAC4 += dataAC[j*5 + 4];
		
		for (int i=0; i<9; i++) {
			yauXn[i] += data[j*22 + i + 2];
			yauZn[i] += data[j*22 + i + 13];
		}
		
		if (j%bin == bin-1) {
		
			double time = 0.5*(x[j]+x[j-bin]+1.024);
			bool boost=0;
			bool idle=0;
			
			//if (time<=bfin[h] && time>=bini[h]) boost = 1;
			
			for (int k=0; k<nboost; k++) {
				if (binitmp[k]<=time) {
					if (bfintmp[k]>=time) {
						boost = 1;
						break;
					}
				}
				else break;
			}
			
			if (time <= idfin[h] && time >= idini[h]) idle = 1;
							
			if (!boost && !idle) {
				grX[h]->SetPoint(ib, time, yauX/bin);
				grX[h]->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauX)/bin
				grZ[h]->SetPoint(ib, time, yauZ/bin);
				grZ[h]->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauZ)/bin
				
				for (int i=0; i<9; i++) {
					grXn[h][i]->SetPoint(ib, time, yauXn[i]/bin);
					grZn[h][i]->SetPoint(ib, time, yauZn[i]/bin);
				}
				
				ib++;
			}
			grAC4[h]->SetPoint(ib, time, yauAC4/bin);
			grAC4[h]->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauAC4)/bin
			ibAC++;
		
			yauX = 0.;
			yauZ = 0.;
			yauAC4=0.;
			for (int i=0; i<9; i++) {
				yauXn[i] = 0.;
				yauZn[i] = 0.;
			}
			
		}
	}
		
	// close everything and free memory
	fi->Close();
	delete data;
	delete dataAC;
}

// graphycs section

TCanvas *c2 = new TCanvas();
c2->Divide(1,3, 0.01, 0.01);
c2->cd(1);
double lsize = 0.07;
//double *yX = gaux->GetY();
//grX->GetXaxis()->SetLimits(TMath::MinElement(ns,x), TMath::MaxElement(ns,x));
//grX->GetHistogram()->SetMaximum(1.1*TMath::MaxElement(ns,y));
//grX->GetHistogram()->SetMinimum(0.);
for (h=0; h<norbit; h++) {
	if (h==0) {
		grX[h]->SetTitle("");
		grX[h]->GetXaxis()->SetRangeUser(0., 86000.);
		grX[h]->GetXaxis()->SetLabelSize(lsize);
		grX[h]->GetXaxis()->SetTitleSize(lsize);
		grX[h]->GetXaxis()->SetTitle("Time (s)");
		grX[h]->GetXaxis()->CenterTitle();
		grX[h]->GetYaxis()->SetLabelSize(lsize);
		grX[h]->GetYaxis()->SetTitleSize(lsize);
		grX[h]->GetYaxis()->SetTitleOffset(0.6);
		grX[h]->GetYaxis()->SetTitle("Counts / 1.024 s");
		grX[h]->GetYaxis()->CenterTitle();
		grX[h]->Draw("ALP");
	} else grX[h]->Draw("LPSAME");
}

TText t1;
t1.SetTextSize(0.07);
t1.DrawTextNDC(0.15, 0.85, "Plane X integrated SRM");

c2->cd(2);
for (h=0; h<norbit; h++) {
	if (h==0) {
		grZ[h]->SetTitle("");
		grZ[h]->GetXaxis()->SetRangeUser(0., 86000.);
		grZ[h]->GetXaxis()->SetLabelSize(lsize);
		grZ[h]->GetXaxis()->SetTitleSize(lsize);
		grZ[h]->GetXaxis()->SetTitle("Time (s)");
		grZ[h]->GetXaxis()->CenterTitle();
		grZ[h]->GetYaxis()->SetLabelSize(lsize);
		grZ[h]->GetYaxis()->SetTitleSize(lsize);
		grZ[h]->GetYaxis()->SetTitleOffset(0.6);
		grZ[h]->GetYaxis()->SetTitle("Counts / 1.024 s");
		grZ[h]->GetYaxis()->CenterTitle();
		grZ[h]->Draw("ALP");
	} else grZ[h]->Draw("LPSAME");
}

TText t2;
t2.SetTextSize(0.07);
t2.DrawTextNDC(0.15, 0.85, "Plane Z integrated SRM");

c2->cd(3);
for (h=0; h<norbit; h++) {
	if (h==0) {
		grAC4[h]->SetTitle("");
		grAC4[h]->GetXaxis()->SetRangeUser(0., 86000.);
		grAC4[h]->GetXaxis()->SetLabelSize(lsize);
		grAC4[h]->GetXaxis()->SetTitleSize(lsize);
		grAC4[h]->GetXaxis()->SetTitle("Time (s)");
		grAC4[h]->GetXaxis()->CenterTitle();
		grAC4[h]->GetYaxis()->SetLabelSize(lsize);
		grAC4[h]->GetYaxis()->SetTitleSize(lsize);
		grAC4[h]->GetYaxis()->SetTitleOffset(0.6);
		grAC4[h]->GetYaxis()->SetTitle("Counts / 1.024 s");
		grAC4[h]->GetYaxis()->CenterTitle();
		grAC4[h]->Draw("ALP");
	} else grAC4[h]->Draw("LPSAME");
}

TText t3;
t3.SetTextSize(0.07);
t3.DrawTextNDC(0.15, 0.85, "Anti-Coincidence panel 4 SRM");

TCanvas *c3 = new TCanvas("c3", "c3", 800, 1000);
c3->Divide(2,9, 0.01, 0.01);

Float_t tmpEband[11]={0.0875, 0.2625, 0.525, 1.05, 2.1, 4.2, 8.4, 16.8, 33.6, 67.2, 761.6};
Float_t tmpEerr[11]={0.0875, 0.0875, 0.175, 0.35, 0.7, 1.4, 2.8, 5.6, 11.2, 22.4, 672.};

lsize = 0.12;
for (int i=0; i<9; i++) {
	c3->cd(i*2+1);
	for (h=0; h<norbit; h++) {
		if (h==0) {	
			grXn[h][i]->SetTitle("");
			grXn[h][i]->GetXaxis()->SetRangeUser(0., 86000.);
			grXn[h][i]->GetXaxis()->SetLabelSize(lsize);
			grXn[h][i]->GetXaxis()->SetTitleSize(lsize);
			grXn[h][i]->GetXaxis()->SetTitle("Time (s)");
			grXn[h][i]->GetXaxis()->CenterTitle();
			grXn[h][i]->GetYaxis()->SetLabelSize(lsize);
			grXn[h][i]->GetYaxis()->SetTitleSize(lsize);
			grXn[h][i]->GetYaxis()->SetTitleOffset(0.3);
			grXn[h][i]->GetYaxis()->SetTitle("Counts / 1.024 s");
			grXn[h][i]->GetYaxis()->CenterTitle();
			grXn[h][i]->Draw("ALP");
		} else grXn[h][i]->Draw("LPSAME");
	}
	
	char ebandx[100];
	sprintf(ebandx, "Plane X %5.2f - %5.2f MeV", tmpEband[i+2] - tmpEerr[i+2], tmpEband[i+2] + tmpEerr[i+2]);
	TText t4;
	t4.SetTextSize(lsize);
	t4.DrawTextNDC(0.15, 0.75, ebandx);

	
	c3->cd(i*2+2);
	for (h=0; h<norbit; h++) {
		if (h==0) {	
			grZn[h][i]->SetTitle("");
			grZn[h][i]->GetXaxis()->SetRangeUser(0., 86000.);
			grZn[h][i]->GetXaxis()->SetLabelSize(lsize);
			grZn[h][i]->GetXaxis()->SetTitleSize(lsize);
			grZn[h][i]->GetXaxis()->SetTitle("Time (s)");
			grZn[h][i]->GetXaxis()->CenterTitle();
			grZn[h][i]->GetYaxis()->SetLabelSize(lsize);
			grZn[h][i]->GetYaxis()->SetTitleSize(lsize);
			grZn[h][i]->GetYaxis()->SetTitleOffset(0.3);
			grZn[h][i]->GetYaxis()->SetTitle("Counts / 1.024 s");
			grZn[h][i]->GetYaxis()->CenterTitle();
			grZn[h][i]->Draw("ALP");
		} else grZn[h][i]->Draw("LPSAME");
	}
	
	char ebandz[100];
	sprintf(ebandz, "Plane Z %5.2f - %5.2f MeV", tmpEband[i+2] - tmpEerr[i+2], tmpEband[i+2] + tmpEerr[i+2]);
	TText t5;
	t5.SetTextSize(lsize);
	t5.DrawTextNDC(0.15, 0.75, ebandz);
}
