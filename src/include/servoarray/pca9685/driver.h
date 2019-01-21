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

#ifndef SERVOARRAY_PCA9685_DRIVER_H
#define SERVOARRAY_PCA9685_DRIVER_H

#include <unistd.h>
#include <servoarray/driver.h>

#include "pca9685/pca9685.h"

namespace pca9685 {

class PCA9685Driver final : public ServoArray::Driver {
  PCA9685 pca9685_;
  std::uint16_t min_pulse_;
  std::uint16_t max_pulse_;

public:
  PCA9685Driver(std::uint8_t bus, std::uint8_t address, std::uint16_t min_pulse=150, std::uint16_t max_pulse=600);

  void write(std::size_t, double) override;

  std::size_t size() const override;
};

}

extern "C" ServoArray::Driver* servoarray_driver(const ServoArray::DriverParams&);

#endif
