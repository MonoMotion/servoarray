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

const std::unordered_map<std::string, std::size_t>& MapConfig::names() const& {
  return this->names_;
}

std::unordered_map<std::string, std::size_t> MapConfig::names() && {
  return std::move(this->names_);
}

MapConfig& MapConfig::merge(const MapConfig& other) {
  for (const auto& p : other.names_) {
    this->names_[p.first] = p.second;
  }

  return *this;
}

UserConfig::UserConfig(const std::string& path) {
  auto const config = toml::parse(path);

  this->mapping_.names_ = toml::get_or<std::unordered_map<std::string, std::size_t>>(config, "mapping", {});

  // TODO: Seperate this into some function
  {
    auto const it_d = config.find("driver");
    if (it_d == config.end()) {
      return;
    }
    auto const driver = toml::get<toml::table>(it_d->second);
    this->driver_.name_ = toml::get_or<std::string>(driver, "name", "");

    auto const it_p = driver.find("params");
    if (it_p == driver.end()) {
      return;
    }

    auto const params = toml::get<toml::table>(it_p->second);

    for (const auto& p : params) {
      const auto& name = p.first;
      auto&& drv_params = this->driver_.params_[name];

      const auto& entries = toml::get<toml::table>(p.second);
      for (const auto& entry : entries) {
        const auto& key = entry.first;
        const auto& value = entry.second;

        // TODO: Support array and table type
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
}

UserConfig::UserConfig(const std::vector<std::string>& paths) {
  for(auto const& path : paths) {
    this->merge(UserConfig{path});
  }
}

const DriverConfig& UserConfig::driver() const& { return this->driver_; }
DriverConfig UserConfig::driver() && { return std::move(this->driver_); }

const MapConfig& UserConfig::mapping() const& { return this->mapping_; }
MapConfig UserConfig::mapping() && { return std::move(this->mapping_); }

UserConfig& UserConfig::merge(const UserConfig& other) {
  this->driver_.merge(other.driver());
  this->mapping_.merge(other.mapping());

  return *this;
}

}
