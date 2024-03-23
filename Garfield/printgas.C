#include <iostream>
#include <sstream>
#include <string>
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

#include "Plotting.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  MediumMagboltz * gas = new MediumMagboltz();

 // Setup the gas
  const double pressure = 750.; //Torr
  const double temperature = 293.15; //K
 
 // Set the temperature [K] and pressure [Torr]
  gas->SetTemperature(temperature);
  gas->SetPressure(pressure);
  gas->SetComposition("co2", 80, "ic4h10", 20);

  gas->LoadGasFile("co2_80_iso_20.gas");
  gas->LoadIonMobility("/home/musser/helix/garfield/Data/IonMobility_CO2+_CO2.txt");
  gas->PrintGas();

  
}
