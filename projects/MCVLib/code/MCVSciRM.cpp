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

#include "MCVSciRM.h"
ClassImp(MCVSciRM);

MCVSciRM::MCVSciRM() : MCVStdAnalysis(), grmX3913(NULL), grmZ3913(NULL), grmX(NULL), grmZ(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVSciRM::MCVSciRM(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in), grmX3913(NULL), grmZ3913(NULL), grmX(NULL), grmZ(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVSciRM::~MCVSciRM()
{

}

//
//--------------------------------------------------------------------
//

int MCVSciRM::Calculate()
{
	if (rawHisto==NULL && info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	grmX3913 =(TGraphErrors *) rawHisto->FindObject("gRMX3913");
	grmZ3913 =(TGraphErrors *) rawHisto->FindObject("gRMZ3913");
	grmX =(TGraphErrors *) rawHisto->FindObject("gRMX");
	grmZ =(TGraphErrors *) rawHisto->FindObject("gRMZ");
	
	hXR =(TH1F *) rawHisto->FindObject("hRMXR");
	hZR =(TH1F *) rawHisto->FindObject("hRMZR");
	hX =(TH1F *) rawHisto->FindObject("sciRMX");
	hZ =(TH1F *) rawHisto->FindObject("sciRMZ");

	if (grmX3913==NULL || grmZ3913==NULL || grmX==NULL || grmZ==NULL) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}
	E 	= grmX3913->GetX();
	E_err 	= grmX3913->GetEX();
	rmX3913 = grmX3913->GetY();
	rmX3913_err = grmX3913->GetEY();
	rmZ3913 = grmZ3913->GetY();
	rmZ3913_err = grmZ3913->GetEY();
	rmX = grmX->GetY();
	rmX_err = grmX->GetEY();
	rmZ = grmZ->GetY();
	rmZ_err = grmZ->GetEY();
	
	// load reference values and make comparison
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (info) {
		if (!refname) {
			refname = new TString();
			refname->Append(getenv("MCALSW"));
			refname->Append(conf->GetValue(".sci.ratemeters.reference.", "ref.txt"));
		}
		fref = fopen(refname->Data(), "r");
		if (fref) {
			nout1X = nout1Z = nout2X = nout2Z = 0;
			int id;
			float loE;
			float hiE;
			fscanf(fref, "run %d\n", &refrun);
			fscanf(fref, "band	 E_min (MeV)	 E_max (MeV)	 rm_X (evt/s)	 sigma_X	 rm_Z (evt/s)	 sigma_Z\n");
			for (int i=1; i<11; i++) {
				fscanf(fref, "%d %f %f %f %f %f %f\n", &id, &loE, &hiE, &refX[i], &refX_err[i], &refZ[i], &refZ_err[i]);
				refE[i] = 0.5*(loE+hiE);
				refE_err[i] = 0.5*(hiE-loE);
			}
			fclose(fref);
			nsigma = conf->GetValue(".sci.ratemeters.deviation.", 1.);
			
			// compares calculated ratemeters / 39.13 ratemeters / reference ratemeters
			for (int i=1; i<11; i++) {
				//dev1X[i] = fabs(rmX3913[i] - rmX[i-1])/rmX3913_err[i];
				//dev2X[i] = fabs(rmX3913[i] - refX[i])/rmX3913_err[i];
				dev1X[i] = 100.*(rmX3913[i] - rmX[i-1])/rmX[i-1];
				dev2X[i] = 100.*(rmX3913[i] - refX[i])/refX[i];
				if (info->pkttype==3909 && fabs(dev1X[i])>nsigma) {
					outofspec[i] = TRUE;
					nout1X++;
				}
				if (fabs(dev2X[i])>nsigma) {
					outofspec[i] = TRUE;
					nout2X++;
				}
				//dev1Z[i] = fabs(rmZ3913[i] - rmZ[i-1])/rmZ3913_err[i];
				//dev2Z[i] = fabs(rmZ3913[i] - refZ[i])/rmZ3913_err[i];
				dev1Z[i] = 100.*(rmZ3913[i] - rmZ[i-1])/rmZ[i-1];
				dev2Z[i] = 100.*(rmZ3913[i] - refZ[i])/refZ[i];
				if (info->pkttype==3909 && fabs(dev1Z[i])>nsigma) {
					outofspec[i+20] = TRUE;
					nout1Z++;
				}
				if (fabs(dev2Z[i])>nsigma) {
					outofspec[i+20] = TRUE;
					nout2Z++;
				}
			}
			if (nout1X || nout1Z || nout2X || nout2Z) status = OUT_OF_SPEC;
			else status = OK;
		} else 	status = REF_FILE_NOT_FOUND;
	} else status = RUN_INFO_NOT_SET;
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVSciRM::PrintResults()
{
	printf("\n--------------------- SCIENTIFIC RATEMETERS ----------------------\n");
	if (info) printf("run\t\t %d\n", info->runid);
	if (status == OK || status == OUT_OF_SPEC) {
		printf("reference run\t %d\n\n", refrun);
		printf("Energy bands:\nband\t E_min (MeV)\t E_max (MeV)\n");
		for (int i=1; i<11; i++) printf("%d\t %10.2f\t %10.2f\n", i, E[i]-E_err[i], E[i]+E_err[i]);
		printf("\n\t calculated RM\t\t\t 39.13 RM\t\t\t reference 39.13 RM\t\t deviation (%%)\n");
		printf("band\t rm_X (evt/s)\t sigma_X\t rm_X (evt/s)\t sigma_X\t rm_X (evt/s)\t sigma_X\t calc/3913\t 3913/ref\n"); 
		for (int i=1; i<11; i++) {
			printf("%d\t %10.2f\t %10.2f\t %10.3f\t %10.3f\t %10.3f\t %10.3f\t %7.1f\t %7.1f\t", i, rmX[i-1], rmX_err[i-1] , rmX3913[i], rmX3913_err[i], refX[i], refX_err[i], dev1X[i], dev2X[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		printf("\n\t calculated RM\t\t\t 39.13 RM\t\t\t reference 39.13 RM\t\t deviation (%%)\n");
		printf("band\t rm_Z (evt/s)\t sigma_Z\t rm_Z (evt/s)\t sigma_Z\t rm_Z (evt/s)\t sigma_Z\t calc/3913\t 3913/ref\n"); 
		for (int i=1; i<11; i++) {
			printf("%d\t %10.2f\t %10.2f\t %10.3f\t %10.3f\t %10.3f\t %10.3f\t %7.1f\t %7.1f\t", i, rmZ[i-1], rmZ_err[i-1] , rmZ3913[i], rmZ3913_err[i], refZ[i], refZ_err[i], dev1Z[i], dev2Z[i]);
			if (outofspec[i+20]) printf("*****\n");
			else printf("\n");
		}
		cout << endl;
		if (nout1X) cout << "WARNING: " << nout1X << " calculeated sci. RM on plane X with deviation from 39.13 > " << nsigma << "%" << endl;
		if (nout2X) cout << "WARNING: " << nout2X << " sci. RM on plane X with deviation from reference > " << nsigma << "%" << endl;
		if (nout1Z) cout << "WARNING: " << nout1Z << " calculeated sci. RM on plane X with deviation from 39.13 > " << nsigma << "%" << endl;
		if (nout2Z) cout << "WARNING: " << nout2Z << " sci. RM on plane Z with deviation from reference > " << nsigma << "%" << endl;
		
		// print comparison between sci.RM algorithm and energy calibration
		printf("\n\t X sci RM\t E. reconstruction\t deviation (%%)\n");
		for (int i=1; i<11; i++) {
			printf("%d\t %10.2f\t %10.2f\t %10.3f\t \n", i, hX->GetBinContent(i+1), hXR->GetBinContent(i+1), 100.*(hXR->GetBinContent(i+1)/hX->GetBinContent(i+1) - 1.));
		}
		printf("\ntot\t %10.2f\t %10.2f\t %10.3f\t \n", hX->Integral(2, 11), hXR->Integral(2, 11), 100.*(hXR->Integral(2, 11)/hX->Integral(2, 11) - 1.));
		printf("\n\t Z sci RM\t E. reconstruction\t deviation (%%)\n");
		for (int i=1; i<11; i++) {
			printf("%d\t %10.2f\t %10.2f\t %10.3f\t \n", i, hZ->GetBinContent(i+1), hZR->GetBinContent(i+1), 100.*(hZR->GetBinContent(i+1)/hZ->GetBinContent(i+1) - 1.));
		}
		printf("\ntot\t %10.2f\t %10.2f\t %10.3f\t \n", hZ->Integral(2, 11), hZR->Integral(2, 11), 100.*(hZR->Integral(2, 11)/hZ->Integral(2, 11) - 1.));
		
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVSciRM::SaveToFile()
{
	if (status==OK || status==OUT_OF_SPEC) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".sci.ratemeters.path.", "./");
		outname += conf->GetValue(".sci.ratemeters.template.", "sci_RM_");
		outname += info->runid;
		outname += ".txt";
		cout << "Writing scientific ratemeters to file " << outname.Data() << endl;
		fout = fopen(outname.Data(), "w");
		if (fout) {
			fprintf(fout, "run\t %d\n", info->runid);
			fprintf(fout, "band\t E_min (MeV)\t E_max (MeV)\t rm_X (evt/s)\t sigma_X\t rm_Z (evt/s)\t sigma_Z\n"); 
			for (int i=1; i<11; i++) fprintf(fout, "%d\t %10.2f\t %10.2f\t %10.3f\t %10.3f\t %10.3f\t %10.3f\n", i, E[i]-E_err[i], E[i]+E_err[i], rmX3913[i], rmX3913_err[i], rmZ3913[i], rmZ3913_err[i]);
		} else cout << "*** impossible to write file " << outname.Data() << endl;
	} else PrintComment();
	return;
}

//
//--------------------------------------------------------------------
//

void MCVSciRM::UpdateGlobalFile()
{
	float t0, dt;
	double tin;
	int nold, nadd;
	TGraphErrors	*aux;
	TGraph		*aux2, *aux3;
	TObjArray 	*rawrm = NULL;
	TObjArray 	*rawac = NULL;
	double		*x, *y;
	char gname[10];
	TFile *fh;
	
	// open ROOT data file
	TString outname;
	outname.Append(getenv("MCALSW"));
	outname += conf->GetValue(".global.history.file.", "./global.root");
	fh = new TFile(outname.Data(), "UPDATE");  
	
	// retrieve or create graph  
	tin = info->tstart;
	t0 = (info->tstart + info->tstop)/2.;
	dt = (info->tstop - info->tstart)/2.;
	if (rawHisto) {
		rawrm = (TObjArray *) rawHisto->FindObject("rawSciRM3913");
		rawac = (TObjArray *) rawHisto->FindObject("rawACRM3913");
	}
	
	for (int i=1; i<=11; i++) {
		// work on aggregate Sci RM X
		aux = NULL;
		nold = 0;
		sprintf(gname, "rmX%d", i);
		fh->GetObject(gname, aux);
		if (aux == NULL) {
			aux = CreateGraphErrors(gname);
			aux->GetXaxis()->SetTitle("Time (s)");
			aux->GetXaxis()->SetTitleSize(0.05);
			aux->GetXaxis()->CenterTitle();
			aux->GetYaxis()->SetTitle("Counts/s");
			aux->GetYaxis()->CenterTitle();
			aux->GetYaxis()->SetTitleSize(0.05);
		}
		nold = aux->GetN();
		aux->Set(nold+1);
		aux->SetPoint(nold, t0, rmX3913[i-1]);
		aux->SetPointError(nold, dt, sqrt(0.5*rmX3913[i-1]/dt));
		aux->Write(gname, TObject::kOverwrite);
		
		// work on aggregate Sci RM Z
		aux = NULL;
		nold = 0;
		sprintf(gname, "rmZ%d", i);
		fh->GetObject(gname, aux);
		if (aux == NULL) {
			aux = CreateGraphErrors(gname);
			aux->GetXaxis()->SetTitle("Time (s)");
			aux->GetXaxis()->SetTitleSize(0.05);
			aux->GetXaxis()->CenterTitle();
			aux->GetYaxis()->SetTitle("Counts/s");
			aux->GetYaxis()->CenterTitle();
			aux->GetYaxis()->SetTitleSize(0.05);
		}
		nold = aux->GetN();
		aux->Set(nold+1);
		aux->SetPoint(nold, t0, rmZ3913[i-1]);
		aux->SetPointError(nold, dt, sqrt(0.5*rmZ3913[i-1]/dt));
		aux->Write(gname, TObject::kOverwrite);
		
		// work on puntual Sci RM X
		aux2 = NULL;
		nold = 0;
		sprintf(gname, "grmX%d", i);
		fh->GetObject(gname, aux2);
		if (aux2 == NULL) {
			aux2 = CreateGraph(gname);
			aux2->GetXaxis()->SetTitle("Time (s)");
			aux2->GetXaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitle("Counts/s");
			aux2->GetYaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitleSize(0.05);
		}
		nold = aux2->GetN();
		aux3 = (TGraph *) rawrm->At(i-1);
		x = aux3->GetX();
		y = aux3->GetY();
		nadd = aux3->GetN();
		aux2->Set(nold+nadd);
//		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, tin+x[j], y[j]);
		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, x[j], y[j]);	// use absolute time
		aux2->Write(gname, TObject::kOverwrite);
		
		// work on puntual Sci RM Z
		aux2 = NULL;
		nold = 0;
		sprintf(gname, "grmZ%d", i);
		fh->GetObject(gname, aux2);
		if (aux2 == NULL) {
			aux2 = CreateGraph(gname);
			aux2->GetXaxis()->SetTitle("Time (s)");
			aux2->GetXaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitle("Counts/s");
			aux2->GetYaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitleSize(0.05);
		}
		nold = aux2->GetN();
		aux3 = (TGraph *) rawrm->At(i+10);
		x = aux3->GetX();
		y = aux3->GetY();
		nadd = aux3->GetN();
		aux2->Set(nold+nadd);
//		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, tin+x[j], y[j]);
		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, x[j], y[j]);	// use absolute time
		aux2->Write(gname, TObject::kOverwrite);
	}
	
	for (int i=0; i<5; i++) {	
		// work on puntual AC Sci RM
		aux2 = NULL;
		nold = 0;
		sprintf(gname, "gAC%d", i);
		fh->GetObject(gname, aux2);
		if (aux2 == NULL) {
			aux2 = CreateGraph(gname);
			aux2->GetXaxis()->SetTitle("Time (s)");
			aux2->GetXaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitle("Counts/s");
			aux2->GetYaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitleSize(0.05);
		}
		nold = aux2->GetN();
		aux3 = (TGraph *) rawac->At(i);
		x = aux3->GetX();
		y = aux3->GetY();
		nadd = aux3->GetN();
		aux2->Set(nold+nadd);
//		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, tin+x[j], y[j]);
		for (int j=0; j<nadd; j++) aux2->SetPoint(nold+j, x[j], y[j]);	// use absolute time
		aux2->Write(gname, TObject::kOverwrite);
	}
	info->Write("info", TObject::kOverwrite);
		
	fh->Close();
	return;
}
