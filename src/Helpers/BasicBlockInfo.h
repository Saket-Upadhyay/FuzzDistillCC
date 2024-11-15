//
// Created by Saket Upadhyay on 11/15/24.
//

#ifndef FUZZPILER_BASICBLOCKINFO_H
#define FUZZPILER_BASICBLOCKINFO_H

#include <cstddef>
#include <string>

class BasicBlockInfo {
private:
  std::size_t block_id; // Unique identifier for the block
  std::string block_name;
  int instruction_count;
  int in_degree;           // Number of incoming edges
  int out_degree;          // Number of outgoing edges
  int static_allocations;  // How many times the block was executed
  int dynamic_allocations; // Number of errors encountered
  int dynamic_memops;
  int is_vulnerable;

public:
  // Constructor to initialize the object
  explicit BasicBlockInfo(int id)
      : block_id(id), block_name("NF"), instruction_count(0), in_degree(0),
        out_degree(0), static_allocations(0), dynamic_allocations(0),
        dynamic_memops(0), is_vulnerable(0) {}

  // Getters for the class attributes (optional)
  [[nodiscard]] size_t getBlockId() const { return block_id; }

  // Setters for the class attributes (optional)
  void setInDegree(int degree) { in_degree = degree; }

  void setOutDegree(int degree) { out_degree = degree; }

  void setInstructionCount(int count) { instruction_count = count; }

  void setBlockName(std::string name) { block_name = std::move(name); }

  void setStaticAllocations(int count) { static_allocations = count; }

  void setDynamicAllocations(int count) { dynamic_allocations = count; }

  void setDynamicMemops(int count) { dynamic_memops = count; }

  void setIsVulnerable(int isVulnerable) { is_vulnerable = isVulnerable; }

  // getCSVinfo
  [[nodiscard]]
  std::string toCSV() const {
  return std::to_string(block_id) + ";" + block_name + ";" +
         std::to_string(instruction_count) + ";" + std::to_string(in_degree) +
         ";" + std::to_string(out_degree) + ";" +
         std::to_string(static_allocations) + ";" +
         std::to_string(dynamic_allocations) + ";" +
         std::to_string(dynamic_memops) + ";" + std::to_string(is_vulnerable);
}

};

#endif // FUZZPILER_BASICBLOCKINFO_H
