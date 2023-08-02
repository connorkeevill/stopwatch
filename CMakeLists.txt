cmake_minimum_required(VERSION 3.26)
project(stopwatch)

set(CMAKE_CXX_STANDARD 20)

add_executable(stopwatch main.cpp
        Stopwatch.hpp)
