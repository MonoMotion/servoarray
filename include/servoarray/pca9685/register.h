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

#ifndef SERVOARRAY_REGISTER_H
#define SERVOARRAY_REGISTER_H

#include <cstdint>

namespace ServoArray {

enum class Register : std::uint8_t {
  MODE1 = 0x00,
  MODE2 = 0x01,
  SUBADR1 = 0x02,
  SUBADR2 = 0x03,
  SUBADR3 = 0x04,
  ALLCALLADR = 0x05,
  LED0 = 0x6,
  LED0_ON_L = 0x6,
  LED0_ON_H = 0x7,
  LED0_OFF_L = 0x8,
  LED0_OFF_H = 0x9,
  ALLLED_ON_L = 0xFA,
  ALLLED_ON_H = 0xFB,
  ALLLED_OFF_L = 0xFC,
  ALLLED_OFF_H = 0xFD,
  PRE_SCALE = 0xFE
};

Register operator+(const Register& reg, const std::uint8_t& offset);

}

#endif
