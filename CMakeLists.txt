cmake_minimum_required(VERSION 3.1)
project(post-mortem)
add_definitions(-std=c++14 -fno-rtti -fno-exceptions -fvisibility=hidden -fvisibility-inlines-hidden -Wall)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--wrap=malloc")
add_library(post-mortem SHARED src/main.cpp)

add_executable(post-mortem-log src/reader.cpp)
add_definitions(-ggdb)
add_executable(test-cpp tests/delete.cpp)
