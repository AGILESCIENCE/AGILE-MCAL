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

#include "MCVElectricalCalib.h"
ClassImp(MCVElectricalCalib);

MCVElectricalCalib::MCVElectricalCalib() : MCVStdAnalysis()
{
	thr = 100;
	pnum = 2;

}

//
//--------------------------------------------------------------------
//

MCVElectricalCalib::MCVElectricalCalib(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	thr = 100;
	pnum = 2;
	for (int i=0; i<60; i++) {
		p1cen[i] = 0;
		p1sig[i] = 0;
		p1amp[i] = 0;
		p2cen[i] = 0;
		p2sig[i] = 0;
		p2amp[i] = 0;
		r1cen[i] = 0;
		r1sig[i] = 1.;
		r2cen[i] = 0;
		r2sig[i] = 1.;
		p1nevt[i] = 0;
		p2nevt[i] = 0;
	}
	nextraoff1 = 0;
	nextrasig1 = 0;
	nextraoff2 = 0;
	nextrasig2 = 0;
	nminevt1 = 0;
	nminevt2 = 0;
}

//
//--------------------------------------------------------------------
//

MCVElectricalCalib::~MCVElectricalCalib()
{

}

//
//--------------------------------------------------------------------
//

int MCVElectricalCalib::Calculate()
{
	TObjArray *    rPDA=NULL;
	TObjArray *    rPDB=NULL;
	
	if (rawHisto==NULL && info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info->pkttype!=3910) {
		status = WRONG_PKTTYPE;
		return status;
	}
	rPDA =(TObjArray *) rawHisto->FindObject("rawPDA");
	rPDB =(TObjArray *) rawHisto->FindObject("rawPDB");
	if (rPDA==NULL || rPDB==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	
	for (int i=0; i<30; i++) {	// working on PD-A
		TH1F *aux = (TH1F *) rPDA->At(i);
		Int_t nfound = FitPeaks(i, aux, info->oA[i]+thr, 4096);
	}
	
	for (int i=0; i<30; i++) {	// working on PD-B
		TH1F *aux = (TH1F *) rPDB->At(i);
		Int_t nfound = FitPeaks(i+30, aux, info->oB[i] + thr, 4096);

	}
	
	// rework unresolved peaks
	float p1avg;
	int n1=0;
	for (int i=0; i<60; i++) { // get average peak 1 position
		if (p1cen[i]>0) {
			n1++;
			p1avg += p1cen[i];
		}
	}
	p1avg /= n1;
	
	float p2avg;
	int n2=0;
	for (int i=0; i<60; i++) { // get average peak 2 position
		if (p2cen[i]>0) {
			n2++;
			p2avg += p2cen[i];
		}
	}
	p2avg /= n2;
	
	for (int i=0; i<60; i++) {
		if (p1cen[i]==0 || p2cen[i]==0) {
			TH1F *aux;
			if (i<30) aux = (TH1F *) rPDA->At(i);
			else aux = (TH1F *) rPDB->At(i-30);
			Int_t nfound = FitPeaks(i, aux, p1avg*0.8, p2avg*1.2);
			if (nfound==0) cout << "Still cannot find peaks for PD " << i << " between " << p1avg*0.8 << " and " << p2avg*1.2 << endl;
		}
	}
	
	// load reference values and make comparison
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info) {
		if (!refname) {
			refname = new TString();
			refname->Append(getenv("MCALSW"));
			if (p1avg > 1000.) refname->Append(conf->GetValue(".elcal.reference.setup1.", "ref1.txt"));
			else refname->Append(conf->GetValue(".elcal.reference.setup2.", "ref2.txt"));
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			char 	plane[3], PD[3];
			int	baddr;
			float 	ampl;
			fscanf(fref, "run %d\n", &refrun);
			fscanf(fref, "plane 	bar 	PD 	C1 (ch)	sigma1 	C2 (ch)	sigma2\n");
			for (int i=0; i<60; i++) fscanf(fref, "%s %d %s %f %f %f %f", &plane, &baddr, &PD, &r1cen[i], &r1sig[i], &r2cen[i], &r2sig[i]);
			fclose(fref);
			
			nextraoff1 = 0;
			nextrasig1 = 0;
			nextraoff2 = 0;
			nextrasig2 = 0;
			maxoffdev = conf->GetValue(".elcal.center.deviation.", 0.);
			maxsigdev = conf->GetValue(".elcal.sigma.deviation.", 0.);
			minevents = conf->GetValue(".elcal.minimum.events.", 500.);
			for (int i=0; i<60; i++) {
				if (100.*fabs(p1cen[i]/r1cen[i] - 1.) > maxoffdev) {
					nextraoff1++;
					outofspec[i] = TRUE;
				}
				if (100.*(p1sig[i]/r1sig[i] - 1.) > maxsigdev) {
					nextrasig1++;
					outofspec[i] = TRUE;
				}
				if (100.*fabs(p2cen[i]/r2cen[i] - 1.) > maxoffdev) {
					nextraoff2++;
					outofspec[i] = TRUE;
				}
				if (100.*(p2sig[i]/r2sig[i] - 1.) > maxsigdev) {
					nextrasig2++;
					outofspec[i] = TRUE;
				}
				if (p1nevt[i] < minevents) {
					nminevt1++;
					outofspec[i] = TRUE;
				}
				if (p2nevt[i] < minevents) {
					nminevt2++;
					outofspec[i] = TRUE;
				}
			}
			if (nextrasig1 == 0 && nextraoff1 == 0 && nextrasig2 == 0 && nextraoff2 == 0) status = OK;
			else status = OUT_OF_SPEC;
		} else 	status = REF_FILE_NOT_FOUND;
	} else status = RUN_INFO_NOT_SET;
	return status;
}

//
//--------------------------------------------------------------------
//

int MCVElectricalCalib::FitPeaks(int ipd, TH1F *aux, float xmin, float xmax)
{
	double 	par[3], par2[3]; 
	char fname[20];
	
	aux->SetAxisRange( xmin, xmax, "X");
	TSpectrum *s = new TSpectrum(pnum);
	Int_t nfound = s->Search(aux,1,"goff", 0.2); //"new"
	if (nfound == 0) {
		cout << "PD " << ipd << ": no peak found" << endl;
		return 0;
	} else {
		Float_t *xpeaks = (Float_t*) s->GetPositionX();
	
		// fit peak 1
		par[1] = aux->GetXaxis()->FindBin(xpeaks[0]);
		par[0] = aux->GetBinContent(par[1]);
		par[2] = 2.;
		sprintf(fname, "peak_%d_1", ipd);
		peak  = new TF1(fname,"gaus", par[1]-20, par[1]+20 );
		peak->SetParameters(par);
		peak->SetLineWidth(1);
		peak->SetLineColor(2);
		aux->Fit(fname,"QR0+");
		peak->GetParameters(par);
		p1cen[ipd] = (float) par[1];
		p1sig[ipd] = (float) par[2];
		p1amp[ipd] = (float) par[0];
		p1nevt[ipd] = aux->Integral(p1cen[ipd]-4*p1sig[ipd], p1cen[ipd]+4*p1sig[ipd]);
		
		// fit peak 2
		if (nfound > 1) {
			par2[1] = aux->GetXaxis()->FindBin(xpeaks[1]);
			par2[0] = aux->GetBinContent(par2[1]);
			par2[2] = 2.;
			sprintf(fname, "peak_%d_2", ipd);
			peak2  = new TF1(fname,"gaus", par2[1]-20, par2[1]+20 );
			peak2->SetParameters(par2);
			peak2->SetLineWidth(1);
			peak2->SetLineColor(2);
			aux->Fit(fname,"QR0+");
			peak2->GetParameters(par2);
			p2cen[ipd] = (float) par2[1];
			p2sig[ipd] = (float) par2[2];
			p2amp[ipd] = (float) par2[0];
			p2nevt[ipd] = aux->Integral(p2cen[ipd]-4*p2sig[ipd], p2cen[ipd]+4*p2sig[ipd]);
		}
		
		// plot peaks
		if (c1 && ipd<30) {
			c1->cd(ipd+1);
			peak->Draw("same");
			if (nfound > 1) peak2->Draw("same");
			aux->SetAxisRange( par[1]-50, nfound > 1? par2[1]+50 : par[1]+50, "X");
			c1->Update();
		} else if  (c2 && ipd>=30) {
			c2->cd(ipd-29);
			peak->Draw("same");
			if (nfound > 1) peak2->Draw("same");
			aux->SetAxisRange( par[1]-50, nfound > 1? par2[1]+50 : par[1]+50, "X");
			c2->Update();
		} else {
			delete peak;
			delete peak2;
		}
	}
	return nfound;
}

//
//--------------------------------------------------------------------
//

void MCVElectricalCalib::PrintResults()
{
	printf("\n--------------------- ELECTRICAL CALIBRATION ANALYSIS ----------------------\n");
	if (status == OK || status == OUT_OF_SPEC) {
		printf("\nreference run \t%d\n", refrun);
		printf("\n\t\t\treference 1 \tpeak 1\n");
		printf("plane \tbar \tPD \tC (ch) \tsigma \tC (ch) \tsigma \tn. evt \tdelta C (%) \tdelta sigma (%)\n");
		char 	plane[3], PD[3];
		for (int i=0; i<60; i++) {
			if (i<30) sprintf(PD, "A");
			else sprintf(PD, "B");
			if (i/15 == 0 || i/15 == 2) sprintf(plane, "X");
			else sprintf(plane, "Z");
			printf("%s \t%d \t%s \t%6.1f \t%6.2f \t%6.1f \t%.2f  \t%6.0f \t%6.1f \t\t%6.1f\t\t", plane, i%15 + 1, PD, r1cen[i], r1sig[i], p1cen[i], p1sig[i], p1nevt[i], 100.*(p1cen[i]/r1cen[i]- 1.), 100.*(p1sig[i]/r1sig[i] - 1.));
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		
		printf("\n\t\t\treference 2 \tpeak 2\n");
		printf("plane \tbar \tPD \tC (ch) \tsigma \tC (ch) \tsigma \tn. evt \tdelta C (%) \tdelta sigma (%)\n");
		for (int i=0; i<60; i++) {
			if (i<30) sprintf(PD, "A");
			else sprintf(PD, "B");
			if (i/15 == 0 || i/15 == 2) sprintf(plane, "X");
			else sprintf(plane, "Z");
			printf("%s \t%d \t%s \t%6.1f \t%6.2f \t%6.1f \t%.2f  \t%6.0f \t%6.1f \t\t%6.1f\t\t", plane, i%15 + 1, PD, r2cen[i], r2sig[i], p2cen[i], p2sig[i], p2nevt[i], 100.*(p2cen[i]/r2cen[i]-1.), 100.*(p2sig[i]/r2sig[i] - 1.));
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		if (nextraoff1) cout << endl << "WARNING: " << nextraoff1 << " PDs with delta_center > " << maxoffdev << " ch for peak 1" << endl;
		if (nextrasig1) cout << endl << "WARNING: " << nextrasig1 << " PDs with delta_sigma > "<< maxsigdev << " % for peak 1" << endl;
		if (nextraoff2) cout << endl << "WARNING: " << nextraoff2 << " PDs with delta_center > " << maxoffdev << " ch for peak 2" << endl;
		if (nextrasig2) cout << endl << "WARNING: " << nextrasig2 << " PDs with delta_sigma > "<< maxsigdev << " % for peak 2" << endl;
		if (nminevt1) cout << endl << "WARNING: " << nminevt1 << " PDs with # of events < " << minevents << " for peak 1" << endl;
		if (nminevt2) cout << endl << "WARNING: " << nminevt2 << " PDs with # of events < " << minevents << " for peak 2" << endl;
	} else cout << status << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVElectricalCalib::SaveToFile()
{	
	char 	plane[3], PD[3];
	if (conf) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".elcal.data.path.", "./");
		outname += conf->GetValue(".elcal.data.template.", "elcal_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing electrical calibration peaks data to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "run \t%d\n", info->runid);
			fprintf(fout, "plane \tbar \tPD \tC1 (ch)\tsigma1 \tC2 (ch)\tsigma2\n");
			for (int i=0; i<60; i++) {
				if (i<30) sprintf(PD, "A");
				else sprintf(PD, "B");
				if (i/15 == 0 || i/15 == 2) sprintf(plane, "X");
				else sprintf(plane, "Z");
				fprintf(fout, "%s \t%d \t%s \t%6.1f \t%6.2f \t%6.1f \t%6.2f\n", plane, i%15 + 1, PD, p1cen[i], p1sig[i], p2cen[i], p2sig[i]);
			}			
			fclose(fout);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	}
	return;
}
