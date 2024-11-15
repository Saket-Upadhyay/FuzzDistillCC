add_executable(fuzzpiler)

target_sources(fuzzpiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_subdirectory(CFGAnalysisPass)
add_subdirectory(LoopFeaturesPass)

target_link_libraries(fuzzpiler
        PRIVATE
        CFGAnalysisPass
        LoopFeaturesPass
        ${llvm_libs})

target_include_directories(fuzzpiler
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/CFGAnalysisPass
         ${CMAKE_CURRENT_SOURCE_DIR}/LoopFeaturesPass
)

