// Last update: 3-july-2006
// changes a 3098 L1 fits into a .root file,
// m.galli, june 2006


#include "MCVFile3916.h"
#include "version.h"

int main(int argc, char *argv[])
{
	char tmp;
	MCVFile3916 *fp3916;

	cout << "convPKP3916toRoot version " << VERSION << endl << "Trying to open fits file: " << argv[1] << endl;

        cout << " reading 3916 L1 file and defining hists.: "<< argv[1] << endl ; 
	cout << " Output: " << argv[2] << endl;
	cout << " ECI: " << argv[3] << endl;
		
	fp3916 = new MCVFile3916(argv[1], 1, atoi(argv[3]));

	cout << " calling CreateRootFile for 3916 data " << endl ; 
	
	fp3916->WriteRootFile(argv[2]);

	cout << "All done ..." << endl;
	//cout << "Strike any key + return to continue" << endl;
	//cin >> tmp;
	
	
        cout << " wait please ... freeing space ... " << endl ; 
	//delete fp;
	//delete fp3916;

	return 0;
}
