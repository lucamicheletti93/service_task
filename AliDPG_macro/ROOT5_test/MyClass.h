#ifndef MYCLASS_H
#define MYCLASS_H
#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include <string>
#include <vector>

class MyClass : public TObject
{
 public:
   MyClass();
   virtual ~MyClass();

   void SetFuncPtr(Int_t (*funcPtr) (Int_t)) { fFuncPtr = funcPtr; }
   void ApplyFuncPtr();

 private:
   Int_t (*fFuncPtr) (Int_t);

ClassDef(MyClass,1)
};

#endif
