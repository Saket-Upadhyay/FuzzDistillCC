add_library(fuzzdistillcc_utils)
target_sources(fuzzdistillcc_utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fuzzdistillcc_utilities.cpp ${CMAKE_CURRENT_SOURCE_DIR}/fuzzdistillcc_utilities.h)

target_include_directories(fuzzdistillcc_utils PRIVATE ..)
