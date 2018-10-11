cmake_minimum_required(VERSION 2.7)

add_library(pca9685 STATIC pca9685.cpp register.cpp)
add_library(servoarray STATIC servoarray.cpp)
target_link_libraries(servoarray pca9685)
