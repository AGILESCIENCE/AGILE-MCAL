// MM 17/06/09 analyze the results of scirm_analyses

{	
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);  // 2D graph colour version
	//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.07, "XYZ");
	mcalstyle->SetTitleSize(0.07, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	// open root file 1
	TFile fresults("./results_dec2008.root", "READ");
	fresults.ls();
	
	TH3F *h3xtot;
	TH3F *h3ztot;
	TH3F *h3xhr;
	TH3F *h3zhr;
	fresults.GetObject("h3xtot", h3xtot);
	fresults.GetObject("h3ztot", h3ztot);
	fresults.GetObject("h3xhr", h3xhr);
	fresults.GetObject("h3zhr", h3zhr);
	TH2D *h2xtot = (TH2D *) h3xtot->Project3D("zy");
	TH2D *h2ztot = (TH2D *) h3ztot->Project3D("zy");
	TH2D *h2xhr  = (TH2D *) h3xhr->Project3D("zy");
	TH2D *h2zhr  = (TH2D *) h3zhr->Project3D("zy");
	
	TCanvas c1("c1", "c1", 800, 800);
	c1.Divide(2,2);
	c1.cd(1);
	gPad->SetLogz();
	h2xtot->Draw("COLZ");
	c1.cd(2);
	gPad->SetLogz();
	h2ztot->Draw("COLZ");
	c1.cd(3);
	gPad->SetLogz();
	h2xhr->Draw("COLZ");
	c1.cd(4);
	gPad->SetLogz();
	h2zhr->Draw("COLZ");

	// open root file 2
	TFile fresults_2("./results_dec2007.root", "READ");
	fresults_2.ls();
	
	TH3F *h3xtot_2;
	TH3F *h3ztot_2;
	TH3F *h3xhr_2;
	TH3F *h3zhr_2;
	fresults_2.GetObject("h3xtot", h3xtot_2);
	fresults_2.GetObject("h3ztot", h3ztot_2);
	fresults_2.GetObject("h3xhr", h3xhr_2);
	fresults_2.GetObject("h3zhr", h3zhr_2);
	TH2D *h2xtot_2 = (TH2D *) h3xtot_2->Project3D("zy");
	TH2D *h2ztot_2 = (TH2D *) h3ztot_2->Project3D("zy");
	TH2D *h2xhr_2  = (TH2D *) h3xhr_2->Project3D("zy");
	TH2D *h2zhr_2  = (TH2D *) h3zhr_2->Project3D("zy");
	
	TCanvas c2("c2", "c2", 800, 800);
	c2.Divide(2,2);
	c2.cd(1);
	gPad->SetLogz();
	h2xtot_2->Draw("COLZ");
	c2.cd(2);
	gPad->SetLogz();
	h2ztot_2->Draw("COLZ");
	c2.cd(3);
	gPad->SetLogz();
	h2xhr_2->Draw("COLZ");
	c2.cd(4);
	gPad->SetLogz();
	h2zhr_2->Draw("COLZ");
	
	// select a geographical region
	int chmin =1;	// -20. deg
	int chmax =7;	// +50. deg
	TH1D *h1xtot = h2xtot->ProjectionY("h1xtot", chmin, chmax);
	TH1D *h1ztot = h2ztot->ProjectionY("h1ztot", chmin, chmax);
	TH1D *h1xhr  = h2xhr->ProjectionY("h1xhr", chmin, chmax);
	TH1D *h1zhr  = h2zhr->ProjectionY("h1zhr", chmin, chmax);
	
	TH1D *h1xtot_2 = h2xtot_2->ProjectionY("h1xtot_2", chmin, chmax);
	TH1D *h1ztot_2 = h2ztot_2->ProjectionY("h1ztot_2", chmin, chmax);
	TH1D *h1xhr_2  = h2xhr_2->ProjectionY("h1xhr_2", chmin, chmax);
	TH1D *h1zhr_2  = h2zhr_2->ProjectionY("h1zhr_2", chmin, chmax);
	
	// normalize histo
	cout << h1xtot->GetEntries() << endl;
	cout << h1ztot->GetEntries() << endl;
	cout << h1xhr->GetEntries() << endl;
	cout << h1zhr->GetEntries() << endl;
	cout << h1xtot_2->GetEntries() << endl;
	cout << h1ztot_2->GetEntries() << endl;
	cout << h1xhr_2->GetEntries() << endl;
	cout << h1zhr_2->GetEntries() << endl;
	int nh1xtot = h1xtot->GetEntries();
	int nh1ztot = h1ztot->GetEntries();
	int nh1xhr  = h1xhr->GetEntries();
	int nh1zhr  = h1zhr->GetEntries();
	int nh1xtot_2 = h1xtot_2->GetEntries();
	int nh1ztot_2 = h1ztot_2->GetEntries();
	int nh1xhr_2  = h1xhr_2->GetEntries();
	int nh1zhr_2  = h1zhr_2->GetEntries();
	h1xtot->Scale(1./nh1xtot);
	h1ztot->Scale(1./nh1ztot);
	h1xhr->Scale(1./nh1xhr);
	h1zhr->Scale(1./nh1zhr);
	h1xtot_2->Scale(1./nh1xtot_2);
	h1ztot_2->Scale(1./nh1ztot_2);
	h1xhr_2->Scale(1./nh1xhr_2);
	h1zhr_2->Scale(1./nh1zhr_2);
	
	TCanvas c3("c3", "c3", 800, 600);
	c3.Divide(2,2);
	c3.cd(1);
	gPad->SetLogy();
	h1xtot->Draw();
	h1xtot_2->SetLineColor(2);
	h1xtot_2->Draw("SAME");
	c3.cd(2);
	gPad->SetLogy();
	h1ztot->Draw();
	h1ztot_2->SetLineColor(2);
	h1ztot_2->Draw("SAME");
	c3.cd(3);
	gPad->SetLogy();
	h1xhr->Draw();
	h1xhr_2->SetLineColor(2);
	h1xhr_2->Draw("SAME");
	c3.cd(4);
	gPad->SetLogy();
	h1zhr->Draw();
	h1zhr_2->SetLineColor(2);
	h1zhr_2->Draw("SAME");
	
	
}
