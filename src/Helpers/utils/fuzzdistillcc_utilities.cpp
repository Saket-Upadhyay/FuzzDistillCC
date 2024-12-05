//
// Created by Saket Upadhyay on 11/18/24.
//

#include "fuzzdistillcc_utilities.h"
#include "fstream"
#include "iostream"
#include <cxxabi.h>

std::string const FNCSVHEADER =
    "Function ID;Function Name;Instructions;BBs;In-degree;Out-degree;Static "
    "Allocations;Dynamic "
    "Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;"
    "InDirectCalls;VULNERABLE";

std::string const BBCSVHEADER =
    "Block ID;Block Name;Instructions;In-degree;Out-degree;Static "
    "Allocations;Dynamic "
    "Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;"
    "InDirectCalls;VULNERABLE";

#define WRITEHEADER false

/***
 *
 * @param filename
 * @param block_info_vector
 */
void save_to_csv(const std::string &filename,
                 const std::vector<BasicBlockInfo> &block_info_vector) {
  // Create and open the CSV file
  std::ofstream csvFile(filename, std::ios::out | std::ios::app);

  // Check if the file is open
  if (!csvFile) {
    std::cerr << "Error opening file!" << std::endl;
    return; // Exit if the file couldn't be opened
  }
  if constexpr (WRITEHEADER) {
    csvFile << BBCSVHEADER << std::endl;
  }

  // Write data to the CSV file
  for (const auto &block : block_info_vector) {
    csvFile << block.toCSV() << std::endl;
  }

  // Close the CSV file
  csvFile.close();

  std::cout << "CSV file '" + filename + "' created successfully!\n";
}

/***
 * Overload save to csv
 * @param filename
 * @param function_info_vector
 */
void save_to_csv(const std::string &filename,
                 const std::vector<FunctionInfo> &function_info_vector) {
  // Create and open the CSV file
  std::ofstream csvFile(filename, std::ios::out | std::ios::app);

  // Check if the file is open
  if (!csvFile) {
    std::cerr << "Error opening file!" << std::endl;
    return; // Exit if the file couldn't be opened
  }
  if constexpr (WRITEHEADER) {
    csvFile << FNCSVHEADER << std::endl;
  }

  // Write data to the CSV file
  for (const auto &func : function_info_vector) {
    csvFile << func.toCSV() << std::endl;
  }

  // Close the CSV file
  csvFile.close();

  std::cout << "CSV file '" + filename + "' created successfully!\n";
}

/***
 * Takes a C++ mangled function names and returns a demangled string.
 * @param mangledName
 * @return Demangled Name
 */
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