#include "servoarray/servoarray.h"

#include <iostream>
#include <iomanip>
#include <chrono>

template<typename F>
void bench(std::string const& title, std::size_t times, F target) {
  std::cout << title << std::endl;
  auto start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < times; i++) {
    target(i);
  }
  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;
  std::cout << "\tTotal elapsed time: " << std::setw(12) << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms" << std::endl;
  std::cout << "\tAvarage latency   : " << std::setw(12) << std::chrono::duration_cast<std::chrono::microseconds>(elapsed / times).count() << "us" << std::endl;
}

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cerr << "bench <bus> <address> <min_pulse> <max_pulse> <times>" << std::endl;
    return 1;
  }

  const std::uint8_t bus   = static_cast<std::uint8_t>(strtol(argv[1], nullptr, 0));
  const std::uint8_t addr  = static_cast<std::uint8_t>(strtol(argv[2], nullptr, 0));
  const std::uint16_t min_pulse  = static_cast<std::uint16_t>(strtol(argv[3], nullptr, 0));
  const std::uint16_t max_pulse  = static_cast<std::uint16_t>(strtol(argv[4], nullptr, 0));
  const std::uint16_t times  = static_cast<std::uint16_t>(strtol(argv[5], nullptr, 0));

  auto sa = ServoArray::ServoArray(bus, addr, min_pulse, max_pulse);

  auto const array_size = sa.size();

  bench("Set (0)", times, [&sa](size_t i) { sa.set(0, 0); });
}
