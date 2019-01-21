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

  if (argc != 4) {
    std::cerr << "bench <bus> <address> <times>" << std::endl;
    return 1;
  }

  const std::string name {argv[1]};
  const std::uint16_t times  = static_cast<std::uint16_t>(strtol(argv[2], nullptr, 0));

  ServoArray::DriverParams params;
  auto sa = ServoArray::ServoArray(name, params);

  auto const array_size = sa.size();

  bench("Set", times, [&sa](size_t i) { sa.write(0, 0); });
  bench("Get", times, [&sa](size_t i) { sa.read(0); });
}
