cmake_minimum_required(VERSION 3.16)
project(Dialogue)

set(CMAKE_CXX_STANDARD 23)

add_executable (Dialogue source/main.cpp)

target_link_libraries(Dialogue Matrix)