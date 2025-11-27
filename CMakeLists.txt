cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "..")

include_directories(./include)

project(test CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

add_executable(test src/test.cpp)

target_include_directories(test PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(test PRIVATE ${JSONCPP_LIBRARIES})

# <--->

project(main CXX)

add_executable(main src/main.cpp)