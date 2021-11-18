cmake_minimum_required(VERSION 3.20)
project(Assignment0)

set(CMAKE_CXX_STANDARD 20)
#add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(Assignment0 image.C matrix.C main.cpp ifs.cpp ifs.h random.cpp random.h)
