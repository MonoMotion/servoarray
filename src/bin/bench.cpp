#include "servoarray/servoarray.h"

#include <boost/lexical_cast.hpp>

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
  if (argc < 2) {
    std::cerr << argv[0] << " times [name]" << std::endl;
    return -1;
  }

  const auto times = boost::lexical_cast<std::size_t>(argv[1]);
  const std::string name {argc > 2 ? argv[2] : ""};
  auto sa = ServoArray::ServoArray(name);

  bench("Set", times, [&sa](size_t i) { sa.write(0, 0); });
  bench("Get", times, [&sa](size_t i) { sa.read(0); });

  bench("Set (subscript)", times, [&sa](size_t i) { sa[0] = 0; });
  bench("Get (subscript)", times, [&sa](size_t i) { static_cast<double>(sa[0]); });
}
