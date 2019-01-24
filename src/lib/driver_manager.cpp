#include "servoarray/driver_manager.h"

#include <boost/filesystem.hpp>

#include <sstream>
#include <dlfcn.h>

namespace ServoArray {

void DriverManager::expand_paths(std::vector<std::string>& paths) {
  const char* home = std::getenv("HOME");
  if (!home) {
    return;
  }

  for (auto&& path : paths) {
    if (path[0] == '~') {
      path.replace(0, 1, home);
    }
  }
}

void DriverManager::add_default_search_paths() {
  //
  // Driver path resolution order
  //
  // 1. $SA_DRIVER_PATH
  // 2. additional_paths
  // 3. ./
  // 4. SERVOARRAY_DEFAULT_PATHS (set in cmake)
  // 5. dynamic linker searches for the library
  //

  const char* env_path = std::getenv("SA_DRIVER_PATH");
  if(env_path) {
    std::string path;
    std::istringstream s (env_path);
    while(std::getline(s, path, ':')) {
      this->paths_.insert(this->paths_.begin(), path);
    }
  }

  this->paths_.push_back(".");

  for (auto const& path : {SERVOARRAY_DEFAULT_PATHS}) {
    this->paths_.push_back(path);
  }

  const char* home = std::getenv("HOME");
  if (!home) {
    return;
  }

  for (auto&& path : this->paths_) {
    if (path[0] == '~') {
      path.replace(0, 1, home);
    }
  }
}

DriverManager::DriverManager(const std::vector<std::string>& paths, std::vector<std::string> config_files) : paths_(paths), loaded_drivers_() {
  this->add_default_search_paths();
  DriverManager::expand_paths(this->paths_);

  DriverManager::expand_paths(config_files);
  this->user_config_ = UserConfig{config_files};
}

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
  return name + SERVOARRAY_DRIVER_POSTFIX;
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

  return file.native();
}

DriverManager default_manager;

}
