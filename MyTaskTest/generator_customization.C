Bool_t userTrigger(AliStack *stack)
{
	printf("____________________________\n");
	printf("USER TRIGGER IMPLEMENTATION \n");

	Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
	printf("____________________________\n");
  if(nTracks>0){return kTRUE;}
  else{return kFALSE;}

	/*
	Int_t nTracks  = stack->GetNtrack();

  Int_t partCounter = 0;                                                        // counter of particles with pT > pT threshold
  for(int i = 0;i < nTracks;i++){
    TParticle *track = (TParticle*) stack->Particle(i);
    if(track->Pt() > 5.){
      printf("pT = %f \n",track->Pt());
      partCounter++;
    }
  }

  if(partCounter>0){return kTRUE;}
  else{
    printf("No particle with pT > %f GeV/c --> EVENT DISCARDED \n",5.);
    return kFALSE;
  }
	// è giusto che ci siano particelle con pT < 5 GeV/c perchè l'evento viene salvato nel momento in cui almeno una particella ha pT maggiore della soglia
	*/
}

void generator_customization(AliGenerator* generator)
{
	/// myCode
  //gROOT->LoadMacro("/home/luca/GITHUB/service_task/local_train_test/userTrigger.C+");
  /// myCode

	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/JEWEL/gen_jewel.sh");
	//((AliGenExtFile*) generator)->SetMultiplicityTrigger(100);
	//((AliGenExtFile*) generator)->SetPtTrigger(5.);

	// Implementation with the standard std::function
	//std::function<Bool_t(AliStack*)> funcUserTrigger = userTrigger;
	//((AliGenExtFile*) generator)->SetUserTrigger(funcUserTrigger);


	Int_t nTracks_cut = 100;

	// Implementation with lambda function - Long
  //std::function<Bool_t(AliStack*)> funcUserTrigger = [nTracks_cut](AliStack *stack) -> Bool_t {
    //Int_t nTracks = stack->GetNtrack();
    //printf("n Tracks = %i \n",nTracks);
    //if(nTracks> nTracks_cut ){return kTRUE;}
    //else{return kFALSE;}
  //};

	// Implementation with lambda function Short
	std::function<Bool_t(AliStack*)> funcUserTrigger = [nTracks_cut](AliStack *stack) -> Bool_t { return stack->GetNtrack() > nTracks_cut; };


  ((AliGenExtFile*) generator)->SetUserTrigger(funcUserTrigger);
}
