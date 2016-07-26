#include "mcalibration.h"
#include "version.h"



int main(int argc, char *argv[])
{
	// set graphics environment
	TStyle *mcalstyle;
	
	cout << "mcalibration version " << VERSION << endl;
	
	TApplication theApp("App", &argc, argv);
	mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.06, "xyz");
	mcalstyle->SetTitleSize(0.06, "xyz");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	TFile *fp = new TFile("calibration_output.root", "recreate");
	MCVCalibration *calib = new MCVCalibration();
	
	drawPdGraphics(calib);
	drawLightAsymmetry(calib);
	fp->Close();
		
	theApp.Run();
	return 0;
}

//
//--------------------------------------------------------------------
//

void drawPdGraphics(MCVCalibration* calib)
{
	TString cname("Na22 PD calibration data  -  viewer version ");
	cname += VERSION;
	TCanvas *c3 = new TCanvas("calib_data", cname.Data(), 800, 600);
	c3->Divide(5, 6);
	TObjArray *hcA = calib->GetPdAGraph();
	TObjArray *hcB = calib->GetPdBGraph();
	TObjArray *fcA = new TObjArray(30,0);
	fcA->SetName("fcA");
	TObjArray *fcB = new TObjArray(30,0);
	fcB->SetName("fcB");
	
	for (int i=0; i<30; i++) {
		c3->cd(i+1);
		TGraphErrors *gauxA = NULL, *gauxB = NULL;
		TString gnameA("gA");
		TString gnameB("gB");
		gnameA += i;
		gnameB += i;
		gauxA = (TGraphErrors *) hcA->FindObject(gnameA.Data());
		gauxB = (TGraphErrors *) hcB->FindObject(gnameB.Data());
		if (gauxA) gauxA->Draw("AP");
		else cout << "cannot find graph " << gnameA.Data() << endl;
		if (gauxB) gauxB->Draw("PSAME");
		else cout << "cannot find graph " << gnameB.Data() << endl;
		
		// do polinomial fitting
		TString fnameA("fA");
		fnameA += i;
		TF1 *fA = new TF1(fnameA.Data(),"pol3", -19., 19.);	
		fA->SetLineColor(2);
		gauxA->Fit(fA,"R0+");
		fA->SetLineWidth(1);
		fA->Draw("same");
		fcA->AddLast(fA);
		
		TString fnameB("fB");
		fnameB += i;
		TF1 *fB = new TF1(fnameB.Data(),"pol3", -19., 19.);	
		fB->SetLineColor(3);
		gauxB->Fit(fB,"R0+");
		fB->SetLineWidth(1);
		fB->Draw("same");
		fcB->AddLast(fB);
	}
	fcA->Write("fcA", TObject::kSingleKey);
	fcB->Write("fcB", TObject::kSingleKey);
	return;
}

//
//--------------------------------------------------------------------
//

void drawLightAsymmetry(MCVCalibration* calib)
{
	TString cname("Lyght asymmetry function ln(B/A)  -  viewer version ");
	cname += VERSION;
	TCanvas *c4 = new TCanvas("light_asymmetry", cname.Data(), 800, 600);
	c4->Divide(5, 6);
	TObjArray *hcBonA = calib->GetBonAGraph();
	TObjArray *fcBonA = new TObjArray(30,0);
	fcBonA->SetName("fcBonA");
	
	for (int i=0; i<30; i++) {
		c4->cd(i+1);
		TGraphErrors *gauxBonA = NULL;
		TString gname("gBonA");
		gname += i;
		gauxBonA = (TGraphErrors *) hcBonA->FindObject(gname.Data());
		if (gauxBonA) gauxBonA->Draw("AP");
		else cout << "cannot find graph " << gname.Data() << endl;
		
		// do polinomial fitting
		TString fname("fBonA");
		fname += i;
		TF1 *totalpol = new TF1(fname.Data(),"pol3", -19., 19.);	
		totalpol->SetLineColor(2);
		gauxBonA->Fit(totalpol,"R0+");
		totalpol->SetLineWidth(1);
		totalpol->Draw("same");
		fcBonA->AddLast(totalpol);
	}
	fcBonA->Write("fcBonA", TObject::kSingleKey);
	return;
}

