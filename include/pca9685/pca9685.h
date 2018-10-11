#ifndef SERVOARRAY_DRIVER_H
#define SERVOARRAY_DRIVER_H

#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <string>
#include <stdexcept>
#include <type_traits>

#include "register.h"

static constexpr float CLOCK_FREQ = 25000000.0;

class PCA9685 {
  std::uint8_t bus;
  std::uint8_t address;
  int fd;

public:
  PCA9685(std::uint8_t bus, std::uint8_t addr);

  void set_pwm_freq(float freq);
  void set_pwm(std::uint8_t index, std::uint16_t on, std::uint16_t off);
  std::uint8_t num_servos();

private:
  void reset();

  void write_reg(Register reg, std::uint8_t data);
  std::uint8_t read_reg(Register reg);

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  std::size_t write_seq(T const* data, std::size_t size) {
    const auto length = sizeof(T) * size;
    if(::write(this->fd, data, length) != length) {
      throw std::runtime_error("Failed to write byte");
    }
    return length;
  }

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  std::size_t write(T data) {
    if(::write(this->fd, &data, sizeof(T)) != sizeof(T)) {
      throw std::runtime_error("Failed to write byte");
    }
    return sizeof(T);
  }

  template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
  T read() {
    T buf;
    ::read(this->fd, &buf, sizeof(T));
    return buf;
  }
};

#endif
