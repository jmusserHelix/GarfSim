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

#include "Plotting.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  TRint* app = new TRint("Garfield", &argc, argv, 0, 0);


  TH1D *SensewireSig[180];

  std::stringstream wid;
  for (int iplane=0;iplane<3;iplane++){
    for (int iw =0;iw<60;iw++){
      int iadd = iplane*60 + iw;
      wid.str("");
      wid << "a_" <<iplane << "_" << iw ;
      std::cout << iadd << " " << wid.str() << std::endl;
      std::string str(wid.str());
      const char * name = str.c_str();
      SensewireSig[iadd] = new TH1D(name,name,1500,0,15000);
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
 
  SolidBox * enclosure = new SolidBox(0,0,0,25,25,25);
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
 
  for (int iplane=0;iplane<4;iplane++){
    for(int iw=0;iw<121;iw++){
      float y = 24-iw*cathodesep;
      float x = 24-16.0*iplane;
      cmp->AddWire(x,y,2 * rCathode, vCathode, "c");
std::cout << " wire " << x << " " << y << " " << vCathode << " " << "c" << std::endl;
    }
  }
 
  for (int iplane=0;iplane<3;iplane++){
    for(int iw=0;iw<60;iw++){
      wid << "a_" << iplane << "_" << iw;
      
      float y = 23.6-iw*anodesep;
      float sign = 1.0;
      if(iw%2==0) sign = -1.0;
      float x = 16-iplane*16+sign*300e-4;
      cmp->AddWire(x,y,2 * rAnode, vAnode, "a");
      std::cout << " wire " << x << " " << y << " " << vAnode << " " << "a" << std::endl;

    }
    for(int iw=0;iw<61;iw++){
      float y = 24-iw*potentialsep;
      float x = 16-iplane*16;
      cmp->AddWire(x,y,2 * rPotential, vPotential, "p");
std::cout << " wire " << x << " " << y << " " << vPotential << " " << "p" << std::endl;
    }
  }

  cmp->AddPlaneX(-25,-10000,"cP"); 
  cmp->AddPlaneX(25,-10000,"cP");
  
  float v;
  for (int iplane=0;iplane<2;iplane++){
    for(int iw=0;iw<241;iw++){
      float y = 24.5-49*iplane;
      float x = 24-iw*cathodesep/2;
      if(x>16)
	{ 
	  v=vCathode + (vPotential-vCathode)*(24-x)/8.;
	}
      else if(x<=16 && x>8) 
	{
	  v=vPotential + (vCathode-vPotential)*(16-x)/8.;
	}
      else if(x<=8 && x>0)
	{ 
	  v=vCathode + (vPotential-vCathode)*(8-x)/8.;
	}
      else if(x<=0 && x>-8) 
	{
	  v=vPotential + (vCathode-vPotential)*(-x)/8.;
	}
      else if(x<=-8 && x>-16)
	{ 
	  v=vCathode + (vPotential-vCathode)*(-8-x)/8.;
	}
      else if(x<=-16 && x>-24) 
	{
	  v=vPotential + (vCathode-vPotential)*(-16-x)/8.;
	}
      std::cout << " wire " << x << " " << y << " " << v << " " << "T" << std::endl;
      cmp->AddWire(x,y,2 * rCathode, v, "T");
    }
  }

  TCanvas* canvas1 = new TCanvas();
  TCanvas * canvas2 = new TCanvas();
  TCanvas* canvas3 = new TCanvas();
  TCanvas* canvas4 = new TCanvas();

  Sensor * sensor = new Sensor;
  ViewSignal * vs1 = new ViewSignal;
  sensor->AddComponent(cmp);
  sensor->SetTimeWindow(0,10,1500);
  
  cmp->AddReadout("a"); 
  sensor->AddElectrode(cmp,"a");
  vs1->SetSensor(sensor);

  ViewCell * view = new ViewCell();
  ViewField * viewfield = new ViewField();
  view->SetComponent(cmp);
  view->DisableWireMarkers();
  view->SetArea(-17,-25,-1,-8,25,1);

  viewfield->SetComponent(cmp);
  viewfield->SetSensor(sensor);
  viewfield->SetArea(-26,-26,26,26);

  //  viewfield->PlotSurface("e");
  //  int dum;
  // std::cin >> dum;

  viewfield->SetCanvas(canvas4);
  viewfield->PlotContour();
  canvas4->Print("Vcont_2000.pdf","pdf");
   
  vs1->SetCanvas(canvas2);

  view->SetCanvas(canvas1);
  view->Plot2d();
  canvas1->Update();
   
  ViewDrift * vd = new ViewDrift();
  vd->SetArea(-17,-25,-1,-8,25,1);
  vd->SetCanvas(canvas1);

  //    DriftLineRKF* driftline = new DriftLineRKF();

  //    DriftLineRKF* driftline_i = new DriftLineRKF();

 
      AvalancheMC * driftline = new AvalancheMC();
      driftline->SetDistanceSteps(0.002);
      driftline->EnableMagneticField();
      driftline->EnableDiffusion();
      driftline->SetSensor(sensor);
      driftline->EnablePlotting(vd);
  //  driftline->EnableSignalCalculation();

      AvalancheMC * driftline_i = new AvalancheMC();
      driftline_i->SetDistanceSteps(0.002);
      driftline_i->EnableMagneticField();
      driftline_i->SetSensor(sensor);
      //      driftline_i->EnablePlotting(vd);
      driftline_i->EnableSignalCalculation();
    
  TrackHeed * track = new TrackHeed();
  float trackenergy = 5e8;
  float trackangle = 0.0;
  float trackstartX = -10;
  float trackstartY = 24;
  track->SetSensor(sensor);
  track->SetParticle("proton");
  track->SetKineticEnergy(trackenergy);
  track->EnablePlotting(vd);
  track->NewTrack(trackstartX,trackstartY,0,0,trackangle,-(1-trackangle*trackangle),0);


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
  std::cout << "rough # clusters: " << track->GetClusterDensity()*50 << std::endl;
  int icluster=0, ielectron=0;
  //  driftline->EnableDebugging();

  while (track->GetCluster(xcl, ycl, zcl, tcl, ncl, ecl, extra)){
    std::cout << " new cluster " << icluster << " " << ielectron << " " << xcl << " " << ycl << std::endl;
    for(i = 0; i < ncl; i++){
  
      track->GetElectron(i,x,y,z,t,e,dx,dy,dz);
      std:: cout << " ****  " << i  << " of " << ncl << "  electrons " << x << " " << y << " " << z << " " << t << " " << e << " " << dx << " " << dy << " " << dz << std::endl; 
      driftline->DriftElectron(x,y,z,0);  
      int nelectronpoints = driftline->GetNumberOfElectronEndpoints();
      driftline->GetElectronEndpoint(0, xendpoint, yendpoint, zendpoint, tendpoint, xendpoint2, yendpoint2, zendpoint2, tendpoint2, stat);

      std::cout << " ion start point "  << xendpoint2 << " " << yendpoint2 << " " << tendpoint2 << std::endl;
      double angle = RndmGaussian(0,1.4);
      driftline_i->DriftIon(xendpoint2 + r*sin(angle), yendpoint2 + r*cos(angle), zendpoint2,tendpoint2);

      int iplane, iw;
      if(xendpoint2 < -4) {
	iplane=0;
      }
      else if (xendpoint2>4){
	iplane = 2;
	
      }
      else{
	iplane = 1;
      }

      iw = (int)(yendpoint+23.7)/anodesep;

      if(iw>=0 && iw <60) {
 
	int iadd = iplane*60 + iw;
	std::cout << " signal on plane " << iplane << " wire " << iw <<  " " << iadd  << std::endl;
		vs1->PlotSignal("a");
		//		vd->Plot();
                canvas2->Update();
		//		canvas1->Update();
		if(iadd>=0 && iadd<180){
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

  TFile * Outfile = new TFile("signalsP0_2000.root","recreate");
  for (int iplane=0;iplane<3;iplane++){
    for(int iw=0;iw<60;iw++){
      int iadd = iplane*48 + iw;
      SensewireSig[iadd]->Write();
    }
  }
  
  Outfile->Close();
  //vs1->PlotSignal("a");
  // vd->Plot(true, false);
  // canvas1->Update();
  // canvas1->Print("newtrack.gif");
 
  app->Run(kTRUE);
  
}
