cmake_minimum_required(VERSION 3.0)
project(GLGenerator)

set(CMAKE_CXX_STANDARD 14)

file(GLOB GENERATOR_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(GLGenerator ${GENERATOR_SOURCES})
