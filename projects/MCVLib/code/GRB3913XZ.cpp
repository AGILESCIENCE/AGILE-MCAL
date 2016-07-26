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

#include "GRB3913XZ.h"

ClassImp(GRB3913XZ);

GRB3913XZ::GRB3913XZ() 
{
	// NB: this constructor MUST be implemented even is useless, otherwise you will get 
	// /home/martino/mcalsw/lib/libMCV.so: undefined reference to `GRB::GRB[in-charge]()'
}


//
//--------------------------------------------------------------------
//

GRB3913XZ::GRB3913XZ(TString rootfilename) : data(NULL), pX(NULL), pZ(NULL)
{
	pX = new GRB3913(rootfilename, 0);
	pZ = new GRB3913(rootfilename, 1);
}

//
//--------------------------------------------------------------------
//

GRB3913XZ::~GRB3913XZ()
{

}

//
//--------------------------------------------------------------------
//

int GRB3913XZ::LightCurve(double t0L, double tpre, double tpost)
{
	pX->LightCurve(t0L, tpre, tpost);
	pZ->LightCurve(t0L, tpre, tpost);
	
	t90X = pX->GetT90();
	t90Z = pZ->GetT90();
	return 0;
}

//
//--------------------------------------------------------------------
//

void GRB3913XZ::SpectralAnalysis()
{
	pX->SpectralAnalysis();
	pZ->SpectralAnalysis();
	
	// reset t90 according to the plane with greatest fluence
	double fX = pX->GetSelFluence();
	double fZ = pZ->GetSelFluence();
	if (fX > fZ) {
		cout << "Higher fluence in plane X: using plane X t90" << endl;
		int ilo = pX->GetI90Lo();
		int ihi = pX->GetI90Hi();
		pZ->SetT90(ilo, ihi);
	} else {
		cout << "Higher fluence in plane Z: using plane Z t90" << endl;
		int ilo = pZ->GetI90Lo();
		int ihi = pZ->GetI90Hi();
		pX->SetT90(ilo, ihi);	
	}
	
	t90X = pX->GetT90();
	t90Z = pZ->GetT90();
}

//
//--------------------------------------------------------------------
//

void GRB3913XZ::CustomSpectralAnalysis()
{
	pX->CustomSpectralAnalysis();
	pZ->CustomSpectralAnalysis();
}

//
//--------------------------------------------------------------------
//

void GRB3913XZ::SetGRBBoundaries( double tbkgini, double tbkgfin, double tgrbini, double tgrbfin)
{
	pX->SetGRBBoundaries(tbkgini, tbkgfin, tgrbini, tgrbfin);
	pZ->SetGRBBoundaries(tbkgini, tbkgfin, tgrbini, tgrbfin);
	return;	
}

//
//--------------------------------------------------------------------
//

void GRB3913XZ::DrawCustomLightCurve(double ti, double tf)
{
	cpXZ = new TCanvas("cpXZ", "AGILE-MCAL light curve", 2*3000/(pX->GetNEbins() + 1), 1000);
	cpXZ->Divide(2, 1);
	
	cpXZ->cd(1);
	pX->DrawLightCurve((TPad *) gPad, ti, tf);
	
	cpXZ->cd(2);
	pZ->DrawLightCurve((TPad *) gPad, ti, tf);
	
}

//
//--------------------------------------------------------------------
//

void GRB3913XZ::SaveCustomLightCurve(TString outpath)
{
	if (!cpXZ) {
		cout << "*** GRB3913XZ::SaveCustomLightCurve() ERROR: no canvas created yet!" << endl;
		return;
	}
	
	TString fname(outpath);
	fname += pX->GetFileHeader()->Data();
	fname += ".gif";
	cpXZ->Print(fname);
	return;
}

//
//--------------------------------------------------------------------
//

int GRB3913XZ::SaveInfo(TString infofile)
{
	pX->SaveInfo(infofile, 1);
	pZ->SaveInfo(infofile, 0);
	return 0;
}

//
//--------------------------------------------------------------------
//

int GRB3913XZ::SaveAnalysis(TString outpath)
{
	pX->SaveAnalysis(outpath);
	pZ->SaveAnalysis(outpath);
	return 0;
}

