{
	// MM 08/06/10 use 96 energy bin (finer detail at high energy)
	// MM 26/05/10 creates spectra from calo.dat file (MonteCarlo simulation results)
	
	gROOT->Reset();
	
	// MCAL energy channels boundaries (MeV) from file ~/local/mcalsw/share/mcalfits_conf/canali_bin0.dat
	int nbins = 97;
	float emc[] = {0.173000	,0.223000	,0.277000	,0.344539	,0.362613	,0.389725	,0.416836	,0.443947	,0.471058	,0.498170	,0.552706	,0.606629	,0.660551	,0.714474	,0.768396	,0.822319	,0.876241	,0.930164	,0.984086	,1.038009	,1.091932	,1.145854	,1.199777	,1.307622	,1.415467	,1.523312	,1.631157	,1.739002	,1.846847	,1.954692	,2.067977	,2.181516	,2.351825	,2.522134	,2.692443	,2.862752	,3.033061	,3.203370	,3.373679	,3.543988	,3.771066	,3.998145	,4.225224	,4.452302	,4.792920	,5.133538	,5.474156	,5.814774	,6.155392	,6.496010	,6.950167	,7.404324	,7.858482	,8.312639	,8.766796	,9.334493	,9.902189	,10.469890	,11.037580	,11.605280	,12.172980	,12.740670	,13.308370	,13.876070	,14.443760	,15.125000	,15.806230	,16.487470	,17.168710	,18.194500	,19.500300	,20.900000	,22.400000	,24.007700	,25.730800	,27.578000	,29.557000	,31.678400	,33.952100	,36.389000	,39.000700	,41.799900	,44.800000	,48.016000	,51.461700	,55.155300	,59.114000	,63.357000	,67.904100	,72.777800	,78.001300	,83.600000	,89.600000	,156.00300	,271.61600	,472.912000	,823.38700	,1433.6};
	
/*	// MCAL energy channels boundaries (MeV) from file ~/local/mcalsw/share/mcalfits_conf/canali_KW_bin1.dat
	int nbins = 76;
	float emc[] = {0.173000	,0.223000	,0.277000	,0.344539	,0.362613	,0.389725	,0.416836	,0.443947	,0.471058	,0.498170	,0.552706	,0.606629	,0.660551	,0.714474	,0.768396	,0.822319	,0.876241	,0.930164	,0.984086	,1.038009	,1.091932	,1.145854	,1.199777	,1.307622	,1.415467	,1.523312	,1.631157	,1.739002	,1.846847	,1.954692	,2.067977	,2.181516	,2.351825	,2.522134	,2.692443	,2.862752	,3.033061	,3.203370	,3.373679	,3.543988	,3.771066	,3.998145	,4.225224	,4.452302	,4.792920	,5.133538	,5.474156	,5.814774	,6.155392	,6.496010	,6.950167	,7.404324	,7.858482	,8.312639	,8.766796	,9.334493	,9.902189	,10.469890	,11.037580	,11.605280	,12.172980	,12.740670	,13.308370	,13.876070	,14.443760	,15.125000	,15.806230	,16.487470	,17.168710	,20.900000	,27.578000	,36.389000	,48.016000	,63.357000	,83.600000	,472.912000	,1433.6};
*/
	
	TH1F hmc("hmc", "hmc", nbins, emc);
	
	// fill with simulation data
	ifstream in;
	// in.open("/home/mcal/data_analysis/calibration/frascati/2481/calo_2345.dat");
//	in.open("/home/mcal/data_analysis/calibration/frascati/2580/calo_2580.dat");
	in.open("/home/mcal/data_analysis/calibration/frascati/newsim/calo_2481_bkg.dat");
	Int_t evt, plane, bar, evt0;
	Float_t energy, eA, eB, energy0;
	in >> evt >> plane >> bar >> energy >> eA >> eB;
	evt0 = evt;
	energy0 = energy;
	
	while (in >> evt >> plane >> bar >> energy >> eA >> eB) {
		//cout << evt << "  " << energy*1000. << endl;
		if (evt == evt0) energy0 +=energy;
		else {
			//cout << evt << "  " << energy*1000. << endl;
			//cout << "*** " << evt0 << "  " << energy0*1000. << endl;
			hmc->Fill(energy0*1000.);
			energy0 = energy;
			evt0 = evt;
		}
	}	// a volte puo' perdere l'ultimo evento
	in.close();
	
	// print data to make xspec-compliant pha files
	cout << endl << "pha file" << endl;
	for (int i=1; i<= nbins; i++) printf("%2d \t%8.0f\n", i, hmc->GetBinContent(i));

}	