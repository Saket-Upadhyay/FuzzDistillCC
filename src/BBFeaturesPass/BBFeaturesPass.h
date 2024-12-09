//
// Created by Saket Upadhyay on 11/6/24.
//

#ifndef BBFEATURESPASS_H
#define BBFEATURESPASS_H


#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {
    class BBFeaturesPass : public PassInfoMixin<BBFeaturesPass> {
    public:
        llvm::PreservedAnalyses run(llvm::Module &aModule,
                                    llvm::ModuleAnalysisManager &MAM);

        bool runOnModule(llvm::Module &targetModule, llvm::ModuleAnalysisManager &MAM);

    private:
        static AnalysisKey Key;
        friend PassInfoMixin<BBFeaturesPass>;
    };
} // namespace llvm


#endif //BBFEATURESPASS_H
