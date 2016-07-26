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

#include "MCVSubMS.h"
ClassImp(MCVSubMS);

MCVSubMS::MCVSubMS(TH1F *hs):hSubMS(NULL)
{
	// tmp: info to be read from configuration file
	int tmp_nthr[4] = {5, 5, 5, 5};
	bool tmp_activewin[4] = {FALSE, FALSE, TRUE, FALSE};
	double tmp_winduration[4] = {140., 250., 585., 1024.};
	
	hSubMS = hs;
	for (int i=0; i<4; i++) {
		nwin[i] = ntrg[i] = 0;
		iniwin[i] = NULL;
		finwin[i] = NULL;
		nthr[i] = tmp_nthr[i];
		activewin[i] = tmp_activewin[i];
		winduration[i] = tmp_winduration[i];
		tprev[i] = -CHKTIME;
	}
}

//
//--------------------------------------------------------------------
//

MCVSubMS::~MCVSubMS()
{
/*	subwin *currwin;

	for (int i=0; i<4; i++) {
		if (activewin[i]) {
			cout << "Deleting open " << nwin[i] << " sub-ms window for time window " << i << endl;
			currwin = iniwin[i];
			while (!currwin->next) {
				currwin = currwin->next;
				delete currwin->prev;
			}
			delete currwin;
		}
	}*/
}

//
//--------------------------------------------------------------------
//

void MCVSubMS::ProcessEvent(MCALevt *evt)
{
	int naux;
	subwin *currwin, *oldwin;

	for (int i=0; i<4; i++) {
		if (activewin[i]) {
			if (nwin[i] > 0) {
				naux = nwin[i];
				currwin = iniwin[i];
				for (int j=0; j<naux; j++) {
					if (evt->time > currwin->tfin) {	// close a window, update histogram
						hSubMS->Fill(currwin->n);
						iniwin[i] = currwin->next;
						oldwin = currwin;
						currwin = oldwin->next;
						delete oldwin;
						nwin[i]--;
					} else {	// update a window
						AddEvt(currwin, evt);
						currwin = currwin->next;
					}
				}
			}	
			
			// add new window
			if (nwin[i] == 0) {
				nwin[i]++;
				iniwin[i] = InitSubwin(evt->time, winduration[i]);
				AddEvt(iniwin[i], evt);
				finwin[i] = iniwin[i];			
			} else {
				if (nwin[i] < MAXWIN) {
					nwin[i]++;
					currwin = InitSubwin(evt->time, winduration[i]);
					AddEvt(currwin, evt);
					currwin->prev = finwin[i];
					finwin[i]->next = currwin;
					finwin[i] = currwin;
				}			
			}
			
			// check for trigger
			if (nwin[i] > nthr[i]) {	
				if ((evt->time - tprev[i]) > CHKTIME) {
					printf("NEW \tsub-ms trigger n. %d at time %15f on window %d  n. events = %d\n", ntrg[i], evt->time, i, nwin[i]);
					ntrg[i]++;
					tprev[i] = evt->time;
				} else {
					printf("\tsub-ms trigger n. %d at time %15f on window %d  n. events = %d (same trigger as previous)\n", ntrg[i]-1, evt->time, i, nwin[i]);
				}
			}
		} 
	}

}

//
//--------------------------------------------------------------------
//

subwin *MCVSubMS::InitSubwin(double t0, double dt)
{
	subwin *sw = new subwin;
	sw->tini = t0;
	sw->tfin = t0 + dt;
	sw->prev = NULL;
	sw->next = NULL;
	sw->n = 0;
	return sw;
}

//
//--------------------------------------------------------------------
//

void MCVSubMS::AddEvt(subwin *sw, MCALevt *inevt)
{
	int auxmult;
	int auxSciRME;
	
	MCALevt tmp;
	MCALevt *currevt;
	if (sw->n < 10) {
		currevt = &(sw->evtarr[sw->n]);   
		currevt->time = inevt->time;
		auxmult = inevt->mult;
		currevt->mult = auxmult;
		currevt->totEX = 0;
		currevt->totEZ = 0;
		for (int i=0; i<30; i++) currevt->bar[i] = inevt->bar[i];
		for (int i=0; i<auxmult; i++) {
			currevt->pdA[i] = inevt->pdA[i];
			currevt->pdB[i] = inevt->pdB[i];
			currevt->pos[i] = inevt->pos[i];
			currevt->E[i] = inevt->E[i];
			currevt->totEX = inevt->totEX;
			currevt->totEZ = inevt->totEZ;
			
		}
		currevt->Etot = inevt->Etot;
	} else cout << "*** N>10!" << endl;
	(sw->n)++;
	return;
}

//
//--------------------------------------------------------------------
//
