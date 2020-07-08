cmake_minimum_required(VERSION 3.15)

# project(fastio_examples)
set(CMAKE_FOLDER "testsuites")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
message(FATAL_ERROR "fast_io no longer supports GCC < 10 version because of incomplete C++ standard library support for concepts lite. Please build the master version of GCC from its official or mirror repo. Or use Visual Studio Preview")
endif()

add_definitions(-ffast-math -Wall -Wextra -s)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
add_definitions(-Xclang -fconcepts-ts -ffast-math -s)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(ebcdic 0000.transforms/ebcdic.cc)