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
#include "mcalguess.h"

void drawBasicHisto(TObjArray *hc)
{
	TCanvas *c1, *c2;

	c1 = new TCanvas("MCAL_data", "basic histograms", 800, 600);
	c1->Divide(2, 2);
	
	c1->cd(1);	
	TH1F *hNBarsX = (TH1F *) hc->FindObject("hNBarsX");
	TH1F *hNBarsZ = (TH1F *) hc->FindObject("hNBarsZ");
	if (hNBarsX && hNBarsZ) {	
		hNBarsX->Draw();
		hNBarsZ->SetLineColor(2);
		hNBarsZ->Draw("SAME");
	} else cout << "*** cannot find object hNBarsX or hNBarsZ" << endl;
	
	c1->cd(2);
	TH1F *hNTracksX = (TH1F *) hc->FindObject("hNTracksX");
	TH1F *hNTracksZ = (TH1F *) hc->FindObject("hNTracksZ");
	if (hNTracksX) {	
		hNTracksX->Draw();
		hNTracksZ->SetLineColor(2);
		hNTracksZ->Draw("SAME");
	} else cout << "*** cannot find object hNTracksX or hNTracksZ" << endl;
	
	c1->cd(3);
	TH1F *hEX   = (TH1F *) hc->FindObject("hEX");
	TH1F *hEZ   = (TH1F *) hc->FindObject("hEZ");
	TH1F *hEtot = (TH1F *) hc->FindObject("hEtot");
	if (hEX && hEZ && hEtot) {	
		hEtot->Draw();
		hEX->SetLineColor(2);
		hEX->Draw("SAME");
		hEZ->SetLineColor(3);
		hEZ->Draw("SAME");
	} else cout << "*** cannot find object hEX or hEZ or hEtot" << endl;
		
	c2 = new TCanvas("MCAL_data_2", "basic histograms 2", 800, 600);
	c2->Divide(2, 2);
	
	c2->cd(1);
/*	TH2F *hTracksCorr = (TH2F *) hc->FindObject("hTracksCorr");
	if (hTracksCorr) {	
		hTracksCorr->Draw("COLZTEXT");
	} else cout << "*** cannot find object hTracksCorr" << endl;*/
	
	TH2F *hBarCorr = (TH2F *) hc->FindObject("hBarCorr");
	if (hBarCorr) {	
		float norm = 1./hBarCorr->GetEntries();
		hBarCorr->Scale(norm);		
		hBarCorr->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBarCorr" << endl;
	
	c2->cd(2);
	TH1F *hERatio = (TH1F *) hc->FindObject("hERatio");
	TH1F *hERatioSel = (TH1F *) hc->FindObject("hERatioSel");
	if (hERatio && hERatioSel) {	
		hERatio->Draw();
		hERatioSel->SetLineColor(2);
		hERatioSel->Draw("SAME");
	} else cout << "*** cannot find object hERatio or hERatioSel" << endl;
	
	c2->cd(3);
	TH2F *hEXEZ = (TH2F *) hc->FindObject("hEXEZ");
	if (hEXEZ) {	
		float norm = 1./hEXEZ->GetEntries();
		hEXEZ->Scale(norm);		
		hEXEZ->Draw("COLZTEXT");
	} else cout << "*** cannot find object hEXEZ" << endl;
	
	c2->cd(4);
	TH2F *hEREtot = (TH2F *) hc->FindObject("hEREtot");
	if (hEREtot) {	
		hEREtot->Draw("COLZ");
	} else cout << "*** cannot find object hEREtot" << endl;

	return;
}

//
//--------------------------------------------------------------------
//

void drawTrigTrackHisto(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Trig&Track data analysis
	
	TCanvas *c3;
	c3 = new TCanvas("MCAL_data_3", "Trig & Track", 800, 600);
	c3->Divide(2, 2);
	c3->cd(1);
	
	TH1F *hETagged = (TH1F *) hc->FindObject("hETagged");
	if (hETagged) {
		hETagged->Draw();
	} else cout << "*** cannot find object hETagged" << endl;
	
	c3->cd(2);
	TH2F *hEtotEtag = (TH2F *) hc->FindObject("hEtotEtag");
	TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
	if (hEtotEtag) {	
		for (int i=0; i<nEtag-1; i++) {
			TString pname("projection_");
			pname += i+1;
			TH1D *aux = hEtotEtag->ProjectionY(pname.Data(), i+1, i+1, "");
			//cout << "Slice number " << i+1 << "  -  # underflow entries = " <<  aux->GetBinContent(0) << "  -  # total entries = " <<  aux->Integral() << endl;
			aux->SetLineColor(i+1);
			float norm = 1./aux->Integral();
			//aux->Scale(norm);
			if (i==0) {
				aux->GetXaxis()->SetTitle("E_MCAL (MeV)");
				aux->GetYaxis()->SetTitle("Counts");
				aux->Draw();
			}
			else aux->Draw("SAME");
			TString erange("E_PTS = ");
			erange += Estep[i];
			erange += " - ";
			erange += Estep[i+1];
			erange += " MeV";
			leg->AddEntry(aux, erange.Data(), "lp");
		}
		leg->Draw();
	} else cout << "*** cannot find object hEtotEtag" << endl;
	
	c3->cd(3);
	TH2F *hEratioEtag = (TH2F *) hc->FindObject("hEratioEtag");
	if (hEratioEtag) {	
		for (int i=0; i<nEtag-1; i++) {
			TString pname("hEratioEtag_");
			pname += i+1;
			TH1D *aux = hEratioEtag->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = 1./aux->Integral();
			aux->Scale(norm);
			aux->SetLineColor(i+1);
			if (i==0) aux->Draw();
			else aux->Draw("SAME");
		}
		leg->Draw();
	} else cout << "*** cannot find object hEratioEtag" << endl;

	return;
}

//
//--------------------------------------------------------------------
//

void drawTrigTrackHisto2(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Trig&Track data analysis
	
	TCanvas *c5;
	c5 = new TCanvas("MCAL_data_5", "Tracks distribution for different energy range", 800, 600);
	c5->Divide(2, 2);
	c5->cd(1);
	
	c5->cd(1);
	TH2F *hTracks1 = (TH2F *) hc->FindObject("hTracks1");
	if (hTracks1) {	
		hTracks1->Draw("COLZTEXT");
	} else cout << "*** cannot find object hTracks1" << endl;
	
	c5->cd(2);
	TH2F *hTracks2 = (TH2F *) hc->FindObject("hTracks2");
	if (hTracks2) {	
		hTracks2->Draw("COLZTEXT");
	} else cout << "*** cannot find object hTracks2" << endl;
	
	c5->cd(3);
	TH2F *hTracks3 = (TH2F *) hc->FindObject("hTracks3");
	if (hTracks3) {	
		hTracks3->Draw("COLZTEXT");
	} else cout << "*** cannot find object hTracks3" << endl;
	
	c5->cd(4);
	TH2F *hTracks4 = (TH2F *) hc->FindObject("hTracks4");
	if (hTracks4) {	
		hTracks4->Draw("COLZTEXT");
	} else cout << "*** cannot find object hTracks4" << endl;

	return;
}

//
//--------------------------------------------------------------------
//

void drawTrigTrackHisto3(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Trig&Track data analysis
	
	TCanvas *c6;
	c6 = new TCanvas("MCAL_data_6", "Bars distribution for tracks (1,1) for different energy range", 800, 600);
	c6->Divide(2, 2);
	c6->cd(1);
	
	c6->cd(1);
	TH2F *hBars1 = (TH2F *) hc->FindObject("hBars1");
	if (hBars1) {	
		hBars1->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBars1" << endl;
	
	c6->cd(2);
	TH2F *hBars2 = (TH2F *) hc->FindObject("hBars2");
	if (hBars2) {	
		hBars2->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBars2" << endl;
	
	c6->cd(3);
	TH2F *hBars3 = (TH2F *) hc->FindObject("hBars3");
	if (hBars3) {	
		hBars3->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBars3" << endl;
	
	c6->cd(4);
	TH2F *hBars4 = (TH2F *) hc->FindObject("hBars4");
	if (hBars4) {	
		hBars4->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBars4" << endl;

	return;
}

//
//--------------------------------------------------------------------
//

void drawKalmanHisto(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Kalman data analysis
	
	TCanvas *c4;
	
	c4 = new TCanvas("MCAL_data_4", "TKalman", 800, 600);
	c4->Divide(2, 2);

	c4->cd(1);
	TGraph *gEMEK = (TGraph *) hc->FindObject("gEMEK");
	if (gEMEK) {
		gEMEK->GetXaxis()->SetTitle("Kalman Energy (MeV)");
		gEMEK->GetXaxis()->SetTitleOffset(1.4);
		gEMEK->GetXaxis()->CenterTitle();
		gEMEK->GetYaxis()->SetTitle("MCAL Energy");
		gEMEK->GetYaxis()->CenterTitle();
		gEMEK->GetYaxis()->SetTitleOffset(1.5);
		gEMEK->Draw("AP");
		TLine *l = new TLine(0., 0., 1000., 1000.);
		l->SetLineColor(2);
		l->Draw();
	} else cout << "*** cannot find object gEMEK" << endl;
	
	c4->cd(2);
	TH2F *hEdev = (TH2F *) hc->FindObject("hEdev");
	TLegend *leg = new TLegend(0.6, 0.6, 0.95, 0.95);
	if (hEdev) {	
		for (int i=0; i<nEtag-1; i++) {
			TString pname("hEKEtag_");
			pname += i+1;
			TH1D *aux = hEdev->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = aux->Integral();
			//aux->Scale(1./norm);
			aux->SetLineColor(i+1);
			if (i==0) {
				aux->GetXaxis()->SetTitle("E_K/E_PTS -1 ");
				aux->GetYaxis()->SetTitle("Counts");
				aux->Draw();
			} else aux->Draw("SAME");
			TString erange("E_PTS = ");
			erange += Estep[i];
			erange += " - ";
			erange += Estep[i+1];
			erange += " MeV";
			leg->AddEntry(aux, erange.Data(), "lp");
			
			if (norm>0.) {
				cout << pname.Data() << " : " << 100.*aux->Integral(1, 5)/norm << "% < 0.5  -  " << 100.*aux->Integral(15, 200)/norm << "% > 0.5 " << endl;
			}
		}
		leg->Draw();
	} else cout << "*** cannot find object hEdev" << endl;
	
	c4->cd(3);
	TH2F *hEK = (TH2F *) hc->FindObject("hEK");
	TH2F *hEKcorr = (TH2F *) hc->FindObject("hEKcorr");
	bool first=TRUE;
	float norm[10], normcorr[10];
	if (hEK && hEKcorr) {	
/*		printf("bin \tnEK \tnEKcor \tbin_0 \tbin_1 \tbin_2 \tbin_3 \tbin_4 \tbin_5 \tbin_6\n");
		for (int i=0; i<nEtag-1; i++) {
			TString pname("hEK_");
			pname += i+1;
			TH1D *aux = hEK->ProjectionY(pname.Data(), i+1, i+1, "");
			TString pnamecorr("hEKcorr_");
			pnamecorr += i+1;
			TH1D *auxcorr = hEKcorr->ProjectionY(pnamecorr.Data(), i+1, i+1, "");
			norm[i] = aux->Integral();
			normcorr[i] = auxcorr->Integral();
			printf("%d \t%.0f \t%.0f \t", i, norm[i], normcorr[i]);
			if (norm[i] == 0) {
				cout << endl;
				continue;
			}
			aux->Scale(1./norm[i]);
			aux->SetLineColor(i+1);
			if (first) {
				aux->GetXaxis()->SetTitle("E_K (MeV)");
				aux->GetYaxis()->SetTitle("Counts");
				aux->Draw();
				first = FALSE;
			} else aux->Draw("SAME");
			
			auxcorr->Scale(1./normcorr[i]);
			auxcorr->SetLineColor(i+1);
			auxcorr->SetLineStyle(2);
			auxcorr->Draw("SAME");
			
			for (int i=0; i<nEtag-1; i++) printf("%.4f\t", auxcorr->GetBinContent(i+1)-aux->GetBinContent(i+1));
			cout << endl;
		}
		leg->Draw();
*/

	// MM 26/1/07 modifica per leggibilita' grafico su correzione diagonale
			TLegend *leg2 = new TLegend(0.6, 0.6, 0.95, 0.95);
		
			// 50 MeV
			TH1D *aux2 = hEK->ProjectionY("hEK_2", 2, 2, "");
			TH1D *auxcorr2 = hEKcorr->ProjectionY("hEKcorr_2", 2, 2, "");
			aux2->Scale(1./aux2->Integral());
			aux2->SetLineColor(1);
			aux2->Draw();
			auxcorr2->Scale(1./auxcorr2->Integral());
			auxcorr2->SetLineColor(1);
			auxcorr2->SetLineStyle(2);
			auxcorr2->Draw("SAME");
			leg2->AddEntry(aux2, "E =   50 MeV", "lp");
			leg2->AddEntry(auxcorr2, "E =   50 MeV corrected", "lp");
			
			// 100 MeV
			TH1D *aux3 = hEK->ProjectionY("hEK_3", 3, 3, "");
			TH1D *auxcorr3 = hEKcorr->ProjectionY("hEKcorr_3", 3, 3, "");
			aux3->Scale(1./aux3->Integral());
			aux3->SetLineColor(2);
			aux3->Draw("SAME");
			auxcorr3->Scale(1./auxcorr3->Integral());
			auxcorr3->SetLineColor(2);
			auxcorr3->SetLineStyle(2);
			auxcorr3->Draw("SAME");
			leg2->AddEntry(aux3, "E =  100 MeV", "lp");
			leg2->AddEntry(auxcorr3, "E =  100 MeV corrected", "lp");
		
			// 400 MeV
			TH1D *aux5 = hEK->ProjectionY("hEK_5", 5, 5, "");
			TH1D *auxcorr5 = hEKcorr->ProjectionY("hEKcorr_5", 5, 5, "");
			aux5->Scale(1./aux5->Integral());
			aux5->SetLineColor(3);
			aux5->Draw("SAME");
			auxcorr5->Scale(1./auxcorr5->Integral());
			auxcorr5->SetLineColor(3);
			auxcorr5->SetLineStyle(2);
			auxcorr5->Draw("SAME");
			leg2->AddEntry(aux5, "E =  400 MeV", "lp");
			leg2->AddEntry(auxcorr5, "E =  400 MeV corrected", "lp");
		
			// 1 GeV
			TH1D *aux6 = hEK->ProjectionY("hEK_6", 6, 6, "");
			TH1D *auxcorr6 = hEKcorr->ProjectionY("hEKcorr_6", 6, 6, "");
			aux6->Scale(1./aux6->Integral());
			aux6->SetLineColor(4);
			aux6->Draw("SAME");
			auxcorr6->Scale(1./auxcorr6->Integral());
			auxcorr6->SetLineColor(4);
			auxcorr6->SetLineStyle(2);
			auxcorr6->Draw("SAME");
			leg2->AddEntry(aux6, "E = 1000 MeV", "lp");
			leg2->AddEntry(auxcorr6, "E = 1000 MeV corrected", "lp");
			leg2->Draw();

		
	} else cout << "*** cannot find object hEK" << endl;
	
	
/*	TH2F *hEtotEtag_sel = (TH2F *) hc->FindObject("hEtotEtag_sel");
	if (hEtotEtag_sel) {	
		for (int i=0; i<nEtag-1; i++) {
			TString pname("hEtotEtag_sel_");
			pname += i+1;
			TH1D *aux = hEtotEtag_sel->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = 1./aux->Integral();
			//aux->Scale(norm);
			aux->SetLineColor(i+1);
			if (i==0) {
				aux->GetXaxis()->SetTitle("E_MCAL (MeV)");
				aux->GetYaxis()->SetTitle("Counts");
				aux->Draw();
			} else aux->Draw("SAME");
		}
		leg->Draw();
	} else cout << "*** cannot find object hEtotEtag_sel" << endl;*/
	
	c4->cd(4);
	if (hEK) hEK->Draw("COLZTEXT");
	
	
/*	TH2F *hEratioEtag_sel = (TH2F *) hc->FindObject("hEratioEtag_sel");
	if (hEratioEtag_sel) {	
		for (int i=0; i<nEtag-1; i++) {
			TString pname("hEratioEtag_sel_");
			pname += i+1;
			TH1D *aux = hEratioEtag_sel->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = 1./aux->Integral();
			//aux->Scale(norm);
			aux->SetLineColor(i+1);
			if (i==0) {
				aux->GetXaxis()->SetTitle("EX/EZ ");
				aux->GetYaxis()->SetTitle("Counts");
				aux->Draw();
			} else aux->Draw("SAME");
		}
		leg->Draw();
	} else cout << "*** cannot find object hEratioEtag_sel" << endl;*/

	return;
}

//
//--------------------------------------------------------------------
//

void drawKalmanTrackHisto(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Kalman tracks vs MCAL points analysis
	
	TCanvas *c7;
	c7 = new TCanvas("MCAL_data_7", "Kalman tracks vs MCAL points", 800, 600);
	c7->Divide(3, 2);
	c7->cd(1);
	
	c7->cd(1);
	TH2F *hPosDevP1X = (TH2F *) hc->FindObject("hPosDevP1X");
	if (hPosDevP1X) {	
		hPosDevP1X->Draw("COLZ");
	} else cout << "*** cannot find object hPosDevP1X" << endl;
	
	c7->cd(2);
	TH2F *hXDevP1X = (TH2F *) hc->FindObject("hXDevP1X");
	if (hXDevP1X) {	
		TH1D *aux = hXDevP1X->ProjectionY("hXDevP1X_tot", 1, 4, "");
		aux->GetXaxis()->SetTitle("X deviation (cm) ");
		aux->GetYaxis()->SetTitle("Counts");
		aux->Draw();
		//cout << "hXDevP1X_tot  -  # underflow entries = " <<  aux->GetBinContent(0) << "  -  # in range entries = " <<  aux->Integral() << "  -  # overflow entries = " <<  aux->GetBinContent(101) << endl;		
	} else cout << "*** cannot find object hXDevP1X" << endl;
	
	// draws the same histogram divided by bar number (check for calibration)
	TH2F *hXDevP1X_bar = (TH2F *) hc->FindObject("hXDevP1X_bar");
	if (hXDevP1X_bar) {	
		for (int i=0; i<15; i++) {
			TString pname("hXDevP1X_bar_");
			pname += i;
			TH1D *aux = hXDevP1X_bar->ProjectionY(pname.Data(), i+1, i+1, "");
			aux->SetLineColor(i);
			aux->Draw("SAME");
			//cout << pname.Data() << "  -  # underflow entries = " <<  aux->GetBinContent(0) << "  -  # in range entries = " <<  aux->Integral() << "  -  # overflow entries = " <<  aux->GetBinContent(101) << endl;		
		}
	} else cout << "*** cannot find object hXDevP1X_bar" << endl;
	
	
	c7->cd(3);
	TH1F *hZDevP1X = (TH1F *) hc->FindObject("hZDevP1X");
	if (hZDevP1X) {	
		hZDevP1X->Draw();
		//cout << "hZDevP1X  -  # Zero entries = " <<  hZDevP1X->GetBinContent(0) << "  -  # total entries = " <<  hZDevP1X->Integral() << "  -  # upper limit entries = " <<  hZDevP1X->GetBinContent(101) << endl;
	} else cout << "*** cannot find object hZDevP1X" << endl;
	
	c7->cd(4);
	TH2F *hPosDevP2X = (TH2F *) hc->FindObject("hPosDevP2X");
	if (hPosDevP2X) {	
		hPosDevP2X->Draw("COLZ");
	} else cout << "*** cannot find object hPosDevP2X" << endl;
	
	c7->cd(5);
	TH2F *hXDevP2X = (TH2F *) hc->FindObject("hXDevP2X");
	if (hXDevP2X) {	
		TH1D *aux = hXDevP2X->ProjectionY("hXDevP2X_tot", 1, 4, "");
		aux->GetXaxis()->SetTitle("X deviation (cm) ");
		aux->GetYaxis()->SetTitle("Counts");
		aux->Draw();
		//cout << "hXDevP2X_tot  -  # underflow entries = " <<  aux->GetBinContent(0) << "  -  # in range entries = " <<  aux->Integral() << "  -  # overflow entries = " <<  aux->GetBinContent(101) << endl;		
	} else cout << "*** cannot find object hXDevP2X" << endl;
	
	c7->cd(6);
	TH1F *hZDevP2X = (TH1F *) hc->FindObject("hZDevP2X");
	if (hZDevP2X) {	
		hZDevP2X->Draw();
	} else cout << "*** cannot find object hZDevP2X" << endl;

}

//
//--------------------------------------------------------------------
//

void drawKalmanTrackHisto_energy(TObjArray *hc, int nEtag, Double_t *Estep)
{
	// draw histograms for Kalman tracks vs MCAL points analysis SELECTED WITH ENERGY
	
	TCanvas *c8;
	c8 = new TCanvas("MCAL_data_8", "Kalman tracks vs MCAL points ENERGY SELECTED", 800, 600);
	c8->Divide(4, 3);	
	c8->cd(1);
	
	TH2F *hXDevP1X = (TH2F *) hc->FindObject("hXDevP1X");
	if (hXDevP1X) {	
		for (int i=0; i<nEtag-1; i++) {
			c8->cd(i+1);
			TString pname("hXDevP1X_");
			pname += i+1;
			TString erange("P1 X deviation: E_PTS = ");
			erange += Estep[i];
			erange += " - ";
			erange += Estep[i+1];
			erange += " MeV";
			TH1D *aux = hXDevP1X->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = 1./aux->Integral();
			//cout << "hXDevP1X Slice number " << i+1 << "  -  # Zero entries = " <<  aux->GetBinContent(0) << "  -  # total entries = " <<  aux->Integral() << "  -  # upper limit entries = " <<  aux->GetBinContent(101) << endl;
			//aux->Scale(norm);
			aux->SetLineColor(i+1);
			aux->GetXaxis()->SetTitle("X deviation (cm) ");
			aux->GetYaxis()->SetTitle("Counts");
			aux->SetTitle(erange.Data());
			aux->Draw();
		}
	} else cout << "*** cannot find object hXDevP1X" << endl;
	
	TH2F *hXDevP2X = (TH2F *) hc->FindObject("hXDevP2X");
	if (hXDevP2X) {	
		for (int i=0; i<nEtag-1; i++) {
			c8->cd(i+5);
			TString pname("hXDevP2X_");
			pname += i+1;
			TString erange("P2 X deviation: E_PTS = ");
			erange += Estep[i];
			erange += " - ";
			erange += Estep[i+1];
			erange += " MeV";
			TH1D *aux = hXDevP2X->ProjectionY(pname.Data(), i+1, i+1, "");
			float norm = 1./aux->Integral();
			//cout << "hXDevP2X Slice number " << i+1 << "  -  # Zero entries = " <<  aux->GetBinContent(0) << "  -  # total entries = " <<  aux->Integral() << "  -  # upper limit entries = " <<  aux->GetBinContent(101) << endl;
			//aux->Scale(norm);
			aux->SetLineColor(i+1);
			aux->GetXaxis()->SetTitle("X deviation (cm) ");
			aux->GetYaxis()->SetTitle("Counts");
			aux->SetTitle(erange.Data());
			aux->Draw();
		}
	} else cout << "*** cannot find object hXDevP2X" << endl;

}

//
//--------------------------------------------------------------------
//

void drawResponseHisto(TObjArray *hc, int nEtag, Double_t *Estep)
{
	TCanvas *c9;
	c9 = new TCanvas("MCAL_data_9", "MCAL energy guess", 800, 600);
	c9->Divide(4, 3);	
	
	c9->cd(1);	
	TH2F *hEMEK = (TH2F *) hc->FindObject("hEMEK");
	if (hEMEK) {	
		hEMEK->Draw("COLZTEXT");
	} else cout << "*** cannot find object hEMEK" << endl;
	
	c9->cd(2);	
	TH2F *hEMEtag = (TH2F *) hc->FindObject("hEMEtag");
	if (hEMEtag) {	
		hEMEtag->Draw("COLZTEXT");
	} else cout << "*** cannot find object hEMEtag" << endl;

	c9->cd(3);	
	TH2F *hBarCorr_RM = (TH2F *) hc->FindObject("hBarCorr_RM_3");
	if (hBarCorr_RM) {
		hBarCorr_RM->Draw("COLZTEXT");
	} else cout << "*** cannot find object hBarCorr_RM" << endl;

	c9->cd(4);	
	TH2F *hEXEZ_RM = (TH2F *) hc->FindObject("hEXEZ_RM_3");
	if (hEXEZ_RM) {
		hEXEZ_RM->Draw("COLZTEXT");
	} else cout << "*** cannot find object hEXEZ_RM" << endl;

	TH2F *hEK = (TH2F *) hc->FindObject("hEK");	
	TH2F *hEMavgEtag = (TH2F *) hc->FindObject("hEMavgEtag");
	if (hEK && hEMEtag && hEMavgEtag) {	
		for (int i=0; i<nEtag-1; i++) {
			c9->cd(i+5);
			TString pname("hEMEtag_");
			pname += i+1;
			TString erange("EM vs EK: E tag = ");
			erange += Estep[i];
			erange += " - ";
			erange += Estep[i+1];
			erange += " MeV";
			TH1D *aux = hEMEtag->ProjectionY(pname.Data(), i+1, i+1, "");
			aux->SetLineColor(1);
			aux->GetXaxis()->SetTitle("Energy (MeV) ");
			aux->GetYaxis()->SetTitle("Counts");
			aux->SetTitle(erange.Data());
			aux->Draw();
			
			TString pname2("hEK_");
			pname2 += i+1;
			TH1D *aux2 = hEK->ProjectionY(pname2.Data(), i+1, i+1, "");
			aux2->SetLineColor(2);
			aux2->Draw("SAME");
						
			/*TString pname3("hEMavgEtag_");
			pname3 += i+1;
			TH1D *aux3 = hEMavgEtag->ProjectionY(pname3.Data(), i+1, i+1, "");
			aux3->SetLineColor(3);
			aux3->Draw("SAME");*/
		}
	} else cout << "*** cannot find object hEK or hEMEtag or hEMavgEtag" << endl;
}

//
//--------------------------------------------------------------------
//

void drawMCALCorrectionHisto(TObjArray *hc, int nEtag, Double_t *Estep)
{
	TLine *l;
	TCanvas *c11;
	c11 = new TCanvas("MCAL_data_11", "MCAL GRID Energy correction", 800, 600);
	c11->Divide(4, 3);

			
	for (int i=0; i<nEtag; i++) {
		TGraph *gaux=NULL;
		TString gname("gEMEK_");
		gname += i;
		gaux = (TGraph *) hc->FindObject(gname.Data());
	
		c11->cd(i+1);
		if (gaux) {
			gaux->GetXaxis()->SetTitle("Kalman Energy (MeV)");
			gaux->GetXaxis()->SetTitleOffset(1.4);
			gaux->GetXaxis()->CenterTitle();
			gaux->GetYaxis()->SetTitle("MCAL Energy");
			gaux->GetYaxis()->CenterTitle();
			gaux->GetYaxis()->SetTitleOffset(1.5);
			gaux->Draw("AP");
			l = new TLine(0., 0., 1000., 1000.);
			l->SetLineColor(2);
			l->Draw();
		} else cout << "*** cannot find object " << gname.Data() << endl;
	}
	
	c11->cd(9);
	TH1F *hEMcorr = (TH1F *) hc->FindObject("hEMcorr");
	hEMcorr->Draw();
	
	return;
}

//
//--------------------------------------------------------------------
//

void drawResponseMatrix(TObjArray *hc, selection *sel)
{
	TCanvas *c10;
	c10 = new TCanvas("MCAL_data_10", "MCAL Response Matrix", 800, 600);
	c10->Divide(4, 3);

	if (sel->buildResponseMatrix || sel->doMcalGuess) {
		int ilimit = NBINS<=6 ? NBINS : 6;
		for (int i=0; i<ilimit; i++) {
			cout << "moving to pad " << (i/2)*4 + i%2 +1<< endl;
			c10->cd((i/2)*4 + i%2 +1);
			gPad->SetLogz(1);
			TString name("hBarCorr_RM_");
			name += i;
			TH2F *aux = (TH2F *) hc->FindObject(name.Data());
			if (aux) {
				aux->SetStats(0);
				aux->Draw("COLZ");
			}
			else cout << "*** cannot find object " << name.Data() << endl;
			
			cout << "moving to pad " << (i/2)*4 + i%2 + 3 << endl;
			c10->cd((i/2)*4 + i%2 + 3);
			gPad->SetLogz(1);
			TString name2("hEXEZ_RM_");
			name2 += i;
			TH2F *aux2 = (TH2F *) hc->FindObject(name2.Data());
			if (aux2) {
				aux2->SetStats(0);
				aux2->Draw("COLZ");
			}
			else cout << "*** cannot find object " << name2.Data() << endl;
		}
	} else cout << "Nothing to do in canvas MCAL_data_10" << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void printTrackResults(TObjArray *histo, int nevents, selection *sel, MCVFileInfo *info)
{
	// prints tracks analysis results in table form

	TH1F *hNBarsX = (TH1F *) histo->FindObject("hNBarsX");
	TH1F *hNBarsZ = (TH1F *) histo->FindObject("hNBarsZ");
	TH1F *hNTracksX = (TH1F *) histo->FindObject("hNTracksX");
	TH1F *hNTracksZ = (TH1F *) histo->FindObject("hNTracksZ");
	TH2F *hTracksCorr = (TH2F *) histo->FindObject("hTracksCorr");
	
	int nX = hNBarsX->Integral(1, 15);
	int nZ = hNBarsZ->Integral(1, 15);
	cout << "Total number of events = " << nevents << endl;
	cout << "Number of events with energy deposit greater than " << sel->minE << " MeV in one bar on plane X = " << nX << " (" << 100.*nX/nevents << "%)" << endl;
	cout << "Number of events with energy deposit greater than " << sel->minE << " MeV in one bar on plane Z = " << nZ << " (" << 100.*nZ/nevents << "%)" << endl;
	cout << "Number of events with (1,1) tracks topology = " << hTracksCorr->GetBinContent(2,2) << " (" << 100.*hTracksCorr->GetBinContent(2,2)/nevents << "%) rate = " << hTracksCorr->GetBinContent(2,2)/info->teff << " Hz" << endl;
	cout << "Number of events with (2,2) tracks topology = " << hTracksCorr->GetBinContent(3,3) << " (" << 100.*hTracksCorr->GetBinContent(3,3)/nevents << "%) rate = " << hTracksCorr->GetBinContent(3,3)/info->teff << " Hz" << endl;
	
	// print the normalized TracksX vs TracksZ histogram
	printf("\nEvents\n");
	printf("\t   X0 \t   X1 \t   X2 \t   X3 \t   X4 \t   X5 \t   X6 \t   X7 \t   X8\n");
	for (int j=0; j<=8; j++) {
		printf("Z%d", j);
		for (int i=0; i<=8; i++) if (hTracksCorr->GetBinContent(i+1,j+1)>0) 
						printf("\t%6.0f", hTracksCorr->GetBinContent(i+1,j+1));
					else printf("\t      ");
		printf("\n");
	}
	
	printf("\nEvents fraction\n");
	printf("\tX0 \tX1 \tX2 \tX3 \tX4 \tX5 \tX6 \tX7 \tX8\n");
	for (int j=0; j<=8; j++) {
		printf("Z%d", j);
		for (int i=0; i<=8; i++) if (hTracksCorr->GetBinContent(i+1,j+1)>0) 
						printf("\t%6.4f", hTracksCorr->GetBinContent(i+1,j+1)/nevents);
					else printf("\t      ");
		printf("\n");
	}
	
	printf("\nEvents rate\n");
	printf("\tX0 \tX1 \tX2 \tX3 \tX4 \tX5 \tX6 \tX7 \tX8\n");
	for (int j=0; j<=8; j++) {
		printf("Z%d", j);
		for (int i=0; i<=8; i++) if (hTracksCorr->GetBinContent(i+1,j+1)>0)
						 printf("\t%6.4f", hTracksCorr->GetBinContent(i+1,j+1)/info->teff);
					else printf("\t      ");
		printf("\n");
	}
	return;
}
