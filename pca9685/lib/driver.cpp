#include "pca9685/driver.h"
#include "pca9685/constants.h"

namespace pca9685 {

PCA9685Driver::PCA9685Driver(std::uint8_t bus, std::uint8_t address, std::uint16_t min_pulse, std::uint16_t max_pulse)
  : pca9685_(bus, address), min_pulse_(min_pulse), max_pulse_(max_pulse) {}

void PCA9685Driver::write(std::size_t index, double rad) {
  if (index >= this->pca9685_.num_servos()) {
    throw std::out_of_range("Channel index out of bounds");
  }

  if (!constants::is_valid_pos(rad)) {
    throw std::out_of_range("Position value must be within the range from -pi/2 to pi/2");
  }

  const auto pulse = (rad - constants::min_pos<double>) * (this->max_pulse_ - this->min_pulse_) / (constants::max_pos<double> - constants::min_pos<double>) + this->min_pulse_;
  this->pca9685_.set_pwm(index, 0, static_cast<std::uint16_t>(pulse));
}

std::size_t PCA9685Driver::size() const {
  return this->pca9685_.num_servos();
}

}

extern "C" ServoArray::Driver* servoarray_driver(const ServoArray::DriverParams& params) {
  return new pca9685::PCA9685Driver(params.get<std::uint8_t>("bus"), params.get<std::uint8_t>("address"), params.get_or<unsigned>("min_pulse", 150), params.get_or<unsigned>("max_pulse", 600));
}
