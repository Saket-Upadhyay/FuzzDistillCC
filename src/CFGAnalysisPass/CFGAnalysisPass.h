//
// Created by Saket Upadhyay on 11/6/24.
//

#ifndef CFGANALYSISPASS_H
#define CFGANALYSISPASS_H


#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {
    class CFGAnalysisPass
        : public PassInfoMixin<CFGAnalysisPass> {
    public:
        llvm::PreservedAnalyses run(llvm::Module &aModule,
                                    llvm::ModuleAnalysisManager &MAM);

        bool runOnModule(llvm::Module &targetModule);

    private:
        static AnalysisKey Key;
        friend PassInfoMixin<CFGAnalysisPass>;
    };
} // namespace llvm


#endif //CFGANALYSISPASS_H
