# servoarray

![Travis (.org)](https://img.shields.io/travis/coord-e/servoarray.py.svg)
![PyPI](https://img.shields.io/pypi/v/servoarray.svg)
![PyPI - License](https://img.shields.io/pypi/l/servoarray.svg)
![C++](https://img.shields.io/badge/C%2B%2B-14-orange.svg)

A fast implementation of PCA9685 driver, written in C++ and available as python module

## Install

```shell
pip install servoarray
```

## Usage

```python
from servoarray import ServoArray

sa = ServoArray(bus=1, address=0x40, min_pulse=150, max_pulse=600)

# Now you can set a radian value to the servo
sa[0] = 3.14 / 2  # Max
sa[0] = -3.14 / 2 # Min

# Slicing and reverse indexing are supported as the normal list
sa[:] = [0] * len(sa)
print(sa[-1])
```
