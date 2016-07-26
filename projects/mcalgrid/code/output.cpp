//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "mcalgrid.h"
	
// output routines
void drawGridHisto(TObjArray *hc)
{
	// graphic section
	TCanvas *c1 = new TCanvas("grid_data", "GRID data", 800, 600);
	c1->Divide(4, 4);
	
	c1->cd(1);
	gPad->SetLogy();
	TH1F *hScang = (TH1F *) hc->FindObject("hScang");
	TH1F *hScangS = (TH1F *) hc->FindObject("hScangS");
	hScang->Draw();
	hScangS->Draw("SAME");
	
	c1->cd(2);
	gPad->SetLogy();
	TH1F *hRX = (TH1F *) hc->FindObject("hRX");
	TH1F *hRXS = (TH1F *) hc->FindObject("hRXS");
	hRX->Draw();
	hRXS->Draw("SAME");
	
	c1->cd(3);
	gPad->SetLogy();
	TH1F *hRZ = (TH1F *) hc->FindObject("hRZ");
	TH1F *hRZS = (TH1F *) hc->FindObject("hRZS");
	hRZ->Draw();
	hRZS->Draw("SAME");
	
	c1->cd(4);
	TH1F *hTheta = (TH1F *) hc->FindObject("hTheta");
	TH1F *hThetaS = (TH1F *) hc->FindObject("hThetaS");
	hTheta->Draw();
	hThetaS->Draw("SAME");
	
	c1->cd(5);
	TH1F *hPhi = (TH1F *) hc->FindObject("hPhi");
	TH1F *hPhiS = (TH1F *) hc->FindObject("hPhiS");
	hPhi->Draw();
	hPhiS->Draw("SAME");
	
	c1->cd(6);
	TH1F *hNseg = (TH1F *) hc->FindObject("hNseg");
	TH1F *hNsegS = (TH1F *) hc->FindObject("hNsegS");
	hNseg->Draw();
	hNsegS->Draw("SAME");
	
	c1->cd(7);
	TH1F *hIniPlane = (TH1F *) hc->FindObject("hIniPlane");
	TH1F *hIniPlaneS = (TH1F *) hc->FindObject("hIniPlaneS");
	hIniPlane->Draw();
	hIniPlaneS->Draw("SAME");
	
	c1->cd(8);
	TH1F *hFinPlane = (TH1F *) hc->FindObject("hFinPlane");
	TH1F *hFinPlaneS = (TH1F *) hc->FindObject("hFinPlaneS");
	hFinPlane->Draw();
	hFinPlaneS->Draw("SAME");
	
	c1->cd(9);
	TH2F *hPosClIni = (TH2F *) hc->FindObject("hPosClIni");
	hPosClIni->Draw("COLZ");
	
	c1->cd(10);
	TH2F *hPosClEnd = (TH2F *) hc->FindObject("hPosClEnd");
	hPosClEnd->Draw("COLZ");
	
	c1->cd(11);
	TH2F *hPosClIniS = (TH2F *) hc->FindObject("hPosClIniS");
	hPosClIniS->Draw("COLZ");
	
	c1->cd(12);
	TH2F *hPosClEndS = (TH2F *) hc->FindObject("hPosClEndS");
	hPosClEndS->Draw("COLZ");
	
	c1->cd(13);
	TH2F *hPosDevS = (TH2F *) hc->FindObject("hPosDevS");
	hPosDevS->Draw("COLZ");
	
	c1->cd(14);
	TH1F *hPosDevX = (TH1F *) hc->FindObject("hPosDevX");
	hPosDevX->Draw();
	
	c1->cd(15);
	TH1F *hPosDevZ = (TH1F *) hc->FindObject("hPosDevZ");
	hPosDevZ->Draw();
	return;
}

void drawMcalHisto(TObjArray *hc)
{
	TCanvas *c2 = new TCanvas("MCAL_data", "MCAL data", 1200, 600);
	c2->Divide(6, 4);
	
	c2->cd(1);
	TH2F *hPosDevMCAL = (TH2F *) hc->FindObject("hPosDevMCAL");
	hPosDevMCAL->Draw("COLZ");
	
	c2->cd(2);
	TH1F *hMCALDevX = (TH1F *) hc->FindObject("hMCALDevX");
	hMCALDevX->Draw();
	
	c2->cd(3);
	TH1F *hMCALDevZ = (TH1F *) hc->FindObject("hMCALDevZ");
	hMCALDevZ->Draw();
	
	c2->cd(4);
	TH1F *hMCALEnergyX = (TH1F *) hc->FindObject("hMCALEnergyX");
	hMCALEnergyX->Draw();
	
	c2->cd(5);
	TH1F *hMCALEnergyZ = (TH1F *) hc->FindObject("hMCALEnergyZ");
	hMCALEnergyZ->Draw();
	
	// draw position deviation for different bars
	TH2F *hBarDevX = (TH2F *) hc->FindObject("hBarDevX");
	TH2F *hBarDevZ = (TH2F *) hc->FindObject("hBarDevZ");
	float evtPos[30];
	c2->cd(7);
	TLegend *leg1 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("X");
		pname += i+1;
		TH1D *aux = hBarDevX->ProjectionY(pname.Data(), i+1, i+1, "");
		evtPos[i] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("X deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg1->AddEntry(aux, pname.Data(), "lp");
	}
	leg1->Draw();
	
	c2->cd(8);
	TLegend *leg2 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("X");
		pname += i+6;
		TH1D *aux = hBarDevX->ProjectionY(pname.Data(), i+6, i+6, "");
		evtPos[i+5] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("X deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg2->AddEntry(aux, pname.Data(), "lp");
	}
	leg2->Draw();
	
	c2->cd(9);
	TLegend *leg3 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("X");
		pname += i+11;
		TH1D *aux = hBarDevX->ProjectionY(pname.Data(), i+11, i+11, "");
		evtPos[i+10] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("X deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg3->AddEntry(aux, pname.Data(), "lp");
	}
	leg3->Draw();
	
	c2->cd(10);
	TLegend *leg4 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("Z");
		pname += i+1;
		TH1D *aux = hBarDevZ->ProjectionY(pname.Data(), i+1, i+1, "");
		evtPos[i+15] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Z deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg4->AddEntry(aux, pname.Data(), "lp");
	}
	leg4->Draw();
	
	c2->cd(11);
	TLegend *leg5 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("Z");
		pname += i+6;
		TH1D *aux = hBarDevZ->ProjectionY(pname.Data(), i+6, i+6, "");
		evtPos[i+20] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Z deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg5->AddEntry(aux, pname.Data(), "lp");
	}
	leg5->Draw();
	
	c2->cd(12);
	TLegend *leg6 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("Z");
		pname += i+11;
		TH1D *aux = hBarDevZ->ProjectionY(pname.Data(), i+11, i+11, "");
		evtPos[i+25] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Z deviation (cm)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg6->AddEntry(aux, pname.Data(), "lp");
	}
	leg6->Draw();
	
	// draw energy distributions for different bars
	float evtE[30];
	TH2F *hBarEX = (TH2F *) hc->FindObject("hBarEX");
	TH2F *hBarEZ = (TH2F *) hc->FindObject("hBarEZ");
	c2->cd(13);
	TLegend *leg11 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EX");
		pname += i+1;
		TH1D *aux = hBarEX->ProjectionY(pname.Data(), i+1, i+1, "");
		evtE[i] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg11->AddEntry(aux, pname.Data(), "lp");
	}
	leg11->Draw();
	
	c2->cd(14);
	TLegend *leg12 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EX");
		pname += i+6;
		TH1D *aux = hBarEX->ProjectionY(pname.Data(), i+6, i+6, "");
		evtE[i+5] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg12->AddEntry(aux, pname.Data(), "lp");
	}
	leg12->Draw();
	
	c2->cd(15);
	TLegend *leg13 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EX");
		pname += i+11;
		TH1D *aux = hBarEX->ProjectionY(pname.Data(), i+11, i+11, "");
		evtE[i+10] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg13->AddEntry(aux, pname.Data(), "lp");
	}
	leg13->Draw();
	
	c2->cd(16);
	TLegend *leg14 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EZ");
		pname += i+1;
		TH1D *aux = hBarEZ->ProjectionY(pname.Data(), i+1, i+1, "");
		evtE[i+15] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg14->AddEntry(aux, pname.Data(), "lp");
	}
	leg14->Draw();
	
	c2->cd(17);
	TLegend *leg15 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EZ");
		pname += i+6;
		TH1D *aux = hBarEZ->ProjectionY(pname.Data(), i+6, i+6, "");
		evtE[i+20] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg15->AddEntry(aux, pname.Data(), "lp");
	}
	leg15->Draw();
	
	c2->cd(18);
	TLegend *leg16 = new TLegend(0.6, 0.6, 0.95, 0.95);
	for (int i=0; i<5; i++) {
		TString pname("EZ");
		pname += i+11;
		TH1D *aux = hBarEZ->ProjectionY(pname.Data(), i+11, i+11, "");
		evtE[i+25] = aux->GetMean();
		aux->SetLineColor(i+1);
		if (i==0) {
			aux->GetXaxis()->SetTitle("Energy (MeV)");
			aux->GetYaxis()->SetTitle("Counts");
			aux->Draw();
		}
		else aux->Draw("SAME");
		leg16->AddEntry(aux, pname.Data(), "lp");
	}
	leg16->Draw();
	
	// print out deviations:
	printf("bar \tPos dev. (cm) \tmean Energy (MeV)\n");
	for (int i=0; i<30; i++) {
		printf("%c%d \t%10.2f \t%10.2f\n", i<15 ? 'X' : 'Z', i<15 ? i+1 : i-14, evtPos[i], evtE[i]);
	}

}

void drawCalibrationHisto(TObjArray *hc, TObjArray *fcBonA)
{
	TCanvas *c3 = new TCanvas("calib_data", "MCAL calibration data", 800, 600);
	c3->Divide(5, 6);
	
	
	for (int i=0; i<30; i++) {
		c3->cd(i+1);
		TString gname("gDev_");
		gname += i;
		TGraph *gaux = (TGraph *) hc->FindObject(gname.Data());
		gaux->Draw("AP");
		((TF1 *) fcBonA->At(i))->Draw("SAME");
	}
	return;
}
