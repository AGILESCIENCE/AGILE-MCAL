init()
{
gROOT->Reset();
TDatime da(2007, 1, 1, 0, 0, 0);
TDatime dstart(2007, 5, 10, 0, 0, 0);
TDatime dstop(2007, 5, 12, 24, 0, 0);
 float  tini=dstart.Convert() - da.Convert();
 float  tend=dstop.Convert() - da.Convert();
TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
mcalstyle->SetCanvasBorderMode(0);
mcalstyle->SetPadBorderMode(0);
mcalstyle->SetPadColor(0);
mcalstyle->SetCanvasColor(0);
mcalstyle->SetTitleColor(1); // 0
mcalstyle->SetStatColor(0);
mcalstyle->SetPalette(1,0);
mcalstyle->SetFrameBorderMode(0);
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
mcalstyle->SetTimeOffset(da.Convert());
gROOT->SetStyle("mcalstyle");

}

draw()
{
TDatime da(2007, 1, 1, 0, 0, 0);
TDatime dstart(2007, 5, 10, 0, 0, 0);
TDatime dstop(2007, 5, 12, 24, 0, 0);
long int ti = da.Convert();
double tini=-1.;
double tend=-1.;
// float  tini=dstart.Convert() - da.Convert();
// float  tend=dstop.Convert() - da.Convert();
TCanvas *c1 = new TCanvas("c1", "global data", 800, 600);
c1->Divide(2, 2);
char gname[10];

TFile *f= new TFile("/data1/mcalsw_output_COMMISSIONING/misc/MCAL_global_data_COMMISSIONING_070511.root");
//TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_IABG_2007_TV_3901.root");

// work on global sci RM
/*
TCanvas *c2 = new TCanvas("c2", "sci RM (global)", 800, 600);
c2->Divide(6, 4);
	TGraphErrors	*aux;

	for (int i=1; i<=11; i++) {
		// work on Sci RM X
		aux = NULL;
		sprintf(gname, "rmX%d", i);
		f->GetObject(gname, aux);
		c2->cd(i);
		if (aux) {
			aux->GetXaxis()->SetTitle("Time (s)");
			aux->GetXaxis()->SetTitleSize(0.05);
			aux->GetXaxis()->CenterTitle();
			aux->GetYaxis()->SetTitle("Counts/s");
			aux->GetYaxis()->CenterTitle();
			aux->GetYaxis()->SetTitleSize(0.05);
			//aux->GetXaxis()->SetTimeDisplay(1);
			//aux->GetXaxis()->SetTimeOffset(da.Convert());
			//aux->GetXaxis()->SetTimeFormat("%H");
			//aux->GetXaxis()->SetRangeUser(tini, tend);
			int npt = aux->GetN();
			double miny = TMath::MinElement(npt, aux->GetY()); 
			double maxy = TMath::MaxElement(npt, aux->GetY());
			double minx = TMath::MinElement(npt, aux->GetX()); 
			double maxx = TMath::MaxElement(npt, aux->GetX());
			aux->GetXaxis()->SetLimits(minx, maxx);
			aux->GetHistogram()->SetMaximum(1.1*maxy);
			aux->Draw("AP");
		}
	
		// work on Sci RM Z
		aux = NULL;
		sprintf(gname, "rmZ%d", i);
		f->GetObject(gname, aux);
		c2->cd(i+12);
		if (aux) {
			aux->GetXaxis()->SetTitle("Time (s)");
			aux->GetXaxis()->SetTitleSize(0.05);
			aux->GetXaxis()->CenterTitle();
			aux->GetYaxis()->SetTitle("Counts/s");
			aux->GetYaxis()->CenterTitle();
			aux->GetYaxis()->SetTitleSize(0.05);
			//aux->GetXaxis()->SetTimeDisplay(1);
			//aux->GetXaxis()->SetTimeOffset(da.Convert());
			//aux->GetXaxis()->SetTimeFormat("%H");
			int npt = aux->GetN();
			double miny = TMath::MinElement(npt, aux->GetY()); 
			double maxy = TMath::MaxElement(npt, aux->GetY());
			double minx = TMath::MinElement(npt, aux->GetX()); 
			double maxx = TMath::MaxElement(npt, aux->GetX());
			aux->GetXaxis()->SetLimits(minx, maxx);
			aux->GetHistogram()->SetMaximum(1.1*maxy);
			aux->Draw("AP");
		}
	}
*/
// work on puntual sci RM

TCanvas *c3 = new TCanvas("c3", "sci RM (puntual)", 800, 600);
c3->Divide(6, 4);
TCanvas *c4 = new TCanvas("c4", "sci RM (puntual, histogram style)", 800, 600);
c4->Divide(6, 4);
TGraph *aux2;

int ibin;
char hname[10];
int rebinfactor[]={1, 10, 1, 1, 10, 10, 10, 10, 100, 100, 100};
TH1D *hg;

	
	// plot histograms
	for (int i=1; i<=11; i++) {
			c4->cd(i);
		sprintf(gname, "grmX%d", i);
		f->GetObject(gname, aux2);

			int npt = aux2->GetN();
			double miny = TMath::MinElement(npt, aux2->GetY()); 
			double maxy = TMath::MaxElement(npt, aux2->GetY());
			double minx = TMath::MinElement(npt, aux2->GetX()); 
			double maxx = TMath::MaxElement(npt, aux2->GetX());
				tini = minx;
				tend = maxx+1.;
			
			TString hs("hrmX");
			hs += i;

			hg = new TH1D(hs.Data(), hs.Data(), (int) (tend-tini), tini, tend);
			double *x = aux2->GetX();
			double *y = aux2->GetY();
			for (int k=0; k<npt; k++) if(x[k]>tini && x[k]<tend) {
			    ibin = (int) (x[k]-tini);
			    hg->SetBinContent(ibin, y[k]);
			}
			hg->Rebin(10);
			hg->Scale(1./10.);
			hg->Draw();//"P0"
			
			c4->cd(i+12);
		sprintf(gname, "grmZ%d", i);
		f->GetObject(gname, aux2);

			int npt = aux2->GetN();
			double miny = TMath::MinElement(npt, aux2->GetY()); 
			double maxy = TMath::MaxElement(npt, aux2->GetY());
			double minx = TMath::MinElement(npt, aux2->GetX()); 
			double maxx = TMath::MaxElement(npt, aux2->GetX());
				tini = minx;
				tend = maxx+1.;
			
			TString hs2("hrmZ");
			hs2 += i;
			
			hg = new TH1D(hs2, hs2, (int) (tend-tini), tini, tend);
			double *x = aux2->GetX();
			double *y = aux2->GetY();
			for (int k=0; k<npt; k++) if(x[k]>tini && x[k]<tend) {
			    ibin = (int) (x[k]-tini);
			    hg->SetBinContent(ibin, y[k]);
			}
			hg->Rebin(10);
			hg->Scale(1./10.);
			hg->Draw();//"P0"
	}

f->Close();
}

// ------------------------------------------------------------
showRun()
{
TDatime da(2007, 1, 1, 0, 0, 0);
TDatime dstart(2007, 5, 10, 0, 0, 0);
TDatime dstop(2007, 5, 12, 24, 0, 0);
 bool showtemp = kTRUE;  // if TRUE, print mean run temperature in table
 TString dataname("/data1/mcalsw_output_COMMISSIONING/misc/MCAL_global_data_COMMISSIONING_070511.root");
 TString hkname("/data1/COMMISSIONING_AGILE/070511/hk253-257.root");
long int ti = da.Convert();
 float  tini=dstart.Convert() - da.Convert();
 float  tend=dstop.Convert() - da.Convert();

 MCVFileInfo *info = new MCVFileInfo();
TFile *f= new TFile(dataname);
 TTree *gdata = (TTree *) f->Get("runInfo");
TBranch *br = gdata->GetBranch("run_info");
br->SetAddress(&info);

TGraphErrors *XA;
XA = f->GetObject("dsXA", XA);
TGraphErrors *ZA;
ZA = f->GetObject("dsZA", ZA);
TGraphErrors *XB;
XB = f->GetObject("dsXB", XB);
TGraphErrors *ZB;
ZB = f->GetObject("dsZB", ZB);

TGraph *TXA=NULL, *TXB=NULL, *TZA=NULL, *TZB=NULL, *TXZONE=NULL, *TZZONE=NULL;
TFile *hkf = NULL;

if (showtemp) {
	hkf= new TFile(hkname);
	TXA = hkf->GetObject("M6207_0_MCAL_XBARS_SIDE_A_TEMP", TXA);
	TXB = hkf->GetObject("M6208_0_MCAL_XBARS_SIDE_B_TEMP", TXB);
	TZA = hkf->GetObject("M6215_0_MCAL_ZBARS_SIDE_A_TEMP", TZA);
	TZB = hkf->GetObject("M6216_0_MCAL_ZBARS_SIDE_B_TEMP", TZB);
	TXZONE = hkf->GetObject("M6209_0_MCAL_MCAL_XZONE_TEMP", TXZONE);
	TZZONE = hkf->GetObject("M6217_0_MCAL_MCAB_ZZONE_TEMP", TZZONE);
}

 int nrun = gdata->GetEntries();
 printf("run \ttype \tt start \t\t\tduration (s) \tevents \t\trate (evt/s) \tdsXA \tdsZA \tdsXB \tdsZB\n");
 for (int i=0; i<nrun; i++) {
     gdata->GetEntry(i);
     if (info->tstart>tini && info->tstart<tend) {
	 TDatime dc;
	 dc.Set(ti+info->tstart);    // NB: TERMINE -3600 SOLO PER LE MISURE PRESE DOPO IL PASSAGGIO ALL'ORA LEGALE DEL 26/3/06
	 printf("%d \t%d \t%s \t%6.0f \t\t%7d \t%7.1f ", info->runid, info->pkttype, dc.AsString(), info->teff, info->nevents, info->nevents/info->teff);
	 if (info->pkttype == 3902) printf("\t%6.1f \t%6.1f \t%6.1f \t%6.1f \n", XA->Eval(0.5*(info->tstart+info->tstop)), ZA->Eval(0.5*(info->tstart+info->tstop)), XB->Eval(0.5*(info->tstart+info->tstop)), ZB->Eval(0.5*(info->tstart+info->tstop)));
	 else printf("\n");
     }
}

if (showtemp) {
	printf("\nTemperature information:\nrun \ttype \tt start \t\t\t TXA \tTXB \tTZA \tTZB \t\tTXZONE \t\tTZZONE\n");
	for (int i=0; i<nrun; i++) {
 		gdata->GetEntry(i);
     		if (info->tstart>tini && info->tstart<tend) {
	 		TDatime dc;
	 		dc.Set(ti+info->tstart);    // NB: TERMINE -3600 SOLO PER LE MISURE PRESE DOPO IL PASSAGGIO ALL'ORA LEGALE DEL 26/3/06
			float txa = -20. + 0.619*TXA->Eval(0.5*(info->tstart+info->tstop));
			float txb = -20. + 0.619*TXB->Eval(0.5*(info->tstart+info->tstop));
			float tza = -20. + 0.619*TZA->Eval(0.5*(info->tstart+info->tstop));
			float tzb = -20. + 0.619*TZB->Eval(0.5*(info->tstart+info->tstop));
			float txzone = -20. + 0.619*TXZONE->Eval(0.5*(info->tstart+info->tstop));
			float tzzone = -20. + 0.619*TZZONE->Eval(0.5*(info->tstart+info->tstop));
	 		printf("%d \t%d \t%s \t%6.1f \t%6.1f \t%6.1f \t%6.1f \t\t%6.1f \t\t%6.1f\n", info->runid, info->pkttype, dc.AsString(), txa, txb, tza, tzb, txzone, tzzone);
		}
	}
}

f->Close();
if (hkf) hkf->Close();
}
