Bool_t userTrigger(AliStack *stack)
{
	printf("____________________________\n");
	printf("USER TRIGGER IMPLEMENTATION \n");
	Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
	printf("____________________________\n");
  if(nTracks>100){return kTRUE;}
  else{return kFALSE;}
}

void generator_customization(AliGenerator* generator)
{
	/// myCode
  //gROOT->LoadMacro("/home/luca/GITHUB/service_task/local_train_test/userTrigger.C+");
  /// myCode

	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/JEWEL/gen_jewel.sh");
	//((AliGenExtFile*) generator)->SetMultiplicityTrigger(100);
	//((AliGenExtFile*) generator)->SetPtTrigger(5.);

	std::function<Bool_t(AliStack*)> funcUserTrigger = userTrigger;
	((AliGenExtFile*) generator)->SetUserTrigger(funcUserTrigger);
}
