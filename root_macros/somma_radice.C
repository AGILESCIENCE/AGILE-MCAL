{
// Root 3.10/01 macro 
// analisi sull'uso della somma o della radice del prodotto dei segnali per la stima dell'energia di MCAL per i ratemeters
// Martino Marisaldi 2/12/2004

#include "stdio.h"
#include "Riostream.h"
#include "math.h"

   gROOT->Reset();

// set up the graphic environment
   gStyle->SetOptStat(1);
   gStyle->SetPalette(9,0);
   gStyle->SetCanvasColor(0);//33
   gStyle->SetFrameFillColor(0);//18

   gStyle->SetLabelSize(0.04, "X");
   gStyle->SetLabelSize(0.04, "Y");
   gStyle->SetTitleSize(0.04, "A");
   gStyle->SetTitleSize(0.04, "X");
   gStyle->SetTitleSize(0.04, "Y");
   gStyle->SetTitleSize(0.04, "Z");
   gStyle->SetTitleColor(0);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(0);
   
   c1 = new TCanvas("c1","somma o radice",100,100,800,600);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->Divide(2,2);

   c2 = new TCanvas("c2","spectra",50,50,1000,800);
   c2->SetFillColor(0);
   c2->SetBorderMode(0);
   c2->Divide(4,3);

// MCAL parameters

int nBars = 15; // bars per plane
float lyMean = 20.; // mean light yield in e-/keV 
float lySigma = 3.; // light yield standard deviation (or 1/2 interval for uniform distribution)
float alphaMean = 0.025;  // mean bar attenuation in cm^-1
float alphaSigma = 0.005; // bar attenuation standard deviation (or 1/2 interval for uniform distribution)
float barLength = 37.5; // bar length (cm)

float enoise = 1000.; // electronic noise in e- rms
float range = 250.; // energy range for one bar hit at the PD (MeV)
double che = 4096./(range*1000.*lyMean); // electrons per channel
int offset = 0.; // ADC offset in channels

// simulation parameters

int nev = 100000; // number of random events to be simulated
float noiseCoef = 1; // 1: electronic noise include; 0: electronic noise off
int nE = 6; // number of energy values to be studied
float E[6]={300., 500., 1000., 2000., 5000., 10000.}; // energy values to be studied

float *alpha = new float[nBars];  
float *ly = new float[nBars];

// auxiliary variables

float xA = -0.5*barLength;
float xB = 0.5*barLength;
float coeff = exp(alphaMean*xB);  // multiplicative coefficient for product calculations
float erange = range*1000.*lyMean; // range in electrons

// fill the bar parameters with random distribution

TH1F *halpha = new TH1F("halpha","attenuation coefficient histogram", 40, 0.010, 0.050);
TH1F *hly = new TH1F("hly","light yield histogram", 20, 10., 30.);

for (int i=0; i<nBars; i++) {
  //  alpha[i]=gRandom->Gaus(alphaMean, alphaSigma);  // gaussian distribution
  //  ly[i]=gRandom->Gaus(lyMean, lySigma);
  alpha[i]=gRandom->Uniform(alphaMean-alphaSigma, alphaMean+alphaSigma);  // uniform distribution
  ly[i]=gRandom->Uniform(lyMean-lySigma, lyMean+lySigma);
  halpha->Fill(alpha[i]);
  hly->Fill(ly[i]);
}

c1->cd(1);
halpha->Draw();
c1->cd(2);
hly->Draw();

// generate events at random positions in random bars

TH1F *hpos = new TH1F("hpos","events position histogram", 100, xA, xB);
TH1F *hbar = new TH1F("hbar","light yield histogram", nBars, 0, nBars);

float *pos = new float[nev];
int *bar = new int[nev];
for (int i=0; i<nev; i++) {
  pos[i] = gRandom->Uniform(xA, xB); // default: all bar length (xA, xB)
  bar[i] = gRandom->Integer(nBars);
  hpos->Fill(pos[i]);
  hbar->Fill(bar[i]);
}

c1->cd(3);
hpos->Draw();
c1->cd(4);
hbar->Draw();

// work on events and build histograms

double eA, eB;
int chA, chB;
float smean, srms, pmean, prms;
char name[10];

TH1F *hs0 = new TH1F("hs0","0.3 MeV sum histogram", 4096, 0, 4096);
TH1F *hp0 = new TH1F("hp0","0.3 MeV product histogram", 4096, 0, 4096);

TH1F *hs1 = new TH1F("hs1","0.5 MeV sum histogram", 4096, 0, 4096);
TH1F *hp1 = new TH1F("hp1","0.5 MeV product histogram", 4096, 0, 4096);

TH1F *hs2 = new TH1F("hs2","1 MeV sum histogram", 4096, 0, 4096);
TH1F *hp2 = new TH1F("hp2","1 MeV product histogram", 4096, 0, 4096);

TH1F *hs3 = new TH1F("hs3","2 MeV sum histogram", 4096, 0, 4096);
TH1F *hp3 = new TH1F("hp3","2 MeV product histogram", 4096, 0, 4096);

TH1F *hs4 = new TH1F("hs4","5 MeV sum histogram", 4096, 0, 4096);
TH1F *hp4 = new TH1F("hp4","5 MeV product histogram", 4096, 0, 4096);

TH1F *hs5 = new TH1F("hs5","10 MeV sum histogram", 4096, 0, 4096);
TH1F *hp5 = new TH1F("hp5","10 MeV product histogram", 4096, 0, 4096);

// loop on all energy values

printf("E (MeV) \tSomma (ADC ch) \t\t\tRadice (ADC ch)\n");
printf("\t\tmedia \tsigma \tDE/E FWHM \tmedia \tsigma \tDE/E FWHM\n\n");
for (int i=0; i<nE; i++) {
  TH1F *hse = new TH1F("hse","electrons sum histogram", 300000, 0, 300000);
  TH1F *hpe = new TH1F("hpe","electrons product histogram", 200000, 0, 200000);
  float E0 = E[i];
  sprintf(name, "hs%d", i);
  TH1F *hs = gROOT->FindObject(name);
  sprintf(name, "hp%d", i);
  TH1F *hp = gROOT->FindObject(name);

  // loop on all events
  for (int j=0; j<nev; j++) {
    eA = E0*ly[bar[j]]*exp(-alpha[bar[j]]*(pos[j]-xA)) + noiseCoef*gRandom->Gaus(0, enoise);  // signal in e- at PD A
    eB = E0*ly[bar[j]]*exp(-alpha[bar[j]]*(xB-pos[j])) + noiseCoef*gRandom->Gaus(0, enoise);  // signal in e- at PD B
    hse->Fill(eA+eB);
    if (eA>=0 && eB>=0) hpe->Fill(sqrt(eA*eB));
    chA = int(eA*che);
    chB = int(eB*che);
    hs->Fill(chA+chB - 2*offset);
    if (eA>=0 && eB>=0) hp->Fill(int(coeff*sqrt((chA-offset)*(chB-offset))));
  }

  // output data

  // output in electrons
  /*  smean = hse->GetMean();
  srms = hse->GetRMS();
  pmean = hpe->GetMean();
  prms = hpe->GetRMS();
  printf("%6.2f \t\t%6.0f \t%6.0f \t%6.4f \t\t%6.0f \t%6.0f \t%6.4f\n", E0*0.001, smean, srms, 2.35*srms/smean, pmean, prms, 2.35*prms/pmean); 
  */
  // output in ADC channels (default output)
  smean = hs->GetMean();
  srms = hs->GetRMS();
  pmean = hp->GetMean();
  prms = hp->GetRMS();
  printf("%6.2f \t\t%6.2f \t%6.2f \t%6.4f \t\t%6.2f \t%6.2f \t%6.4f\n", E0*0.001, smean, srms, 2.35*srms/smean, pmean, prms, 2.35*prms/pmean); 


  c2->cd(2*i+1);
  hs->Draw();
  c2->cd(2*i+2);
  hp->Draw();

  delete hse;
  delete hpe;
}

}
