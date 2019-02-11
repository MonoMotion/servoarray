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

function(sa_add_bin target)
  cmake_parse_arguments(ARG "INSTALL" "NAME" "" ${ARGN})

  if(ARG_NAME)
    set(name ${ARG_NAME})
  else()
    set(name ${target})
  endif()

  add_executable(${target}_bin ${target}.cpp)
  set_target_properties(${target}_bin PROPERTIES OUTPUT_NAME "${name}")
  target_link_libraries(${target}_bin servoarray_lib)
  sa_set_compile_options(${target}_bin)

  if(${ARG_INSTALL})
    install(TARGETS ${target}_bin RUNTIME DESTINATION bin)
  endif()
endfunction()
