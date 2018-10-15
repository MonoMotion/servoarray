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

#include "servoarray/pca9685/pca9685.h"

namespace ServoArray {

PCA9685::PCA9685(std::uint8_t bus_, std::uint8_t addr_) : bus(bus_), address(addr_), buf_count(0) {
  char dev_path[64];
  std::sprintf(dev_path, "/dev/i2c-%d", this->bus);
  if ((this->fd = ::open(dev_path, O_RDWR)) < 0) {
    throw std::runtime_error("Failed to open i2c device file " + std::string{ dev_path });
  }

  if (ioctl(this->fd, I2C_SLAVE, this->address) < 0) {
    throw std::runtime_error("Unable to get bus access to talk to slave");
  }
  this->reset();
  this->set_pwm_freq(40);
}

std::uint8_t PCA9685::num_servos() {
  return 16;
}

//
// set_pwm and set_pwm_freq are written based on TeraHz/PCA9685
// https://github.com/TeraHz/PCA9685
//
// Copyright © 2012 Georgi Todorov  <terahz@geodar.com>
//
void PCA9685::set_pwm_freq(float freq) {
  uint8_t prescale_val = static_cast<uint8_t>(CLOCK_FREQ / 4096 / freq)  - 1;
  this->write_reg(Register::MODE1, 0x10); //sleep
  this->write_reg(Register::PRE_SCALE, prescale_val); // multiplyer for PWM frequency
  this->write_reg(Register::MODE1, 0x80); //restart
  this->write_reg(Register::MODE2, 0x04); //totem pole (default)

  auto const old_mode = this->read_reg(Register::MODE1);
  this->write_reg(Register::MODE1, old_mode | 0xa0); // Turn on auto increment
}

void PCA9685::set_pwm(std::uint8_t index, std::uint16_t on, std::uint16_t off) {
  if (index >= this->num_servos()) {
    throw std::out_of_range("Channel index out of bounds");
  }
  this->write_reg_buf(Register::LED0_ON_L  + 4 * index, on & 0xFF);
  // There's no need to specify register bacause auto increment is enabled
  this->write_buf(on >> 8);
  this->write_buf(off & 0xFF);
  this->write_buf(off >> 8);
  this->flush_buf();
}

void PCA9685::reset() {
  this->write_reg(Register::MODE1, 0x00);
  this->write_reg(Register::MODE2, 0x04);
}

void PCA9685::write_reg(Register reg, std::uint8_t data) {
  uint8_t buf[2] = {static_cast<uint8_t>(reg), data};
  this->write_seq(buf, 2);
}

void PCA9685::write_reg_buf(Register reg, std::uint8_t data) {
  this->buf[this->buf_count++] = static_cast<std::uint8_t>(reg);
  this->buf[this->buf_count++] = data;
}

void PCA9685::write_buf(std::uint8_t data) {
  this->buf[this->buf_count++] = data;
}

void PCA9685::flush_buf() {
  this->write_seq(this->buf.data(), this->buf_count);
  this->buf_count = 0;
}

std::uint8_t PCA9685::read_reg(Register reg) {
  this->write(static_cast<std::uint8_t>(reg));
  return this->read<std::uint8_t>();
}

}
