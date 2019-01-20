#include "servoarray/driver_manager.h"

namespace ServoArray {

DriverManager::DriverManager(const std::string& path) : path_(path), loaded_drivers_() {}

Driver* DriverManager::get(const std::string& name) const {
  // TODO: throw errors::DriverNotFoundError when the name is not found
  return this->loaded_drivers_.at(name);
}

bool DriverManager::is_loaded(const std::string& name) const {
  const auto& drivers = this->loaded_drivers_;
  return drivers.find(name) != drivers.end();
}

}
