init()
{
gROOT->Reset();
//TDatime da(2006, 1, 1, 0, 0, 0);
//TDatime dstart(2006, 7, 11, 0, 0, 0);
//TDatime dstop(2006, 7, 20, 24, 0, 0);
// TDatime da(2007, 1, 1, 0, 0, 0);
// TDatime dstart(2007, 2, 3, 20, 0, 0);
// TDatime dstop(2007, 2, 4, 2, 0, 0);

TDatime da(2006, 1, 1, 0, 0, 0);
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
TDatime da(2006, 1, 1, 0, 0, 0);
TDatime dstart(2007, 5, 10, 0, 0, 0);
TDatime dstop(2007, 5, 12, 24, 0, 0);
// TDatime da(2007, 1, 1, 0, 0, 0);
// TDatime dstart(2007, 2, 3, 21, 15, 0);
// TDatime dstop(2007, 2, 3, 22, 0, 0);
 float  tini=dstart.Convert() - da.Convert();
 float  tend=dstop.Convert() - da.Convert();
TCanvas *c1 = new TCanvas("c1", "global data", 800, 600);
c1->Divide(2, 2);

//TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_IABG_2006_TVTB.root");
//TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_IABG_2007_bus_power.root");
TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_COMMISSIONING_070511.root");

TGraphErrors *XA = NULL;
XA = f->GetObject("dsXA", XA);
c1->cd(1);
if (XA) {
	XA->GetXaxis()->SetTitle("Time");
	XA->GetXaxis()->SetTitleSize(0.05);
	XA->GetXaxis()->CenterTitle();
	XA->GetYaxis()->SetTitle("Variation from reference (%)");
	XA->GetYaxis()->CenterTitle();
	XA->GetYaxis()->SetTitleSize(0.05);
XA->GetXaxis()->SetTimeDisplay(1);
XA->GetXaxis()->SetTimeFormat("%H:%M");
XA->GetXaxis()->SetRangeUser(tini, tend);
XA->Draw("AP");
}

TGraphErrors *ZA = NULL;
ZA = f->GetObject("dsZA", ZA);
c1->cd(2);
if (ZA) {
	ZA->GetXaxis()->SetTitle("Time");
	ZA->GetXaxis()->SetTitleSize(0.05);
	ZA->GetXaxis()->CenterTitle();
	ZA->GetYaxis()->SetTitle("Variation from reference (%)");
	ZA->GetYaxis()->CenterTitle();
	ZA->GetYaxis()->SetTitleSize(0.05);
ZA->GetXaxis()->SetTimeDisplay(1);
ZA->GetXaxis()->SetTimeFormat("%H:%M");
ZA->GetXaxis()->SetRangeUser(tini, tend);
ZA->Draw("AP");
}

TGraphErrors *XB = NULL;
XB = f->GetObject("dsXB", XB);
c1->cd(3);
if (XB) {
	XB->GetXaxis()->SetTitle("Time");
	XB->GetXaxis()->SetTitleSize(0.05);
	XB->GetXaxis()->CenterTitle();
	XB->GetYaxis()->SetTitle("Variation from reference (%)");
	XB->GetYaxis()->CenterTitle();
	XB->GetYaxis()->SetTitleSize(0.05);
XB->GetXaxis()->SetTimeDisplay(1);
XB->GetXaxis()->SetTimeFormat("%H:%M");
XB->GetXaxis()->SetRangeUser(tini, tend);
XB->Draw("AP");
}

TGraphErrors *ZB = NULL;
ZB = f->GetObject("dsZB", ZB);
c1->cd(4);
if (ZB) {
	ZB->GetXaxis()->SetTitle("Time");
	ZB->GetXaxis()->SetTitleSize(0.05);
	ZB->GetXaxis()->CenterTitle();
	ZB->GetYaxis()->SetTitle("Variation from reference (%)");
	ZB->GetYaxis()->CenterTitle();
	ZB->GetYaxis()->SetTitleSize(0.05);
ZB->GetXaxis()->SetTimeDisplay(1);
ZB->GetXaxis()->SetTimeFormat("%H:%M");
ZB->GetXaxis()->SetRangeUser(tini, tend);
ZB->Draw("AP");
}

// work on global sci RM

TCanvas *c2 = new TCanvas("c2", "sci RM", 800, 600);
c2->Divide(6, 4);
char gname[10];
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
			aux->GetXaxis()->SetTimeDisplay(1);
			aux->GetXaxis()->SetTimeFormat("%H");
			aux->GetXaxis()->SetRangeUser(tini, tend);
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
			aux->GetXaxis()->SetTimeDisplay(1);
			aux->GetXaxis()->SetTimeFormat("%H");
			aux->GetXaxis()->SetRangeUser(tini, tend);
			aux->Draw("AP");
		}
	}

// work on puntual sci RM

TCanvas *c3 = new TCanvas("c3", "sci RM", 800, 600);
c3->Divide(6, 4);
	TGraph	*aux2;
//TH1F *hg;
double *x, *y;
long int npt;
int ibin;
char hname[10];
int rebinfactor[]={1, 10, 1, 1, 10, 10, 10, 10, 100, 100, 100};

	TCanvas *cx = new TCanvas("cx", "Plane X - Sci. RM 2 (170 - 350 keV)", 800, 400);
	TCanvas *cz = new TCanvas("cz", "Plane Z - Sci. RM 2 (170 - 350 keV)", 800, 400);
	for (int i=1; i<=11; i++) {
		// work on Sci RM X
		aux2 = NULL;
		sprintf(gname, "grmX%d", i);
		f->GetObject(gname, aux2);
		c3->cd(i);
		if (aux2) {
			aux2->GetXaxis()->SetTitle("Time (s)");
			aux2->GetXaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitle("Counts/s");
			aux2->GetYaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->SetTimeDisplay(1);
			aux2->GetXaxis()->SetTimeFormat("%H:%M");
			aux2->GetXaxis()->SetRangeUser(tini, tend);
			if (i==2) {
				cx->cd();
				aux2->Draw("AP");
				TLine *l;
				l = new TLine(0., 
				c3->cd(i);
			}

		sprintf(hname, "hrmX%d", i);

			TH1F *hg = new TH1F("hg", gname, (int) (tend-tini), tini, tend);
			npt = aux2->GetN();
			x = aux2->GetX();
			y = aux2->GetY();
			for (int k=0; k<npt; k++) if(x[k]>tini && x[k]<tend) {
			    ibin = (int) (x[k]-tini);
			    hg->SetBinContent(ibin, y[k]);
			}
			if (rebinfactor[i-1]>1) hg->Rebin(rebinfactor[i-1]);
			hg->Draw("P0");
		}
	
		// work on Sci RM Z
		aux2 = NULL;
		sprintf(gname, "grmZ%d", i);
		f->GetObject(gname, aux2);
		c3->cd(i+12);
		if (aux2) {
			aux2->GetXaxis()->SetTitle("Time (s)");
			aux2->GetXaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitle("Counts/s");
			aux2->GetYaxis()->CenterTitle();
			aux2->GetYaxis()->SetTitleSize(0.05);
			aux2->GetXaxis()->SetTimeDisplay(1);
			aux2->GetXaxis()->SetTimeFormat("%H:%M");
			aux2->GetXaxis()->SetRangeUser(tini, tend);
			//aux2->Draw("AP");
			if (i==2) {
				cz->cd();
				aux2->Draw("AP");
				c3->cd(i+12);
			}

		sprintf(hname, "hrmZ%d", i);

			TH1F *hg = new TH1F("hg", gname, (int) (tend-tini), tini, tend);
			npt = aux2->GetN();
			x = aux2->GetX();
			y = aux2->GetY();
			for (int k=0; k<npt; k++) if(x[k]>tini && x[k]<tend) {
				ibin = (int) (x[k]-tini);
			    hg->SetBinContent(ibin, y[k]);
			}
			if (rebinfactor[i-1]>1) hg->Rebin(rebinfactor[i-1]);
			hg->Draw("P0");
		}
	}


c3->cd(24);
TH1F *hg = new TH1F("hg", gname, (int) (tend-tini), tini, tend);  // SA IL SIGNORE PERCHE'!


f->Close();
}


showRun()
{
 TDatime da(2006, 1, 1, 0, 0, 0);
 TDatime dstart(2006, 6, 22, 0, 0, 0);
 TDatime dstop(2006, 6, 23, 24, 0, 0);
 bool showtemp = kTRUE;  // if TRUE, print mean run temperature in table
 TString dataname("/data1/mcalsw_output/misc/MCAL_global_data_IABG_2006_RFT-3.root");
 TString hkname("/data1/AGILE_MCAL_AIV_Data/IABG/RFT_3_060623_post_vibrazioni/hk_rft_3.root");
 //TString hkname("/data1/AGILE_MCAL_AIV_Data/IABG/TBTV/hk_iabg_TBTV_complete.root");
 //TString hkname("/data1/AGILE_MCAL_AIV_Data/IABG/RFT_4_060725_post_TBTV/hk_iabg_rft-4_post-TBTV.root");
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
	 dc.Set(ti+info->tstart - 3600);    // NB: TERMINE -3600 SOLO PER LE MISURE PRESE DOPO IL PASSAGGIO ALL'ORA LEGALE DEL 26/3/06
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
	 		dc.Set(ti+info->tstart - 3600);    // NB: TERMINE -3600 SOLO PER LE MISURE PRESE DOPO IL PASSAGGIO ALL'ORA LEGALE DEL 26/3/06
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
