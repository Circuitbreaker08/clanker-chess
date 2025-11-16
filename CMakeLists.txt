cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

include_directories(./include)

project(hello CXX)

add_executable(test src/hello.cpp)