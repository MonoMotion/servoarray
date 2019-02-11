#include "servoarray/servoarray.h"

#include <args.hxx>

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
  args::ArgumentParser argparser("servoarray benchmark");
  args::HelpFlag help(argparser, "help", "Print this help", {'h', "help"});
  args::Positional<std::string> arg_driver(argparser, "driver", "Driver name");
  args::ValueFlag<std::size_t> arg_times(argparser, "times", "times", {'n', "times"});

  try {
    argparser.ParseCLI(argc, argv);
  } catch (const args::Help&){
    std::cout << argparser;
    return 0;
  } catch (const args::ParseError& e){
    std::cerr << e.what() << std::endl;
    std::cerr << argparser;
    return -1;
  }

  const std::size_t times {arg_times ? args::get(arg_times) : 1000};
  const std::string name {arg_driver ? args::get(arg_driver) : ""};

  auto sa = ServoArray::ServoArray(name);

  bench("Set", times, [&sa](size_t i) { sa.write(0, 0); });
  bench("Get", times, [&sa](size_t i) { sa.read(0); });

  bench("Set (subscript)", times, [&sa](size_t i) { sa[0] = 0; });
  bench("Get (subscript)", times, [&sa](size_t i) { static_cast<double>(sa[0]); });
}
