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

DriverConfig::DriverConfig(const std::string& name, const DriverParams& params) : name_(name), params_(params) {}
const std::string& DriverConfig::name() const& { return this->name_; }
std::string DriverConfig::name() && { return std::move(this->name_); }

void DriverConfig::set_name(const std::string& name) { this->name_ = name; }

const DriverParams& DriverConfig::params() const& { return this->params_; }
DriverParams DriverConfig::params() && { return std::move(this->params_); }

void DriverConfig::set_params(const DriverParams& params) { this->params_ = params; }

DriverConfig& DriverConfig::merge(const DriverConfig& other) {
  this->name_ = other.name();
  this->params_.merge(other.params());

  return *this;
}

UserConfig::UserConfig(const std::string& path) {
  auto const config = toml::parse(path);

  auto const driver = toml::get<toml::table>(config.at("driver"));
  auto const name = toml::get_or<std::string>(driver, "name", "");

  auto const params = toml::get<toml::table>(driver.at("params"));

  DriverParams p;
  for (const auto& entry : params) {
    const auto& key = entry.first;
    const auto& value = entry.second;

    switch(value.type()) {
      case toml::value_t::Integer: p.put(key, toml::get<int>(value)); break;
      case toml::value_t::Float: p.put(key, toml::get<float>(value)); break;
      case toml::value_t::Boolean: p.put(key, toml::get<bool>(value)); break;
      case toml::value_t::String: p.put(key, toml::get<std::string>(value)); break;
      default: throw std::runtime_error(toml::format_error("Unsupported parameter type", value, toml::stringize(value.type())));
    }
  }

  this->driver_.set_name(name);
  this->driver_.set_params(p);
}

UserConfig::UserConfig(std::initializer_list<std::string> paths) {
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
