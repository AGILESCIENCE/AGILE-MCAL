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


// first commissioning 3913 file
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H000130_3913.root");
//TFile fi("~/local/mcalsw/bin/mcalsw_output/H/H000132_3913.root");
TFile fi("~/mcalsw_output_COMMISSIONING/H/H000132_3913.root");

TObjArray *ai = (TObjArray *) fi->Get("rawHisto");

TGraphErrors *hi, *hz;
hi = (TGraphErrors *) ai->FindObject("gRMX3913");
hz = (TGraphErrors *) ai->FindObject("gRMZ3913");

TCanvas *c1;
c1 = new TCanvas();
c1->Draw();
gPad->SetLogx();
gPad->SetLogy();
hi->SetTitle("MCAL Scientific Ratemeters count spectra");
hi->SetTitle(0);
hi->GetXaxis()->SetLimits(0.1, 2000.);
hi->GetXaxis()->SetTitle("Energy (MeV)");
hi->GetYaxis()->SetTitle("Counts / s");
hi->SetMarkerColor(4);
hi->SetMarkerStyle(10);
hi->SetLineColor(4);
hi->Draw("AP");

hz->SetMarkerColor(2);
hz->SetMarkerStyle(10);
hz->SetLineColor(2);
hz->Draw("PSAME");

TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
leg->AddEntry(hi, "contact 131 X-plane, AC OFF", "lp");
leg->AddEntry(hz, "contact 131 Z-plane, AC OFF", "lp");

if (1) {	// add a second spectrum
	TFile fe("~/mcalsw_output_COMMISSIONING/H/H000226_3909.root");
	
	TObjArray *ae = (TObjArray *) fe->Get("rawHisto");
	
	TGraphErrors *he, *hez;
	he = (TGraphErrors *) ae->FindObject("gRMX3913");
	hez = (TGraphErrors *) ae->FindObject("gRMZ3913");

	he->SetMarkerColor(4);
	he->SetLineColor(4);
	he->SetLineStyle(2);
	he->Draw("PSAME");
	
	hez->SetMarkerColor(2);
	hez->SetLineColor(2);
	hez->SetLineStyle(2);
	hez->Draw("PSAME");
	
	leg->AddEntry(he, "contact 226 X-plane, AC ON nominal w=20", "lp");
	leg->AddEntry(hez, "contact 226 Z-plane, AC ON nominal w=20", "lp");
}

if (1) {	// add a third spectrum
	TFile fe2("~/mcalsw_output_COMMISSIONING/H/H000242_3913.root");
	
	TObjArray *ae2 = (TObjArray *) fe2->Get("rawHisto");
	
	TGraphErrors *he2, *hez2;
	he2 = (TGraphErrors *) ae2->FindObject("gRMX3913");
	hez2 = (TGraphErrors *) ae2->FindObject("gRMZ3913");

	he2->SetMarkerColor(4);
	he2->SetLineColor(4);
	he2->SetLineStyle(3);
	he2->Draw("PSAME");
	
	hez2->SetMarkerColor(2);
	hez2->SetLineColor(2);
	hez2->SetLineStyle(3);
	hez2->Draw("PSAME");
	
	leg->AddEntry(he2, "contact 242 X-plane, AC ON nominal w=24", "lp");
	leg->AddEntry(hez2, "contact 242 Z-plane, AC ON nominal w=24", "lp");
}

leg->Draw();

}
