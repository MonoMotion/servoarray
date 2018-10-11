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

add_library(pca9685_lib STATIC pca9685.cpp register.cpp)
set_target_properties(pca9685_lib PROPERTIES OUTPUT_NAME "pca9685")
set_target_properties(pca9685_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(servoarray_lib STATIC servoarray.cpp)
set_target_properties(servoarray_lib PROPERTIES OUTPUT_NAME "servoarray")
set_target_properties(servoarray_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(servoarray_lib pca9685_lib)
