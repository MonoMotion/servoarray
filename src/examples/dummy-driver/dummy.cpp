#include <servoarray/driver.h>
#include <cstdint>
#include <iostream>

class DummyDriver final : public ServoArray::Driver {
  std::size_t size_;

public:
  DummyDriver(std::size_t size) : size_(size) {}

  void write(std::size_t idx, double pos) override {
    std::cout << "[write] index: " << idx << "\t position: " << pos << std::endl;
  }

  double read(std::size_t idx) override {
    std::cout << "[read]  index: " << idx << std::endl;
    return 0;
  }

  std::size_t size() const override {
    return this->size_;
  }
};

extern "C" ServoArray::Driver* servoarray_driver(const ServoArray::DriverParams& params) {
  for(auto const& p : params) {
    std::cout << "[init]  key: " << p.first << "\t value: " << p.second.data() << std::endl;
  }
  return new DummyDriver(params.get_or<std::size_t>("size", 5));
}
