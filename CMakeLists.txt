cmake_minimum_required(VERSION 3.12)
project(competitive-programming)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-stack_size,0x100000000")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -O2 -DLOCAL")
unset(CMAKE_CXX_FLAGS_RELEASE CACHE)  # remove NDEBUG and -O3

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(prewritten-code)
add_subdirectory(tasks)
