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

#include <iostream>

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cerr << "set_pwm <bus> <address> <index>" << std::endl;
    return 1;
  }

  const std::uint8_t bus   = static_cast<std::uint8_t>(strtol(argv[1], nullptr, 0));
  const std::uint8_t addr  = static_cast<std::uint8_t>(strtol(argv[2], nullptr, 0));
  const std::uint8_t index = static_cast<std::uint8_t>(strtol(argv[3], nullptr, 0));

  auto driver = ServoArray::PCA9685(bus, addr);

  while(true) {
    std::uint16_t pulselen;
    std::cout << "> ";
    std::cin >> pulselen;
    driver.set_pwm(index, 0, pulselen);
  }
}
