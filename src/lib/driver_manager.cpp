#include "servoarray/driver_manager.h"

#include <dlfcn.h>

namespace ServoArray {

DriverManager::DriverManager(const std::string& path) : path_(path), loaded_drivers_() {}

Driver* DriverManager::get(const std::string& name) const {
  // TODO: throw errors::DriverNotFoundError when the name is not found
  return this->loaded_drivers_.at(name);
}

Driver* DriverManager::load(const std::string& name) {
  const auto path = this->resolve(name);
  auto* handle = dlopen(path.c_str(), RTLD_LAZY);
  if (!handle) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not load " + path);
  }

  auto* f = static_cast<Driver* (*)()>(dlsym(handle, "servoarray_driver"));
  if (!f) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not import symbol 'servoarray_driver'");
  }

  // TODO: Close handle properly
  return f();
}

Driver* DriverManager::get_or_load(const std::string& name) {
  if (this->is_loaded(name)) {
    return this->get(name);
  } else {
    return this->load(name);
  }
}

bool DriverManager::is_loaded(const std::string& name) const {
  const auto& drivers = this->loaded_drivers_;
  return drivers.find(name) != drivers.end();
}

}
