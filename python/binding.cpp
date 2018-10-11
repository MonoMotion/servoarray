#include <pybind11/pybind11.h>

#include "servoarray/servoarray.h"

namespace py = pybind11;

PYBIND11_MODULE(servoarray, m) {
  m.doc() = "ServoArray: A fast implementation of servo motor array written in C++, also available as a python module";
  py::class_<ServoArray::ServoArray>(m, "ServoArray")
    .def(py::init<std::uint8_t, std::uint8_t, std::uint16_t, std::uint16_t>(), py::arg("bus") = 1, py::arg("address") = 0x40, py::arg("min_pulse") = 150, py::arg("max_pulse") = 600)
    .def("set", &ServoArray::ServoArray::set)
    .def("get", &ServoArray::ServoArray::get);
}

