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

#include <toml11/toml.hpp>

#include "servoarray/user_config.h"

namespace ServoArray {

const std::string& DriverConfig::name() const& { return this->name_; }
std::string DriverConfig::name() && { return std::move(this->name_); }

const DriverParams& DriverConfig::params(const std::string& name) const { return this->params_.at(name); }

bool DriverConfig::has_params_for(const std::string& name) const {
  return this->params_.find(name) != this->params_.end();
}

DriverConfig& DriverConfig::merge(const DriverConfig& other) {
  this->name_ = other.name();
  for(const auto& p : other.params_) {
    this->params_[p.first].merge(p.second);
  }

  return *this;
}

UserConfig::UserConfig(const std::string& path) {
  auto const config = toml::parse(path);

  auto const driver = toml::find<toml::table>(config, "driver");
  this->driver_.name_ = toml::get_or<std::string>(driver, "name", "");

  auto const params = toml::find<toml::table>(driver, "params");

  for (const auto& p : params) {
    const auto& name = p.first;
    auto&& drv_params = this->driver_.params_[name];

    const auto& entries = toml::get<toml::table>(p.second);
    for (const auto& entry : entries) {
      const auto& key = entry.first;
      const auto& value = entry.second;

      switch(value.type()) {
        case toml::value_t::Integer: drv_params.put(key, toml::get<int>(value)); break;
        case toml::value_t::Float:   drv_params.put(key, toml::get<float>(value)); break;
        case toml::value_t::Boolean: drv_params.put(key, toml::get<bool>(value)); break;
        case toml::value_t::String:  drv_params.put(key, toml::get<std::string>(value)); break;
        default: throw std::runtime_error(toml::format_error("Unsupported parameter type", value, toml::stringize(value.type())));
      }
    }
  }
}

UserConfig::UserConfig(const std::vector<std::string>& paths) {
  for(auto const& path : paths) {
    this->merge(UserConfig{path});
  }
}

const DriverConfig& UserConfig::driver() const& { return this->driver_; }
DriverConfig UserConfig::driver() && { return std::move(this->driver_); }

UserConfig& UserConfig::merge(const UserConfig& other) {
  this->driver_.merge(other.driver());

  return *this;
}

}
