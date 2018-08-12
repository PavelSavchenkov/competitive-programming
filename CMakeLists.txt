cmake_minimum_required(VERSION 3.12)
project(competitive-programming)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-stack_size,0x100000000")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOCAL -Wall -Wextra ")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")  # remove NDEBUG and -O3, replace with own flags

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(prewritten-code)
add_subdirectory(tasks)
