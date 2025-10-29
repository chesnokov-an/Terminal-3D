cmake_minimum_required(VERSION 3.16)
project(Main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")

add_executable (Main source/main.cpp)

target_link_libraries(Main Matrix Polyline Buffer Utils Dialogue)