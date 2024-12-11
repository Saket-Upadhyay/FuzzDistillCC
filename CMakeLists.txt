cmake_minimum_required(VERSION 3.28)
project(FuzzDistillCC)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

# Are we using FuzzDistillCC to generate a dataset?
set(ISNISTTRAINING 0)
# Generate Feature CSVs with (1) or without (0) header
set(WRITEHEADERS 1)

find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_DIR}")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(LLVM REQUIRED CONFIG)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(LLVM REQUIRED CONFIG)
else ()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not supported yet")
endif ()


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# LLVM_RTTI Configuration
if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif ()

# Generate Feature CSVs with (1) or without (0) header
add_definitions(-DWRITEHEADER=${WRITEHEADERS})

# Are we using FuzzDistillCC to generate a dataset?
add_definitions(-DISNISTTRAININGSET=${ISNISTTRAINING})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include_directories(${LLVM_INCLUDE_DIRS} src/Helpers src/Helpers/utils)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(src)
