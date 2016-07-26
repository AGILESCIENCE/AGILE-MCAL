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

#include "mcalimaging.h"
#include <fitsio.h>
void initBasicHisto(TObjArray *hc, float *mcalEbin)
{
	TH1F *hNBarsX, *hNBarsZ, *hNTracksX, *hNTracksZ, *hERatio, *hERatioSel, *hEX, *hEZ, *hEtot;
	TH2F *hTracksCorr, *hEXEZ, *hEREtot, *hEtotEtag, *hEratioEtag, *hEdev, *hBarCorr;
   	


	hNBarsX = new TH1F("hNBarsX", "number of bars selected on plane X", 15, 0, 15);
	hNBarsX->GetXaxis()->SetTitle("Number of bars");
	hNBarsX->GetXaxis()->SetTitleSize(0.05);
	hNBarsX->GetXaxis()->SetLabelSize(0.05);
	hNBarsX->GetXaxis()->CenterTitle();
	hNBarsX->GetYaxis()->SetTitle("Counts");
	hNBarsX->GetYaxis()->SetTitleSize(0.05);
	hNBarsX->GetYaxis()->SetTitleOffset(1.2);
	hNBarsX->GetYaxis()->SetLabelSize(0.05);
	hNBarsX->GetYaxis()->CenterTitle();
	hc->AddLast(hNBarsX);
	
	hNBarsZ = new TH1F("hNBarsZ", "number of bars selected on plane Z", 15, 0, 15);
	hNBarsZ->GetXaxis()->SetTitle("Number of bars");
	hNBarsZ->GetXaxis()->SetTitleSize(0.05);
	hNBarsZ->GetXaxis()->SetLabelSize(0.05);
	hNBarsZ->GetXaxis()->CenterTitle();
	hNBarsZ->GetYaxis()->SetTitle("Counts");
	hNBarsZ->GetYaxis()->SetTitleSize(0.05);
	hNBarsZ->GetYaxis()->SetTitleOffset(1.2);
	hNBarsZ->GetYaxis()->SetLabelSize(0.05);
	hNBarsZ->GetYaxis()->CenterTitle();
	hc->AddLast(hNBarsZ);
	
	hNTracksX = new TH1F("hNTracksX", "number of tracks on plane X", 15, 0, 15);
	hNTracksX->GetXaxis()->SetTitle("Number of tracks");
	hNTracksX->GetXaxis()->SetTitleSize(0.05);
	hNTracksX->GetXaxis()->SetLabelSize(0.05);
	hNTracksX->GetXaxis()->CenterTitle();
	hNTracksX->GetYaxis()->SetTitle("Counts");
	hNTracksX->GetYaxis()->SetTitleSize(0.05);
	hNTracksX->GetYaxis()->SetTitleOffset(1.2);
	hNTracksX->GetYaxis()->SetLabelSize(0.05);
	hNTracksX->GetYaxis()->CenterTitle();
	hc->AddLast(hNTracksX);
	
	hNTracksZ = new TH1F("hNTracksZ", "number of tracks on plane Z", 15, 0, 15);
	hNTracksZ->GetXaxis()->SetTitle("Number of tracks");
	hNTracksZ->GetXaxis()->SetTitleSize(0.05);
	hNTracksZ->GetXaxis()->SetLabelSize(0.05);
	hNTracksZ->GetXaxis()->CenterTitle();
	hNTracksZ->GetYaxis()->SetTitle("Counts");
	hNTracksZ->GetYaxis()->SetTitleSize(0.05);
	hNTracksZ->GetYaxis()->SetTitleOffset(1.2);
	hNTracksZ->GetYaxis()->SetLabelSize(0.05);
	hNTracksZ->GetYaxis()->CenterTitle();
	hc->AddLast(hNTracksZ);

	hTracksCorr = new TH2F("hTracksCorr", "X and Z number of tracks correlation", 8, 0, 8, 8, 0, 8);
	hTracksCorr->GetXaxis()->SetTitle("Number of tracks on plane X");
	hTracksCorr->GetYaxis()->SetTitle("Number of tracks on plane Z");
	hc->AddLast(hTracksCorr);

	hBarCorr = new TH2F("hBarCorr", "X and Z number of bars correlation", 16, 0, 15, 16, 0, 15);
	hBarCorr->GetXaxis()->SetTitle("Number of fired bars on plane X");
	hBarCorr->GetYaxis()->SetTitle("Number of fired bars on plane Z");
	hc->AddLast(hBarCorr);

	hERatio = new TH1F("hERatio", "EZ / EX", 1000, 0., 10.);
	hERatio->GetXaxis()->SetTitle("EZ/EX");
	hERatio->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hERatio);

	hEX = new TH1F("hEX", "EX", 50, 0., 500.);
	hEX->GetXaxis()->SetTitle("EX (MeV)");
	hEX->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hEX);

	hEZ = new TH1F("hEZ", "EZ", 50, 0., 500.);
	hEZ->GetXaxis()->SetTitle("EZ (MeV)");
	hEZ->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hEZ);

	hEtot = new TH1F("hEtot", "hEtot", 50, 0., 500.);
	hEtot->GetXaxis()->SetTitle("Etot (MeV)");
	hEtot->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hEtot);

	hERatioSel = new TH1F("hERatioSel", "EZ / EX", 1000, 0., 10.);
	hc->AddLast(hERatioSel);
	
	hEXEZ = new TH2F("hEXEZ", "EZ vs EX", NEBINX, mcalEbin, NEBINZ, mcalEbin);
	hEXEZ->GetXaxis()->SetTitle("EX (MeV)");
	hEXEZ->GetYaxis()->SetTitle("EZ (MeV)");
	hc->AddLast(hEXEZ);
	
	hEREtot = new TH2F("hEREtot", "EZ/EX vs Etot", 200, 0., 200., 200, 0., 5.);
	hEREtot->GetXaxis()->SetTitle("Etot (MeV)");
	hEREtot->GetYaxis()->SetTitle("EZ/EX");
	hc->AddLast(hEREtot);

	return;
}

//
//--------------------------------------------------------------------
//

void initTrigTrackHisto(TObjArray *hc, Double_t *Estep, int nEtag)
{
	TH1F *hETagged;
	TH2F *hEtotEtag, *hEratioEtag;
	TH2F *hTracks1, *hTracks2, *hTracks3, *hTracks4;
	TH2F *hBars1, *hBars2, *hBars3, *hBars4;

	hETagged = new TH1F("hETagged", "Tagged photon Energy", nEtag-1, Estep);
	hETagged->GetXaxis()->SetTitle("Energy (MeV)");
	hETagged->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hETagged);
	
	hEtotEtag = new TH2F("hEtotEtag", "Etot vs Tagged photon Energy", nEtag-1, Estep, 50, 0., 500.);
	hc->AddLast(hEtotEtag);
	
	hEratioEtag = new TH2F("hEratioEtag", "EX/EZ vs Tagged photon Energy", nEtag-1, Estep, 50, 0., 10.);
	hc->AddLast(hEratioEtag);

	hTracks1 = new TH2F("hTracks1", "X and Z number of tracks - energy range PTS 0-50 MeV", 8, 0, 8, 8, 0, 8);
	hTracks1->GetXaxis()->SetTitle("Number of tracks on plane X");
	hTracks1->GetYaxis()->SetTitle("Number of tracks on plane Z");
	hc->AddLast(hTracks1);

	hTracks2 = new TH2F("hTracks2", "X and Z number of tracks - energy range PTS 50-100 MeV", 8, 0, 8, 8, 0, 8);
	hTracks2->GetXaxis()->SetTitle("Number of tracks on plane X");
	hTracks2->GetYaxis()->SetTitle("Number of tracks on plane Z");
	hc->AddLast(hTracks2);

	hTracks3 = new TH2F("hTracks3", "X and Z number of tracks - energy range PTS 100-200 MeV", 8, 0, 8, 8, 0, 8);
	hTracks3->GetXaxis()->SetTitle("Number of tracks on plane X");
	hTracks3->GetYaxis()->SetTitle("Number of tracks on plane Z");
	hc->AddLast(hTracks3);

	hTracks4 = new TH2F("hTracks4", "X and Z number of tracks - energy range PTS 200-500 MeV", 8, 0, 8, 8, 0, 8);
	hTracks4->GetXaxis()->SetTitle("Number of tracks on plane X");
	hTracks4->GetYaxis()->SetTitle("Number of tracks on plane Z");
	hc->AddLast(hTracks4);

	hBars1 = new TH2F("hBars1", "X and Z number of bars for tracks (1,1) - energy range PTS 0-50 MeV", 8, 0, 8, 8, 0, 8);
	hBars1->GetXaxis()->SetTitle("Number of bars on plane X");
	hBars1->GetYaxis()->SetTitle("Number of bars on plane Z");
	hc->AddLast(hBars1);

	hBars2 = new TH2F("hBars2", "X and Z number of bars for tracks (1,1) - energy range PTS 50-100 MeV", 8, 0, 8, 8, 0, 8);
	hBars2->GetXaxis()->SetTitle("Number of bars on plane X");
	hBars2->GetYaxis()->SetTitle("Number of bars on plane Z");
	hc->AddLast(hBars2);

	hBars3 = new TH2F("hBars3", "X and Z number of bars for tracks (1,1) - energy range PTS 100-200 MeV", 8, 0, 8, 8, 0, 8);
	hBars3->GetXaxis()->SetTitle("Number of bars on plane X");
	hBars3->GetYaxis()->SetTitle("Number of bars on plane Z");
	hc->AddLast(hBars3);

	hBars4 = new TH2F("hBars4", "X and Z number of bars for tracks (1,1) - energy range PTS 200-500 MeV", 8, 0, 8, 8, 0, 8);
	hBars4->GetXaxis()->SetTitle("Number of bars on plane X");
	hBars4->GetYaxis()->SetTitle("Number of bars on plane Z");
	hc->AddLast(hBars4);
	
	return;
}

//
//--------------------------------------------------------------------
//

void initKalmanHisto(TObjArray *hc, Double_t *Estep, int nEtag)
{
	// creates histograms for Kalman correlation
	TH2F *hEdev, *hEtotEtag_sel, *hEratioEtag_sel,*hPosDevP1X, *hPosDevP2X, *hEK, *hEKcorr;
	TH1F *hZDevP1X, *hZDevP2X;
	TH2F *hXDevP1X, *hXDevP2X;
	TGraph *gEMEK;
	
	hEdev = new TH2F("hEdev", "E_K/E_PTS -1", nEtag-1, Estep, 200, -1., 19.);
	hEdev->GetXaxis()->SetTitle("E_K/E_PTS -1 ");
	hEdev->GetYaxis()->SetTitle("Counts");
	hc->AddLast(hEdev);
	
	hEK = new TH2F("hEK", "E_K vs E_tag", nEtag-1, Estep, nEtag-1, Estep);
	hEK->GetXaxis()->SetTitle("E_tag ");
	hEK->GetYaxis()->SetTitle("E_K");
	hc->AddLast(hEK);
	
	hEKcorr = new TH2F("hEKcorr", "E_K CORRECTED vs E_tag", nEtag-1, Estep, nEtag-1, Estep);
	hEKcorr->GetXaxis()->SetTitle("E_tag (MeV)");
	hEKcorr->GetYaxis()->SetTitle("E_K (MeV)");
	hc->AddLast(hEKcorr);
	
	gEMEK = new TGraph(200000);
	gEMEK->SetName("gEMEK");
	gEMEK->SetTitle("E_MCAL vs E_Kalman");
	hc->AddLast(gEMEK);

	hEtotEtag_sel   = new TH2F("hEtotEtag_sel", "Etot vs Tagged photon Energy SELECTED", nEtag-1, Estep, 50, 0., 500.);
	hc->AddLast(hEtotEtag_sel);
	
	hEratioEtag_sel = new TH2F("hEratioEtag_sel", "EX/EZ vs Tagged photon Energy SELECTED", nEtag-1, Estep, 50, 0., 10.);
	hc->AddLast(hEratioEtag_sel);

	hPosDevP1X = new TH2F("hPosDevP1X", "Kalman P1 deviation from MCAL X best point", 50, -20., 20., 50, -20., 20.);
	hPosDevP1X->GetXaxis()->SetTitle("X (cm)");
	hPosDevP1X->GetXaxis()->SetLabelSize(0.05);
	hPosDevP1X->GetXaxis()->SetTitleSize(0.05);
	hPosDevP1X->GetXaxis()->CenterTitle();
	hPosDevP1X->GetYaxis()->SetTitle("Z (cm)");
	hPosDevP1X->GetYaxis()->SetLabelSize(0.05);
	hPosDevP1X->GetYaxis()->SetTitleSize(0.05);
	hPosDevP1X->GetYaxis()->SetTitleOffset(1.4);
	hPosDevP1X->GetYaxis()->CenterTitle();
	hPosDevP1X->GetZaxis()->SetTitle("Counts");
	hPosDevP1X->GetZaxis()->CenterTitle();
	hc->AddLast(hPosDevP1X);
	
	hXDevP1X = new TH2F("hXDevP1X", "Kalman P1 deviation from MCAL X best point: X projection", nEtag-1, Estep, 100, -20., 20.);
	hc->AddLast(hXDevP1X);
	
	hZDevP1X = new TH1F("hZDevP1X", "Kalman P1 deviation from MCAL X best point: Z projection", 100, -20., 20.);
	hZDevP1X->GetXaxis()->SetTitle("Z deviation (cm)");
	hZDevP1X->GetXaxis()->SetTitleSize(0.05);
	hZDevP1X->GetXaxis()->SetLabelSize(0.05);
	hZDevP1X->GetXaxis()->CenterTitle();
	hZDevP1X->GetYaxis()->SetTitle("Counts");
	hZDevP1X->GetYaxis()->SetTitleSize(0.05);
	hZDevP1X->GetYaxis()->SetTitleOffset(1.2);
	hZDevP1X->GetYaxis()->SetLabelSize(0.05);
	hZDevP1X->GetYaxis()->CenterTitle();
	hc->AddLast(hZDevP1X);

	hPosDevP2X = new TH2F("hPosDevP2X", "Kalman P2 deviation from MCAL X best point", 50, -20., 20., 50, -20., 20.);
	hPosDevP2X->GetXaxis()->SetTitle("X (cm)");
	hPosDevP2X->GetXaxis()->SetLabelSize(0.05);
	hPosDevP2X->GetXaxis()->SetTitleSize(0.05);
	hPosDevP2X->GetXaxis()->CenterTitle();
	hPosDevP2X->GetYaxis()->SetTitle("Z (cm)");
	hPosDevP2X->GetYaxis()->SetLabelSize(0.05);
	hPosDevP2X->GetYaxis()->SetTitleSize(0.05);
	hPosDevP2X->GetYaxis()->SetTitleOffset(1.4);
	hPosDevP2X->GetYaxis()->CenterTitle();
	hPosDevP2X->GetZaxis()->SetTitle("Counts");
	hPosDevP2X->GetZaxis()->CenterTitle();
	hc->AddLast(hPosDevP2X);
	
	hXDevP2X = new TH2F("hXDevP2X", "Kalman P2 deviation from MCAL X best point: X projection", nEtag-1, Estep, 100, -20., 20.);
	hc->AddLast(hXDevP2X);
	
	hZDevP2X = new TH1F("hZDevP2X", "Kalman P2 deviation from MCAL X best point: Z projection", 100, -20., 20.);
	hZDevP2X->GetXaxis()->SetTitle("Z deviation (cm)");
	hZDevP2X->GetXaxis()->SetTitleSize(0.05);
	hZDevP2X->GetXaxis()->SetLabelSize(0.05);
	hZDevP2X->GetXaxis()->CenterTitle();
	hZDevP2X->GetYaxis()->SetTitle("Counts");
	hZDevP2X->GetYaxis()->SetTitleSize(0.05);
	hZDevP2X->GetYaxis()->SetTitleOffset(1.2);
	hZDevP2X->GetYaxis()->SetLabelSize(0.05);
	hZDevP2X->GetYaxis()->CenterTitle();
	hc->AddLast(hZDevP2X);

	TH2F *hXDevP1X_bar = new TH2F("hXDevP1X_bar", "Kalman P1 dev. from MCAL X best point: X projection X BAR", 15, 0., 15. , 100, -20., 20.);
	hc->AddLast(hXDevP1X_bar);
	
	
	return;
}

//
//--------------------------------------------------------------------
//

void initResponseHisto(TObjArray *hc, Double_t *Estep, int nEtag)
{
	// creates histograms for MCAL response correlation
	
	TH2F *hEMEK, *hEMEtag, *hEMavgEtag;
	
	hEMEK = new TH2F("hEMEK", "MCAL guess vs K energy", nEtag, Estep, nEtag, Estep);
	hEMEK->GetXaxis()->SetTitle("Kalman energy (MeV)");
	hEMEK->GetYaxis()->SetTitle("MCAL guess energy (MeV)");
	hc->AddLast(hEMEK);
	
	hEMEtag = new TH2F("hEMEtag", "MCAL guess vs E tag", nEtag, Estep, nEtag, Estep);
	hEMEtag->GetXaxis()->SetTitle("Tagged photon energy (MeV)");
	hEMEtag->GetYaxis()->SetTitle("MCAL guess energy (MeV)");
	hc->AddLast(hEMEtag);
	
	hEMavgEtag = new TH2F("hEMavgEtag", "photon estimated weighted energy vs E tag", nEtag, Estep, nEtag, Estep);
	hEMavgEtag->GetXaxis()->SetTitle("Tagged photon energy (MeV)");
	hEMavgEtag->GetYaxis()->SetTitle("estimated weighted energy (MeV)");
	hc->AddLast(hEMavgEtag);

}

//
//--------------------------------------------------------------------
//

void initMCALCorrectionHisto(TObjArray *hc, Double_t *Estep, int nEtag)
{
	TGraph 	*gaux;
	TH1F  	*hEMcorr;
	
	for (int i=0; i<nEtag; i++) {
		gaux = new TGraph(200000);
		TString gname("gEMEK_");
		gname += i;
		gaux->SetName(gname.Data());
		hc->AddLast(gaux);
	}
	
	hEMcorr = new TH1F("hEMcorr", "Events with EM > EK", nEtag-1, Estep);
	hEMcorr->GetXaxis()->SetTitle("Energy (MeV)");
	hEMcorr->GetXaxis()->SetTitleSize(0.05);
	hEMcorr->GetXaxis()->SetLabelSize(0.05);
	hEMcorr->GetXaxis()->CenterTitle();
	hEMcorr->GetYaxis()->SetTitle("Counts");
	hEMcorr->GetYaxis()->SetTitleSize(0.05);
	hEMcorr->GetYaxis()->SetTitleOffset(1.2);
	hEMcorr->GetYaxis()->SetLabelSize(0.05);
	hEMcorr->GetYaxis()->CenterTitle();
	hc->AddLast(hEMcorr);

	
	return;
}

//
//--------------------------------------------------------------------
//

int loadSelection(const char *calname, selection *s)
{
	TEnv *conf;	
	cout << "Loading configuration file " << calname << endl;
	conf = new TEnv(calname);
	if (!conf) {
		cout << "*** cannot find standard configuration file " << calname << endl;
		return 1;
	}
	
	s->isTrigTrack   = conf->GetValue(".is.trig.track.", 0.);
	s->doKalmanCorr  = conf->GetValue(".do.kalman.correlation.", 0.);
	
	s->nrun = conf->GetValue(".number.of.runs.", 1);
	s->rootfile   = new TString[s->nrun];
	s->lv1file    = new TString[s->nrun];
	s->kalmanfile = new TString[s->nrun];
	
	for (int i=0; i<s->nrun; i++) {
		TString rootname(".root.file.");
		rootname += i;
		(s->rootfile[i]).Append(conf->GetValue(rootname.Data(), "root.txt"));
		TString lv1name(".lv1.file.");
		lv1name += i;
		(s->lv1file[i]).Append(conf->GetValue(lv1name.Data(), "lv1.txt"));
		TString kalmanname(".kalman.file.");
		kalmanname += i;
		(s->kalmanfile[i]).Append(conf->GetValue(kalmanname.Data(), "kalman.txt"));
	}
	
	s->minTrX = conf->GetValue(".min.trk.X.", 0);
	s->maxTrX = conf->GetValue(".max.trk.X.", 15);
	s->minTrZ = conf->GetValue(".min.trk.Z.", 0);
	s->maxTrZ = conf->GetValue(".max.trk.Z.", 15);
	
	s->minBarX = conf->GetValue(".min.bar.X.", 0);
	s->maxBarX = conf->GetValue(".max.bar.X.", 15);
	s->minBarZ = conf->GetValue(".min.bar.Z.", 0);
	s->maxBarZ = conf->GetValue(".max.bar.Z.", 15);
	
	s->minE = conf->GetValue(".energy.threshold.", 0.);
	s->parname = new TString(conf->GetValue(".parameter.file.", "par.root"));

	s->buildResponseMatrix = conf->GetValue(".build.response.matrix.", 0);
	s->doMcalGuess = conf->GetValue(".do.mcal.analysis.", 0);
	return 0;
}

//
//--------------------------------------------------------------------
//

void printerror( int status)
{   // Print out cfitsio error messages and exit program
	if (status)  {
		fits_report_error(stderr, status);  // print error report
		exit( status );                     // terminate the program, returning error status
	}
	return;
}
