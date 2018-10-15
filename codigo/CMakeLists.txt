cmake_minimum_required(VERSION 3.7)
project(Codigo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp basics.h bellman-ford.cpp bellman-ford.h floyd-warshall.cpp floyd-warshall.h prim.cpp prim.h kruskal.cpp kruskal.h)

add_executable(Codigo ${SOURCE_FILES})