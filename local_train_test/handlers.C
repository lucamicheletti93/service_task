#ifdef __CLING__
R__ADD_INCLUDE_PATH($ALICE_ROOT)
#include "ANALYSIS/macros/train/AddMCGeneratorHandler.C"
#endif

void handlers()
{
  {
    #ifndef __CLING__
    gROOT->LoadMacro(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/train/AddMCGeneratorHandler.C"));
    #endif
    AliVEventHandler* handler = AddMCGeneratorHandler();
AliESDEvent *esdE = new AliESDEvent();
  esdE->CreateStdContent();
  // esdE->SetBeamType("p-p");
  // esdE->GetESDRun()->GetBeamType();
  // esdE->GetESDRun()->Print();

  AliESDVertex *vtx = new AliESDVertex(0.,0.,100);
  vtx->SetName("VertexTracks");
  vtx->SetTitle("VertexTracks");
  esdE->SetPrimaryVertexTracks(vtx);
  AliAnalysisManager *mgr =     AliAnalysisManager::GetAnalysisManager();
  AliDummyHandler *dumH =   static_cast<AliDummyHandler*>(mgr->GetInputEventHandler());
  dumH->SetEvent(esdE);
  // esdE->GetList()->ls();
  }
}
