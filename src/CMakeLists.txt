cmake_minimum_required(VERSION 2.7)

add_executable(set_pwm set_pwm.cpp)
target_link_libraries(set_pwm pca9685)

add_executable(servo_array servo_array.cpp)
target_link_libraries(servo_array servoarray)
