cmake_minimum_required(VERSION 3.16)
project(Buffer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_library(Buffer STATIC source/Buffer.cpp)

target_include_directories(Buffer PUBLIC include)