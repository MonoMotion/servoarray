#include "servoarray/driver_manager.h"

#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace ServoArray {

DriverManager::DriverManager(const std::vector<std::string>& paths) : paths_(paths), loaded_drivers_() {}

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
