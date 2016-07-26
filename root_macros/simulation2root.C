int simulation2root(char *input_fits="in.fits", char *output_root="out.root")
{
	// MM 02/12/2009
	// converte un file fits (in formato DFE-TE) in un file root (in formato MCAL standard, i.e. il mio)
	
	// e.g.: simulation2root("/home/mcal/bkg_sim/good/SIM_ae_hitlist.fits", "/home/mcal/bkg_sim/good/SIM_ae_hitlist.root")

	gROOT->Reset();
	gSystem->Load("/home/mcal/local/mcalsw/lib/libMCV.so");

	MCVFileTE *fp;
//	fp = new MCVFileTE("/home/mcal/bkg_sim/good/SIM_ae_hitlist.fits");
	fp = new MCVFileTE(input_fits);
	fp->PrintCalibrationParameters();
//	fp->SetRootFileName("/home/mcal/bkg_sim/good/SIM_ae_hitlist.root");
	fp->SetRootFileName(output_root);
	fp->CreateRootFile();
	
	delete fp;
	return 0;
}