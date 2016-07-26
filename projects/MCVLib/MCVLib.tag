<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="struct">
    <name>MCALevt</name>
    <filename>structMCALevt.html</filename>
    <member kind="variable">
      <type>Double_t</type>
      <name>time</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>mult</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Short_t</type>
      <name>bar</name>
      <anchor>o2</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Short_t</type>
      <name>pdA</name>
      <anchor>o3</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Short_t</type>
      <name>pdB</name>
      <anchor>o4</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Float_t</type>
      <name>pos</name>
      <anchor>o5</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Float_t</type>
      <name>E</name>
      <anchor>o6</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Float_t</type>
      <name>Etot</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>totEX</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>totEZ</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVAnalysis</name>
    <filename>classMCVAnalysis.html</filename>
    <member kind="function">
      <type></type>
      <name>MCVAnalysis</name>
      <anchor>a1</anchor>
      <arglist>(TTree *t, MCVFileInfo *in=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoLombNormPeriod</name>
      <anchor>a3</anchor>
      <arglist>(float tstart, float tstop, int bar, int pd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoLombNormPeriod</name>
      <anchor>a4</anchor>
      <arglist>(float x[], float y[], unsigned long n)</arglist>
    </member>
    <member kind="function">
      <type>TH2F *</type>
      <name>GetCorrelationMatrix</name>
      <anchor>a5</anchor>
      <arglist>(float tstart, float tstop)</arglist>
    </member>
    <member kind="function">
      <type>TGraph *</type>
      <name>GetLombNormPeriod</name>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Double_t</type>
      <name>GetLinearCorrelation</name>
      <anchor>a7</anchor>
      <arglist>(int bar1, int pd1, int bar2, int pd2, float tstart, float tstop)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetData</name>
      <anchor>a8</anchor>
      <arglist>(TTree *t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetFileInfo</name>
      <anchor>a9</anchor>
      <arglist>(MCVFileInfo *in)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetHifac</name>
      <anchor>a10</anchor>
      <arglist>(float hf)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetOfac</name>
      <anchor>a11</anchor>
      <arglist>(float of)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a12</anchor>
      <arglist>(MCVAnalysis, 1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Avevar</name>
      <anchor>b0</anchor>
      <arglist>(float data[], unsigned long n, float *ave, float *var)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Error</name>
      <anchor>b1</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Fasper</name>
      <anchor>b2</anchor>
      <arglist>(float x[], float y[], unsigned long n, float ofac, float hifac, float wk1[], float wk2[], unsigned long nwk, unsigned long *nout, unsigned long *jmax, float *prob)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Four1</name>
      <anchor>b3</anchor>
      <arglist>(float data[], unsigned long nn, int isign)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Realft</name>
      <anchor>b4</anchor>
      <arglist>(float data[], unsigned long n, int isign)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Spread</name>
      <anchor>b5</anchor>
      <arglist>(float y, float yy[], unsigned long n, float x, int m)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>long</type>
      <name>lmaxarg1</name>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>long</type>
      <name>lmaxarg2</name>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>long</type>
      <name>lminarg1</name>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>long</type>
      <name>lminarg2</name>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>sqrarg</name>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MCALevt</type>
      <name>currevt</name>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MCVFileInfo *</type>
      <name>info</name>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TTree *</type>
      <name>tdata</name>
      <anchor>p7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>long</type>
      <name>nentries</name>
      <anchor>p8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>ofac</name>
      <anchor>p9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>hifac</name>
      <anchor>p10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float *</type>
      <name>wk1</name>
      <anchor>p11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float *</type>
      <name>wk2</name>
      <anchor>p12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float *</type>
      <name>time</name>
      <anchor>p13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float *</type>
      <name>yA</name>
      <anchor>p14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float *</type>
      <name>yB</name>
      <anchor>p15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned long</type>
      <name>nout</name>
      <anchor>p16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned long</type>
      <name>jmax</name>
      <anchor>p17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>prob</name>
      <anchor>p18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>nevt</name>
      <anchor>p19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraph *</type>
      <name>gLomb</name>
      <anchor>p20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH2F *</type>
      <name>hcorr</name>
      <anchor>p21</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVFile</name>
    <filename>classMCVFile.html</filename>
    <member kind="function">
      <type></type>
      <name>MCVFile</name>
      <anchor>a1</anchor>
      <arglist>(const char *name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>CreateRootFile</name>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsGrid</name>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPDHUData</name>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPDHUFormat</name>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintCalibrationParameters</name>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintFileInfo</name>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetCalibrationFile</name>
      <anchor>a8</anchor>
      <arglist>(char *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetSeed</name>
      <anchor>a9</anchor>
      <arglist>(unsigned int s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a11</anchor>
      <arglist>(MCVFile, 1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>printerror</name>
      <anchor>b0</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>CalculateEandPos</name>
      <anchor>b1</anchor>
      <arglist>(int cmult, int cbar, Short_t pdA, Short_t pdB)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>unsigned long</type>
      <name>GetTime</name>
      <anchor>b2</anchor>
      <arglist>(char *timestring)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>Initialize</name>
      <anchor>b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>LoadCalibrationParameters</name>
      <anchor>b4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>UpdateFileInfo</name>
      <anchor>b5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>fitsfile *</type>
      <name>fptr</name>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>status</name>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>nfields</name>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>comment</name>
      <anchor>p3</anchor>
      <arglist>[100]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>path</name>
      <anchor>p4</anchor>
      <arglist>[200]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Double_t *</type>
      <name>timearray</name>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MCVFileInfo</type>
      <name>fileInfo</name>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TString</type>
      <name>calFile</name>
      <anchor>p7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TEnv *</type>
      <name>conf</name>
      <anchor>p8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TEnv *</type>
      <name>stdconf</name>
      <anchor>p9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>barLen</name>
      <anchor>p10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>gainFactor</name>
      <anchor>p11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>u0A</name>
      <anchor>p12</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>u0B</name>
      <anchor>p13</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>alphaA</name>
      <anchor>p14</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>alphaB</name>
      <anchor>p15</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>oA</name>
      <anchor>p16</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>oB</name>
      <anchor>p17</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>alphaMean</name>
      <anchor>p18</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>alphaDelta</name>
      <anchor>p19</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>cx1</name>
      <anchor>p20</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>cx2</name>
      <anchor>p21</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>cE1</name>
      <anchor>p22</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Int_t</type>
      <name>soA</name>
      <anchor>p23</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Int_t</type>
      <name>soB</name>
      <anchor>p24</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TFile *</type>
      <name>fp</name>
      <anchor>p25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TTree *</type>
      <name>tdata</name>
      <anchor>p26</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>rootfilename</name>
      <anchor>p27</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MCALevt</type>
      <name>currevt</name>
      <anchor>p28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned int</type>
      <name>seed</name>
      <anchor>p29</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVFileInfo</name>
    <filename>classMCVFileInfo.html</filename>
    <member kind="function">
      <type>void</type>
      <name>PrintCalibrationParameters</name>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintFileInfo</name>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a4</anchor>
      <arglist>(MCVFileInfo, 1)</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>filename</name>
      <anchor>o0</anchor>
      <arglist>[200]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>runid</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>pkttype</name>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>nevents</name>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>tstart</name>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>tstop</name>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>ttot</name>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bool_t</type>
      <name>pdhuformat</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bool_t</type>
      <name>pdhudata</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bool_t</type>
      <name>grid</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>seed</name>
      <anchor>o10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TString</type>
      <name>calFile</name>
      <anchor>o11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>barLen</name>
      <anchor>o12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>gainFactor</name>
      <anchor>o13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>u0A</name>
      <anchor>o14</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>u0B</name>
      <anchor>o15</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>alphaA</name>
      <anchor>o16</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>alphaB</name>
      <anchor>o17</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>oA</name>
      <anchor>o18</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>oB</name>
      <anchor>o19</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>soA</name>
      <anchor>o20</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>soB</name>
      <anchor>o21</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable">
      <type>Int_t</type>
      <name>nlowDT</name>
      <anchor>o22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>lowDTti</name>
      <anchor>o23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>lowDTtf</name>
      <anchor>o24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Double_t</type>
      <name>teff</name>
      <anchor>o25</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVFilePKP</name>
    <filename>classMCVFilePKP.html</filename>
    <base>MCVFile</base>
    <base protection="private">MCVHisto</base>
    <member kind="function">
      <type></type>
      <name>MCVFilePKP</name>
      <anchor>a1</anchor>
      <arglist>(const char *name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>CreateRootFile</name>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Bool_t</type>
      <name>Add3913SciRatemeters</name>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Process3902Data</name>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Process3909Data</name>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintEvent</name>
      <anchor>a7</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a8</anchor>
      <arglist>(MCVFilePKP, 1)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVHisto</name>
    <filename>classMCVHisto.html</filename>
    <member kind="function">
      <type>Double_t</type>
      <name>GetEffectiveTime</name>
      <anchor>a2</anchor>
      <arglist>(Int_t ibar)</arglist>
    </member>
    <member kind="function">
      <type>Double_t</type>
      <name>GetEffectiveTime</name>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a4</anchor>
      <arglist>(MCVHisto, 1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>FillGlobalHisto</name>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>FillHisto</name>
      <anchor>b1</anchor>
      <arglist>(MCALevt *evt)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>ResizeCountRateHisto</name>
      <anchor>b2</anchor>
      <arglist>(int time)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>FillCountRateDistribution</name>
      <anchor>b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawPDA</name>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawPDB</name>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>raw2D</name>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawPos</name>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawE</name>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawDTime</name>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawCRate</name>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawCRDist</name>
      <anchor>p7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawMult</name>
      <anchor>p8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawSciRM3913</name>
      <anchor>p9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>sciRMX</name>
      <anchor>p10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>sciRMZ</name>
      <anchor>p11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gRMX</name>
      <anchor>p12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gRMZ</name>
      <anchor>p13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gRMX3913</name>
      <anchor>p14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gRMZ3913</name>
      <anchor>p15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>totDT</name>
      <anchor>p16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH2F *</type>
      <name>barcorr</name>
      <anchor>p17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH2F *</type>
      <name>barcorrLowDT</name>
      <anchor>p18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>totCR</name>
      <anchor>p19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>totMult</name>
      <anchor>p20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH1F *</type>
      <name>totE</name>
      <anchor>p21</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH2F *</type>
      <name>hX</name>
      <anchor>p22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TH2F *</type>
      <name>hZ</name>
      <anchor>p23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gCRX</name>
      <anchor>p24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gCRZ</name>
      <anchor>p25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gMultX</name>
      <anchor>p26</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TGraphErrors *</type>
      <name>gMultZ</name>
      <anchor>p27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawHisto</name>
      <anchor>p28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TFile *</type>
      <name>fh</name>
      <anchor>p29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>histofilename</name>
      <anchor>p30</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Double_t</type>
      <name>prtime</name>
      <anchor>p31</anchor>
      <arglist>[30]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Double_t</type>
      <name>prt</name>
      <anchor>p32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Int_t</type>
      <name>prmult</name>
      <anchor>p33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Int_t</type>
      <name>prbar</name>
      <anchor>p34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Int_t</type>
      <name>nlowDT</name>
      <anchor>p35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Double_t</type>
      <name>lowDTti</name>
      <anchor>p36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Double_t</type>
      <name>lowDTtf</name>
      <anchor>p37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Float_t</type>
      <name>Eband</name>
      <anchor>p38</anchor>
      <arglist>[11]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Float_t</type>
      <name>Eerr</name>
      <anchor>p39</anchor>
      <arglist>[11]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Float_t</type>
      <name>gf</name>
      <anchor>p40</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVRawMonitor</name>
    <filename>classMCVRawMonitor.html</filename>
    <member kind="function">
      <type></type>
      <name>MCVRawMonitor</name>
      <anchor>a0</anchor>
      <arglist>(const TGWindow *p, TGWindow *main, UInt_t w, UInt_t h)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>CloseWindow</name>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>FillRaw</name>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Bool_t</type>
      <name>ProcessMessage</name>
      <anchor>a4</anchor>
      <arglist>(Long_t msg, Long_t parm1, Long_t parm2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Set2DLimits</name>
      <anchor>a5</anchor>
      <arglist>(float, float, float, float)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetConf</name>
      <anchor>a6</anchor>
      <arglist>(TEnv *c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetFileInfo</name>
      <anchor>a7</anchor>
      <arglist>(MCVFileInfo *info)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetHistoCollection</name>
      <anchor>a8</anchor>
      <arglist>(TObjArray *hc)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a9</anchor>
      <arglist>(MCVRawMonitor, 1)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MCVStdAnalysis</name>
    <filename>classMCVStdAnalysis.html</filename>
    <member kind="function">
      <type></type>
      <name>MCVStdAnalysis</name>
      <anchor>a1</anchor>
      <arglist>(TObjArray *hc, MCVFileInfo *in=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetGridOffset</name>
      <anchor>a3</anchor>
      <arglist>(TCanvas *c1=NULL, TCanvas *c2=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintGridOffset</name>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetFileInfo</name>
      <anchor>a5</anchor>
      <arglist>(MCVFileInfo *in)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetHistoCollection</name>
      <anchor>a6</anchor>
      <arglist>(TObjArray *hc)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ClassDef</name>
      <anchor>a7</anchor>
      <arglist>(MCVStdAnalysis, 1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>LoadConf</name>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TObjArray *</type>
      <name>rawHisto</name>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MCVFileInfo *</type>
      <name>info</name>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>TEnv *</type>
      <name>conf</name>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>groff</name>
      <anchor>p3</anchor>
      <arglist>[60]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>grsig</name>
      <anchor>p4</anchor>
      <arglist>[60]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>gramp</name>
      <anchor>p5</anchor>
      <arglist>[60]</arglist>
    </member>
  </compound>
</tagfile>
