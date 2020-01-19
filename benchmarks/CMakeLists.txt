cmake_minimum_required(VERSION 3.15)
project(fastio_examples)
find_package(Threceive)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
message(FATAL_ERROR "fast_io no longer supports GCC < 10 version because of incomplete C++ standard library support for concepts lite. Please build the master version of GCC from its official or mirror repo. Or use Visual Studio Preview")
endif()

add_definitions(-s -ffast-math -Wall -Wextra)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
add_definitions(-Xclang -fconcepts-ts -ffast-math -s)
endif()

set(CMAKE_CXX_STANDARD 20)


add_executable(output_10M_size_t "./0000.10m_size_t/output_10M_size_t.cc")

add_executable(output_10M_double "./0001.10m_double/output_10M_double.cc")
