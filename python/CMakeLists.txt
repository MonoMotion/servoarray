cmake_minimum_required(VERSION 2.7)

pybind11_add_module(servoarray SHARED binding.cpp)
target_link_libraries(servoarray servoarray_lib)
