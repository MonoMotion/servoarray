#ifndef SERVOARRAY_CONSTANTS_H
#define SERVOARRAY_CONSTANTS_H

#include <algorithm>

namespace ServoArray {
namespace Constants {

template <class T>
static constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884e+00);

template <class T>
static constexpr T max_pos = pi<T> / 2;

template <class T>
static constexpr T min_pos = - pi<T> / 2;

template <class T>
constexpr T clip_pos(T rad) {
  return std::min(std::max(rad, min_pos<T>), max_pos<T>);
}

template <class T>
constexpr T is_valid_pos(T rad) {
  return rad < max_pos<T> && rad > min_pos<T>;
}

}
}

#endif
