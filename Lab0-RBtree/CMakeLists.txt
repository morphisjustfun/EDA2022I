cmake_minimum_required(VERSION 3.10)
project(RBtree)

set(CMAKE_CXX_STANDARD 11)

include_directories(./src)
add_executable(main
        src/main.cpp
        src/rbtree.cpp
        src/rbtree.h
        )
