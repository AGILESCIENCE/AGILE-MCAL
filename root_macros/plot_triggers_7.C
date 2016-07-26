int plot_triggers_7(char *inpath="./tmp.root", char *outpath="./output.dat")
{
	// MM 08/04/15 parameterized input file, based on plot_triggers_5.C
	// MM 18/10/12 added selection criteria for HE trigger analysis (trg 4)

	// MM 20/08/12 based on macro plot_triggers_4.C
	//             uses TChain to concatenate triggers data previously stored in root trees. Much faster!
	//	       NB: to save new root files remember to uncomment at line 106!
	
	gROOT->Reset();
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

	struct offline_trg {
		int contact;
		int id;
		double t0;
		int counts;
		float hr;
		float dt;
		float lon;
		float lat;
		float h;
		float etheta;
		float ephi;
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int usec;
		float loctime;
		float eavg;
		float emax;
		int boost;
		int flag;
		int tgflag;
		int c[8];
	};
	offline_trg atrg;

	TChain ttrg("triggers");	
	ttrg.Add("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/trg_2_040418-041012_processed.root");
	ttrg.Add(inpath);
	
	ttrg.SetBranchAddress("contact",&atrg.contact);
	ttrg.SetBranchAddress("id",&atrg.id);
	ttrg.SetBranchAddress("t0",&atrg.t0);
	ttrg.SetBranchAddress("counts",&atrg.counts);
	ttrg.SetBranchAddress("hr",&atrg.hr);
	ttrg.SetBranchAddress("dt",&atrg.dt);
	ttrg.SetBranchAddress("lon",&atrg.lon);
	ttrg.SetBranchAddress("lat",&atrg.lat);
	ttrg.SetBranchAddress("h",&atrg.h);
	ttrg.SetBranchAddress("etheta",&atrg.etheta);
	ttrg.SetBranchAddress("ephi",&atrg.ephi);
	ttrg.SetBranchAddress("year",&atrg.year);
	ttrg.SetBranchAddress("month",&atrg.month);
	ttrg.SetBranchAddress("day",&atrg.day);
	ttrg.SetBranchAddress("hour",&atrg.hour);
	ttrg.SetBranchAddress("min",&atrg.min);
	ttrg.SetBranchAddress("sec",&atrg.sec);
	ttrg.SetBranchAddress("usec",&atrg.usec);
	ttrg.SetBranchAddress("loctime",&atrg.loctime);
	ttrg.SetBranchAddress("eavg",&atrg.eavg);
	ttrg.SetBranchAddress("emax",&atrg.emax);
	ttrg.SetBranchAddress("boost",&atrg.boost);
	ttrg.SetBranchAddress("flag",&atrg.flag);
	ttrg.SetBranchAddress("tgflag",&atrg.tgflag);
	ttrg.SetBranchAddress("c0",&atrg.c[0]);
	ttrg.SetBranchAddress("c1",&atrg.c[1]);
	ttrg.SetBranchAddress("c2",&atrg.c[2]);
	ttrg.SetBranchAddress("c3",&atrg.c[3]);
	ttrg.SetBranchAddress("c4",&atrg.c[4]);
	ttrg.SetBranchAddress("c5",&atrg.c[5]);
	ttrg.SetBranchAddress("c6",&atrg.c[6]);
	ttrg.SetBranchAddress("c7",&atrg.c[7]);
	int nentries = ttrg.GetEntries();
	cout << "Total number of entries = " << nentries << endl;


// salva su file selezione migliore corrente 
//(cut12 && cut_top2 && !cut_deb1 && !cut_deb10) (per TGF standard ricavati con burst_search_2)
	int i=0, nt=0;
//	TFile *newfile = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/selection.root","recreate");
//	TFile *newfile = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_4/trg4_selection_01.root","recreate");
//	TFile *newfile = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-Dec13.root","recreate");
//	TFile *newfile = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/HE_TGF_Mar09-Dec13.root","recreate");
//	TFile *newfile = new TFile("/home/mcal/data_analysis/VSB/offline_trigger_search/trigger_search_2/LE_TGF_Mar09-17Feb15.root","recreate");
	TFile *newfile = new TFile(outpath,"recreate");
	TTree *newtree = ttrg.CloneTree(0);
	while (ttrg.GetEntry(i++)) { // NB: check the selection on max energy (E_max <= or > 30 MeV) 
		
		if (atrg.boost == 0  && atrg.flag == 1 && atrg.contact != 10553 && atrg.contact >= 40900 && atrg.counts>9 && atrg.hr>=0.5 && atrg.h>0. && atrg.emax<30. && (atrg.c[0] + atrg.c[2]) > 0 && (atrg.c[1] + atrg.c[3]) > 0 && (atrg.c[4] + atrg.c[6]) > 0 && (atrg.c[5] + atrg.c[7]) > 0) {
		
		//if (atrg.boost == 0  && atrg.flag == 1 && atrg.contact != 10553 && atrg.contact != 17083 && atrg.lon >= -180. && atrg.lon <= 180. && (atrg.lon <= -70. || atrg.lon >= 0.)) {
		
		//if (atrg.boost == 0  && atrg.year == 2011 && atrg.month == 10 && atrg.day == 23 && atrg.lon >= -180. && atrg.lon <= 180. ) {
			nt++;
			//printf("%6d \t%20.6f \t%.6f \t%.2f \t%.2f \t%.2f \t%.2f \t%.2f\n", atrg.contact, atrg.t0, atrg.dt, atrg.counts, atrg.hr, atrg.lon, atrg.lat, atrg.loctime);
			printf("%6d \t%3d \t%20.6f\n", atrg.contact, atrg.id, atrg.t0); // list for timing analysis
			
			newtree->Fill();
		}
	}
	cout << nt << endl; 
	newtree->Print();
	newtree->Write("triggers");
	delete newfile;


//TCut cut0 = "tgflag == 1 && contact<12511";	// TGF consolidate sample
TCut cut0 = "tgflag == 1";	// TGF consolidate sample
TCut cut1 = "boost == 1 ";			// BOOST sample
TCut cut2 = "boost == 0 && flag == 1";		// non-BOOST on-board triggers
TCut cut3 = "boost == 0 && counts>9 && hr>0.5 && h>0.";	// TGF-paper-like selection
TCut cut4 = "boost == 0 && h>0. && dt>0";	// 
TCut cut5 = "boost == 0 && counts>8 && hr>0.5 && h>0. && dt>0.";	// relaxed selection
TCut cut6 = "boost == 0 && counts>8 && hr>0.4 && h>0. && dt>0.";	// relaxed selection
TCut cut7 = "boost == 0 && counts>7 && hr>1. && h>0. && dt>0.";	// relaxed selection
TCut cut8 = "boost == 0 && flag == 1 && counts>6 && hr>=0.4 && h>0. && dt>0.";	// relaxed selection
TCut cut9 = "boost == 0 && flag == 1 && counts==8 && h>0. && dt>0.";	// relaxed selection
TCut cut10 = "boost == 0 && counts>7 && hr>0.4 && h>0. && dt>0.";	// relaxed selection
TCut cut11 = "boost == 0 && counts>9 && hr>0.4 && h>0. && dt>0.";	// relaxed selection
TCut cut12 = "boost == 0 && flag == 1 && counts>9 && hr>=0.5 && h>0.";	// TGF-paper-like selection (requires onboard trigger)
TCut cut13 = "boost == 0 && flag == 1 && counts==9 && hr>=0.5 && h>0. && dt>0.";	
TCut cut14 = "boost == 0  && flag == 1  && counts>9";	//  && flag == 1 && counts>9 TGF-paper-like selection

// debug cuts
TCut cut_deb0 = "etheta>=147. && etheta<=149";	// extra picco outlier usando cut4
TCut cut_deb1 = "contact == 10553";		// 
TCut cut_deb2 = "loctime<6.5"; 
TCut cut_deb3 = "emax > 50."; 
TCut cut_deb4 = "lon >= -15. && lon <= -7."; 
TCut cut_deb5 = "(c0+c1+c2+c3+c4+c5+c6+c7)/(counts) > 2."; 	// high multiple / single ratio
TCut cut_deb6 = "loctime>5. && loctime<6.5"; 
TCut cut_deb7 = "emax > 100."; 
TCut cut_deb8 = "lon <= -82."; 
TCut cut_deb9 = "loctime>3. && loctime<5."; 
TCut cut_deb10 = "emax > 30."; 
TCut cut_deb11 = "counts<10 || hr<0.5"; 
TCut cut_deb12 = "contact == 17083";		// 
TCut cut_deb13 = "lon >= -70. && lon <= 0."; 
TCut cut_deb14 = "lon >= -180. && lon <= 180."; 

// topological cuts (NB: sembra che il casting float non serva)
TCut cut_top1 = "(c0+c1+c2+c3-c4-c5-c6-c7)/(c0+c1+c2+c3+c4+c5+c6+c7) > -0.5 && (c0+c1+c2+c3-c4-c5-c6-c7)/(c0+c1+c2+c3+c4+c5+c6+c7)<0.7 && (c0+c1+c2+c3+c4+c5+c6+c7)/(counts) <= 2. && (c2+c3-c0-c1)/(c0+c1+c2+c3)>-0.8 && (c2+c3-c0-c1)/(c0+c1+c2+c3)<0.8 && (c6+c7-c4-c5)/(c4+c5+c6+c7)>-0.8 &&  (c6+c7-c4-c5)/(c4+c5+c6+c7)<0.8";

// TCut cut_top1 = "((float)(c0+c1+c2+c3-c4-c5-c6-c7))/(c0+c1+c2+c3+c4+c5+c6+c7) > -0.5 && ((float)(c0+c1+c2+c3-c4-c5-c6-c7))/(c0+c1+c2+c3+c4+c5+c6+c7)<0.7 && ((float)(c0+c1+c2+c3+c4+c5+c6+c7))/(counts) <= 2. && ((float)(c2+c3-c0-c1))/(c0+c1+c2+c3)>-0.8 && ((float)(c2+c3-c0-c1))/(c0+c1+c2+c3)<0.8 && ((float)(c6+c7-c4-c5))/(c4+c5+c6+c7)>-0.8 &&  ((float)(c6+c7-c4-c5))/(c4+c5+c6+c7)<0.8"

TCut cut_top2 = "(c0+c2) > 0 && (c1+c3) > 0 && (c4+c6) > 0 && (c5+c7) > 0";
TCut cut_top3 = "(c0+c1) > 0 && (c2+c3) > 0 && (c4+c5) > 0 && (c6+c7) > 0";

//TCut cut = cut8 && cut_top1 && !cut_deb1 && !cut_deb7;  // <-- this is good!
//TCut cut = cut3 && cut_top1 && !cut_deb1 && cut_deb10;
//TCut cut = cut8 && cut_top1 && !cut_deb1 && !cut_deb10 && cut_deb11;
//TCut cut = cut10 && cut_top1 && !cut_deb1 && !cut_deb3 && !cut_deb6;

//TCut cut = cut8 && cut_top1 && !cut_deb1 && !cut_deb10;  
//TCut cut = cut12 && cut_top2 && !cut_deb1 && !cut_deb10;  
TCut cut = cut2 && !cut_deb1 && !cut_deb12 && cut_deb14 && !cut_deb13;  

// ------------------------------
// print selections
// ------------------------------
int i=0;
// HE events burst_search_4 (to understand high energy events outside the SAA)
//while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.flag == 1 && atrg.counts>9 && atrg.contact != 17083) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f\n", atrg.contact, atrg.t0, atrg.dt,  atrg.lon, atrg.lat, atrg.counts, atrg.emax);};
//while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 & (atrg.lon<-70. || atrg.lon>0.) && atrg.contact != 17083) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f\n", atrg.contact, atrg.t0, atrg.dt,  atrg.lon, atrg.lat, atrg.counts, atrg.emax);};

/*while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.counts>9 && atrg.hr>0.5 && atrg.h>0. && atrg.emax>50.) printf("%6d \t%20.6f \t%8.2f\n", atrg.contact, atrg.t0, atrg.emax);};

while (ttrg.GetEntry(i++)) {if (atrg.tgflag == 1 && atrg.contact<12511) printf("%6d \t%20.6f \t%8.2f \t%8.2f \t%4d \t%.6f \t%8.2f \t%8.2f \t%8.2f \t%8.2f\n", atrg.contact, atrg.t0, atrg.lon, atrg.lat, atrg.counts, atrg.dt, atrg.etheta, atrg.ephi, atrg.loctime, atrg.hr);};

// formato for makespectra (new version 27/11/09)
// cut0
while (ttrg.GetEntry(i++)) {if (atrg.tgflag == 1 && atrg.contact<12511) printf("%6d \t%20.6f \t%.6f\n", atrg.contact, atrg.t0, atrg.dt);};

// cut0 && !cut_top1 // NB: non stampa niente ???
while (ttrg.GetEntry(i++)) {if (atrg.tgflag == 1 && ((atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7]) <= -0.5 || (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])>=0.7 || (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])/(atrg.counts) > 2. || (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])<=-0.8 || (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])>=0.8 || (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])<=-0.8 ||  (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])>=0.8)) printf("%6d \t%20.6f \t%.6f\n", atrg.contact, atrg.t0, atrg.dt);};

//cut3 && cut_deb3 && !cut_deb1;
while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.counts>9 && atrg.hr>0.5 && atrg.h>0. && atrg.emax > 50. && atrg.contact!=10553) printf("%6d \t%20.6f \t%.6f\n", atrg.contact, atrg.t0, atrg.dt);};

// cut5 && cut_deb5 (to understand high energy events outside the SAA)
while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.counts>8 && atrg.hr>0.5 && atrg.h>0. && atrg.dt>0. &&  (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])/((float) atrg.counts) > 2.) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f\n", atrg.contact, atrg.t0, atrg.dt,  atrg.lon, atrg.lat, atrg.counts, atrg.emax);};

// cut8 && cut_top1 && !cut_deb1 && !cut_deb7; (relaxed selection, doubles statistics)
while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.flag == 1 && atrg.counts>6 && atrg.hr>0.4 && atrg.h>0. && atrg.dt>0. && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7]) > -0.5 && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])<0.7 && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])/(atrg.counts) <= 2. && (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])>-0.8 && (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])<0.8 && (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])>-0.8 &&  (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])<0.8 && atrg.contact != 10553 && atrg.emax <= 100.) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f \n", atrg.contact, atrg.t0, atrg.dt, atrg.lon, atrg.lat, atrg.counts, atrg.loctime);};

// cut3 && cut_top1 && !cut_deb1 && !cut_deb10; (standard selection, no trigger requirement, increase statistics)
while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && atrg.counts>9 && atrg.hr>0.5 && atrg.h>0. && atrg.dt>0. && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7]) > -0.5 && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]-atrg.c[4]-atrg.c[5]-atrg.c[6]-atrg.c[7])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])<0.7 && (atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3]+atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])/(atrg.counts) <= 2. && (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])>-0.8 && (atrg.c[2]+atrg.c[3]-atrg.c[0]-atrg.c[1])/(atrg.c[0]+atrg.c[1]+atrg.c[2]+atrg.c[3])<0.8 && (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])>-0.8 &&  (atrg.c[6]+atrg.c[7]-atrg.c[4]-atrg.c[5])/(atrg.c[4]+atrg.c[5]+atrg.c[6]+atrg.c[7])<0.8 && atrg.contact != 10553 && atrg.emax <= 40.) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f \n", atrg.contact, atrg.t0, atrg.dt, atrg.lon, atrg.lat, atrg.counts, atrg.loctime);};

// HE events burst_search_4 (to understand high energy events outside the SAA)
while (ttrg.GetEntry(i++)) {if (atrg.boost == 0 && flag == 1 && atrg.counts>9 ) printf("%6d \t%20.6f \t%.6f \t%8.2f \t%8.2f \t%6d \t%8.2f\n", atrg.contact, atrg.t0, atrg.dt,  atrg.lon, atrg.lat, atrg.counts, atrg.emax);};

// ------------------------------
// main distributions canvas 
// ------------------------------

// canvas 3x4
TCanvas c2("c2", "c2", 1000, 800);
c2.Divide(3,4);

// draw current cut

c2.cd(1);
ttrg.Draw("(year-2008)*12+month", cut);
c2.cd(4);
ttrg.Draw("loctime", cut);
c2.cd(7);
ttrg.Draw("lon", cut);	
c2.cd(10);
ttrg.Draw("lat", cut);

c2.cd(2);
ttrg.Draw("etheta", cut);
c2.cd(5);
ttrg.Draw("ephi", cut);
c2.cd(8);
gPad->SetLogy();
ttrg.Draw("counts", cut);
c2.cd(11);
gPad->SetLogx();
gPad->SetLogy();
ttrg.Draw("dt", cut);

c2.cd(3);
gPad->SetLogy();
ttrg.Draw("hr", cut && "hr<20");
c2.cd(6);
gPad->SetLogy();
gPad->SetLogx();
ttrg.Draw("eavg", cut);
c2.cd(9);
gPad->SetLogx();
gPad->SetLogy();
ttrg.Draw("emax", cut);
c2.cd(12);
gPad->SetLogx();
gPad->SetLogy();
ttrg.Draw("dt/(counts-1)", cut && "dt>0");

//
// draw cut0 (class A TGF sample as reference superimposed to current cut)
//

c2.cd(1);
TH1F *h1 = (TH1F *) gPad->GetPrimitive("htemp");
h1->SetLineColor(2);
gPad->Update();
ttrg.Draw("(year-2008)*12+month", cut0, "same");
c2.cd(4);
TH1F *h4 = (TH1F *) gPad->GetPrimitive("htemp");
h4->SetLineColor(2);
gPad->Update();
ttrg.Draw("loctime", cut0, "same");
c2.cd(7);
TH1F *h7 = (TH1F *) gPad->GetPrimitive("htemp");
h7->SetLineColor(2);
gPad->Update();
ttrg.Draw("lon", cut0, "same");	
c2.cd(10);
TH1F *h10 = (TH1F *) gPad->GetPrimitive("htemp");
h10->SetLineColor(2);
gPad->Update();
ttrg.Draw("lat", cut0, "same");

c2.cd(2);
TH1F *h2 = (TH1F *) gPad->GetPrimitive("htemp");
h2->SetLineColor(2);
gPad->Update();
ttrg.Draw("etheta", cut0, "same");
c2.cd(5);
TH1F *h5 = (TH1F *) gPad->GetPrimitive("htemp");
h5->SetLineColor(2);
gPad->Update();
ttrg.Draw("ephi", cut0, "same");
c2.cd(8);
gPad->SetLogy();
TH1F *h8 = (TH1F *) gPad->GetPrimitive("htemp");
h8->SetLineColor(2);
gPad->Update();
ttrg.Draw("counts", cut0, "same");
c2.cd(11);
gPad->SetLogx();
gPad->SetLogy();
TH1F *h11 = (TH1F *) gPad->GetPrimitive("htemp");
h11->SetLineColor(2);
gPad->Update();
ttrg.Draw("dt", cut0, "same");

c2.cd(3);
gPad->SetLogy();
TH1F *h3 = (TH1F *) gPad->GetPrimitive("htemp");
h3->SetLineColor(2);
gPad->Update();
ttrg.Draw("hr", cut0, "same");
c2.cd(6);
gPad->SetLogy();
gPad->SetLogx();
TH1F *h6 = (TH1F *) gPad->GetPrimitive("htemp");
h6->SetLineColor(2);
gPad->Update();
ttrg.Draw("eavg", cut0, "same");
c2.cd(9);
gPad->SetLogx();
gPad->SetLogy();
TH1F *h9 = (TH1F *) gPad->GetPrimitive("htemp");
h9->SetLineColor(2);
gPad->Update();
ttrg.Draw("emax", cut0, "same");
c2.cd(12);
gPad->SetLogx();
gPad->SetLogy();
TH1F *h12 = (TH1F *) gPad->GetPrimitive("htemp");
h12->SetLineColor(2);
gPad->Update();
ttrg.Draw("dt/(counts-1)", cut0 && "dt>0", "same");


// ------------------------------
// geographic distribution canvas 
// ------------------------------

TCut cut_PacificO  = "(lon>=-180. && lon<-90.) || (lon>=150. && lon<=180.)"; 
TCut cut_SAmerica  = "lon>=-90. && lon<-40."; 
TCut cut_AtlanticO = "lon>=-40. && lon<-10."; 
TCut cut_Africa    = "lon>=-10. && lon<50."; 
TCut cut_IndianO   = "lon>=50. && lon<90."; 
TCut cut_SEAsia    = "lon>=90. && lon<150."; 


TCanvas c4("c4", "c4", 1200, 800);
c4.Divide(6,4);

c4.cd(1);
ttrg.Draw("(year-2008)*12+month", cut && cut_PacificO);
c4.cd(2);
ttrg.Draw("(year-2008)*12+month", cut && cut_SAmerica);
c4.cd(3);
ttrg.Draw("(year-2008)*12+month", cut && cut_AtlanticO);
c4.cd(4);
ttrg.Draw("(year-2008)*12+month", cut && cut_Africa);
c4.cd(5);
ttrg.Draw("(year-2008)*12+month", cut && cut_IndianO);
c4.cd(6);
ttrg.Draw("(year-2008)*12+month", cut && cut_SEAsia);

c4.cd(7);
ttrg.Draw("loctime", cut && cut_PacificO);
c4.cd(8);
ttrg.Draw("loctime", cut && cut_SAmerica);
c4.cd(9);
ttrg.Draw("loctime", cut && cut_AtlanticO);
c4.cd(10);
ttrg.Draw("loctime", cut && cut_Africa);
c4.cd(11);
ttrg.Draw("loctime", cut && cut_IndianO);
c4.cd(12);
ttrg.Draw("loctime", cut && cut_SEAsia);

// ---------------
// topology canvas 
// ---------------

TCanvas c3("c3", "c3", 1000, 800);
c3.Divide(4,4);

// draw current cut

TH1F *ht;

c3.cd(1);
gPad->SetLogy();
ttrg.Draw("c0", cut);
c3.cd(2);
gPad->SetLogy();
ttrg.Draw("c1", cut);
c3.cd(3);
gPad->SetLogy();
ttrg.Draw("c2", cut);
c3.cd(4);
gPad->SetLogy();
ttrg.Draw("c3", cut);

c3.cd(5);
gPad->SetLogy();
ttrg.Draw("c4", cut);
c3.cd(6);
gPad->SetLogy();
ttrg.Draw("c5", cut);
c3.cd(7);
gPad->SetLogy();
ttrg.Draw("c6", cut);
c3.cd(8);
gPad->SetLogy();
ttrg.Draw("c7", cut);

c3.cd(9);
gPad->SetLogy();
ttrg.Draw("(c1+c3-c0-c2)/(c0+c1+c2+c3)", cut);    // X-plane X-axis asymmetry
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);
c3.cd(10);
gPad->SetLogy();
ttrg.Draw("(c2+c3-c0-c1)/(c0+c1+c2+c3)", cut);   // X-plane Z-axis asymmetry
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);
c3.cd(11);
gPad->SetLogy();
ttrg.Draw("(c6+c7-c4-c5)/(c4+c5+c6+c7)", cut);  // Z-plane X-axis asymmetry
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);
c3.cd(12);
gPad->SetLogy();
ttrg.Draw("(c5+c7-c4-c6)/(c4+c5+c6+c7)", cut);   // Z-plane Z-axis asymmetry
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);

c3.cd(13);
gPad->SetLogy();
ttrg.Draw("(c0+c1+c2+c3-c4-c5-c6-c7)/(c0+c1+c2+c3+c4+c5+c6+c7)", cut);	// top/bottom asymmetry
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);
c3.cd(14);
gPad->SetLogy();
ttrg.Draw("(c0+c1+c2+c3+c4+c5+c6+c7)/(counts)", cut);	// ~multiple/single ratio
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->Rebin(10);
c3.cd(15);
// ttrg.Draw("c6", cut);
c3.cd(16);
// ttrg.Draw("c7", cut);

//
// draw cut0 (class A TGF sample as reference superimposed to current cut)
//

c3.cd(1);
ht = (TH1F *) gPad->GetPrimitive("htemp");
ht->SetLineColor(2);
gPad->Update();
ttrg.Draw("c0", cut0, "same");
c3.cd(2);
TH1F *ht2 = (TH1F *) gPad->GetPrimitive("htemp");
ht2->SetLineColor(2);
gPad->Update();
ttrg.Draw("c1", cut0, "same");
c3.cd(3);
TH1F *ht3 = (TH1F *) gPad->GetPrimitive("htemp");
ht3->SetLineColor(2);
gPad->Update();
ttrg.Draw("c2", cut0, "same");
c3.cd(4);
TH1F *ht4 = (TH1F *) gPad->GetPrimitive("htemp");
ht4->SetLineColor(2);
gPad->Update();
ttrg.Draw("c3", cut0, "same");

c3.cd(5);
TH1F *ht5 = (TH1F *) gPad->GetPrimitive("htemp");
ht5->SetLineColor(2);
gPad->Update();
ttrg.Draw("c4", cut0, "same");
c3.cd(6);
TH1F *ht6 = (TH1F *) gPad->GetPrimitive("htemp");
ht6->SetLineColor(2);
gPad->Update();
ttrg.Draw("c5", cut0, "same");
c3.cd(7);
TH1F *ht7 = (TH1F *) gPad->GetPrimitive("htemp");
ht7->SetLineColor(2);
gPad->Update();
ttrg.Draw("c6", cut0, "same");
c3.cd(8);
TH1F *ht8 = (TH1F *) gPad->GetPrimitive("htemp");
ht8->SetLineColor(2);
gPad->Update();
ttrg.Draw("c7", cut0, "same");

c3.cd(9);
TH1F *ht9 = (TH1F *) gPad->GetPrimitive("htemp");
ht9->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c1+c3-c0-c2)/(c0+c1+c2+c3)", cut0, "same");    // X-plane X-axis asymmetry
c3.cd(10);
TH1F *ht10 = (TH1F *) gPad->GetPrimitive("htemp");
ht10->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c2+c3-c0-c1)/(c0+c1+c2+c3)", cut0, "same");   // X-plane Z-axis asymmetry
c3.cd(11);
TH1F *ht11 = (TH1F *) gPad->GetPrimitive("htemp");
ht11->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c6+c7-c4-c5)/(c4+c5+c6+c7)", cut0, "same");  // Z-plane X-axis asymmetry
c3.cd(12);
TH1F *ht12 = (TH1F *) gPad->GetPrimitive("htemp");
ht12->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c5+c7-c4-c6)/(c4+c5+c6+c7)", cut0, "same");   // Z-plane Z-axis asymmetry

c3.cd(13);
TH1F *ht13 = (TH1F *) gPad->GetPrimitive("htemp");
ht13->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c0+c1+c2+c3-c4-c5-c6-c7)/(c0+c1+c2+c3+c4+c5+c6+c7)", cut0, "same");	// top/bottom asymmetry
c3.cd(14);
TH1F *ht14 = (TH1F *) gPad->GetPrimitive("htemp");
ht14->SetLineColor(2);
gPad->Update();
ttrg.Draw("(c0+c1+c2+c3+c4+c5+c6+c7)/(counts)", cut0, "same");	// ~multiple/single ratio

// -----------
// miscellanea
// -----------

int ind =  ttrg.BuildIndex("t0");

*/
	
	
	
/*	TH3F *topology = new TH3F("topology", "topology", 100, 0., 100.,  50, 0., 0.010, 50, 0., 5.); // counts, dt, hr
	
	// build histo for waiting times
	float tini = 0.1;
	float tfin = 100000.;
	int nbins = 24;
	float fact = (10.)**(1./4.);
	float abin[42];
	abin[0] = 0.1;
	for (int i=1; i<=nbins; i++) abin[i] = abin[i-1]*fact;
	TH1F *deltat = new TH1F("deltat","deltat", nbins, abin);	
	
	int index = 0;
	int sel = 0;
	char aux[10], path[100];
	int trgid, counts[100000], contact[100000], bflag[100000];
	double t0[100000], hr[100000], dt[100000], lon[100000], lat[100000], h[100000], etheta[100000], ephi[100000];
	while (inFile >> contact[sel] >> trgid >> t0[sel] >> counts[sel] >> hr[sel] >> dt[sel] >> lon[sel] >> lat[sel] >> h[sel] >> etheta[sel] >> ephi[sel] >> bflag[sel]) {
		//g1->SetPoint(index, hr, counts);
		if (bflag[sel]==0) topology->Fill(counts[sel], dt[sel], hr[sel]);
		//if (counts[sel]>=50. || dt[sel]>=0.010 || hr[sel]>=5.) cout << path << "  " << t0 << "  " << counts[sel] << "  " << hr[sel] << "  " << dt[sel] << endl;
		// do main selection
		if (hr[sel]>=0.5 && counts[sel]>=10 && bflag[sel]==0) {
			printf("%4d \t%06d \t%.6f \t%d \t%.2f \t%.6f\n", sel, contact[sel], t0[sel], counts[sel], hr[sel], dt[sel]);
			if (sel) deltat->Fill(t0[sel]-t0[sel-1]);
			sel++;
		}
		
		index++;
	}
	cout << "Processed " << index << " triggers" << endl;
	cout << "Selected  " << sel << " triggers" << endl;
	inFile.close();
	
	TCanvas *c9 = new TCanvas ("c9", "c9", 800, 600);
	//g1->Draw("AP");
	c9->Divide(3,2);
	
	c9->cd(1);
	gPad->SetLogz(1);
	TH2F *zyt = topology->Project3D("zy");
	zyt->SetStats(0);
	zyt->SetTitle("HR vs. dt");
	zyt->GetXaxis()->SetTitle("dt");
	zyt->GetXaxis()->CenterTitle();
	zyt->GetYaxis()->SetTitle("HR");
	zyt->GetYaxis()->CenterTitle();
	zyt->GetYaxis()->SetTitleOffset(2.);
	zyt->DrawClone("colZ");
	
	c9->cd(2);
	gPad->SetLogz(1);
	TH2F *xyt = topology->Project3D("yx");
	xyt->SetStats(0);
	xyt->SetTitle("dt vs. counts");
	xyt->GetXaxis()->SetTitle("counts");
	xyt->GetXaxis()->CenterTitle();
	xyt->GetYaxis()->SetTitle("dt");
	xyt->GetYaxis()->CenterTitle();
	xyt->DrawClone("colZ");
	
	c9->cd(3);
	gPad->SetLogz(1);
	TH2F *zxt = topology->Project3D("zx");
	zxt->SetStats(0);
	zxt->SetTitle("HR vs. counts");
	zxt->GetXaxis()->SetTitle("counts");
	zxt->GetXaxis()->CenterTitle();
	zxt->GetYaxis()->SetTitle("HR");
	zxt->GetYaxis()->CenterTitle();
	zxt->GetYaxis()->SetTitleOffset(2.);
	zxt->DrawClone("colZ");	
	
	// plot slices
	
	c9->cd(4);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(0, 5);
	TH2F *xyt_HR1 = topology->Project3D("yx");
	xyt_HR1->SetStats(0);
	xyt_HR1->SetTitle("dt vs. counts - HR < 0.5");
	xyt_HR1->GetXaxis()->SetTitle("counts");
	xyt_HR1->GetXaxis()->CenterTitle();
	xyt_HR1->GetYaxis()->SetTitle("dt");
	xyt_HR1->GetYaxis()->CenterTitle();
	xyt_HR1->DrawClone("colZ");
	
	c9->cd(5);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(6, 10);
	TH2F *xyt_HR2 = topology->Project3D("yx");
	xyt_HR2->SetStats(0);
	xyt_HR2->SetTitle("dt vs. counts - 0.5 < HR < 1.");
	xyt_HR2->GetXaxis()->SetTitle("counts");
	xyt_HR2->GetXaxis()->CenterTitle();
	xyt_HR2->GetYaxis()->SetTitle("dt");
	xyt_HR2->GetYaxis()->CenterTitle();
	xyt_HR2->DrawClone("colZ");
	
	c9->cd(6);
	gPad->SetLogz(1);
	topology->GetZaxis()->SetRange(11, 50);
	TH2F *xyt_HR3 = topology->Project3D("yx");
	xyt_HR3->SetStats(0);
	xyt_HR3->SetTitle("dt vs. counts - HR > 1.");
	xyt_HR3->GetXaxis()->SetTitle("counts");
	xyt_HR3->GetXaxis()->CenterTitle();
	xyt_HR3->GetYaxis()->SetTitle("dt");
	xyt_HR3->GetYaxis()->CenterTitle();
	xyt_HR3->DrawClone("colZ");
	
	
	
	TCanvas *c10 = new TCanvas ("c10", "c10", 800, 600);
	//topology->Draw("BOX");
	c10->Divide(2,2);
	
	c10->cd(1);
	topology->GetZaxis()->SetRange(1, 50);
	topology->GetXaxis()->SetRange(1, 100);
	TH1F *x = topology->Project3D("x");
	//x->SetStats(0);
	x->SetTitle("counts");
	x->GetXaxis()->SetTitle("counts");
	x->GetXaxis()->CenterTitle();
	x->GetXaxis()->SetLabelSize(0.06);
	x->GetXaxis()->SetTitleSize(0.06);
	x->GetXaxis()->SetTitleOffset(0.8);
	x->GetYaxis()->SetTitle("n.");
	x->GetYaxis()->CenterTitle();
	x->GetYaxis()->SetLabelSize(0.06);
	x->GetYaxis()->SetTitleSize(0.06);
	x->GetYaxis()->SetTitleOffset(0.8);
	x->DrawClone();

	c10->cd(2);
	//topology->GetZaxis()->SetRange(6, 50);
	TH1F *y = topology->Project3D("y");
	//y->SetStats(0);
	y->SetTitle("duration");
	y->GetXaxis()->SetTitle("duration");
	y->GetXaxis()->CenterTitle();
	y->GetXaxis()->SetLabelSize(0.06);
	y->GetXaxis()->SetTitleSize(0.06);
	y->GetXaxis()->SetTitleOffset(0.8);
	y->GetYaxis()->SetTitle("n.");
	y->GetYaxis()->CenterTitle();
	y->GetYaxis()->SetLabelSize(0.06);
	y->GetYaxis()->SetTitleSize(0.06);
	y->GetYaxis()->SetTitleOffset(0.8);
	y->DrawClone();

	c10->cd(3);
	//topology->GetZaxis()->SetRange(6, 50);
	TH1F *z = topology->Project3D("z");
	//z->SetStats(0);
	z->SetTitle("HR");
	z->GetXaxis()->SetTitle("HR");
	z->GetXaxis()->CenterTitle();
	z->GetXaxis()->SetLabelSize(0.06);
	z->GetXaxis()->SetTitleSize(0.06);
	z->GetXaxis()->SetTitleOffset(0.8);
	z->GetYaxis()->SetTitle("n.");
	z->GetYaxis()->CenterTitle();
	z->GetYaxis()->SetLabelSize(0.06);
	z->GetYaxis()->SetTitleSize(0.06);
	z->GetYaxis()->SetTitleOffset(0.8);
	z->DrawClone();

	c10->cd(4);
	deltat->Draw();*/
}
