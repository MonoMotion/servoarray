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

#include <iostream>
#include <atomic>
#include <signal.h>

static std::atomic<bool> quit(false);

void register_signal(int);
void quit_handler(int);

int main(int argc, char **argv) {

  const std::string name {argc > 1 ? argv[1] : ""};
  auto sa = ServoArray::ServoArray(name);

  register_signal(SIGINT);
  register_signal(SIGQUIT);
  register_signal(SIGTERM);

  while(true) {
    std::size_t index;
    std::cout << "(index) > ";
    std::cin >> index;

    double rad;
    std::cout << " (rad)  > ";
    std::cin >> rad;

    sa[index] = rad;
    std::cout << index << " -> " << sa[index] << std::endl;

    if(quit.load()) break;
  }
}

void register_signal(int signal) {
  struct sigaction action{};
  action.sa_handler = quit_handler;
  sigfillset(&action.sa_mask);
  sigaction(signal, &action, nullptr);
}

void quit_handler(int) {
  quit.store(true);
}
