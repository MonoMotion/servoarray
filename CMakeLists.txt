cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)

project(servoarray VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_CPP_STANDARD -std=c++14)

include_directories(include/)

add_subdirectory(lib)
add_subdirectory(src)

add_subdirectory(pybind11)

add_subdirectory(python)
