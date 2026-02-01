cmake_minimum_required(VERSION 3.31)
project(MySTL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(MySTL main.cpp
        headers/containers.h
        headers/array.h
        applications/arrayOnly.cpp
)
