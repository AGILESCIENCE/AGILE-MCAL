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

#include "MCVGridOffset.h"
ClassImp(MCVGridOffset);

MCVGridOffset::MCVGridOffset() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVGridOffset::MCVGridOffset(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{

}

//
//--------------------------------------------------------------------
//

MCVGridOffset::~MCVGridOffset()
{
//	if (tdata) delete tdata;
}

//
//--------------------------------------------------------------------
//

int MCVGridOffset::Calculate()
{
	TObjArray *    rPDA=NULL;
	TObjArray *    rPDB=NULL;
	double 	par[3]; 
	char fname[20];
	
	if (rawHisto==NULL && info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info->pkttype!=3902) {
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
		if (c1) c1->cd(i+1);
		TH1F *aux = (TH1F *) rPDA->At(i);
   		TSpectrum *s = new TSpectrum(1);
   		Int_t nfound = s->Search(aux,1,"new");
		if (nfound == 0) {
			par[1] = info->oA[i];
			//cout << "*** bar " << i << " - PD-B - NO peak found!" << endl;
			//groff[i] = 0.;
			//grsig[i] = 0.;
			//gramp[i] = 0.;
			//continue;
		} else {
			Float_t *xpeaks = (Float_t*) s->GetPositionX();
			//cout << "bar " << i << " - PD-A - found peak at ch " << xpeaks[0] << endl;
			par[1] = aux->GetXaxis()->FindBin(xpeaks[0]);
		}
      		par[0] = aux->GetBinContent(par[1]);
		par[2] = 2.;
		sprintf(fname, "peakoffset_%d_A", i);
		peak  = new TF1(fname,"gaus", par[1]-20, par[1]+20 );
		peak->SetParameters(par);
		peak->SetLineWidth(1);
		peak->SetLineColor(2);
   		aux->Fit(fname,"QR0+");
		peak->GetParameters(par);
		groff[i] = (float) par[1];
		grsig[i] = (float) par[2];
		gramp[i] = (float) par[0];
		if (c1) {
			peak->Draw("same");
			aux->SetAxisRange( par[1]-20, par[1]+20, "X");
			c1->Update();
		} else delete peak;
	}
	
	for (int i=0; i<30; i++) {	// working on PD-B
		if (c2) c2->cd(i+1);
		TH1F *aux = (TH1F *) rPDB->At(i);
   		TSpectrum *s = new TSpectrum(1);
   		Int_t nfound = s->Search(aux,1,"new");
		if (nfound == 0) {
			par[1] = info->oB[i];
			//cout << "*** bar " << i << " - PD-B - NO peak found!" << endl;
			//groff[i+30] = 0.;
			//grsig[i+30] = 0.;
			//gramp[i+30] = 0.;
			//continue;
		} else {
			Float_t *xpeaks = (Float_t*)  s->GetPositionX();
			//cout << "bar " << i << " - PD-B - found peak at ch " << xpeaks[0] << endl;
			par[1] = aux->GetXaxis()->FindBin(xpeaks[0]);
		}
      		par[0] = aux->GetBinContent(par[1]);
		par[2] = 2.;
		sprintf(fname, "peakoffset_%d_B", i);
		peak  = new TF1(fname,"gaus", par[1]-20, par[1]+20 );
		peak->SetParameters(par);
		peak->SetLineWidth(1);
		peak->SetLineColor(2);
   		aux->Fit(fname,"QR0+");
		peak->GetParameters(par);
		groff[i+30] = par[1];
		grsig[i+30] = par[2];
		gramp[i+30] = par[0];
		if (c2) {
			peak->Draw("same");
			aux->SetAxisRange( par[1]-20, par[1]+20, "X");
			c2->Update();
		} else delete peak;
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
			refname->Append(conf->GetValue(".grid.offset.reference.", "ref.txt"));
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			char 	plane[3], PD[3];
			int	baddr;
			float 	ampl;
			fscanf(fref, "plane	 bar	 PD	 offset		 sigma	 ampl\n");
			for (int i=0; i<60; i++) fscanf(fref, "%s %d %s %f %f %f", &plane, &baddr, &PD, &refoff[i], &refsig[i], &ampl);
			fclose(fref);
			
			nextraoff = 0;
			nextrasig = 0;
			maxoffdev = conf->GetValue(".grid.offset.deviation.", 0.);
			maxsigdev = conf->GetValue(".grid.sigma.deviation.", 0.);
			for (int i=0; i<60; i++) {
				if (fabs(groff[i]-refoff[i]) > maxoffdev) {
					nextraoff++;
					outofspec[i] = TRUE;
				}
				if (100.*(grsig[i]/refsig[i] - 1.) > maxsigdev) {
					nextrasig++;
					outofspec[i] = TRUE;
				}
			}
			if (nextrasig == 0 && nextraoff == 0) status = OK;
			else status = OUT_OF_SPEC;
		} else 	status = REF_FILE_NOT_FOUND;
	} else status = RUN_INFO_NOT_SET;
		
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVGridOffset::PrintResults()
{
	printf("\n--------------------- GRID OFFSET ANALYSIS ----------------------\n");
	if (status == OK || status == OUT_OF_SPEC) {
		cout << endl << "Comparison between run " << info->runid << " and reference file " << refname->Data() << endl;
		printf("\nplane \tbar \tPD \told C (ch) \told sigma \tnew C (ch) \tnew sigma \tdelta C (ch) \tdelta sigma (%)\n");
		char 	plane[3], PD[3];
		for (int i=0; i<60; i++) {
			if (i<30) sprintf(PD, "A");
			else sprintf(PD, "B");
			if (i/15 == 0 || i/15 == 2) sprintf(plane, "X");
			else sprintf(plane, "Z");
			printf("%s \t%d \t%s \t%.3f \t%.3f \t\t%.3f \t%.3f \t\t%7.3f \t%.3f\t\t", plane, i%15 + 1, PD, refoff[i], refsig[i], groff[i], grsig[i], refoff[i]-groff[i], 100.*(grsig[i]/refsig[i] - 1.));
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		
		// evaluate and print mean offset and sigma variation per plane
		avgXAdoff=0.;
		avgXAdsig=0.;
		avgXBdoff=0.;
		avgXBdsig=0.;
		avgZAdoff=0.;
		avgZAdsig=0.;
		avgZBdoff=0.;
		avgZBdsig=0.;
		for (int i=0; i<15; i++) {
			avgXAdoff += refoff[i]-groff[i];
			avgXAdsig += 100.*(grsig[i]/refsig[i] - 1.);
			avgZAdoff += refoff[i+15]-groff[i+15];
			avgZAdsig += 100.*(grsig[i+15]/refsig[i+15] - 1.);
			avgXBdoff += refoff[i+30]-groff[i+30];
			avgXBdsig += 100.*(grsig[i+30]/refsig[i+30] - 1.);
			avgZBdoff += refoff[i+45]-groff[i+45];
			avgZBdsig += 100.*(grsig[i+45]/refsig[i+45] - 1.);
		}
		avgXAdoff /= 15.;
		avgXAdsig /= 15.;
		avgXBdoff /= 15.;
		avgXBdsig /= 15.;
		avgZAdoff /= 15.;
		avgZAdsig /= 15.;
		avgZBdoff /= 15.;
		avgZBdsig /= 15.;
		printf("side XA: mean offset variation = %8.2f ch - mean sigma variation = %8.2f %%\n", avgXAdoff, avgXAdsig);
		printf("side ZA: mean offset variation = %8.2f ch - mean sigma variation = %8.2f %%\n", avgZAdoff, avgZAdsig);
		printf("side XB: mean offset variation = %8.2f ch - mean sigma variation = %8.2f %%\n", avgXBdoff, avgXBdsig);
		printf("side ZB: mean offset variation = %8.2f ch - mean sigma variation = %8.2f %%\n", avgZBdoff, avgZBdsig);
		
		if (nextraoff) cout << endl << "WARNING: " << nextraoff << " PDs with delta_offset > " << maxoffdev << " ch" << endl;
		if (nextrasig) cout << endl << "WARNING: " << nextrasig << " PDs with delta_sigma > "<< maxsigdev << " %" << endl;
		return;
	}
	
	if (status != NO_CALC) {
		printf(" PD\t center\t sigma\t ampl\n");
		for (int i=0; i<60; i++) printf("%d\t %6.2f\t %6.2f\t %6.0f \n", i, groff[i], grsig[i], gramp[i]);
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVGridOffset::SaveToFile()	
{	
	if (conf) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".grid.offset.path.", "./");
		outname += conf->GetValue(".grid.offset.template.", "grid_offset_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing calculated offset to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "plane\t bar\t PD\t offset\t\t sigma\t ampl\n"); 
			for (int i=0; i<30; i++) {	// working on PD-A
				if (fout) {
					if (i<15) fprintf(fout, "X\t %.0d\t A\t %.3f\t %.3f\t %.1f\n", i+1, groff[i], grsig[i], gramp[i]);
					else  fprintf(fout, "Z\t %.0d\t A\t %.3f\t %.3f\t %.1f\n", i-14, groff[i], grsig[i], gramp[i]);
				}
			}
			for (int i=0; i<30; i++) {	// working on PD-B
				if (fout) {
					if (i<15) fprintf(fout, "X\t %.0d\t B\t %.3f\t %.3f\t %.1f\n", i+1, groff[i+30], grsig[i+30], gramp[i+30]);
					else  fprintf(fout, "Z\t %.0d\t B\t %.3f\t %.3f\t %.1f\n", i-14, groff[i+30], grsig[i+30], gramp[i+30]);
				}
			}
			fclose(fout);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVGridOffset::UpdateGlobalFile()
{
	float t0, dt;
	int nold;
	TGraphErrors	*aux;
	
	// open ROOT data file
	TString outname;
	outname.Append(getenv("MCALSW"));
	outname += conf->GetValue(".global.history.file.", "./global.root");
	TFile *fh = new TFile(outname.Data(), "UPDATE");  
	
	// retrieve or create graph  
	t0 = (info->tstart + info->tstop)/2.;
	dt = (info->tstop - info->tstart)/2.;
	
	aux = NULL;
	nold = 0;
	fh->GetObject("dsXA", aux);
	if (aux == NULL) {
		aux = CreateGraphErrors("dsXA");
		aux->GetXaxis()->SetTitle("Time (s)");
		aux->GetXaxis()->SetTitleSize(0.05);
		aux->GetXaxis()->CenterTitle();
		aux->GetYaxis()->SetTitle("Variation from reference (%)");
		aux->GetYaxis()->CenterTitle();
		aux->GetYaxis()->SetTitleSize(0.05);
	}
	nold = aux->GetN();
	aux->Set(nold+1);
	aux->SetPoint(nold, t0, avgXAdsig);
	aux->SetPointError(nold, dt, 0.);
	aux->Write("dsXA", TObject::kOverwrite);
	
	aux = NULL;
	nold = 0;
	fh->GetObject("dsZA", aux);
	if (aux == NULL) {
	aux = CreateGraphErrors("dsZA");
		aux->GetXaxis()->SetTitle("Time (s)");
		aux->GetXaxis()->SetTitleSize(0.05);
		aux->GetXaxis()->CenterTitle();
		aux->GetYaxis()->SetTitle("Variation from reference (%)");
		aux->GetYaxis()->CenterTitle();
		aux->GetYaxis()->SetTitleSize(0.05);
	}
	nold = aux->GetN();
	aux->Set(nold+1);
	aux->SetPoint(nold, t0, avgZAdsig);
	aux->SetPointError(nold, dt, 0.);
	aux->Write("dsZA", TObject::kOverwrite);
	
	aux = NULL;
	nold = 0;
	fh->GetObject("dsXB", aux);
	if (aux == NULL) {
		aux = CreateGraphErrors("dsXB");
		aux->GetXaxis()->SetTitle("Time (s)");
		aux->GetXaxis()->SetTitleSize(0.05);
		aux->GetXaxis()->CenterTitle();
		aux->GetYaxis()->SetTitle("Variation from reference (%)");
		aux->GetYaxis()->CenterTitle();
		aux->GetYaxis()->SetTitleSize(0.05);
	}
	nold = aux->GetN();
	aux->Set(nold+1);
	aux->SetPoint(nold, t0, avgXBdsig);
	aux->SetPointError(nold, dt, 0.);
	aux->Write("dsXB", TObject::kOverwrite);
	
	aux = NULL;
	nold = 0;
	fh->GetObject("dsZB", aux);
	if (aux == NULL) {
		aux = CreateGraphErrors("dsZB");
		aux->GetXaxis()->SetTitle("Time (s)");
		aux->GetXaxis()->SetTitleSize(0.05);
		aux->GetXaxis()->CenterTitle();
		aux->GetYaxis()->SetTitle("Variation from reference (%)");
		aux->GetYaxis()->CenterTitle();
		aux->GetYaxis()->SetTitleSize(0.05);
	}
	nold = aux->GetN();
	aux->Set(nold+1);
	aux->SetPoint(nold, t0, avgZBdsig);
	aux->SetPointError(nold, dt, 0.);
	aux->Write("dsZB", TObject::kOverwrite);
	
	fh->Close();
	return;
}

//
//--------------------------------------------------------------------
//

void MCVGridOffset::UpdateCalibrationFile()
{
	char parameterName[100], name[100];
	if (status==OK || status==OUT_OF_SPEC) {
		TString newname;
		newname.Append(getenv("MCALSW"));
		newname += conf->GetValue(".modified.calibration.path.", "./");
		newname += conf->GetValue(".updated.grid.template.", "grid_test_");
		int id=0;
		for (id=1; id<=1000; id++) {
			sprintf(name, "%s%d.conf", newname.Data(), id);
			fout = fopen(name, "r");
			if (fout) fclose(fout);
			else break;
		}
		newname += id;
		newname += ".conf";
		
		char *buffer;
		long size;
		
		TString oldname;
		oldname.Append(getenv("MCALSW"));
		oldname += conf->GetValue(".grid.calibration.file.", "grid.conf");
		ifstream infile (oldname.Data(),ifstream::binary);
		ofstream outfile (newname.Data(),ofstream::binary);
		
		// get size of file
		infile.seekg(0,ifstream::end);
		size=infile.tellg();
		infile.seekg(0);
		
		// allocate memory for file content
		buffer = new char[size];
		
		// read content of infile
		infile.read(buffer,size);
		
		// write to outfile
		outfile.write(buffer,size);
		
		// release dynamically-allocated memory
		delete[] buffer;
		outfile.close();
		infile.close();
		
		TEnv *newconf = new TEnv(newname.Data());
		for (int i=0; i<30; i++) {
			sprintf(parameterName, ".Offset.Bar%02d.A", (i<15 ? i : i+1));
			newconf->SetValue(parameterName, groff[i]);  
			sprintf(parameterName, ".Offset.Bar%02d.B", (i<15 ? i : i+1));
			newconf->SetValue(parameterName, groff[i+30]);  
		}
		newconf->SetValue(".ReferenceRun", info->runid);
		newconf->Save();
		cout << "Saved new GRID Calibration with updated offsets to file: " << newname.Data() << endl;
		delete newconf;
	} else PrintComment();
	return;
}
