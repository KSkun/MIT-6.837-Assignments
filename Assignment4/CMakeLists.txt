cmake_minimum_required(VERSION 3.20)
project(Assignment4)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC "*.h" "*.cpp" "*.C")

add_executable(${PROJECT_NAME} ${SRC})

# OpenGL dependencies
set(GLUT_ROOT_PATH glut)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
