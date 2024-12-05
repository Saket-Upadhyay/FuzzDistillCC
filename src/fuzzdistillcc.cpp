//
// Created by Saket Upadhyay on 3/15/23.
//

// Basic Imports
#include <llvm/Passes/PassBuilder.h>

#include <iostream>
#include <llvm/IR/Verifier.h>

// LLVM Imports
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"

// Project Imports
#include "BBFeaturesPass.h"
#include "FnFeaturesPass.h"

// Aliasing Namespace
namespace commandline = llvm::cl;

// Adding commandline options
static llvm::cl::OptionCategory FuzzPilerCategory{"FuzzPiler options"};

static commandline::opt<std::string> InputModule{
    commandline::Positional, commandline::desc{"<Module to analyze>"},
    commandline::value_desc{"bitcode filename"}, commandline::Required,
    commandline::cat{FuzzPilerCategory}};

/**
 * Takes the module and runs the poss on it.
 * @param targetModule
 */
static void runCustomPass(llvm::Module &targetModule) {
  llvm::ModulePassManager MPM;
  MPM.addPass(llvm::BBFeaturesPass());
  MPM.addPass(llvm::FnFeaturesPass());

  llvm::ModuleAnalysisManager MAM;
  llvm::FunctionAnalysisManager FAM;
  llvm::CGSCCAnalysisManager CAM;
  llvm::LoopAnalysisManager LAM;
  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerCGSCCAnalyses(CAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CAM, MAM);

  MPM.run(targetModule, MAM);


}

/**
 * Main function takes a file as argument to pass it to the LLVMIR parser.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
  commandline::HideUnrelatedOptions(FuzzPilerCategory);
  commandline::ParseCommandLineOptions(argc, argv, "Instruments object life");
  llvm::llvm_shutdown_obj SDO;
  std::string errorMessage;
  llvm::raw_string_ostream errorStream(errorMessage);

  // Parse IR
  llvm::SMDiagnostic error;
  llvm::LLVMContext context;
  std::unique_ptr<llvm::Module> aModule =
      llvm::parseIRFile(InputModule.getValue(), error, context);

  if (!aModule) {
    llvm::errs() << "Error reading the bitcode file: " << InputModule << "\n";
    error.print(argv[0], llvm::errs());
    return -1;
  }

  llvm::errs() << "\n\n";

  // Send the module to the runInstrumentPass function to be instrumented.
  runCustomPass(*aModule);

  if (verifyModule(*aModule, &errorStream)) {
    // Verification failed, print the error message
    errorStream.flush();
    llvm::errs() << "Error: Invalid module - " << errorMessage << "\n";
    return -1;
  }

  return 0;
}
