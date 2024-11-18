add_library(fuzzpiler_utils)
target_sources(fuzzpiler_utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fuzzpiler_utilities.cpp ${CMAKE_CURRENT_SOURCE_DIR}/fuzzpiler_utilities.h)

target_include_directories(fuzzpiler_utils PRIVATE ..)
