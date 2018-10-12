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

  void set(std::int16_t index, double rad) {
    const auto u8idx = this->cast_index<uint8_t>(index);
    if (index >= 0) {
      return this->sa.set(u8idx, rad);
    } else {
      return this->sa.set(static_cast<uint8_t>(this->sa.size() + index), rad);
    }
  }
  void set(py::slice slice, py::list list) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    if (py::len(list) != length) {
      throw std::out_of_range("Can't assign the different length of list from slice");
    }
    for (auto const& elem : list) {
      const auto idx = this->cast_index<uint8_t>(start);
      this->sa.set(idx, elem.cast<double>());
      start += step;
    }
  }
  double get(std::int16_t index) {
    const auto u8idx = this->cast_index<uint8_t>(index);
    if (index >= 0) {
      return this->sa.get(u8idx);
    } else {
      return this->sa.get(static_cast<uint8_t>(this->sa.size() + index));
    }
  }
  py::list get(py::slice slice) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    auto l = py::list();
    for (std::size_t i = 0; i < length; ++i) {
      const auto idx = this->cast_index<uint8_t>(start);
      l.append(this->sa.get(idx));
      start += step;
    }
    return l;
  }

  std::uint8_t size() { return this->sa.size(); }

private:
  template<typename Int, typename T, std::enable_if_t<std::numeric_limits<T>::is_signed>* = nullptr>
  Int cast_index(T v) {
    static_assert(sizeof(Int) < sizeof(T), "check_index is only available to convert larger integer to smaller one");
    if (std::numeric_limits<Int>::max() < v || -std::numeric_limits<Int>::max() > v) {
      throw std::out_of_range("Index out of bounds");
    }
    return static_cast<Int>(v);
  }

  template<typename Int, typename T, std::enable_if_t<!std::numeric_limits<T>::is_signed>* = nullptr>
  Int cast_index(T v) {
    static_assert(sizeof(Int) < sizeof(T), "check_index is only available to convert larger integer to smaller one");
    if (std::numeric_limits<Int>::max() < v) {
      throw std::out_of_range("Index out of bounds");
    }
    return static_cast<Int>(v);
  }
};

}

PYBIND11_MODULE(servoarray, m) {
  m.doc() = "ServoArray: A fast implementation of servo motor array written in C++, also available as a python module";
  py::class_<Adaptor::ServoArray>(m, "ServoArray")
    .def(py::init<std::uint8_t, std::uint8_t, std::uint16_t, std::uint16_t>(), py::arg("bus") = 1, py::arg("address") = 0x40, py::arg("min_pulse") = 150, py::arg("max_pulse") = 600)
    .def("set", py::overload_cast<std::int16_t, double>(&Adaptor::ServoArray::set))
    .def("get", py::overload_cast<std::int16_t>(&Adaptor::ServoArray::get))
    .def("__len__", &Adaptor::ServoArray::size)
    .def("__setitem__", py::overload_cast<py::slice, py::list>(&Adaptor::ServoArray::set))
    .def("__setitem__", py::overload_cast<std::int16_t, double>(&Adaptor::ServoArray::set))
    .def("__getitem__", py::overload_cast<py::slice>(&Adaptor::ServoArray::get))
    .def("__getitem__", py::overload_cast<std::int16_t>(&Adaptor::ServoArray::get));
}

