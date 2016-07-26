int info_time(double t0, char *outpath="./outtime.tmp")
{
	// MM 05/11/09 write UT time starting from t0. checked, OK!

	TTimeStamp epoch(2004, 1, 1, 0, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	TTimeStamp tzero;
	tzero.Set((int) t0, 0, epoch.GetSec(),0);
	printf("%s.%06d\n", tzero.AsString("s"), (int) (fmod(t0,1.)*1.E6));
	
	UInt_t yy, mm, dd, hh, min, sec;
	int date = tzero.GetDate(1,0, &yy, &mm, &dd);
	int time = tzero.GetTime(1,0, &hh, &min, &sec);
	cout << yy << " " << mm << " " << dd << " " << hh << " " << min << " " << sec << endl;
	
	
	/*FILE *fp;
	fp = fopen(outpath, "w");
	fprintf(fp, "%s.%06d", tzero.AsString("s"), (int) (fmod(t0,1.)*1.E6));
	fclose(fp);
	*/
	
	return 0;
}