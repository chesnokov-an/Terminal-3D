cmake_minimum_required(VERSION 3.16)
project(Buffer)

set(CMAKE_CXX_STANDARD 23)

add_library(Buffer STATIC source/Buffer.cpp)

target_include_directories(Buffer PUBLIC include)