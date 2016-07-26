{
//	epoch = new TTimeStamp(2003, 12, 31, 23, 0, 0, 0, 1, 0);	// reference date for TT time: 2004/01/01 00:00:01
	epoch = new TTimeStamp(2004, 1, 1, 0, 0, 0, 0, 1, 0);		// reference date for TT time: 2004/01/01 00:00:01
	tzero1 = new TTimeStamp();
	tzero2 = new TTimeStamp();
	double t01 = 133014356.656;  // 2008-03-19 12:25:56
	double t02 = 115091741.696;  // 2007-08-25 01:55:41
	tzero1->Set((int) t01 , 0, epoch->GetSec(),0);
	printf("Time0 TT %12.0f    UT %s\n", t01, tzero1->AsString("s"));

	tzero2->Set((int) t02 , 0, epoch->GetSec(),0);
	printf("Time0 TT %12.0f    UT %s\n", t02, tzero2->AsString("s"));

}