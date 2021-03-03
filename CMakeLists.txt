cmake_minimum_required(VERSION 3.17)
project(Sparq)

set(CMAKE_CXX_STANDARD 14)

add_executable(SparQ src/main.cpp
                        src/LinkedList.cpp
                        src/LinkedList.h
                        src/Editor.cpp
                        src/Editor.h)