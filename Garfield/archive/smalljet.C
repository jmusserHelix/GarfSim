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
#include "TMath.h"

using namespace Garfield;

int main(int argc, char * argv[]) {

  char * simoutFile;
  double trackx,trackang;
  int ntrack;
  simoutFile = getenv("DCSimRootfile");
  std::cout << "Output File: "<< simoutFile << std::endl;
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

  //  ifstream options;
  //  options.open("DCsimOptions.txt");
  // char simoutFile[50];
  // double trackx,trackang;
  // int ntrack;
  //  options  >> simoutFile >> trackx >> trackang >> ntrack;

  bool realtimeplots = false;
  int maxclustersize = 100;

  TRint* app = new TRint("Garfield", &argc, argv, 0, 0);
  TH1D *SensewireSig[180];
  TFile * Outfile = new TFile("signalsP0_1800.root","recreate");
  TH1F ncluster("ncluster","ncluster",100,0.,2000.);
  TH1F clustersize("clustersize","clustersize",1000.,0.,100.);

  std::stringstream wid;
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

  DCsim * sim = new DCsim();
  sim->MakeTree(simoutFile);
  
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
 
  SolidBox * enclosure = new SolidBox(0,0,0,9,3.7,10);
  geo->AddSolid(enclosure, gas);
  cmp->SetGeometry(geo);

  const double vCathode = -10000;
  const double rCathode = 175e-4;
  const double vAnode = 0;
  const double rAnode = 25e-4;
  const double vPotential= -1800;
  const double rPotential = 175e-4;
  
  const double anodesep = 0.8;
  const double potentialsep = 0.8;
  const double cathodesep =0.4;
   
  for (int iplane=0;iplane<2;iplane++){
    for(int iw=0;iw<17;iw++){
      float y = 3.2-iw*cathodesep;
      float x = 8-16.0*iplane;
      cmp->AddWire(x,y,2 * rCathode, vCathode, "c");
std::cout << " wire " << x << " " << y << " " << vCathode << " " << "c" << std::endl;
    }
  }
  
  int nwire = 7;
  sim->SetNwires(nwire);
  sim->InitDigitization();
  for (int iplane=0;iplane<1;iplane++){
    for(int iw=0;iw<nwire;iw++){
      wid << "a_" << iplane << "_" << iw;      
      float y = 2.4-iw*anodesep;
      float sign = 1.0;
      if(iw%2==0) sign = -1.0;
      float x = sign*300e-4;
      cmp->AddWire(x,y,2 * rAnode, vAnode, "a");
      sim->SetWirePos(iw+iplane*nwire,x,y);
      std::cout << " wire " << x << " " << y << " " << vAnode << " " << "a" << std::endl;

    }
    for(int iw=0;iw<nwire+1;iw++){
      float y = 2.8-iw*potentialsep;
      float x = 0;
      cmp->AddWire(x,y,2 * rPotential, vPotential, "p");
std::cout << " wire " << x << " " << y << " " << vPotential << " " << "p" << std::endl;
    }
    cmp->AddWire(0,3.2,2 * rPotential, vPotential, "p");
    cmp->AddWire(0,-3.2,2 * rPotential, vPotential, "p");
  }

  //  cmp->AddPlaneX(-25,-10000,"cP"); 
  //  cmp->AddPlaneX(25,-10000,"cP");
  
  float v;
  for (int iplane=0;iplane<2;iplane++){
    for(int iw=0;iw<81;iw++){
      float y = 3.6-7.2*iplane;
      float x = 8-iw*cathodesep/2;
      if(x>=0)
	{ 
	  v=vCathode + (vPotential-vCathode)*(8-x)/8.;
	}
      else 
	{
	  v=vPotential + (vCathode-vPotential)*(-x)/8.;
	}
     
      std::cout << " wire " << x << " " << y << " " << v << " " << "T" << std::endl;
      cmp->AddWire(x,y,2 * rCathode, v, "T");
    }
  }

 
  Sensor * sensor = new Sensor;
  ViewSignal * vs1 = new ViewSignal;
  sensor->AddComponent(cmp);
  sensor->SetTimeWindow(0,2,5000);
  
  cmp->AddReadout("a"); 
  sensor->AddElectrode(cmp,"a");
  vs1->SetSensor(sensor);

  ViewCell * view = new ViewCell();
  ViewField * viewfield = new ViewField();
  view->SetComponent(cmp);
  view->DisableWireMarkers();
  view->SetArea(-9,-4,-1,9,4,1);

  viewfield->SetComponent(cmp);
  viewfield->SetSensor(sensor);
  viewfield->SetArea(-9,-4,9,4);

  //  viewfield->PlotSurface("e");

  ViewDrift * vd = new ViewDrift();
  vd->SetArea(-9,-4,-1,9,4,1);

  int dum;
  
  if(realtimeplots){
    TCanvas* canvas1 = new TCanvas();
    TCanvas * canvas2 = new TCanvas();
    TCanvas* canvas3 = new TCanvas();
    TCanvas* canvas4 = new TCanvas();

    vd->SetCanvas(canvas2);

    viewfield->SetCanvas(canvas4);
    viewfield->PlotContour();
    canvas4->Print("Vcont_1800.pdf","pdf");
    
    vs1->SetCanvas(canvas3);
    
    view->SetCanvas(canvas1);
    view->Plot2d();
    canvas1->Update();
   
  }
  
  //  std::cin >> dum;
  //    DriftLineRKF* driftline = new DriftLineRKF();
  //    DriftLineRKF* driftline_i = new DriftLineRKF();
 
  AvalancheMC * driftline = new AvalancheMC();
  driftline->SetDistanceSteps(0.001);
  driftline->EnableMagneticField();
  driftline->EnableDiffusion();
  driftline->SetSensor(sensor);
  driftline->EnablePlotting(vd);
  //  driftline->EnableSignalCalculation();
  
  AvalancheMC * driftline_i = new AvalancheMC();
  driftline_i->SetDistanceSteps(0.001);
  driftline_i->EnableMagneticField();
  driftline_i->SetSensor(sensor);
  //      driftline_i->EnablePlotting(vd);
  driftline_i->EnableSignalCalculation();
    
  TrackHeed * track = new TrackHeed();
  
  
  for (int itrack=0;itrack<ntrack;itrack++){
    int clustercount=0;
    float trackenergy = 1e9;
    float trackangle = trackang;
    float trackstartX = trackx;
    float trackstartY = 3.5;
    track->SetSensor(sensor);
    track->SetParticle("p");
    track->SetKineticEnergy(trackenergy);
    track->EnablePlotting(vd);

    std::cout << " NEW TRACK STARTED " << itrack << " x: " << trackstartX << std::endl;
   
    double dirx = trackangle/TMath::Sqrt(1+trackangle*trackangle);
    double diry = -1.0/TMath::Sqrt(1+trackangle*trackangle);

    track->NewTrack(trackstartX,trackstartY,0,0,dirx,diry,0);
    
    for (int iplane=0;iplane<1;iplane++){
      for (int iw =0;iw<7;iw++){
	int iadd = iplane*7 + iw;
	SensewireSig[iadd]->Reset();
      }
    }

    double xcl, ycl, zcl, tcl, ecl, extra;
    int ncl;
    double r=0.01;
    double x, y, z, t, e, dx,dy,dz;
    double xendpoint = 0, yendpoint = 0, zendpoint=0;
    double xendpoint2 = 0, yendpoint2 = 0, zendpoint2=0;
    double tendpoint = 0, tendpoint2 = 0;
    
    double dummy = 0;
    int i=0;
    int stat;
    std::cout << "rough # clusters: " << track->GetClusterDensity()*5.6 << std::endl;
    int icluster=0, ielectron=0;
    //  driftline->EnableDebugging();
    
    while (track->GetCluster(xcl, ycl, zcl, tcl, ncl, ecl, extra)){
      std::cout << " new cluster " << icluster << " " << ielectron << " " << xcl << " " << ycl << std::endl;
      clustercount++;

      for(i = 0; i < ncl; i++){
	if(ncl<maxclustersize){
	  
	  clustersize.Fill(ncl);
	  track->GetElectron(i,x,y,z,t,e,dx,dy,dz);
	  std:: cout << " ****  " << i  << " of " << ncl << "  electrons " << x << " " << y << " " << z << " " << t << " " << e << " " << dx << " " << dy << " " << dz << std::endl; 
	  driftline->DriftElectron(x,y,z,0);  
	  int nelectronpoints = driftline->GetNumberOfElectronEndpoints();
	  driftline->GetElectronEndpoint(0, xendpoint, yendpoint, zendpoint, tendpoint, xendpoint2, yendpoint2, zendpoint2, tendpoint2, stat);
	  
	  std::cout << " ion start point "  << xendpoint2 << " " << yendpoint2 << " " << tendpoint2 << std::endl;
	  double angle = RndmGaussian(0,1.4);
	  driftline_i->DriftIon(xendpoint2 + r*sin(angle), yendpoint2 + r*cos(angle), zendpoint2,tendpoint2);
	  
	  int iplane, iw;	
	  iplane=0;
	  iw = (int)((yendpoint2+2.5)/anodesep);
	  
	  if(iw>=0 && iw <7) {
	    int iadd = iw;
	    std::cout << " signal on plane " << iplane << " wire " << iw <<  " " << iadd  << std::endl;
	    vs1->PlotSignal("a");
	    //		vd->Plot();
	    //              canvas2->Update();
	    //		canvas1->Update();
	    if(iadd>=0 && iadd<7){
	      SensewireSig[iadd]->Add(vs1->GetHistogram());
	    }
	  }
	  else std::cout << " invalid wire " << iw << std::endl;
	  sensor->ClearSignal();
	  ielectron++;
	}
	icluster++;
	
	if(ielectron>40000) break;
      }
    }
    for(int iw=0;iw<7;iw++){
      int iadd = iw;
      sim->Digitize(iw,SensewireSig[iadd]);
      int threshcrossing = sim->FirstThreshold(iw);
      sim->DCDataPacket[iw].threshbin = threshcrossing;
      sim->DCDataPacket[iw].Sample.clear();
      std::cout << " wire thresh " << iw << " " << threshcrossing << std::endl;    
      for(int isample=threshcrossing-1;isample<threshcrossing+5;isample++){
	std::cout << " sample " << sim->GetSample(iw,isample) << std::endl;
	sim->DCDataPacket[iw].Sample.push_back(sim->GetSample(iw,isample));
      };
      
      SensewireSig[iadd]->Write();
    }
    ncluster.Fill(clustercount);
    sim->FitLeadingEdges();
    sim->FillTree( trackstartX,trackstartY,trackangle); 
  }

  ncluster.Write();
  clustersize.Write();
  sim->WriteTree();

  Outfile->Close();
  if(realtimeplots){
    //vs1->PlotSignal("a");
    // vd->Plot(true, false);
    // canvas1->Update();
    // canvas1->Print("newtrack.gif");
  }
  app->Run(kTRUE);
  
}
