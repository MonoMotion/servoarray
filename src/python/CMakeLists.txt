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

pybind11_add_module(servoarray SHARED binding.cpp)
target_link_libraries(servoarray servoarray_lib)
sa_set_compile_options(servoarray)

install(TARGETS servoarray LIBRARY DESTINATION ${LIB_INSTALL_DIR})
