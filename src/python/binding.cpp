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
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "servoarray/servoarray.h"
#include "servoarray/servomap.h"

namespace py = pybind11;

namespace Adaptor {

static ServoArray::DriverParams to_driver_params(py::dict py_params) {
  ServoArray::DriverParams params;
  for (auto item : py_params) {
    auto const& py_key = item.first;
    auto const& py_value = item.second;

    if(!py::isinstance<py::str>(py_key)) {
      throw std::runtime_error("Parameter dict must have string keys");
    }
    auto key = py::cast<std::string>(py_key);

    if (py::isinstance<py::int_>(py_value)) {
      params.put(key, py::cast<int>(py_value));
    } else if (py::isinstance<py::float_>(py_value)) {
      params.put(key, py::cast<float>(py_value));
    } else if (py::isinstance<py::str>(py_value)) {
      params.put(key, py::cast<std::string>(py_value));
    } else if (py::isinstance<py::bool_>(py_value)) {
      params.put(key, py::cast<bool>(py_value));
    } else {
      throw std::runtime_error("Unsupported parameter value " + std::string(py::str(py_value)));
    }
  }

  return params;
}

class ServoArray {
public:
  ::ServoArray::ServoArray sa;

  ServoArray(const std::string& name, py::dict py_params, ::ServoArray::DriverManager& manager) : sa(name, to_driver_params(py_params), manager) {}

  void write(std::int16_t index, double rad) {
    if (index >= 0) {
      return this->sa.write(index, rad);
    } else {
      return this->sa.write(this->sa.size() + index, rad);
    }
  }
  void write_slice(py::slice slice, py::array_t<double> list) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    if (static_cast<std::size_t>(list.size()) != length) {
      throw std::out_of_range("Can't assign the different length of list from slice");
    }
    for (std::size_t i = 0; i < length; ++i) {
      double rad = list.at(i);
      this->sa.write(start, rad);
      start += step;
    }
  }
  double read(std::int16_t index) {
    if (index >= 0) {
      return this->sa.read(index);
    } else {
      return this->sa.read(this->sa.size() + index);
    }
  }
  py::array_t<double> read_slice(py::slice slice) {
    std::size_t start, stop, step, length;
    if (!slice.compute(this->sa.size(), &start, &stop, &step, &length))
      throw py::error_already_set();

    auto l = py::array_t<double>(length);
    for (std::size_t i = 0; i < length; ++i) {
      l.mutable_at(i) = this->sa.read(start);
      start += step;
    }
    return l;
  }

  std::uint8_t size() { return this->sa.size(); }

  void set_read_mode(::ServoArray::ReadMode mode) { this->sa.set_read_mode(mode); }
  ::ServoArray::ReadMode read_mode() const { return this->sa.read_mode(); }
};

}

PYBIND11_MODULE(servoarray, m) {
  m.doc() = "ServoArray: A fast implementation of servo motor array written in C++, also available as a python module";

  py::class_<::ServoArray::DriverManager>(m, "DriverManager")
    .def(py::init<const std::vector<std::string>&, bool>(), py::arg("paths"), py::arg("load_defaults") = true)
    .def("load", [](::ServoArray::DriverManager& manager, const std::string& name, py::dict params) {
        return manager.load(name, Adaptor::to_driver_params(params));
      })
    .def("get", &::ServoArray::DriverManager::get)
    .def("append_search_path", &::ServoArray::DriverManager::append_search_path)
    .def("load_config_file", [](::ServoArray::DriverManager& manager, const std::string& path) {
        manager.load_user_config(::ServoArray::UserConfig{path});
      })
    .def("is_loaded", &::ServoArray::DriverManager::is_loaded);

  py::enum_<::ServoArray::ReadMode>(m, "ReadMode")
    .value("Cached", ::ServoArray::ReadMode::Cached)
    .value("Direct", ::ServoArray::ReadMode::Direct);

  py::class_<Adaptor::ServoArray>(m, "ServoArray")
    .def(py::init<const std::string&, py::dict, ::ServoArray::DriverManager&>(), py::arg("name") = "", py::arg("params") = py::dict(), py::arg("manager") = ::ServoArray::default_manager)
    .def("write", &Adaptor::ServoArray::write)
    .def("read", &Adaptor::ServoArray::read)
    .def_property("read_mode", &Adaptor::ServoArray::read_mode, &Adaptor::ServoArray::set_read_mode)
    .def("__len__", &Adaptor::ServoArray::size)
    .def("__setitem__", &Adaptor::ServoArray::write_slice)
    .def("__setitem__", &Adaptor::ServoArray::write)
    .def("__getitem__", &Adaptor::ServoArray::read_slice)
    .def("__getitem__", &Adaptor::ServoArray::read);

  py::class_<::ServoArray::ServoMap>(m, "ServoMap")
    .def(py::init([](const Adaptor::ServoArray& array, const std::unordered_map<std::string, std::size_t>& names) {
          return ::ServoArray::ServoMap(array.sa, names);
        }))
    .def(py::init([](const Adaptor::ServoArray& array, ::ServoArray::DriverManager& manager) {
          return ::ServoArray::ServoMap(array.sa, manager);
        }), py::arg("array"), py::arg("manager") = ::ServoArray::default_manager)
    .def("write", &::ServoArray::ServoMap::write)
    .def("read", &::ServoArray::ServoMap::read)
    .def_property_readonly("array", &::ServoArray::ServoMap::array)
    .def_property_readonly("names", &::ServoArray::ServoMap::names)
    .def("__len__", &::ServoArray::ServoMap::size)
    .def("__setitem__", &::ServoArray::ServoMap::write)
    .def("__getitem__", &::ServoArray::ServoMap::read);
}

