#include <Riostream.h>
#include <string>
#include <vector>

#include "TObject.h"
#include "TSystem.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "MyClass.h"

ClassImp(MyClass)

//______________________________________________________________________________
MyClass::MyClass(): TObject() {
	// default constructor
}
//______________________________________________________________________________
 MyClass::~MyClass() {
	// destructor
}
//______________________________________________________________________________
void MyClass::ApplyFuncPtr() {
	printf("Function pointer = %i \n",fFuncPtr(0));
}
