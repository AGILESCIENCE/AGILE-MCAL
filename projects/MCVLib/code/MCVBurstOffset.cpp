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

#include "MCVBurstOffset.h"
ClassImp(MCVBurstOffset);

MCVBurstOffset::MCVBurstOffset() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVBurstOffset::MCVBurstOffset(bool pd, TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{
	pdB = pd;
}

//
//--------------------------------------------------------------------
//

MCVBurstOffset::~MCVBurstOffset()
{

}

//
//--------------------------------------------------------------------
//

int MCVBurstOffset::Calculate()
{
	TObjArray 	*rPD=NULL;
	TF1 		*peak;
	double 		par[3]; 
	char 		fname[20];
	
	// standard checks
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (rawHisto==NULL && info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	if (!info) {
		status = RUN_INFO_NOT_SET;
		return status;
	}
	if (info->pkttype!=3909 && info->pkttype!=3908) {
		status = WRONG_PKTTYPE;
		return status;
	}
	if (pdB) rPD =(TObjArray *) rawHisto->FindObject("rawPDB");
	else rPD =(TObjArray *) rawHisto->FindObject("rawPDA");
	if (!rPD) {
		status = REF_HISTO_NOT_FOUND;
		return status;
	}

	// fit offset peaks
	for (int i=0; i<30; i++) {	
		if (c1) c1->cd(i+1);
		TH1F *aux = (TH1F *) rPD->At(i);
		par[1] = aux->GetMaximumBin();
      		par[0] = aux->GetBinContent(par[1]);
		par[2] = 2.;
		sprintf(fname, "burstoffset_%d", i);
		peak  = new TF1(fname,"gaus", par[1]-15, par[1]+15 );
		peak->SetParameters(par);
		peak->SetLineWidth(1);
		peak->SetLineColor(2);
   		aux->Fit(fname,"QR0+");
		peak->GetParameters(par);
		pc[i] = (float) par[1];
		ps[i] = (float) par[2];
		pa[i] = (float) par[0];
		if (c1) {
			peak->Draw("same");
			aux->SetAxisRange( par[1]-15, par[1]+15, "X");
			c1->Update();
		} else delete peak;
	}
	
	// compare results with current offset values
	nextraoff = 0;
	maxoffdev = conf->GetValue(".burst.offset.deviation.", 0.);
	if (pdB) refoff = info->oB;
	else refoff = info->oA;
	for (int i=0; i<30; i++) {
		offdev[i] = pc[i]-refoff[i];
		if (fabs(offdev[i]) > maxoffdev) {
			nextraoff++;
			outofspec[i] = TRUE;
		}
	}
	if (nextraoff == 0) status = OK;
	else status = OUT_OF_SPEC;			
	return status;
}

//
//--------------------------------------------------------------------
//

void MCVBurstOffset::PrintResults()
{
	printf("\n--------------------- BURST OFFSET ON PD-%c ------------------------------\n", pdB ? 'B' : 'A');
	if (status == OK || status == OUT_OF_SPEC) {
		printf("\nplane \tbar \treference (ch) \tcalculated (ch) \tsigma \tvariation (ch)\n");
		for (int i=0; i<15; i++) {
			printf("X \t%d \t%.3f \t%.3f \t\t%.3f \t%.3f  \t", i+1, refoff[i], pc[i], ps[i], offdev[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		for (int i=15; i<30; i++) {
			printf("Z \t%d \t%.3f \t%.3f \t\t%.3f \t%.3f  \t", i-14, refoff[i], pc[i], ps[i], offdev[i]);
			if (outofspec[i]) printf("*****\n");
			else printf("\n");
		}
		if (nextraoff) cout << endl << "WARNING: " << nextraoff << " PDs with delta_offset > " << maxoffdev << " ch" << endl;
	} else PrintComment();
	printf("\n--------------------------------------------------------------------------\n");
	return;
}

//
//--------------------------------------------------------------------
//

void MCVBurstOffset::SaveToFile()
{
	char name[100];
	if (status==OK || status==OUT_OF_SPEC) {
		TString outname;
		outname.Append(getenv("MCALSW"));
		outname += conf->GetValue(".burst.offset.path.", "./");
		outname += conf->GetValue(".burst.offset.template.", "burst_offset_");
		outname += info->runid;
		outname += (pdB ? "_B" : "_A");
		int id=0;
		for (id=1; id<=100; id++) {
			sprintf(name, "%s_%d.txt", outname.Data(), id);
			fout = fopen(name, "r");
			if (fout) fclose(fout);
			else break;
		}
		fout = fopen(name, "w");
		cout << "Writing offset calcultation results to file " << name << endl;
		if (fout) {
			fprintf(fout, "run\t %d\n", info->runid);
			fprintf(fout, "PD\t %c\n", pdB ? 'B' : 'A');
			fprintf(fout, "\nplane \tbar \treference (ch) \tcalculated (ch) \tsigma \tvariation (ch)\n");
			for (int i=0; i<15; i++) fprintf(fout, "X \t%d \t%.3f \t%.3f \t\t%.3f \t%.3f\n", i+1, refoff[i], pc[i], ps[i], offdev[i]);
			for (int i=15; i<30; i++) fprintf(fout, "Z \t%d \t%.3f \t%.3f \t\t%.3f \t%.3f\n", i-14, refoff[i], pc[i], ps[i], offdev[i]);
		} else cout << "*** impossible to write file " << name << endl;
		fclose(fout);
	} else PrintComment();
	return;
}

//
//--------------------------------------------------------------------
//

void MCVBurstOffset::UpdateCalibrationFile()
{
	char parameterName[100], name[100];
	if (status==OK || status==OUT_OF_SPEC) {
		TString newname;
		newname.Append(getenv("MCALSW"));
		newname += conf->GetValue(".modified.calibration.path.", "./");
		newname += conf->GetValue(".updated.conf.template.", "burst_test_");
		int id=0;
		for (id=1; id<=100; id++) {
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
		oldname += conf->GetValue(".burst.calibration.file.", "burst.conf");
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
			sprintf(parameterName, ".Offset.Bar%02d.%c", (i<15 ? i : i+1), (pdB ? 'B' : 'A'));
			newconf->SetValue(parameterName, pc[i]);  
		}
		newconf->Save();
		delete newconf;
	} else PrintComment();
	return;
}