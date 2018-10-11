cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)

project(servoarray VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)

add_subdirectory(lib)
