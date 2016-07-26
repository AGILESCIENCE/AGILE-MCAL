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
#include "MCVFile.h"
#include "MCVCalibration.h"

ClassImp(MCVCalibration);

MCVCalibration::MCVCalibration()
{
	// load calibration parameters from configuration file
	TString confname;
	confname.Append(getenv("MCALSW"));
	confname += STANDARD_CONF;
	stdconf = new TEnv(confname);
	
	calibName.Append(getenv("MCALSW"));
	calibName += stdconf->GetValue(".burst.cal.file.","burst.conf");
	cout << "Loading calibration parameters for IASF-BO calibrations: " << calibName.Data() << endl;
	
	// load calibration parameters
	conf = new TEnv(calibName);
	char parameterName[50];
	int ii;
	for (UInt_t i=0; i<30; i++) {  
		if (i<15) ii=i;
		else ii=i+1;
		sprintf(parameterName, ".Gain.Bar%02d.A", ii);
		u0A[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Gain.Bar%02d.B", ii);
		u0B[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Alpha.Bar%02d.A", ii);
		alphaA[i] = conf->GetValue(parameterName, 1.); 
		sprintf(parameterName, ".Alpha.Bar%02d.B", ii);
		alphaB[i] = conf->GetValue(parameterName, 1.);  
		sprintf(parameterName, ".Offset.Bar%02d.A", ii);
		oA[i] = conf->GetValue(parameterName, 0.);  
		sprintf(parameterName, ".Offset.Bar%02d.B", ii);
		oB[i] = conf->GetValue(parameterName, 0.);   
	}
	XDatFile.Append(getenv("MCALSW"));
	XDatFile += stdconf->GetValue(".x.dat.file.","x.dat");
	ZDatFile.Append(getenv("MCALSW"));
	ZDatFile += stdconf->GetValue(".z.dat.file.","z.dat");
	calFile.Append(getenv("MCALSW"));
	calFile += stdconf->GetValue(".peak.dat.file.","peak.dat");
	GetXDataStatus();
	GetZDataStatus();
	
	// init histo/graph collections
	hcpdA  = new TObjArray(30);
	hcpdB  = new TObjArray(30);
	hcBonA = new TObjArray(30);
	for (int i=0; i<20; i++) poserr[i]=0.1;	//error in position due to collimator opening (2mm)
	SetGraph();
}  


MCVCalibration::~MCVCalibration()
{
	cout << "Deleting..." << endl;
	return;
}

//
// -------------------------------------------
//

void MCVCalibration::GetXDataStatus()
{
	int 	runid;
	int 	countX=0;
	float 	X, Y, ya;
	float	cent, sigma, ampl, centErr, sigmaErr, amplErr;
	int 	bar1[]={0, 1, 2, 3, 4};
	int 	bar2[]={5, 6, 7, 8, 9};
	int 	bar3[]={10, 11, 12, 13, 14};
	int 	bar4[]={0, 1, 2, 3, 4, 5, 6, 7};
	int 	bar5[]={8, 9, 10, 11, 12, 13, 14};
	int 	npoints;
	int 	*bars;
	char	peakid[50];
	const char 	*parameters; 
	TEnv *cal;
	cal = new TEnv(calFile.Data());   

// work on X plane calibration: read file with run-id - position correspondence
  	FILE *f1;
  	f1=fopen(XDatFile.Data(), "r");
  	if (!f1) {
    		cout << "*** No file " << XDatFile.Data() << " found!" << endl;
    		return;
  	}
	while ( !feof(f1) ) {
  		fscanf(f1, "%d %f %f", &runid, &X, &Y);
		cout << "run: " << runid << "  X= " << X << "  Y= " << Y << endl;
		if (X == -145.) {
			bars=bar3;
			npoints=5;
		} else if (X == 0.) {
			bars=bar2;
			npoints=5;
		} else if (X == 145.) {
			bars=bar1;
			npoints=5;
		} else if (X == -87.) {
			bars=bar5;
			npoints=7;
		} else if (X == 87.) {
			bars=bar4;
			npoints=8;
		} else {
			cout << "*** Wrong X position!" << endl;
			return;
		} 
		ya = -0.1*Y;
		for (int i=0; i<npoints; i++) {
			// look for already processed peaks
			int complete = 0;
			sprintf(peakid, ".Run%d.Bar%02d.%c.cal.", runid, bars[i], 'A');
			parameters = cal->GetValue(peakid, "void");
//			cout << peakid << "    " << parameters << endl;
			if (parameters != "void") {
				complete+=1;
				sscanf(parameters, "%f %f %f %f %f %f", &cent, &sigma, &ampl, &centErr, &sigmaErr, &amplErr);
				cout << "center= " << cent << " +/- " << centErr << "  sigma= " << sigma << " +/- " << sigmaErr << endl;
				int ibar = bars[i];
				int ihit = nhit[ibar][0];
				data[ibar][0][0][ihit] = ya;	// old version: 18.75 + ya
				data[ibar][0][1][ihit] = cent - oA[ibar]; 
				data[ibar][0][2][ihit] = centErr;
				(nhit[ibar][0])++;
			}
			sprintf(peakid, ".Run%d.Bar%02d.%c.cal.", runid, bars[i], 'B');
			parameters = cal->GetValue(peakid, "void");
//			cout << peakid << "    " << parameters << endl;
			if (parameters != "void") {
				complete+=2;
				sscanf(parameters, "%f %f %f %f %f %f", &cent, &sigma, &ampl, &centErr, &sigmaErr, &amplErr);
				cout << "center= " << cent << " +/- " << centErr << "  sigma= " << sigma << " +/- " << sigmaErr << endl;
				int ibar = bars[i];
				int ihit = nhit[ibar][1];
				data[ibar][1][0][ihit] = ya;	// old version: 18.75 - ya;
				data[ibar][1][1][ihit] = cent - oB[ibar]; 
				data[ibar][1][2][ihit] = centErr;
				(nhit[ibar][1])++;				
			}
			int color = 1;
			switch (complete) {
				case 1:
					color = 2;
					break;
				case 2:
					color = 5;
					break;
				case 3:
					color = 3;
					break;
 		                default:
            			        break;
			}
/*			TMarker *m = new TMarker(bars[i]+1, ya, 21);
			m->SetMarkerSize(0.4);
			m->SetMarkerColor(color);
			m->Draw();*/
		}
		countX++;
	}
	fclose(f1);
	delete cal;
	SortData();
}

//
// -------------------------------------------
//

void MCVCalibration::GetZDataStatus()
{
	int 	runid;
	int 	countZ=0;
	float 	X, Y, ya;
	float	cent, sigma, ampl, centErr, sigmaErr, amplErr;
	int 	bar1[]={16, 17, 18, 19, 20};
	int 	bar2[]={21, 22, 23, 24, 25};
	int 	bar3[]={26, 27, 28, 29, 30};
	int 	npoints;
	int 	*bars;
	char	peakid[50];
	const char 	*parameters; 
	char parameterName[50];
	
	TEnv *cal;
	cal = new TEnv(calFile.Data());   

// work on Z plane calibration: read file with run-id - position correspondence
  	FILE *f1;
  	f1=fopen(ZDatFile.Data(), "r");
  	if (!f1) {
    		cout << "*** No file " << ZDatFile.Data() << " found!" << endl;
    		return;
  	}
	while ( !feof(f1) ) {
  		fscanf(f1, "%d %f %f", &runid, &X, &Y);
		cout << "run: " << runid << "  X= " << X << "  Y= " << Y << endl;
		if (X == -145.) {
			bars=bar3;
			npoints=5;
		} else if (X == 0.) {
			bars=bar2;
			npoints=5;
		} else if (X == 145.) {
			bars=bar1;
			npoints=5;
		} else {
			cout << "*** Wrong X position!" << endl;
			return;
		} 
		ya = -0.1*Y;
		for (int i=0; i<npoints; i++) {
			// look for already processed peaks
			int complete = 0;
			sprintf(peakid, ".Run%d.Bar%02d.%c.cal.", runid, bars[i], 'A');
			parameters = cal->GetValue(peakid, "void");
//			cout << peakid << "    " << parameters << endl;
			if (parameters != "void") {
				complete+=1;
				sscanf(parameters, "%f %f %f %f %f %f", &cent, &sigma, &ampl, &centErr, &sigmaErr, &amplErr);
				cout << "center= " << cent << " +/- " << centErr << "  sigma= " << sigma << " +/- " << sigmaErr << endl;
				int ibar = bars[i]-1;
				int ihit = nhit[ibar][0];
				data[ibar][0][0][ihit] = ya;	// old version: 18.75 + ya
				data[ibar][0][1][ihit] = cent - oA[ibar]; 
				data[ibar][0][2][ihit] = centErr;
				(nhit[ibar][0])++;
			}
			sprintf(peakid, ".Run%d.Bar%02d.%c.cal.", runid, bars[i], 'B');
			parameters = cal->GetValue(peakid, "void");
//			cout << peakid << "    " << parameters << endl;
			if (parameters != "void") {
				complete+=2;
				sscanf(parameters, "%f %f %f %f %f %f", &cent, &sigma, &ampl, &centErr, &sigmaErr, &amplErr);
				cout << "center= " << cent << " +/- " << centErr << "  sigma= " << sigma << " +/- " << sigmaErr << endl;
				int ibar = bars[i]-1;
				int ihit = nhit[ibar][1];
				data[ibar][1][0][ihit] = ya;	// old version: 18.75 - ya;
				data[ibar][1][1][ihit] = cent - oB[ibar]; 
				data[ibar][1][2][ihit] = centErr;
				(nhit[ibar][1])++;				
			}
			int color = 1;
			switch (complete) {
				case 1:
					color = 2;
					break;
				case 2:
					color = 5;
					break;
				case 3:
					color = 3;
					break;
 		                default:
            			        break;
			}
/*			TMarker *m = new TMarker(ya, bars[i]-15, 21);
			m->SetMarkerSize(0.4);
			m->SetMarkerColor(color);
			m->Draw();*/
		}
		countZ++;
	}
	fclose(f1);
	delete cal;
	SortData();
}

//
// -------------------------------------------
//

void MCVCalibration::SortData()
{
	// sort data by means of straight insertion method (see Numerical Recipes chapter 8.1)
	float aux0, aux1, aux2;
	int nn, i, j;
	for (int ibar=0; ibar<31; ibar++) {
		for (int ipd=0; ipd<2; ipd++) {
			if (nhit[ibar][ipd]>1) {
				nn = nhit[ibar][ipd];
				for (j=1; j<nn; j++) {					// pick out each element in turn
					aux0 = data[ibar][ipd][0][j];
					aux1 = data[ibar][ipd][1][j];
					aux2 = data[ibar][ipd][2][j];
					i = j-1;
					while (i>=0 && data[ibar][ipd][0][i] > aux0) {	// look for the place to insert it
						data[ibar][ipd][0][i+1] = data[ibar][ipd][0][i];
						data[ibar][ipd][1][i+1] = data[ibar][ipd][1][i];
						data[ibar][ipd][2][i+1] = data[ibar][ipd][2][i];
						i--;
					}
					data[ibar][ipd][0][i+1] = aux0;			// insert it!
					data[ibar][ipd][1][i+1] = aux1;
					data[ibar][ipd][2][i+1] = aux2;
				}
			}
		}
	}
}

//
// -------------------------------------------
//

void MCVCalibration::SetGraph()
{
 	// remember:	nhit[31][2];		//[bar id][PD (0=A, 1=B)] static
	// remember:	data[31][2][3][30];	//[bar id][PD (0=A, 1=B)][x(cm), y(ch), y_err(ch)][point]
	
	int nhA, nhB, nhBonA;			// number of hits
	float xBonA[30], yBonA[30], dyBonA[30];
	
	for (int i=0; i<30; i++) {
		nhA = nhit[i][0];
		nhB = nhit[i][1];	
		TGraphErrors *grA = new TGraphErrors(nhA, &data[i][0][0][0], &data[i][0][1][0], poserr, &data[i][0][2][0]);
		TString gnameA("gA");
		TString gnameB("gB");
		TString gnameBonA("gBonA");
		gnameA += i;
		gnameB += i;
		gnameBonA += i;
		
		grA->GetHistogram()->SetXTitle("Position (cm)");
		grA->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
		grA->GetHistogram()->GetXaxis()->CenterTitle();
		grA->GetHistogram()->SetYTitle("Peak center (ADC counts)");
		grA->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		grA->GetHistogram()->GetYaxis()->CenterTitle();	
		grA->SetLineColor(4);
		grA->SetName(gnameA.Data());
		hcpdA->AddLast(grA);
			
		TGraphErrors *grB = new TGraphErrors(nhB, &data[i][1][0][0], &data[i][1][1][0], poserr, &data[i][1][2][0]);
		grB->SetName(gnameB.Data());
		hcpdB->AddLast(grB);
		
		// build the ln(B/A) graphs
		nhBonA = 0;
		for (int j=0; j< nhB; j++) {
			for (int k=0; k<nhA; k++) {
				if (data[i][1][0][j] == data[i][0][0][k]) {	// if measure at both PDs is available
					xBonA[nhBonA] = data[i][1][0][j];
					yBonA[nhBonA] = log(data[i][1][1][j] / data[i][0][1][k]);
					dyBonA[nhBonA] = data[i][1][2][j]/data[i][1][1][j] + data[i][0][2][k]/data[i][0][1][k];
					nhBonA++;
					break;
				}
			}
		}
		TGraphErrors *grBonA = new TGraphErrors(nhBonA, xBonA, yBonA, poserr, dyBonA);
		grBonA->SetName(gnameBonA.Data());
		hcBonA->AddLast(grBonA);
	}

}

//
// -------------------------------------------
//

// TO BE COMPLETED

void MCVCalibration::FitCurve(int bar, int model)
{
/*	// do model fit on the selected bar
	// model = 0: standard exponential fit on all data
	//       = 1: exponential fit in the bar center + pol3 fit at the borders
	//       = 2: full polinomial fit

	float poserr[20];		// error in position
	float resA[20], resB[20];	// residuals in %
	float reserrA[20], reserrB[20];	// error on residuals in %
//	float peakerr[20];
	float dy;
	float gainA, gainB;
	int nhA, nhB;			// number of hits
	char grtitle[50], grtitlecompl[50];
	TF1 *totalA, *totalB;
	
	cbar = bar;
	nhA = nhit[bar][0];
	nhB = nhit[bar][1];
	for (int i=0; i<20; i++) {
		poserr[i]=0.1;	//error in position due to collimator opening (2mm)
//		peakerr[i]=1.;	// estimated error on the peak center in ADC channels
	}
	
	if (bar<0 || bar>30) {
		cout << "*** Wrong bar address (" << bar << "). Exiting!" << endl;
		return;
	}
	
	// build output graphs and do exponential fit
	TGraphErrors *grA = hcpdA->
	if (bar<15) sprintf(grtitle, "Bar %d (X%d) - 1.275 MeV peaks", bar, bar+1);
	else sprintf(grtitle, "Bar %d (Z%d) - 1.275 MeV peaks", bar, bar-14);
	grA->GetHistogram()->SetXTitle("Position (cm)");
	grA->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
	grA->GetHistogram()->GetXaxis()->CenterTitle();
	grA->GetHistogram()->SetYTitle("Peak center (ADC counts)");
	grA->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
	grA->GetHistogram()->GetYaxis()->CenterTitle();	
	grA->SetLineColor(4);
	grA->Draw("AP");
		
	TGraphErrors *grB = new TGraphErrors(nhB, &data[bar][1][0][0], &data[bar][1][1][0], poserr, &data[bar][1][2][0]);
	grB->Draw("PSAME");
	
	switch (model) {
		case 0:		// expo model
			totalA = ExpoModel(grA, 0);
			totalB = ExpoModel(grB, 1);
			sprintf(grtitlecompl, "%s - Full exponential model", grtitle);
			break;
		case 1:		// expo + pol3 model
			totalA = ExpoPolModel(grA, 0);
			totalB = ExpoPolModel(grB, 1);
			sprintf(grtitlecompl, "%s - Exponential + pol3 model", grtitle);
			break;
		case 2:		// polinomial model
			totalA = PolModel(grA, 0);
			totalB = PolModel(grB, 1);
			sprintf(grtitlecompl, "%s - Full polinomial model", grtitle);
			break;
		default:
			break;
	}
	grA->SetTitle(grtitlecompl);
	
	// build and draw residuals graphs
	spd2->cd();	// work on PD-A
	for (int i=0; i<nhA; i++) {
		dy = data[bar][0][1][i];
		resA[i] = 100.*(totalA->Eval(data[bar][0][0][i]) - dy)/dy;
		reserrA[i] = 100.*data[bar][0][2][i]/dy; 
	}
	TGraphErrors *grResA = new TGraphErrors(nhA, &data[bar][0][0][0], resA, poserr, reserrA);
	grResA->SetTitle("");
	grResA->GetHistogram()->SetXTitle("Position (cm)");
	grResA->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
	grResA->GetHistogram()->GetXaxis()->CenterTitle();
	grResA->GetHistogram()->SetYTitle("PD-A Residual (%)");
	grResA->GetHistogram()->GetYaxis()->SetTitleSize(0.1);
	grResA->GetHistogram()->GetYaxis()->SetLabelSize(0.1);
	grResA->GetHistogram()->GetYaxis()->SetTitleOffset(0.5);
	grResA->GetHistogram()->GetYaxis()->CenterTitle();	
	grResA->Draw("AP");
	TLine *lA = new TLine(-20., 0., 20., 0.);
	lA->Draw();
	
	spd3->cd();	// work on PD-B
	for (int i=0; i<nhB; i++) {
		dy = data[bar][1][1][i];
		resB[i] = 100.*(totalB->Eval(data[bar][1][0][i]) - dy)/dy;
		reserrB[i] = 100.*data[bar][1][2][i]/dy; 
	}
	TGraphErrors *grResB = new TGraphErrors(nhB, &data[bar][1][0][0], resB, poserr, reserrB);
	grResB->SetTitle("");
	grResB->GetHistogram()->SetXTitle("Position (cm)");
	grResB->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
	grResB->GetHistogram()->GetXaxis()->CenterTitle();
	grResB->GetHistogram()->SetYTitle("Residual (%)");
	grResB->GetHistogram()->GetYaxis()->SetTitleSize(0.1);
	grResB->GetHistogram()->GetYaxis()->SetLabelSize(0.1);
	grResB->GetHistogram()->GetYaxis()->SetTitleOffset(0.5);
	grResB->GetHistogram()->GetYaxis()->CenterTitle();	
	grResB->Draw("AP");
	TLine *lB = new TLine(-20., 0., 20., 0.);
	lB->Draw();
	
	calib = TRUE;
	c1->Update();
	*/
	return;
}
/*
//
// -------------------------------------------
//

void MCVCalibration::CalcSplinePar(int bar, int pd)
{
	double par[2];
	if (bar<0 || bar>30 || pd<0 || pd>1) {
		cout << "*** Wrong bar address (" << bar << ") or pd (" << pd << "). Exiting!" << endl;
		return;
	}
	TCanvas *c1 = fDisplay->GetCanvas();
	c1->cd(1);
	TGraph *grA = new TGraph(nhit[bar][0], &data[bar][0][0][0], &data[bar][0][1][0]);
	grA->SetMarkerStyle(21);
	grA->Draw("AP");
	c1->Update();
	TGraph *grB = new TGraph(nhit[bar][1], &data[bar][1][0][0], &data[bar][1][1][0]);
	grB->SetMarkerStyle(22);
	grB->Draw("Psame");
	c1->Update();

	// do traditional exponential fit
	TF1 *total = new TF1("total","expo", -19., 19.);
	total->SetLineColor(2);
   	grA->Fit("total","R0+");
	total->SetLineWidth(1);
   	total->Draw("same");
	total->GetParameters(par);
	float gainA, alphaA;
	gainA = total->Eval(-18.75)/1.275;
	alphaA = -par[1];
	cout << "Gain at PD A edge= " << gainA << " ch/MeV " << endl;
	cout << "Alpha A = " << alphaA << " cm^-1" << endl;
	
	TF1 *pA = new TF1("pA","pol5", -20., 20.);		// polinomial fit
	pA->SetLineColor(6);
   	grA->Fit("pA","FR0+");
	pA->SetLineWidth(1);
   	pA->Draw("same");
	
	TF1 *totalB = new TF1("totalB","expo", -19., 19.);
	totalB->SetLineColor(2);
   	grB->Fit("totalB","R0+");
	totalB->SetLineWidth(1);
   	totalB->Draw("same");
	totalB->GetParameters(par);
	float gainB, alphaB;
	gainB = totalB->Eval(18.75)/1.275;
	alphaB = par[1];
	cout << "Gain at PD B edge= " << gainB << " ch/MeV " << endl;
	cout << "Alpha B = " << alphaB << " cm^-1" << endl;
	
	TF1 *pB = new TF1("pB","pol5", -20., 20.);		// polinomial fit
	pB->SetLineColor(7);
   	grB->Fit("pB","FR0+");
	pB->SetLineWidth(1);
   	pB->Draw("same");
	
	c1->Update();
	
	// side A: init cubic spline interpolation arrays
  	cout << "init spline, n_hits=" << nhit[bar][pd] << endl; 
  	Spline(&data[bar][0][0][0]-1, &data[bar][0][1][0]-1, nhit[bar][0], 1e30, 1e30, &CLN::y2[bar][0][0]);  // NB: -1 for Numerical Recipes array conventions
	
	cout << endl << "PD-A Spline parameters: " << endl;
  	for (int i=1; i<nhit[bar][0]+1; i++) cout << data[bar][0][0][i-1] << "  " << data[bar][0][1][i-1] << "  " << CLN::y2[bar][0][i] << endl;
	
	cout << endl << "PD-A Sample interpolation: " << endl;
	for (CLN::X=-19.; CLN::X<=19.; CLN::X++) {
		Splint(&data[bar][0][0][0]-1, &data[bar][0][1][0]-1, &CLN::y2[bar][0][0], nhit[bar][0], CLN::X, &CLN::Y);
		cout << CLN::X << "  " << CLN::Y << endl;
	}
	
	TF1 *funbar;
	funbar = new TF1("funbar", CLN::barf, -19., 19., 2);
	funbar->SetParameters(bar, 0);
	funbar->SetLineWidth(1);
	funbar->SetLineColor(3);
	funbar->Draw("same");
	
	// side B: init cubic spline interpolation arrays
  	cout << "init spline, n_hits=" << nhit[bar][1] << endl; 
  	Spline(&data[bar][1][0][0]-1, &data[bar][1][1][0]-1, nhit[bar][1], 1e30, 1e30, &CLN::y2[bar][1][0]);  // NB: -1 for Numerical Recipes array conventions
	
	cout << endl << "PD-B Spline parameters: " << endl;
  	for (int i=1; i<nhit[bar][1]+1; i++) cout << data[bar][1][0][i-1] << "  " << data[bar][1][1][i-1] << "  " << CLN::y2[bar][1][i] << endl;
	
	cout << endl << "PD-B Sample interpolation: " << endl;
	for (CLN::X=-19.; CLN::X<=19.; CLN::X++) {
		Splint(&data[bar][1][0][0]-1, &data[bar][1][1][0]-1, &CLN::y2[bar][1][0], nhit[bar][1], CLN::X, &CLN::Y);
		cout << CLN::X << "  " << CLN::Y << endl;
	}
	
	TF1 *funbarB;
	funbarB = new TF1("funbarB", CLN::barf, -19., 19., 2);
	funbarB->SetParameters(bar, 1);
	funbarB->SetLineWidth(1);
	funbarB->SetLineColor(3);
	funbarB->Draw("same");
	
	c1->Update();
	
	// work on function KA(x) = fA(x)/fB(x)
	
	if (nhit[bar][0] != nhit[bar][1]) {
		cout << "Different number of points in A and B. Cannot proceed evaluating K(x) function!" << endl;
		return;
	}
	c1->cd(2);
	for (int i=0; i<nhit[bar][0]; i++) {	// set the A/B array
		dataK[bar][0][0][i] = data[bar][0][0][i];
		dataK[bar][1][0][i] = data[bar][1][0][i];
		dataK[bar][0][1][i] = data[bar][0][1][i] / data[bar][1][1][i];
		dataK[bar][1][1][i] = data[bar][1][1][i] / data[bar][0][1][i];
	}
  	cout << "init spline for KA function" << endl; 
  	Spline(&dataK[bar][0][0][0]-1, &dataK[bar][0][1][0]-1, nhit[bar][0], 1e30, 1e30, &CLN::y2K[bar][0][0]);  
	
	cout << endl << "KA(x) spline parameters: " << endl;
  	for (int i=1; i<nhit[bar][0]+1; i++) cout << dataK[bar][0][0][i-1] << "  " << dataK[bar][0][1][i-1] << "  " << CLN::y2K[bar][0][i] << endl;
	
	cout << endl << "KA(x) sample interpolation: " << endl;
	for (CLN::X=-19.; CLN::X<=19.; CLN::X++) {
		Splint(&dataK[bar][0][0][0]-1, &dataK[bar][0][1][0]-1, &CLN::y2K[bar][0][0], nhit[bar][0], CLN::X, &CLN::Y);
		cout << CLN::X << "  " << CLN::Y << endl;
	}
	
	TGraph *grKA = new TGraph(nhit[bar][0], &dataK[bar][0][0][0], &dataK[bar][0][1][0]);
	grKA->SetMarkerStyle(21);
	grKA->Draw("AP");
	
	TF1 *funbarKA;
	funbarKA = new TF1("funbarKA", CLN::bark, -19., 19., 2);
	funbarKA->SetParameters(bar,0);
	funbarKA->SetLineWidth(1);
	funbarKA->SetLineColor(3);
	funbarKA->Draw("same");
	
	TF1 *polfit5A = new TF1("polfit5A","pol5", -20., 20.);
	polfit5A->SetLineColor(4);
   	grKA->Fit("polfit5A","R0+");
	polfit5A->SetLineWidth(1);
   	polfit5A->Draw("same");

	pA->GetParameters(&param[0]);
	pB->GetParameters(&param[6]);
	for (int k=0; k<12; k++) cout << "pA/pB parameter " << k << " = " << param[k] << endl;
	
	TF1 *polratioA = new TF1("polratioA","pol5(0)/pol5(6)", -20., 20.);  // "pA/pB"
	polratioA->SetParameters(param);
	polratioA->SetLineColor(8);
	polratioA->SetLineWidth(1);
   	polratioA->Draw("same");

	double param2[12];
	pA->GetParameters(&param2[6]);
	pB->GetParameters(&param2[0]);
	TF1 *polratioB = new TF1("polratioB","pol5(0)/pol5(6)", -20., 20.);  // "pA/pB"
	polratioB->SetParameters(param2);
	polratioB->SetLineColor(9);
	polratioB->SetLineWidth(1);
   	polratioB->Draw("same");

	// work on function KB(x) = fB(x)/fA(x)
	
  	cout << "init spline for KB function" << endl; 
  	Spline(&dataK[bar][1][0][0]-1, &dataK[bar][1][1][0]-1, nhit[bar][0], 1e30, 1e30, &CLN::y2K[bar][1][0]);  
	
	cout << endl << "KB(x) spline parameters: " << endl;
  	for (int i=1; i<nhit[bar][0]+1; i++) cout << dataK[bar][1][0][i-1] << "  " << dataK[bar][1][1][i-1] << "  " << CLN::y2K[bar][1][i] << endl;
	
	cout << endl << "KB(x) sample interpolation: " << endl;
	for (CLN::X=-19.; CLN::X<=19.; CLN::X++) {
		Splint(&dataK[bar][1][0][0]-1, &dataK[bar][1][1][0]-1, &CLN::y2K[bar][1][0], nhit[bar][0], CLN::X, &CLN::Y);
		cout << CLN::X << "  " << CLN::Y << endl;
	}
	
	TGraph *grKB = new TGraph(nhit[bar][0], &dataK[bar][1][0][0], &dataK[bar][1][1][0]);
	grKB->SetMarkerStyle(22);
	grKB->Draw("Psame");
	
	TF1 *funbarKB;
	funbarKB = new TF1("funbarKB", CLN::bark, -19., 19., 2);
	funbarKB->SetParameters(bar,1);
	funbarKB->SetLineWidth(1);
	funbarKB->SetLineColor(3);
	funbarKB->Draw("same");
	
	TF1 *polfit5B = new TF1("polfit5B","pol5", -20., 20.);
	polfit5B->SetLineColor(5);
   	grKB->Fit("polfit5B","R0+");
	polfit5B->SetLineWidth(1);
   	polfit5B->Draw("same");
	
	cout << endl << "Test inversion:" << endl;
	for (float yy=0.7; yy<2.3; yy+=0.1) {
		double xx = funbarKB->GetX(yy, -19., 19.);
		cout << yy << "  " << xx << endl;
	}
	
	// check whether KA(x) is a good approximation of fA(x)/fB(x)
	
	TF1 *funbarAoverB;
	funbarAoverB = new TF1("funbarAoverB", CLN::barfAoverB, -19., 19., 2);
	funbarAoverB->SetParameters(bar,0);
	funbarAoverB->SetLineWidth(1);
	funbarAoverB->SetLineColor(2);
	funbarAoverB->Draw("same");
	
	cout << endl << "K(x) vs fA(x)/fB(x): " << endl;
	for (CLN::X=-19.; CLN::X<=19.; CLN::X++) {
		double yK  = funbarKA->Eval(CLN::X);
		double yAB = funbarAoverB->Eval(CLN::X);
		cout << CLN::X << "  " << yK << "  " << yAB << "  delta % = " << (yK-yAB)/yK*100. << endl;
	}
	
	c1->Update();
	
	// check differences in position reconstruction with the various algorithms
	c1->cd(3);
	TH1F *posExp;
	TH1F *posSpl;
	TH1F *posPol5A;
	TH1F *posPol5B;
	TH1F *posPolRatioA;
	TH1F *posPolRatioB;
	posExp = new TH1F("posExp", "posExp", 60, -30., 30.);
	posSpl = new TH1F("posSpl", "posSpl", 60, -30., 30.);
	posPol5A = new TH1F("posPol5A", "posPol5A", 60, -30., 30.);
	posPol5B = new TH1F("posPol5B", "posPol5B", 60, -30., 30.);
	posPolRatioA = new TH1F("posPolRatioA", "posPolRatioA", 60, -30., 30.);
	posPolRatioB = new TH1F("posPolRatioB", "posPolRatioB", 60, -30., 30.);
	double alphaMean  = 0.5*(alphaA + alphaB);
	double alphaDelta = 0.5*(alphaA - alphaB);
	double C0 = 0.5*(log(gainA/gainB) - alphaDelta*37.5)/alphaMean;
	double C1 = 0.5/alphaMean;
	double pos, AoverB, pos1;
    	srand48( (unsigned int)time( NULL ) );	// init seed for random number generator
	for (int j=0; j<1; j++) {
		for (int i=0; i<1000; i++) {
			pos = -17.75 + 35.5*drand48();
			posExp->Fill(pos);
			AoverB = exp((pos-C0)/C1);
			pos1 = funbarKA->GetX(AoverB, -19., 19.);
			posSpl->Fill(pos1);
			pos1 = polfit5A->GetX(AoverB, -19., 19.);
			posPol5A->Fill(pos1);
			pos1 = polfit5B->GetX(1./AoverB, -19., 19.);
			posPol5B->Fill(pos1);
			pos1 = polratioA->GetX(AoverB, -19., 19.);
			posPolRatioA->Fill(pos1);			
//			pos1 = polratioB->GetX(1./AoverB, -19., 19.);
//			posPolRatioB->Fill(pos1);			
		}
		cout << "performed " << 1000*(j+1) << " cycles..." << endl;
	}
	posExp->SetLineColor(2);
	posExp->Draw();
	posSpl->SetLineColor(1);
	posSpl->Draw("SAME");
	posPol5A->SetLineColor(4);
	posPol5A->Draw("SAME");
	posPol5B->SetLineColor(5);
	posPol5B->Draw("SAME");
	posPolRatioA->SetLineColor(8);
	posPolRatioA->Draw("SAME");
	posPolRatioB->SetLineColor(9);
	posPolRatioB->Draw("SAME");
	c1->Update();
}

//
// -------------------------------------------
//

void MCVCalibration::SaveSplinePar(int bar)
{
  	FILE *f1;
  	f1=fopen(SPLINEFILE, "a");
  	if (!f1) {
    		cout << "*** No file " << SPLINEFILE << " found!" << endl;
    		return;
  	}
	fprintf(f1, "%d\n", bar);
	for (int pd=0; pd<2; pd++) {		// print to file fA and fB parameters
		int nn = nhit[bar][pd];
		fprintf(f1, "%d\n", nn);
		for (int i=0; i<nn; i++) fprintf(f1, "%f \t%f \t%.9f\n", data[bar][pd][0][i], data[bar][pd][1][i], CLN::y2[bar][pd][i+1]);
	}
	for (int pd=0; pd<2; pd++) {		// print to file KA and KB parameters
		int nn = nhit[bar][pd];
		fprintf(f1, "%d\n", nn);
		for (int i=0; i<nn; i++) fprintf(f1, "%f \t%f \t%.9f\n", dataK[bar][pd][0][i], dataK[bar][pd][1][i], CLN::y2K[bar][pd][i+1]);
	}
	fclose(f1);
	return;
}

//
// -------------------------------------------
//

void MCVCalibration::SavePolPar(int bar)
{
  	FILE *f1;
  	f1=fopen(POLFILE, "a");
	fprintf(f1, "%d  ", bar);
	for (int i=0; i<12; i++) {		// print to file pol5 fit parameters
		fprintf(f1, "%.9f  ", param[i]);
	}
	fprintf(f1, "\n");
	fclose(f1);
	return;
}


//
// -------------------------------------------
//

TF1 *MCVCalibration::ExpoModel(TGraphErrors *gr, int pd)
{
	// apply exponential model to all points of the bar

	double par[2];
	double aux1, aux2;
	
	spd1->cd();
	cout << endl << "Exponential fit on all dataset" << endl;
	TF1 *total = new TF1("total","expo", -15., 15.);	// NB: cut off the bar borders
	total->SetLineColor(2);
   	gr->Fit("total","R0+");
	total->SetLineWidth(1);
   	total->Draw("same");
	total->GetParameters(par);
	aux1 = total->Eval(-18.75)/1.275;
	aux2 = total->Eval(18.75)/1.275;
	if (pd==0) {	// PD-A
//		cgainA = total->Eval(-18.75)/1.275;
		if (aux1 > aux2) cgainA = aux1;
		else cgainA = aux2;
		cout << "Gain at PD-A edge= " << cgainA << " ch/MeV " << endl;
		cout << "Alpha-A = " << fabs(par[1]) << " cm^-1" << endl;
		calphaA = fabs(par[1]);
	} else {	// PD-B
//		cgainB = total->Eval(18.75)/1.275;
		if (aux1 > aux2) cgainB = aux1;
		else cgainB = aux2;
		cout << "Gain at PD-B edge= " << cgainB << " ch/MeV " << endl;
		cout << "Alpha-B = " << fabs(par[1]) << " cm^-1" << endl;
		calphaB = fabs(par[1]);
	}
	return total;
}

//
// -------------------------------------------
//

TF1 *MCVCalibration::ExpoPolModel(TGraphErrors *gr, int pd)
{
	// apply exponential model in the center of the bar
	// and polinomial fit at the bar borders

	double par[2], polpar[3];
	float gain;
	double *xx, *yy, *data, *vconst;
	double xa;
	int npt;
	
	float xlo=-12.;
	float xhi= 12.;
	TF1 *polA;
	
	spd1->cd();
	cout << endl << "Exponential fit in the center + pol3 fit at the borders" << endl;
	TF1 *totalp = new TF1("totalp","expo", xlo, xhi);	
	totalp->SetLineColor(2);
   	gr->Fit("totalp","R0+");
	totalp->SetLineWidth(1);
   	totalp->Draw("same");
	totalp->GetParameters(par);
	if (pd==0) {	// PD-A
		gain = totalp->Eval(-18.75)/1.275;
		cout << "Gain at PD-A edge= " << gain << " ch/MeV " << endl;
		cout << "Alpha-A = " << par[1] << " cm^-1" << endl;
	} else {	// PD-B
		gain = totalp->Eval(18.75)/1.275;
		cout << "Gain at PD-B edge= " << gain << " ch/MeV " << endl;
		cout << "Alpha-B = " << par[1] << " cm^-1" << endl;
	}
	
	// work on PD-A side polinomial fit
	xx = gr->GetX();
	yy = gr->GetY();
	
	npt=0;
	while(xx[npt]<xlo) npt++;
	cout << "On PD-A side:" << npt << " points to be modeled." << endl;
	data   = new double[(npt+2)*(npt+2)];
	vconst = new double[npt+2];
	if (npt==3) {
		for (int i=0; i<3; i++) {
			xa = xx[i];
			data[i*5]= xa*xa*xa*xa;
			data[i*5+1]= xa*xa*xa;
			data[i*5+2]= xa*xa;
			data[i*5+3]= xa;
			data[i*5+4]= 1.;
			vconst[i] = yy[i];
		}
		data[15]= xlo*xlo*xlo*xlo;
		data[16]= xlo*xlo*xlo;
		data[17]= xlo*xlo;
		data[18]= xlo;
		data[19]= 1.;
		data[20]=4.*xlo*xlo*xlo;
		data[21]=3.*xlo*xlo;
		data[22]=2.*xlo;
		data[23]=1.;
		data[24]=0.;
		vconst[3] = totalp->Eval(xlo);
		vconst[4] = par[1]*totalp->Eval(xlo);
	}
	else if (npt==2) {
		for (int i=0; i<2; i++) {
			xa = xx[i];
			data[i*4]= xa*xa*xa;
			data[i*4+1]= xa*xa;
			data[i*4+2]= xa;
			data[i*4+3]= 1.;
			vconst[i] = yy[i];
		}
		data[8]= xlo*xlo*xlo;
		data[9]= xlo*xlo;
		data[10]= xlo;
		data[11]= 1.;
		data[12]=3.*xlo*xlo;
		data[13]=2.*xlo;
		data[14]=1.;
		data[15]=0.;
		vconst[2] = totalp->Eval(xlo);
		vconst[3] = par[1]*totalp->Eval(xlo);
	}
	for (int i=0; i<npt+2; i++) cout << vconst[i] << endl;
	
	TMatrixD mA(npt+2, npt+2, data, "");
	
	TVectorD vA(npt+2, vconst);
	TDecompLU luA(mA);
	luA.Solve(vA);
	vconst = vA.GetMatrixArray();
	for (int i=0; i<npt+2; i++) {
		cout << vconst[i] << endl;
		polpar[i] = vconst[npt+1-i];
	}
	if (npt==3) polA = new TF1("polA","pol4", -19., xlo);	
	else 	polA = new TF1("polA","pol3", -19., xlo);
	polA->SetParameters(polpar);
	polA->SetLineColor(2);
	polA->SetLineWidth(1);
	polA->Draw("same");
	
		
	return totalp;
}

//
// -------------------------------------------
//

TF1 *MCVCalibration::PolModel(TGraphErrors *gr, int pd)
{
	// apply polinomial model to all data

	double par[6];
	float gain;
	
	spd1->cd();
	cout << endl << "Polinomial model to all dataset" << endl;
	TF1 *totalpol = new TF1("totalpol","pol5", -19., 19.);	
	totalpol->SetLineColor(2);
   	gr->Fit("totalpol","R0+");
	totalpol->SetLineWidth(1);
   	totalpol->Draw("same");
	if (pd==0) totalpol->GetParameters(&param[0]);
	else  totalpol->GetParameters(&param[6]);
	return totalpol;
}

//
// -------------------------------------------
//

void MCVCalibration::SaveExpoPar()
{
	// save exponential fit parameters into MCALBurstTest.conf

	int 	auxbar;
	char 	parameterName[20];
	double 	oldgain, oldalpha;
	
	if (calib) {	// data are ready to be saved
		TEnv *conf;
		conf = new TEnv(CONF_FILENAME_BURST);
		cout << endl << "Updating calibration data on bar (0..29)" << cbar << endl;
		if (cbar<15) auxbar = cbar;
		else auxbar = cbar+1;
		
		// update PD-A gain
		sprintf(parameterName, ".Gain.Bar%02d.A", auxbar);
		oldgain = conf->GetValue(parameterName, -1.);
		if (oldgain<0.) {
			cout << "No parameter " << parameterName << " found. Exiting!" << endl;
			return;
		} else {
			cout << "Old PD-A Gain = " << oldgain << " ch/MeV" << endl;
			cout << "New PD-A Gain = " << cgainA  << " ch/MeV" << endl;
			conf->SetValue(parameterName, cgainA);
		}
		
		// update PD-B gain
		sprintf(parameterName, ".Gain.Bar%02d.B", auxbar);
		oldgain = conf->GetValue(parameterName, -1.);
		if (oldgain<0.) {
			cout << "No parameter " << parameterName << " found. Exiting!" << endl;
			return;
		} else {
			cout << "Old PD-B Gain = " << oldgain << " ch/MeV" << endl;
			cout << "New PD-B Gain = " << cgainB  << " ch/MeV" << endl;
			conf->SetValue(parameterName, cgainB);
		}
		
		// update PD-A alpha
		sprintf(parameterName, ".Alpha.Bar%02d.A", auxbar);
		oldalpha = conf->GetValue(parameterName, -1.);
		if (oldgain<0.) {
			cout << "No parameter " << parameterName << " found. Exiting!" << endl;
			return;
		} else {
			cout << "Old PD-A Alpha = " << oldalpha << " cm^-1" << endl;
			cout << "New PD-A Alpha = " << calphaA  << " cm^-1" << endl;
			conf->SetValue(parameterName, calphaA);
		}
		
		// update PD-B alpha
		sprintf(parameterName, ".Alpha.Bar%02d.B", auxbar);
		oldalpha = conf->GetValue(parameterName, -1.);
		if (oldgain<0.) {
			cout << "No parameter " << parameterName << " found. Exiting!" << endl;
			return;
		} else {
			cout << "Old PD-B Alpha = " << oldalpha << " cm^-1" << endl;
			cout << "New PD-B Alpha = " << calphaB  << " cm^-1" << endl;
			conf->SetValue(parameterName, calphaB);
		}
		
		calib = FALSE;
		conf->Save();
		delete conf;
	} 
	else cout << "Data not ready or already saved. Do Expo calibration first!" << endl;
	return;
}
*/