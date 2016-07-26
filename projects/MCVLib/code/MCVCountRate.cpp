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

#include "MCVCountRate.h"
ClassImp(MCVCountRate);

MCVCountRate::MCVCountRate() : MCVStdAnalysis(), crx(NULL), sigx(NULL), crz(NULL), sigz(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVCountRate::MCVCountRate(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in), crx(NULL), sigx(NULL), crz(NULL), sigz(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVCountRate::~MCVCountRate()
{

}

//
//--------------------------------------------------------------------
//

int MCVCountRate::Calculate()
{
	// retrieve count rate from gCRX and gCRZ already stored graphics.
	// TH1F::GetMean() and GetRMS() are used: NO DIRECT C.R. DISTRIBUTION GAUSSIAN FIT IMPLEMENTED YET
	// To do: total rate = total counts / effective time
	
	TGraphErrors *gcrx=NULL;
	TGraphErrors *gcrz=NULL;
	
	if (!rawHisto) {
		status = RAW_HISTO_NOT_SET;
		return status;
	}
	
	gcrx = (TGraphErrors *) rawHisto->FindObject("gCRX");
	gcrz = (TGraphErrors *) rawHisto->FindObject("gCRZ");
	if (gcrx && gcrz) {
		crx = gcrx->GetY();
		sigx = gcrx->GetEY();
		crz = gcrz->GetY();
		sigz = gcrz->GetEY();
	} else {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
		
	// compare results to reference run
	CompareReferenceData();
	
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVCountRate::CompareReferenceData()
{
	npois = 0;
	// load reference data
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return;
	}
	if (info) {
		if (!refname) {
			refname = new TString();
			refname->Append(getenv("MCALSW"));
			switch (info->pkttype) {
				case 3901:
					refname->Append(conf->GetValue(".obs.rate.reference.", "ref.txt"));
				break;
				case 3902:
					refname->Append(conf->GetValue(".grid.rate.reference.", "ref.txt"));
				break;
				case 3909:
					refname->Append(conf->GetValue(".burst.rate.reference.", "ref.txt"));
				break;
				default:
					refname->Append(conf->GetValue(".dfete.rate.reference.", "ref.txt"));
				break;
			}
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			float refrate;
			char 	plane[3];
			int	baddr;
			drmax;
			nextra=0;
			if (info->grid) drmax = conf->GetValue(".grid.rate.deviation.", 0.);
			else drmax = conf->GetValue(".burst.rate.deviation.", 0.);
			fscanf(fref, "run %d\n", &refrun);
			fscanf(fref, "rate %f\n", &refrate);
			fscanf(fref, "plane	 bar	 rate (evt/s)	 sigma\n");
			for (int i=0; i<15; i++) {
				fscanf(fref, "%s %d %f %f",  &plane, &baddr, &refcr[i], &refsig[i]);
				dr[i] = 100.*(crx[i]-refcr[i])/refcr[i];
				if (fabs(dr[i])>drmax) {
					outofspec[i] = TRUE;
					nextra++;
				} 
			}
			for (int i=15; i<30; i++) {
				fscanf(fref, "%s %d %f %f",  &plane, &baddr, &refcr[i], &refsig[i]);
				dr[i] = 100.*(crz[i-15]-refcr[i])/refcr[i];
				if (fabs(dr[i])>drmax) {
					outofspec[i] = TRUE;
					nextra++;
				} 
			}
			if (nextra) status = OUT_OF_SPEC;
			else status = OK;
			fclose(fref);
		
			// check whether rate distribution is compliant to Poisson statistics (not for 39.01 data)
			if (info->pkttype != 3901) {
				maxpoisdev = conf->GetValue(".poisson.rate.deviation.", 0.);
				for (int i=0; i<15; i++) {
					if (fabs(sigx[i]*sigx[i]/crx[i]-1.) > maxpoisdev) {
						outofspec[i] = TRUE;
						npois++;
					}
				}
				for (int i=15; i<30; i++) {
					if (fabs(sigz[i-15]*sigz[i-15]/crz[i-15]-1.) > maxpoisdev) {
						outofspec[i] = TRUE;
						npois++;
					}
				}
				if (npois) status = OUT_OF_SPEC;
			}
		} else status = REF_FILE_NOT_FOUND;
	} else status = RUN_INFO_NOT_SET;	
	
	return;
}

//
//--------------------------------------------------------------------
//

void MCVCountRate::PrintResults()
{
	printf("\n--------------------- COUNT RATE ANALYSIS ----------------------\n");
	if (info) printf("run\t\t %d\ntotal rate\t %.2f evt/s\n", info->runid, info->nevents / info->teff);
	if (status == OK || status == OUT_OF_SPEC) {
		printf("reference run\t %d\n\n", refrun);
		printf("plane\t bar\t rate\t sigma\t sigma^2/rate\t ref_rate\t ref_sigma\t delta_rate (%)\n"); 
		for (int i=0; i<15; i++) {
			printf("X\t %6d\t %.4f\t %.4f\t %.3f\t\t %.4f\t\t %.4f\t\t %.1f\t\t", i+1, crx[i], sigx[i], sigx[i]*sigx[i]/crx[i], refcr[i], refsig[i], dr[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		for (int i=15; i<30; i++) {
			printf("Z\t %6d\t %.4f\t %.4f\t %.3f\t\t %.4f\t\t %.4f\t\t %.1f\t\t", i-14, crz[i-15], sigz[i-15], sigz[i-15]*sigz[i-15]/crz[i-15], refcr[i], refsig[i], dr[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		if (nextra) cout << endl << "WARNING: " << nextra << " bars with delta_rate > " << drmax << " %" << endl;
		if (npois)  cout << endl << "WARNING: " << npois << " bars with anomalous sigma on count rate distribution" << endl;
		return;
	}
	
	if (status != NO_CALC) {
		printf("\nplane\t bar\t rate (evt/s)\t sigma\t sigma^2/rate\n"); 
		for (int i=0; i<15; i++) {
			printf("X\t %6d\t\t %.4f\t %.4f\t %.3f\t\t", i+1, crx[i], sigx[i], sigx[i]*sigx[i]/crx[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		for (int i=0; i<15; i++) {
			printf("Z\t %6d\t\t %.4f\t %.4f\t %.3f\t\t", i+1, crz[i], sigz[i], sigz[i]*sigz[i]/crz[i]);
			if (outofspec[i+15]) printf("*****\n");
			else printf("\n");
		}
		if (npois)  cout << endl << "WARNING: " << npois << " bars with anomalous sigma" << endl;
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVCountRate::SaveToFile()	
{	
	if (conf) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".count.rate.path.", "./");
		outname += conf->GetValue(".count.rate.template.", "count_rate_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing calculated count rate to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "run\t %d\nrate\t %.2f\n", info->runid, info->nevents / info->teff);
			fprintf(fout, "plane\t bar\t rate (evt/s)\t sigma\n"); 
			for (int i=0; i<15; i++) fprintf(fout, "X\t %6d\t\t %.4f\t %.4f\n", i+1, crx[i], sigx[i]);
			for (int i=0; i<15; i++) fprintf(fout, "Z\t %6d\t\t %.4f\t %.4f\n", i+1, crz[i], sigz[i]);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	}
	return;
}