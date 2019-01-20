#include "servoarray/driver_manager.h"

#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace ServoArray {

DriverManager::DriverManager(const std::vector<std::string>& paths) : paths_(paths), loaded_drivers_() {}

std::shared_ptr<Driver> DriverManager::get(const std::string& name) const {
  // TODO: throw errors::DriverNotFoundError when the name is not found
  return this->loaded_drivers_.at(name);
}

std::shared_ptr<Driver> DriverManager::load(const std::string& name) {
  const auto path = this->resolve(name);
  auto* handle = dlopen(path.c_str(), RTLD_LAZY);
  if (!handle) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not load " + path);
  }

  auto* servoarray_driver = static_cast<Driver* (*)()>(dlsym(handle, "servoarray_driver"));
  if (!servoarray_driver) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not import symbol 'servoarray_driver'");
  }

  auto* servoarray_cleanup = static_cast<void (*)(Driver*)>(dlsym(handle, "servoarray_cleanup"));
  if (!servoarray_cleanup) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not import symbol 'servoarray_cleanup'");
  }

  auto deleter = [servoarray_cleanup, handle](Driver* driver) {
    servoarray_cleanup(driver);
    if(dlclose(handle) != 0) {
      // TODO: throw errors::DriverCleanupError
      // TODO: Use dlerror(3) to obtain error message
      throw std::runtime_error("Could not close " + path);
    }
  };

  Driver* driver = servoarray_driver();
  return std::shared_ptr<Driver>(driver, deleter);
}

std::shared_ptr<Driver> DriverManager::get_or_load(const std::string& name) {
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

static std::string DriverManager::driver_file_name(const std::string& name) const {
  // TODO: Include version information of servoarray
  // TODO: Include system information ($CC -dumpmachine ?)
  return name + ".servoarray.so";
}

std::string DriverManager::resolve(const std::string& name) const {
  namespace fs = boost::filesystem;

  fs::path file {DriverManager::driver_file_name(name)};

  for (const auto& path : this->paths) {
    fs::path dir {path};
    fs::path path = dir / file;
    if (fs::exists(path)) {
      return fs::canonical(path).native();
    }
  }

  // TODO: throw errors::DriverNotFoundError
  throw std::runtime_error("Could not resolve driver name " + name);
}

}
