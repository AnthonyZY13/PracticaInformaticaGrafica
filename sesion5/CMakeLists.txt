cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME sesion5)
set(CODE_PATH src)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH ../binary)

project(${PROJECT_NAME})
include_directories(${CODE_PATH} /usr/include)
file(GLOB_RECURSE CODE_FILES ${CODE_PATH}/*.*)
add_executable(${PROJECT_NAME} ${CODE_FILES})
target_link_libraries(${PROJECT_NAME} GL GLEW glfw assimp freeimage)
