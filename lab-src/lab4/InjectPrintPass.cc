#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace llvm;

#define FILE_PATH "res.log"

namespace {
  struct InjectPrintPass : public ModulePass {
    static char ID;

    InjectPrintPass() : ModulePass(ID) {}

    bool runOnModule(Module &M);

  };
}


bool InjectPrintPass::runOnModule(Module &M) {
	bool Instrumented = false;
	auto &CTX = M.getContext();

	PointerType *PrintfArgTy = PointerType::getUnqual(Type::getInt128Ty(CTX));
	FunctionType *PrintfTy = FunctionType::get(Type::getVoidTy(CTX), PrintfArgTy);
	FunctionCallee Printf = M.getOrInsertFunction("printFuncName", PrintfTy);

	for (auto &F : M) {
		if (F.getName() == "printFuncName") continue;
		if (F.isDeclaration()) continue;

		Instrumented = true;
	
		IRBuilder<> IRB(&*F.getEntryBlock().getFirstInsertionPt());
		auto FuncName = IRB.CreateGlobalStringPtr(F.getName());

		IRB.CreateCall(Printf, {FuncName});
	}
	
	return Instrumented;
}

char InjectPrintPass::ID = 0;

// Automatically enable the pass in clang
static void registerInjectPrintPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new InjectPrintPass());
}

static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerInjectPrintPass);

// register for opt
static RegisterPass<InjectPrintPass> X(
	"inject-print",
	"inject printf pass",
	false,
	true);