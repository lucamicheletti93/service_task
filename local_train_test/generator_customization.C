//Bool_t myTrigger(const AliStack &stack)
//{
	//return kTRUE;
//}

//Int_t myTrigger()
//{
	//return 999;
//}

void generator_customization(AliGenerator* generator)
{
	/// myCode
  //gROOT->LoadMacro("/home/luca/cernbox/service_task/DPG/custom_function_test/myTrigger.C+");
  /// myCode


	//void (*foo)(void);
	//foo = &myTrigger;
	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/JEWEL/gen_jewel.sh");
	((AliGenExtFile*) generator)->SetMultiplicityTrigger(kFALSE);




  //std::function<void()> fn = &AliGenExtExec::myTrigger;
	//((AliGenExtFile*) generator)->SetUserTrigger(fn);

	//((AliGenExtFile*) generator)->SetUserTrigger(&myTrigger);

	//((AliGenExtFile*) generator)->SetUserTrigger(&myTrigger);																						// Standard function
	//generator->SetUserTrigger([](const AliStack &stack){ return kTRUE; });														// Lambda function
}
