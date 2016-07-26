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

#include "MCVGridEnergyCal.h"
ClassImp(MCVGridEnergyCal);

MCVGridEnergyCal::MCVGridEnergyCal() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVGridEnergyCal::MCVGridEnergyCal(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	meanmpv=0.;
}

//
//--------------------------------------------------------------------
//

MCVGridEnergyCal::~MCVGridEnergyCal()
{
	meanmpv=0.;
	meanE=0.;
}

//
//--------------------------------------------------------------------
//

int MCVGridEnergyCal::Calculate()
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
	
		aux->Rebin(20);	// 10
		aux->SetAxisRange(2., 40., "X");
		aux->SetAxisRange(0., 0.08, "Y");
		if (aux->GetEntries()) {
			aux->Scale(info->teff);
			sprintf(peakname, "peak_%d", i);
			peak  = new TF1(peakname,"landau", 8.,  25.); // (int)
			sprintf(bkgname, "bkg_%d", i);
			bkg   = new TF1(bkgname, "expo", 2., 7.);
			sprintf(totname, "total_%d", i);
			total = new TF1(totname,"expo(0)+landau(2)", 2., 35.);
			
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
	if (!conf) {
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
	} else status = RUN_INFO_NOT_SET;
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVGridEnergyCal::PrintResults()
{
	printf("\n--------------------- GRID ENERGY CALIBRATION ----------------------\n");
	printf("\nreference run\t%d\n", refrun);	
	printf("\tcurrent data \t\t\t\t\t\t\treference \t\t\t\t\tdeviations (%)\n");
	printf("bar \tMPV (MeV) \tsigma (MeV)  \tmean (MeV) \trate (evt/s) \tMPV (MeV) \tsigma (MeV) \trate (evt/s) \tMPV (%)\tsig (%)\trate (%)\t\n");
	for (int i=0; i<30; i++) {
		printf("%c%d \t%6.2f \t\t%6.2f \t\t%6.2f \t\t%6.3f \t\t%6.2f \t\t%6.2f \t\t%6.3f \t\t%6.2f \t%6.2f \t%6.2f \t", i<15 ? 'X' : 'Z', i<15 ? i+1 : i-14, mpv[i], sig[i], mE[i], amp[i], refmpv[i], refsig[i], refamp[i], 100.*(mpv[i]/refmpv[i] - 1.), 100.*(sig[i]/refsig[i] - 1.), 100.*(amp[i]/refamp[i] - 1.));
		if (outofspec[i]) printf("*****\n");
		else printf("\n");
	}
	cout << endl << "Calculated mean MPV for muons in MCAL = " << meanmpv << " MeV" << endl;
	if (noutmpv>0) cout << "WARNING: " << noutmpv << " bars with muon MPV deviation from reference > " << maxmpvdev << " %" << endl;
	if (noutsig>0) cout << "WARNING: " << noutsig << " bars with muon sigma deviation from reference > " << maxsigdev << " %" << endl;
	if (noutamp>0) cout << "WARNING: " << noutamp << " bars with muon rate deviation from reference > " << maxampdev << " %" << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVGridEnergyCal::SaveToFile()
{
	if (conf) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".grid.ecal.path.", "./");
		outname += conf->GetValue(".grid.ecal.template.", "gridecal_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing GRID energy calibration data to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "run \t%d\n", info->runid);
			fprintf(fout, "bar \tmuon MPV (MeV) \tsigma (MeV) \tfrequency (evt/s)\n");
			for (int i=0; i<30; i++) {
				if (i<15) fprintf(fout, "X%d \t%6.2f \t\t%6.2f \t\t%6.3f \t\t\n", i+1, mpv[i], sig[i], amp[i]);
				else      fprintf(fout, "Z%d \t%6.2f \t\t%6.2f \t\t%6.3f \t\t\n", i-14, mpv[i], sig[i], amp[i]);
			}
			fclose(fout);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVGridEnergyCal::UpdateCalibrationFile()
{
	char parameterName[100], name[100];
	float refmpv, gfact;
	if (status==OK || status==OUT_OF_SPEC) {
		TString newname;
		newname.Append(getenv("MCALSW"));
		newname += conf->GetValue(".modified.calibration.path.", "./");
		newname += conf->GetValue(".updated.grid-gain.template.", "grid_test_E_");
		int id=0;
/*		for (id=1; id<=1000; id++) {
			sprintf(name, "%s%d.conf", newname.Data(), id);
			fout = fopen(name, "r");
			if (fout) fclose(fout);
			else break;
		}
		newname += id;*/
		
		newname += info->runid;
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
		newconf->SetValue(".GainFactor", 1.);
		
		refmpv = conf->GetValue(".muon.mpv.energy.", 10.);
		cout << "Changing gain parameters to obtain a muon MPV = " << refmpv << endl;
		for (int i=0; i<30; i++) {
			gfact = mpv[i]/refmpv;
//			cout << i << "   " << gfact <<  "   " << info->u0A[i] <<  "   " << endl;
			sprintf(parameterName, ".Gain.Bar%02d.A", (i<15 ? i : i+1));
			newconf->SetValue(parameterName, info->u0A[i]*gfact);  
			sprintf(parameterName, ".Gain.Bar%02d.B", (i<15 ? i : i+1));
			newconf->SetValue(parameterName, info->u0B[i]*gfact);  
		}
		newconf->SetValue(".ReferenceRun", info->runid);
		newconf->Save();
		cout << "Saved new GRID Calibration with updated individual gains to file: " << newname.Data() << endl;
		delete newconf;
	} else PrintComment();
	return;
}
