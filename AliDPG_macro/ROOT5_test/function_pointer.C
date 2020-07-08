#include "MyClass.h"

Int_t myFunction(Int_t x){
	x = 0;
	if(x == 0){return kTRUE;}
	else return kFALSE;
}

void function_pointer(){
	Int_t (*funcPtr)(Int_t) = myFunction;
	MyClass *myclass = new MyClass();
	myclass -> SetFuncPtr(funcPtr);
	myclass -> ApplyFuncPtr();
	//printf("%i \n",funcPtr(0));
}
