// This file is part of servoarray.
//
// servoarray is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// servoarray is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SERVOARRAY_DRIVER_PARAMS_H
#define SERVOARRAY_DRIVER_PARAMS_H

#include <unistd.h>
#include <unordered_map>
#include <memory>
#include <string>

namespace ServoArray {

// A Key-Value store to hold initialization parameters
// TODO: Using primitive types to keep ABI compatibility
class DriverParams {
public:
  template<typename T>
  const T& get(const std::string& key) const {
    auto it = this->data_.find(key);
    if (it == this->data_.end()) {
      // TODO: Use errors::ParamKeyError
      throw std::runtime_error("Could not find " + key);
    } else {
      return *static_cast<const T*>(it->second.get());
    }
  }

  template<typename T>
  const T& get_or(const std::string& key, const T& default_) const {
    auto it = this->data_.find(key);
    if (it == this->data_.end()) {
      return default_;
    } else {
      return *static_cast<const T*>(it->second.get());
    }
  }

  template<typename T, typename... Args>
  void emplace(const std::string& key, Args&&... args) {
    this->data_.emplace(key, std::make_shared<T>(args...));
  }

  void erase(const std::string& key) {
    this->data_.erase(key);
  }

private:
  // TODO: Pimpl?
  std::unordered_map<std::string, std::shared_ptr<void>> data_;
};

}

#endif
