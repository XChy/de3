project(de3)
cmake_minimum_required(VERSION 3.14)

option(BUILD_TEST "Set to complie the test samples" ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(
  llvm_libs
  aggressiveinstcombine
  analysis
  binaryformat
  bitreader
  bitstreamreader
  bitwriter
  cfguard
  core
  coroutines
  filecheck
  instcombine
  instrumentation
  ipo
  irprinter
  irreader
  libdriver
  linker
  object
  option
  passes
  scalaropts
  support
  symbolize
  target
  targetparser
  textapi
  transformutils
  vectorize)

message(STATUS "LLVM LIBS: ${llvm_libs}")

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-unused-variable  -fno-rtti -pthread -O0 -g -ggdb -std=c++20"
)
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -std=c++20 -fno-rtti -O1 -pthread -fopenmp")

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

include_directories(./src)
file(
  GLOB_RECURSE
  CORE_SRC
  "src/transforms/*.cpp"
  "src/transforms/*.h"
  "src/utils/*.h"
  "src/utils/*.cpp"
  "src/indicators/*.h"
  "src/indicators/*.cpp")

add_library(UnoptGenCore STATIC ${CORE_SRC})

add_subdirectory(src/tools/deoptgen)
add_subdirectory(src/tools/moclassify)
add_subdirectory(src/tools/mochecker)
add_subdirectory(src/tools/mo-diff)


set(CMAKE_CXX_STANDARD 20)
