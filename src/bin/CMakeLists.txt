# This file is part of servoarray.
#
# servoarray is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# servoarray is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.7)

add_executable(set_pwm_bin set_pwm.cpp)
set_target_properties(set_pwm_bin PROPERTIES OUTPUT_NAME "set_pwm")
target_link_libraries(set_pwm_bin pca9685_lib)

add_executable(servo_array_bin servo_array.cpp)
set_target_properties(servo_array_bin PROPERTIES OUTPUT_NAME "servo_array")
target_link_libraries(servo_array_bin servoarray_lib)

add_executable(bench_bin bench.cpp)
set_target_properties(bench_bin PROPERTIES OUTPUT_NAME "bench")
target_link_libraries(bench_bin servoarray_lib)
