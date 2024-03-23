
{
  gROOT->ProcessLine(".x crestrootstyle.C");
  TFile noisefile("NoiseStudy.root");
  TTree * noisetree = (TTree *)noisefile.Get("t");
  noisetree->UseCurrentStyle();

}
