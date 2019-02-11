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

#include "servoarray/servoarray.h"

#include <args.hxx>

#include <iostream>
#include <cstdint>

int main(int argc, char **argv) {
  args::ArgumentParser argparser("Reset all joints");
  args::HelpFlag help(argparser, "help", "Print this help", {'h', "help"});
  args::Positional<std::string> arg_driver(argparser, "driver", "Driver name");

  try {
    argparser.ParseCLI(argc, argv);
  } catch (const args::Help&){
    std::cout << argparser;
    return 0;
  } catch (const args::ParseError& e){
    std::cerr << e.what() << std::endl;
    std::cerr << argparser;
    return -1;
  }

  const std::string driver {arg_driver ? args::get(arg_driver) : ""};

  auto sa = ServoArray::ServoArray(driver);

  for (std::size_t i = 0; i < sa.size(); i++) {
    sa[i] = 0;
  }
}
