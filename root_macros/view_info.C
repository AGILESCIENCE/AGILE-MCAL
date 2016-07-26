{
TFile *f= new TFile("/data1/mcalsw_output/misc/MCAL_global_data_2006.root");
TTree *gdata = (TTree *) f->Get("runInfo");
MCVFileInfo *in = new MCVFileInfo();
TBranch *br = gdata->GetBranch("run_info");
br->SetAddress(&in);
int nrun = gdata->GetEntries();
for (int i=0; i<nrun; i++) {
    gdata->GetEntry(i);
    in->PrintFileInfo();
}
}
