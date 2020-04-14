void generator_customization(AliGenerator* generator)
{
  ///---------------------------------------------------------------------------
  Int_t nTracks_cut = 100;
  std::function<Bool_t(AliStack*)> funcUserTrigger = [nTracks_cut](AliStack *stack) -> Bool_t { return stack->GetNtrack() > nTracks_cut; };
  generator->SetUserTrigger(funcUserTrigger);
  ///---------------------------------------------------------------------------
}
