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

#include "MCVEnergyThreshold.h"
ClassImp(MCVEnergyThreshold);

MCVEnergyThreshold::MCVEnergyThreshold() : MCVStdAnalysis()
{
	nout50 = nout90 = 0;
}

//
//--------------------------------------------------------------------
//

MCVEnergyThreshold::MCVEnergyThreshold(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	nout50 = nout90 = 0;
}

//
//--------------------------------------------------------------------
//

MCVEnergyThreshold::~MCVEnergyThreshold()
{

}

//
//--------------------------------------------------------------------
//

int MCVEnergyThreshold::Calculate()
{
	TLine	*l1;
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
		if (c1) c1->cd(i+1);
		TH1F *aux = (TH1F *) rE->At(i);
		aux->SetAxisRange(0., 1., "X");
		if (aux->GetEntries()) {
   			TSpectrum *s = new TSpectrum(4);
			Int_t nfound = s->Search(aux,2,"goff");	// 1
			if (nfound == 0) {
				cout << "*** bar " << i << " - PD-A - NO peak found!" << endl;
				continue;
			} else {
				Float_t *xpeaks = s->GetPositionX();
				int k50=1, k90=1;
				float cmax=0.;
				float c50=0.;
				float c90=0.;
				for (int k=0; k<nfound; k++) {
					int ibin = aux->GetXaxis()->FindBin(xpeaks[k]);
					if (cmax < aux->GetBinContent(ibin)) cmax=aux->GetBinContent(ibin);
				}
				while (c50 <= 0.5*cmax) c50 = aux->GetBinContent(k50++);	// get the 50% threshold
				while (c90 <= 0.9*cmax) c90 = aux->GetBinContent(k90++);	// get the 50% threshold
				th50[i] = (k50-2)*0.020;
				th90[i] = (k90-2)*0.020;
				if (c1) {
					l1 = new TLine(th50[i], 0., th50[i], cmax*1.2);  
					l1->SetLineColor(2);
					l1->Draw();
					c1->Update();
				}
			}
			delete s;
		}
	}
	
	// load reference values and make comparison
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info) {
		char bar[4];
		
		if (!refname) {
			refname = new TString();
			refname->Append(getenv("MCALSW"));
			refname->Append(conf->GetValue(".threshold.reference.file.", "thr.txt"));
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			fscanf(fref, "run %d\n", &refrun);
			fscanf(fref, "bar 	E_th 50%% (MeV) 	E_th 90%% (MeV)\n");
			for (int i=0; i<30; i++) fscanf(fref, "%s %f %f", &bar, &ref50[i], &ref90[i]);
			fclose(fref);
		}
		maxdev = conf->GetValue(".threshold.max.deviation.", 1.);
		for (int i=0; i<30; i++) {
			if (100.*fabs(th50[i]/ref50[i] -1.) > maxdev) {
				outofspec[i] = TRUE;
				nout50++;
			}
			if (100.*fabs(th90[i]/ref90[i] -1.) > maxdev) {
				outofspec[i] = TRUE;
				nout90++;
			}
		}
		if (nout50 == 0 && nout90 == 0) status = OK;
		else status = OUT_OF_SPEC;
	}
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVEnergyThreshold::PrintResults()
{
	float avgth50=0., avgth90=0.;
	printf("\n--------------------- ENERGY THRESHOLD ----------------------\n");
	if (status == OK || status == OUT_OF_SPEC) {
		printf("\nreference run\t%d\n", refrun);
		printf("\nbar \tE_th 50% (MeV) \tE_th 90% (MeV) \tref. 50% (MeV) \tref. 90% (MeV)\n\n");
		for (int i=0; i<30; i++) {
			if (i<15) {
				printf("X%d \t%6.3f \t\t%6.3f \t\t%6.3f \t\t%6.3f\t", i+1, th50[i], th90[i], ref50[i], ref90[i]);
				if (outofspec[i]) printf("*****\n");
				else printf("\n");
			} else {
				printf("Z%d \t%6.3f \t\t%6.3f \t\t%6.3f \t\t%6.3f\t", i-14, th50[i], th90[i], ref50[i], ref90[i]);
				if (outofspec[i]) printf("*****\n");
				else printf("\n");
			}
			avgth50 += th50[i];
			avgth90 += th90[i];
		}
		avgth50 /= 30.;
		avgth90 /= 30.;
		cout << "Average threshold @ 50% of the peak counts = " << avgth50 << endl;
		cout << "Average threshold @ 90% of the peak counts = " << avgth90 << endl;
		if (nout50>0) cout << endl << "WARNING: " << nout50 << " bars with deviation from reference 50% threshold > " << maxdev << " %" << endl;
		if (nout90>0) cout << endl << "WARNING: " << nout90 << " bars with deviation from reference 90% threshold > " << maxdev << " %" << endl;
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVEnergyThreshold::SaveToFile()
{
	if (conf) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".threshold.data.path.", "./");
		outname += conf->GetValue(".threshold.data.template.", "threshold_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing threshold data to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "run \t%d\n", info->runid);
			fprintf(fout, "bar \tE_th 50% (MeV) \tE_th 90% (MeV)\n");
			for (int i=0; i<30; i++) {
				if (i<15) fprintf(fout, "X%d \t%6.3f \t\t%6.3f\n", i+1, th50[i], th90[i]);
				else      fprintf(fout, "Z%d \t%6.3f \t\t%6.3f\n", i-14, th50[i], th90[i]);
			}
			fclose(fout);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	}

	return;
}
