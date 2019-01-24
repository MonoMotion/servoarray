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

#ifndef SERVOARRAY_SERVOARRAY_H
#define SERVOARRAY_SERVOARRAY_H

#include <cstdint>

#include "servoarray/driver.h"
#include "servoarray/driver_manager.h"

namespace ServoArray {

class ServoArray {
  std::shared_ptr<Driver> driver_;

public:
  ServoArray(std::shared_ptr<Driver>);
  ServoArray(const std::string& name = "", const DriverParams& = {}, DriverManager& = default_manager);

  void write(std::size_t index, double rad);
  double read(std::size_t index) const;

  std::size_t size() const;
};

}

#endif
