{
gROOT->Reset();
TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
mcalstyle->SetCanvasBorderMode(0);
mcalstyle->SetPadBorderMode(0);
mcalstyle->SetPadColor(0);
mcalstyle->SetCanvasColor(0);
mcalstyle->SetTitleColor(1); // 0
mcalstyle->SetStatColor(0);
mcalstyle->SetPalette(1,0);
mcalstyle->SetFrameBorderMode(0);
mcalstyle->SetLabelSize(0.05, "X");
mcalstyle->SetLabelSize(0.05, "Y");
gROOT->SetStyle("mcalstyle");

TFile fi("/data1/mcalsw_output_COMMISSIONING/H/H000601.root");
TObjArray *ai = (TObjArray *) fi->Get("rawHisto");

TGraphErrors *hi, *hz;
hi = (TGraphErrors *) ai->FindObject("gRMX3913");
hz = (TGraphErrors *) ai->FindObject("gRMZ3913");

}