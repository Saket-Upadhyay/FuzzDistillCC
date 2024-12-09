//
// Created by Saket Upadhyay on 11/6/24.
//

#ifndef FNFEATURESPASS_H
#define FNFEATURESPASS_H


#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {
    class FnFeaturesPass : public PassInfoMixin<FnFeaturesPass> {
    public:
        llvm::PreservedAnalyses run(llvm::Module &aModule,
                                    llvm::ModuleAnalysisManager &MAM);

        bool runOnModule(llvm::Module &targetModule, llvm::ModuleAnalysisManager &MAM);

    private:
        static AnalysisKey Key;
        friend PassInfoMixin<FnFeaturesPass>;
    };
} // namespace llvm


#endif //FNFEATURESPASS_H
