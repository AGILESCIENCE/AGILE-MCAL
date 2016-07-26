// MM 18/06/09 work on a single 3D histo
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

	// open root file 1, contains 3D histo  
	// X axis = orbits  0. - 20000.,  bin 10 orbits  (2000)
	// Y axis = longitude -20. - 340., bin 10 deg (36 bin)
	// Z axis = ki2  0. - 100., bin 0.25 (400 bin)
	TFile fresults("~/data_analysis/scirm/results_6002-7300.root", "READ");
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
	
	// 1: Africa
	h3xtot->GetYaxis()->SetRange(1,7);
	h3ztot->GetYaxis()->SetRange(1,7);
	h3xhr->GetYaxis()->SetRange(1,7);
	h3zhr->GetYaxis()->SetRange(1,7);
	TH2D *h2xtot_1 = (TH2D *) h3xtot->Project3D("zx");
	TH2D *h2ztot_1 = (TH2D *) h3ztot->Project3D("zx");
	TH2D *h2xhr_1  = (TH2D *) h3xhr->Project3D("zx");
	TH2D *h2zhr_1  = (TH2D *) h3zhr->Project3D("zx");
	
	TCanvas c1("c1", "c1", 800, 800);
	c1.Divide(2,2);
	c1.cd(1);
	gPad->SetLogz();
	h2xtot_1->GetXaxis()->SetRangeUser(3000, 11000);
	h2xtot_1->DrawClone("COLZ");
	c1.cd(2);
	gPad->SetLogz();
	h2ztot_1->GetXaxis()->SetRangeUser(3000, 11000);
	h2ztot_1->DrawClone("COLZ");
	c1.cd(3);
	gPad->SetLogz();
	h2xhr_1->GetXaxis()->SetRangeUser(3000, 11000);
	h2xhr_1->DrawClone("COLZ");
	c1.cd(4);
	gPad->SetLogz();
	h2zhr_1->GetXaxis()->SetRangeUser(3000, 11000);
	h2zhr_1->DrawClone("COLZ");
	
	// dec 2008
	TCanvas c2("c2", "c2", 800, 800);
	c2.Divide(2,2);
	c2.cd(1);
	gPad->SetLogz();
	h2xtot_1->GetXaxis()->SetRangeUser(3100, 4000);//8294, 8731
	h2xtot_1->DrawClone("COLZ");
	c2.cd(2);
	gPad->SetLogz();
	h2ztot_1->GetXaxis()->SetRangeUser(3100, 4000);
	h2ztot_1->DrawClone("COLZ");
	c2.cd(3);
	gPad->SetLogz();
	h2xhr_1->GetXaxis()->SetRangeUser(3100, 4000);
	h2xhr_1->DrawClone("COLZ");
	c2.cd(4);
	gPad->SetLogz();
	h2zhr_1->GetXaxis()->SetRangeUser(3100, 4000);
	h2zhr_1->DrawClone("COLZ");
	
	// may 2009
	TCanvas c3("c3", "c3", 800, 800);
	c3.Divide(2,2);
	c3.cd(1);
	gPad->SetLogz();
	h2xtot_1->GetXaxis()->SetRangeUser(4000, 5000);//10425, 10863
	h2xtot_1->DrawClone("COLZ");
	c3.cd(2);
	gPad->SetLogz();
	h2ztot_1->GetXaxis()->SetRangeUser(4000, 5000);
	h2ztot_1->DrawClone("COLZ");
	c3.cd(3);
	gPad->SetLogz();
	h2xhr_1->GetXaxis()->SetRangeUser(4000, 5000);
	h2xhr_1->DrawClone("COLZ");
	c3.cd(4);
	gPad->SetLogz();
	h2zhr_1->GetXaxis()->SetRangeUser(4000, 5000);
	h2zhr_1->DrawClone("COLZ");
	
	//compare distributions
	
	// make projections
	int chmin_1 = 310;
	int chmax_1 = 400;
	TH1D *h1xtot_1 = h2xtot_1->ProjectionY("h1xtot", chmin_1, chmax_1);
	TH1D *h1ztot_1 = h2ztot_1->ProjectionY("h1ztot", chmin_1, chmax_1);
	TH1D *h1xhr_1  = h2xhr_1->ProjectionY("h1xhr", chmin_1, chmax_1);
	TH1D *h1zhr_1  = h2zhr_1->ProjectionY("h1zhr", chmin_1, chmax_1);
	
	int chmin_2 = 400;
	int chmax_2 = 500;
	TH1D *h1xtot_2 = h2xtot_1->ProjectionY("h1xtot_2", chmin_2, chmax_2);
	TH1D *h1ztot_2 = h2ztot_1->ProjectionY("h1ztot_2", chmin_2, chmax_2);
	TH1D *h1xhr_2  = h2xhr_1->ProjectionY("h1xhr_2", chmin_2, chmax_2);
	TH1D *h1zhr_2  = h2zhr_1->ProjectionY("h1zhr_2", chmin_2, chmax_2);
	
	// normalize histo
	cout << h1xtot_1->GetEntries() << endl;
	cout << h1ztot_1->GetEntries() << endl;
	cout << h1xhr_1->GetEntries() << endl;
	cout << h1zhr_1->GetEntries() << endl;
	cout << h1xtot_2->GetEntries() << endl;
	cout << h1ztot_2->GetEntries() << endl;
	cout << h1xhr_2->GetEntries() << endl;
	cout << h1zhr_2->GetEntries() << endl;
	int nh1xtot_1 = h1xtot_1->GetEntries();
	int nh1ztot_1 = h1ztot_1->GetEntries();
	int nh1xhr_1  = h1xhr_1->GetEntries();
	int nh1zhr_1  = h1zhr_1->GetEntries();
	int nh1xtot_2 = h1xtot_2->GetEntries();
	int nh1ztot_2 = h1ztot_2->GetEntries();
	int nh1xhr_2  = h1xhr_2->GetEntries();
	int nh1zhr_2  = h1zhr_2->GetEntries();
	h1xtot_1->Scale(1./nh1xtot_1);
	h1ztot_1->Scale(1./nh1ztot_1);
	h1xhr_1->Scale(1./nh1xhr_1);
	h1zhr_1->Scale(1./nh1zhr_1);
	h1xtot_2->Scale(1./nh1xtot_2);
	h1ztot_2->Scale(1./nh1ztot_2);
	h1xhr_2->Scale(1./nh1xhr_2);
	h1zhr_2->Scale(1./nh1zhr_2);
	
	TCanvas c4("c4", "c4", 800, 600);
	c4.Divide(2,2);
	c4.cd(1);
	gPad->SetLogy();
	h1xtot_1->Draw();
	h1xtot_2->SetLineColor(2);
	h1xtot_2->Draw("SAME");
	c4.cd(2);
	gPad->SetLogy();
	h1ztot_1->Draw();
	h1ztot_2->SetLineColor(2);
	h1ztot_2->Draw("SAME");
	c4.cd(3);
	gPad->SetLogy();
	h1xhr_1->Draw();
	h1xhr_2->SetLineColor(2);
	h1xhr_2->Draw("SAME");
	c4.cd(4);
	gPad->SetLogy();
	h1zhr_1->Draw();
	h1zhr_2->SetLineColor(2);
	h1zhr_2->Draw("SAME");
/*	// open root file 2
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
	

*/
	
}
