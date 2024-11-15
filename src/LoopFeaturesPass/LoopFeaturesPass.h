//
// Created by Saket Upadhyay on 11/6/24.
//

#ifndef LOOPFEATURESPASS_H
#define LOOPFEATURESPASS_H


#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {
    class LoopFeaturesPass
        : public PassInfoMixin<LoopFeaturesPass> {
    public:
        llvm::PreservedAnalyses run(llvm::Function &aFunction,
                                    llvm::FunctionAnalysisManager &MAM);

        bool runOnFunction(llvm::Function &targetFunction, llvm::FunctionAnalysisManager &MAM);

    private:
        static AnalysisKey Key;
        friend PassInfoMixin<LoopFeaturesPass>;
    };
} // namespace llvm


#endif //LOOPFEATURESPASS_H
