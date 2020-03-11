///
//#include <functional>
///

//Bool_t myTrigger(const AliStack &stack)
//{
	//return kTRUE;
//}

//Int_t myTrigger()
//{
	//return 999;
//}

//Int_t myTrigger(Int_t num){
	//printf("TRIGGER SET EXTERNALLY! \n");
	//return num + 999;
//}

void myTrigger(AliStack *stack)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("TRIGGER SETTATO ESTERNAMENTE \n");
	printf("N Tracks = %i \n",stack->GetNtrack());
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

void generator_customization(AliGenerator* generator)
{
	/// myCode
  //gROOT->LoadMacro("/home/luca/GITHUB/service_task/local_train_test/myTrigger.C+");
  /// myCode


	//void (*foo)(void);
	//foo = &myTrigger;
	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/JEWEL/gen_jewel.sh");
	((AliGenExtFile*) generator)->SetMultiplicityTrigger(kTRUE);

	//((AliGenExtFile*) generator)->SetUserTrigger(&myTrigger);

	std::function<void(AliStack*)> f_myTrigger = myTrigger;
	((AliGenExtFile*) generator)->SetUserTrigger(f_myTrigger);


  //std::function<void()> fn = &AliGenExtExec::myTrigger;
	//((AliGenExtFile*) generator)->SetUserTrigger(fn);

	//((AliGenExtFile*) generator)->SetUserTrigger(&myTrigger);

	//((AliGenExtFile*) generator)->SetUserTrigger(&myTrigger);																						// Standard function
	//generator->SetUserTrigger([](const AliStack &stack){ return kTRUE; });														// Lambda function
}
