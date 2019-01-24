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

#include "servoarray/servoarray.h"

namespace ServoArray {

ServoArray::ServoArray(std::shared_ptr<Driver> driver) : driver_(driver) {}
ServoArray::ServoArray(const std::string& name, const DriverParams& params, DriverManager& manager) : driver_(manager.load(name, params)) {}

void ServoArray::write(std::size_t index, double rad) {
  this->driver_->write(index, rad);
}

double ServoArray::read(std::size_t index) const {
  return this->driver_->read(index);
}

void ServoArray::set_read_mode(ReadMode mode) {
  this->read_mode_ = mode;
}

ReadMode ServoArray::read_mode() const {
  return this->read_mode_;
}

std::size_t ServoArray::size() const {
  return this->driver_->size();
}

}
