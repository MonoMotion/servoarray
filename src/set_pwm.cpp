#include "pca9685/pca9685.h"

#include <iostream>

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cerr << "set_pwm <bus> <address> <index>" << std::endl;
    return 1;
  }

  const std::uint8_t bus = strtol(argv[1], NULL, 0);
  const std::uint8_t addr = strtol(argv[2], NULL, 8);
  const std::uint8_t index = strtol(argv[3], NULL, 8);

  auto driver = PCA9685(bus, addr);

  while(true) {
    std::uint16_t pulselen;
    std::cout << "> ";
    std::cin >> pulselen;
    driver.set_pwm(index, 0, pulselen);
  }
  return 0;
}
