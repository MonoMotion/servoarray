#ifndef SERVOARRAY_REGISTER_H
#define SERVOARRAY_REGISTER_H

#include <cstdint>

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

#endif
