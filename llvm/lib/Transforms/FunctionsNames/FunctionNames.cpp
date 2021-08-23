#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/CallSite.h"
//#include "llvm/IR/Instruction.h"

//большинство функци будем использовать из пространства имён llvm
using namespace llvm;
// анонимное пространство имён
namespace {
  //struct FunctionsNames : public FunctionPass {
  class FunctionsNames : public FunctionPass {
  public:
    static char ID;
    FunctionsNames() : FunctionPass(ID) {}
// переопределяем абстрактный метод, наследуемый от FunctionPass
    bool runOnFunction(Function &F) override {
      // Print name of function
      outs() << "*";
      outs().write_escaped(F.getName()) << '\n';

      for (auto& B : F) {    // Iterate over each Basic Blocks in Functions
        for (auto& I : B) {  // Iterate over each Instructions in Basic Blocks
          // Dynamically cast Instruction to CallInst.
          // This step will return false for any instruction which is
          // not a CallInst
          if(CallInst *CI = dyn_cast<CallInst>(&I)) {
            // Print out the function name
            outs() << " |-" << CI->getCalledFunction()->getName() << "\n";
          }
        }
      }
      return false;
    }
  }; // end of struct FunctionsNames
}  // end of anonymous namespace
// инициализируем pass ID
char FunctionsNames::ID = 0;
//регистрируем pass
static RegisterPass<FunctionsNames> X("func-names", "Display Function Names",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);
//аргумент для прогона перед всеми оптимизациям
static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
      legacy::PassManagerBase &PM) { PM.add(new FunctionsNames()); });
