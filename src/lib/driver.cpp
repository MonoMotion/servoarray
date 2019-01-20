#include "servoarray/driver.h"

namespace ServoArray {

const void* DriverParams::operator[](const char* key) const {
  std::string key_str { key };
  auto it = this->data_.find(key_str);
  if (it == this->data.end()) {
    return std::nullptr;
  } else {
    return it->get();
  }
}

Driver::~Driver() = default;

double Driver::read(std::size_t) const {
  // default implementation, return value is unspecified
  return 0;
}

}
