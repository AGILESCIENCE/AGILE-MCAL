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

#include "MCVRetriggering.h"
ClassImp(MCVRetriggering);

MCVRetriggering::MCVRetriggering() : MCVStdAnalysis(), rDT(NULL), rPDA(NULL), rCR(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVRetriggering::MCVRetriggering(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in), rDT(NULL), rPDA(NULL), rCR(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVRetriggering::~MCVRetriggering()
{

}

//
//--------------------------------------------------------------------
//

int MCVRetriggering::Calculate()
{
	double 	par[2];
	nlowdeadtime = 0;
	nretriggering = 0;
	mindt = conf->GetValue(".minimum.dead.time.", 15.);
	maxfrac = conf->GetValue(".retrigger.maximum.excess.", 0.0005);
	tmax = conf->GetValue(".retrigger.maximum.time.", 50);
	nsigma = conf->GetValue(".retrigger.significance.level.", 3.);
	if (!rawHisto) {
		status = RAW_HISTO_NOT_SET;
		return status;
	}
	if (info->pdhuformat && info->pkttype!=3909) {
		status = WRONG_PKTTYPE;
		return status;
	}
	rDT  = (TObjArray *) rawHisto->FindObject("rawDTime");
	rCR  = (TObjArray *) rawHisto->FindObject("rawCRate");
	rPDA = (TObjArray *) rawHisto->FindObject("rawPDA");
	if (rDT == NULL || rPDA == NULL || rCR == NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	
	// calculate events per bar and average rate (from effective time)
	for (int i=0; i<30; i++) {
		totevts[i] = ((TH1F *) rPDA->At(i))->GetEntries();	// get total number of events
		avgCR[i] = totevts[i]/GetEffectiveTime(i);
	}
	
	for (int i=0; i<30; i++) {	// working on delta T histograms   
		if (c1) c1->cd(i+1);
		TH1F *aux = (TH1F *) rDT->At(i);
		// creates the exponential delta t function
		double mu = avgCR[i];
		fdt  = new TF1("fdt","expo", 0., 100000. );
		par[0] = log(mu*totevts[i]*1.E-6); // *trebin*1.E-6
		par[1] = -mu*1.E-6;		
		fdt->SetParameters(par);
		
		double cf = aux->Integral(tmax+1, 5000);
		double ef = fdt->Integral(tmax+1, 5000);
		par[0] = par[0]+log(cf/ef);	// normalize time delay function
		fdt->SetParameters(par);
		
		// dead time calculation
		if (aux->GetEntries() != 0) {
			for (int k=1; k<1000; k++) {	// gets the first non-zero time bin
				if (aux->GetBinContent(k) != 0) {
					tfirst[i] = k-1;
					break;
				}
			}
			for (int k=1; k<1000; k++) {	// gets the dead time bin
				if (aux->GetBinContent(k) >= 0.5*fdt->Eval(k-0.5)) {
					tdead[i] = k-1;
					break;
				}
			}
		} else tdead[i] = -1;
		if (tdead[i]<mindt) {
			outofspec[i]=TRUE;
			nlowdeadtime++;
		}
		
		// integral of counts figure of merit
		double c0 = aux->Integral(0, tmax);
		double e0 = fdt->Integral(tdead[i], tmax);
		extracnts[i] = c0 - e0;
		extrafrac[i] = fabs(extracnts[i])/totevts[i];
		if (e0>0) statsig[i] = fabs(extracnts[i])/sqrt(e0);
		else statsig[i] = 100.;
		if (extrafrac[i]>maxfrac && statsig[i]>nsigma) {
			outofspec[i]=TRUE;
			nretriggering++;
		}
   		if (c1) {
			fdt->SetLineWidth(1);
			fdt->SetLineColor(2);
			fdt->Draw("same");
			c1->Update();
		}
	}
	if (nretriggering>0) status = OUT_OF_SPEC;
	else status = OK;

	return status;
}

//
//--------------------------------------------------------------------
//

void MCVRetriggering::PrintResults()
{
	printf("\n--------------------- RETRIGGERING SEARCH ----------------------\n");
	if (info) printf("run\t\t %d\n", info->runid);
	if (status == OK || status == OUT_OF_SPEC) {
		printf("minimum allowed dead time = %.0f\n", mindt);
		printf("maximum allowed retriggering events fraction = %.4f\n", maxfrac);
		printf("\nplane\t bar\t rate (Hz)\t T_dead (us)\t T_ini (us)\t retrigger. fract.\t significance level \n");
		for (int i=0; i<15; i++) {
			printf(" X\t %d\t %6.2f\t\t %d\t\t %d\t\t %8f\t\t %6.1f\t", i+1, avgCR[i], tdead[i], tfirst[i], extrafrac[i], statsig[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		for (int i=15; i<30; i++) {
			printf(" Z\t %d\t %6.2f\t\t %d\t\t %d\t\t %8f\t\t %6.1f\t", i-14, avgCR[i], tdead[i], tfirst[i], extrafrac[i], statsig[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		cout << endl;
		if (nretriggering)  cout << "WARNING: " << nretriggering << " bars show retriggering" << endl;
		if (nlowdeadtime)  cout << "WARNING: " << nlowdeadtime << " bars show a low dead time" << endl;
		return;
	} 
	return;
}
