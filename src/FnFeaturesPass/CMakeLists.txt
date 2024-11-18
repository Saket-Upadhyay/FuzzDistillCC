add_library(FnFeaturesPass)
target_sources(FnFeaturesPass PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/FnFeaturesPass.h ${CMAKE_CURRENT_SOURCE_DIR}/FnFeaturesPass.cpp)

llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(FnFeaturesPass PRIVATE ${llvm_libs} fuzzpiler_utils)