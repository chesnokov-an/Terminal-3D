cmake_minimum_required(VERSION 3.16)
project(Polyline)

set(CMAKE_CXX_STANDARD 23)

add_library(Polyline STATIC source/Polyline.cpp)

target_include_directories(Polyline PUBLIC include)
