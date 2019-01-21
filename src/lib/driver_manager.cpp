#include "servoarray/driver_manager.h"

#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace ServoArray {

DriverManager::DriverManager(const std::vector<std::string>& paths) : paths_(paths), loaded_drivers_() {}

std::shared_ptr<Driver> DriverManager::get(const std::string& name) const {
  // TODO: throw errors::DriverNotFoundError when the name is not found
  return this->loaded_drivers_.at(name);
}

std::shared_ptr<Driver> DriverManager::load(const std::string& name, const DriverParams& params) {
  const auto path = this->resolve(name);
  auto* handle = dlopen(path.c_str(), RTLD_LAZY);
  if (!handle) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not load " + path);
  }

  auto* servoarray_driver = reinterpret_cast<Driver* (*)(const DriverParams&)>(dlsym(handle, "servoarray_driver"));
  if (!servoarray_driver) {
    // TODO: throw errors::DriverLoadError
    // TODO: Use dlerror(3) to obtain error message
    throw std::runtime_error("Could not import symbol 'servoarray_driver'");
  }

  auto deleter = [path, handle](Driver* driver) {
    delete driver;

    if(dlclose(handle) != 0) {
      // TODO: throw errors::DriverCleanupError
      // TODO: Use dlerror(3) to obtain error message
      throw std::runtime_error("Could not close " + path);
    }
  };

  Driver* driver = servoarray_driver(params);
  std::shared_ptr<Driver> sptr {driver, deleter};
  this->loaded_drivers_.emplace(name, sptr);
  return sptr;
}

bool DriverManager::is_loaded(const std::string& name) const {
  const auto& drivers = this->loaded_drivers_;
  return drivers.find(name) != drivers.end();
}

std::string DriverManager::driver_file_name(const std::string& name) {
  // TODO: Include version information of servoarray
  // TODO: Include system information ($CC -dumpmachine ?)
  return name + ".servoarray.so";
}

std::string DriverManager::resolve(const std::string& name) const {
  namespace fs = boost::filesystem;

  fs::path file {DriverManager::driver_file_name(name)};

  for (const auto& path : this->paths_) {
    fs::path dir (path);
    fs::path file_path = dir / file;
    if (fs::exists(file_path)) {
      return fs::canonical(file_path).native();
    }
  }

  // TODO: throw errors::DriverNotFoundError
  throw std::runtime_error("Could not resolve driver name " + name);
}

}
