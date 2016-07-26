#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <fstream>
#include <sstream>
#include <math.h>

#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TTree.h>
#include <TText.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TH3.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TDatime.h>
#include <TTimeStamp.h>
#include <TMath.h>

int main(int argc, char **argv) {	

	// MM 03/07/09 modified from vsb_analysis_correlation.C
	// MM 29/06/09 modified to work in batch mode for the FF script
	
	// the executable is built this way (see root 5.14 manual @ACliC chapter, page 398:
	// root [1] .L vsb_analysis_correlation.C++       (creates vsb_analysis_correlation_C.so)
	// export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
	// g++ -o vsb_analysis_correlation vsb_analysis_correlation.C `root-config --cflags --libs` ./vsb_analysis_correlation_C.so -DSTANDALONE
	
	// MM 08/06/09 modified to analyze Apr09 - May09 (orbits 009992 - 010900)
	//             added local time plot;
	// MM 19/05/09 modified to analyze Fullekrug data for dataset 09-05-04-07 (contacts 10479 - 10523)
	// MM 25/03/09 modified to analyze Fullekrug data for dataset 09-03-20 (contacts 9787 - 9839)
	// MM 17/03/09 modified to analyze Fullekrug data for dataset 09-03-16 (contacts 9747 - 9780)
	// MM 12/03/09 modified to analyze Fullekrug data
	// MM 12/02/09 modified columns according to the new version of mcalanalizer
	// MM 01/12/08 include anche info sul background, messe fluence e bkg come float
	// MM 18/11/08 valuta la significativita' degli eventi su diversi tempi scala
	
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
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");
	
	int process_event_data = 1;

	struct Trigger  {
			int runid;
			int trg;
			double t0;
			double tstop;
			float ttot;
			char date[20];
			char time[20];
			float mint3908;	// minimum time obtained from 3908 packet data with mcalanalizer SW
			float maxt3908;	// maximum time obtained from 3908 packet data with mcalanalizer SW
			char type[5];
			int sa;
			int lng;
			int subms;
			int flg01ms;	//  1ms flag
			int flg16ms;	// 16ms flag
			int nrm;
			int rmid;
			int sitid;
			int ber;
			int bet;
			float lon;
			float lat;
			float h;
			float dt;
			int ntrg;	// number of reconstructed sub-ms triggers
			float mint;	// minimum time 
			float dt0;	// time difference of the found trigger with respect to the initial t0 (bstart)
			float tgrbini;
			float t90;
			float tbin;	// added since 15/10/08
			float cbkg;	// added since 15/10/08 
			float counts;
			float cpeak;
			float avgE;
			int nEbins;
			float c[4];
			float b[4];
			int x[15];
			int z[15];
			int cub[8];		// spatial zones (cuboni)
	};
	
	// ----------- read data and fill tree ---------------
	// VSB file: VSB_global_info_corr.dat   VSB_global_info_1ms-only.dat
	
	TTree tvsb;
	Trigger atrg;
	 // tvsb.ReadFile("/data1/mcal/data_analysis/VSB/analysis_09-03-16/VSB_global_info_10.dat",	 // dataset 16/3
	 tvsb.ReadFile(argv[1], "runid/I:trg/I:t0/D:tstop/D:ttot/F:date/C:time/C:mint3908/F:maxt3908/F:type/C:sa/I:lng/I:subms/I:flg01ms/I:flg16ms/I:nrm/I:rmid/I:sitid/I:ber/I:bet/I:lon/F:lat/F:h/F:dt/F:ntrg/I:mint/F:dt0/F:tgrbini/F:t90/F:tbin/F:cbkg/F:counts/F:cpeak/F:avgE/F:nEbins/I:c0/F:c1/F:c2/F:c3/F:b0/F:b1/F:b2/F:b3/F:x1/I:x2/I:x3/I:x4/I:x5/I:x6/I:x7/I:x8/I:x9/I:x10/I:x11/I:x12/I:x13/I:x14/I:x15/I:z1/I:z2/I:z3/I:z4/I:z5/I:z6/I:z7/I:z8/I:z9/I:z10/I:z11/I:z12/I:z13/I:z14/I:z15/I:cub1/I:cub2/I:cub3/I:cub4/I:cub5/I:cub6/I:cub7/I:cub8/I");
	
	
	tvsb.SetBranchAddress("runid", &atrg.runid);
	tvsb.SetBranchAddress("trg", &atrg.trg);
	tvsb.SetBranchAddress("t0", &atrg.t0);
	tvsb.SetBranchAddress("tstop", &atrg.tstop);
	tvsb.SetBranchAddress("ttot", &atrg.ttot);
	tvsb.SetBranchAddress("date", &atrg.date);
	tvsb.SetBranchAddress("time", &atrg.time);
	tvsb.SetBranchAddress("mint3908", &atrg.mint3908);
	tvsb.SetBranchAddress("maxt3908", &atrg.maxt3908);
	tvsb.SetBranchAddress("type", &atrg.type);
	tvsb.SetBranchAddress("sa", &atrg.sa);
	tvsb.SetBranchAddress("lng", &atrg.lng);
	tvsb.SetBranchAddress("subms", &atrg.subms);
	tvsb.SetBranchAddress("flg01ms", &atrg.flg01ms);
	tvsb.SetBranchAddress("flg16ms", &atrg.flg16ms);
	tvsb.SetBranchAddress("nrm", &atrg.nrm);
	tvsb.SetBranchAddress("rmid", &atrg.rmid);
	tvsb.SetBranchAddress("sitid", &atrg.sitid);
	tvsb.SetBranchAddress("ber", &atrg.ber);
	tvsb.SetBranchAddress("bet", &atrg.bet);
	tvsb.SetBranchAddress("lon", &atrg.lon);
	tvsb.SetBranchAddress("lat", &atrg.lat);
	tvsb.SetBranchAddress("h", &atrg.h);
	tvsb.SetBranchAddress("dt", &atrg.dt);
	tvsb.SetBranchAddress("ntrg", &atrg.ntrg);
	tvsb.SetBranchAddress("mint", &atrg.mint);
	tvsb.SetBranchAddress("dt0", &atrg.dt0);
	tvsb.SetBranchAddress("tgrbini", &atrg.tgrbini);
	tvsb.SetBranchAddress("t90", &atrg.t90);
	tvsb.SetBranchAddress("tbin", &atrg.tbin);
	tvsb.SetBranchAddress("cbkg", &atrg.cbkg);
	tvsb.SetBranchAddress("counts", &atrg.counts);
	tvsb.SetBranchAddress("cpeak", &atrg.cpeak);
	tvsb.SetBranchAddress("avgE", &atrg.avgE);
	tvsb.SetBranchAddress("nEbins", &atrg.nEbins);
	tvsb.SetBranchAddress("c0", &atrg.c[0]);
	tvsb.SetBranchAddress("c1", &atrg.c[1]);
	tvsb.SetBranchAddress("c2", &atrg.c[2]);
	tvsb.SetBranchAddress("c3", &atrg.c[3]);
	tvsb.SetBranchAddress("b0", &atrg.b[0]);
	tvsb.SetBranchAddress("b1", &atrg.b[1]);
	tvsb.SetBranchAddress("b2", &atrg.b[2]);
	tvsb.SetBranchAddress("b3", &atrg.b[3]);
	
	tvsb.SetBranchAddress("x1", &atrg.x[0]);
	tvsb.SetBranchAddress("x2", &atrg.x[1]);
	tvsb.SetBranchAddress("x3", &atrg.x[2]);
	tvsb.SetBranchAddress("x4", &atrg.x[3]);
	tvsb.SetBranchAddress("x5", &atrg.x[4]);
	tvsb.SetBranchAddress("x6", &atrg.x[5]);
	tvsb.SetBranchAddress("x7", &atrg.x[6]);
	tvsb.SetBranchAddress("x8", &atrg.x[7]);
	tvsb.SetBranchAddress("x9", &atrg.x[8]);
	tvsb.SetBranchAddress("x10", &atrg.x[9]);
	tvsb.SetBranchAddress("x11", &atrg.x[10]);
	tvsb.SetBranchAddress("x12", &atrg.x[11]);
	tvsb.SetBranchAddress("x13", &atrg.x[12]);
	tvsb.SetBranchAddress("x14", &atrg.x[13]);
	tvsb.SetBranchAddress("x15", &atrg.x[14]);
	tvsb.SetBranchAddress("z1", &atrg.z[0]);
	tvsb.SetBranchAddress("z2", &atrg.z[1]);
	tvsb.SetBranchAddress("z3", &atrg.z[2]);
	tvsb.SetBranchAddress("z4", &atrg.z[3]);
	tvsb.SetBranchAddress("z5", &atrg.z[4]);
	tvsb.SetBranchAddress("z6", &atrg.z[5]);
	tvsb.SetBranchAddress("z7", &atrg.z[6]);
	tvsb.SetBranchAddress("z8", &atrg.z[7]);
	tvsb.SetBranchAddress("z9", &atrg.z[8]);
	tvsb.SetBranchAddress("z10", &atrg.z[9]);
	tvsb.SetBranchAddress("z11", &atrg.z[10]);
	tvsb.SetBranchAddress("z12", &atrg.z[11]);
	tvsb.SetBranchAddress("z13", &atrg.z[12]);
	tvsb.SetBranchAddress("z14", &atrg.z[13]);
	tvsb.SetBranchAddress("z15", &atrg.z[14]);
	tvsb.SetBranchAddress("cub1", &atrg.cub[0]);
	tvsb.SetBranchAddress("cub2", &atrg.cub[1]);
	tvsb.SetBranchAddress("cub3", &atrg.cub[2]);
	tvsb.SetBranchAddress("cub4", &atrg.cub[3]);
	tvsb.SetBranchAddress("cub5", &atrg.cub[4]);
	tvsb.SetBranchAddress("cub6", &atrg.cub[5]);
	tvsb.SetBranchAddress("cub7", &atrg.cub[6]);
	tvsb.SetBranchAddress("cub8", &atrg.cub[7]);	

	int nentries = tvsb.GetEntries();
	cout << "Total number of entries = " << nentries << endl;
	
	
	
	// ----------- create histograms ---------------
	
	int lonbin = 12; // 36
	TH1F *hlontot = new TH1F("hlontot", "hlontot", lonbin, -180., 180.);
	TH1F *hlon = new TH1F("hlon", "hlon", lonbin, -180., 180.);
	TH1F *hlonBUS = new TH1F("hlonBUS", "hlonBUS", lonbin, -180., 180.);
	
	int lcbin = 12; // 36
	TH1F *hlc  = new TH1F("hlc", "hlc", lcbin, 0., 24.);
	
	int dtbin = 25;	//50
	TH1F *hdttot  = new TH1F("hdttot", "hdttot", dtbin, 0., 6000.);
	TH1F *hdt  = new TH1F("hdt", "hdt", dtbin, 0., 6000.);
	TH1F *hdtBUS  = new TH1F("hdtBUS", "hdtBUS", dtbin, 0., 6000.);
	
	TH2F *hdtlon = new TH2F("hdtlon", "hdtlon", 12, -180., 180., 12, 0., 24.);	//12,8
	TH3F *hdtlonlat = new TH3F("hdtlonlat", "hdtlonlat", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH3F *hdtlonlatBUS = new TH3F("hdtlonlatBUS", "hdtlonlatBUS", 12, -3., 3., 12, -180., 180., 8, 0., 6000.);	//12,8
	TH1F *hbars  = new TH1F("hbars", "hbars", 30, -0.5, 29.5);
	// TH3F *hbarfluhr = new TH3F("hbarfluhr", "hbarfluhr", 30, -0.5, 29.5, 
	
	// c0+c1 , c2+c3 , nbars
	TH3F *topology = new TH3F("topology", "topology", 100, 0., 100.,  100, 0., 100., 30, 0.5, 30.5);
	TH3F *topologyBUS = new TH3F("topologyBUS", "topologyBUS", 100, 0., 100.,  100, 0., 100., 30, 0.5, 30.5);
	
	TH1F *ht90 = new TH1F("ht90", "ht90", 2046, 0., 1.024);
	TH1F *hcpeak = new TH1F("hcpeak", "hcpeak", 100, 0, 100);
	TH1F *hcounts = new TH1F("hcounts", "hcounts", 1000, 0, 1000);
	TH1F *havgE = new TH1F("havgE", "havgE", 200, 0., 100.);
	TH1F *hhr = new TH1F("hhr", "hhr", 1000, 0., 10.);
	
	TH1F *ht90BUS = new TH1F("ht90BUS", "ht90BUS", 2046, 0., 1.024);
	TH1F *hcpeakBUS = new TH1F("hcpeakBUS", "hcpeakBUS", 100, 0, 100);
	TH1F *hcountsBUS = new TH1F("hcountsBUS", "hcountsBUS", 1000, 0, 1000);
	TH1F *havgEBUS = new TH1F("havgEBUS", "havgEBUS", 200, 0., 100.);
	TH1F *hbarsBUS  = new TH1F("hbarsBUS", "hbarsBUS", 30, -0.5, 29.5);
	TH1F *hhrBUS = new TH1F("hhrBUS", "hhrBUS", 1000, 0., 10.);
	
	TH1F *hmint = new TH1F("hmint", "hmint", 10000, -50., 50.);
	TH1F *htgrbini = new TH1F("htgrbini", "htgrbini", 10000, -50., 50.);
	
	TH1F *hsig = new TH1F("hsig", "hsig", 100, 0., 50.);
	TH1F *hsigBUS = new TH1F("hsigBUS", "hsigBUS", 100, 0., 50.);
	
	TGraph *gGRB = new TGraph(1);
	TGraph *gBUS = new TGraph(1);
	
	TGraph *gcntbarGRB = new TGraph(1);
	TGraph *gcntbarBUS = new TGraph(1);
	
	// ----------- process entries & fill histograms ---------------
	
	float hr, hr1;
	int ncGRB=0;
	int ncBUS=0;
	int n1ms=0;
	int hh, mm, ss;
	float lc;
	double t0prev = 0.;
	
	FILE *fptr;
	fptr = fopen(argv[2], "w");
	
	for (int i=0; i<nentries; i++) {
		tvsb.GetEntry(i);
		if(atrg.lon>180.) atrg.lon = atrg.lon-360.;	// longitude correction
		
		if (atrg.t0 != t0prev  ) {
		
			hmint->Fill(atrg.dt0+atrg.mint);
			// for debugging the "missing pre-burst" issue		
			// if (atrg.dt0+atrg.mint > -20.) printf("%06d  %3d  %20.6f  %10.6f\n", atrg.runid, atrg.trg, atrg.t0, atrg.dt0+atrg.mint);
			
			hlontot->Fill(atrg.lon);
			hdttot->Fill(atrg.dt);					
			htgrbini->Fill(atrg.tgrbini);
				
			hr = (atrg.c[1] + atrg.c[0])>0 ? ((float) (atrg.c[2] + atrg.c[3])) / (atrg.c[1] + atrg.c[0]) : 100.;
			// hr = (atrg.b[1] + atrg.b[0])>0 ? ((float) (atrg.b[2] + atrg.b[3])) / (atrg.b[1] + atrg.b[0]) : 0.;
				
			// calculate numbre of triggered bars
			int nbars = 0;
			for (int k=0; k<15; k++) if (atrg.x[k]>0) nbars++;
			for (int k=0; k<15; k++) if (atrg.z[k]>0) nbars++;
			
			if (atrg.counts>9 && hr>=0.5 ) { // && atrg.counts>9 && hr>0.2 
														
				// is it flagged as a GRB?
				if (atrg.type[0]=='G' ) {
					ht90->Fill(atrg.t90);
					hcpeak->Fill(atrg.cpeak + atrg.cbkg);
					hcounts->Fill(atrg.counts + atrg.cbkg);
					havgE->Fill(atrg.avgE);
					hsig->Fill(atrg.cbkg>0 ? atrg.cpeak/sqrt(atrg.cbkg) : -1.);
					hlon->Fill(atrg.lon);
					hdt->Fill(atrg.dt);	
					hhr->Fill(hr);
					
					if (atrg.flg01ms==1) n1ms++;
			
					// calculate local time
					sscanf(atrg.time, "%2d:%2d:%2d", &hh, &mm, &ss);
					lc = fmod(hh + mm/60. + ss/3600. + atrg.lon*0.06667, 24.);
					hlc->Fill(lc);
					
					hdtlonlat->Fill(atrg.lat, atrg.lon, atrg.dt);
					hdtlon->Fill(atrg.lon, lc);		
					
					for (int k=0; k<15; k++) if (atrg.x[k]>0) hbars->Fill(k, atrg.x[k]);
					for (int k=0; k<15; k++) if (atrg.z[k]>0) hbars->Fill(k+15, atrg.z[k]);
					
					gGRB->SetPoint(ncGRB, ncGRB, atrg.runid);
					// gcntbarGRB->SetPoint(ncGRB, nbars, atrg.counts);
					// gcntbarGRB->SetPoint(ncGRB, atrg.c1 + atrg.c2 + atrg.c3, atrg.counts);
					// gcntbarGRB->SetPoint(ncGRB, nbars, atrg.c[2] + atrg.c[3]);
					gcntbarGRB->SetPoint(ncGRB, nbars, atrg.counts);
					// gcntbarGRB->SetPoint(ncGRB, atrg.c[0] + atrg.c[1], atrg.c[2] + atrg.c[3]);
					topology->Fill(atrg.c[0] + atrg.c[1], atrg.c[2] + atrg.c[3], nbars);
					ncGRB++;
					
					// stampa in formato giusto da dare in pasto a makespectra
					// if (1) cout << atrg.runid << "  " << atrg.trg << "  " << atrg.counts + atrg.cbkg << "  " << atrg.cpeak << "  " << atrg.subms << "  " << hr << "  " << atrg.lon << "  " << atrg.lat << "  " << atrg.dt << "   " << atrg.t0 << "  " << atrg.t90 << "  " << atrg.dt0 << "  " << atrg.tgrbini << "  " << endl;
					
 					
					// format for sferics correlation
					double peakdt = (fmod(atrg.t0,1.) + atrg.tgrbini);
					if (1 && peakdt>=0. && peakdt<1.) {
					// if (peakdt<0. || peakdt>=1.) {
						// printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f \t%10.0f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (peakdt*1.E6), atrg.lon, atrg.lat, atrg.cpeak);
											
						// format for checking with Fullekrug results
						if (1) fprintf(fptr, "%s \t%s \t%6d \t%3d \t%6.2f \t%6.2f \t%3d \t%3d \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%20.6f \t%8.6f\n", atrg.date, atrg.time, atrg.runid, atrg.trg, atrg.counts + atrg.cbkg, atrg.cpeak, atrg.subms, atrg.flg01ms, atrg.flg16ms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t0 + atrg.tgrbini, atrg.t90);

						
						// aggiungo l'HR per Steve Cummer e il t0 per la correlazione
						if (0) fprintf(fptr, "%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f \t%10.0f \t%10.0f \t%10.2f \t%20.6f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (peakdt*1.E6), atrg.lon, atrg.lat, atrg.cpeak, atrg.counts, hr, atrg.t0 + atrg.tgrbini);
											
						// aggiungo il t0, serve per la correlazione
						// printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f \t%10.0f \t%20.6f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (peakdt*1.E6), atrg.lon, atrg.lat, atrg.cpeak, atrg.t0 + atrg.tgrbini);
						
					}
					if (peakdt<0. && peakdt>=1.) cout << "*** " << atrg.runid << "-" << atrg.trg << " peakdt (us) = " << peakdt*1.E6 << endl;
					
					// format for makespectra
					if (0) printf("%6d \t%3d \t%6.2f \t%6.2f \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%20.6f \t%8.6f \t%8.6f \t%8.6f\n", atrg.runid, atrg.trg, atrg.counts + atrg.cbkg, atrg.cpeak, atrg.subms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t0, atrg.t90, atrg.dt0, atrg.tgrbini);
					
					// format for sferics correlation
					if (0) printf("%6d \t%d \t%s \t%s.%06d \t%10.2f \t%10.2f \t%6.0f\n",atrg.runid, atrg.trg, atrg.date, atrg.time, (int) (fmod(atrg.t0,1.)*1.E6), atrg.lon, atrg.lat, atrg.counts + atrg.cbkg);
					
					// format for checking with Fullekrug results
					if (0) printf("%s \t%s \t%6d \t%3d \t%6.2f \t%6.2f \t%3d \t%3d \t%3d \t%6.3f  \t%8.2f \t%6.2f \t%6.1f \t%20.6f \t%8.6f\n", atrg.date, atrg.time, atrg.runid, atrg.trg, atrg.counts + atrg.cbkg, atrg.cpeak, atrg.subms, atrg.flg01ms, atrg.flg16ms, hr, atrg.lon, atrg.lat, atrg.dt, atrg.t0, atrg.t90);
				}
			}
		}
		t0prev = atrg.t0;
	}
	fclose(fptr);
	
	// normalize histo
	
	int nGRB = ht90->GetEntries();
	cout << "Number of selected GRB events: " << nGRB << " - with 1ms flag set: " << n1ms << endl;
	ht90->Scale(1./nGRB);
	hcpeak->Scale(1./nGRB);
	hcounts->Scale(1./nGRB);
	havgE->Scale(1./nGRB);
	hbars->Scale(1./nGRB);
	hsig->Scale(1./nGRB);
	hhr->Scale(1./nGRB);
	hlon->Sumw2();
	hlon->Scale(1./nGRB);
	hlc->Sumw2();
	hlc->Scale(1./nGRB);
	
	int nBUS = ht90BUS->GetEntries();
	cout << "Number of selected BUS events: " << nBUS << endl;

}