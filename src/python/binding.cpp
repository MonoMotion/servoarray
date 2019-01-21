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
#include <algorithm>
#include <cstdlib>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "servoarray/servoarray.h"

namespace py = pybind11;

namespace Adaptor {

ServoArray::DriverParams to_driver_params(py::dict py_params) {
  ServoArray::DriverParams params;
  for (auto item : py_params) {
    auto const& py_key = item.first;
    auto const& py_value = item.second;

    if(!py::isinstance<py::str>(py_key)) {
      throw std::runtime_error("Parameter dict must have string keys");
    }
    auto key = py::cast<std::string>(py_key);

    if (py::isinstance<py::int_>(py_value)) {
      params.emplace<int>(key, py::cast<int>(py_value));
    } else if (py::isinstance<py::float_>(py_value)) {
      params.emplace<float>(key, py::cast<float>(py_value));
    } else if (py::isinstance<py::bool_>(py_value)) {
      params.emplace<bool>(key, py::cast<bool>(py_value));
    } else {
      throw std::runtime_error("Unsupported parameter value " + std::string(py::str(py_value)));
    }
  }

  return params;
}

class ServoArray {
  ::ServoArray::ServoArray sa;

public:
  ServoArray(const std::string& name, py::dict py_params) : sa(name, to_driver_params(py_params)) {}

  void write(std::int16_t index, double rad) {
    const auto u8idx = this->cast_index<uint8_t>(index);
    if (index >= 0) {
      return this->sa.write(u8idx, rad);
    } else {
      return this->sa.write(static_cast<uint8_t>(this->sa.size() + index), rad);
    }
  }
  void write(py::slice slice, py::array_t<double> list) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    if (static_cast<std::size_t>(list.size()) != length) {
      throw std::out_of_range("Can't assign the different length of list from slice");
    }
    for (std::size_t i = 0; i < length; ++i) {
      const auto idx = this->cast_index<uint8_t>(start);
      double rad = list.at(i);
      this->sa.write(idx, rad);
      start += step;
    }
  }
  double read(std::int16_t index) {
    const auto u8idx = this->cast_index<uint8_t>(index);
    if (index >= 0) {
      return this->sa.read(u8idx);
    } else {
      return this->sa.read(static_cast<uint8_t>(this->sa.size() + index));
    }
  }
  py::array_t<double> read(py::slice slice) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    auto l = py::array_t<double>(length);
    for (std::size_t i = 0; i < length; ++i) {
      const auto idx = this->cast_index<uint8_t>(start);
      l.mutable_at(i) = this->sa.read(idx);
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
    .def(py::init<const std::string&, py::dict>())
    .def("write", py::overload_cast<std::int16_t, double>(&Adaptor::ServoArray::write))
    .def("read", py::overload_cast<std::int16_t>(&Adaptor::ServoArray::read))
    .def("__len__", &Adaptor::ServoArray::size)
    .def("__writeitem__", py::overload_cast<py::slice, py::array_t<double>>(&Adaptor::ServoArray::write))
    .def("__writeitem__", py::overload_cast<std::int16_t, double>(&Adaptor::ServoArray::write))
    .def("__readitem__", py::overload_cast<py::slice>(&Adaptor::ServoArray::read))
    .def("__readitem__", py::overload_cast<std::int16_t>(&Adaptor::ServoArray::read));
}

