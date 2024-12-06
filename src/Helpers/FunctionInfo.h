//
// Created by Saket Upadhyay on 11/18/24.
//

#ifndef FUZZPILER_FUNCTIONINFO_H
#define FUZZPILER_FUNCTIONINFO_H

#include <cstddef>
#include <string>

class FunctionInfo {
private:
  std::size_t id;
  std::string function_name;
  uint instruction_count;
  uint block_count;
  uint argument_count;
  uint in_degree;           // Number of incoming edges
  uint out_degree;          // Number of outgoing edges
  uint static_allocations;  // How many times the block was executed
  uint dynamic_allocations; // Number of errors encountered
  uint dynamic_memops;
  uint num_loops;
  uint is_vulnerable;
  uint cond_branches;   // Number of conditional branch instructions
  uint uncond_branches; // Number of unconditional branch instructions
  uint direct_calls;
  uint indirect_calls;

public:
  explicit FunctionInfo(const size_t id)
      : id(id), instruction_count(0), block_count(0), argument_count(0),
        in_degree(0), out_degree(0), static_allocations(0),
        dynamic_allocations(0), dynamic_memops(0), is_vulnerable(0),
        cond_branches(0), uncond_branches(0), direct_calls(0),
        indirect_calls(0), num_loops(0) {}

  void setBlockCount(uint blockCount) { block_count = blockCount; }
  void setIsVulnerable(uint isVulnerable) { is_vulnerable = isVulnerable; }
  void setFunctionName(const std::string &functionName) {
    function_name = functionName;
  }
  void setArgumentCount(uint argumentCount) { argument_count = argumentCount; }
  void setInstructionCount(const uint instructionCount) {
    instruction_count = instructionCount;
  }
  void setInDegree(const uint inDegree) { in_degree = inDegree; }
  void setOutDegree(const uint outDegree) { out_degree = outDegree; }
  void setStaticAllocations(uint staticAllocations) {
    static_allocations = staticAllocations;
  }
  void setDynamicAllocations(uint dynamicAllocations) {
    dynamic_allocations = dynamicAllocations;
  }
  [[nodiscard]] const std::string &getFunctionName() const {
    return function_name;
  }
  void setDynamicMemops(const uint dynamicMemops) {
    dynamic_memops = dynamicMemops;
  }
  void setCondBranches(uint condBranches) { cond_branches = condBranches; }
  void setUncondBranches(uint uncondBranches) {
    uncond_branches = uncondBranches;
  }
  void setDirectCalls(uint directCalls) { direct_calls = directCalls; }
  void setIndirectCalls(uint indirectCalls) { indirect_calls = indirectCalls; }

  void setNumLoops(uint numLoops) { num_loops = numLoops; }

  // getCSVinfo
  [[nodiscard]] std::string toCSV() const {
    return std::to_string(id) + ";" + function_name + ";" +
           std::to_string(instruction_count) + ";" +
           std::to_string(block_count) + ";" + std::to_string(in_degree) + ";" +
           std::to_string(out_degree) + ";" +
           std::to_string(num_loops) + ";" +
           std::to_string(static_allocations) + ";" +
           std::to_string(dynamic_allocations) + ";" +
           std::to_string(dynamic_memops) + ";" +
           std::to_string(cond_branches) + ";" +
           std::to_string(uncond_branches) + ";" +
           std::to_string(direct_calls) + ";" + std::to_string(indirect_calls) +
           ";" + std::to_string(is_vulnerable);
  }
};

#endif // FUZZPILER_FUNCTIONINFO_H
