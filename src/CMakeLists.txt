cmake_minimum_required(VERSION 2.7)

add_executable(set_pwm set_pwm.cpp)
target_link_libraries(set_pwm pca9685)
