offset_correction(char *filename="Hmerged_025000-025010.root")
{
	// MM 04/04/2012 
	// offset correction on a combined dataset created with macro process_histo.C
	
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

	TFile *fh = new TFile(filename);
	fh.ls();
	TIter next = fh->GetListOfKeys();
	TKey *key;
	TObjArray *rh;
	MCVFileInfo *info;
	
	while ((key = (TKey *)next())) {
		if (strncmp(key->GetName(),"rawHisto", 8) == 0) rh = (TObjArray *) fh->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fh->Get(key->GetName());
	}
	if (info) {
		info->PrintFileInfo();
		info->PrintCalibrationParameters();
	}
	
	// analyze offset pdA
	MCVBurstOffset *offsetA = new MCVBurstOffset(0, rh, info);
	offsetA->Calculate();
	offsetA->PrintResults();
	double *oA = offsetA->GetPeakCenter();
	
	// analyze offset pdB
	MCVBurstOffset *offsetB = new MCVBurstOffset(1, rh, info);
	offsetB->Calculate();
	offsetB->PrintResults();
	double *oB = offsetB->GetPeakCenter();
	
	// update calibration file
	char parameterName[50];
	TEnv *newconf = new TEnv();
	newconf->ReadFile("/home/mcal/local/mcalsw/share/MCVLib_conf/MCALBurstTest_601.conf", kEnvAll);
	for (int i=0; i<30; i++) {
		sprintf(parameterName, ".Offset.Bar%02d.A", (i<15 ? i : i+1)); 
		newconf->SetValue(parameterName, oA[i]);  
		sprintf(parameterName, ".Offset.Bar%02d.B", (i<15 ? i : i+1));
		newconf->SetValue(parameterName, oB[i]); 
	}
	sprintf(parameterName, "new_offset_conf_%06d.conf", info->runid); 
	newconf->WriteFile(parameterName, kEnvAll);
	
	delete offsetA;
	delete offsetB;
		
}