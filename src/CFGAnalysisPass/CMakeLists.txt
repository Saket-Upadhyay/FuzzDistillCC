add_library(CFGAnalysisPass)
target_sources(CFGAnalysisPass PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/CFGAnalysisPass.h ${CMAKE_CURRENT_SOURCE_DIR}/CFGAnalysisPass.cpp)

target_include_directories(CFGAnalysisPass PRIVATE)
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(CFGAnalysisPass PRIVATE ${llvm_libs})
