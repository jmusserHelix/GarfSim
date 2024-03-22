
{
  gROOT->ProcessLine(".x crestrootstyle.C");
  TFile noisefile("NoiseStudy.root");
  TTree * noisetree = noisefile.Get("t");
  noisetree->UseCurrentStyle();

}
