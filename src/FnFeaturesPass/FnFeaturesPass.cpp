//
// Created by drdope on 11/6/24.
//

#include "FnFeaturesPass.h"

#include "FunctionInfo.h"
#include "fuzzpiler_utilities.h"
#include <cxxabi.h>
#include <iostream>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Analysis/CFG.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Instructions.h>

uint get_conditional_branches(llvm::Function *Fn) {
  uint cond_branchcounter = 0;
  for (auto &BB : *Fn) {
    for (auto &I : BB) {
      if (llvm::isa<llvm::BranchInst>(I)) {
        auto *BI = cast<llvm::BranchInst>(&I);
        if (BI->isConditional()) {
          cond_branchcounter += BI->getNumSuccessors();
        }
      }

      else if (llvm::isa<llvm::SwitchInst>(I)) {
        auto *SI = cast<llvm::SwitchInst>(&I);
        cond_branchcounter += SI->getNumSuccessors();
      }
    }
  }
  return cond_branchcounter;
}

uint get_unconditional_branches(llvm::Function *Fn) {
  uint uncond_branchcounter = 0;
  for (auto &BB : *Fn) {
    for (auto &I : BB) {
      if (llvm::isa<llvm::BranchInst>(I)) {
        auto *BI = cast<llvm::BranchInst>(&I);
        if (BI->isUnconditional()) {
          uncond_branchcounter += BI->getNumSuccessors();
        }
      }
    }
  }
  return uncond_branchcounter;
}

uint get_direct_calls(llvm::Function *Fn) {
  uint dcalls = 0;
  for (auto &BB : *Fn) {
    for (auto &I : BB) {

      if (llvm::isa<llvm::CallInst>(I)) {
        auto *CI = cast<llvm::CallInst>(&I);
        if (!CI->isIndirectCall()) {
          dcalls++;
        }
      }
    }
  }
  return dcalls;
}

uint get_indirect_calls(llvm::Function *Fn) {
  uint indcalls = 0;
  for (auto &BB : *Fn) {
    for (auto &I : BB) {
      if (llvm::isa<llvm::CallInst>(I)) {
        auto *CI = cast<llvm::CallInst>(&I);
        if (CI->isIndirectCall()) {
          indcalls++;
        }
      }
    }
  }
  return indcalls;
}

void count_allocations_in_function(llvm::Function *Fn, uint &staticAllocCount,
                                   uint &dynamicAllocCount,
                                   uint &dynamicMemOpsCount) {
  // Iterate over all instructions in the Function
  for (auto &BB : *Fn) {
    for (auto &I : BB) {
      // Check for alloca instructions (stack allocations)
      if (isa<llvm::AllocaInst>(I)) {
        staticAllocCount++; // Increment stack allocation count
      }
      // Check for malloc/calloc or other allocation calls (heap allocations)
      else if (isa<llvm::CallInst>(I)) {
        auto *callInst = cast<llvm::CallInst>(&I);
        llvm::Function *calledFunc = callInst->getCalledFunction();
        // Check if the function being called is malloc, calloc, or any other
        // heap allocation
        if (calledFunc) {
          std::string demangled_func_name =
              demangle_name_or_get_original_back(calledFunc->getName().str());
          //         llvm::errs()<<demangled_func_name;
          if ((demangled_func_name.find("malloc") == 0 ||
               demangled_func_name.find("calloc") == 0 ||
               demangled_func_name.find("realloc") == 0 ||
               calledFunc->getName().str() == "free" ||
               demangled_func_name.find("operator delete") == 0 ||
               demangled_func_name.find("operator new[]") == 0 ||
               demangled_func_name.find("operator new") == 0)) {
            dynamicAllocCount++; // Increment heap allocation count
          }
          if ((demangled_func_name.find("memmove") == 0 ||
               demangled_func_name.find("memcpy") == 0 ||
               demangled_func_name.find("memset") == 0 ||
               demangled_func_name.find("memcmp") == 0 ||
               demangled_func_name.find("mmap") == 0)) {
            dynamicMemOpsCount++;
          }
        }
      }
    }
  }
}

bool llvm::FnFeaturesPass::runOnModule(llvm::Module &targetModule,
                                       llvm::ModuleAnalysisManager &MAM) {
  bool Changed = false;
  std::vector<FunctionInfo> function_data_vector;
  //      llvm::FunctionAnalysisManager &FAM =
  //        MAM.getResult<FunctionAnalysisManagerModuleProxy>(targetModule)
  //            .getManager();
  // LoopInfo &LI = FAM.getResult<LoopAnalysis>(Function);
  //
  //     for (auto loop : LI) {
  //       for (auto loopbbs : loop->getBlocks()) {
  //         errs() << loopbbs->getName().str() << "\n";
  //         // THESE BBS ARE LOOPED
  //       }
  //     }

  for (auto &F : targetModule) {

    uint staticAllocCount = 0;
    uint dynamicAllocCount = 0;
    uint dynamicMemOps = 0;
    count_allocations_in_function(&F, staticAllocCount, dynamicAllocCount,
                                  dynamicMemOps);

    const auto id = reinterpret_cast<size_t>(&F);
    FunctionInfo temp_fn(id);
    temp_fn.setFunctionName(
        "F" + demangle_name_or_get_original_back(F.getName().str()));
    if (temp_fn.getFunctionName().find("FCWE") == 0) {
      if (F.getInstructionCount() > 0) {
        temp_fn.setInstructionCount(F.getInstructionCount());
        temp_fn.setBlockCount([&F](uint blockcount = 0) {
          for (auto &BasicBlock : F) {
            blockcount++;
          }
          return blockcount;
        }());

        if (temp_fn.getFunctionName().find("bad") != std::string::npos) {
          temp_fn.setIsVulnerable(1);
        }
        temp_fn.setArgumentCount(F.getNumOperands());
        temp_fn.setCondBranches(get_conditional_branches(&F));
        temp_fn.setUncondBranches(get_unconditional_branches(&F));
        temp_fn.setDirectCalls(get_direct_calls(&F));
        temp_fn.setIndirectCalls(get_indirect_calls(&F));
        temp_fn.setStaticAllocations(staticAllocCount);
        temp_fn.setDynamicAllocations(dynamicAllocCount);
        temp_fn.setDynamicMemops(dynamicMemOps);

        function_data_vector.emplace_back(temp_fn);
      }
    }
  }

  // Save all data
  save_to_csv(targetModule.getName().str() + "_FunctionFeatures.csv",
              function_data_vector);
  return Changed;
}

llvm::PreservedAnalyses
llvm::FnFeaturesPass::run(llvm::Module &aModule,
                          llvm::ModuleAnalysisManager &MAM) {
  if (runOnModule(aModule, MAM)) {
    return llvm::PreservedAnalyses::none();
  }
  return llvm::PreservedAnalyses::all();
}

//// Pass Registration, new PM

llvm::PassPluginLibraryInfo getFnFeaturesPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "fnfeaturext", "v0.1",
          [](llvm::PassBuilder &PB) {
            PB.registerOptimizerLastEPCallback(
                [](llvm::ModulePassManager &MPM, llvm::OptimizationLevel OL) {
                  MPM.addPass(llvm::FnFeaturesPass());
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getFnFeaturesPluginInfo();
}
