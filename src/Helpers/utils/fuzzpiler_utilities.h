//
// Created by Saket Upadhyay on 11/18/24.
//

#ifndef FUZZPILER_FUZZPILER_UTILITIES_H
#define FUZZPILER_FUZZPILER_UTILITIES_H
#include "../BasicBlockInfo.h"
#include "../FunctionInfo.h"
#include <vector>

extern void save_to_csv(const std::string &filename,
                        const std::vector<BasicBlockInfo> &block_info_vector);

extern void save_to_csv(const std::string &filename,
                        const std::vector<FunctionInfo> &function_info_vector);

extern std::string demangle_name_or_get_original_back(const std::string &mangledName);

#endif // FUZZPILER_FUZZPILER_UTILITIES_H
