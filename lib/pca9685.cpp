#include "pca9685/pca9685.h"

PCA9685::PCA9685(std::uint8_t bus, std::uint8_t addr) : bus(bus), address(addr) {
  char dev_path[64];
  std::sprintf(dev_path, "/dev/i2c-%d", this->bus);
  if ((this->fd = ::open(dev_path, O_RDWR)) < 0) {
    throw std::runtime_error("Faild to open i2c device file " + std::string{ dev_path });
  }

  if (ioctl(this->fd, I2C_SLAVE, this->address) < 0) {
    throw std::runtime_error("Unable to get bus access to talk to slave");
  }
  this->reset();
  this->set_pwm_freq(40);
}

void PCA9685::set_pwm_freq(float freq) {
  uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq)  - 1;
  this->write_reg(Register::MODE1, 0x10); //sleep
  this->write_reg(Register::PRE_SCALE, prescale_val); // multiplyer for PWM frequency
  this->write_reg(Register::MODE1, 0x80); //restart
  this->write_reg(Register::MODE2, 0x04); //totem pole (default)
}

void PCA9685::set_pwm(std::uint8_t index, std::uint16_t on, std::uint16_t off) {
  this->write_reg(Register::LED0_ON_L  + 4 * index, on & 0xFF);
  this->write_reg(Register::LED0_ON_H  + 4 * index, on >> 8);
  this->write_reg(Register::LED0_OFF_L + 4 * index, off & 0xFF);
  this->write_reg(Register::LED0_OFF_H + 4 * index, off >> 8);
}

void PCA9685::reset() {
  this->write_reg(Register::MODE1, 0x00);
  this->write_reg(Register::MODE2, 0x04);
}

void PCA9685::write_reg(Register reg, std::uint8_t data) {
  uint8_t buf[2] = {static_cast<uint8_t>(reg), data};
  this->write_seq(buf, 2);
}

std::uint8_t PCA9685::read_reg(Register reg) {
  this->write(static_cast<std::uint8_t>(reg));
  return this->read<std::uint8_t>();
}
