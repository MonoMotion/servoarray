#include <cstdint>

#include "pca9685/register.h"

Register operator+(const Register& reg, const std::uint8_t& offset) {
  const auto address = static_cast<std::uint8_t>(reg) + offset;
  return Register(address);
}
