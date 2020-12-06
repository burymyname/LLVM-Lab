#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


using namespace llvm;

#define FILE_PATH "res.log"

namespace {
  struct FindSucBBPass : public FunctionPass {
    static char ID;

    FindSucBBPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F);

  };
}


bool FindSucBBPass::runOnFunction(Function &F) {
	bool Instrumented = false;

	if (F.getName() == "TIFFFillTile") {
		std::string filename = FILE_PATH;
		std::error_code EC;
		raw_fd_ostream File(filename, EC, sys::fs::OF_Text);

		for (auto &BB : F) {
			auto *Inst = BB.getTerminator();
			//outs() << *Inst << "\n";

			if (Inst->getNumSuccessors() > 1) {
				File << BB << "\n";
			}
		}
	}

	return Instrumented;
}

char FindSucBBPass::ID = 0;

// Automatically enable the pass in clang
static void registerFindSucBBPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new FindSucBBPass());
}

static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerFindSucBBPass);

// register for opt
static RegisterPass<FindSucBBPass> X(
	"find-suc-BB",
	"find successor basic block pass",
	false,
	true);