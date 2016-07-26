int print_event_list(int contact=30620, double t0=291672738.746714, char *fout="./tmp.dat", float dt=0.1)
{
	char fname[100];
	sprintf(fname,"/MCAL_PROC/ARCHIVE/%06d/RT%06d_3908.root", contact, contact);
	TFile *f = new TFile(fname,"r");
	TTree *t = (TTree *) f->Get("tdata");
	FILE *fo;
	fo = fopen(fout, "w");
	double time;
	float energy;
 	t->SetBranchAddress("time", &time);
	t->SetBranchAddress("Etot", &energy);
	int i=0; 
	while (t->GetEntry(i++)) if (time>t0-dt && time <t0+dt) fprintf(fo, "%.6f \t%.2f\n", time-t0, energy);
	fclose(fo);
	return 0;
}

