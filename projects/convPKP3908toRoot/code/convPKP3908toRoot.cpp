// Last update: 3-july-2006
// changes a 3098 L1 fits into a .root file,
// m.galli, june 2006


#include "MCVFilePKP3908.h"
#include "version.h"

int main(int argc, char *argv[])
{
	char tmp;
	//MCVFilePKP *fp;
	MCVFilePKP3908 *fp3908;
	cout << "convPKP3908toRoot version " << VERSION << endl << "Trying to open fits file: " << argv[1] << endl;

	//fp = new MCVFilePKP(argv[1]);
	//fp->PrintFileInfo();
	//fp->PrintCalibrationParameters();
	//fp->CreateRootFile();
	//fp->PrintFileInfo();

        cout << " reading 3908 L1 file and defining hists.: "<< argv[1] << endl ; 
		
	fp3908 = new MCVFilePKP3908(argv[1]);

	fp3908->Resize_bsr_ric() ; // set histo between DATE_OBS and DATE_END

	fp3908->PrintFileInfo();
	
	//fp3908->PrintCalibrationParameters(); 
	
	cout << " calling CreateRootFile for 3908 data " << endl ; 
	
	fp3908->CreateRootFile3908();

	cout << "All done ..." << endl;
	//cout << "Strike any key + return to continue" << endl;
	//cin >> tmp;
	
	
        cout << " wait please ... freeing space ... " << endl ; 
	//delete fp;
	delete fp3908;

	return 0;
}
