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

function base_image_python() {
  if [ -n "${CROSS:-}" ]; then
    echo "balenalib/${CROSS}-python:${PYTHON_VERSION}-${DEBIAN_VERSION}-build"
  else
    echo "python:${PYTHON_VERSION}-slim-${DEBIAN_VERSION}"
  fi
}

function base_image_deb() {
  if [ -n "${CROSS:-}" ]; then
    echo "balenalib/${CROSS}:${DEBIAN_VERSION}-build"
  else
    echo "debian:${DEBIAN_VERSION}-slim"
  fi
}

function base_image() {
  case ${BUILD_TYPE} in
    "python" ) base_image_python ;;
    "deb" ) base_image_deb ;;
    * ) echo "unknown build type ${BUILD_TYPE}"; exit -1 ;;
  esac
}

readonly BASE_IMAGE=$(base_image)

echo "BASE_IMAGE=${BASE_IMAGE}"
docker build ci/image -t builder --build-arg BASE_IMAGE=${BASE_IMAGE}
