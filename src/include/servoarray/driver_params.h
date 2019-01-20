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
// Using primitive types to keep ABI compatibility
class DriverParams {
public:
  const void* get(const char* key, void* default_ = nullptr) const {
    std::string key_str { key };
    auto it = this->data_.find(key_str);
    if (it == this->data.end()) {
      return default_;
    } else {
      return it->get();
    }
  }

  template<typename... Args>
  void emplace(const char* key, Args&&... args) {
    this->data_.emplace(std::string{key}, new T(args...));
  }

  void erase(const char* key) {
    this->data_.erase(std::string{key});
  }

private:
  // TODO: Pimpl?
  std::unordered_map<std::string, void*> data_;
};

}

#endif
