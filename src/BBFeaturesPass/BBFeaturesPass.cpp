//
// Created by Saket Upadhyay on 11/6/24.
//

#define ISNISTTRAININGSET false

#include "BBFeaturesPass.h"

#include "BasicBlockInfo.h"
#include <fuzzdistillcc_utilities.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/Instructions.h>

#include <cassert>

uint get_in_degree(llvm::BasicBlock *BB);

uint get_out_degree(llvm::BasicBlock *BB);

uint get_conditional_branches(llvm::BasicBlock *BB);
uint get_unconditional_branches(llvm::BasicBlock *BB);
uint get_direct_calls(llvm::BasicBlock *BB);
uint get_indirect_calls(llvm::BasicBlock *BB);

void count_allocations_in_basic_block(llvm::BasicBlock *BB,
                                      uint &staticAllocCount,
                                      uint &dynamicAllocCount,
                                      uint &dynamicMemOpsCount);

bool llvm::BBFeaturesPass::runOnModule(llvm::Module &TargetModule,
                                       llvm::ModuleAnalysisManager &MAM) {
  bool Changed = true;
  uint functioncount = 0;
  uint blockcount = 0;

  std::vector<BasicBlockInfo> blocks;

  for (auto &Function : TargetModule) {
    functioncount++;
#if ISNISTTRAININGSET == true
    if (demangle_name_or_get_original_back(Function.getName().str())
            .find("CWE") == 0) {
#endif
      for (auto &BasicBlock : Function) {
        blockcount++;

        // Count static and dynamic allocations in BB1
        uint staticAllocCount = 0;
        uint dynamicAllocCount = 0;
        uint dynamicMemOps = 0;
        count_allocations_in_basic_block(&BasicBlock, staticAllocCount,
                                         dynamicAllocCount, dynamicMemOps);

        size_t id = reinterpret_cast<size_t>(&BasicBlock);
        BasicBlockInfo temp_block(id);
        temp_block.setInstructionCount([&BasicBlock](int instructioncount = 0) {
          for (auto &Inst : BasicBlock) {
            instructioncount++;
          }
          return instructioncount;
        }());
        temp_block.setInDegree(get_in_degree(&BasicBlock));
        temp_block.setOutDegree(get_out_degree(&BasicBlock));
        temp_block.setStaticAllocations(staticAllocCount);
        temp_block.setDynamicAllocations(dynamicAllocCount);
        temp_block.setDynamicMemops(dynamicMemOps);
        temp_block.setUncondBranches(get_conditional_branches(&BasicBlock));
        temp_block.setUncondBranches(get_unconditional_branches(&BasicBlock));
        temp_block.setIndirentCalls(get_indirect_calls(&BasicBlock));
        temp_block.setDirectCalls(get_direct_calls(&BasicBlock));

        temp_block.setBlockName("BB" + std::to_string(blockcount) + "_" +
                                demangle_name_or_get_original_back(
                                    BasicBlock.getParent()->getName().str()));
        BasicBlock.setName("BB" + std::to_string(blockcount) + "_" +
                           std::to_string(id));

        if (temp_block.get_block_name().find("bad") != std::string::npos) {
          temp_block.setIsVulnerable(1);
        }

        blocks.push_back(temp_block);
      }
    }
#if ISNISTTRAININGSET == true
  }
#endif

  save_to_csv(TargetModule.getName().str() + "_BBFeatures.csv", blocks);
  return Changed;
}

llvm::PreservedAnalyses
llvm::BBFeaturesPass::run(llvm::Module &AModule,
                          llvm::ModuleAnalysisManager &MAM) {
  // assert(Changed);
  if (runOnModule(AModule, MAM)) {
    return llvm::PreservedAnalyses::none();
  }
  return llvm::PreservedAnalyses::all();
}

//// Pass Registration, new PM

llvm::PassPluginLibraryInfo getBBFeatuesPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "bbfeaturext", "v0.1",
          [](llvm::PassBuilder &PB) {
            PB.registerOptimizerLastEPCallback(
                [](llvm::ModulePassManager &MPM, llvm::OptimizationLevel OL) {
                  MPM.addPass(llvm::BBFeaturesPass());
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getBBFeatuesPluginInfo();
}

// Basic Block Information Helpers

uint get_in_degree(llvm::BasicBlock *BB) {
  uint inDegree = 0;

  // Iterate over all the functions in the module
  for (auto &F : *BB->getParent()->getParent()) {
    // Iterate over all basic blocks in the function
    for (auto &block : F) {
      // Get the terminator instruction of the block
      llvm::Instruction *termInst = block.getTerminator();
      if (!termInst)
        continue; // Skip if there's no terminator (shouldn't happen)

      // Check if the terminator instruction has successors
      for (unsigned i = 0; i < termInst->getNumSuccessors(); ++i) {
        // If one of the successors is our BasicBlock, increment in-degree
        if (termInst->getSuccessor(i) == BB) {
          inDegree++;
        }
      }
    }
  }
  return inDegree;
}

uint get_out_degree(llvm::BasicBlock *BB) {
  uint outDegree = 0;

  // Get the terminator instruction for the basic block
  llvm::Instruction *termInst = BB->getTerminator();
  if (!termInst)
    return 0; // If no terminator, out-degree is 0

  // Check all the successors (branches) of the terminator
  outDegree = termInst->getNumSuccessors();

  return outDegree;
}

void count_allocations_in_basic_block(llvm::BasicBlock *BB,
                                      uint &staticAllocCount,
                                      uint &dynamicAllocCount,
                                      uint &dynamicMemOpsCount) {
  // Iterate over all instructions in the basic block
  for (auto &I : *BB) {
    // Check for alloca instructions (stack allocations)
    if (isa<llvm::AllocaInst>(I)) {
      staticAllocCount++; // Increment stack allocation count
    }
    // Check for malloc/calloc or other allocation calls (heap allocations)
    else if (isa<llvm::CallInst>(I)) {
      auto *callInst = cast<llvm::CallInst>(&I);
      llvm::Function *calledFunc = callInst->getCalledFunction();
      // Check if the function being called is malloc, calloc, or any other heap
      // allocation
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

uint get_conditional_branches(llvm::BasicBlock *BB) {
  uint cond_branchcounter = 0;
  for (auto &I : *BB) {
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
  return cond_branchcounter;
}

uint get_unconditional_branches(llvm::BasicBlock *BB) {
  uint uncond_branchcounter = 0;
  for (auto &I : *BB) {
    if (llvm::isa<llvm::BranchInst>(I)) {
      auto *BI = cast<llvm::BranchInst>(&I);
      if (BI->isUnconditional()) {
        uncond_branchcounter += BI->getNumSuccessors();
      }
    }
  }
  return uncond_branchcounter;
}

uint get_direct_calls(llvm::BasicBlock *BB) {
  uint dcalls = 0;
  for (auto &I : *BB) {

    if (llvm::isa<llvm::CallInst>(I)) {
      auto *CI = cast<llvm::CallInst>(&I);
      if (!CI->isIndirectCall()) {
        dcalls++;
      }
    }
  }
  return dcalls;
}

uint get_indirect_calls(llvm::BasicBlock *BB) {
  uint indcalls = 0;
  for (auto &I : *BB) {
    if (llvm::isa<llvm::CallInst>(I)) {
      auto *CI = cast<llvm::CallInst>(&I);
      if (CI->isIndirectCall()) {
        indcalls++;
      }
    }
  }
  return indcalls;
}