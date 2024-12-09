add_executable(fuzzdistillcc)

target_sources(fuzzdistillcc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fuzzdistillcc.cpp)
add_subdirectory(BBFeaturesPass)
add_subdirectory(FnFeaturesPass)
add_subdirectory(Helpers/utils)

target_link_libraries(fuzzdistillcc
        PRIVATE
        BBFeaturesPass
        FnFeaturesPass
        fuzzdistillcc_utils
        ${llvm_libs})

target_include_directories(fuzzdistillcc
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/BBFeaturesPass
        ${CMAKE_CURRENT_SOURCE_DIR}/FnFeaturesPass
        ${CMAKE_CURRENT_SOURCE_DIR}/Helpers
)

