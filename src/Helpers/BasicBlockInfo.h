//
// Created by Saket Upadhyay on 11/15/24.
//

#ifndef FUZZPILER_BASICBLOCKINFO_H
#define FUZZPILER_BASICBLOCKINFO_H

#include <cstddef>
#include <string>

typedef unsigned int uint;

class BasicBlockInfo {
private:
  std::size_t block_id; // Unique identifier for the block
  std::string block_name;
  int instruction_count;
  uint in_degree;           // Number of incoming edges
  uint out_degree;          // Number of outgoing edges
  uint static_allocations;  // How many times the block was executed
  uint dynamic_allocations; // Number of errors encountered
  uint dynamic_memops;
  uint is_vulnerable;
  uint cond_branches;   // Number of conditional branch instructions
  uint uncond_branches; // Number of unconditional branch instructions
  uint direct_calls;
  uint indirect_calls;
  uint call_freq;       // number of calls in BB
  uint syscall_count;

public:
  // Constructor to initialize the object
  explicit BasicBlockInfo(std::size_t id)
      : block_id(id), block_name("NF"), instruction_count(0), in_degree(0),
        out_degree(0), static_allocations(0), dynamic_allocations(0),
        dynamic_memops(0), is_vulnerable(0), cond_branches(0),
        uncond_branches(0), call_freq(0), syscall_count(0) {}

  // Getters for the class attributes (optional)
  [[nodiscard]] size_t getBlockId() const { return block_id; }

  // Setters for the class attributes (optional)
  void setInDegree(uint degree) { in_degree = degree; }

  void setOutDegree(uint degree) { out_degree = degree; }

  void setInstructionCount(uint count) { instruction_count = count; }

  void setBlockName(std::string name) { block_name = std::move(name); }

  void setStaticAllocations(uint count) { static_allocations = count; }

  void setIndirentCalls(uint indirentCalls) { indirect_calls = indirentCalls; }
  void setDirectCalls(uint directCalls) { direct_calls = directCalls; }

  void setDynamicAllocations(uint count) { dynamic_allocations = count; }

  void setDynamicMemops(uint count) { dynamic_memops = count; }

  void setSyscallCount(uint syscallCount) { syscall_count = syscallCount; }
  void setCondBranches(uint condBranches) { cond_branches = condBranches; }
  void setUncondBranches(uint uncondBranches) {
    uncond_branches = uncondBranches;
  }
  void setIsVulnerable(uint isVulnerable) { is_vulnerable = isVulnerable; }
  void setCallFreq(uint callFreq) { call_freq = callFreq; }

  // getCSVinfo
  [[nodiscard]]
  std::string toCSV() const {
    return std::to_string(block_id) + ";" + block_name + ";" +
           std::to_string(instruction_count) + ";" + std::to_string(in_degree) +
           ";" + std::to_string(out_degree) + ";" +
           std::to_string(static_allocations) + ";" +
           std::to_string(dynamic_allocations) + ";" +
           std::to_string(dynamic_memops) + ";" +
           std::to_string(cond_branches) + ";" +
           std::to_string(uncond_branches) + ";" +
           std::to_string(direct_calls) + ";" +
           std::to_string(indirect_calls) + ";" +
           std::to_string(is_vulnerable);
  }
};

#endif // FUZZPILER_BASICBLOCKINFO_H
