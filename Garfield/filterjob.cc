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
#include "TRandom3.h"


using namespace Garfield;

int main(int argc, char * argv[]) {
  TRint* app = new TRint("Garfield", &argc, argv, 0, 0);

  char * siminFile;
  char * simoutFile;

  double trackx,trackang;
  double tracky = 3.5;
  int ntrack;
  bool drawsig = false;

  TCanvas* canvas1 = new TCanvas();
  canvas1->cd();

  siminFile = getenv("DCSimRootfile");
  std::cout << "Input File: "<< siminFile << std::endl;

  simoutFile = getenv("DCSimFilterRootfile");
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

  TRandom3 r;
  DCsim * sim = new DCsim(r);
  sim->MakeTree(simoutFile);
  sim->SetNwires(7);
  sim->InitDigitization();

  TH1D *SensewireSig;
  TH1D *UnfilteredVoltageSig;
  TH1D *UnfilteredNoNoiseVoltageSig;
  TH1D *Int1Sig;
  TH1D *Diff1Sig;
  TH1D* Int2Sig;
  TH1D* FinalWaveform;
  TH1D* NoiseWave;
  TH1D* FlatWave;
  TH1D* FinalNoNoiseWaveform;
  
  TFile infile1("rootfiles/Besignal6.root");
  TFile infile2("rootfiles/Besignal7.root");
  TFile infile3("rootfiles/Besignal8.root");
  TFile infile4("rootfiles/Besignal9.root");
  TFile infile5("rootfiles/Besignal10.root");
  TFile infile6("rootfiles/Besignal11.root");
  TFile infile7("rootfiles/Besignal12.root");
  TFile infile8("rootfiles/Besignal13.root");
  TFile infile9("rootfiles/Besignal14.root");
  TFile infile10("rootfiles/Besignal15.root");
  /*
  
  TFile infile1("rootfiles/signal6.root");
  TFile infile2("rootfiles/signal7.root");
  TFile infile3("rootfiles/signal8.root");
  TFile infile4("rootfiles/signal9.root");
  TFile infile5("rootfiles/signal10.root");
  TFile infile6("rootfiles/signal11.root");
  TFile infile7("rootfiles/signal12.root");
  TFile infile8("rootfiles/signal13.root");
  TFile infile9("rootfiles/signal14.root");
  TFile infile10("rootfiles/signal15.root");
  */
  TFile * inf;

  std::stringstream widin;
  std::stringstream widout;
  std::stringstream widout3;

  double int1 = 20;    //ns
  double dif1 = -1;   //ns
  double int2 = 20;    //ns
  double t_F = 20;   //ns

  double i_P = 7e-12; // A/sqrt(nz)
  double e_S = 4e-9; //V/sqrt(hz)
  double kT = 4e-21;   //V-A-s
  double R_w = 1700;   //ohms

  double R_s = e_S*e_S/(4*kT);
  double R_p = 4*kT/(i_P*i_P);
  double R_f = 20000;    //  transimpedence gain
  double C_w = 5e-12;   // F
  double T_noisecorner = C_w*(e_S/i_P);
  double T_det = C_w * R_w;
  double i_Pwire = TMath::Sqrt(4*kT/R_w);
  double ballisticdeficit;
  double a_F1;
  double a_F2;
  double VoutMax;
  double filteredRMSV;
  double filteredSlope;

  double f_z = 1.47e6;  // values here are from jensign.com/noise for LTC 6289, Q=1/sqrt(2)
  double f_p= 2.0e7;
  double f0 = 2.7e7;
  double Ven = 531e-6;
  double Vth = 98e-6;
  double Vin = 339e-6;
  double Vrf = TMath::Sqrt(4*kT*f0*R_f);
  double Vdet = TMath::Sqrt(4*kT*f0/R_w)*R_f;

  std::cout << " " << std::endl; std::cout <<" " <<  std::endl;
  std::cout << "Amp noise parameters:   i_P= " << i_P*1e12 << " pA/sqrt(hz), e_s= " <<  e_S*1e9 << " nV/sqrt(hz)" << std::endl;
  std::cout << "Wire Noise Current= "<<i_Pwire*1e12 << " pA/sqrt(hz)" << std::endl;
  std::cout << "Chamber parameters: C_w=" << C_w*1e12 << " pF,  R_w =" <<R_w << " ohms" << std::endl; 
  std::cout << "Noise Corner Time Const= " << T_noisecorner*1e9 << " ns,  Detector Time Const= " << T_det*1e9 << "ns" << std::endl;
  std::cout << "Series Noise R= "<< R_s << " ohm, Parallel Noise R= " << R_p/1e3 << " kohm, Feedback R= "<<R_f/1e3 << " kohm" << std::endl;

  sim->LoadAmpParameters(R_w,
			 Vrf,
			 Ven,
			 Vth,
			 Vin,
			 Vdet,
			 f_z,
			 f_p,
			 f0 );
			 
  
  TFile * noisefile = new TFile("NoiseStudy.root","RECREATE");
  TTree * Noisetree = new TTree("t","");
  Noisetree->Branch("baldef",&ballisticdeficit,"baddef/d");
  Noisetree->Branch("a_F1",&a_F1,"a_F1/d");
  Noisetree->Branch("a_F2",&a_F2,"a_F2/d");
  Noisetree->Branch("int1",&int1,"int1/d");
  Noisetree->Branch("dif1",&dif1,"dif1/d");
  Noisetree->Branch("int2",&int2,"int2/d");
  Noisetree->Branch("T_f",&t_F,"T_f/d");
  Noisetree->Branch("T_C",&T_noisecorner,"T_C/d");
  Noisetree->Branch("T_D",&T_det,"T_D/d");
  Noisetree->Branch("R_w",&R_w,"R_w/d");
  Noisetree->Branch("C_s",&C_w,"C_w/d");
  Noisetree->Branch("R_s",&R_s,"R_s/d");
  Noisetree->Branch("R_p",&R_p,"R_p/d");
  Noisetree->Branch("e_S",&e_S,"e_S/d");
  Noisetree->Branch("i_P",&i_P,"i_P/d");
  Noisetree->Branch("VoutMax",&VoutMax, "VoutMax/d");
  Noisetree->Branch("RMSv",&filteredRMSV, "RMSv/d");
  Noisetree->Branch("slope",&filteredSlope, "slope/d");

  TH1D* ImpulseSig = new TH1D("impulse","Impulse",5000,0,10000);
  UnfilteredVoltageSig = new TH1D("AmpOut","AmpOut",5000,0,10000);
  //  UnfilteredNoiseVoltageSig = new TH1D("NoiseVolt","NoiseVolt",5000,0,10000);
  Int1Sig = new TH1D("Int1Sig","Int1Sig",5000,0,10000);
  Diff1Sig = new TH1D("Diff1Sig","Diff1Sig",5000,0,10000);
  Int2Sig = new TH1D("Int2Sig","Int2Sig",5000,0,10000);
  FinalWaveform = new TH1D("FinalWaveform","FinalWaveform",5000,0,10000);
  NoiseWave = new TH1D("NoiseWave","NoiseWave",5000,0,10.0e-6);
  FinalNoNoiseWaveform = new TH1D("FinalNoNoiseWaveform","FinalNoNoiseWaveform",5000,0,10000);
  UnfilteredNoNoiseVoltageSig = new TH1D("NoNoiseVolt","NoNoiseVolt",5000,0,10000);
  FlatWave = new TH1D("FlatWave","FlatWave",5000,0,10.0e-6);
  FlatWave->Reset();

  for (int1 =20; int1<=20 ; int1+=10){
    
    for (dif1 =0; dif1<=0; dif1+=50){
      
      for (int2 =20; int2<=20; int2+=10){

  // amplifier parameters

	// generate impulse response for filter weighting
 	
	ImpulseSig->SetBinContent(2000,1);
	sim->Filter(ImpulseSig,Int1Sig,Diff1Sig,Int2Sig,int1,dif1,int2);
	if(drawsig){
	  sim->DCsimOutFile()->cd();
	  ImpulseSig->Write();
	  Int1Sig->Write();
	  Diff1Sig->Write();
	  Int2Sig->Write();
	}
	a_F1 = sim->GetWeightingIntegralI1(Int2Sig,t_F);
	a_F2 = sim->GetWeightingIntegralI2(Int2Sig,t_F);
	std::cout << " Shaper Time Constants:  Int1= "<< int1 << " ns, Diff= " << dif1 << " ns, Int2= " << int2 << " ns" << std::endl; 
	std::cout << "Impulse Weighting Function Parameters:  a_F1= "<<a_F1 << ", a_F2= " << a_F2 << " T_f= "<<  t_F << " ns " << std::endl;
	std::cout << " " <<  std::endl; std::cout << " " <<std::endl;
	

	// loop over tracks and wire signals, obtained signal waveforms starting from raw
	// which are generated by Garfield.

	double bd_ave = 0;
	double nbd=0;
	double Vmax_ave=0;
	double nwaveform=0;
	double nslope=0;
	double filteredRMSVave=0;
	double filteredslopeave=0;
	double nfile=10;
	for (int ifile=0;ifile<nfile;ifile++){
	  if(ifile==0)inf=&infile1;
	  if(ifile==1)inf=&infile2;
	  if(ifile==2)inf=&infile3;
	  if(ifile==3)inf=&infile4;
	  if(ifile==4)inf=&infile5;
	  if(ifile==5)inf=&infile6;
	  if(ifile==6)inf=&infile7;
	  if(ifile==7)inf=&infile8;
	  if(ifile==8)inf=&infile9;
	  if(ifile==9)inf=&infile10;

	  for(int itrack=1;itrack<ntrack;itrack++){
	    for(int iw = 0;iw<7;iw++){
	      inf->cd();
	      int iadd = iw;
	      int iplane=0;
	      
	      widin.str("");
	      widin << "a_" <<iplane << "_" << iw <<";" << itrack;	    
	      std::string strin(widin.str());
	      const char * namein = strin.c_str();
	      std::cout << "new raw waveform:   " << namein << std::endl;
	      
	      if(drawsig){
		
		widout.str("");
		widout << "I1_" << int1 << "D1_" << dif1 << "I2_" << int2 <<  "a_" <<iplane << "_" << iw <<"AmpOut";
		widout3.str("");
		widout3 << "I1_" << int1 << "D1_" << dif1 << "I2_" << int2 <<  "a_" <<iplane << "_" << iw <<"Filtered";
		
		std::string strout(widout.str());
		const char * nameout = strout.c_str();
		//	    std::cout << iadd << " " << nameout << std::endl;
		
		std::string strout3(widout3.str());
		const char * nameout3 = strout3.c_str();
		//  std::cout << iadd << " " << nameout3 << std::endl;
		
		UnfilteredVoltageSig = new TH1D(nameout,nameout,5000,0,10000);
		FinalWaveform = new TH1D(nameout3,nameout3,5000,0,10000);
	      }
	      
	      inf->GetObject(namein,SensewireSig);
	      SensewireSig->Print();
	      
	      sim->CreateNoise(NoiseWave);
	      sim->ApplyGainandNoise(SensewireSig,NoiseWave, UnfilteredVoltageSig);
	      sim->Filter(UnfilteredVoltageSig,Int1Sig,Diff1Sig,FinalWaveform,int1,dif1,int2);
	      
	      //  repeat without noise to get clean rising edge slope val
	      sim->ApplyGainandNoise(SensewireSig,FlatWave,UnfilteredNoNoiseVoltageSig);
	      sim->Filter(UnfilteredNoNoiseVoltageSig,Int1Sig,Diff1Sig,FinalNoNoiseWaveform,int1,dif1,int2);
	      
	      bd_ave +=sim->GetBallisticDeficit(UnfilteredVoltageSig,FinalWaveform);
	      Vmax_ave += FinalWaveform->GetBinContent(FinalWaveform->GetMinimumBin());
	      
	      nbd++;
	      nwaveform++;
	      
	      if(drawsig){
		sim->DCsimOutFile()->cd();
		SensewireSig->Write();
		UnfilteredVoltageSig->Write();
		FinalWaveform->Write();
	      }
	      inf->cd();
	      
	      sim->Digitize(iw, FinalWaveform); 
	      
	      int threshbin = sim->FirstThreshold(iw);
	      int prethreshbin = sim->FirstThresholdPreDig(FinalNoNoiseWaveform);
	      
	      sim->DCDataPacket[iw].threshbin = threshbin;
	      sim->DCDataPacket[iw].Sample.clear();
	      
	      if(threshbin>0) {
		std::cout << " wire thresh " << iw << " " << threshbin <<  " " << prethreshbin << std::endl;   

		// advance threshold up to three bins, such that first fit point
		// is first sample above baseline.

		if(sim->GetSample(iw,threshbin-3)==0)threshbin++;
		if(sim->GetSample(iw,threshbin-3)==0)threshbin++;
		if(sim->GetSample(iw,threshbin-3)==0)threshbin++;

		for(int isample=threshbin-3;isample<threshbin-3+sim->GetnSamples();isample++){
		  std::cout << " sample " << sim->GetSample(iw,isample) << std::endl;
		  sim->DCDataPacket[iw].Sample.push_back(sim->GetSample(iw,isample));
		}    
		if(prethreshbin>350){
		  double thisrms = sim->GetPreSampleRMS(FinalWaveform, 1 ,300);
		  filteredRMSVave += thisrms;
		  filteredslopeave += sim->GetLeadingEdgeSlope(FinalNoNoiseWaveform,thisrms,prethreshbin,40);
		  nslope++;
		}
	      }
	      else{
		std::cout << " No Threshold Crossing! " << std::endl;    
		for(int isample=threshbin-1;isample<threshbin+5;isample++){
		  sim->DCDataPacket[iw].Sample.push_back(0);
		}    
	      }
	      
	    }
	    sim->FitLeadingEdges();
	    sim->FillTree( trackx,tracky,trackang);
	  }
	}
	ballisticdeficit = bd_ave/nbd;
	VoutMax = Vmax_ave/nwaveform;
	filteredRMSV= filteredRMSVave/nslope;
	filteredSlope =filteredslopeave/nslope;

	std::cout << " Ballistic Deficit:  " << ballisticdeficit << std::endl;
	std::cout << " Average Vmax:  " << VoutMax << " Volts " << std::endl;

	std::cout << " RMS Noise:  " << filteredRMSV*1e3 << " mV " << std::endl;
	std::cout << " Leading Edge Slope:  " << filteredSlope*1e3 << " mV/ns " << std::endl;

	noisefile->cd();
	Noisetree->Fill();
	
      }
    }
  }

  sim->DCsimOutFile()->cd();
  sim->WriteTree();
  sim->DCsimOutFile()->Close(); 
  noisefile->cd();
  Noisetree->Write();
  noisefile->Close();

 }
