#include "mcalgrid.h"
#include "version.h"
using namespace std;

int main(int argc, char *argv[])
{
	bool batch=FALSE;
	
	int nrow = 0;
	int ngood = 0;
	int status = 0;
	int nevents, minpkt, maxpkt, ipl, iph, fpl, fph;
	float maxscat, thetamin, thetamax, phimin, phimax, dxmax, dzmax;
	float ipxmin, ipxmax, ipzmin, ipzmax, fpxmin, fpxmax, fpzmin, fpzmax, mcaleth;
	char line[100];
//	FILE *fp=NULL;
	ifstream fp;
	float *data;
	
	TH1F *hScang, *hRX, *hRZ, *hTheta, *hPhi, *hNseg, *hIniPlane, *hFinPlane;
	TH1F *hScangS, *hRXS, *hRZS, *hThetaS, *hPhiS, *hNsegS, *hIniPlaneS, *hFinPlaneS, *hMCALDevX, *hMCALDevZ;
	TH1F *hMCALEnergyX, *hMCALEnergyZ, *hPosDevX, *hPosDevZ;
	TH2F *hPosClIni, *hPosClEnd;
	TH2F *hPosClIniS, *hPosClEndS, *hPosDevS, *hPosDevMCAL, *hBarDevX, *hBarDevZ, *hBarEX, *hBarEZ; 
	TGraph *garr[30];
	int narr[30];
	for (int i=0; i<30; i++) narr[i] = 0;
	
//	TCanvas *c1, *c2;
	TStyle *mcalstyle;
	TString   calFile, track3Dfile, lv1file, rootfile, outfile, conffile;
	TEnv *conf;
	
	MCALevt inevt;		//!< Input event
	
	int nsel = 0, nmu = 0, nini = 0, jevt=0, s1 = 0, s2 = 0;
	int pkt, event, nbars, ix, iz, id, barx, barz;
	int n0X=0, n0Z=0, n0XZ=0;
	double xe, ze, dist, aux, dx, dz;
	double xm, zm, mdist, dxm, dzm, xmcal, zmcal;
	double cfact = M_PI/180.;	// conversion factor from deg to steradians
	bool px, pz;

	cout << "mcalgrid version " << VERSION << endl;
	
	TObjArray *hc = NULL;
	   
	// cout help on command syntax
	if (argc == 1) {
		cout << "command options:" << endl;
		cout << "-batch : work in batch mode" << endl;
		cout << "-c <filename> : use this configuration file" << endl;
		cout << "-r <filename> : load this ROOT event file (overwrites configuration file)" << endl;
		cout << "-t <filename> : load this 3DTrack file (overwrites configuration file" << endl;
		cout << "-l <filename> : load this lv1 file (overwrites configuration file" << endl;
		cout << "-f <filename> : write histo to file (append mode)" << endl;
		return 0;
	}
	
	// ------------- parse command line data -------------------
	int iopt=1;
	while (iopt < argc) {
		TString cmd(argv[iopt]);
		
		// work in batch mode?
		if (cmd.Contains("-batch", TString::kExact)) batch = TRUE;
		
		// input ROOT file
		if (cmd.Contains("-r", TString::kExact)) rootfile += argv[++iopt];
		
		// track 3D file
		if (cmd.Contains("-t", TString::kExact)) track3Dfile += argv[++iopt];
		
		// lv1 fits file
		if (cmd.Contains("-l", TString::kExact)) lv1file += argv[++iopt];
		
		// output ROOT file
		if (cmd.Contains("-f", TString::kExact)) outfile += argv[++iopt];
		
		// configuration file
		if (cmd.Contains("-c", TString::kExact)) conffile += argv[++iopt];
		
		iopt++;
	}	
	
	if (conffile.Length() > 0) {
		// load configuration file
		cout << "Loading configuration file " << conffile.Data() << endl;
		conf = new TEnv(conffile.Data());
	} else {
		// load default configuration file
		calFile.Append(getenv("MCALSW"));
		calFile += MCALGRID_CONF;
		cout << "Loading default configuration file " << calFile.Data() << endl;
		conf = new TEnv(calFile.Data());
	}
	if (!conf) {
		cout << "*** cannot find standard configuration file " << calFile.Data() << endl;
		return 1;
	}
	
	// load data files (if not specified by command line arguments)
	if (track3Dfile.Length() == 0) track3Dfile.Append(conf->GetValue(".track3D.file.", "3D.txt"));
	if (lv1file.Length() == 0) lv1file.Append(conf->GetValue(".lv1.file.", "lv1.txt"));
	if (rootfile.Length() == 0) rootfile.Append(conf->GetValue(".root.file.", "root.txt"));
	
	// load selection criteria
	minpkt 	= conf->GetValue(".initial.packet.", 0);
	maxpkt 	= conf->GetValue(".final.packet.", 0);
	maxscat = conf->GetValue(".max.scattering.angle.", 0);
	ipl 	= conf->GetValue(".first.plane.min.", 0);
	iph 	= conf->GetValue(".first.plane.max.", 0);
	fpl 	= conf->GetValue(".last.plane.min.", 0);
	fph 	= conf->GetValue(".last.plane.max.", 0);
	thetamin= conf->GetValue(".theta.min.", 0.);
	thetamax= conf->GetValue(".theta.max.", 90.);
	phimin= conf->GetValue(".phi.min.", 0.);
	phimax= conf->GetValue(".phi.max.", 360.);
	dxmax= conf->GetValue(".deltax.max.", 0.);
	dzmax= conf->GetValue(".deltaz.max.", 0.);
	ipxmin= conf->GetValue(".first.plane.X.min.", -100.);
	ipxmax= conf->GetValue(".first.plane.X.max.", 0.);
	ipzmin= conf->GetValue(".first.plane.Z.min.", -100.);
	ipzmax= conf->GetValue(".first.plane.Z.max.", 0.);
	fpxmin= conf->GetValue(".last.plane.X.min.", -100.);
	fpxmax= conf->GetValue(".last.plane.X.max.", 0.);
	fpzmin= conf->GetValue(".last.plane.Z.min.", -100.);
	fpzmax= conf->GetValue(".last.plane.Z.max.", 0.);
	mcaleth= conf->GetValue(".mcal.energy.thr.", 0.);	
	
	TFile *fh = NULL;
	if (outfile.Length() > 0) {
		cout << "Create/update output file " << outfile.Data() << endl;
		fh = new TFile(outfile.Data(), "update");
		TIter next = fh->GetListOfKeys();
		TKey *key;
		TObjArray *hc1;
		while ((key = (TKey *)next())) {
			if (strncmp(key->GetName(),"hc", 8) == 0) hc1 = (TObjArray *) fh->Get(key->GetName());
			hc = (TObjArray *) hc1->Clone();
		}
		fh->Close();
	}
	
	if (!hc) {
		cout << "Create new histograms " << endl;
		// no previous collection found in outfile: init histogram collection
		hc = new TObjArray(200,0);
		initGridHisto(hc);
		initMcalHisto(hc);
		initCalibrationHisto(hc);
	}
	
	// retrieve histograms
	hPosClIni = (TH2F *) hc->FindObject("hPosClIni");
	hPosClEnd = (TH2F *) hc->FindObject("hPosClEnd");
	hPosClIniS = (TH2F *) hc->FindObject("hPosClIniS");
	hPosClEndS = (TH2F *) hc->FindObject("hPosClEndS");
	hPosDevS = (TH2F *) hc->FindObject("hPosDevS");
	hPosDevMCAL = (TH2F *) hc->FindObject("hPosDevMCAL");
	hBarDevX = (TH2F *) hc->FindObject("hBarDevX");
	hBarDevZ = (TH2F *) hc->FindObject("hBarDevZ");
	
	hScang = (TH1F *) hc->FindObject("hScang");
	hRX = (TH1F *) hc->FindObject("hRX");
	hRZ = (TH1F *) hc->FindObject("hRZ");
	hTheta = (TH1F *) hc->FindObject("hTheta");
	hPhi = (TH1F *) hc->FindObject("hPhi");
	hNseg = (TH1F *) hc->FindObject("hNseg");
	hIniPlane = (TH1F *) hc->FindObject("hIniPlane");
	hFinPlane = (TH1F *) hc->FindObject("hFinPlane");
	hScangS = (TH1F *) hc->FindObject("hScangS");
	hRXS = (TH1F *) hc->FindObject("hRXS");
	hRZS = (TH1F *) hc->FindObject("hRZS");
	hThetaS = (TH1F *) hc->FindObject("hThetaS");
	hPhiS = (TH1F *) hc->FindObject("hPhiS");
	hNsegS = (TH1F *) hc->FindObject("hNsegS");
	hIniPlaneS = (TH1F *) hc->FindObject("hIniPlaneS");
	hFinPlaneS = (TH1F *) hc->FindObject("hFinPlaneS");
	hMCALDevX = (TH1F *) hc->FindObject("hMCALDevX");
	hMCALDevZ = (TH1F *) hc->FindObject("hMCALDevZ");
	hMCALEnergyX = (TH1F *) hc->FindObject("hMCALEnergyX");
	hMCALEnergyZ = (TH1F *) hc->FindObject("hMCALEnergyZ");
	hPosDevX = (TH1F *) hc->FindObject("hPosDevX");
	hPosDevZ = (TH1F *) hc->FindObject("hPosDevZ");
	hBarEX = (TH2F *) hc->FindObject("hBarEX");
	hBarEZ = (TH2F *) hc->FindObject("hBarEZ");
	for (int i=0; i<30; i++) {
		TString gname("gDev_");
		gname += i;
		garr[i] = (TGraph *) hc->FindObject(gname.Data());
	}
	
	// load data and fill histograms
	cout << "Reading track file " << track3Dfile.Data() << endl;
	// get number of entries
	fp.open(track3Dfile.Data());
	
	//char str[1001];
	string str;
	while ( !fp.eof() ) {
		getline(fp, str);
		//cout << str << endl;
		nrow++;
	}
	//nrow = 9024;
	cout << "Number of rows = " << nrow << endl;
		
	fp.close();
	fp.clear();	// <-- FONDAMENTALE! altrimenti non resetta lo status EOF del file
	fp.open(track3Dfile.Data());
	int nsmax=0;
	for (int i=0; i<nrow; i++) {
		getline(fp, str);
		int len = str.length();
		int ns=0, si=0, se=0;
		do {
			while (str[se] != '\t'&& se < len) se++;
			ns++;
			si = ++se;
		} while (se<len);
		if (ns > nsmax) nsmax = ns;
	}
	cout << "max number of elements / row (current value set to 30): " << nsmax << endl;
	fp.close();
	fp.clear();	// <-- FONDAMENTALE! altrimenti non resetta lo status EOF del file
	fp.open(track3Dfile.Data());
		
	// allocate dynamic memory
	data = new float[NCOL*nrow];
	
	// finally read data!
	istringstream instream;
	for (int i=0; i<nrow; i++) {
		getline(fp, str);
		instream.clear();     // Reset from possible previous errors.
 		instream.str(str);      // Use s as source of input.		
	//	cout << endl << i << "   " << str.length();
		int len = str.length();
		int ns=0;
		
	//	instream >> data[i*NCOL + 0] >> data[i*NCOL + 1];
	//	cout  << "   " << data[i*NCOL + 0] << "   " << data[i*NCOL + 1] << "   "  << instream.eof() << endl;
		
		while (!instream.eof() && ns<30) {
			instream >> data[i*NCOL + ns];
	//		cout << ns << "  " << instream.eof();
			ns++;
		} 

		pkt   = (int) data[i*NCOL+1];
		if (pkt<minpkt) continue;
		else if (pkt>maxpkt) break;
		ngood++;
		if (data[i*NCOL+3] > 0.) {
			hScang->Fill(data[i*NCOL+3]/data[i*NCOL+9]);
			hRX->Fill(data[i*NCOL+4]);
			hRZ->Fill(data[i*NCOL+5]);
			hTheta->Fill(data[i*NCOL+6]);
			hPhi->Fill(data[i*NCOL+7]);
			hNseg->Fill(data[i*NCOL+9]);
			hIniPlane->Fill(data[i*NCOL+10]);
			hFinPlane->Fill(data[i*NCOL+14]);
	//		hPosClIni->Fill(data[i*NCOL+11], data[i*NCOL+13]);
	//		hPosClEnd->Fill(data[i*NCOL+15], data[i*NCOL+17]);
		}
	//	cout << i << "  " << data[i*NCOL+3] << "  " << data[i*NCOL+6] << "  " << data[i*NCOL+7] << endl;
	
	}
	fp.close();
	
/*	cout << endl;
	for (int j=0; j<20; j++) {
		for (int k=0; k<30; k++) {
			cout << data[j*NCOL+k] << "  ";
		}
		cout << endl;
	}*/
	
	// open ROOT file
	TFile *fd;
	TTree *indata = NULL;
	MCVFileInfo *info = NULL;
	cout << "Reading ROOT file " << rootfile.Data() << endl;
	fd = new TFile(rootfile.Data());
	TIter next0 = fd->GetListOfKeys();
	TKey *key;
	while ((key = (TKey *)next0())) {
		if (strcmp(key->GetClassName(),"TTree") == 0) indata = (TTree *) fd->Get(key->GetName());
		if (strcmp(key->GetClassName(),"MCVFileInfo") == 0) info = (MCVFileInfo *) fd->Get(key->GetName());
	}
	nevents = info->nevents;
	// connect input tree
	indata->SetBranchAddress("time", &inevt.time);
	indata->SetBranchAddress("mult", &inevt.mult);
	indata->SetBranchAddress("bar", inevt.bar);
	indata->SetBranchAddress("pdA", inevt.pdA);
	indata->SetBranchAddress("pdB", inevt.pdB);
	indata->SetBranchAddress("pos", inevt.pos);
	indata->SetBranchAddress("E", inevt.E);
	indata->SetBranchAddress("Etot", &inevt.Etot);
	
	// open fits file
	fitsfile *fptr;		// fits file pointer
	unsigned long *pktnumb;
	Short_t *evtnumb;
	double *timearray;
	pktnumb = new unsigned long[nevents];
	evtnumb = new Short_t[nevents];
	timearray = new double[nevents];
	cout << "Reading fits file " << lv1file.Data() << endl;
	if ( fits_open_file(&fptr, lv1file.Data(), READONLY, &status) )  printerror( status );
	if ( fits_movnam_hdu(fptr, ANY_HDU, "EVENTS", 0, &status) )  printerror( status );
	
	if (fits_read_col(fptr, TINT32BIT, 1, 1, 1, nevents, 0, pktnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TSHORT, 2, 1, 1, nevents, 0, evtnumb, NULL, &status)) printerror(status);
	if (fits_read_col(fptr, TDOUBLE, 51, 1, 1, nevents, 0, timearray, NULL, &status)) printerror(status);
	double t0 = timearray[0];

	// loop on data
	for (int i=0; i< nrow; i++) {
		pkt   = (int) data[i*NCOL+1];
		if (pkt<minpkt) continue;
		else if (pkt>maxpkt) break;
		
		if (data[i*NCOL+3]/data[i*NCOL+9] <= maxscat) s1++;	
		else continue;
		if (data[i*NCOL+10] >= ipl && data[i*NCOL+10] <= iph && data[i*NCOL+14] >= fpl && data[i*NCOL+14] <= fph) s2++;
		else continue;
		hPosClIni->Fill(data[i*NCOL+11], data[i*NCOL+13]);
		hPosClEnd->Fill(data[i*NCOL+15], data[i*NCOL+17]);
		if (phimin>=0.) {
			if (data[i*NCOL+7] < phimin || data[i*NCOL+7] > phimax) continue;
		} else {
			if (data[i*NCOL+7] < phimin+360 && data[i*NCOL+7] > phimax) continue;
		}
		if (data[i*NCOL+6] >= thetamin && data[i*NCOL+6] <= thetamax) nini++;
		else continue;
		if (data[i*NCOL+11]<ipxmin || data[i*NCOL+11]>ipxmax || data[i*NCOL+13]<ipzmin || data[i*NCOL+13]>ipzmax || data[i*NCOL+15]<fpxmin || data[i*NCOL+15]>fpxmax || data[i*NCOL+17]<fpzmin || data[i*NCOL+17]>fpzmax) continue;
			
			// calculate expected position on the last plane
			dist = (data[i*NCOL+12] - data[i*NCOL+16])*tan(cfact*data[i*NCOL+6]);  // SBAGLIATO sin
			aux = cfact*data[i*NCOL+7];
//			xe = data[i*NCOL+11] - dist*cos(aux);	// versione originale
//			ze = data[i*NCOL+13] + dist*sin(aux);
			xe = data[i*NCOL+11] + dist*cos(aux);
			ze = data[i*NCOL+13] - dist*sin(aux);   // - 
			dx = xe - data[i*NCOL+15];
			dz = ze - data[i*NCOL+17];
			hPosDevS->Fill(dx, dz);
			hPosDevX->Fill(dx);
			hPosDevZ->Fill(dz);
//			hPhiS->Fill(data[i*NCOL+7]);
//			if (i<20) printf("%d \t%d \t%d \t%6.2f \t%6.2f \n", i, pkt, (int) data[i*NCOL+2], xe, ze);
			
			// selection on deviation from reconstructed track and last plane position
			if (fabs(dx) <= dxmax && fabs(dz) <= dzmax) {	//0.2
				nmu++;
				
				// find out mcal data
				event = (int) data[i*NCOL+2];
				while ((pktnumb[jevt] != pkt || evtnumb[jevt] != event) && jevt < nevents) jevt++;
				if (jevt == nevents) {
					cout << "*** WARNING: cannot find pkt " << pkt << "  event " << event << " in fits file!" << endl;
					jevt=0;
					continue;
				}
				indata->GetEntry(jevt);
				
				// find out how many bars with more than some MeV
				nbars=0;
				px = FALSE;
				pz = FALSE;
				for (int k=0; k<30; k++) { // inevt.mult
					id = inevt.bar[k];
					if (id>=0 && inevt.E[id] > mcaleth) {
						nbars++;
						if (k>=15) {	
							pz = TRUE;
							iz = id;
							barz = k;
						} else {
							px = TRUE;
							ix = id;
							barx = k;
						}
					}
				}
//				printf("%d \t%d \t%d \t%d \t%6.2f \t%d \t%s\n", nsel, pkt, event, jevt, inevt.Etot, nbars, nbars==2 && px && pz ? "GOOD" : "");
				
				// last selection: only 2 bars, one per plane
				if (!px && pz) {
					n0X++;
					hPosClIniS->Fill(data[i*NCOL+11], data[i*NCOL+13]);
					hPosClEndS->Fill(data[i*NCOL+15], data[i*NCOL+17]);
				}
				if (px && !pz) n0Z++;
				if (!px && !pz) n0XZ++;
				
				if (nbars==2 && px && pz) {
					nsel++;
					
					// fill selected histograms
					hScangS->Fill(data[i*NCOL+3]/data[i*NCOL+9]);
					hRXS->Fill(data[i*NCOL+4]);
					hRZS->Fill(data[i*NCOL+5]);
					hThetaS->Fill(data[i*NCOL+6]);
					hPhiS->Fill(data[i*NCOL+7]);
					hNsegS->Fill(data[i*NCOL+9]);
					//hPosClIniS->Fill(data[i*NCOL+11], data[i*NCOL+13]);
					//hPosClEndS->Fill(data[i*NCOL+15], data[i*NCOL+17]);
					hIniPlaneS->Fill(data[i*NCOL+10]);
					hFinPlaneS->Fill(data[i*NCOL+14]);
					
					// project the track on MCAL plane X and Z
					dist = tan(cfact*data[i*NCOL+6]);
					aux = cfact*data[i*NCOL+7];
					
					// proietta a partire dal primo cluster
					xm = data[i*NCOL+11] - (Y_PLANEX - data[i*NCOL+12])*dist*cos(aux);
					zm = data[i*NCOL+13] + (Y_PLANEZ - data[i*NCOL+12])*dist*sin(aux); 

					// oppure: proietta a partire dall'ultimo cluster
//					xm = data[i*NCOL+15] - (Y_PLANEX - data[i*NCOL+16])*dist*cos(aux);
//					zm = data[i*NCOL+17] + (Y_PLANEZ - data[i*NCOL+16])*dist*sin(aux); 
					xmcal = XREF - inevt.pos[ix];	// X coordinate of the reference frame respect to center
					zmcal = ZREF - inevt.pos[iz];	// Z coordinate of the reference frame respect to center
					dxm = xmcal - xm;
					dzm = zmcal - zm;
					hPosDevMCAL->Fill(dxm, dzm);
					hMCALDevX->Fill(dxm);
					hMCALDevZ->Fill(dzm);
					hMCALEnergyX->Fill(inevt.E[ix]*cos(data[i*NCOL+6]*M_PI/180.));	// normalized to path
					hMCALEnergyZ->Fill(inevt.E[iz]*cos(data[i*NCOL+6]*M_PI/180.));	// normalized to path
					
					hBarEX->Fill(barx, inevt.E[ix]*cos(data[i*NCOL+6]*M_PI/180.));	// normalized to path
					hBarEZ->Fill(barz-15, inevt.E[iz]*cos(data[i*NCOL+6]*M_PI/180.));// normalized to path
					
					hBarDevX->Fill(barx, dxm);
					hBarDevZ->Fill(barz-15, dzm);
					
					//garr[barx]->SetPoint(++(narr[barx]), xm, dxm);
					//garr[barz]->SetPoint(++(narr[barz]), zm, dzm);
					garr[barx]->SetPoint(++(narr[barx]), XREF - xm, log((inevt.pdB[ix]-info->oB[barx])/(inevt.pdA[ix]-info->oA[barx])));
					garr[barz]->SetPoint(++(narr[barz]), ZREF - zm, -log((inevt.pdB[iz]-info->oB[barz])/(inevt.pdA[iz]-info->oA[barz])));
					
			//		printf("%d \t%d \t%d \t%d \t%6.2f | \t%6.2f \t%6.2f \t%6.2f \t%6.2f \t%6.2f \t%6.2f | \t%6.2f \t%6.2f\n", nsel, pkt, event, jevt, inevt.Etot, xe, ze, xm, zm, xmcal, zmcal, inevt.E[ix], inevt.E[iz]);
				}
			}
//		}
	
	}
	cout << "Events fulfilling ST requirement 1 (scattering angle)= " << s1 << "(" << 100.*((float) s1)/ngood << "%)" << endl;
	cout << "Events fulfilling also ST requirement 2 (initial and final planes)= " << s2 << "(" << 100.*((float) s2)/ngood << "%)" << endl;
	cout << "Events fulfilling also ST requirement 3 (theta)= " << nini << "(" << 100.*((float) nini)/ngood << "%)" << endl;
	cout << "Total number of events fulfilling also ST position requirements = " << nmu << "(" << 100.*((float) nmu)/ngood << "%)" << endl;
	cout << "Total number of events fulfilling also MCAL requirements = " << nsel << "(" << 100.*((float) nsel)/ngood << "%)" << endl;
	cout << "Ratio #MCAL events / #ST events = " << 100.*((float) nsel)/nmu << " %" << endl;
	cout << "No bars on plane X: " << 100.*n0X/((float) nsel) << " %" << endl;
	cout << "No bars on plane Z: " << 100.*n0Z/((float) nsel) << " %" << endl;
	cout << "No bars on both planes: " << 100.*n0XZ/((float) nsel) << " %" << endl;
	
	// output
	
	// save data to output file, if required
	if (outfile.Length() > 0) {
		fh = new TFile(outfile.Data(), "update");
		hc->Write("hc", TObject::kSingleKey);
		fh->Close();
	}
	
	// set graphics environment
	TApplication theApp("App", &argc, argv);
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
	gROOT->SetStyle("mcalstyle");
	gROOT->ForceStyle();
	
	drawGridHisto(hc);
	drawMcalHisto(hc);
	TString polFile;
	polFile += conf->GetValue(".burst.polinomial.functions.","pol.root");
	cout << "retrieving polinomial calibration functions from file " << polFile.Data() << endl;
	TFile fppol(polFile.Data());
	TObjArray *fcBonA = (TObjArray *) fppol.Get("fcBonA");
	drawCalibrationHisto(hc, fcBonA);
	fppol.Close();
	
	theApp.Run();
	delete data;
	delete pktnumb;
	delete timearray;
	return 0;
}

//
//--------------------------------------------------------------------
//

void printerror( int status)
{   // Print out cfitsio error messages and exit program
	if (status)  {
		fits_report_error(stderr, status);  // print error report
		exit( status );                     // terminate the program, returning error status
	}
	return;
}
