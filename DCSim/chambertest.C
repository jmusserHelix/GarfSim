 {
#include "/home/musser/helix/garfield/Include/MediumMagboltz.hh"
  

  MediumMagboltz* gas = new MediumMagboltz();
  gas->SetComposition("ar", 5.,"co2", 95.);
  //Set temperature [K] and pressure [Torr].
  gas->SetPressure(1 * 760.);
  gas->SetTemperature(293.15);
  gas->SetFieldGrid(100., 100.e3,20,true);
  const int ncoll = 10;
  const bool verbose = true;
  gas->GenerateGasTable(ncoll, verbose);
  cout << " done " << endl;
  gas->WriteGasFile("co2_95_ar_05.gas");
}
