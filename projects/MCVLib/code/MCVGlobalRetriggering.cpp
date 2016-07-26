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


#include "MCVGlobalRetriggering.h"
ClassImp(MCVGlobalRetriggering);

MCVGlobalRetriggering::MCVGlobalRetriggering() : MCVStdAnalysis(), hDT(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVGlobalRetriggering::MCVGlobalRetriggering(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in), hDT(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVGlobalRetriggering::~MCVGlobalRetriggering()
{

}

//
//--------------------------------------------------------------------
//

int MCVGlobalRetriggering::Calculate()
{
	
	maxfrac = conf->GetValue(".globalretrig.maximum.excess.", 0.01);
	tmax = conf->GetValue(".globalretrig.maximum.time.", 100);
	fitlow = conf->GetValue(".globalretrig.fit.low.", 100.);
	fithi = conf->GetValue(".globalretrig.fit.hi.", 1000.);
	if (!rawHisto) {
		status = RAW_HISTO_NOT_SET;
		return status;
	}
	if (info->pdhuformat && info->pkttype!=3909) {
		status = WRONG_PKTTYPE;
		return status;
	}
	hDT  = (TH1F *) rawHisto->FindObject("totDT");
	if (hDT == NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	mu = info->nevents / info->teff;		// average total count rate
	fdt  = new TF1("fdt","expo", fitlow, fithi);
	par[0] = log(mu*1.E-6*info->nevents); // *trebin*1.E-6
	par[1] = -mu*1.E-6;		
	double cf = hDT->Integral(100, 5000);
	double ef = fdt->Integral(100, 5000);
	par[0] = par[0]+log(cf/ef);	// normalize time delay function
	fdt->SetParameters(par);
	hDT->SetAxisRange(fitlow, fithi, "X");
	hDT->Fit("fdt","QR0+");
	fdt->GetParameters(par);
	//cout << par[0] << "  " << par[1] << endl;
	fdt->SetRange(0., 100000. );
	
	// integral of counts figure of merit
	c0 = hDT->Integral(2, tmax);
	e0 = fdt->Integral(2, tmax);
	extracnts = c0 - e0;
	extrafrac = extracnts/info->nevents;
   		
	// update graphics if necessary	
	if (c1) {
		c1->cd(10);
		fdt->SetLineWidth(1);
		fdt->SetLineColor(2);
		fdt->Draw("same");
		hDT->SetAxisRange(0., 100., "X");
		c1->Update();
	}

	if (extrafrac>maxfrac) status = OUT_OF_SPEC;
	else status = OK;
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVGlobalRetriggering::PrintResults()
{
	printf("\n------------------ GLOBAL RETRIGGERING SEARCH -------------------\n");
	printf("\nNumber of expected events with DT<%dus = %.0f (%6.2f%%)\n", tmax, e0, 100.*e0/info->nevents); 
	printf("Number of measured events with DT<%dus = %.0f (%6.2f%%)\n", tmax, c0, 100.*c0/info->nevents); 
	printf("Extra fraction of events with DT<%dus  = %6.2f%%\n", tmax, 100.*extrafrac); 
	if (extrafrac>maxfrac) printf("WARNING: Extra fraction of events > %.3f\n", maxfrac);
	return;
}
