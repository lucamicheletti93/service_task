///
/// \file EPOS_UserTrigger.C
/// \brief Configuration of EPOSLHC with external trigger user defined
///
/// Generate EPOSLHC events, while the event is generated, check that
/// the user trigger conditions on the event are fulfilled. The user trigger
/// must be defined in macro UserTrigger.C
///
/// \author Luca Micheletti <luca.micheletti@cern.ch>
/// \author Gustavo Conesa Balbastre <gustavo.conesa.balbastre@cern.ch>

#if !defined(__CINT__) || defined(__MAKECINT__)
 #include "UserTrigger.C"
#endif

//------
/// Main, configure EPOSLHC events via standard configuration in GeneratorConfig.C
/// adding an external event trigger defined in UserTrigger.C
//------
AliGenerator * GeneratorCustom()
{
  AliGenerator * generator = GeneratorEPOSLHC();

#if defined(__CINT__)
  gROOT->LoadMacro("UserTrigger.C+");
#endif

  UserTrigger(generator); 

  return generator;
}
