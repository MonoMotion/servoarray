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
#include <vector>

#include "servoarray/pca9685/pca9685.h"

namespace ServoArray {

class ServoArray {
  PCA9685 driver;
  std::uint16_t min_pulse;
  std::uint16_t max_pulse;

  std::vector<double> values;

public:
  ServoArray(std::uint8_t bus=1, std::uint8_t address=0x40, std::uint16_t min_pulse=150, std::uint16_t max_pulse=600);

  void set(std::uint8_t index, double rad);
  double get(std::uint8_t index);

  std::uint8_t size();
};

}

#endif
