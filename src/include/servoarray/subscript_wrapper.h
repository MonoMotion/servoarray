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

#ifndef SERVOARRAY_SUBSCRIPT_WRAPPER_H
#define SERVOARRAY_SUBSCRIPT_WRAPPER_H

#include "servoarray/servoarray.h"

#include <cstdint>

namespace ServoArray {

class SubscriptWrapper {
  ServoArray* array_;
  std::size_t idx_;

public:
  SubscriptWrapper(ServoArray* array, std::size_t idx) : array_(array), idx_(idx) {}

  double operator=(double pos) {
    this->array_->write(this->idx_, pos);
    return pos;
  }

  operator double() {
    return this->array_->read(this->idx_);
  }
};

}

#endif
