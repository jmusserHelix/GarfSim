#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
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
#include "TrackSimple.hh"
#include "ViewDrift.hh"
#include "TrackHeed.hh"
#include "DriftLineRKF.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "ViewSignal.hh"
#include "Random.hh"
#include <TROOT.h>
#include <TRint.h>
#include "DCsim.hh"
#include <fstream>
#include "Plotting.hh"
#include "TMath.h"
#include <sys/types.h>
#include <unistd.h>



using namespace Garfield;

int main(int argc, char * argv[]) {

  char * simoutFile;

  char * cdum;

  bool realtimeplots = true;

  TRint* app = new TRint("Garfield", &argc, argv, 0, 0);
  TH1D *SensewireSig[180];
  TFile * Outfile = new TFile("FieldPlots.root","recreate");

  std::stringstream wid;

  int pid = getpid();
  timeval t;
  gettimeofday(&t, NULL);
  int seed = pid*t.tv_usec;
  std::cout << "Random Seed: " << seed << std::endl;
  randomEngine.Seed(seed);


  for (int iplane=0;iplane<1;iplane++){
    for (int iw =0;iw<7;iw++){
      int iadd = iplane*7 + iw;
      wid.str("");
      wid << "a_" <<iplane << "_" << iw ;
      std::cout << iadd << " " << wid.str() << std::endl;
      std::string str(wid.str());
      const char * name = str.c_str();
      SensewireSig[iadd] = new TH1D(name,name,5000,0,10000);
    }
  }
  
  Garfield::plottingEngine.SetDefaultStyle();
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

  ComponentAnalyticField * cmp = new ComponentAnalyticField();
  cmp->SetMagneticField(0.,0.,1.0);

  GeometrySimple * geo = new GeometrySimple();
 
  SolidBox * enclosure = new SolidBox(0,0,0,10,4.7,11);
  geo->AddSolid(enclosure, gas);
  cmp->SetGeometry(geo);

  const double vCathode= -10000;
  const double rCathode= 175e-4/2.0;
  const double vAnode= 0;
  const double rAnode= 20e-4/2.0;
  const double vPotential= -2500;
  const double rPotential= 175e-4/2.0;
  
  const double anodesep = 0.8;
  const double potentialsep = 0.8;
  const double cathodesep =0.4;
  const double fieldsep = 0.2;
   
  for (int iplane=0;iplane<2;iplane++){
    for(int iw=0;iw<17;iw++){
      float y = 3.2-iw*cathodesep;
      float x = 8-16.0*iplane;
      cmp->AddWire(x,y,2 * rCathode, vCathode, "c");
      std::cout << " wire " << x << " " << y << " " << vCathode << " " << "c" << std::endl;
    }
  }
  
  int nwire = 7;
  for (int iplane=0;iplane<1;iplane++){
    for(int iw=0;iw<nwire;iw++){
      wid << "a_" << iplane << "_" << iw;      
      float y = 2.4-iw*anodesep;
      float sign = 1.0;
      if(iw%2==0) sign = -1.0;
      float x = sign*300e-4;
      cmp->AddWire(x,y,2 * rAnode, vAnode, "a");
      std::cout << " wire " << x << " " << y << " " << vAnode << " " << "a" << std::endl;
    }
    for(int iw=0;iw<nwire+1;iw++){
      float y = 2.8-iw*potentialsep;
      float x = 0;
      cmp->AddWire(x,y,2 * rPotential, vPotential, "p");
      std::cout << " wire " << x << " " << y << " " << vPotential << " " << "p" << std::endl;
    }
    cmp->AddWire(0,3.2,2 * rPotential, vPotential, "p2");
    cmp->AddWire(0,-3.2,2 * rPotential,vPotential, "p2");
  }

  //  cmp->AddPlaneX(-8,-10000,"cP1"); 
  //  cmp->AddPlaneX(8,-10000,"cP2");
  
  float v;
  float tweak;
  nwire=(int)(16.0/fieldsep) -1;
  for (int iplane=0;iplane<2;iplane++){
    for(int iw=0;iw<nwire;iw++){
      float y = 3.6-7.2*iplane;
      float x = 8-(iw+1)*fieldsep;
      tweak = -0.15;
     
      if(x>=0)
	{ 
	  v= (1+tweak*(8-x)/8 )*( vCathode +  (vPotential-vCathode)*(8-x)/8.)  ;
	}
      else 
	{
	  v=(1+tweak*(8+x)/8)*(vPotential + (vCathode-vPotential)*(-x)/8.);
	}
     
      std::cout << " wire " << x << " " << y << " " << v << " " << "T" << std::endl;
      cmp->AddWire(x,y,2*rCathode, v, "T");
    }
  }

 
  Sensor * sensor = new Sensor;
  ViewSignal * vs1 = new ViewSignal;
 
  sensor->AddComponent(cmp);
  sensor->SetTimeWindow(0,2,5000);
  
  cmp->AddReadout("a"); 
  sensor->AddElectrode(cmp,"a");
  vs1->SetSensor(sensor);
  
  TCanvas* canvas3 = new TCanvas();
  vs1->SetCanvas(canvas3);

  if(realtimeplots){

    ViewCell * view = new ViewCell();
    ViewField * viewfield = new ViewField();
    view->SetComponent(cmp);
    view->DisableWireMarkers();

    viewfield->SetComponent(cmp);
    viewfield->SetSensor(sensor);
    //    viewfield->PlotSurface("e");
    
    ViewDrift * vd = new ViewDrift();

    TCanvas* canvas1 = new TCanvas();
    TCanvas* canvas2 = new TCanvas();
    TCanvas* canvas4 = new TCanvas();

    vd->SetCanvas(canvas2);
    viewfield->SetElectricFieldRange(0,3e5);
    //   viewfield->SetVoltageRange(-2500.0,0.0);
    viewfield->SetNumberOfContours(50);
    viewfield->SetArea(-0.5,-0.5,0.5,0.5);
    viewfield->SetCanvas(canvas4);
    viewfield->PlotProfile(300e-4,-0.5,0,300e-4,0.5,0,"e");
    
    view->SetCanvas(canvas1);
    view->Plot2d();
    canvas1->Update();
    canvas1->Print("View2D.pdf","pdf");
  }
 

  Outfile->Close();
  app->Run(kTRUE);
  
}
