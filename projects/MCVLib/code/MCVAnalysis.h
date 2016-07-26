//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

/* file: lomb.c		G. Moody	12 February 1992
			Last revised:	  10 June 2005

-------------------------------------------------------------------------------
lomb: Lomb periodogram of real data
Copyright (C) 1992-2005 George B. Moody

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA.

You may contact the author by e-mail (george@mit.edu) or postal mail
(MIT Room E25-505A, Cambridge, MA 02139 USA).  For updates to this software,
please visit PhysioNet (http://www.physionet.org/).
_______________________________________________________________________________

The default input to this program is a text file containing a sampled time
series, presented as two columns of numbers (the sample times and the sample
values).  The intervals between consecutive samples need not be uniform (in
fact, this is the most significant advantage of the Lomb periodogram over other
methods of power spectral density estimation).  This program writes the Lomb
periodogram (the power spectral density estimate derived from the input time
series) on the standard output.

The original version of this program was based on the algorithm described in
Press, W.H, and Rybicki, G.B., Astrophysical J. 338:277-280 (1989).  It has
been rewritten using the versions of this algorithm presented in Numerical
Recipes in C (Press, Teukolsky, Vetterling, and Flannery;  Cambridge U. Press,
1992).

This version agrees with 'fft' output (amplitude spectrum up to the Nyquist
frequency with total power equal to the variance);  thanks to Joe Mietus.
*/

#ifndef MCVLib_MCVAnalysis
#define MCVLib_MCVAnalysis

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <TTree.h>
#include <TGraph.h>
#include <TH2F.h>
#include "MCVFileInfo.h"
#include "MCVFile.h"


#define LMAX(a,b) (lmaxarg1 = (a),lmaxarg2 = (b), (lmaxarg1 > lmaxarg2 ? lmaxarg1 : lmaxarg2))
#define LMIN(a,b) (lminarg1 = (a),lminarg2 = (b), (lminarg1 < lminarg2 ? lminarg1 : lminarg2))
#define MOD(a,b)	while (a >= b) a -= b
#define MACC 4
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
#define SQR(a) ((sqrarg = (a)) == 0.0 ? 0.0 : sqrarg*sqrarg)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

//! A class collecting the extended MCAL data analysis routines

/*! This class performs further analysis on MCAL data. 
It is a collection of non-standard routines.
THIS CLASS SHOULD INHERIT FROM CLASS MCVStdAnalysis (NOT IMPLEMENTED YET)
*/


class MCVAnalysis {

	protected:
	 	long lmaxarg1, lmaxarg2;
	 	long lminarg1, lminarg2;
	 	float sqrarg;
		double adclim;		//!< Limit (relative to offset) for data selection for Lomb calculations
		MCALevt currevt;		
		MCVFileInfo	*info;	//!< Object containing all the relevant file information
		
		// data for Lomb normalized periodogram calculations
		TTree	*tdata;		//!< Pointer to the data tree
		long	nentries;
		float ofac;
		float hifac;
		float *wk1;
		float *wk2;
		float *time;
		float *y;
		unsigned long nout;
		unsigned long jmax;
		float prob;
		int   nevt;
		float maxFreq;
		TGraph *gLomb;		//!< Pointer to the Lomb Normalized periodogram
		
		TH2F *hcorr;		//!< Pointer to the correlation matrix
		
		void Avevar(float data[], unsigned long n, float *ave, float *var);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
		void Error(char *s);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
		void Fasper(float x[], float y[], unsigned long n, float ofac, float hifac, float wk1[], float wk2[], unsigned long nwk, unsigned long *nout, unsigned long *jmax, float *prob);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
		void Four1(float data[], unsigned long nn, int isign);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
		void Realft(float data[], unsigned long n, int isign);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
		void Spread(float y, float yy[], unsigned long n, float x, int m);	//!< Function for Lomb Normalized periodogram calculations (from Numerical Recipes)
	
	public:
		MCVAnalysis();
		MCVAnalysis(TTree *t, MCVFileInfo *in=NULL);
		~MCVAnalysis();	//virtual 
		
		void DoLombNormPeriod(float tstart, float tstop, int bar, int pd);	//!< Calculate the Lomb Normalized periodogram (LNP, from Numerical Recipes)
		/*!< \param tstart is the start time (microsec) of the interval you want to calculate the LNP in
		\param tstop is the stop time (microsec) of the interval you want to calculate the LNP in
		\param bar is the bar (0..29) you want to calculate the LNP on
		\param pd is the PD (0=PD-A; 1=PD-B) you want to calculate the LNP on
		*/
		
		void DoLombNormPeriod(float x[], float y[], unsigned long n);	//!< Calculate the Lomb Normalized periodogram (LNP, from Numerical Recipes)
		/*!< \param x is the abscissa array (time, microsec)
		\param y is the variable value (typically a PD output)
		\param n is the dimension of x and y arrays (number of events) 
		*/
		
		TH2F *GetCorrelationMatrix(float tstart, float tstop);	//!< Returns a pointer to the correlation matrix
		/*!<
		It returns a 2D histogram H, where H(i,j) is the linear correlation coefficient of the graph PD-j vs. PD-i. 
		Of course H(i,j)=H(j,i) and H(i,i)=1 so only half the matrix is filled up. 
		i and j are organized from 0 to 59 as follows: X1-A..X15-A, X1-B..X15-B, Z1-A..Z15-A, Z1-B..Z15-B. 
		If can be applied only to GRID physical calibration data (39.02) because for each event all 60 PDs are read out. For un-correlated data (as it SHOULD be) H(i,j) must be close to 0, while for correlated data H(i,j) tends toward +/- 1.
		*/
		
		TGraph *GetLombNormPeriod() {return gLomb;};		//!< Returns a pointer to the Lomb Normalized periodogram  graph
		
		Double_t GetLinearCorrelation(int bar1, int pd1, int bar2, int pd2, float tstart, float tstop);	//!< Returns the linear correlation between 2 PDs
		/*!< \param bar1 (0..29) is the first bar to correlate
		\param pd1 (0=PD-A; 1=PD-B) is the PD of the first bar you want to correlate
		\param bar2 (0..29) is the second bar to correlate
		\param pd2 (0=PD-A; 1=PD-B) is the PD of the second bar you want to correlate
		\param tstart is the start time (microsec) of the interval you want to calculate correlation in
		\param tstop is the stop time (microsec) of the interval you want to calculate correlation in
		*/
		float GetMaxFreq() {return maxFreq;};
		int   GetEvents() {return nevt;};
		float GetProb() {return prob;};
		
		void SetADCRelativeLimit(double adc) {adclim = adc;};
		void SetData(TTree *t);				//!< Set pointer to data tree
		void SetFileInfo(MCVFileInfo *in) {info = in;};	//!< Set pointer to file info
		void SetHifac(float hf) {hifac = hf;};		//!< Set parameter hifac for LNP calculations
		/*!< See Numerical Recipes for explanation */
		
		void SetOfac(float of) {ofac = of;};		//!< Set parameter ofac for LNP calculations
		/*!< Set over-sampling factor. See Numerical Recipes for explanation */
		
		ClassDef(MCVAnalysis,1);	//!< required for ROOT dictionary generation using rootcint
};



#endif
