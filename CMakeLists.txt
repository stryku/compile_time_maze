cmake_minimum_required(VERSION 3.6)
project(maze)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp hash.hpp maze.hpp utils.hpp path_finder.hpp pos.hpp)
add_executable(maze ${SOURCE_FILES})