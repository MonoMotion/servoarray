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

cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)

project(servoarray VERSION 0.7.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_CPP_STANDARD -std=c++14)

if(NOT CONFIG)
  set(CONFIG Release)
endif()

set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/)

include(config/servoarrayTools.cmake)  # sets SERVOARRAY_DRIVER_SUFFIX
set(SERVOARRAY_DEFAULT_DRIVER_PATHS ~/.servoarray/ ${CMAKE_INSTALL_PREFIX}/lib/servoarray)
set(SERVOARRAY_DEFAULT_CONFIG_FILES ~/.servoarrayrc ./.servoarrayrc)

option(BUILD_PYTHON "Build servoarray python module" ON)

find_package(Boost 1.36.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include/)
include_directories(${CMAKE_BINARY_DIR}/include/)
include_directories(SYSTEM third_party/toml11)

include(cmake/compile-options.cmake)
include(cmake/cpack.cmake)

add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(include)

add_subdirectory(config)
add_subdirectory(examples)

if (${BUILD_PYTHON})
  add_subdirectory(third_party/pybind11)
  # pybind11 headers produce so many errors with strict compiler options
  include_directories(SYSTEM ${PYBIND11_INCLUDE_DIR})

  add_subdirectory(python)
endif()
