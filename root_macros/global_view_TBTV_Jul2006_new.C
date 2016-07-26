init()
{
gROOT->Reset();
TDatime da(2006, 1, 1, 0, 0, 0);
TDatime dstart(2006, 7, 11, 0, 0, 0);
TDatime dstop(2006, 7, 20, 24, 0, 0);
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
TDatime dstart(2006, 7, 11, 0, 0, 0);
TDatime dstop(2006, 7, 20, 24, 0, 0);
 float  tini=dstart.Convert() - da.Convert();
 float  tend=dstop.Convert() - da.Convert();
TCanvas *c1 = new TCanvas("c1", "global data", 800, 600);
c1->Divide(2, 2);

TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_IABG_2006_TVTB.root");

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
}