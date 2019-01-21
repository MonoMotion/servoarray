add_executable(set_pwm_bin set_pwm.cpp)
set_target_properties(set_pwm_bin PROPERTIES OUTPUT_NAME "set_pwm")
target_link_libraries(set_pwm_bin pca9685_lib)

