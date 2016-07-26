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

#include "GRB3908.h"

ClassImp(GRB3908);

GRB3908::GRB3908() : GRB()
{
	// NB: this constructor MUST be implemented even is useless, otherwise you will get 
	// /home/martino/mcalsw/lib/libMCV.so: undefined reference to `GRB::GRB[in-charge]()'
}


//
//--------------------------------------------------------------------
//

GRB3908::~GRB3908()
{
	fi->Close();
}

//
//--------------------------------------------------------------------
//

GRB3908::GRB3908(TString rootfilename) : GRB(), data(NULL), cStdLE(NULL), cStdME(NULL), cStdHE(NULL), hpoisson(NULL), gEt(NULL)
{

	// load root events file
	cout << "GRB3908::GRB3908() Opening rootfile " << rootfilename.Data() << endl;
	fi= new TFile(rootfilename.Data());
	if (fi->IsZombie()) { // fi is not a valid root file: exit
		cout << rootfilename.Data() << " is not a valid ROOT file: exiting!" << endl;
		return;
	} else {
	
		// get data and file info
		TIter next = fi->GetListOfKeys();
		TKey *key;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"tdata", 5) == 0) data = (TTree *) fi->Get(key->GetName());
			if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fi->Get(key->GetName());
		}
		if (info) info->PrintFileInfo();
		nevents = data->GetEntries();
	}
	
	// connect the input tree
	data->SetBranchAddress("time", &inevt.time);
	data->SetBranchAddress("mult", &inevt.mult);
	data->SetBranchAddress("bar", inevt.bar);
	data->SetBranchAddress("pdA", inevt.pdA);
	data->SetBranchAddress("pdB", inevt.pdB);
	data->SetBranchAddress("pos", inevt.pos);
	data->SetBranchAddress("E", inevt.E);
	data->SetBranchAddress("Etot", &inevt.Etot);
	
	// get default time boundaries
	data->GetEntry(0);
	tstart = inevt.time;
	data->GetEntry(nevents-1);
	tstop = inevt.time;
	
	// find the real tstart and tstop in case of not monotonic behaviour
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		if (inevt.time < tstart) tstart = inevt.time;
		if (inevt.time > tstop)  tstop  = inevt.time;
	}
	
	t0=0.;
	nbins=0;
	tbin = 0.032;
	
	// init default light curves
	for (int kband=0; kband < 3; kband++) {
		for (int izone=0; izone < 4; izone++) {
			for (int jtime=0; jtime < 7; jtime++) {
				double dt = 0.016;	// 16 ms basic time bin
				for (int i=0; i<jtime; i++) dt *= 2.;
				int nbinsL = (int) ((tstop-tstart)/dt);
				tstop = tstart + nbinsL*dt;
				TString hname("hlc_");
				hname += kband;  hname += "_";  hname += izone;  hname += "_";  hname += jtime;
				hlc[kband][izone][jtime] = new TH1F(hname, hname, nbinsL, tstart, tstop);
			}
		}
	}
	
	// diagnostic histogram initialization
	InitDiagnostic();
	for (int i=0; i<30; i++) nbars[i] = 0;
	for (int i=0; i<8; i++)  cuboni[i] = 0;
	dt0 = 0.;	// init offset with respect to t0. To be used with VSB analysis via method FindTrueT0()
	nsubms = 0;	// number of sub-ms triggers found by method FindTrueT0()
	mint = 0.;
	tsearch = 0.512;	// 0.512
}

//
//--------------------------------------------------------------------
//

void GRB3908::InitDiagnostic()
{
	int npos = 15;		// position array
	double ipos = -30.;
	double fpos =  30.;
	double *pos = new double[npos+1];
	for (int i=0; i<=npos; i++) pos[i] = ipos + i*(fpos-ipos)/npos; 
	
	int nbar = 15;		// bars array
	double ibar = 0;
	double fbar = 15;
	double *bar = new double[nbar+1];
	for (int i=0; i<=nbar; i++) bar[i] = ibar + i*(fbar-ibar)/nbar;

	hX = new TH3F("h3X", "X plane events distribution", nbar, bar, npos, pos, nEbins, mcalEbin);
	hX->SetStats(0);
	hX->SetTitle("Events distribution on X plane");
	hZ = new TH3F("h3Z", "Z plane events distribution",npos, pos, nbar, bar, nEbins, mcalEbin);
	hZ->SetStats(0);
	hZ->SetTitle("Events distribution on Z plane");
	hbarsbkg = new TH1F("hbarsbkg", "background bars rate", 30, 0, 30);
	hbarsgrb = new TH1F("hbarsgrb", "GRB bars rate", 30, 0, 30);

	hDTgrb = new TH1F("hDTgrb", "GRB delta_T histogram", 10000, 0., 0.010);
	hDTbkg = new TH1F("hDTbkg", "background delta_T histogram", 10000, 0., 0.010);
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::FillDiagnostic(MCALevt *evt)
{
	// diagnostic ONLY dedicated to selected events
	
}

//
//--------------------------------------------------------------------
//

int GRB3908::DefaultLightCurve()
{
	cout << "GRB::DefaultLightCurve() building default light curves. Processing " << nevents << " events" << endl;
	
	// process data: fill default light curves
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		
		// get the bar with highest energy deposit and set the spatial zone
		double emax = -1.;
		int ibar = -1;
		int izone = 3;	// Z9-Z15
		for (int k=0; k<30; k++) {
			int id = inevt.bar[k];
			if (id>=0 && (inevt.E)[id] > emax) {
				emax = (inevt.E)[id];
				ibar = k;
			}
			if (id == inevt.mult - 1) break;
		}
		if (ibar<8) izone = 0;		// X1-X8
		else if (ibar<15) izone = 1;	// X9-X15
		else if (ibar<23) izone = 2;	// Z1-Z8
		
		// get energy band
		int kband=2;	// high energy: E>1.4 MeV
		if (inevt.Etot<0.7) kband=0;
		else if (inevt.Etot<1.4) kband=1;
		
		// fill default light curve histograms
		for (int jtime=0; jtime < 7; jtime++) {
			hlc[kband][izone][jtime]->Fill(inevt.time);
		}
	}
	
	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB3908::DrawDefaultLightCurve()
{
	// plot default light curves (Low Energy)
	cStdLE = new TCanvas("cStdLE", "Default MCAL Light Curves (Low Energy)", 1000, 800);
	cStdLE->Divide(2, 2);
	for (int izone=0; izone < 4; izone++) {
		cStdLE->cd(izone+1);
		hlc[0][izone][6]->Draw();
		for (int jtime=0; jtime < 6; jtime++) {
			hlc[0][izone][jtime]->SetLineColor(jtime+1);
			hlc[0][izone][jtime]->Draw("SAME");
		}
	}
	
	// plot default light curves (Medium Energy)
	cStdME = new TCanvas("cStdME", "Default MCAL Light Curves (Medium Energy)", 1000, 800);
	cStdME->Divide(2, 2);
	for (int izone=0; izone < 4; izone++) {
		cStdME->cd(izone+1);
		hlc[1][izone][6]->Draw();
		for (int jtime=0; jtime < 6; jtime++) {
			hlc[1][izone][jtime]->SetLineColor(jtime+1);
			hlc[1][izone][jtime]->Draw("SAME");
		}
	}
	
	// plot default light curves (High Energy)
	cStdHE = new TCanvas("cStdHE", "Default MCAL Light Curves (High Energy)", 1000, 800);
	cStdHE->Divide(2, 2);
	for (int izone=0; izone < 4; izone++) {
		cStdHE->cd(izone+1);
		hlc[2][izone][6]->Draw();
		for (int jtime=0; jtime < 6; jtime++) {
			hlc[2][izone][jtime]->SetLineColor(jtime+1);
			hlc[2][izone][jtime]->Draw("SAME");
		}
	}
	return;
}

//
//--------------------------------------------------------------------
//

int GRB3908::CustomLightCurve(double t0L, double tpre, double tpost, double eminL, double emaxL, int barsXL, int barsZL, double tbinL)
{
	emin = eminL;
	emax = emaxL;
	tbin = tbinL;
	barsX = barsXL;
	barsZ = barsZL;
	
	cout << "GRB3908::CustomLightCurve() building custom light curves. Processing " << nevents << " events" << endl;
	
	// set the time boundaries and create histogram
	if (t0L > 0.) {
		tstart = tpre;
		tstop = tpost;
		t0=t0L + dt0;	// NB: dt0 is 0 unless the sub-millisecond t0 search is performed with FindTrueT0()
	}
	nbins = (int) ((tstop-tstart)/tbin)+1;
	double tend =  tstart + nbins*tbin;
	hclc = new TH1D("hcustom", "Custom light curve", nbins, tstart, tend);
	h2clc = new TH2D("h2D", "Energy binned light curve", nbins, tstart, tend, nEbins, mcalEbin);

	hLE = new TH1F("hLE", "low energy histogram", nbins, tstart, tend);	// for HR calculations
	hHE = new TH1F("hLE", "low energy histogram", nbins, tstart, tend);
	ghr = new TGraphErrors();

	tzero = new TTimeStamp();
	tzero->Set((int) t0 , 0, epoch->GetSec(),0);
	printf("Time0 TT %12.0f    UT %s\n", t0, tzero->AsString("s"));
	
	// get time0 information
	char caux[10];
	sprintf(caux, "%d", tzero->GetDate(1));
	sscanf(caux, "%4d%2d%2d", &year, &month, &day);
	sprintf(caux, "%06d", tzero->GetTime(1));
	sscanf(caux, "%2d%2d%2d", &hour, &min, &sec);
	
	// process data: fill custom light curve
	double tini = t0+tstart;
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		//if (i<100) printf("%20.6f\n", inevt.time); // verified that inevt.time is conformal to FITS file time
		bool good = TRUE;
		
		// do time, energy and spatial selection
		if (inevt.time<tini) good = FALSE;
		if (good) {
			if (mint == 0.) mint = inevt.time - t0;
			int ibar = -1;
			double etot = -1.;
			for (int k=0; k<30; k++) {
				int id = inevt.bar[k];
				if (id>=0 && (inevt.E)[id] > etot) {
					etot = (inevt.E)[id];
					ibar = k;
				}
				if (id == inevt.mult - 1) break;
			}
			int ib=1;
			if (ibar<15) {
				ib<<=ibar;
				if (!(barsX & ib)) good = FALSE;			
			} else {
				ibar-=15;
				ib<<=ibar;
				if (!(barsZ & ib)) good = FALSE;
			}
			h2clc->Fill(inevt.time - t0, inevt.Etot);	// a monte della selezione in energia
			if (good) {
				if (inevt.Etot <= thr) hLE->Fill(inevt.time - t0);	// apply position selection (if any) to HR calculations
				else hHE->Fill(inevt.time - t0);
			}
		}
		if (good && (inevt.Etot<emin || inevt.Etot>emax)) good = FALSE;		
		if (good) {
			hclc->Fill(inevt.time - t0);
		}
	}

	return 0;
}

//
//--------------------------------------------------------------------
//

int GRB3908::StandardDiagnostic()
{
	if (!t0) {
		cout << "GRB::StandardDiagnostic() : cannot perform standard diagnostic: no t0 specified!" << endl;
		return 1;
	} else cout << "GRB::StandardDiagnostic() : performing standard diagnostic on dataset" << endl;
	
	// perform standard diagnostic on events
	double dt;
	double tprev = tbkgini - 10.;
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		dt = inevt.time - t0;
		if ((dt < tbkgini || dt > tbkgfin) && (dt < tgrbini || dt > tgrbfin)) continue;
		
		if (dt >= tbkgini && dt <= tbkgfin) { // background data	
			if (inevt.Etot>=emin && inevt.Etot<=emax) {
				for (int k=0; k<30; k++) {	// fill bars histogram
					int id = inevt.bar[k];
					if (id>=0) hbarsbkg->Fill(k);
					if (id == inevt.mult - 1) break;
				}
				hDTbkg->Fill(dt-tprev);		// fill DT histogram
				tprev = dt;
			}
		} 
		
		if (dt >= tgrbini && dt <= tgrbfin){ // GRB data
			if (inevt.Etot>=emin && inevt.Etot<=emax) {
				int ibar = -1;
				double etot = -1.;
				int id;
				for (int k=0; k<30; k++) {
					id = inevt.bar[k];
					if (id>=0) {
						hbarsgrb->Fill(k);
						if ((inevt.E)[id] > etot) {
							etot = (inevt.E)[id];
							ibar = k;
						}
						// to uncomment if you want all the bars in the position histo
						if (k<15) hX->Fill(k, inevt.pos[id], inevt.E[id]); // fill h3X and h3Z
						else hZ->Fill(inevt.pos[id], k-15, inevt.E[id]);
					}
					if (id == inevt.mult - 1) break;
				}
				id = inevt.bar[ibar];
				// to uncomment if you want the most energetic bar only
				//if (ibar<15) hX->Fill(ibar, inevt.pos[id], inevt.Etot); // fill h3X and h3Z
				//else hZ->Fill(inevt.pos[id], ibar-15, inevt.Etot);
				hDTgrb->Fill(dt-tprev);		// fill DT histogram
				tprev = dt;
			}
			//if (VERBOSE) data->Show(-1, 30);
		} 
	}
	
	// normalization
	hbarsbkg->Sumw2();
	hbarsbkg->Scale(1./(tbkgfin-tbkgini));
	hbarsgrb->Sumw2();
	hbarsgrb->Scale(1./(tgrbfin-tgrbini));
	
	hDTbkg->Scale(1./(tbkgfin-tbkgini));
	hDTgrb->Scale(1./(tgrbfin-tgrbini));
	
	// build events/bin histogram and compare to Poisson distribution
	float *ncounts = new float[nbins];
	int n0 = 1;
	float nmax = 0.;
	float nmean = 0.;
	while (hclc->GetBinLowEdge(n0) < -10.) {
		ncounts[n0] = hclc->GetBinContent(n0);
		if (ncounts[n0] > nmax) nmax = ncounts[n0];
		nmean += ncounts[n0];
		n0++;
	}
	nmax += 2;
	hpoisson = new TH1F("hpoisson", "hpoisson", (int) nmax, 0., (int) nmax);
	for (int i=1; i<n0; i++) hpoisson->Fill(ncounts[i]);
	hpoisson->Sumw2();
	hpoisson->Scale(1./(n0-1));
	nmean /= (n0-1);
	cout << "GRB::StandardDiagnostic() : mean background = " << nmean << " events/bin" << endl;

	// graphic section
	cStdDiag = new TCanvas("cStdDiag", "Standard diagnostics", 1000, 900);
	cStdDiag->Divide(2, 3);
	
	// draw background events binning and compare to Poisson function
	cStdDiag->cd(1);
	hpoisson->Draw();
	TF1 *fpoisson = new TF1("fpoisson","TMath::PoissonI(x,[0])",0, nmax); 
	fpoisson->SetParameter(0, nmean); 
	fpoisson->SetLineColor(2);
	fpoisson->Draw("SAME");
	printf("n \tData \tPoisson\n");
	for (int i=0; i<nmax; i++) printf("%d \t%.4f \t%4f\n", i, hpoisson->GetBinContent(i+1), fpoisson->Eval(i)); 
	
	// draw bars histogram and compare bkg and grb normalized counts
	cStdDiag->cd(2);
	hbarsgrb->SetLineColor(2);
	hbarsgrb->GetXaxis()->SetTitle("bar #");
	hbarsgrb->GetXaxis()->SetTitleOffset(0.8);
	hbarsgrb->GetXaxis()->CenterTitle();
	hbarsgrb->GetYaxis()->SetTitle("rate (evt/s)");
	hbarsgrb->GetYaxis()->SetTitleOffset(0.9);
	hbarsgrb->GetYaxis()->CenterTitle();
	hbarsgrb->Draw();
	hbarsbkg->Draw("SAME");
	
	// draw DT histogram and compare bkg and grb normalized counts
	cStdDiag->cd(3);
	hDTgrb->SetLineColor(2);
	hDTgrb->Draw();
	hDTbkg->Draw("SAME");
	
	// draw position distribution for selected events
	// cPosDiag = new TCanvas("cPosDiag", "Position diagnostics", 2200/(nEbins + 1), 1000);
	// DrawPositionDiagnostic(cPosDiag);

	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB3908::DrawPositionDiagnostic(TPad *ppos)
{
	// draw position distribution for selected events
	ppos->Divide(2, nEbins + 1);
	
	// draw total position distribution
	
	hX->GetZaxis()->SetRange(0, nEbins+1);
	TH2F *haux = (TH2F *) hX->Project3D("yx");
	haux->SetName("hX_tot");
	haux->SetStats(0);
	haux->SetTitle("");
	hZ->GetZaxis()->SetRange(0, nEbins+1);
	TH2F *haux2 = (TH2F *) hZ->Project3D("yx");
	haux2->SetName("hZ_tot");
	haux2->SetStats(0);
	haux2->SetTitle("");
	double maxX = haux->GetMaximum();
	double maxZ = haux2->GetMaximum();
	double maxval = (maxX > maxZ ? maxX : maxZ);
	
	TLatex t2;
	t2.SetTextSize(0.06);
	char stitle[100];
	
	ppos->cd(1);
	gPad->SetRightMargin(0.2);
	sprintf(stitle, "Plane X  total position distribution");
	haux->GetZaxis()->SetRangeUser(0, maxval);
	haux->GetYaxis()->SetRangeUser(-20., 20.);
	haux->GetXaxis()->SetTitle("Bar number");
	haux->GetXaxis()->SetLabelSize(0.06);
	haux->GetXaxis()->SetTitleSize(0.06);
	haux->GetXaxis()->SetTitleOffset(0.8);
	haux->GetZaxis()->SetTitle("Counts");
	haux->GetZaxis()->CenterTitle();
	haux->GetZaxis()->SetTitleSize(0.06);
	haux->GetZaxis()->SetLabelSize(0.06);
	haux->GetXaxis()->CenterTitle();
	haux->GetYaxis()->SetTitle("Position (cm)");
	haux->GetYaxis()->SetLabelSize(0.06);
	haux->GetYaxis()->SetTitleSize(0.06);
	haux->GetYaxis()->SetTitleOffset(0.8);
	haux->GetYaxis()->CenterTitle();
	haux->Draw("COLZ");
	t2.DrawTextNDC(0.1, 0.92, stitle);
	
	ppos->cd(2);
	gPad->SetRightMargin(0.2);
	sprintf(stitle, "Plane Z  total position distribution");
	haux2->GetZaxis()->SetRangeUser(0, maxval);
	haux2->GetXaxis()->SetRangeUser(-20., 20.);
	haux2->GetXaxis()->SetTitle("Position (cm)");
	haux2->GetXaxis()->SetLabelSize(0.06);
	haux2->GetXaxis()->SetTitleSize(0.06);
	haux2->GetXaxis()->SetTitleOffset(0.8);
	haux2->GetZaxis()->SetTitle("Counts");
	haux2->GetZaxis()->CenterTitle();
	haux2->GetZaxis()->SetTitleSize(0.06);
	haux2->GetZaxis()->SetLabelSize(0.06);
	haux2->GetXaxis()->CenterTitle();
	haux2->GetYaxis()->SetTitle("Bar number");
	haux2->GetYaxis()->SetLabelSize(0.06);
	haux2->GetYaxis()->SetTitleSize(0.06);
	haux2->GetYaxis()->SetTitleOffset(0.8);
	haux2->GetYaxis()->CenterTitle();
	haux2->Draw("COLZ");
	t2.DrawTextNDC(0.1, 0.92, stitle);
	
	// draw energy selected position distribution
	for (int i=0; i<nEbins; i++) {
		TString hnameX("hX_");
		hnameX += i;
		hX->GetZaxis()->SetRange(i+1, i+1);
		TH2F *haux = (TH2F *) hX->Project3D("yx");
		haux->SetName(hnameX);
		haux->SetStats(0);
		haux->SetTitle("");
		
		TString hnameZ("hZ_");
		hnameZ += i;
		hZ->GetZaxis()->SetRange(i+1, i+1);
		TH2F *haux2 = (TH2F *) hZ->Project3D("yx");
		haux2->SetName(hnameZ);
		haux2->SetStats(0);
		haux2->SetTitle("");
		
		maxX = haux->GetMaximum();
		maxZ = haux2->GetMaximum();
		maxval = (maxX > maxZ ? maxX : maxZ);
	
		ppos->cd(2*i + 3);
		gPad->SetRightMargin(0.2);
		sprintf(stitle, "Plane X  %.2f - %.2f MeV", mcalEbin[i], mcalEbin[i+1]);
		haux->GetZaxis()->SetRangeUser(0, maxval);
		haux->GetYaxis()->SetRangeUser(-20., 20.);
		haux->GetXaxis()->SetTitle("Bar number");
		haux->GetXaxis()->SetLabelSize(0.06);
		haux->GetXaxis()->SetTitleSize(0.06);
		haux->GetXaxis()->SetTitleOffset(0.8);
		haux->GetZaxis()->SetTitle("Counts");
		haux->GetZaxis()->CenterTitle();
		haux->GetZaxis()->SetTitleSize(0.06);
		haux->GetZaxis()->SetLabelSize(0.06);
		haux->GetXaxis()->CenterTitle();
		haux->GetYaxis()->SetTitle("Position (cm)");
		haux->GetYaxis()->SetLabelSize(0.06);
		haux->GetYaxis()->SetTitleSize(0.06);
		haux->GetYaxis()->SetTitleOffset(0.8);
		haux->GetYaxis()->CenterTitle();
		haux->Draw("COLZ");
		t2.DrawTextNDC(0.1, 0.92, stitle);
	
		ppos->cd(2*i + 4);
		gPad->SetRightMargin(0.2);
		sprintf(stitle, "Plane Z  %.2f - %.2f MeV", mcalEbin[i], mcalEbin[i+1]);
		haux2->GetZaxis()->SetRangeUser(0, maxval);
		haux2->GetXaxis()->SetRangeUser(-20., 20.);
		haux2->GetXaxis()->SetTitle("Position (cm)");
		haux2->GetXaxis()->SetLabelSize(0.06);
		haux2->GetXaxis()->SetTitleSize(0.06);
		haux2->GetXaxis()->SetTitleOffset(0.8);
		haux2->GetZaxis()->SetTitle("Counts");
		haux2->GetZaxis()->CenterTitle();
		haux2->GetZaxis()->SetTitleSize(0.06);
		haux2->GetZaxis()->SetLabelSize(0.06);
		haux2->GetXaxis()->CenterTitle();
		haux2->GetYaxis()->SetTitle("Bar number");
		haux2->GetYaxis()->SetLabelSize(0.06);
		haux2->GetYaxis()->SetTitleSize(0.06);
		haux2->GetYaxis()->SetTitleOffset(0.8);
		haux2->GetYaxis()->CenterTitle();
		haux2->Draw("COLZ");
		t2.DrawTextNDC(0.1, 0.92, stitle);
	}
	return;
}


//
//--------------------------------------------------------------------
//

void GRB3908::DrawCustomLightCurve(double ti, double tf)
{
	cpXZ = new TCanvas("cpXZ", "AGILE-MCAL light curve", 3000/(GetNEbins() + 1), 1000);
	cpXZ->cd();
	DrawLightCurve((TPad *) gPad, ctpeak + ti, ctpeak + tf);
}

//
//--------------------------------------------------------------------
//

void GRB3908::SaveCustomLightCurve(TString outpath)
{
	if (!cpXZ) {
		cout << "*** GRB3908::SaveCustomLightCurve() ERROR: no canvas created yet!" << endl;
		return;
	}
	
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	fname += ".gif";
	cpXZ->Print(fname);
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::SaveDiagnostic(TString outpath)
{
	if (!cTotal) {
		cout << "*** GRB3908::SaveDiagnostic() ERROR: no canvas created yet!" << endl;
		return;
	}
	
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	// fname += "_pos.gif";
	// cPosDiag->Print(fname);
	
	// save a single TCanvas including light curve, position and bars distributions
	fname += "_diag.gif";
	cTotal->Print(fname);
	
	
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::SaveVSBDiagnostic(TString outpath)
{
	if (!cVSBDiag) {
		cout << "*** GRB3908::SaveVSBDiagnostic() ERROR: no canvas created yet!" << endl;
		return;
	}
	
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	fname += "_bars.gif";
	cVSBDiag->Print(fname);
	return;
}

//
//--------------------------------------------------------------------
//

float *GRB3908::GetPha(double ti, double tf, int nbL, double *spbinsL)
{
	int nb;
	double *spbins;
	
	if (nbL) {
		nb = nbL;
		spbins = spbinsL;
	} else {
		nb = nEbins;
		spbins = mcalEbin;
	}

	TH1F *hpha;

	hpha = new TH1F("hpha", "hpha", nb, spbins);

	cout << "GRB3908::GetPha() building spectrum in interval " << ti << " - " << tf << 
	        " respect to t0. Processing " << nevents << " events" << endl;
	//cout << "energy bands:" << endl;
	//for (int i=0; i<nb; i++) printf("%d \t%6f \t%6f\n", i, spbins[i], spbins[i+1]);
	
	// process data
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		//if (i<100) printf("%20.6f\n", inevt.time); // verified that inevt.time is conformal to FITS file time
		bool good = TRUE;
		
		// do time, and spatial selection
		if (inevt.time<t0+ti || inevt.time>t0+tf) good = FALSE;
		if (good) {
			int ibar = -1;
			double etot = -1.;
			for (int k=0; k<30; k++) {
				int id = inevt.bar[k];
				if (id>=0 && (inevt.E)[id] > etot) {
					etot = (inevt.E)[id];
					ibar = k;
				}
				if (id == inevt.mult - 1) break;
			}
			int ib=1;
			if (ibar<15) {
				ib<<=ibar;
				if (!(barsX & ib)) good = FALSE;			
			} else {
				ibar-=15;
				ib<<=ibar;
				if (!(barsZ & ib)) good = FALSE;
			}
		}
		if (good) hpha->Fill(inevt.Etot);
	}
	
	float *pha1 = new float[nb];
	for (int i=0; i<nb; i++) pha1[i] = hpha->GetBinContent(i+1);
	delete hpha;
	return pha1;
}

//
//--------------------------------------------------------------------
//

int GRB3908::SaveIPNMsg(TString outpath)
{
	if (t0 == 0) {
		cout << "GRB3908::SaveIPNMsg() : cannot save output message. Create custom light curve first (need t0)!" << endl;
		return 1;
	} else cout << "GRB3908::SaveIPNMsg() : Saving IPN time-only GCN notice." << endl;
	
/*
This is the format from latest Scott's mail:

TITLE:          IPN MESSAGE
NOTICE_DATE:    Tue 15 Jul 08 18:55:01 UT       <this ilne is mostly for human consumption>
NOTICE_TYPE:    IPN Time_only                   <either Time_only  or  Annulus>
TRIGGER_ID:     nnnn                            <mission-specific; 0 if unknown>
IPN_DATE:       YY/MM/DD
IPN_TIME:       sssss.ss SOD {hh:mm:ss.ss} UT
IPN_WINDOW:     ss.s  sec                       <0 if unknown or not available>
EVENT_DUR:      sss.s  sec                      <0 if unknown or not available>
GRB_CONF:       Definite                        <or Probably, Possibly, Def_not>
MISSION:        <mission-instrument identifier, see list of strings below>
OPERATIONAL:    Yes or No                       <No means: PwrOff, Cal_mode, SAA, etc>
BLIP_FOUND:     Yes or No                       <only appears if this is in response to another notice>
ECLIPTIC:       North   or   South   or   Middle_band   or   Unknown   <mostly for KW>
TEST_FLAG:      1   (or 0)                      <optional: another way to do the TEST-by-COMMENT method>
COMMENTS:       Whatever you want.  Put whatever you want -- go wild!
COMMENTS:       It will be copied char-for-char to the outgoing Notice.
COMMENTS:       There can be multiple comment lines.
COMMENTS:       THIS IS A TEST. REPEAT THIS IS A TEST.    <== DELETE FOR WORLD DISTRIBUTION.
*/
	
	FILE *fp;
	TString fname(outpath);
	fname += GetFileHeader()->Data();
	fname += "_GCN-time-only.txt";
	fp = fopen(fname.Data(), "w");
	
	TTimeStamp now;
	TTimeStamp daystart(year, month, day, 0, 0, 0, 0, 1, -epoch->GetSec());
	double binstart = t0 - daystart.GetSec();
	int csec =  (int) (binstart*100.)%100;
		
	fprintf(fp, "TITLE:          IPN MESSAGE\n");
	fprintf(fp, "NOTICE_DATE:    %s\n", now.AsString());
	fprintf(fp, "NOTICE_TYPE:    IPN Time_only\n");
	fprintf(fp, "TRIGGER_ID:     0\n");
	fprintf(fp, "IPN_DATE:       %02d/%02d/%02d\n", year-2000, month, day);
	fprintf(fp, "IPN_TIME:       %8.2f SOD {%02d:%02d:%02d.%02d} UT\n", binstart, hour, min, sec, csec);
	fprintf(fp, "IPN_WINDOW:     0.022 sec\n");
	fprintf(fp, "EVENT_DUR:      %5.1f\n", t90);
	fprintf(fp, "GRB_CONF:       Definite\n");
	fprintf(fp, "MISSION:        AGILE-MCAL\n");
	fprintf(fp, "OPERATIONAL:    Yes\n");
	fprintf(fp, "ECLIPTIC:       Unknown\n");
	fprintf(fp, "TEST_FLAG:      0\n");
	fprintf(fp, "COMMENTS:       AGILE-MCAL on-board triggered event\n");

	fclose(fp);
	
	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB3908::CalculatePeakFluxVSB()
{
	// calculate peak flux on selected histogram (suitable to work on the whole dataset)
	// it is not worth working on energy bands at this stage
	
	double tc;
	double val;
	
	// calculate i0: histogram index corresponding to t0
	
	while (hclc->GetBinLowEdge(i0)<=0.) i0++;
	i0--;
	cout << "GRB3908::CalculatePeakFluxVSB(): t0 at bin " << i0 << " corresponding to time " << hclc->GetBinLowEdge(i0) << endl;
	
	if (dt0 == 0.) {	// non sub-ms trigger search performed: look for the maximum peak and look around it
		int nsearch = (int) (tsearch/tbin +1.);
		cout << "GRB3908::CalculatePeakFluxVSB(): tsearch = " << tsearch << "; peak flux search bin interval: " << i0-nsearch << " - " << i0+nsearch << endl;
		for (int j=i0-nsearch; j<=i0+nsearch; j++) {
			tc = hclc->GetBinLowEdge(j);
			val = hclc->GetBinContent(j);
			if (val > cfpeak) {
				cfpeak = val;
				ctpeak = hclc->GetBinLowEdge(j);
				cbinpeak = j;
			}
		}
		ecfpeak = sqrt(cfpeak);		// NO BKG subtraction
		
	} else {		// sub-ms trigger search performed: look around the new t0
		for (int j=1; j<nbins; j++) {
			tc = hclc->GetBinLowEdge(j);
			if (tc<-0.010) continue;
			if (tc>0.010) break;
			val = hclc->GetBinContent(j);
			if (val > cfpeak) {
				cfpeak = val;
				ctpeak = hclc->GetBinLowEdge(j);
				cbinpeak = j;
			}
		}
		ecfpeak = sqrt(cfpeak);		// NO BKG subtraction
	
	}

	if (VERBOSE) printf("sel \t%6.1f - %8.1f \t%8.1f +/- %8.1f \t%8.3f \t%8.2f\n", 
		     emin, emax, cfpeak, ecfpeak, ctpeak, (cfpeak-cbkg)/sqrt(cbkg));

	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::SpectralAnalysisVSB()
{
	printf("\nOrbit: \t\t%d \n", info->runid);
	printf("Data type: \t%d \n", info->pkttype);
	printf("Time0:  \t%.6f (TT)   %s (UT) \n", t0, tzero->AsString("s"));
	printf("Temporal bin:  \t%.6f s \n", tbin);
	printf("Selection: \tEnergy range %6.2f - %6.2f MeV\n\t\tBars: %04X (X)  %04X (Z)\n", emin, emax, barsX, barsZ);
		
	// --------------- PEAK FLUX CALCULATION --------------
	
	CalculatePeakFluxVSB();
	
	
	// --------------- T90 CALCULATION --------------
	CalculateT90VSB();
	
	
	// --------------- FLUENCE CALCULATION IN T90 --------------
	CalculateFluenceVSB();	
	
	// --------------- MEAN ENERGY AND BARS DIAGNOSTIC --------------
	VSBDiagnostic();
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::CustomSpectralAnalysisVSB()
{
	// based on GRB::CustomSpectralAnalysis()
	printf("\nOrbit: \t\t%d \n", info->runid);
	printf("Data type: \t%d \n", info->pkttype);
	printf("Time0:  \t%.6f (TT)   %s (UT) \n", t0, tzero->AsString("s"));
	printf("Temporal bin:  \t%.6f s \n", tbin);
	printf("Selection: \tEnergy range %6.2f - %6.2f MeV\n\t\tBars: %04X (X)  %04X (Z)\n", emin, emax, barsX, barsZ);
	
	// --------------- PEAK FLUX CALCULATION --------------
	CalculatePeakFluxVSB();
	
	
	// --------------- T90 GRB TIME INTERVAL SETTING --------------
	double tc;
	double val;
	
	// if grb time are equal, get GRB time interval around peak flux
	if (tgrbini == tgrbfin) {
		tgrbini = ctpeak -fabs(tgrbini);
		tgrbfin = ctpeak +fabs(tgrbini);
	}
	
	// recalculate tgrbini and tgrbfin to match time bin
	cout << "tgrbini= " << tgrbini << endl;
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (fabs(tc-tgrbini)<0.9*tbin) {	// tc>=tgrbini non va bene quando tgrbini==0  && val>0.
			printf("%20.6f  %20.6f  %20.6f\n", hclc->GetBinLowEdge(j-1), tc, hclc->GetBinLowEdge(j+1));
			tgrbini = tc;
			i90lo = j;
			break;
		}
	}
	for (int j=1; j<nbins; j++) {
		tc = hclc->GetBinLowEdge(j);
		val = hclc->GetBinContent(j);
		if (fabs(tc-tgrbfin)<0.9*tbin ) {		// tc>=tgrbfin && val>0.
			tgrbfin = tc;
			i90hi = j;
			break;
		}
	}
	if (VERBOSE) cout << endl << "Rescaled GRB calculation interval to values " << tgrbini << "  -  " << tgrbfin 
			  << " s respect to t0" << endl;
	t90 = tgrbfin - tgrbini;
	et90 = tbin*sqrt(2.);	// two bins summed in quadrature
	
	if (VERBOSE) printf("Estimated T90 = %.3f +/- %.3f s\n", t90, et90);
	
	// --------------- FLUENCE CALCULATION IN T90 --------------
	CalculateFluenceVSB();	
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::CalculateT90VSB()
{
	// duration estimation method for Very Short Burst 
	// it's the total duration, T90 is misleading in this case but it's kept for consistency with the long GRB case
	// assume 0 background on ~ms timescale (0.36 evt/ms)
	// define candidate VSB as being separated by at least 5 (TBV) bins equal 0 

	double totcnts=0.;
	double auxbkg;
	double cnts;
	int nstop = 0;
	i90lo=cbinpeak;
	i90hi=cbinpeak;
	
	// estimate total number of counts
	for (int j=cbinpeak; j<nbins; j++) {
		cnts = hclc->GetBinContent(j);
		if (cnts == 0) nstop++;
		else i90hi = j;
		if (nstop >= 1) break;	// originally it was 5
		totcnts += cnts;
	}
	nstop = 0;
	i90hi++;
	for (int j=cbinpeak-1; j>0; j--) {
		cnts = hclc->GetBinContent(j);
		if (cnts == 0) nstop++;
		else i90lo = j;
		if (nstop >= 1) break;	// originally it was 5
		totcnts += cnts;	// now this is the total number of VSB events (total uncalibrated fluence)
	}
	if (1) cout << endl << "Estimated number of GRB counts = " << totcnts << " between bins " << i90lo << " - " << i90hi << " (peak bin = " << cbinpeak << ") corresponding to t-t0: " << hclc->GetBinLowEdge(i90lo) << " - " << hclc->GetBinLowEdge(i90hi) << endl;
	
	// re-define tgrbini and tgrbfin and set t90
	tgrbini = hclc->GetBinLowEdge(i90lo);
	tgrbfin = hclc->GetBinLowEdge(i90hi);
	
	t90 = tgrbfin - tgrbini;
	et90 = tbin*sqrt(2.);	// two bins summed in quadrature
	
	if (1) printf("Estimated T90 = %.6f +/- %.6f s\n", t90, et90);
	
	return;
}

//
//--------------------------------------------------------------------
//

void GRB3908::CalculateFluenceVSB()
{
	// fluence estimation method for Very Short Burst 
	// use the total duration calculated with GRB3908::CalculateT90VSB()
	// assume 0 background on ~ms timescale (0.36 evt/ms)
	
	double fhi=0.;
	double auxbkg;
	
	for (int i=0; i<nEbins; i++) fluence[i] = 0.;
	cfluence = 0.;
	
	// calculate total and grb counts (~ fluence) on different energy bands between t0 and t90
	for (int i= 0; i<nEbins; i++) {
		TH1D *haux = h2clc->ProjectionX("htmp", i+1, i+1, "");
		for (int j=i90lo; j<i90hi; j++) fluence[i] += haux->GetBinContent(j);
		delete haux;
		
		if (i>0) fhi += fluence[i];
		
		efluence[i] = sqrt(fluence[i]);
	}
	for (int j=i90lo; j<i90hi; j++) cfluence += hclc->GetBinContent(j);
	ecfluence = sqrt(cfluence);
	
	if (1) {
		printf("\nFluence in T90 (counts) \nid \tE band (MeV) \t\tFluence (counts)\n");
		for (int i=0; i<nEbins; i++) printf("%d \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", 
		     i, mcalEbin[i], mcalEbin[i+1], fluence[i], efluence[i]);
		printf("sel \t%6.1f - %6.1f \t%6.1f +/- %6.1f\n", emin, emax, cfluence, ecfluence); 
	}
	
	return;
}

//
//--------------------------------------------------------------------
//

int GRB3908::VSBDiagnostic()
{
	if (!t0 || !t90) {
		cout << "GRB3908::VSBDiagnostic() : cannot perform VSB diagnostic: no t0 specified or t90=0!" << endl;
		return 1;
	} else cout << "GRB3908::VSBDiagnostic() : performing VSB diagnostic on dataset" << endl;
	
	gbart = new TGraph(3);
	gbart->SetTitle("bars vs time");
	gEt = new TGraph(3);
	gEt->SetTitle("Energy vs time");
	int nb = 0;
	int nE = 0;
	
	// perform standard diagnostic on events
	double dt;
	printf("t_i = t0 + tgrbini = %20.6f \ntime serie: t-t_i   E(MeV)\n", t0);
	for (int i=0; i<nevents; i++) {
		data->GetEntry(i);
		dt = inevt.time - t0;
		
		// dump time serie for Fullekrug
		//if (dt >= tgrbini-0.010 && dt < tgrbfin+0.010) printf("%10.6f \t%6.2f\n", dt, inevt.Etot);

		// dump time serie to extract (with grep from logfile) high energy photons and plot them
		if (dt >= tgrbini && dt <= tgrbfin) printf("EVENT %06d \t%20.6f \t%6.2f\n", info->runid, inevt.time, inevt.Etot);
		
				
		if (dt >= tgrbini && dt < tgrbfin){ // GRB data
			avgE += inevt.Etot;
			if (inevt.Etot > maxE) maxE = inevt.Etot;
			// data->Show(i, 30);
			
			gEt->SetPoint(nE++, dt, inevt.Etot);

			for (int k=0; k<30; k++) {
				int id = inevt.bar[k];
				if (id>=0) {
					nbars[k]++;	// update bars histogram
					gbart->SetPoint(nb++, dt, k);
					
					if (k<8) {	// fill up the "cuboni" (big cubes) histogram
						 if (inevt.pos[id]<=0.) cuboni[0]++;
						 else cuboni[1]++;
					}
					if (k>=8 && k<15) {
						if (inevt.pos[id]<=0.) cuboni[2]++;
						else cuboni[3]++;
					}
					if (k>=15 && k<23) {
						if (inevt.pos[id]<=0.) cuboni[4]++;
						else cuboni[5]++;
					}
					if (k>=23) {
						if (inevt.pos[id]<=0.) cuboni[6]++;
						else cuboni[7]++;
					}
					
				}
				if (id == inevt.mult - 1) break;
			}
			
			// dump data to stdout
			if (VERBOSE) {
				printf("%6.0f  %8.3f  %2d |",(dt-tgrbini)*1.E6, inevt.Etot, inevt.mult);
				for (int k=0; k<30; k++) {
					int id = inevt.bar[k];
					printf("  %2d  %4d  %4d  %8.3f  %8.3f |", k, (id>=0 ? inevt.pdA[id] : 0), (id>=0 ? inevt.pdB[id] : 0), (id>=0 ? inevt.E[id] : 0.), (id>=0 ? inevt.pos[id] : 0.));
				}
				printf("\n");
			}
		} 
	}
	cout << "Total energy = " << avgE << " MeV in " << cfluence << " events: ";
	avgE /= cfluence;
	cout << avgE << " MeV average energy" << endl << "Bars occupation:  ";
	for (int i=0; i<30; i++) cout << nbars[i] << "  ";
	cout << endl << "Spatial regions occupation:  ";
	for (int i=0; i<8; i++) cout << cuboni[i] << "  ";
	cout << endl;
	cout << "Contact " << info->runid << " maximum energy in GRB interval = " << maxE << " MeV" << endl; 
	
	// graphic section
	// cVSBDiag = new TCanvas("cVSBDiag", "VSB diagnostics", 600, 800);
	// DrawVSBDiagnostic(cVSBDiag);

	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB3908::DrawVSBDiagnostic(TPad *pbar)
{
	pbar->Divide(1, 2);
	pbar->cd(1);
	gPad->SetLogy();
	gEt->SetMarkerStyle(22);
	gEt->SetMarkerSize(0.6);
	gEt->GetHistogram()->GetXaxis()->SetLabelSize(0.03);
	gEt->GetHistogram()->GetXaxis()->SetTitleSize(0.03);
	gEt->GetHistogram()->GetXaxis()->SetTitle("t-t_ini (s)");
	gEt->GetHistogram()->GetXaxis()->SetRangeUser(tgrbini, tgrbfin);
	gEt->GetHistogram()->GetYaxis()->SetLabelSize(0.03);
	gEt->GetHistogram()->GetYaxis()->SetTitleSize(0.03);
	gEt->GetHistogram()->GetYaxis()->SetTitle("Energy (MeV)");
	gEt->Draw("AP");
	
	pbar->cd(2);
	gbart->SetMarkerStyle(2);
	gbart->SetMarkerSize(0.5);
	gbart->GetHistogram()->GetXaxis()->SetLabelSize(0.03);
	gbart->GetHistogram()->GetXaxis()->SetTitleSize(0.03);
	gbart->GetHistogram()->GetXaxis()->SetTitle("t-t_ini (s)");
	gbart->GetHistogram()->GetXaxis()->SetRangeUser(tgrbini, tgrbfin);
	gbart->GetHistogram()->GetYaxis()->SetLabelSize(0.03);
	gbart->GetHistogram()->GetYaxis()->SetTitleSize(0.03);
	gbart->GetHistogram()->GetYaxis()->SetTitle("bar id");
	gbart->GetHistogram()->GetYaxis()->SetRangeUser(0., 29.);
	gbart->Draw("AP");
	return;
}

//
//--------------------------------------------------------------------
//

int GRB3908::SaveVSBInfo(TString outpath)
{
	
	FILE *fp;
	TString fname(outpath);
	fname += "VSB_info.txt";
	fp = fopen(fname.Data(), "w");
	
	fprintf(fp, "  %3d  %10.6f  %10.6f  %10.6f  %10.6f  %10.6f  %10.6f  %10.6f  %10.6f  %8.3f  %3d  ", nsubms, mint, dt0, tgrbini, t90, tbin, cbkg,  cfluence, cfpeak, avgE, nEbins);
	for (int i=0; i<nEbins; i++) fprintf(fp, "%10.6f  ", fluence[i]);
	for (int i=0; i<nEbins; i++) fprintf(fp, "%10.6f  ", bkg[i]);
	for (int i=0; i<30; i++)     fprintf(fp, "%3d ", nbars[i]);
	for (int i=0; i<8; i++)      fprintf(fp, "%3d ", cuboni[i]);
	fprintf(fp, "\n");
	fclose(fp);
	
	return 0;
}

//
//--------------------------------------------------------------------
//

double GRB3908::FindTrueT0(double t0L, double dt, int thr)
{
	// implementation of submillisecond burst search logic
	
	int nevt=0;
	double t0test=-1.;
	double *auxt = new double[nevents];
	
	for (int i=0; i<nevents; i++) {		// get a subset of the time array around t0
		data->GetEntry(i);
		if (inevt.time>t0L-3.) {
			if (inevt.time<t0L+3.) {
				auxt[nevt++] = inevt.time;
			} else break;
		}
	}
	
	int ntrg=0;
	for (int i=0; i<nevt-thr; i++) {	// scan reduced time array to find event clusters above threshold
		if (auxt[i+thr-1] - auxt[i] <= dt) {
			ntrg++;
			dt0 = auxt[i] - t0L;
			if (ntrg == 1) {
				printf("SUB-MS trigger found at t = %20.6f - delta-t = %10.6f s respect to default t0\n", auxt[i], dt0);
				auxt[nsubms++] = dt0;	// store at position nsubms the t0 for the new trigger found
			}
		} else {
			ntrg = 0;
		}
	}
	cout << "GRB3908::FindTrueT0(): found a total number of " <<  nsubms << " independent sub-ms triggers" << endl;
	
	// search for the new trigger time closest to t0
	for (int i=0; i<nsubms; i++) {
		if (fabs(auxt[i]) < fabs(dt0)) dt0 = auxt[i];
	}
	delete auxt;
	
	return dt0;
}

//
//--------------------------------------------------------------------
//

void GRB3908::DrawDiagnostic(double ti, double tf)
{
	cTotal = new TCanvas("cTotal", "AGILE-MCAL trigger diagnostic", 3*2200/(nEbins + 1), 1000);
	cTotal->Divide(3,1);
	cTotal->cd(1);
	DrawLightCurve((TPad *) gPad, ctpeak + ti, ctpeak + tf);
	
	cTotal->cd(2);
	DrawPositionDiagnostic((TPad *) gPad);
	
	cTotal->cd(3);
	DrawVSBDiagnostic((TPad *) gPad);
	
	return;
}
