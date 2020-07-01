Bool_t userTrigger(AliStack *stack)
{
	printf("____________________________\n");
	printf("USER TRIGGER IMPLEMENTATION \n");

	Int_t nTracks  = stack->GetNtrack();
  	printf("n Tracks = %i \n",nTracks);
	printf("____________________________\n");
  	if(nTracks>0){return kTRUE;}
  	else{return kFALSE;}
}

void generator_customization(AliGenerator* generator){
	((AliGenExtExec*) generator)->SetPathScript("$ALICE_PHYSICS/PWG/MCLEGO/CRMC/gen_eposlhc.sh");
}
