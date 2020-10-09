void compare_test_outputs(){
  TFile *file_mult_cut = new TFile("/home/luca/GITHUB/service_task/MyTaskTest/example_mult_cut/AnalysisResults.root","READ");
  TDirectoryFile *dir_mult_cut = (TDirectoryFile*) file_mult_cut -> Get("MyTask");
  TList *list_mult_cut = (TList*) dir_mult_cut -> Get("MyOutputContainer");
  TH1F *hist_mult_cut = (TH1F*) list_mult_cut -> FindObject("fHistMult");

  TCanvas *canvas_mult_cut = new TCanvas("canvas_mult_cut","",600,600);
  hist_mult_cut -> Draw();



  TFile *file_no_mult_cut = new TFile("/home/luca/GITHUB/service_task/MyTaskTest/example_no_mult_cut/AnalysisResults.root","READ");
  TDirectoryFile *dir_no_mult_cut = (TDirectoryFile*) file_no_mult_cut -> Get("MyTask");
  TList *list_no_mult_cut = (TList*) dir_no_mult_cut -> Get("MyOutputContainer");
  TH1F *hist_no_mult_cut = (TH1F*) list_no_mult_cut -> FindObject("fHistMult");

  TCanvas *canvas_no_mult_cut = new TCanvas("canvas_no_mult_cut","",600,600);
  hist_no_mult_cut -> Draw();


  // test Jochen : cut applied directly inside AliGenExtFile.cxx
  TFile *file_test_Jochen = new TFile("/home/luca/GITHUB/service_task/MyTaskTest/test_Jochen/AnalysisResults.root","READ");
  TDirectoryFile *dir_test_Jochen = (TDirectoryFile*) file_test_Jochen -> Get("MyTask");
  TList *list_test_Jochen = (TList*) dir_test_Jochen -> Get("MyOutputContainer");
  TH1F *hist_test_Jochen = (TH1F*) list_test_Jochen -> FindObject("fHistMult");
  hist_test_Jochen -> SetMarkerStyle(20); hist_test_Jochen -> SetMarkerColor(kBlack); hist_test_Jochen -> SetLineColor(kBlack);

  TLegend *legend_test_Jochen = new TLegend(0.15,0.5,0.35,0.7); legend_test_Jochen -> SetTextSize(0.03); legend_test_Jochen -> SetBorderSize(0);
  legend_test_Jochen -> AddEntry(hist_mult_cut,"Trigger set externally","F");
  legend_test_Jochen -> AddEntry(hist_test_Jochen,"Trigger set in AliRoot","P");

  TCanvas *canvas_test_Jochen = new TCanvas("canvas_test_Jochen","",600,600);
  hist_mult_cut -> Draw();
  hist_test_Jochen -> Draw("Psame");
  legend_test_Jochen -> Draw("same");
}
