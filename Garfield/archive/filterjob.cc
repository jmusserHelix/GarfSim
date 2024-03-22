#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TFile.h>
#include <TH1.h>
#include "MediumMagboltz.hh"
#include "FundamentalConstants.hh"
#include "SolidBox.hh"
#include "ComponentAnalyticField.hh"
#include "GeometrySimple.hh"
#include "ViewCell.hh"
#include "ViewField.hh"
#include  "TrackSimple.hh"
#include  "ViewDrift.hh"
#include "TrackHeed.hh"
#include "DriftLineRKF.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "ViewSignal.hh"
#include "Random.hh"
#include <TROOT.h>
#include <TRint.h>
#include "DCsim.hh"
#include <fstream.h>
#include "Plotting.hh"
#include <TMath.h>



using namespace Garfield;

int main(int argc, char * argv[]) {
  TRint* app = new TRint("Garfield", &argc, argv, 0, 0);

  char * siminFile;
  char * simoutFile;

  double trackx,trackang;
  double tracky = 3.5;
  int ntrack;

  siminFile = getenv("DCSimRootfile");
  std::cout << "Input File: "<< siminFile << std::endl;

  simoutFile = getenv("DCSimFilterRootfile");
  std::cout << "Input File: "<< simoutFile << std::endl;
  char * cdum;

  cdum = getenv("DCSimNtrack");
  ntrack  = atoi(cdum);
  std::cout << " # tracks " << ntrack << std::endl;

  cdum= getenv("DCSimtrackx");
  trackx = atof(cdum);
  std::cout << " Track Start X " << trackx << std::endl;

  cdum = getenv("DCSimtrackang");
  trackang = atof(cdum);
  std::cout << " Track Angle " << trackang << std::endl;

  DCsim * sim = new DCsim();
 
  sim->MakeTree(simoutFile);
  sim->SetNwires(7);
  sim->InitDigitization();

  TH1D *SensewireSig;
  TH1D *FilteredSensewireSig;

  TFile infile(siminFile);
  std::stringstream wid;
  std::stringstream widin;
  std::stringstream widout;
  infile.Print();
  TCanvas* canvas1 = new TCanvas();
  canvas1->cd();
   for(int itrack=1;itrack<ntrack;itrack++){
     for(int iw = 0;iw<7;iw++){
      infile.cd();
      int iadd = iw;
      int iplane=0;
      wid.str("");
      widin.str("");
      wid << "a_" <<iplane << "_" << iw ;
      widin << "a_" <<iplane << "_" << iw <<";" << itrack;
      widout.str("");
      widout << "a_" <<iplane << "_" << iw <<"Filtered";

       std::string str(wid.str());
      const char * name = str.c_str();

      std::string strin(widin.str());
      const char * namein = strin.c_str();

      std::string strout(widout.str());
      const char * nameout = strin.c_str();
      std::cout << iadd << " " << nameout << std::endl;
   

      //     SensewireSig = new TH1D(name,name,5000,0,10000);
      FilteredSensewireSig = new TH1D(nameout,nameout,5000,0,10000);
      infile.GetObject(namein,SensewireSig);
      SensewireSig->Print();
      sim->Filter(SensewireSig,FilteredSensewireSig);

      sim->DCsimOutFile()->cd();
      FilteredSensewireSig->Write();
      infile.cd();
 
      sim->Digitize(iw, FilteredSensewireSig);
      std::cout << " here " << std::endl;
      int threshbin = sim->FirstThreshold(iw,FilteredSensewireSig);
      std::cout << " wire thresh " << iw << " " << threshbin << std::endl;    
      for(int isample=threshbin-1;isample<threshbin+5;isample++){
	std::cout << " sample " << sim->GetSample(iw,isample) << std::endl;
	sim->DCDataPacket[iw].Sample.push_back(sim->GetSample(isample,FilteredSensewireSig));
      }    
     }
     sim->FitLeadingEdges();
     sim->FillTree( trackx,tracky,trackang); 
   }

   sim->DCsimOutFile()->cd();
   sim->WriteTree();
   sim->DCsimOutFile()->Close();
     
  
}
