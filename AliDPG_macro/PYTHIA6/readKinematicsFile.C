void readKinematicsFile(){
  Int_t nEvents = 100;
  Int_t nEntry = 0;
  TFile *file = new TFile("Kinematics.root","READ");

  TH1D *histNEvents = new TH1D("histNEvents","",nEvents,0,nEvents);
  TH1D *histMultiplicity = new TH1D("histNEvents","",100,0.,1000.);

  for(int i = 0;i < nEvents;i++){
    nEntry = 0;
    TDirectoryFile *dirFile = (TDirectoryFile*) file -> Get(Form("Event%i",i));
    TTree *tree = (TTree*) dirFile -> Get("TreeK");
    nEntry = tree -> GetEntries();
    if(i < 100){cout << nEntry << endl;}
    histMultiplicity -> Fill(nEntry);
    histNEvents -> SetBinContent(i+1,nEntry);
    dirFile -> Close();
  }

  TCanvas *canvasNEvents = new TCanvas("canvasNEvents","",600,600);
  histNEvents -> Draw("H");

  TCanvas *canvasMultiplicity = new TCanvas("canvasMultiplicity","",600,600);
  histMultiplicity -> Draw("H");
}
