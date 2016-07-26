apoisson(double m=1., int n=1)
{
  Double_t pois = TMath::Exp(-m);
  Double_t cum = pois;
  
  printf("%3d  %.6e  %.6e  %.6e  %.6e \n", 0, pois, cum, 1. - cum, TMath::Gamma(1, m));
  for (int i=1; i<=n; i++) {
    pois *= (m/i);
    cum += pois;
    printf("%3d  %.6e  %.6e  %.6e  %.6e\n", i, pois, cum, 1. - cum, TMath::Gamma(i+1, m));
  }


}