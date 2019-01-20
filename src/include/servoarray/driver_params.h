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
  const void* get(const char* key, void* default_ = nullptr) const;

private:
  // TODO: Pimpl?
  std::unordered_map<std::string, std::unique_ptr<void*>> data_;
};

}

#endif
