cmake_minimum_required(VERSION 2.8)
project(philosophers_dinning)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

set (CMAKE_CXX_FLAGS "-pthread")

include_directories(include)

add_executable(philosophers_dinning src/main.cpp include/philosopher.h include/fork.h include/topic.h src/philosopher.cpp src/topic.cpp src/fork.cpp)