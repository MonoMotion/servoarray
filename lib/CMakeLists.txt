cmake_minimum_required(VERSION 2.7)

add_library(pca9685_lib STATIC pca9685.cpp register.cpp)
set_target_properties(pca9685_lib PROPERTIES OUTPUT_NAME "pca9685")
add_library(servoarray_lib STATIC servoarray.cpp)
set_target_properties(servoarray_lib PROPERTIES OUTPUT_NAME "servoarray")
target_link_libraries(servoarray_lib pca9685_lib)
