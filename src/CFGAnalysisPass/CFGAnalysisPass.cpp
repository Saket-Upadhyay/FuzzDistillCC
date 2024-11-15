//
// Created by drdope on 11/6/24.
//

#include "CFGAnalysisPass.h"

#include "BasicBlockInfo.h"
#include <cxxabi.h>
#include <fstream>
#include <iostream>
#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/Instructions.h>
#include <utility>

std::string demangle_name_or_get_original_back(const std::string &mangledName);

void printLLVMErrs(llvm::StringRef Msg);

void save_to_csv(const std::string &filename,
                 std::vector<BasicBlockInfo> &blocks);

int get_in_degree(llvm::BasicBlock *BB);

int get_out_degree(llvm::BasicBlock *BB);

void count_allocations_in_basic_block(llvm::BasicBlock *BB,
                                      int &staticAllocCount,
                                      int &dynamicAllocCount,
                                      int &dynamicMemOpsCount);

bool llvm::CFGAnalysisPass::runOnModule(llvm::Module &TargetModule,
                                        llvm::ModuleAnalysisManager &MAM) {
  bool Changed = true;
  int instructioncount = 0;
  int functioncount = 0;
  int blockcount = 0;

  std::vector<BasicBlockInfo> blocks;

  for (auto &Function : TargetModule) {
    functioncount++;
    llvm::FunctionAnalysisManager &FAM =
        MAM.getResult<FunctionAnalysisManagerModuleProxy>(TargetModule)
            .getManager();
//LoopInfo &LI = FAM.getResult<LoopAnalysis>(Function);
//
//    for (auto loop : LI) {
//      for (auto loopbbs : loop->getBlocks()) {
//        errs() << loopbbs->getName().str() << "\n";
//        // THESE BBS ARE LOOPED
//      }
//    }

    for (auto &BasicBlock : Function) {
      blockcount++;
      for (auto &Inst : BasicBlock) {
        instructioncount++;
      }

      int indeg = get_in_degree(&BasicBlock);
      int outdeg = get_out_degree(&BasicBlock);
      // Count static and dynamic allocations in BB1
      int staticAllocCount = 0;
      int dynamicAllocCount = 0;
      int dynamicMemOps = 0;
      count_allocations_in_basic_block(&BasicBlock, staticAllocCount,
                                       dynamicAllocCount, dynamicMemOps);

      size_t id = reinterpret_cast<size_t>(&BasicBlock);
      BasicBlockInfo temp_block(id);
      temp_block.setInstructionCount(instructioncount);
      temp_block.setInDegree(indeg);
      temp_block.setOutDegree(outdeg);
      temp_block.setStaticAllocations(staticAllocCount);
      temp_block.setDynamicAllocations(dynamicAllocCount);
      temp_block.setDynamicMemops(dynamicMemOps);
      std::string trydemangleBBparent = demangle_name_or_get_original_back(
          BasicBlock.getParent()->getName().str());
      temp_block.setBlockName("BB" + std::to_string(blockcount) + "_" +
                              trydemangleBBparent);
      BasicBlock.setName("BB" + std::to_string(blockcount) + "_" +
                         std::to_string(id));
      //      printLLVMErrs(BasicBlock.getName().str());
      blocks.push_back(temp_block);
      instructioncount = 0;
    }



  }

  save_to_csv(TargetModule.getName().str() + "_CFGAnalysisPass.csv", blocks);
  return Changed;
}

llvm::PreservedAnalyses
llvm::CFGAnalysisPass::run(llvm::Module &AModule,
                           llvm::ModuleAnalysisManager &MAM) {
  if (runOnModule(AModule, MAM)) {
    return llvm::PreservedAnalyses::none();
  }
  return llvm::PreservedAnalyses::all();
}

//// Pass Registration, new PM

llvm::PassPluginLibraryInfo getInstrumentationPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "cfgext", "v0.1", [](llvm::PassBuilder &PB) {
            PB.registerOptimizerLastEPCallback(
                [](llvm::ModulePassManager &MPM, llvm::OptimizationLevel OL) {
                  MPM.addPass(llvm::CFGAnalysisPass());
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getInstrumentationPluginInfo();
}

// HELPER FUNCTIONS
std::string demangle_name_or_get_original_back(const std::string &mangledName) {
  int status = 0;
  char *demangled =
      abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);
  std::string demangledName;

  if (status == 0) {
    demangledName = std::string(demangled);
  } else {
    demangledName = mangledName; // Return mangled name if demangling fails
  }

  std::free(demangled);
  return demangledName;
}

void save_to_csv(const std::string &filename,
                 std::vector<BasicBlockInfo> &blocks) {
  // Create and open the CSV file
  std::ofstream csvFile(filename, std::ios::out | std::ios::app);

  // Check if the file is open
  if (!csvFile) {
    std::cerr << "Error opening file!" << std::endl;
    return; // Exit if the file couldn't be opened
  }
  csvFile << "Block ID;Block Name;Instructions;In-degree;Out-degree;Static "
             "Allocations;Dynamic Allocations;MemOps;VULNERABLE"
          << std::endl;

  // Write data to the CSV file
  for (const auto &block : blocks) {
    csvFile << block.toCSV() << std::endl;
  }

  // Close the CSV file
  csvFile.close();

  printLLVMErrs("CSV file '" + filename + "' created successfully!\n");
}

void printLLVMErrs(llvm::StringRef Msg) { llvm::errs() << Msg << "\n"; }

// Basic Block Information Helpers

int get_in_degree(llvm::BasicBlock *BB) {
  int inDegree = 0;

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

int get_out_degree(llvm::BasicBlock *BB) {
  int outDegree = 0;

  // Get the terminator instruction for the basic block
  llvm::Instruction *termInst = BB->getTerminator();
  if (!termInst)
    return 0; // If no terminator, out-degree is 0

  // Check all the successors (branches) of the terminator
  outDegree = termInst->getNumSuccessors();

  return outDegree;
}

void count_allocations_in_basic_block(llvm::BasicBlock *BB,
                                      int &staticAllocCount,
                                      int &dynamicAllocCount,
                                      int &dynamicMemOpsCount) {
  // Iterate over all instructions in the basic block
  for (auto &I : *BB) {
    // Check for alloca instructions (stack allocations)
    if (isa<llvm::AllocaInst>(I)) {
      staticAllocCount++; // Increment stack allocation count
    }
    // Check for malloc/calloc or other allocation calls (heap allocations)
    else if (isa<llvm::CallInst>(I)) {
      llvm::CallInst *callInst = cast<llvm::CallInst>(&I);
      llvm::Function *calledFunc = callInst->getCalledFunction();
      // Check if the function being called is malloc, calloc, or any other heap
      // allocation
      if (calledFunc) {
        std::string demangled_func_name =
            demangle_name_or_get_original_back(calledFunc->getName().str());
        printLLVMErrs(demangled_func_name);
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
