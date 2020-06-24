Bool_t userPtTrigger(AliStack *stack){
  Int_t partCounter = 0;                                                        // counter of particles with pT > pT threshold
  Int_t nTracks = stack->GetNtrack();
  for(int i = 0;i < nTracks;i++){
    TParticle *track = (TParticle*) stack->Particle(i);
    if(track->Pt() > 8.){                         // Selecting events with D0 mesons & pT > 4 GeV/c
      printf("   pT = %f GeV/c \n",track->Pt());
      printf("   PDG = %i \n",track->GetPdgCode());
      partCounter++;
    }
  }
  if(partCounter > 0){return kTRUE;}
  else{return kFALSE;}
}

void generator_customization(AliGenerator* generator)
{
  ///---------------------------------------------------------------------------
  // Trigger : standard c++ function
  //std::function<Bool_t(AliStack*)> funcUserTrigger = userPtTrigger;
  // Trigger : lambda expression
  Int_t nTracks_cut = 70;
  std::function<Bool_t(AliStack*)> funcUserTrigger = [nTracks_cut](AliStack *stack) -> Bool_t { return stack->GetNtrack() > nTracks_cut; };

  generator->SetUserTrigger(funcUserTrigger);
  ///---------------------------------------------------------------------------
}
