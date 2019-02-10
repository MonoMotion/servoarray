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

#include <boost/property_tree/ptree.hpp>

namespace ServoArray {

// A Key-Value store to hold initialization parameters
// TODO: Using primitive types to keep ABI compatibility
class DriverParams {
public:
  using data_type = boost::property_tree::ptree;
  using iterator = data_type::iterator;
  using const_iterator = data_type::const_iterator;

private:
  data_type data_;

public:
  DriverParams() = default;

  template<typename T>
  T get(const std::string& key) const {
    if(auto const value = this->data_.get_optional<T>(key)) {
      return value.get();
    } else {
      // TODO: Use errors::ParamKeyError
      throw std::runtime_error("Could not find " + key);
    }
  }

  template<typename T>
  T get_or(const std::string& key, const T& default_) const {
    // TODO: Use the default-value version of ptree::get
    if(auto const value = this->data_.get_optional<T>(key)) {
      return value.get();
    } else {
      return default_;
    }
  }

  template<typename T>
  void put(const std::string& key, const T& value) {
    this->data_.put(key, value);
  }

  void erase(const std::string& key) {
    this->data_.erase(key);
  }

  iterator begin() noexcept { return this->data_.begin(); }
  iterator end() noexcept { return this->data_.end(); }

  const_iterator begin() const noexcept { return this->data_.begin(); }
  const_iterator end() const noexcept { return this->data_.end(); }

  const_iterator cbegin() const noexcept { return this->data_.begin(); }
  const_iterator cend() const noexcept { return this->data_.end(); }

  DriverParams& merge(const DriverParams& other) {
    this->merge_impl("", other.data_);

    return *this;
  }

  DriverParams merged(const DriverParams& other) const {
    DriverParams copy {*this};
    return copy.merge(other);
  }

private:
  void merge_impl(const boost::property_tree::ptree::path_type& path, const boost::property_tree::ptree& tree) {
    this->data_.put(path, tree.data());

    for (const auto& p : tree) {
      const auto child_path = path / boost::property_tree::ptree::path_type(p.first);
      this->merge_impl(child_path, p.second);
    }
  }
};

}

#endif
