//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "MCVBarMonitor.h"
ClassImp(MCVBarMonitor);

MCVBarMonitor::MCVBarMonitor() : MCVStdAnalysis()
{

}

//
//--------------------------------------------------------------------
//

MCVBarMonitor::MCVBarMonitor(TObjArray *hc, MCVFileInfo *in) : MCVStdAnalysis(hc, in)
{

}

//
//--------------------------------------------------------------------
//

MCVBarMonitor::~MCVBarMonitor()
{

}

//
//--------------------------------------------------------------------
//

int MCVBarMonitor::Calculate()
{
	if (rawHisto==NULL && info==NULL) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
	
	if (!conf) {
		status = CONF_FILE_NOT_FOUND;
		return status;
	}
//	if (info) {

	return status;
}

//
//--------------------------------------------------------------------
//

void MCVBarMonitor::PrintResults()
{
	printf("\n---------------------  ----------------------\n");
	
	return;
}
