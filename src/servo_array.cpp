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

#include "servoarray/servoarray.h"

#include <iostream>

int main(int argc, char **argv) {

  if (argc != 5) {
    std::cerr << "servo_array <bus> <address> <min_pulse> <max_pulse>" << std::endl;
    return 1;
  }

  const std::uint8_t bus = strtol(argv[1], NULL, 0);
  const std::uint8_t addr = strtol(argv[2], NULL, 0);
  const std::uint16_t min_pulse = strtol(argv[3], NULL, 0);
  const std::uint16_t max_pulse = strtol(argv[4], NULL, 0);

  auto sa = ServoArray::ServoArray(bus, addr, min_pulse, max_pulse);

  while(true) {
    unsigned index;
    std::cout << "(index) > ";
    std::cin >> index;

    double rad;
    std::cout << " (rad)  > ";
    std::cin >> rad;

    sa.set(index, rad);
    std::cout << index << " -> " << sa.get(index) << std::endl;
  }
  return 0;
}
