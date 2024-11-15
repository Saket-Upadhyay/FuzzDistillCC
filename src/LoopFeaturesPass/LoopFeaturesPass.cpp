//
// Created by drdope on 11/6/24.
//

#include "LoopFeaturesPass.h"

#include <cxxabi.h>
#include <fstream>
#include <iostream>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Analysis/CFG.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>
#include <utility>
#include "BasicBlockInfo.h"




std::string demangle_name_or_get_original_back(const std::string &mangledName);

void printLLVMErrs(llvm::StringRef Msg);

void save_to_csv(const std::string &filename,
                 std::vector<BasicBlockInfo> &blocks);


bool llvm::LoopFeaturesPass::runOnFunction(llvm::Function &targetFunction, llvm::FunctionAnalysisManager &FAM){
  bool Changed = false;
  std::vector<BasicBlockInfo> blocks;

  LoopInfo &LI = FAM.getResult<LoopAnalysis>(targetFunction);

  for (auto loop :LI) {
    for(auto loopbbs:loop->getBlocks()){
        errs()<<loopbbs->getName().str()<<"\n";
        //THESE BBS ARE LOOPED
    }
  }


//  save_to_csv(TargetModule.getName().str() + "_CFGAnalysisPass.csv", blocks);
  return Changed;
}

llvm::PreservedAnalyses
llvm::LoopFeaturesPass::run(llvm::Function &aFunction, llvm::FunctionAnalysisManager &FAM){
  if (runOnFunction(aFunction, FAM)) {
    return llvm::PreservedAnalyses::none();
  }
  return llvm::PreservedAnalyses::all();
}

//// Pass Registration, new PM

llvm::PassPluginLibraryInfo getLoopFeaturesPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "loopfeext", "v0.1", [](llvm::PassBuilder &PB) {
            PB.registerOptimizerLastEPCallback(
                [](llvm::ModulePassManager &MPM, llvm::OptimizationLevel OL) {
                  MPM.addPass(llvm::createModuleToFunctionPassAdaptor(llvm::LoopFeaturesPass()));
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopFeaturesPluginInfo();
}

// HELPER FUNCTIONS
//std::string demangle_name_or_get_original_back(const std::string &mangledName) {
//  int status = 0;
//  char *demangled =
//      abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);
//  std::string demangledName;
//
//  if (status == 0) {
//    demangledName = std::string(demangled);
//  } else {
//    demangledName = mangledName; // Return mangled name if demangling fails
//  }
//
//  std::free(demangled);
//  return demangledName;
//}

//void save_to_csv(const std::string &filename,
//                 std::vector<BasicBlockInfo> &blocks) {
//  // Create and open the CSV file
//  std::ofstream csvFile(filename, std::ios::out | std::ios::app);
//
//  // Check if the file is open
//  if (!csvFile) {
//    std::cerr << "Error opening file!" << std::endl;
//    return; // Exit if the file couldn't be opened
//  }
//  csvFile << "Block ID;Block Name;Instructions;In-degree;Out-degree;Static "
//             "Allocations;Dynamic Allocations;MemOps;VULNERABLE"
//          << std::endl;
//
//  // Write data to the CSV file
//  for (const auto &block : blocks) {
//    csvFile << block.toCSV() << std::endl;
//  }
//
//  // Close the CSV file
//  csvFile.close();
//
//  printLLVMErrs("CSV file '" + filename + "' created successfully!\n");
//}
