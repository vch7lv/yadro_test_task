
cmake_minimum_required(VERSION 3.14)
project(tape_proj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(main)
add_subdirectory(tapelib)
add_subdirectory(tests)
