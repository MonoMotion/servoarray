#ifndef SERVOARRAY_SERVOARRAY_H
#define SERVOARRAY_SERVOARRAY_H

#include <cstdint>
#include <vector>

#include "pca9685/pca9685.h"

class ServoArray {
  PCA9685 driver;
  std::uint16_t max_pulse;
  std::uint16_t min_pulse;

  std::vector<double> values;

public:
  ServoArray(std::uint8_t bus=1, std::uint8_t address=0x40, std::uint16_t min_pulse=150, std::uint16_t max_pulse=600);

  void set(std::uint8_t index, double rad);
  double get(std::uint8_t index);
};

#endif
