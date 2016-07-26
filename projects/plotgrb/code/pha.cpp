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

#include "plotgrb.h"
#include "version.h"

#include "phaFile.h" 
#include "EChannels.h" 

using namespace std ; 

int savePha(int nch2, float *spP1, float dtime, TString *fname)
{
	// by M. Galli, copiata e adattata by M. Marisaldi 10 Oct. 2007, modificata 20 Feb. 2008

	string phatemplate = getenv("MCALSW");
	phatemplate += "/share/mcalfits_conf/phatemplate.txt" ;
	
	phaFile * phaP1 = new phaFile(fname->Data(),phatemplate); // for plane 1 (upper or X)
  	
	int nch = nch2; 
	
	//  sets the num of E bins in the SPECTRUM extension
	
	std::string hduname="SPECTRUM" ;
	
	std::string keyname1="DETCHANS" ; int value1=nch ;     // max num of channels
	std::string keyname2="TLMIN001" ; int value2=1   ;     // first ch. num
	std::string keyname3="TLMAX001" ; int value3=nch ;     // last ch num 
	std::string keyname4="EXPOSURE" ; float value4=dtime ;     // grb integration time 
	
	phaP1-> setIntKey(keyname1, value1, hduname) ;
	phaP1-> setIntKey(keyname2, value2, hduname) ;
	phaP1-> setIntKey(keyname3, value3, hduname) ;
	phaP1-> setFloatKey(keyname4, value4, hduname) ;
	
	for ( int i=0 ; i < nch ; i++ )   // for each E channel ( plane 1)
	{
		// i+1: fits channels begin from 1, not 0 , as all arrays in C,C++,java etc.
		int channel=i+1 ; 
		phaP1->OutEline( channel, spP1[i]>0. ? (int) spP1[i] : 0) ;     
	}
		
	phaP1->chiudi() ; 
	
	return 0;

}

TCanvas *plotSpectrum(int nb, double *spbins, float *spbkg, float dtbkg, float *spgrb, float dtgrb)
{
	TGraphErrors *gsp = new TGraphErrors(nb);
	for (int j=0; j<nb; j++) {
		gsp->SetPoint(j, 0.5*(spbins[j]+spbins[j+1]), spgrb[j]/dtgrb - spbkg[j]/dtbkg);
		gsp->SetPointError(j, 0.5*(spbins[j+1]-spbins[j]), 3*sqrt(spgrb[j]/(dtgrb*dtgrb) + spbkg[j]/(dtbkg*dtbkg)));
	}
	TCanvas *csp = new TCanvas("csp", "csp", 800, 600);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	gsp->GetXaxis()->SetTitle("Energy (MeV)");
	gsp->GetXaxis()->SetTitleOffset(1.4);
	gsp->GetXaxis()->CenterTitle();
	gsp->GetYaxis()->SetTitle("Counts/s");
	gsp->GetYaxis()->CenterTitle();
	gsp->GetYaxis()->SetTitleOffset(1.5);
	gsp->SetTitle("MCAL GRB count spectrum");
	gsp->GetXaxis()->SetTitleSize(0.04);
	gsp->GetXaxis()->SetTitleOffset(1.);
	gsp->GetXaxis()->SetLabelSize(0.04);
	gsp->GetYaxis()->SetTitleSize(0.04);
	gsp->GetYaxis()->SetTitleOffset(1.);
	gsp->GetYaxis()->SetLabelSize(0.04);
	gsp->Draw("AP");
	
	return csp;
}
