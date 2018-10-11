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

class PCA9685 {
  std::uint8_t bus;
  std::uint8_t address;
  int fd;

public:
  PCA9685(std::uint8_t bus, std::uint8_t addr) : bus(bus), address(addr) {
    char dev_path[64];
    std::sprintf(dev_path, "/dev/i2c-%d", this->bus);
    if ((this->fd = ::open(dev_path, O_RDWR)) < 0) {
      throw std::runtime_error("Faild to open i2c device file " + std::string{ dev_path });
    }

    if (ioctl(this->fd, I2C_SLAVE, this->address) < 0) {
      throw std::runtime_error("Unable to get bus access to talk to slave");
    }
  }

private:
  void write_reg(std::uint8_t reg, std::uint8_t data) {
    uint8_t buf[2] = {reg, data};
    this->write_seq(buf, 2);
  }

  std::uint8_t read_reg(std::uint8_t reg) {
    this->write(reg);
    return this->read<std::uint8_t>();
  }

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
