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

macro(sa_convlist var)
  foreach(path ${ARGN})
    if(${var})
      set(${var} "${${var}}, \"${path}\"")
    else()
      set(${var} "\"${path}\"")
    endif()
  endforeach()
endmacro()

sa_convlist(CONFIG_SERVOARRAY_DEFAULT_CONFIG_FILES ${SERVOARRAY_DEFAULT_CONFIG_FILES})
sa_convlist(CONFIG_SERVOARRAY_DEFAULT_DRIVER_PATHS ${SERVOARRAY_DEFAULT_DRIVER_PATHS})
configure_file(servoarray/config.h.in servoarray/config.h)

install(DIRECTORY . DESTINATION ${INCLUDE_INSTALL_DIR} FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/servoarray/config.h DESTINATION ${INCLUDE_INSTALL_DIR}/servoarray)
