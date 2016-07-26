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

#include "MCVFitPeak.h"
ClassImp(MCVFitPeak);

MCVFitPeak::MCVFitPeak() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVFitPeak::MCVFitPeak(double E, TObjArray *hc, MCVFileInfo *in, int Earr) : MCVStdAnalysis(hc, in)
{
	Et = E;
	dEhi = 0.4;
	dElo = 0.3;
	isEarray = Earr;
}

//
//--------------------------------------------------------------------
//

MCVFitPeak::~MCVFitPeak()
{

}

//
//--------------------------------------------------------------------
//

int MCVFitPeak::Calculate()
{
	TObjArray *    rE=NULL;
	double 	par[5], par1[3]; 
	double 	dE = 0.2; 
	TF1 *peak;
	TF1 *bkg;
	TF1 *total;
	double cnst = sqrt(2*TMath::Pi())/0.020;
	
	if (rawHisto==NULL || info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (isEarray) rE = rawHisto;	// this is for call via MCVBkgSubtraction
	else rE =(TObjArray *) rawHisto->FindObject("rawE");
	if (rE==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	unsigned long int bid=1;
	for (int i=0; i<30; i++) {	// working on Energy histo
		if (barmask & bid) {
			if (c1) c1->cd(i+1);
			aux = (TH1F *) rE->At(i);
			FindPeak();
			cout << i << "   " << Ei << endl;
			ibin = aux->GetXaxis()->FindBin(Et);
			par1[0] = aux->GetBinContent(ibin);
	//		Ei=Et;	// To be commented when using FindPeak()
			par1[1] = Ei; 
			par1[2] = 0.060;
			peak  = new TF1("peak","gaus", Ei-0.2,  Ei+0.2); // (int) 0.1
			bkg   = new TF1("bkg", "expo", Ei-dElo,  Ei+dEhi);
	//		total = new TF1("total","expo(0)+gaus(2)", Ei-dElo,  Ei+dEhi);
			total = new TF1("total","gaus(2)", Ei-0.06,  Ei+0.2);
			peak->SetParameters(par1);
			aux->Fit("peak","QR0+");
			aux->Fit("bkg","QR0+");
			peak->GetParameters(&par[2]);
			bkg->GetParameters(&par[0]);
			total->SetParameters(par);
			aux->Fit("total","QR0+");
			total->GetParameters(par);
			pa[i] = par[2];
			pc[i] = par[3];
			ps[i] = fabs(par[4]);
			pr[i] = pa[i]*ps[i]*cnst;
			rki2[i] = total->GetChisquare()/((dEhi+dElo)/0.020);
			
			if (c1) {
				//aux->SetAxisRange(Ei-dElo,  Ei+dEhi, "X");
				total->SetLineWidth(2);
				total->SetLineColor(2);
				total->Draw("same");
			/*	
				peak->SetRange(Ei-dElo,  Ei+dEhi);
				peak->SetParameters(&par[2]);
				peak->SetLineWidth(1);
				peak->SetLineColor(3);
				peak->Draw("same");
				
				bkg->SetParameters(par);
				bkg->SetLineWidth(1);
				bkg->SetLineColor(4);
				bkg->Draw("same");
				c1->Update();*/
			} 
		}
		bid << 1;
	}
	status = OK;
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVFitPeak::PrintResults()
{
	printf("\n--------------------- FITTING PEAKS AT E = %.3f MeV ----------------------\n", Et);
	printf("\nplane \tbar \tcenter (MeV) \tsigma (MeV) \tampl \tcounts \tred. Ki^2 \tdeviation (%)\n");
	for (int i=0; i<15; i++) printf("X \t%d \t%.3f \t\t%.3f \t\t%.3f \t%.2f  \t%.2f \t\t%.2f\n", i+1, pc[i], ps[i], pa[i], pr[i], rki2[i], 100.*(pc[i]/Et -1.));
	for (int i=15; i<30; i++) printf("Z \t%d \t%.3f \t\t%.3f \t\t%.3f \t%.2f  \t%.2f \t\t%.2f\n", i-14, pc[i], ps[i], pa[i], pr[i], rki2[i], 100.*(pc[i]/Et -1.));
	printf("\n--------------------------------------------------------------------------\n", Et);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFitPeak::FindPeak()
{
	// find expected peak in energy histogram. use this method because class TSpectrum does not work here!
	float y, ypmax, ynmax;
	int ipmax, inmax, ip, in, nlop, nlon, nstep;
	int ini = aux->GetXaxis()->FindBin(Et);
	float yini = aux->GetBinContent(ini);
	double c;
	ibin = ini;
	Ei = yini;
	
	ip=ini + 1;
	nlop = 0;
	ypmax = ynmax = yini;
	ipmax = inmax = ini;
	nstep=10;
	do {
		y = aux->GetBinContent(ip);
		if (y>ypmax) {
			ipmax = ip;
			ypmax = y;
			if (nlop>0) nlop--;
		} else nlop++;
		if (nlop==1) nstep+=2;
		ip++;
	} while (nlop<3 && (ip-ini)<nstep);
	
	in = ini-1;
	nlon = 0;
	nstep=12;
	do {
		y = aux->GetBinContent(in);
		if (y>ynmax) {
			inmax = in;
			ynmax = y;
			if (nlon>0) nlon--;
		} else nlon++;
		if (nlon==1) nstep+=2;
		in--;	
	} while (nlon<3 && (ini-in)<nstep);
//	printf("inmax=%d \tynmax=%f\t nlon=%d \tipmax=%d \typmax=%f\t nlop=%d \n", inmax, ynmax, nlon, ipmax, ypmax, nlop);
	
	if (nlop>0 && nlon>0) {
		if (ynmax > ypmax) {
			Ei = aux->GetXaxis()->GetBinCenter(inmax);
			ibin = inmax;
		} else {
			Ei =  aux->GetXaxis()->GetBinCenter(ipmax);
			ibin = ipmax;
		} 			
	} else {
		if (nlop<nlon) {
			Ei = aux->GetXaxis()->GetBinCenter(inmax);
			ibin = inmax;
		} else {
			Ei =  aux->GetXaxis()->GetBinCenter(ipmax);
			ibin = ipmax;
		} 			
	}	
}

//
//--------------------------------------------------------------------
//

void MCVFitPeak::SaveToFile()
{
	char name[100];
	double cnst = sqrt(2*TMath::Pi())/0.020;
	if (status==OK || status==OUT_OF_SPEC) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".fit.peak.path.", "./");
		outname += conf->GetValue(".fit.peak.template.", "peak_");
		outname += info->runid;
		int id=0;
		for (id=1; id<=100; id++) {
			sprintf(name, "%s_%.0fkeV_%d.txt", outname.Data(), Et*1000., id);
			fout = fopen(name, "r");
			if (fout) fclose(fout);
			else break;
		}
		fout = fopen(name, "w");
		cout << "Writing peak fit results to file " << name << endl;
		if (fout) {
			fprintf(fout, "run\t %d\n", info->runid);
			fprintf(fout, "histo\t %s\n", rawHisto->GetName());
			fprintf(fout, "E\t %f\n", Et);
			fprintf(fout, "\nplane \tbar \tcenter (MeV) \tsigma (MeV) \tampl \tcounts \tred. Ki^2 \tdeviation (%)\n");
			for (int i=0; i<15; i++) fprintf(fout, "X \t%d \t%.3f \t\t%.3f \t\t%.3f \t%.2f  \t%.2f \t\t%.2f\n", i+1, pc[i], ps[i], pa[i], pa[i]*ps[i]*cnst, rki2[i], 100.*(pc[i]/Et -1.));
			for (int i=15; i<30; i++) fprintf(fout, "Z \t%d \t%.3f \t\t%.3f \t\t%.3f \t%.2f  \t%.2f \t\t%.2f\n", i-14, pc[i], ps[i], pa[i], pa[i]*ps[i]*cnst, rki2[i], 100.*(pc[i]/Et -1.));
		} else cout << "*** impossible to write file " << name << endl;
		fclose(fout);
	} else PrintComment();
	return;
}

//
//--------------------------------------------------------------------
//

void MCVFitPeak::UpdateFit(int bar, double cen, double sig, double amp, double rk2)
{
	if (bar>=0 && bar<=29) {
		pc[bar] = cen;
		ps[bar] = fabs(sig);
		pa[bar] = amp;
		rki2[bar] = rk2;
	} else cout << "*** Wrong bar number! " << endl;
	return;
}
