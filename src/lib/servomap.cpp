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

#include "servoarray/servomap.h"

namespace ServoArray {

ServoMap::ServoMap(const ServoArray& sa, const std::unordered_map<std::string, std::size_t> names) : sa_(sa), names_(names) {}

void ServoMap::write(const std::string& name, double rad) {
  // TODO: Throw proper error
  this->sa_.write(this->names_.at(name), rad);
}

double ServoMap::read(const std::string& name) const {
  // TODO: Throw proper error
  return this->sa_.read(this->names_.at(name));
}

const ServoArray& ServoMap::array() const { return this->sa_; }

std::size_t ServoMap::size() const { return this->sa_.size(); }

}
