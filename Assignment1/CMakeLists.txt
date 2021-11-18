cmake_minimum_required(VERSION 3.20)
project(Assignment1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Assignment1 main.cpp hit.h material.h ray.h scene_parser.C scene_parser.h object3d.h
        vectors.h matrix.C matrix.h camera.cpp camera.h sphere.cpp sphere.h group.cpp group.h
        image.C image.h)
