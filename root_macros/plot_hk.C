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

// load hk data
ifstream fin("/home/martino/AGILE/activities/flight/in-flight_performance/hkmon2.txt");	// file usato per proceedings SPIE 2008 (ncont = 4469)
ifstream fin("/home/martino/AGILE/activities/flight/in-flight_performance/hkmon1000-8000.txt");	// file usato per proceedings IEEE-NSS 2008
int ncont = 6918;
float avg[6918][62];
float err[6918][62];
float contact[6918];
char  flag[10];
float ti[6918];

for (int i=0; i<6918; i++) {
	fin >> flag >> contact[i] >> ti[i];
	for (int j=0; j<62; j++) {
		fin >> avg[i][j] >> err[i][j];
	}
}

// make graph
TGraph *gIC  = new TGraph();
TGraph *gPDV = new TGraph();
TGraph *gTXA = new TGraph();
TGraph *gTXB = new TGraph();
TGraph *gTZA = new TGraph();
TGraph *gTZB = new TGraph();

int nPDV=0;
for (int i=0; i<6918; i++) {
	gIC->SetPoint(i, contact[i], avg[i][0]);
	if (avg[i][1]>-28.8) {	// to not-show false measurements
		gPDV->SetPoint(nPDV, contact[i], avg[i][1]);
		nPDV++;
	}
	gTXA->SetPoint(i, contact[i], avg[i][6]);
	gTXB->SetPoint(i, contact[i], avg[i][7]);
	gTZA->SetPoint(i, contact[i], avg[i][9]);
	gTZB->SetPoint(i, contact[i], avg[i][10]);
	//if (i<100) cout << contact[i] << "  " << avg[i][6] << endl;
}

// graphycs section

TCanvas *c2 = new TCanvas("c2", "c2", 800, 900);	// 800, 600 x 4 grafici
c2->Divide(1,6, 0.01, 0.01);
double lsize = 0.08;

c2->cd(5);
gIC->SetTitle("");
gIC->GetXaxis()->SetRangeUser(1100., 8000.);
gIC->GetXaxis()->SetLabelSize(lsize);
gIC->GetXaxis()->SetTitleSize(lsize);
gIC->GetXaxis()->SetTitle("Contact n.");
gIC->GetXaxis()->CenterTitle();
gIC->GetYaxis()->SetLabelSize(lsize);
gIC->GetYaxis()->SetTitleSize(lsize);
gIC->GetYaxis()->SetTitleOffset(0.6);
gIC->GetYaxis()->SetTitle("mA");
gIC->GetYaxis()->CenterTitle();
gIC->Draw("ALP");
TText t1;
t1.SetTextSize(lsize);
t1.DrawTextNDC(0.15, 0.85, "Input Current");

c2->cd(6);
gPDV->SetTitle("");
gPDV->GetXaxis()->SetRangeUser(1100., 8000.);
gPDV->GetXaxis()->SetLabelSize(lsize);
gPDV->GetXaxis()->SetTitleSize(lsize);
gPDV->GetXaxis()->SetTitle("Contact n.");
gPDV->GetXaxis()->CenterTitle();
gPDV->GetYaxis()->SetLabelSize(lsize);
gPDV->GetYaxis()->SetTitleSize(lsize);
gPDV->GetYaxis()->SetTitleOffset(0.6);
gPDV->GetYaxis()->SetTitle("V");
gPDV->GetYaxis()->CenterTitle();
gPDV->Draw("ALP");
TText t2;
t2.SetTextSize(lsize);
t2.DrawTextNDC(0.15, 0.85, "PD bias");

c2->cd(1);
gTXA->SetTitle("");
gTXA->GetXaxis()->SetRangeUser(1100., 8000.);
gTXA->GetXaxis()->SetLabelSize(lsize);
gTXA->GetXaxis()->SetTitleSize(lsize);
gTXA->GetXaxis()->SetTitle("Contact n.");
gTXA->GetXaxis()->CenterTitle();
gTXA->GetYaxis()->SetLabelSize(lsize);
gTXA->GetYaxis()->SetTitleSize(lsize);
gTXA->GetYaxis()->SetTitleOffset(0.6);
gTXA->GetYaxis()->SetTitle("deg C");
gTXA->GetYaxis()->CenterTitle();
gTXA->Draw("ALP");
TText t3;
t3.SetTextSize(lsize);
t3.DrawTextNDC(0.15, 0.85, "Preamp board XA temperature");

c2->cd(2);
gTXB->SetTitle("");
gTXB->GetXaxis()->SetRangeUser(1100., 8000.);
gTXB->GetXaxis()->SetLabelSize(lsize);
gTXB->GetXaxis()->SetTitleSize(lsize);
gTXB->GetXaxis()->SetTitle("Contact n.");
gTXB->GetXaxis()->CenterTitle();
gTXB->GetYaxis()->SetLabelSize(lsize);
gTXB->GetYaxis()->SetTitleSize(lsize);
gTXB->GetYaxis()->SetTitleOffset(0.6);
gTXB->GetYaxis()->SetTitle("deg C");
gTXB->GetYaxis()->CenterTitle();
gTXB->Draw("ALP");
TText t4;
t4.SetTextSize(lsize);
t4.DrawTextNDC(0.15, 0.85, "Preamp board XB temperature");

c2->cd(3);
gTZA->SetTitle("");
gTZA->GetXaxis()->SetRangeUser(1100., 8000.);
gTZA->GetXaxis()->SetLabelSize(lsize);
gTZA->GetXaxis()->SetTitleSize(lsize);
gTZA->GetXaxis()->SetTitle("Contact n.");
gTZA->GetXaxis()->CenterTitle();
gTZA->GetYaxis()->SetLabelSize(lsize);
gTZA->GetYaxis()->SetTitleSize(lsize);
gTZA->GetYaxis()->SetTitleOffset(0.6);
gTZA->GetYaxis()->SetTitle("deg C");
gTZA->GetYaxis()->CenterTitle();
gTZA->Draw("ALP");
TText t5;
t5.SetTextSize(lsize);
t5.DrawTextNDC(0.15, 0.85, "Preamp board ZA temperature");

c2->cd(4);
gTZB->SetTitle("");
gTZB->GetXaxis()->SetRangeUser(1100., 8000.);
gTZB->GetXaxis()->SetLabelSize(lsize);
gTZB->GetXaxis()->SetTitleSize(lsize);
gTZB->GetXaxis()->SetTitle("Contact n.");
gTZB->GetXaxis()->CenterTitle();
gTZB->GetYaxis()->SetLabelSize(lsize);
gTZB->GetYaxis()->SetTitleSize(lsize);
gTZB->GetYaxis()->SetTitleOffset(0.6);
gTZB->GetYaxis()->SetTitle("deg C");
gTZB->GetYaxis()->CenterTitle();
gTZB->Draw("ALP");
TText t6;
t6.SetTextSize(lsize);
t6.DrawTextNDC(0.15, 0.85, "Preamp board ZB temperature");


}
