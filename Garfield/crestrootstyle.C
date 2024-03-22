

void crestrootstyle()
{
  printf("Welcome to the ROOT of all evils \n");

  // Defaults to classic style, but that's OK, we can fix it
  TStyle* CRESTStyle = new TStyle("CRESTStyle", "CREST Style");

  // Centre title
  CRESTStyle->SetTitleAlign(22);
  CRESTStyle->SetTitleX(.5);
  CRESTStyle->SetTitleY(.95);
  CRESTStyle->SetTitleBorderSize(0);

  // No info box
  CRESTStyle->SetOptStat(0);

  //set the background color to white
  CRESTStyle->SetFillColor(10);
  CRESTStyle->SetFrameFillColor(10);
  CRESTStyle->SetCanvasColor(10);
  CRESTStyle->SetPadColor(10);
  CRESTStyle->SetTitleFillColor(0);
  CRESTStyle->SetStatColor(10);

  // Don't put a colored frame around the plots
  CRESTStyle->SetFrameBorderMode(0);
  CRESTStyle->SetCanvasBorderMode(0);
  CRESTStyle->SetPadBorderMode(0);

  // Set the default line color for a fit function to be red
  CRESTStyle->SetFuncColor(kRed);

  // Marker settings
   CRESTStyle->SetMarkerStyle(kFullCircle);

  // No border on legends
  CRESTStyle->SetLegendBorderSize(0);

  // Disabled for violating CREST style guidelines
  // Scientific notation on axes
  //  TGaxis::SetMaxDigits(3);

  // Axis titles
  CRESTStyle->SetTitleSize(.055, "xyz");
  CRESTStyle->SetTitleOffset(.8, "xyz");
  // More space for y-axis to avoid clashing with big numbers
  CRESTStyle->SetTitleOffset(.9, "y");
  // This applies the same settings to the overall plot title
  CRESTStyle->SetTitleSize(.055, "");
  CRESTStyle->SetTitleOffset(.8, "");
  // Axis labels (numbering)
  CRESTStyle->SetLabelSize(.04, "xyz");
  CRESTStyle->SetLabelOffset(.005, "xyz");

  // Prevent ROOT from occasionally automatically zero-suppressing
  CRESTStyle->SetHistMinimumZero();

  // Thicker lines
  CRESTStyle->SetHistLineWidth(2);
  CRESTStyle->SetFrameLineWidth(2);
  CRESTStyle->SetFuncWidth(2);

  // Set the number of tick marks to show
  CRESTStyle->SetNdivisions(506, "xyz");

  // Set the tick mark style
  CRESTStyle->SetPadTickX(1);
  CRESTStyle->SetPadTickY(1);

  // Fonts
  const int kCRESTFont = 42;
  CRESTStyle->SetStatFont(kCRESTFont);
  CRESTStyle->SetLabelFont(kCRESTFont, "xyz");
  CRESTStyle->SetTitleFont(kCRESTFont, "xyz");
  CRESTStyle->SetTitleFont(kCRESTFont, ""); // Apply same setting to plot titles
  CRESTStyle->SetTextFont(kCRESTFont);
  //  CRESTStyle->SetLegendFont(kCRESTFont);

  // Get moodier colours for colz
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  CRESTStyle->SetNumberContours(NCont);

  gROOT->SetStyle("CRESTStyle");

  // Uncomment this line if you want to force all plots loaded from files
  // to use this same style
  //gROOT->ForceStyle();
}

// Put a "CREST Preliminary" tag in the corner
void Preliminary()
{
  TLatex* prelim = new TLatex(.9, .95, "CREST Preliminary");
  prelim->SetTextColor(kBlue);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAlign(32);
  prelim->Draw();
}

// Put a "CREST Simulation" tag in the corner
void Simulation()
{
  TLatex* prelim = new TLatex(.9, .95, "CREST Simulation");
  prelim->SetTextColor(kGray+1);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAlign(32);
  prelim->Draw();
}

void CenterTitles(TH1* histo)
{
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->CenterTitle();
  histo->GetZaxis()->CenterTitle();  
}
