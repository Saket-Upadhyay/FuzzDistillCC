add_library(LoopFeaturesPass)
target_sources(LoopFeaturesPass PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LoopFeaturesPass.h ${CMAKE_CURRENT_SOURCE_DIR}/LoopFeaturesPass.cpp)

target_include_directories(LoopFeaturesPass PRIVATE ../Helpers)
llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(LoopFeaturesPass PRIVATE ${llvm_libs})