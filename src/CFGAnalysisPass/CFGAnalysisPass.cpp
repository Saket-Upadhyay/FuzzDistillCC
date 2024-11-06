//
// Created by drdope on 11/6/24.
//

#include "CFGAnalysisPass.h"


void printLLVMErrs(llvm::StringRef Msg)
{
    llvm::errs() << Msg << "\n";
}

bool llvm::CFGAnalysisPass::runOnModule(llvm::Module& TargetModule)
{
    bool Changed = false;
    int instructioncount = 0;

    for (auto& Function : TargetModule)
    {
        for (auto& BasicBlock : Function)
        {
            for (auto& Inst : BasicBlock)
            {
                instructioncount++;
            }
        }
    }

    printLLVMErrs("Module = "+TargetModule.getName().str()+" | Instructions: " + std::to_string(instructioncount) + "\n");

    return Changed;
}


llvm::PreservedAnalyses
llvm::CFGAnalysisPass::run(llvm::Module& AModule,
                           llvm::ModuleAnalysisManager& MAM)
{
    if (runOnModule(AModule))
    {
        return llvm::PreservedAnalyses::none();
    }
    return llvm::PreservedAnalyses::all();
}


//// Pass Registration, new PM

llvm::PassPluginLibraryInfo getInstrumentationPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "cfgext", "v0.1",
        [](llvm::PassBuilder& PB)
        {
            PB.registerOptimizerLastEPCallback(
                [](llvm::ModulePassManager& MPM, llvm::OptimizationLevel OL)
                {
                    MPM.addPass(llvm::CFGAnalysisPass());
                });
        }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return getInstrumentationPluginInfo();
}
