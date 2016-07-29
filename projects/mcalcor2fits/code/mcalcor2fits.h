
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "fitsio.h" 

// NB il path deve essere assoluto
#define STDTEMPLATE "/home/mcal/BUILD_MCALSW.BUILD14-devel/projects/mcalcor2fits/conf/phlisttemplate.txt"
#define CALFILE     "/home/mcal/BUILD_MCALSW.BUILD14-devel/projects/mcalcor2fits/conf/MCALBurstTest_601.dat"
#define VERBOSE 1

// execute ADC  --> energy / position conversion
void CalculateEandPos(int nevt, int cbar, short int pdA, short int pdB, double *oA, double *oB, double *cx1, double *cx2, double *cE1, float *energy, float *pos );
