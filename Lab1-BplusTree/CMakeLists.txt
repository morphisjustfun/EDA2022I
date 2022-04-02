cmake_minimum_required(VERSION 3.21)
project(Lab1_BplusTree)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Lab1_BplusTree
        src/BplusTree.cpp
        src/BplusTree.h
        src/main.cpp)
