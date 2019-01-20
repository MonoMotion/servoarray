#include "servoarray/driver.h"

namespace ServoArray {

Driver::~Driver() = default;

double Driver::read(std::size_t) const {
  // default implementation, return value is unspecified
  return 0;
}

}
