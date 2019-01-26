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

#ifndef SERVOARRAY_SERVOMAP_H
#define SERVOARRAY_SERVOMAP_H

#include <string>
#include <vector>

#include "servoarray/servoarray.h"

namespace ServoArray {

class SubscriptWrapper;

class ServoMap {
  ServoArray sa_;
  std::unordered_map<std::string, std::size_t> names_;

public:
  ServoMap(const ServoArray&, const std::unordered_map<std::string, std::size_t> names);
  ServoMap(const ServoArray&, DriverManager& manager = default_manager);

  void write(const std::string& name, double rad);
  double read(const std::string& name);

  SubscriptWrapper operator[](const std::string&);

  const ServoArray& array() const;

  std::size_t size() const;

  const std::unordered_map<std::string, std::size_t>& names() const;
  bool has_name(const std::string&) const;
};

}

#include "servoarray/subscript_wrapper.h"

#endif
