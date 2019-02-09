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

sudo apt-get update -y
sudo apt-get install -y python3-setuptools python3-pip
sudo pip3 install twine

function upload_pypi() {
  twine upload -u ${PYPI_USERNAME} -p ${PYPI_PASSWORD} $@
}

${BUILDER_SDIST:-false} && upload_pypi dist/*.tar.gz
${BUILDER_WHEEL:-false} && upload_pypi dist/*.whl
