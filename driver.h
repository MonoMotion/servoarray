#ifndef SERVOARRAY_DRIVER_H
#define SERVOARRAY_DRIVER_H

#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <string>
#include <stdexcept>

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
  void write_byte(std::uint8_t data) {
    if(this->write(&data, 1) != 1) {
      throw std::runtime_error("Failed to write byte");
    }
  }

  std::uint8_t read_byte() {
    std::uint8_t data;
    if(this->read(&data, 1) != 1) {
      throw std::runtime_error("Failed to read byte");
    }
    return data;
  }

  std::size_t write(const void* buf, std::size_t count) {
    return ::write(this->fd, buf, count);
  }

  std::ssize_t read(void* buf, std::size_t count) {
    return ::read(this->fd, buf, count);
  }
};

#endif
