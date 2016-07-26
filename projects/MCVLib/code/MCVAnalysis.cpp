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
#include "MCVAnalysis.h"

ClassImp(MCVAnalysis);

MCVAnalysis::MCVAnalysis(): tdata(NULL), info(NULL), gLomb(NULL), hcorr(NULL)
{
	time = y = wk1 = wk2 = NULL;
	ofac = 4;
	hifac = 5;
	adclim = 2.;
}

//
//--------------------------------------------------------------------
//

MCVAnalysis::MCVAnalysis(TTree *t, MCVFileInfo *in): tdata(NULL), info(NULL), gLomb(NULL), hcorr(NULL)
{
	SetData(t);
	if (in) SetFileInfo(in);
	time = y = wk1 = wk2 = NULL;
	ofac = 4;
	hifac = 2;
	adclim = 2.;
}

//
//--------------------------------------------------------------------
//

MCVAnalysis::~MCVAnalysis()
{
	if (time) delete time;
	if (y) delete y;
	if (wk1) delete wk1;
	if (wk2) delete wk2;
	if (gLomb) delete gLomb;
	if (hcorr) delete hcorr;
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Fasper(float x[], float y[], unsigned long n, float ofac, float hifac, float wk1[], float wk2[], unsigned long nwk, unsigned long *nout, unsigned long *jmax, float *prob)
{
    unsigned long j, k, ndim, nfreq, nfreqt;
    float ave, ck, ckk, cterm, cwt, den, df, effm, expy, fac, fndim, hc2wt,
          hs2wt, hypo, pmax, sterm, swt, var, xdif, xmax, xmin;

    *nout = 0.5*ofac*hifac*n;
    nfreqt = ofac*hifac*n*MACC;
    nfreq = 64;
    while (nfreq < nfreqt) nfreq <<= 1;
    ndim = nfreq << 1;
    if (ndim > nwk) Error("workspaces too small in Fasper\n");
    Avevar(y, n, &ave, &var);
    if (var == 0.0) Error("zero variance in Fasper\n");
    xmin = x[1];
    xmax = xmin;
    for (j = 2; j <= n; j++) {
	if (x[j] < xmin) xmin = x[j];
	if (x[j] > xmax) xmax = x[j];
    }
    xdif = xmax - xmin;
    for (j = 1; j <= ndim; j++) wk1[j] = wk2[j] = 0.0;
    fac = ndim/(xdif*ofac);
    fndim = ndim;
    for (j = 1; j <= n; j++) {
	ck = (x[j] - xmin)*fac;
	MOD(ck, fndim);
	ckk = 2.0*(ck++);
	MOD(ckk, fndim);
	++ckk;
	Spread(y[j] - ave, wk1, ndim, ck, MACC);
	Spread(1.0, wk2, ndim, ckk, MACC);
    }
    Realft(wk1, ndim, 1);
    Realft(wk2, ndim, 1);
    df = 1.0/(xdif*ofac);
    pmax = -1.0;
    for (k = 3, j = 1; j <= (*nout); j++, k += 2) {
	hypo = sqrt(wk2[k]*wk2[k] + wk2[k+1]*wk2[k+1]);
	hc2wt = 0.5*wk2[k]/hypo;
	hs2wt = 0.5*wk2[k+1]/hypo;
	cwt = sqrt(0.5+hc2wt);
	swt = SIGN(sqrt(0.5-hc2wt), hs2wt);
	den = 0.5*n + hc2wt*wk2[k] + hs2wt*wk2[k+1];
	cterm = SQR(cwt*wk1[k] + swt*wk1[k+1])/den;
	sterm = SQR(cwt*wk1[k+1] - swt*wk1[k])/(n - den);
	wk1[j] = j*df;
	wk2[j] = (cterm+sterm)/(2.0*var);
	if (wk2[j] > pmax) pmax = wk2[(*jmax = j)];
    }
    expy = exp(-pmax);
    effm = 2.0*(*nout)/ofac;
    *prob = effm*expy;
    if (*prob > 0.01) *prob = 1.0 - pow(1.0 - expy, effm);
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Spread(float y, float yy[], unsigned long n, float x, int m)
{
    int ihi, ilo, ix, j, nden;
    static int nfac[11] = { 0, 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };
    float fac;

    if (m > 10)
	Error("factorial table too small in Spread");
    ix = (int)x;
    if (x == (float)ix) yy[ix] += y;
    else {
	ilo = LMIN(LMAX((long)(x - 0.5*m + 1.0), 1), n - m + 1);
	ihi = ilo + m - 1;
	nden = nfac[m];
	fac = x - ilo;
	for (j = ilo + 1; j <= ihi; j++) fac *= (x - j);
	yy[ihi] += y*fac/(nden*(x - ihi));
	for (j = ihi-1; j >= ilo; j--) {
	    nden = (nden/(j + 1 - ilo))*(j - ihi);
	    yy[j] += y*fac/(nden*(x - j));
	}
    }
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Avevar(float data[], unsigned long n, float *ave, float *var)
{
    unsigned long j;
    float s, ep;

    for (*ave = 0.0, j = 1; j <= n; j++) *ave += data[j];
    *ave /= n;
    *var = ep = 0.0;
    for (j = 1; j <= n; j++) {
	s = data[j] - (*ave);
	ep += s;
	*var += s*s;
    }
    *var = (*var - ep*ep/n)/(n-1);
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Realft(float data[], unsigned long n, int isign)
{
    unsigned long i, i1, i2, i3, i4, np3;
    float c1 = 0.5, c2, h1r, h1i, h2r, h2i;
    double wr, wi, wpr, wpi, wtemp, theta;

    theta = 3.141592653589793/(double)(n>>1);
    if (isign == 1) {
	c2 = -0.5;
	Four1(data, n>>1, 1);
    } else {
	c2 = 0.5;
	theta = -theta;
    }
    wtemp = sin(0.5*theta);
    wpr = -2.0*wtemp*wtemp;
    wpi = sin(theta);
    wr = 1.0 + wpr;
    wi = wpi;
    np3 = n+3;
    for (i = 2; i <= (n>>2); i++) {
	i4 = 1 + (i3 = np3 - (i2 = 1 + (i1 = i + i - 1)));
	h1r =  c1*(data[i1] + data[i3]);
	h1i =  c1*(data[i2] - data[i4]);
	h2r = -c2*(data[i2] + data[i4]);
	h2i =  c2*(data[i1] - data[i3]);
	data[i1] =  h1r + wr*h2r - wi*h2i;
	data[i2] =  h1i + wr*h2i + wi*h2r;
	data[i3] =  h1r - wr*h2r + wi*h2i;
	data[i4] = -h1i +wr*h2i + wi*h2r;
	wr = (wtemp = wr)*wpr - wi*wpi + wr;
	wi = wi*wpr + wtemp*wpi + wi;
    }
    if (isign == 1) {
	data[1] = (h1r = data[1]) + data[2];
	data[2] = h1r - data[2];
    } else {
	data[1] = c1*((h1r = data[1]) + data[2]);
	data[2] = c1*(h1r - data[2]);
	Four1(data, n>>1, -1);
    }
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Four1(float data[], unsigned long nn, int isign)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
	if (j > i) {
	    SWAP(data[j], data[i]);
	    SWAP(data[j+1], data[i+1]);
	}
//	m = n >> 1;	// NB: diverso da NR!!!
	m = nn;
	while (m >= 2 && j > m) {
	    j -= m;
	    m >>= 1;
	}
	j += m;
    }
    mmax = 2;
    while (n > mmax) {
	istep = mmax << 1;
	theta = isign*(6.28318530717959/mmax);
	wtemp = sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi = sin(theta);
	wr = 1.0;
	wi = 0.0;
	for (m = 1; m < mmax; m += 2) {
	    for (i = m;i <= n;i += istep) {
		j = i + mmax;
		tempr = wr*data[j] - wi*data[j+1];
		tempi = wr*data[j+1] + wi*data[j];
		data[j] = data[i] - tempr;
		data[j+1] = data[i+1] - tempi;
		data[i] += tempr;
		data[i+1] += tempi;
	    }
	    wr = (wtemp = wr)*wpr - wi*wpi + wr;
	    wi = wi*wpr + wtemp*wpi + wi;
	}
	mmax = istep;
    }
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::Error(char *s)
{
    fprintf(stderr, "*** MCVAnalysis error: %s\n", s);
    exit(1);
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::DoLombNormPeriod(float tstart, float tstop, int ibar, int pd)
{
	int ipos, ndim;
	int limA=4096, limB=4096;
	char hname[20];
	
	if (tdata==NULL) {
		cout << "*** No dataset loaded!" << endl;
		return;
	}
	
	if (time) delete time;
	if (y) delete y;
	if (wk1) delete wk1;
	if (wk2) delete wk2;
	if (gLomb) delete gLomb;
	
	time = new float[nentries];
	y    = new float[nentries];
	
	nevt = 0;
	cout << "Working on bar " << ibar << "  PD-" << (pd==0 ? 'A' : 'B') << endl;
	if (info) {
		limA = info->oA[ibar]+adclim;
		limB = info->oB[ibar]+adclim;
		cout << "lim-A = " << limA << "  -  lim-B = " << limB << endl;
	} 
	
	if (pd==0) {	// work on PD-A
		for (int i=0; i<nentries; i++) {
			tdata->GetEntry(i);
			ipos = currevt.bar[ibar];
			if (currevt.time > tstart && currevt.time < tstop && ipos >= 0) {
				if (currevt.pdA[ipos] <=limA ) {
					time[nevt] = (currevt.time-tstart)*1.E-6;
					y[nevt]   =  currevt.pdA[ipos];
					nevt++;
				}
			}
		}
		cout << "nevt = " << nevt << "  -  total events = " << nentries << endl;
	} else {	// work on PD-B
		for (int i=0; i<nentries; i++) {
			tdata->GetEntry(i);
			ipos = currevt.bar[ibar];
			if (currevt.time > tstart && currevt.time < tstop && ipos >= 0) {
				if (currevt.pdB[ipos] <=limB ) {
					time[nevt] = currevt.time*1.E-6;
					y[nevt]   =  currevt.pdB[ipos];
					nevt++;
				}
			}
		}
		cout << "nevt = " << nevt << "  -  total events = " << nentries << endl;
	}
	
	ndim = ofac*hifac*nevt*MACC*4;
	cout << "ndim = " << ndim << endl;
	wk1 = new float[ndim];
	wk2 = new float[ndim];
	
	Fasper(time, y, nevt, ofac, hifac, wk1, wk2, ndim, &nout, &jmax, &prob);
	maxFreq = wk1[jmax];
	cout << "nout = " << nout << endl;
	cout << "jmax = " << jmax << endl;
	cout << "prob = " << prob << endl;
	gLomb = new TGraph(nout, wk1, wk2);
	sprintf(hname, "%c%d PD-%c", ibar<15? 'X' : 'Z', ibar<15? ibar+1 : ibar-14, pd==0? 'A' : 'B');
	gLomb->SetName(hname);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::DoLombNormPeriod(float x[], float y[], unsigned long nevt)
{
	int ndim;
	
	if (wk1) delete wk1;
	if (wk2) delete wk2;
	if (gLomb) delete gLomb;
	ndim = ofac*hifac*nevt*MACC*4;
	wk1 = new float[ndim];
	wk2 = new float[ndim];
	
	Fasper(x, y, nevt, ofac, hifac, wk1, wk2, ndim, &nout, &jmax, &prob);
	cout << "nout = " << nout << endl;
	cout << "jmax = " << jmax << endl;
	cout << "prob = " << prob << endl;
	gLomb = new TGraph(nout, wk1, wk2);
	return;
}

//
//--------------------------------------------------------------------
//

void MCVAnalysis::SetData(TTree *t)
{
	tdata = t;		
	tdata->SetBranchAddress("time", &currevt.time);
	tdata->SetBranchAddress("mult", &currevt.mult);
	tdata->SetBranchAddress("bar", currevt.bar);
	tdata->SetBranchAddress("pdA", currevt.pdA);
	tdata->SetBranchAddress("pdB", currevt.pdB);
	tdata->SetBranchAddress("pos", currevt.pos);
	tdata->SetBranchAddress("E", currevt.E);
	tdata->SetBranchAddress("Etot", &currevt.Etot);
	nentries = tdata->GetEntries();
	cout << "nentries = " << nentries << endl;
	return;
}

//
//--------------------------------------------------------------------
//

Double_t MCVAnalysis::GetLinearCorrelation(int bar1, int pd1, int bar2, int pd2, float tstart, float tstop)
{
	float *x, *y, y1, y2, lim1, lim2;
	Double_t cc;
	int ipos1, ipos2, nevt;
	TGraph *gaux;

	if (tdata==NULL) {
		cout << "*** No dataset loaded!" << endl;
		return -100.;
	}
	if (info==NULL || info->pkttype != 3902) {
		cout << "*** Cannot apply routine GetLinearCorrelation() to non-39.02 data!" << endl;
		return -100.;
	}
	
	x    = new float[nentries];
	y    = new float[nentries];
	nevt=0;
	if (pd1==0) lim1 = info->oA[bar1] + 10.;
	else lim1 = info->oB[bar1] + 20.;
	if (pd2==0) lim2 = info->oA[bar2] + 10.;
	else lim2 = info->oB[bar2] + 20.;
	cout << "lim1 = " << lim1 << "  lim2= " << lim2 << endl;
	
	for (int i=0; i<nentries; i++) {
		tdata->GetEntry(i);
		ipos1 = currevt.bar[bar1];
		ipos2 = currevt.bar[bar2];
		if (currevt.time > tstart && currevt.time < tstop && ipos1 >= 0 && ipos2 >= 0) {
			if (pd1==0) y1=currevt.pdA[ipos1];
			else y1=currevt.pdB[ipos1];
			if (pd2==0) y2=currevt.pdA[ipos2];
			else y2=currevt.pdB[ipos2];
			if (y1<=lim1 && y2<=lim2) {	
				x[nevt]   =  y1;
				y[nevt]   =  y2;
				nevt++;
			}
		}
	}
	cout << "nevt = " << nevt << endl;
	gaux = new TGraph(nevt, x, y);
	cc = gaux->GetCorrelationFactor();
	delete gaux;
	delete x;
	delete y;
	return cc;
}

//
//--------------------------------------------------------------------
//

TH2F *MCVAnalysis::GetCorrelationMatrix(float tstart, float tstop)
{
	int bar1, pd1, bar2, pd2;
	Double_t cc=0.;
	
	if (hcorr) return hcorr;
	hcorr = new TH2F("gcorr", "gcorr", 60, 0, 60, 60, 0, 60);
	
	for (int i=0; i<60; i++) {
		bar1 = (i/30)*15 + i%15;
		pd1 = (i%30)/15;
		for (int j=0; j<i; j++) {
			bar2 = (j/30)*15 + j%15;
			pd2 = (j%30)/15;
			cc = GetLinearCorrelation(bar1, pd1, bar2, pd2, tstart, tstop);
			printf("%d-%d vs %d-%d \t%f\n", bar1, pd1, bar2, pd2, cc);
			hcorr->SetBinContent(i, j, cc); 
		}
	}
	return hcorr;
}