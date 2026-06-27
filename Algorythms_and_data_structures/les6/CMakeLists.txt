cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(dfs)

add_executable(dfs
 ${CMAKE_SOURCE_DIR}/main.cpp
 ${CMAKE_SOURCE_DIR}/graph.h
 )
