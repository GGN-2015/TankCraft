#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <random>

namespace Xn {

struct Int {
  Int() : x(0) {}
  Int(int value) : x(value) {}
  Int(float value) : x(static_cast<int>(value)) {}

  int x;

 public:
  static const Int ZERO;  // 0
  static const Int ONE;   // 1
  static const Int MAX;   // 0x7FFF

 public:
  // 取得[0, max)区间上随机的Int
  // 参数:
  //   - max [Int] : 随机区间上界
  // 返回:
  //   [0, max)区间上随机的Int
  static inline Int Random(const Int &max) {
    const Int random_value = rand();
    return random_value % max;
  }
  // 取得[min, max)区间上随机的Int
  // 参数:
  //   - min [Int] : 随机区间下界
  //   - max [Int] : 随机区间上界
  // 返回:
  //   [min, max)区间上随机的Int
  static inline Int Random(const Int &min, const Int &max) {
    const Int random_value = rand();
    const Int range = max - min;
    return min + random_value % range;
  }

  // 钳制函数，会返回value在[0, max]之间的值
  // 参数:
  //   - value [Int] : 要钳制的值
  //   - max [Int]   : 随机区间上界
  // 返回:
  //   钳制后的值
  // 约定:
  //   - 0 <= max
  static inline Int Clamp(const Int &value, const Int &max) {
    if (value <= 0) return 0;
    if (value >= max) return max;
    return value;
  }
  // 钳制函数，会返回value在[min, max]之间的值
  // 参数:
  //   - value [Int] : 要钳制的值
  //   - min   [Int] : 钳制目标下界
  //   - max   [Int] : 钳制目标上界
  // 返回:
  //   钳制后的值
  // 约定:
  //   - min <= max
  static inline Int Clamp(const Int &value, const Int &min, const Int &max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
  }

 public:
  inline operator int() const { return x; }
  inline Int &operator=(const Int &other) {
    x = other.x;
    return *this;
  }
  inline Int operator+(const Int &other) const { return x + other.x; }
  inline Int operator-(const Int &other) const { return x - other.x; }
  inline Int operator*(const Int &other) const { return x * other.x; }
  inline Int operator/(const Int &other) const { return x / other.x; }
  inline Int operator%(const Int &other) const { return x % other.x; }
  inline Int &operator+=(const Int &other) {
    x += other.x;
    return *this;
  }
  inline Int &operator-=(const Int &other) {
    x -= other.x;
    return *this;
  }
  inline Int &operator*=(const Int &other) {
    x *= other.x;
    return *this;
  }
  inline Int &operator/=(const Int &other) {
    x /= other.x;
    return *this;
  }
  inline bool operator>(const Int &other) const { return x > other.x; }
  inline bool operator<(const Int &other) const { return x < other.x; }
  inline bool operator>=(const Int &other) const { return x >= other.x; }
  inline bool operator<=(const Int &other) const { return x <= other.x; }
  inline bool operator==(const Int &other) const { return x == other.x; }
  inline Int &operator=(const int &other) {
    x = other;
    return *this;
  }
  inline Int operator+(const int &other) const { return x + other; }
  inline Int operator-(const int &other) const { return x - other; }
  inline Int operator*(const int &other) const { return x * other; }
  inline Int operator/(const int &other) const { return x / other; }
  inline Int operator%=(const int &other) const { return x % other; }
  inline Int &operator+=(const int &other) {
    x += other;
    return *this;
  }
  inline Int &operator-=(const int &other) {
    x -= other;
    return *this;
  }
  inline Int &operator*=(const int &other) {
    x *= other;
    return *this;
  }
  inline Int &operator/=(const int &other) {
    x /= other;
    return *this;
  }
  inline bool operator>(const int &other) const { return x > other; }
  inline bool operator<(const int &other) const { return x < other; }
  inline bool operator>=(const int &other) const { return x >= other; }
  inline bool operator<=(const int &other) const { return x <= other; }
  inline bool operator==(const int &other) const { return x == other; }
  inline operator bool() const { return x != 0; }
  inline operator float() const { return static_cast<float>(x); }
};

}  // namespace Xn
