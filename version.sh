#!/bin/bash
#
# Copyright 2018 coord.e
#
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
#
# This script prints semver-compatible version string calculated from current state of git

set -euo pipefail

cd "$(dirname "$0")"

function detailed() {
  local annotated=$(git describe --tags --abbrev=0 2> /dev/null)
  local description=$(git describe --always)
  local replaced=${description/${annotated}-/${annotated}+}
  local semver=${replaced#v}
  echo $semver
}

function simple() {
  local annotated=$(git describe --tags --abbrev=0 2> /dev/null)
  echo ${annotated#v}
}

${1:-detailed}
