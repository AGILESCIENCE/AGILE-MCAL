{
gROOT->Reset();
TFile *f= new TFile("/data1/COMMISSIONING_AGILE/GRB070622/843_MCAL_global_data.root");
TDatime da(2007, 1, 1, 0, 0, 0);
TDatime dstart(2007, 5, 10, 0, 0, 0);
TDatime dstop(2007, 5, 12, 24, 0, 0);
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

TH1D *hg;
TGraph *aux2;
int ibin;
char gname[10];
double tini=-1.;
double tend=-1.;
TCanvas *c4 = new TCanvas("c4", "sci RM (puntual)", 800, 600);
c4->Divide(6, 4);

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
}
