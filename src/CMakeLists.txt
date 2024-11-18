add_executable(fuzzpiler)

target_sources(fuzzpiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_subdirectory(BBFeaturesPass)
add_subdirectory(FnFeaturesPass)
add_subdirectory(Helpers/utils)

target_link_libraries(fuzzpiler
        PRIVATE
        BBFeaturesPass
        FnFeaturesPass
        fuzzpiler_utils
        ${llvm_libs})

target_include_directories(fuzzpiler
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/BBFeaturesPass
        ${CMAKE_CURRENT_SOURCE_DIR}/FnFeaturesPass
        ${CMAKE_CURRENT_SOURCE_DIR}/Helpers
)

