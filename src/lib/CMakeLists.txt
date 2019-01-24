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

add_library(servoarray_lib SHARED servoarray.cpp driver_manager.cpp user_config.cpp servomap.cpp)
set_target_properties(servoarray_lib PROPERTIES OUTPUT_NAME "servoarray")
set_target_properties(servoarray_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(servoarray_lib ${Boost_LIBRARIES} dl)
target_compile_definitions(servoarray_lib PRIVATE SERVOARRAY_DEFAULT_PATHS="~/.servoarray/",\ "/usr/lib/servoarray")
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine OUTPUT_VARIABLE target_triple OUTPUT_STRIP_TRAILING_WHITESPACE)
target_compile_definitions(servoarray_lib PRIVATE SERVOARRAY_DRIVER_POSTFIX=".servoarray-${servoarray_VERSION_MAJOR}-${target_triple}.so")
target_compile_definitions(servoarray_lib PRIVATE SERVOARRAY_DEFAULT_CONFIG_FILES="~/.servoarrayrc",\ "./.servoarrayrc")

sa_set_compile_options(servoarray_lib)

install(TARGETS servoarray_lib LIBRARY DESTINATION ${LIB_INSTALL_DIR})
