cmake_minimum_required(VERSION 3.1)
project(post-mortem)
find_package( Threads )

add_definitions(-std=c++14 -fno-rtti -fno-exceptions -fvisibility=hidden -fvisibility-inlines-hidden -Wall)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--wrap=malloc")
add_library(post-mortem SHARED src/main.cpp)
target_link_libraries(post-mortem ${CMAKE_THREAD_LIBS_INIT})

add_executable(post-mortem-log src/reader.cpp)
