#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
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
  struct FindCallInstPass : public ModulePass {
    static char ID;

    FindCallInstPass() : ModulePass(ID) {}

    bool runOnModule(Module &M);

  };
}


bool FindCallInstPass::runOnModule(Module &M) {
	bool Instrumented = false;

	std::string filename = FILE_PATH;
	std::error_code EC;
	raw_fd_ostream File(filename, EC, sys::fs::OF_Text);

	for (auto &F : M) {
		for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
			// outs() << *I << "\n";
			if (auto *Call = dyn_cast<CallInst>(&*I)) {
				if (auto *Func = Call->getCalledFunction()) {

						File << *Call << "\n";
						File << "target function name: " << Func->getName() << "\n";
				}
			}
		}
	}

	return Instrumented;
}

char FindCallInstPass::ID = 0;

// Automatically enable the pass in clang
static void registerFindCallInstPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new FindCallInstPass());
}

static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerFindCallInstPass);

// register for opt
static RegisterPass<FindCallInstPass> X(
	"find-call-inst",
	"find call instruction pass",
	false,
	true);