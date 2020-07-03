///
/// \file Pythia6_UserTriggerMultExample.C
/// \brief Configuration of Pythia6 with external trigger on particle multiplicity
///
/// Generate PYTHIA6 events, while the event is generated, check that it has 
/// a given amount of generated particles, defined in UserTrigger() function
///
/// \author Luca Micheletti <luca.micheletti@cern.ch>

//------
/// Implementation of example user event MC trigger.
/// Given a pointer to  generated particle stack, 
/// apply an event selection,  in this example 
/// just the simple counter of generated particles
//-------
Bool_t UserTrigger(AliStack *stack)
{
	printf("____________________________\n");
	printf("USER TRIGGER IMPLEMENTATION \n");

	Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
  
  if ( nTracks > 120 )
  {
    printf("\t accepted!\n");
    return kTRUE;
  }
  else
  {
    printf("\t rejected!\n");
    return kFALSE;
  }
}

//------
/// Main, configure PYTHIA6 events via standard configuration in GeneratorConfig.C
/// adding an external event trigger defined in UserTrigger()
//------
AliGenerator * GeneratorCustom()
{
  AliGenerator * generator = GeneratorPythia6();

  // User trigger
  //Bool_t (*funcUserTrigger)(AliStack*) = UserTrigger;
  //generator->SetUserTrigger(funcUserTrigger);
  
  return generator;
}
