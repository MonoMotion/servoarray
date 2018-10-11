cmake_minimum_required(VERSION 2.7)

add_executable(set_pwm_bin set_pwm.cpp)
set_target_properties(set_pwm_bin PROPERTIES OUTPUT_NAME "set_pwm")
target_link_libraries(set_pwm_bin pca9685_lib)

add_executable(servo_array_bin servo_array.cpp)
set_target_properties(servo_array_bin PROPERTIES OUTPUT_NAME "servo_array")
target_link_libraries(servo_array_bin servoarray_lib)
