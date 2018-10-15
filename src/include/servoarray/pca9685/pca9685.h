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

#ifndef SERVOARRAY_DRIVER_H
#define SERVOARRAY_DRIVER_H

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <string>
#include <stdexcept>
#include <type_traits>
#include <array>

#include "register.h"

namespace ServoArray {

static constexpr float CLOCK_FREQ = 25000000.0;

class PCA9685 {
  std::uint8_t bus;
  std::uint8_t address;

  std::array<std::uint8_t, 64> buf;
  std::size_t buf_count;

  int fd;

public:
  PCA9685(std::uint8_t bus, std::uint8_t addr);

  void set_pwm_freq(float freq);
  void set_pwm(std::uint8_t index, std::uint16_t on, std::uint16_t off);
  std::uint8_t num_servos();

private:
  void reset();

  void write_reg(Register reg, std::uint8_t data);
  void write_reg_buf(Register reg, std::uint8_t data);
  void write_buf(std::uint8_t data);
  void flush_buf();
  std::uint8_t read_reg(Register reg);

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  std::size_t write_seq(T const* data, std::size_t size) {
    const auto length = sizeof(T) * size;
    const auto result = ::write(this->fd, data, length);
    if (result == -1) {
      throw this->errno_error<std::runtime_error>(errno);
    }
    if (static_cast<std::size_t>(result) != length) {
      throw std::runtime_error("Failed to write byte");
    }
    return length;
  }

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  std::size_t write(T data) {
    const auto result = ::write(this->fd, &data, sizeof(T));
    if (result == -1) {
      throw this->errno_error<std::runtime_error>(errno);
    }
    if (static_cast<std::size_t>(result) != sizeof(T)) {
      throw std::runtime_error("Failed to write byte");
    }
    return sizeof(T);
  }

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  T read() {
    T buf;
    const auto result = ::read(this->fd, &buf, sizeof(T));
    if (result == -1) {
      throw this->errno_error<std::runtime_error>(errno);
    }
    if (static_cast<std::size_t>(result) != sizeof(T)) {
      throw std::runtime_error("Failed to read byte");
    }
    return buf;
  }

  template<typename T>
  T errno_error(int errnum) {
    char errbuf[64];
    ::strerror_r(errnum, errbuf, 64);
    return T{errbuf};
  }

};

}

#endif
