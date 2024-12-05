add_library(BBFeaturesPass)
target_sources(BBFeaturesPass PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/BBFeaturesPass.h ${CMAKE_CURRENT_SOURCE_DIR}/BBFeaturesPass.cpp)

llvm_map_components_to_libnames(llvm_libs Support Core Passes)
target_link_libraries(BBFeaturesPass PRIVATE ${llvm_libs} fuzzdistillcc_utils)
