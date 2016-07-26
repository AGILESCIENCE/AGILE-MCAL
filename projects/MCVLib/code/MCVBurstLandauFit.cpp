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

#include "MCVBurstLandauFit.h"
ClassImp(MCVBurstLandauFit);

MCVBurstLandauFit::MCVBurstLandauFit() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVBurstLandauFit::MCVBurstLandauFit(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	meanmpv=0.;
}

//
//--------------------------------------------------------------------
//

MCVBurstLandauFit::~MCVBurstLandauFit()
{
	meanmpv=0.;
	meanE=0.;
}

//
//--------------------------------------------------------------------
//

int MCVBurstLandauFit::Calculate()
{
	double par[6];
	char totname[20], peakname[20], bkgname[20];
	TF1 *total, *peak, *bkg;
	TObjArray *    rE=NULL;
	
	if (rawHisto==NULL || info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	rE =(TObjArray *) rawHisto->FindObject("rawE");
	if (rE==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	for (int i=0; i<30; i++) {
		TH1F *aux = (TH1F *) rE->At(i);
		aux->Rebin(50);	// 10
		aux->SetAxisRange(2., 40., "X");
		if (aux->GetEntries()) {
			aux->Scale(info->teff);
			sprintf(peakname, "peak_%d", i);
			peak  = new TF1(peakname,"landau", 8.,  25.); // (int)
			sprintf(bkgname, "bkg_%d", i);
			bkg   = new TF1(bkgname, "expo", 4., 7.);
			sprintf(totname, "total_%d", i);
			total = new TF1(totname,"expo(0)+landau(2)", 4., 35.);
		
//			bkg->SetParLimits(0, 0., 1.E9);
//			bkg->SetParLimits(1, -1.E9, 0.);
			aux->Fit(peakname,"QLMR0+");
			aux->Fit(bkgname,"QLMR0+");
			peak->GetParameters(&par[2]);
			peak->SetLineColor(3);
			peak->SetLineWidth(1);
			bkg->GetParameters(&par[0]);
			bkg->SetLineColor(4);
			bkg->SetLineWidth(1);			
			total->SetLineColor(2);
			total->SetLineWidth(1);
			total->SetParameters(par);
//			total->SetParLimits(0, 0., 1.E9);
//			total->SetParLimits(1, -1.E9, 0.);
			total->SetParLimits(2, 0.5*par[2], 1.5*par[2]);
			total->SetParLimits(3, 0.8*par[3], 1.2*par[3]);
			total->SetParLimits(4, 0.5*par[4], 1.5*par[4]);
			aux->Fit(totname,"QLMBR0+");
			
			total->GetParameters(&par[0]);
			mpv[i] = par[3];
			sig[i] = par[4];
			peak->SetParameters(&par[2]);
			mE[i] = peak->Mean(5., 50.);
			meanE += mE[i];
			bkg->SetParameters(&par[0]);
			amp[i] = peak->Integral(0., 50.)/info->teff;
			meanmpv += par[3];
			if (c1) {
				c1->cd(i+1);
				total->SetRange(2., 40.);
				total->Draw("same");
				peak->SetRange(2., 40.);
				peak->Draw("same");
				bkg->SetRange(2., 40.);
				bkg->Draw("same");
				c1->Update();
			}	
		}	
	}
	meanmpv /=30.;
	meanE /=30.;
	
	// load reference values and make comparison
/*	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info) {
		if (!refname) {
			refname = new TString();
			refname->Append(getenv("MCALSW"));
			refname->Append(conf->GetValue(".grid.ecal.reference.", "ref.txt"));
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			char 	bar[3];
			fscanf(fref, "run %d\n", &refrun);
			fscanf(fref, "bar     muon MPV (MeV)  sigma (MeV)     frequency (evt/s)\n");
			for (int i=0; i<30; i++) fscanf(fref, "%s %f %f %f", &bar, &refmpv[i], &refsig[i], &refamp[i]);
			fclose(fref);
			
			noutmpv = noutsig = noutamp = 0;
			maxmpvdev = conf->GetValue(".grid.ecal.mpv.deviation.", 0.);
			maxsigdev = conf->GetValue(".grid.ecal.sigma.deviation.", 0.);
			maxampdev = conf->GetValue(".grid.ecal.rate.deviation.", 0.);
			for (int i=0; i<30; i++) {
				if (100.*fabs(mpv[i]/refmpv[i]-1.) > maxmpvdev) {
					noutmpv++;
					outofspec[i] = TRUE;
				}
				if (100.*(sig[i]/refsig[i] - 1.) > maxsigdev) {
					noutsig++;
					outofspec[i] = TRUE;
				}
				if (100.*fabs(amp[i]/refamp[i] - 1.) > maxampdev) {
					noutamp++;
					outofspec[i] = TRUE;
				}
			}
			if (noutmpv == 0 && noutsig == 0 && noutamp == 0) status = OK;
			else status = OUT_OF_SPEC;
		} else 	status = REF_FILE_NOT_FOUND;
	} else status = RUN_INFO_NOT_SET;*/
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVBurstLandauFit::PrintResults()
{
	printf("\n--------------------- BURST MUON LANDAU FIT ----------------------\n");
	printf("\nbar \tMPV (MeV) \tsigma (MeV)  \tmean (MeV) \trate (evt/s) \n");
	for (int i=0; i<30; i++) {
		printf("%c%d \t%6.2f \t\t%6.2f \t\t%6.2f \t\t%6.3f \n", i<15 ? 'X' : 'Z', i<15 ? i+1 : i-14, mpv[i], sig[i], mE[i], amp[i]);
	}
	cout << endl << "Calculated mean MPV for muons in MCAL in BURST mode = " << meanmpv << " MeV" << endl;
	return;
}

//
//--------------------------------------------------------------------
//
