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

TFile *fi= new TFile("/data2/data_analysis/5518_5531_MCAL_global_data.root");

// data for boost rejection in orbits 5518-5531
int nboost = 15;
double bini[15] = {138185687.994, 138191403.882, 138197120.261, 138202836.124, 138208552.523, 138214268.323, 138219984.706, 138225700.440, 138231416.801, 138237132.475, 138242848.823, 138248564.452, 138254280.842, 138259996.499, 138265712.961};
double bfin[15] = {138185847.994, 138191563.882, 138197280.261, 138202996.124, 138208712.523, 138214428.323, 138220144.706, 138225860.440, 138231576.801, 138237292.475, 138243008.823, 138248724.452, 138254440.842, 138260156.499, 138265872.961};

// data for enter idle/enter obs rejection
int nidle = 14;
double idini[14] = {4865., 11000., 17145., 23300., 29445., 35590., 41720., 47840., 53955., 60060., 66150., 72240., 78320., 84440.}; 
double idfin[14] = {4885., 11020., 17165., 23320., 29465., 35610., 41740., 47860., 53975., 60080., 66170., 72260., 78360., 84460.};

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
for (int j=0; j<nboost; j++) {
	bini[j] = bini[j]-t0;
	bfin[j] = bfin[j]-t0;
	cout << bini[j] << "  " << bfin[j] << endl;
}

int bin = 100;	// rebin factor
int nb = ns/bin+1;
int ib=0;
int ibAC=0;
double yauX=0.;
double yauZ=0.;
double yauAC4=0.;
double yauXn[9];
double yauZn[9];

TGraphErrors *grX = new TGraphErrors(100);	//nb
TGraphErrors *grZ = new TGraphErrors(100);
TGraphErrors *grAC4 = new TGraphErrors(100);

TGraph *grXn[9];
TGraph *grZn[9];
for (int i=0; i<9; i++) {
	grXn[i] = new TGraph(100);
	grZn[i] = new TGraph(100);
	yauXn[i] = 0.;
	yauZn[i] = 0.;
}

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
		
		for (int k=0; k<nboost; k++) {
			if (bini[k]<=time) {
				if (bfin[k]>=time) {
					boost = 1;
					break;
				}
			}
			else break;
		}
		
		for (int k=0; k<nidle; k++) {
			if (idini[k]<=time) {
				if (idfin[k]>=time) {
					idle = 1;
					break;
				}
			}
			else break;
		}
			
		if (!boost && !idle) {
			grX->SetPoint(ib, time, yauX/bin);
			grX->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauX)/bin
			grZ->SetPoint(ib, time, yauZ/bin);
			grZ->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauZ)/bin
			
			for (int i=0; i<9; i++) {
				grXn[i]->SetPoint(ib, time, yauXn[i]/bin);
				grZn[i]->SetPoint(ib, time, yauZn[i]/bin);
			}
			
			ib++;
		}
		grAC4->SetPoint(ib, time, yauAC4/bin);
		grAC4->SetPointError(ib, 0., 0.);	//0.5*(x[j]-x[j-bin]), sqrt(yauAC4)/bin
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

// graphycs section

TCanvas *c2 = new TCanvas();
c2->Divide(1,3, 0.01, 0.01);
c2->cd(1);
double lsize = 0.07;
//double *yX = gaux->GetY();
//grX->GetXaxis()->SetLimits(TMath::MinElement(ns,x), TMath::MaxElement(ns,x));
//grX->GetHistogram()->SetMaximum(1.1*TMath::MaxElement(ns,y));
//grX->GetHistogram()->SetMinimum(0.);
grX->SetTitle("");
grX->GetXaxis()->SetRangeUser(0., 86000.);
grX->GetXaxis()->SetLabelSize(lsize);
grX->GetXaxis()->SetTitleSize(lsize);
grX->GetXaxis()->SetTitle("Time (s)");
grX->GetXaxis()->CenterTitle();
grX->GetYaxis()->SetLabelSize(lsize);
grX->GetYaxis()->SetTitleSize(lsize);
grX->GetYaxis()->SetTitleOffset(0.6);
grX->GetYaxis()->SetTitle("Counts / 1.024 s");
grX->GetYaxis()->CenterTitle();
grX->Draw("ALP");

TText t1;
t1.SetTextSize(0.07);
t1.DrawTextNDC(0.15, 0.85, "Plane X integrated SRM");

c2->cd(2);
grZ->SetTitle("");
grZ->GetXaxis()->SetRangeUser(0., 86000.);
grZ->GetXaxis()->SetLabelSize(lsize);
grZ->GetXaxis()->SetTitleSize(lsize);
grZ->GetXaxis()->SetTitle("Time (s)");
grZ->GetXaxis()->CenterTitle();
grZ->GetYaxis()->SetLabelSize(lsize);
grZ->GetYaxis()->SetTitleSize(lsize);
grZ->GetYaxis()->SetTitleOffset(0.6);
grZ->GetYaxis()->SetTitle("Counts / 1.024 s");
grZ->GetYaxis()->CenterTitle();
grZ->Draw("ALP");

TText t2;
t2.SetTextSize(0.07);
t2.DrawTextNDC(0.15, 0.85, "Plane Z integrated SRM");

c2->cd(3);
grAC4->SetTitle("");
grAC4->GetXaxis()->SetRangeUser(0., 86000.);
grAC4->GetXaxis()->SetLabelSize(lsize);
grAC4->GetXaxis()->SetTitleSize(lsize);
grAC4->GetXaxis()->SetTitle("Time (s)");
grAC4->GetXaxis()->CenterTitle();
grAC4->GetYaxis()->SetLabelSize(lsize);
grAC4->GetYaxis()->SetTitleSize(lsize);
grAC4->GetYaxis()->SetTitleOffset(0.6);
grAC4->GetYaxis()->SetTitle("Counts / 1.024 s");
grAC4->GetYaxis()->CenterTitle();
grAC4->Draw("ALP");

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
	grXn[i]->SetTitle("");
	grXn[i]->GetXaxis()->SetRangeUser(0., 86000.);
	grXn[i]->GetXaxis()->SetLabelSize(lsize);
	grXn[i]->GetXaxis()->SetTitleSize(lsize);
	grXn[i]->GetXaxis()->SetTitle("Time (s)");
	grXn[i]->GetXaxis()->CenterTitle();
	grXn[i]->GetYaxis()->SetLabelSize(lsize);
	grXn[i]->GetYaxis()->SetTitleSize(lsize);
	grXn[i]->GetYaxis()->SetTitleOffset(0.3);
	grXn[i]->GetYaxis()->SetTitle("Counts / 1.024 s");
	grXn[i]->GetYaxis()->CenterTitle();
	grXn[i]->Draw("ALP");
	
	char ebandx[100];
	sprintf(ebandx, "Plane X %5.2f - %5.2f MeV", tmpEband[i+2] - tmpEerr[i+2], tmpEband[i+2] + tmpEerr[i+2]);
	TText t4;
	t4.SetTextSize(lsize);
	t4.DrawTextNDC(0.15, 0.75, ebandx);

	
	c3->cd(i*2+2);
	grZn[i]->SetTitle("");
	grZn[i]->GetXaxis()->SetRangeUser(0., 86000.);
	grZn[i]->GetXaxis()->SetLabelSize(lsize);
	grZn[i]->GetXaxis()->SetTitleSize(lsize);
	grZn[i]->GetXaxis()->SetTitle("Time (s)");
	grZn[i]->GetXaxis()->CenterTitle();
	grZn[i]->GetYaxis()->SetLabelSize(lsize);
	grZn[i]->GetYaxis()->SetTitleSize(lsize);
	grZn[i]->GetYaxis()->SetTitleOffset(0.3);
	grZn[i]->GetYaxis()->SetTitle("Counts / 1.024 s");
	grZn[i]->GetYaxis()->CenterTitle();
	grZn[i]->Draw("ALP");
	
	char ebandz[100];
	sprintf(ebandz, "Plane Z %5.2f - %5.2f MeV", tmpEband[i+2] - tmpEerr[i+2], tmpEband[i+2] + tmpEerr[i+2]);
	TText t5;
	t5.SetTextSize(lsize);
	t5.DrawTextNDC(0.15, 0.75, ebandz);
}

}
