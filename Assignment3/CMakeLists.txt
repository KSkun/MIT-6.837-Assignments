cmake_minimum_required(VERSION 3.20)
project(Assignment3)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC "*.h" "*.cpp" "*.C")

add_executable(Assignment3 ${SRC})
