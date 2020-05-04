cmake_minimum_required(VERSION 3.15)

project(fast_io)

set(CMAKE_CXX_STANDARD 20)

add_library(fast_io_c_interface SHARED
    src/fast_io_c_interface.cc
)

target_include_directories(fast_io
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)


# Library
# Note: may not work on windows
install (TARGETS fast_io_c_interface
    LIBRARY DESTINATION lib)

# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)