#include <fitsio.h>

#include "hkviewer.h"
#include "version.h"

#define HKVIEWER_CONF "/share/mcal_conf/hkviewer_configuration.conf"
#define HKREFERENCE_CONF "/share/hkmonitor_conf/32_01.ql_file2.PPS"
using namespace std;

int main(int argc, char *argv[])
{
	int 	status = 0;
	int 	nevents;
	int 	parnum;
	int 	colnum;
	int 	typecode;
	long	repeat, width;
	char 	comment[100];
	char 	colname[100];
	char 	strtstart[50]; 
	
	char	*databyte;
	float	*data, tstart, tobs;
	double	*time;
	float  tini=-1., tend=-1.;
		
	fitsfile *fptr;
	
	int	isrelative, usephysical, alarmlevel;
	int nrows, ncols;
	TString outpath;
	TString histoname;
	bool batch=FALSE;
	
	TCanvas *c1;
	TStyle *mcalstyle;
	TApplication *theApp;
	
	cout << "hkviewer version " << VERSION << endl;
		
	// load configuration file
	TString   calFile;		
	calFile.Append(getenv("MCALSW"));
	calFile += HKVIEWER_CONF;
	//cout << "opening configuration file " << calFile.Data() << endl;
	TEnv *conf = new TEnv(calFile.Data());
	if (!conf) {
		cout << "*** cannot find configuration file " << calFile.Data() << endl;
		return 0;
	}
	isrelative  = conf->GetValue(".relative.time.", 1);
	usephysical = conf->GetValue(".use.physical.", 0);
	alarmlevel  = conf->GetValue(".draw.alarm.levels.", 0);
	nrows = conf->GetValue(".nrows.", 5);
	ncols = conf->GetValue(".ncols.", 6);
	TString *boostfile = NULL;

	// parse command line data
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		if (cmd.Contains("-b")) batch=TRUE;
		if (cmd.Contains("-p")) outpath += argv[++iopt];
		if (cmd.Contains("-f")) histoname += argv[++iopt];
		if (cmd.Contains("-s")) boostfile = new TString(argv[++iopt]);
		iopt++;
	}
	if (outpath.Length() > 0 && outpath.EndsWith("/")==FALSE) outpath += "/";
	
	TString bfile(conf->GetValue(".boost.file.", "none"));
	if (!boostfile && !bfile.Contains("none")) {
		boostfile = new TString(getenv("MCALSW"));
		boostfile->Append(bfile);
	}
	
	// load A.B. physical units configuration file
	TString unitFile;
	unitFile.Append(getenv("MCALSW"));
	unitFile += HKREFERENCE_CONF;
	TEnv *units = NULL;
	if (usephysical) {
		units = new TEnv(unitFile.Data());
		if (!units) {
			cout << "*** cannot find required physical units file " << unitFile.Data() << endl;
			return 0;
		}
	}
	
	// set default root file if filename is not specified
	if (histoname.Length() == 0) {
		histoname.Append(getenv("MCALSW"));
		histoname += conf->GetValue(".hkmonitor.file.", "./hkm.root");
	} 
	// open ROOT data file
	TFile *fh = new TFile(histoname.Data(), "READ");  // "RECREATE" "UPDATE"
	
	if (!batch)  theApp = new TApplication("App", &argc, argv);
	mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.06, "xyz");
	mcalstyle->SetTitleSize(0.06, "xyz");
	
	// set time offset
//	TDatime epoch(2003, 12, 31, 23, 0, 1);		// buono per ora LEGALE (estate)
//	TDatime epoch(2004, 1, 1, 0, 0, 1);		// buono per ora SOLARE (inverno)
	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01

//	mcalstyle->SetTimeOffset(epoch.GetSec(), "gmt");
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	TTimeStamp ts;
	
	parnum = conf->GetValue(".parameter.number.", 0);
	int nplot = nrows*ncols;
	int ncanvas = (parnum-1)/nplot;
	int par = 1;
	
	// draws main HK data
	TText tt;
	TString str("MCAL Team - IASF-Bo - hkviewer version ");
	str += VERSION;
	
	TText tt1;
	TString str1("T start: ");

	for (int k=0; k<=ncanvas; k++) {
		TString canvasname(outpath);
		canvasname += "hkview";
		canvasname += k;
		c1 = new TCanvas(canvasname.Data(), "HK monitor", 1000, 750);
		c1->Divide(ncols, nrows);
		c1->cd(1);
		for (int i=1; i<=nplot; i++) {
			TString parname = ".par.";
			parname += par;
			parname += ".";
			TGraph	*aux = NULL;
			TString gname = conf->GetValue(parname.Data(), "ERR");
			fh->GetObject(gname.Data(), aux);
			//cout << "retrieving object " << gname.Data() << endl;
			
			if (k==0 && i==1 && aux) {
				double *t = aux->GetX();
				//ts.Set((UInt_t) t[0] + epoch.Convert());
				ts.Set((int)  t[0], 0, epoch.GetSec(),0);
				str1 += ts.AsString();
			}

			if (aux) {
				c1->cd(i);
				aux->GetXaxis()->SetTitle("Time");
				aux->GetXaxis()->SetTimeDisplay(1);
				aux->GetXaxis()->SetTimeOffset(epoch.GetSec(), "gmt");	// , "gmt" ???
				aux->GetXaxis()->SetTimeFormat("%H:%M:%S");//"%d/%m/%Y-%H:%M"
				
				if (tini<0. || tend<0.) {
					int npt = aux->GetN();
					double *x = aux->GetX();
					tini = x[0];
					tend = x[npt-1];
				}
				
				// retrieve information from A.B. physical units configuration file
				TLine *lhi=NULL, *llo=NULL;
				if (usephysical) {
					TString saux(units->GetValue(gname.Data(), "wrong string"));
					TObjArray *aaux = saux.Tokenize("|");
					TString sunit(((TObjString *) aaux->At(6))->GetString());
					if (sunit.Contains("none")) aux->GetYaxis()->SetTitle("ADC value");
					else aux->GetYaxis()->SetTitle(sunit.Data());
					
					if (alarmlevel) {	// retrieve lower and upper alarm levels
						if ((((TObjString *) aaux->At(11))->GetString()).IsFloat()) {
							double ylo = (((TObjString *) aaux->At(11))->GetString()).Atof();
							llo = new TLine(tini, ylo, tend, ylo);
							llo->SetLineColor(3);
							//cout << "draw llo" << tini << " " << tend << "  " << ylo << endl;
						}
						if ((((TObjString *) aaux->At(12))->GetString()).IsFloat()) {
							double yhi = (((TObjString *) aaux->At(12))->GetString()).Atof();
							lhi = new TLine(tini, yhi, tend, yhi);
							lhi->SetLineColor(2);
						}					
					}
				} else aux->GetYaxis()->SetTitle("ADC value");
				
				aux->GetXaxis()->SetRangeUser(tini, tend);
				aux->GetXaxis()->SetTitleOffset(0.8);
				aux->GetXaxis()->SetTitleSize(0.06);
				aux->GetXaxis()->CenterTitle();				
				aux->GetYaxis()->CenterTitle();
				aux->GetYaxis()->SetTitleOffset(0.8);
				aux->GetYaxis()->SetTitleSize(0.06);
				aux->Draw("ALP");
				if (llo) llo->Draw();
				if (lhi) lhi->Draw();
				if (i == 1) tt1.DrawTextNDC(0.2, 0.95, str1.Data());
			}
			par++;
			if (par>parnum) break;
		}
		canvasname += ".png";
		if (batch) c1->Print(canvasname.Data());
	}
	
	// draws burst trigger RM
	if (1) {	// to be modified
		c1 = hkTrgOutput(fh, epoch);
		TCanvas *c2 = hkTrgStatistics(fh, epoch, boostfile);
		
		if (batch) {
			TString canvasname(outpath);
			canvasname += "hkburst_stat.png";
			c1->Print(canvasname.Data());
		}
	}
	
	if (!batch) theApp->Run();
	return 0;
}
