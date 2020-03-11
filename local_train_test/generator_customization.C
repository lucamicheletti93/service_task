Bool_t myTrigger(AliStack *stack)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("TRIGGER SETTATO ESTERNAMENTE \n");
	Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
  if(nTracks>100){return kTRUE;}
  else{
    printf("n Tracks < %i --> EVENT DISCARDED \n",100);
    return kFALSE;
  }
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

void generator_customization(AliGenerator* generator)
{
	/// myCode
  //gROOT->LoadMacro("/home/luca/GITHUB/service_task/local_train_test/myTrigger.C+");
  /// myCode

	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/JEWEL/gen_jewel.sh");
	//((AliGenExtFile*) generator)->SetMultiplicityTrigger(100);
	//((AliGenExtFile*) generator)->SetPtTrigger(5.);

	std::function<Bool_t(AliStack*)> funcMyTrigger = myTrigger;
	((AliGenExtFile*) generator)->SetUserTrigger(funcMyTrigger);
}
