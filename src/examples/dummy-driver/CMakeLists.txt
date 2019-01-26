include(${CMAKE_SOURCE_DIR}/config/servoarrayTools.cmake)

servoarray_add_driver(dummy dummy.cpp)
sa_set_compile_options(dummy)
