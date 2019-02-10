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

execute_process(COMMAND uname -m OUTPUT_VARIABLE SERVOARRAY_TARGET_MACHINE OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND uname -o OUTPUT_VARIABLE SERVOARRAY_TARGET_OS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(TOLOWER ${SERVOARRAY_TARGET_OS} SERVOARRAY_TARGET_OS_PROCESSED)
string(REGEX REPLACE "[^a-z]" "_" SERVOARRAY_TARGET_OS_PROCESSED ${SERVOARRAY_TARGET_OS_PROCESSED})

set(SERVOARRAY_DRIVER_SUFFIX ".servoarray-${servoarray_VERSION_MAJOR}-${SERVOARRAY_TARGET_MACHINE}-${SERVOARRAY_TARGET_OS_PROCESSED}.so")

function(servoarray_add_driver target)
  cmake_parse_arguments(ARG "" "NAME" "" ${ARGN})

  if(ARG_NAME)
    set(name ${ARG_NAME})
  else()
    set(name ${target})
  endif()

  add_library(${target} SHARED ${ARG_UNPARSED_ARGUMENTS})
  set_target_properties(${target} PROPERTIES PREFIX "")
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${name}")
  set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
  set_target_properties(${target} PROPERTIES MACOSX_RPATH ON)
  set_target_properties(${target} PROPERTIES SUFFIX ${SERVOARRAY_DRIVER_SUFFIX})

  target_link_libraries(${target} ${SERVOARRAY_DRIVER_LIBRARIES})
  target_include_directories(${target} ${inc_system} PRIVATE ${SERVOARRAY_INCLUDE_DIR})
endfunction()
