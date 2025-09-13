cmake_minimum_required(VERSION 3.16)
project(Matrix)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")

add_library(Matrix INTERFACE)

target_include_directories(Matrix INTERFACE include)