#!/bin/bash
# This file is part of servoarray.
#
# servoarray is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# servoarray is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

set -euo pipefail

cd /build

function package_filename() {
  local sa_version=$(/source/version.sh)
  local arch=$(dpkg --print-architecture)

  . /etc/os-release
  if [ -n "${VERSION_ID:-}" ]; then
    echo "servoarray_${sa_version}-deb${VERSION_ID}_${arch}"
  else
    echo "servoarray_${sa_version}_${arch}"
  fi
}

cpack -D CPACK_OUTPUT_FILE_PREFIX=/dist -D CPACK_GENERATOR=DEB -D CPACK_PACKAGE_FILE_NAME=$(package_filename)
