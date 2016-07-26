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

#include "MCVTree.h"
ClassImp(MCVTree);

MCVTree::MCVTree() : MCVFile(), MCVHisto(), indata(NULL), outdata(NULL), inhisto(NULL), ininfo(NULL), conf(NULL)
{

}

//
//--------------------------------------------------------------------
//

MCVTree::MCVTree(TTree *t, MCVFileInfo *in, TObjArray *h) : MCVFile(), MCVHisto(), indata(NULL), outdata(NULL), inhisto(NULL), ininfo(NULL), conf(NULL), sel(NULL)
{
	indata = t;
	ininfo = in;
	outinfo = (MCVFileInfo *) ininfo->Clone();
//	ininfo->PrintCalibrationParameters();
	inevents = indata->GetEntries();
	if (h) inhisto = h;
	ResizeCountRateHisto(ininfo->ttot);
	if (ininfo->gainFactor != 1.) gf=ininfo->gainFactor;
	if (inhisto) Copy3913SciRatemeters();
	
	// load configuration file and set output filenames
	TString   calFile;
	calFile.Append(getenv("MCALSW"));
	calFile += STANDARD_CONF;
	conf = new TEnv(calFile.Data());
	char tmp[100];
	rootdatapath.Append(getenv("MCALSW"));
	roothistopath.Append(getenv("MCALSW"));
	if (conf) { 
		rootdatapath += conf->GetValue(".selected.data.path.", "./");
		roothistopath += conf->GetValue(".selected.histo.path.", "./");
	} else cout << "*** cannot find standard configuration file " << calFile.Data() << endl;
	if (outinfo->pkttype > 0) sprintf(tmp, "run%06d_%4d_info_", outinfo->runid, outinfo->pkttype);
	else sprintf(tmp, "run%06d_BURST_info_", outinfo->runid);
	infoname += tmp;
//	sprintf(tmp, "RT%06d_%d_sel.root", outinfo->runid, outinfo->pkttype);		// _%4d_
	int id=1;	
	for (id=1; id<=NMAX_FILE; id++) {
		TString name = rootdatapath;
		if (outinfo->pkttype > 0) sprintf(tmp, "RT%06d_%4d_sel_%02d.root", outinfo->runid, outinfo->pkttype, id);
		else  sprintf(tmp, "RT%06d_BURST_sel_%02d.root", outinfo->runid, id);
		name += tmp;
		fp = new TFile(name.Data(), "create");
		if (fp->IsOpen()) {
			rootdatapath += tmp;
			if (outinfo->pkttype > 0) sprintf(tmp, "H%06d_%4d_sel_%02d.root", outinfo->runid, outinfo->pkttype, id);
			else  sprintf(tmp, "H%06d_BURST_sel_%02d.root", outinfo->runid, id);
			roothistopath += tmp;
			break;
		} else delete fp;
	}
	if (outinfo->pkttype > 0) sprintf(outdataname, "tdata_%06d_%4d_%02d", outinfo->runid, outinfo->pkttype, id);
	else  sprintf(outdataname, "tdata_%06d_BURST_%02d", outinfo->runid, id);
	Init();
	

}

//
//--------------------------------------------------------------------
//

MCVTree::MCVTree(TTree *t1, MCVFileInfo *in1, TObjArray *h, TTree *t2, MCVFileInfo *in2) : MCVFile(), MCVHisto(h), indata(NULL), outdata(NULL), inhisto(NULL), ininfo(NULL), conf(NULL), sel(NULL)
{	// constructor ONLY for merging datasets
	indata = t1;
	ininfo = in1;
	outinfo = in2;
	
	inevents = indata->GetEntries();
	inhisto = h;
//	ResizeCountRateHisto(ininfo->ttot + in2->ttot + DELTAT);
	ResizeCountRateHisto(ininfo->teff + in2->teff + 200.);
	if (ininfo->gainFactor != 1.) gf=ininfo->gainFactor;

	cout << "Initial dataset info:" << endl;
	in1->PrintFileInfo();
	cout << "Dataset to be added info:" << endl;
	in2->PrintFileInfo();
	
	// connect input tree
	t2->SetBranchAddress("time", &inevt.time);
	t2->SetBranchAddress("mult", &inevt.mult);
	t2->SetBranchAddress("bar", inevt.bar);
	t2->SetBranchAddress("pdA", inevt.pdA);
	t2->SetBranchAddress("pdB", inevt.pdB);
	t2->SetBranchAddress("pos", inevt.pos);
	t2->SetBranchAddress("E", inevt.E);
	t2->SetBranchAddress("Etot", &inevt.Etot);

	t1->SetBranchAddress("time", &currevt.time);
	t1->SetBranchAddress("mult", &currevt.mult);
	t1->SetBranchAddress("bar", currevt.bar);
	t1->SetBranchAddress("pdA", currevt.pdA);
	t1->SetBranchAddress("pdB", currevt.pdB);
	t1->SetBranchAddress("pos", currevt.pos);
	t1->SetBranchAddress("E", currevt.E);
	t1->SetBranchAddress("Etot", &currevt.Etot);
	
	// merge loop
	int n1 = t1->GetEntries();
	t1->GetEntry(n1-1);
	double t0 = currevt.time;
	//cout << "Initial dataset last time = " << t0 << endl;
	int n2 = t2->GetEntries();
	for (int i=0; i<n2; i++) {
		t2->GetEntry(i);
		CopyEvt();
		t1->Fill();
		FillHisto(&currevt);
	}

	// update file info
	in1->nevents = t1->GetEntries();
	in1->notmonotonic += in2->notmonotonic;
	in1->nlowDT += in2->nlowDT;
	in1->teff += in2->teff;
	cout << "After merging dataset info:" << endl;
	in1->PrintFileInfo();
	
	// saving data
	t1->Write();
	in1->Write();

}

//
//--------------------------------------------------------------------
//

MCVTree::~MCVTree()
{
//	if (tdata) delete tdata;
}

//
//--------------------------------------------------------------------
//

void MCVTree::Init()
{

	// connect input tree
	indata->SetBranchAddress("time", &inevt.time);
	indata->SetBranchAddress("mult", &inevt.mult);
	indata->SetBranchAddress("bar", inevt.bar);
	indata->SetBranchAddress("pdA", inevt.pdA);
	indata->SetBranchAddress("pdB", inevt.pdB);
	indata->SetBranchAddress("pos", inevt.pos);
	indata->SetBranchAddress("E", inevt.E);
	indata->SetBranchAddress("Etot", &inevt.Etot);

	// initialize output tree
	outdata = new TTree(outdataname, "MCAL data");
	outdata->Branch("time", &currevt.time, "time/D");
	outdata->Branch("mult", &currevt.mult, "mult/I");
	outdata->Branch("bar", currevt.bar, "bar[30]/S");
	outdata->Branch("pdA", currevt.pdA, "pdA[mult]/S");
	outdata->Branch("pdB", currevt.pdB, "pdB[mult]/S");
	outdata->Branch("pos", currevt.pos, "pos[mult]/F");
	outdata->Branch("E", currevt.E, "E[mult]/F");
	outdata->Branch("Etot", &currevt.Etot, "Etot/F");
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::CopyEvt()
{
	int auxmult;
	int auxSciRME;
	int cbar;
	currevt.time = inevt.time;
	auxmult = inevt.mult;
	currevt.mult = auxmult;
	currevt.totEX = 0;
	currevt.totEZ = 0;
	for (int i=0; i<30; i++) {
		cbar = inevt.bar[i];
		currevt.bar[i] = cbar;
		if (cbar>=0) {
			auxSciRME = inevt.pdA[cbar] + inevt.pdB[cbar] - outinfo->soA[i] - outinfo->soB[i];
			if (i<15) currevt.totEX += auxSciRME;
			else currevt.totEZ += auxSciRME;
		}
	}
	for (int i=0; i<auxmult; i++) {
		currevt.pdA[i] = inevt.pdA[i];
		currevt.pdB[i] = inevt.pdB[i];
		currevt.pos[i] = inevt.pos[i];
		currevt.E[i] = inevt.E[i];
	}
	currevt.Etot = inevt.Etot;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::CopyEvt(bool *barsel)
{
	// copy only selected bars

	int auxmult=0;
	int auxSciRME;
	int cbar;
	currevt.time = inevt.time;
	currevt.totEX = 0;
	currevt.totEZ = 0;
	currevt.Etot = 0.;
	
	for (int i=0; i<30; i++) {
		cbar = inevt.bar[i];
		if (cbar>=0 && barsel[cbar]) {
			currevt.bar[i] = auxmult;
			auxSciRME = inevt.pdA[cbar] + inevt.pdB[cbar] - outinfo->soA[i] - outinfo->soB[i];
			if (i<15) currevt.totEX += auxSciRME;
			else currevt.totEZ += auxSciRME;
			
			currevt.pdA[auxmult] = inevt.pdA[cbar];
			currevt.pdB[auxmult] = inevt.pdB[cbar];
			currevt.pos[auxmult] = inevt.pos[cbar];
			currevt.E[auxmult] = inevt.E[cbar];
			currevt.Etot += inevt.E[cbar];
			auxmult++;
		}
		else currevt.bar[i] = -1;
	}
	
	currevt.mult = auxmult;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::Shift(int nevt, int nshift)
{
	TGraphErrors *gtmp;
	TH1F *htmp;

	// copy scientific ratemeters data
	rawHisto->Remove(gRMX3913);
	delete gRMX3913;
	gtmp = (TGraphErrors *) inhisto->FindObject("gRMX3913");
	gRMX3913 =  (TGraphErrors *) gtmp->Clone();
	rawHisto->AddLast(gRMX3913);
	
	rawHisto->Remove(gRMZ3913);
	delete gRMZ3913;
	gtmp = (TGraphErrors *) inhisto->FindObject("gRMZ3913");
	gRMZ3913 =  (TGraphErrors *) gtmp->Clone();
	rawHisto->AddLast(gRMZ3913);
	
	rawHisto->Remove(gRMX);
	delete gRMX;
	gtmp = (TGraphErrors *) inhisto->FindObject("gRMX");
	gRMX =  (TGraphErrors *) gtmp->Clone();
	rawHisto->AddLast(gRMX);
	
	rawHisto->Remove(gRMZ);
	delete gRMZ;
	gtmp = (TGraphErrors *) inhisto->FindObject("gRMZ");
	gRMZ =  (TGraphErrors *) gtmp->Clone();
	rawHisto->AddLast(gRMZ);
	
/*	rawHisto->Remove(sciRMX);
	delete sciRMX;
	htmp = (TH1F *) inhisto->FindObject("sciRMX");
	sciRMX =  (TH1F *) htmp->Clone();
	rawHisto->AddLast(sciRMX);
	
	rawHisto->Remove(sciRMZ);
	delete sciRMZ;
	htmp = (TH1F *) inhisto->FindObject("sciRMZ");
	sciRMZ =  (TH1F *) htmp->Clone();
	rawHisto->AddLast(sciRMZ);	
*/	
	// update file names
	outdata->SetName("tdata_shift");
	
	double oldtime;
	if (!indata || !outdata) return;
	if (nshift<0) {		// typical case: shift backward time array to erase a fake timetag
		inevents--;
		for (int i=0; i<inevents; i++) {
			if (i%100000==0) cout << "...processed " << i << " events..." << endl;
			indata->GetEntry(i);
			CopyEvt();
			if (i>=nevt) {
				indata->GetEntry(i-nshift);
				currevt.time = inevt.time;
			}
			outdata->Fill();
			FillHisto(&currevt);
		}
	} else {		// less common case keep all timetag and destroy PD data
		for (int i=0; i<inevents; i++) {
			if (i%100000==0) cout << "...processed " << i << " events..." << endl;
			indata->GetEntry(i);
			currevt=inevt;
			if (i>=nevt) {
				oldtime = inevt.time;
				indata->GetEntry(i+nshift);
				CopyEvt();
				currevt.time = oldtime;
			}
			outdata->Fill();
			FillHisto(&currevt);
		}
	
	}
	FillCountRateDistribution();
	outinfo->teff = GetEffectiveTime();
	FillGlobalHisto(outinfo->pkttype, outinfo->teff);
	outinfo->nlowDT = nlowDT;
	outinfo->lowDTti = lowDTti;
	outinfo->lowDTtf = lowDTtf;
	outinfo->notmonotonic = ininfo->notmonotonic;
	outinfo->teff = GetEffectiveTime();
	outinfo->nevents = outdata->GetEntries();	// updates the number of events (the selected ones)
	infoname += "shift";
	outinfo->SetName(infoname.Data());
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::SaveData()
{
	outdata->Write();				// write the new Root tree to file
	outinfo->Write(infoname.Data());
	if (sel) sel->Write();
	fp->Close();
			
	fh = new TFile(roothistopath.Data(), "recreate");
	if (sel) {
		TString hname = "rawHisto_";
		hname += sel->GetName();
		rawHisto->SetName(hname.Data());
		rawHisto->Write(hname.Data(), TObject::kSingleKey);
	} else rawHisto->Write("rawHisto", TObject::kSingleKey);
	outinfo->Write(infoname.Data());
	if (sel) sel->Write();
	fh->Close();
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::Copy3913SciRatemeters()
{
	TGraphErrors *gtmp;
	TGraph *gtmp2;
	TH1F *htmp;
	
	rawHisto->Remove(rawSciRM3913);
	for (int i=0; i<22; i++) {
		gtmp2 = (TGraph *) rawSciRM3913->At(i);
		rawSciRM3913->Remove(gtmp2);
		delete gtmp2;
	}
	for (int i=0; i<22; i++) {
		gtmp2 = (TGraph *) ((TGraph *) ((TObjArray *) inhisto->FindObject("rawSciRM3913"))->At(i))->Clone();
		rawSciRM3913->AddLast(gtmp2);
	}
	rawHisto->AddLast(rawSciRM3913);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::DoSelection(MCVSelection *s)
{
	sel = s;
	TString dname = "tdata_";
	dname += sel->GetName();
	outdata->SetName(dname.Data());
	
	// set PD limits
	double minA[30];
	double maxA[30];
	double minB[30];
	double maxB[30];
	if (sel->pdAEnable) {
		if (sel->relToOffset) {
			for (int i=0; i<30; i++) {
				minA[i] = (ininfo->oA)[i] + sel->minA;
				maxA[i] = (ininfo->oA)[i] + sel->maxA;
			}
		} else {
			for (int i=0; i<30; i++) {
				minA[i] = sel->minA;
				maxA[i] = sel->maxA;
			}
		}
	}
	if (sel->pdBEnable) {
		if (sel->relToOffset) {
			for (int i=0; i<30; i++) {
				minB[i] = (ininfo->oB)[i] + sel->minB;
				maxB[i] = (ininfo->oB)[i] + sel->maxB;
			}
		} else {
			for (int i=0; i<30; i++) {
				minB[i] = sel->minB;
				maxB[i] = sel->maxB;
			}
		}
	}
	
	// main loop
	bool selected, tmpsel, barsel[30];
	int cmult;
	int nbars = sel->nbars;
	double mint, maxt;
	if (sel->timeEnable) {
		indata->GetEntry(0);
		mint = inevt.time + sel->minTime;
		maxt = inevt.time + sel->maxTime;
	}
	for (int id=0; id<inevents; id++) {
		selected = TRUE;
		for (int k=0; k<30; k++) barsel[k] = TRUE;
		if (id%100000==0) cout << "...processed " << id << " events..." << endl;
		indata->GetEntry(id);
		cmult = inevt.mult;
		if (sel->multEnable && (cmult<sel->minMult || cmult>sel->maxMult)) selected = FALSE;
		if (selected && sel->timeEnable && (inevt.time<mint || inevt.time>maxt)) selected = FALSE;
		if (selected && sel->nbars<30) {
			if (sel->isBarStrong) {
				for (int i=0; i<30; i++) if (inevt.bar[i] >=0 && sel->bars[i]==0) selected = FALSE;
			} else {
				tmpsel = FALSE;
				for (int i=0; i<30; i++) if (inevt.bar[i] >=0 && sel->bars[i]==1) tmpsel = TRUE;
				if (!tmpsel) selected = FALSE;
			}
		}
		if (sel->relToOffset) {
			if (selected && sel->pdAEnable) {
				for (int i=0; i<30; i++) if (inevt.bar[i] >=0 && (inevt.pdA[inevt.bar[i]]<minA[i] || inevt.pdA[inevt.bar[i]]>maxA[i])) selected = FALSE;
			}
			if (selected && sel->pdBEnable) {
				for (int i=0; i<30; i++) if (inevt.bar[i] >=0 && (inevt.pdB[inevt.bar[i]]<minB[i] || inevt.pdB[inevt.bar[i]]>maxB[i])) selected = FALSE;
			}
		} else {
			if (selected && sel->pdAEnable) {
				for (int i=0; i<cmult; i++) if (inevt.pdA[i]<sel->minA || inevt.pdA[i]>sel->maxA) selected = FALSE;
			}
			if (selected && sel->pdBEnable) {
				for (int i=0; i<cmult; i++) if (inevt.pdB[i]<sel->minB || inevt.pdB[i]>sel->maxB) selected = FALSE;
			}
		}
		if (selected && sel->posEnable) {
			if (sel->isPosStrong) {
				for (int i=0; i<cmult; i++) if (inevt.pos[i]<sel->minPos || inevt.pos[i]>sel->maxPos) selected = FALSE;
			} else {
				tmpsel = FALSE;
				for (int i=0; i<cmult; i++) if (inevt.pos[i]>=sel->minPos && inevt.pos[i]<=sel->maxPos) tmpsel = TRUE; 
				if (!tmpsel) selected = FALSE;
			}
		}
		if (selected && sel->energyEnable) {
			if (sel->isEStrong) {
				for (int i=0; i<cmult; i++) if (inevt.E[i]<sel->minE || inevt.E[i]>sel->maxE) selected = FALSE;
			} else {
				/*tmpsel = FALSE;	// old selection version
				for (int i=0; i<cmult; i++) if (inevt.E[i]>=sel->minE && inevt.E[i]<=sel->maxE) tmpsel = TRUE; 
				if (!tmpsel) selected = FALSE;*/
				
				selected = FALSE;	// new selection version: work on single bars
				for (int i=0; i<cmult; i++) {
					if (inevt.E[i]<sel->minE || inevt.E[i]>sel->maxE) barsel[i] = FALSE;
					else selected = TRUE;	// TRUE if at least one bar is selected
				}
			}
		}
		
		if (selected) {
			//CopyEvt();
			CopyEvt(barsel);
			outdata->Fill();
			FillHisto(&currevt);
		}	
	}
	FillCountRateDistribution();
	outinfo->nlowDT = nlowDT;
	outinfo->lowDTti = lowDTti;
	outinfo->lowDTtf = lowDTtf;
	outinfo->notmonotonic = ininfo->notmonotonic;
	outinfo->teff = ininfo->teff;
	if (sel->timeEnable) {
		outinfo->tstart = ininfo->tstart + sel->minTime;
		outinfo->tstop  = ininfo->tstart + sel->maxTime;
		//if (outinfo->pkttype == 3901) outinfo->teff = sel->maxTime - sel->minTime;
		//else outinfo->teff = GetEffectiveTime();
		outinfo->teff = sel->maxTime - sel->minTime;
	}
	UpdateSciRM();
	FillGlobalHisto(outinfo->pkttype, outinfo->teff);
	outinfo->nevents = outdata->GetEntries();	// updates the number of events (the selected ones)
	infoname += sel->GetName();
	outinfo->SetName(infoname.Data());
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::ApplyCalibration()
{
	TString dname = "tdata_newcal_";
	outdata->SetName(dname.Data());
	int cmult, cbar, k, n;
	cout << "Applying new calibration parameters: " << endl;
	outinfo->PrintCalibrationParameters();
	
	// set calibration parameters used for calculations in class MCVFile
	seed = outinfo->seed;
	srand48(seed);
	barLen = outinfo->barLen;
	gainFactor = outinfo->gainFactor;
	for (int i=0; i<30; i++) {
		oA[i] = outinfo->oA[i];
		oB[i] = outinfo->oB[i];
		soA[i] = outinfo->soA[i];
		soB[i] = outinfo->soB[i];
		alphaA[i] = outinfo->alphaA[i];
		alphaB[i] = outinfo->alphaB[i];
		u0A[i] = outinfo->u0A[i];
		u0B[i] = outinfo->u0B[i];
	}
	
	// calculate auxiliary parameters for energy/position calculations
	if (gainFactor != 1.) {		// to account for different gain of the grid chains
		for (UInt_t i=0; i<30; i++) {
			u0A[i] *= gainFactor;
			u0B[i] *= gainFactor;
		}
	}
	for (UInt_t i=0; i<30; i++) {
		alphaMean[i]  = (alphaA[i] + alphaB[i])*0.5; 
		alphaDelta[i] = (alphaA[i] - alphaB[i])*0.5;  
		cx1[i] = 0.5 * (log(u0A[i]/u0B[i]) - alphaDelta[i]*barLen)/alphaMean[i];
		cx2[i] = 0.5/alphaMean[i];
		cE1[i] = exp(0.5 * barLen * alphaMean[i]) / sqrt(u0A[i] * u0B[i]);
	}	
	
	// main loop
	for (int id=0; id<inevents; id++) {
		if (id%100000==0) cout << "...processed " << id << " events..." << endl;
		indata->GetEntry(id);
		CopyEvt();
		cmult = currevt.mult;
		cbar = 0;
		n = 0;
		do {
			k = currevt.bar[cbar];
			if(k>=0) {
				CalculateEandPos(cmult, cbar, currevt.pdA[k], currevt.pdB[k]);
				n++;
			}
			cbar++;
		} while (n<cmult && cbar<30);
		outdata->Fill();
		FillHisto(&currevt);
	}

	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::UpdateSciRM()
{
	TGraphErrors *gtmp;
	TGraph *gX, *gZ;
	TH1F *hX, *hZ;
	float rmX[11], rmZ[11];
	double *xx, *yy; 	
	double mint, maxt;
	int np, nout[11]; 
	
	if (sel->timeEnable) {
		// apply time selection to scientific ratemeters
		mint = sel->minTime;
		maxt = sel->maxTime;
		
		for (int i=0; i<11; i++) {
			rmX[i]=0.;
			nout[i] = 0;
			gX = (TGraph *) rawSciRM3913->At(i);
			np = gX->GetN();
			xx = gX->GetX();
			yy = gX->GetY();
			for (int j=0; j<np; j++) {
				if (xx[j]>=mint && xx[j]<=maxt) 
					if (yy[j]<1000) {
						rmX[i] += yy[j];
					} else {
						cout << "*** high RM value at line " << j+1 << " , RM " << i+1 << endl;
						nout[i]++;
					}
			}
			gRMX3913->SetPoint(i, Eband[i], rmX[i]/(maxt - mint - nout[i]));
			gRMX3913->SetPointError(i, Eerr[i], sqrt(rmX[i])/(maxt - mint - nout[i]));
		}
		
		for (int i=0; i<11; i++) {
			rmZ[i]=0.;
			nout[i] = 0;
			gZ = (TGraph *) rawSciRM3913->At(i+11);
			np = gZ->GetN();
			xx = gZ->GetX();
			yy = gZ->GetY();
			for (int j=0; j<np; j++) {
				if (xx[j]>=mint && xx[j]<=maxt) 
					if (yy[j]<1000) {
						rmZ[i] += yy[j];
					} else {
						cout << "*** high RM value at line " << j+1 << " , RM " << i+1 << endl;
						nout[i]++;
					}
			}
			gRMZ3913->SetPoint(i, Eband[i], rmZ[i]/(maxt - mint - nout[i]));
			gRMZ3913->SetPointError(i, Eerr[i], sqrt(rmZ[i])/(maxt - mint - nout[i]));
		}
	} else {
		// copy scientific ratemeters data
		rawHisto->Remove(gRMX3913);
		delete gRMX3913;
		gtmp = (TGraphErrors *) inhisto->FindObject("gRMX3913");
		gRMX3913 =  (TGraphErrors *) gtmp->Clone();
		rawHisto->AddLast(gRMX3913);
		
		rawHisto->Remove(gRMZ3913);
		delete gRMZ3913;
		gtmp = (TGraphErrors *) inhisto->FindObject("gRMZ3913");
		gRMZ3913 =  (TGraphErrors *) gtmp->Clone();
		rawHisto->AddLast(gRMZ3913);	
	}
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::MuonTrackSelection()
{
	bool selected;
	int minbar, bp, nbars, planeX, istart, istop;
	float minE;
	
	// load selection parameters
	minbar = conf->GetValue(".muon.min.nbars.", 15);
	minE   = conf->GetValue(".muon.energy.threshold.", 10.);
	planeX = conf->GetValue(".muon.plane.X.", 0);
	outdata->SetName("tdata_muons");
	
	// main loop
	for (int id=0; id<inevents; id++) {
		selected = TRUE;
//		if (id%100000==0) cout << "...processed " << id << " events..." << endl;
		indata->GetEntry(id);
		
		if (inevt.mult < minbar) selected = FALSE;
		if (selected) {
			nbars = 0;
			if (planeX == 1) {
				istart = 0;
				istop  = 15;
				for (int i=15; i<30; i++) inevt.bar[i] = -10;	// mask the bars on the wrong plane
			} else { 
				istart = 15;
				istop  = 30;
				for (int i=0; i<15; i++) inevt.bar[i] = -10;	// mask the bars on the wrong plane
			}
			for (int i=istart; i<istop; i++) {
				bp = inevt.bar[i];
				if (bp>=0) {
					if (inevt.E[bp] >= minE) nbars++;
					else inevt.bar[i] = -10;		// mask the not-selected bar
				}
			}
			if (nbars < minbar) selected = FALSE;			
		}
		
		if (selected) {
			CopyEvt();
			outdata->Fill();
			FillHisto(&currevt);
		}	
	}
	
	FillCountRateDistribution();
//	UpdateSciRM();
//	FillGlobalHisto(outinfo->pkttype, outinfo->teff);
	infoname += "muons";
	outinfo->nevents = outdata->GetEntries();	// updates the number of events (the selected ones)
	outinfo->SetName(infoname.Data());
	cout << "Run " << outinfo->runid << " ; initial number of events = " << ininfo->nevents << " ; final number of events = " << outinfo->nevents << "  (" << 100.*((float) outinfo->nevents)/ininfo->nevents << " %)" << endl;
	return;
}

//
//--------------------------------------------------------------------
//

void MCVTree::DoZeroSuppression()
{
	int inmult, outmult=0, id0, id1;
	MCALevt tmpevt;

	cout << "Performing Zero-suppression on GRID Physical Calibration data" << endl;
	if (ininfo->pkttype != 3902 && ininfo->pkttype != 1111 && ininfo->pkttype != 2222) {
		cout << "*** Neither 39.02 GRID Physical Calibration data, nor simulation data!" << endl;
		return;
	}
	outdata->SetName("tdata_zerosup");
	
	// main loop
	for (int id=0; id<inevents; id++) {
//		if (id%100000==0) cout << "...processed " << id << " events..." << endl;
		indata->GetEntry(id);
		inmult = inevt.mult;
		outmult = 0;
//		cout << id << " 1" << endl;
			
/*			if (id == 56) {
				cout << endl << "step 1 " << endl << outmult << endl;
				for (int k=0; k<30; k++) cout << inevt.bar[k] << "  ";
				cout << endl;
			}*/
		
		for (int i=0; i<30; i++) {	// look for bars above zero-suppression threshold
			id0 = inevt.bar[i];
			if (id0 >= 0 && (inevt.pdA[id0] > ininfo->oA[i] + ZSTH || inevt.pdB[id0] > ininfo->oB[i] + ZSTH)) {
					tmpevt.bar[i] = outmult++;
			} else tmpevt.bar[i] = -1;
		}
		
		for (int i=0; i<30; i++) {	// copy values for bars above zero-suppression threshold
			id1 = tmpevt.bar[i];
			if (id1 >= 0) {
				id0 = inevt.bar[i];
				tmpevt.pdA[id1] = inevt.pdA[id0];
				tmpevt.pdB[id1] = inevt.pdB[id0];
				tmpevt.E[id1]   = inevt.E[id0];
				tmpevt.pos[id1] = inevt.pos[id0];
				tmpevt.rmE[id1] = inevt.rmE[id0];
			}
		}
		
		// update inevt structure
		for (int i=0; i<30; i++) inevt.bar[i] = tmpevt.bar[i];
		inevt.mult = outmult;
		for (int i=0; i<outmult; i++) {				
			inevt.pdA[i] = tmpevt.pdA[i];
			inevt.pdB[i] = tmpevt.pdB[i];
			inevt.pos[i] = tmpevt.pos[i];
			inevt.E[i]   = tmpevt.E[i];
			inevt.rmE[i] = tmpevt.rmE[i];
		}
					
/*			if (id == 56) {
				cout << endl << "step 3 " << endl << outmult << endl;
				for (int k=0; k<30; k++) cout << inevt.bar[k] << "  ";
				cout << endl;
			}*/
		
		if (outmult > 0) {
			CopyEvt();
			outdata->Fill();
			FillHisto(&currevt);
		}	
	}
	FillCountRateDistribution();
	outinfo->nlowDT = nlowDT;
	outinfo->lowDTti = lowDTti;
	outinfo->lowDTtf = lowDTtf;
	outinfo->notmonotonic = ininfo->notmonotonic;
	outinfo->teff = ininfo->teff;
	cout << "teff= " << outinfo->teff << "   pkttype= " << outinfo->pkttype << endl;
	FillGlobalHisto(outinfo->pkttype, outinfo->teff);
	return;
}
