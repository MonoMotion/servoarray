// This file is part of servoarray.
//
// servoarray is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// servoarray is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

#include <utility>
#include <cstdlib>

#include <pybind11/pybind11.h>

#include "servoarray/servoarray.h"

namespace py = pybind11;

namespace Adaptor {

class ServoArray {
  ::ServoArray::ServoArray sa;

public:
  template<typename... Ts>
  ServoArray(Ts&&... params) : sa(::ServoArray::ServoArray(std::forward<Ts>(params)...)) {}

  void set(std::uint8_t index, double rad) { this->sa.set(index, rad); }
  double get(std::uint8_t index) { return this->sa.get(index); }
  py::list get(py::slice slice) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    auto l = py::list();
    for (std::size_t i = 0; i < length; ++i) {
      l.append(this->sa.get(start));
      start += step;
    }
    return l;
  }

  std::uint8_t size() { return this->sa.size(); }
};

}

PYBIND11_MODULE(servoarray, m) {
  m.doc() = "ServoArray: A fast implementation of servo motor array written in C++, also available as a python module";
  py::class_<Adaptor::ServoArray>(m, "ServoArray")
    .def(py::init<std::uint8_t, std::uint8_t, std::uint16_t, std::uint16_t>(), py::arg("bus") = 1, py::arg("address") = 0x40, py::arg("min_pulse") = 150, py::arg("max_pulse") = 600)
    .def("set", &Adaptor::ServoArray::set)
    .def("get", py::overload_cast<std::uint8_t>(&Adaptor::ServoArray::get))
    .def("__len__", &Adaptor::ServoArray::size)
    .def("__setitem__", &Adaptor::ServoArray::set)
    .def("__getitem__", py::overload_cast<py::slice>(&Adaptor::ServoArray::get))
    .def("__getitem__", py::overload_cast<std::uint8_t>(&Adaptor::ServoArray::get));
}

