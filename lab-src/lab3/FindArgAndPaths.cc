#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

#define RES1 "res1.log"
#define RES2 "res2.log"

namespace {
  struct FindArgAndPathsPass : public FunctionPass {
    static char ID;

    FindArgAndPathsPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F);

  };

  bool FindArgAndPathsPass::runOnFunction(Function &F) {
    bool Instrumented = false;

    if (F.getName() == "TIFFFillTile") {
      // std::string filename1 = RES1;
      // std::string filename2 = RES2;
      // std::error_code EC;
      // raw_fd_ostream File(filename2, EC, sys::fs::OF_Text);

      // for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
      //     for (auto &U : I->operands()) {
      //     }
      //   }
       
      // }


      std::stack<BasicBlock*> S;
     
      auto &Entry = F.getEntryBlock();
      S.push(bb(&Entry, 0));

      while (!S.empty())
      {
        auto Cur = S.top();
        auto *TermInst = (Cur.first)->getTerminator();
        if (Cur.second >= TermInst->getNumSuccessors()) {
          S.pop();
        }

      }
      


    //   for (auto &BB : F) {
    //     S.push(bb(&BB, 0));
    //     BB.printAsOperand(outs(), false);
    //     outs() << " -> ";

    //     auto *TermInst = BB.getTerminator();

    //     while (!S.empty())
    //     {
          
    //     }
        

    //     for (unsigned i = 0; i < TermInst->getNumSuccessors(); ++i) {
    //       auto *B = TermInst->getSuccessor(i);
    //       B->printAsOperand(outs(), false);
    //     }
        
    //   }
    // }

    return Instrumented;
  }

}



char FindArgAndPathsPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerFindArgAndPathsPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new FindArgAndPathsPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerFindArgAndPathsPass);

// register for opt
static RegisterPass<FindArgAndPathsPass> X(
	"find-arg-and-paths",
	"find arg and paths pass",
	false,
	true);
