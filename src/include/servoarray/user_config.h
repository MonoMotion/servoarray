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

#ifndef SERVOARRAY_USER_CONFIG_H
#define SERVOARRAY_USER_CONFIG_H

#include <string>
#include <vector>
#include <unordered_map>

#include "servoarray/driver_params.h"

namespace ServoArray {

class DriverConfig {
  std::string name_;
  std::unordered_map<std::string, DriverParams> params_;
  std::vector<std::string> search_paths_;

  friend class UserConfig;

public:
  const std::string& name() const&;
  std::string name() &&;

  const DriverParams& params(const std::string& name) const;
  bool has_params_for(const std::string& name) const;

  DriverConfig& merge(const DriverConfig&);
};

class MapConfig {
  std::unordered_map<std::string, std::size_t> names_;

  friend class UserConfig;

public:
  const std::unordered_map<std::string, std::size_t>& names() const&;
  std::unordered_map<std::string, std::size_t> names() &&;

  MapConfig& merge(const MapConfig&);
};

class UserConfig {
  DriverConfig driver_;
  MapConfig mapping_;

public:
  UserConfig() = default;
  explicit UserConfig(const std::string& path);
  explicit UserConfig(const std::vector<std::string>& paths);

  const DriverConfig& driver() const&;
  DriverConfig driver() &&;

  const MapConfig& mapping() const&;
  MapConfig mapping() &&;

  UserConfig& merge(const UserConfig&);
};

}

#endif
