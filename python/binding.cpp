// This file is part of servoarray.
//
// servoarray is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
/
// servoarray is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

#include <pybind11/pybind11.h>

#include "servoarray/servoarray.h"

namespace py = pybind11;

PYBIND11_MODULE(servoarray, m) {
  m.doc() = "ServoArray: A fast implementation of servo motor array written in C++, also available as a python module";
  py::class_<ServoArray::ServoArray>(m, "ServoArray")
    .def(py::init<std::uint8_t, std::uint8_t, std::uint16_t, std::uint16_t>(), py::arg("bus") = 1, py::arg("address") = 0x40, py::arg("min_pulse") = 150, py::arg("max_pulse") = 600)
    .def("set", &ServoArray::ServoArray::set)
    .def("get", &ServoArray::ServoArray::get)
    .def("__len__", &ServoArray::ServoArray::size)
    .def("__setitem__", &ServoArray::ServoArray::set)
    .def("__getitem__", &ServoArray::ServoArray::get);
}

