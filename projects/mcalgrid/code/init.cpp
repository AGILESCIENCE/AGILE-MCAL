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

// initialization routines
void initGridHisto(TObjArray *hc)
{
	TH1F *hScang, *hScangS, *hRX, *hRXS, *hRZ, *hRZS, *hTheta, *hThetaS, *hPhi, *hPhiS, *hNseg, *hNsegS;
	TH1F *hIniPlane, *hIniPlaneS, *hFinPlane, *hFinPlaneS, *hPosDevX, *hPosDevZ;
	TH2F *hPosClIni, *hPosClIniS, *hPosClEnd, *hPosClEndS, *hPosDevS;
	
	// build histograms
	hScang = new TH1F("hScang", "mean scattering angle", 3600, 0, 360);
	hScang->GetXaxis()->SetTitle("°");
	hScang->GetXaxis()->SetTitleSize(0.05);
	hScang->GetXaxis()->SetLabelSize(0.05);
	hScang->GetXaxis()->CenterTitle();
	hScang->GetYaxis()->SetTitle("Counts");
	hScang->GetYaxis()->SetTitleSize(0.05);
	hScang->GetYaxis()->SetTitleOffset(1.2);
	hScang->GetYaxis()->SetLabelSize(0.05);
	hScang->GetYaxis()->CenterTitle();
	hScangS = new TH1F("hScangS", "SELECTED mean scattering angle", 3600, 0, 360);
	hScangS->SetLineColor(2);
	hc->AddLast(hScang);
	hc->AddLast(hScangS);
	
	hRX = new TH1F("hRX", "#cluster / #planes for FTB X", 500, 0., 5.);
	hRX->GetXaxis()->SetTitle("RX");
	hRX->GetXaxis()->SetTitleSize(0.05);
	hRX->GetXaxis()->SetLabelSize(0.05);
	hRX->GetXaxis()->CenterTitle();
	hRX->GetYaxis()->SetTitle("Counts");
	hRX->GetYaxis()->SetTitleSize(0.05);
	hRX->GetYaxis()->SetTitleOffset(1.2);
	hRX->GetYaxis()->SetLabelSize(0.05);
	hRX->GetYaxis()->CenterTitle();
	hRXS = new TH1F("hRXS", "SELECTED #cluster / #planes for FTB X", 500, 0., 5.);
	hRXS->SetLineColor(2);
	hc->AddLast(hRX);
	hc->AddLast(hRXS);
	
	hRZ = new TH1F("hRZ", "#cluster / #planes for FTB Z", 500, 0., 5.);
	hRZ->GetXaxis()->SetTitle("RZ");
	hRZ->GetXaxis()->SetTitleSize(0.05);
	hRZ->GetXaxis()->SetLabelSize(0.05);
	hRZ->GetXaxis()->CenterTitle();
	hRZ->GetYaxis()->SetTitle("Counts");
	hRZ->GetYaxis()->SetTitleSize(0.05);
	hRZ->GetYaxis()->SetTitleOffset(1.2);
	hRZ->GetYaxis()->SetLabelSize(0.05);
	hRZ->GetYaxis()->CenterTitle();
	hRZS = new TH1F("hRZS", "SELECTED #cluster / #planes for FTB Z", 500, 0., 5.);
	hRZS->SetLineColor(2);
	hc->AddLast(hRZ);
	hc->AddLast(hRZS);
	
	hTheta = new TH1F("hTheta", "reconstructed Theta angle", 180, 0., 180.);
	hTheta->GetXaxis()->SetTitle("Theta (°)");
	hTheta->GetXaxis()->SetTitleSize(0.05);
	hTheta->GetXaxis()->SetLabelSize(0.05);
	hTheta->GetXaxis()->CenterTitle();
	hTheta->GetYaxis()->SetTitle("Counts");
	hTheta->GetYaxis()->SetTitleSize(0.05);
	hTheta->GetYaxis()->SetTitleOffset(1.2);
	hTheta->GetYaxis()->SetLabelSize(0.05);
	hTheta->GetYaxis()->CenterTitle();
	hThetaS = new TH1F("hThetaS", "SELECTED reconstructed Theta angle", 180, 0., 180.);
	hThetaS->SetLineColor(2);
	hc->AddLast(hTheta);
	hc->AddLast(hThetaS);
	
	hPhi = new TH1F("hPhi", "reconstructed Phi angle", 360, 0., 360.);
	hPhi->GetXaxis()->SetTitle("Phi (°)");
	hPhi->GetXaxis()->SetTitleSize(0.05);
	hPhi->GetXaxis()->SetLabelSize(0.05);
	hPhi->GetXaxis()->CenterTitle();
	hPhi->GetYaxis()->SetTitle("Counts");
	hPhi->GetYaxis()->SetTitleSize(0.05);
	hPhi->GetYaxis()->SetTitleOffset(1.2);
	hPhi->GetYaxis()->SetLabelSize(0.05);
	hPhi->GetYaxis()->CenterTitle();
	hPhiS = new TH1F("hPhiS", "SELECTED reconstructed Phi angle", 360, 0., 360.);
	hPhiS->SetLineColor(2);
	hc->AddLast(hPhi);
	hc->AddLast(hPhiS);
	
	hNseg = new TH1F("hNseg", "Number of segments", 12, 0, 12);
	hNseg->GetXaxis()->SetTitle("# segments");
	hNseg->GetXaxis()->SetTitleSize(0.05);
	hNseg->GetXaxis()->SetLabelSize(0.05);
	hNseg->GetXaxis()->CenterTitle();
	hNseg->GetYaxis()->SetTitle("Counts");
	hNseg->GetYaxis()->SetTitleSize(0.05);
	hNseg->GetYaxis()->SetTitleOffset(1.2);
	hNseg->GetYaxis()->SetLabelSize(0.05);
	hNseg->GetYaxis()->CenterTitle();
	hNsegS = new TH1F("hNsegS", "SELECTED Number of segments", 12, 0, 12);
	hNsegS->SetLineColor(2);
	hc->AddLast(hNseg);
	hc->AddLast(hNsegS);
	
	hIniPlane = new TH1F("hIniPlane", "First track plane", 12, 0, 12);
	hIniPlane->GetXaxis()->SetTitle("# plane");
	hIniPlane->GetXaxis()->SetTitleSize(0.05);
	hIniPlane->GetXaxis()->SetLabelSize(0.05);
	hIniPlane->GetXaxis()->CenterTitle();
	hIniPlane->GetYaxis()->SetTitle("Counts");
	hIniPlane->GetYaxis()->SetTitleSize(0.05);
	hIniPlane->GetYaxis()->SetTitleOffset(1.2);
	hIniPlane->GetYaxis()->SetLabelSize(0.05);
	hIniPlane->GetYaxis()->CenterTitle();
	hIniPlaneS = new TH1F("hIniPlaneS", "SELECTED First track plane", 12, 0, 12);
	hIniPlaneS->SetLineColor(2);
	hc->AddLast(hIniPlane);
	hc->AddLast(hIniPlaneS);
	
	hFinPlane = new TH1F("hFinPlane", "Last track plane", 12, 0, 12);
	hFinPlane->GetXaxis()->SetTitle("# plane");
	hFinPlane->GetXaxis()->SetTitleSize(0.05);
	hFinPlane->GetXaxis()->SetLabelSize(0.05);
	hFinPlane->GetXaxis()->CenterTitle();
	hFinPlane->GetYaxis()->SetTitle("Counts");
	hFinPlane->GetYaxis()->SetTitleSize(0.05);
	hFinPlane->GetYaxis()->SetTitleOffset(1.2);
	hFinPlane->GetYaxis()->SetLabelSize(0.05);
	hFinPlane->GetYaxis()->CenterTitle();
	hFinPlaneS = new TH1F("hFinPlaneS", "SELECTED Last track plane", 12, 0, 12);
	hFinPlaneS->SetLineColor(2);
	hc->AddLast(hFinPlane);
	hc->AddLast(hFinPlaneS);
	
	hPosClIni = new TH2F("hPosClIni", "First Cluster position (X,Z)", 50, -50., 0., 50, -50., 0.);
	hPosClIni->GetXaxis()->SetTitle("X (cm)");
	hPosClIni->GetXaxis()->SetLabelSize(0.05);
	hPosClIni->GetXaxis()->SetTitleSize(0.05);
	hPosClIni->GetXaxis()->CenterTitle();
	hPosClIni->GetYaxis()->SetTitle("Z (cm)");
	hPosClIni->GetYaxis()->SetLabelSize(0.05);
	hPosClIni->GetYaxis()->SetTitleSize(0.05);
	hPosClIni->GetYaxis()->SetTitleOffset(1.4);
	hPosClIni->GetYaxis()->CenterTitle();
	hPosClIni->GetZaxis()->SetTitle("Counts");
	hPosClIni->GetZaxis()->CenterTitle();
	hPosClIniS = new TH2F("hPosClIniS", "SELECTED First Cluster position (X,Z)", 100, -50., 0., 100, -50., 0.);
	hPosClIniS->GetXaxis()->SetTitle("X (cm)");
	hPosClIniS->GetXaxis()->SetLabelSize(0.05);
	hPosClIniS->GetXaxis()->SetTitleSize(0.05);
	hPosClIniS->GetXaxis()->CenterTitle();
	hPosClIniS->GetYaxis()->SetTitle("Z (cm)");
	hPosClIniS->GetYaxis()->SetLabelSize(0.05);
	hPosClIniS->GetYaxis()->SetTitleSize(0.05);
	hPosClIniS->GetYaxis()->SetTitleOffset(1.4);
	hPosClIniS->GetYaxis()->CenterTitle();
	hPosClIniS->GetZaxis()->SetTitle("Counts");
	hPosClIniS->GetZaxis()->CenterTitle();
	hc->AddLast(hPosClIni);
	hc->AddLast(hPosClIniS);

	hPosClEnd = new TH2F("hPosClEnd", "Last Cluster position (Y,Z)", 50, -50., 0., 50, -50., 0.);
	hPosClEnd->GetXaxis()->SetTitle("X (cm)");
	hPosClEnd->GetXaxis()->SetLabelSize(0.05);
	hPosClEnd->GetXaxis()->SetTitleSize(0.05);
	hPosClEnd->GetXaxis()->CenterTitle();
	hPosClEnd->GetYaxis()->SetTitle("Z (cm)");
	hPosClEnd->GetYaxis()->SetLabelSize(0.05);
	hPosClEnd->GetYaxis()->SetTitleSize(0.05);
	hPosClEnd->GetYaxis()->SetTitleOffset(1.4);
	hPosClEnd->GetYaxis()->CenterTitle();
	hPosClEnd->GetZaxis()->SetTitle("Counts");
	hPosClEnd->GetZaxis()->CenterTitle();
	hPosClEndS = new TH2F("hPosClEndS", "SELECTED Last Cluster position (Y,Z)", 100, -50., 0., 100, -50., 0.);
	hPosClEndS->GetXaxis()->SetTitle("X (cm)");
	hPosClEndS->GetXaxis()->SetLabelSize(0.05);
	hPosClEndS->GetXaxis()->SetTitleSize(0.05);
	hPosClEndS->GetXaxis()->CenterTitle();
	hPosClEndS->GetYaxis()->SetTitle("Z (cm)");
	hPosClEndS->GetYaxis()->SetLabelSize(0.05);
	hPosClEndS->GetYaxis()->SetTitleSize(0.05);
	hPosClEndS->GetYaxis()->SetTitleOffset(1.4);
	hPosClEndS->GetYaxis()->CenterTitle();
	hPosClEndS->GetZaxis()->SetTitle("Counts");
	hPosClEndS->GetZaxis()->CenterTitle();
	hc->AddLast(hPosClEnd);
	hc->AddLast(hPosClEndS);
	
	hPosDevS = new TH2F("hPosDevS", "SELECTED Deviation calculation - data position (X,Z)", 200, -10., 10., 200, -10., 10.);
	hPosDevS->GetXaxis()->SetTitle("X (cm)");
	hPosDevS->GetXaxis()->SetLabelSize(0.05);
	hPosDevS->GetXaxis()->SetTitleSize(0.05);
	hPosDevS->GetXaxis()->CenterTitle();
	hPosDevS->GetYaxis()->SetTitle("Z (cm)");
	hPosDevS->GetYaxis()->SetLabelSize(0.05);
	hPosDevS->GetYaxis()->SetTitleSize(0.05);
	hPosDevS->GetYaxis()->SetTitleOffset(1.4);
	hPosDevS->GetYaxis()->CenterTitle();
	hPosDevS->GetZaxis()->SetTitle("Counts");
	hPosDevS->GetZaxis()->CenterTitle();
	hc->AddLast(hPosDevS);
	
	hPosDevX = new TH1F("hPosDevX", "Deviation calculation - data position: X projection", 200, -10., 10.);
	hPosDevX->GetXaxis()->SetTitle("X deviation (cm)");
	hPosDevX->GetXaxis()->SetTitleSize(0.05);
	hPosDevX->GetXaxis()->SetLabelSize(0.05);
	hPosDevX->GetXaxis()->CenterTitle();
	hPosDevX->GetYaxis()->SetTitle("Counts");
	hPosDevX->GetYaxis()->SetTitleSize(0.05);
	hPosDevX->GetYaxis()->SetTitleOffset(1.2);
	hPosDevX->GetYaxis()->SetLabelSize(0.05);
	hPosDevX->GetYaxis()->CenterTitle();
	hc->AddLast(hPosDevX);
	
	hPosDevZ = new TH1F("hPosDevZ", "Deviation calculation - data position: Z projection", 200, -10., 10.);
	hPosDevZ->GetXaxis()->SetTitle("Z deviation (cm)");
	hPosDevZ->GetXaxis()->SetTitleSize(0.05);
	hPosDevZ->GetXaxis()->SetLabelSize(0.05);
	hPosDevZ->GetXaxis()->CenterTitle();
	hPosDevZ->GetYaxis()->SetTitle("Counts");
	hPosDevZ->GetYaxis()->SetTitleSize(0.05);
	hPosDevZ->GetYaxis()->SetTitleOffset(1.2);
	hPosDevZ->GetYaxis()->SetLabelSize(0.05);
	hPosDevZ->GetYaxis()->CenterTitle();
	hc->AddLast(hPosDevZ);
	return;
}

void initMcalHisto(TObjArray *hc)
{
	TH2F *hPosDevMCAL = new TH2F("hPosDevMCAL", "SELECTED Deviation MCAL - calculated data position (X,Z)", 50, -10., 10., 50, -10., 10.);
	hPosDevMCAL->GetXaxis()->SetTitle("X (cm)");
	hPosDevMCAL->GetXaxis()->SetLabelSize(0.05);
	hPosDevMCAL->GetXaxis()->SetTitleSize(0.05);
	hPosDevMCAL->GetXaxis()->CenterTitle();
	hPosDevMCAL->GetYaxis()->SetTitle("Z (cm)");
	hPosDevMCAL->GetYaxis()->SetLabelSize(0.05);
	hPosDevMCAL->GetYaxis()->SetTitleSize(0.05);
	hPosDevMCAL->GetYaxis()->SetTitleOffset(1.4);
	hPosDevMCAL->GetYaxis()->CenterTitle();
	hPosDevMCAL->GetZaxis()->SetTitle("Counts");
	hPosDevMCAL->GetZaxis()->CenterTitle();
	hc->AddLast(hPosDevMCAL);
	
	TH1F *hMCALDevX = new TH1F("hMCALDevX", "Deviation from projected position X", 200, -10., 10.);
	hMCALDevX->GetXaxis()->SetTitle("X deviation (cm)");
	hMCALDevX->GetXaxis()->SetTitleSize(0.05);
	hMCALDevX->GetXaxis()->SetLabelSize(0.05);
	hMCALDevX->GetXaxis()->CenterTitle();
	hMCALDevX->GetYaxis()->SetTitle("Counts");
	hMCALDevX->GetYaxis()->SetTitleSize(0.05);
	hMCALDevX->GetYaxis()->SetTitleOffset(1.2);
	hMCALDevX->GetYaxis()->SetLabelSize(0.05);
	hMCALDevX->GetYaxis()->CenterTitle();
	hc->AddLast(hMCALDevX);
	
	TH1F *hMCALDevZ = new TH1F("hMCALDevZ", "Deviation from projected position Z", 200, -10., 10.);
	hMCALDevZ->GetXaxis()->SetTitle("Z deviation (cm)");
	hMCALDevZ->GetXaxis()->SetTitleSize(0.05);
	hMCALDevZ->GetXaxis()->SetLabelSize(0.05);
	hMCALDevZ->GetXaxis()->CenterTitle();
	hMCALDevZ->GetYaxis()->SetTitle("Counts");
	hMCALDevZ->GetYaxis()->SetTitleSize(0.05);
	hMCALDevZ->GetYaxis()->SetTitleOffset(1.2);
	hMCALDevZ->GetYaxis()->SetLabelSize(0.05);
	hMCALDevZ->GetYaxis()->CenterTitle();
	hc->AddLast(hMCALDevZ);
	
	TH1F *hMCALEnergyX = new TH1F("hMCALEnergyX", "Energy deposit in MCAL bar X", 1000, 0., 100.);
	hMCALEnergyX->GetXaxis()->SetTitle("Energy (MeV)");
	hMCALEnergyX->GetXaxis()->SetTitleSize(0.05);
	hMCALEnergyX->GetXaxis()->SetLabelSize(0.05);
	hMCALEnergyX->GetXaxis()->CenterTitle();
	hMCALEnergyX->GetYaxis()->SetTitle("Counts");
	hMCALEnergyX->GetYaxis()->SetTitleSize(0.05);
	hMCALEnergyX->GetYaxis()->SetTitleOffset(1.2);
	hMCALEnergyX->GetYaxis()->SetLabelSize(0.05);
	hMCALEnergyX->GetYaxis()->CenterTitle();
	hc->AddLast(hMCALEnergyX);
	
	TH1F *hMCALEnergyZ = new TH1F("hMCALEnergyZ", "Energy deposit in MCAL bar Z", 1000, 0., 100.);
	hMCALEnergyZ->GetXaxis()->SetTitle("Energy (MeV)");
	hMCALEnergyZ->GetXaxis()->SetTitleSize(0.05);
	hMCALEnergyZ->GetXaxis()->SetLabelSize(0.05);
	hMCALEnergyZ->GetXaxis()->CenterTitle();
	hMCALEnergyZ->GetYaxis()->SetTitle("Counts");
	hMCALEnergyZ->GetYaxis()->SetTitleSize(0.05);
	hMCALEnergyZ->GetYaxis()->SetTitleOffset(1.2);
	hMCALEnergyZ->GetYaxis()->SetLabelSize(0.05);
	hMCALEnergyZ->GetYaxis()->CenterTitle();
	hc->AddLast(hMCALEnergyZ);
	
	TH2F *hBarDevX = new TH2F("hBarDevX", "Deviation from projected position X - for each bar", 15, 0, 15, 100, -10., 10.);
	hBarDevX->GetXaxis()->SetTitle("X deviation (cm)");
	hBarDevX->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hBarDevX);
	
	TH2F *hBarDevZ = new TH2F("hBarDevZ", "Deviation from projected position Z - for each bar", 15, 0, 15, 100, -10., 10.);
	hBarDevZ->GetZaxis()->SetTitle("Z deviation (cm)");
	hBarDevZ->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hBarDevZ);
	
	TH2F *hBarEX = new TH2F("hBarEX", "X plane Energy distribution - for each bar", 15, 0, 15, 100, 0., 20.);
	hBarEX->GetXaxis()->SetTitle("X Energy (MeV)");
	hBarEX->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hBarEX);
	
	TH2F *hBarEZ = new TH2F("hBarEZ", "Z plane Energy distribution - for each bar", 15, 0, 15, 100, 0., 20.);
	hBarEZ->GetXaxis()->SetTitle("Z Energy (MeV)");
	hBarEZ->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hBarEZ);

	return;
}

void initCalibrationHisto(TObjArray *hc)
{
	// init graphics for bar calibration verification
	
	for (int i=0; i<30; i++) {
		TString gname("gDev_");
		TString gtitle("");
		if (i<15) {
			gtitle += "bar X";
			gtitle += i+1;
		} else {
			gtitle += "bar Z";
			gtitle += i-14;
		}
		gname += i;
		TGraph *gaux = new TGraph(200000);
		gaux->SetName(gname.Data());
		gaux->SetTitle(gtitle.Data());
		hc->AddLast(gaux);
	}
	return;
}

