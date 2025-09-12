cmake_minimum_required(VERSION 3.16)
project(Matrix)

set(CMAKE_CXX_STANDARD 23)

add_library(Matrix STATIC source/Matrix.cpp)

target_include_directories(Matrix PUBLIC include)