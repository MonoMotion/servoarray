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

#ifndef SERVOARRAY_DRIVER_H
#define SERVOARRAY_DRIVER_H

#include "servoarray/driver_params.h"

namespace ServoArray {

// ABC for drivers
class Driver {
public:
  virtual ~Driver() = default;

  virtual void write(std::size_t, double) = 0;
  virtual double read(std::size_t) { return 0; }

  virtual std::size_t size() const = 0;
};

//
// Driver .so must contain this symbol:
// Driver* servoarray_driver(const DriverParams&);
//

}

#endif
