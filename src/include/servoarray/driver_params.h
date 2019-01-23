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
  using data_type = std::unordered_map<std::string, std::shared_ptr<void>>;
  using iterator = data_type::iterator;
  using const_iterator = data_type::const_iterator;

private:
  data_type data_;

public:
  DriverParams() = default;

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

  iterator begin() noexcept { return this->data_.begin(); }
  iterator end() noexcept { return this->data_.end(); }

  const_iterator begin() const noexcept { return this->data_.begin(); }
  const_iterator end() const noexcept { return this->data_.end(); }

  const_iterator cbegin() const noexcept { return this->data_.cbegin(); }
  const_iterator cend() const noexcept { return this->data_.cend(); }

  DriverParams& merge(const DriverParams& other) {
    for (auto const& p : other) {
      this->data_[p.first] = p.second;
    }

    return *this;
  }

  DriverParams merged(const DriverParams& other) const {
    DriverParams copy {*this};
    return copy.merge(other);
  }
};

}

#endif
