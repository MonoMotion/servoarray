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

#ifndef SERVOARRAY_BIN_UTILS_H
#define SERVOARRAY_BIN_UTILS_H

#include <atomic>
#include <signal.h>

namespace util {

// TODO: Use better way to share these code

static std::atomic<bool> quit(false);

void register_signal(int);
void quit_handler(int);
bool should_exit();

void register_signal(int signal) {
  struct sigaction action{};
  action.sa_handler = quit_handler;
  sigfillset(&action.sa_mask);
  sigaction(signal, &action, nullptr);
}

void quit_handler(int) {
  quit.store(true);
}

bool should_exit() {
  return quit.load();
}

}

#endif
