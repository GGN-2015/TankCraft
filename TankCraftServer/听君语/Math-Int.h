#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math-NumberBase.h"

namespace Xn {

struct Int : public XnMath::NumberBase {
  Int() : x(0) {}
  Int(const Int &other) : x(other.x) {}
  Int(const int &value) : x(value) {}
  Int(const unsigned int &value) : x(value) {}
  Int(const float &value) : x(static_cast<int>(value)) {}

  int x;

 public:
  static const Int ZERO;  // 0
  static const Int ONE;   // 1
  static const Int MAX;   // 0x7FFF

 public:
  inline virtual std::wstring ToString() const override {
    return std::to_wstring(x);
  }

 public:
  inline virtual operator bool() const { return x != 0; };
  inline operator int() const { return x; }
  inline operator float() const { return static_cast<float>(x); }
  inline float ToFloat() const { return static_cast<float>(x); }

 public:
  // 数学赋值
  // 注：
  //   会改变内部存储值
  inline Int &Set(const Int &other) {
    x = other.x;
    return *this;
  }
  // 数学取值
  inline const int &Get() { return x; }
  // 数学加法
  // 注：
  //   会改变内部存储值
  inline Int &Add(const Int &other) {
    x += other.x;
    return *this;
  }
  // 数学加法
  inline Int AddJust(const Int &other) const { return x + other.x; }
  // 数学减法
  // 注：
  //   会改变内部存储值
  inline Int &Sub(const Int &other) {
    x -= other.x;
    return *this;
  }
  // 数学被减法
  // 注：
  //   会改变内部存储值
  inline Int &SubBy(const Int &other) {
    x = other.x - x;
    return *this;
  }
  // 数学减法
  inline Int SubJust(const Int &other) const { return x - other.x; }
  // 数学被减法
  inline Int SubJustBy(const Int &other) const { return other.x - x; }
  // 数学乘法
  // 注：
  //   会改变内部存储值
  inline Int &Mul(const Int &other) {
    x *= other.x;
    return *this;
  }
  // 数学乘法
  inline Int MulJust(const Int &other) const { return x * other.x; }
  // 数学除法
  // 注：
  //   会改变内部存储值
  inline Int &Div(const Int &other) {
    x /= other.x;
    return *this;
  }
  // 数学除法
  inline Int DivJust(const Int &other) const { return x / other.x; }
  // 数学取模
  // 注：
  //   会改变内部存储值
  inline Int &Mod(const Int &other) {
    x %= other.x;
    return *this;
  }
  // 数学取模
  inline Int ModJust(const Int &other) const { return x % other.x; }

 public:
  // 将值改为[0, max)区间上随机的Int
  // 参数:
  //   - max [Int] : 随机区间上界
  // 返回:
  //   [0, max)区间上随机的Int
  inline Int &SetRandom(const Int &max) {
    x = Int(rand()).ModJust(max).x;
    return *this;
  }
  // 取得[0, max)区间上随机的Int
  // 参数:
  //   - max [Int] : 随机区间上界
  // 返回:
  //   [0, max)区间上随机的Int
  static inline Int Random(const Int &max) { return Int(rand()).ModJust(max); }
  // 取得[min, max)区间上随机的Int
  // 参数:
  //   - min [Int] : 随机区间下界
  //   - max [Int] : 随机区间上界
  // 返回:
  //   [min, max)区间上随机的Int
  static inline Int Random(const Int &min, const Int &max) {
    const auto range = max.x - min.x;
    return min.x + Random(range).x;
  }

  // 将值钳制在[min, max]之间
  // 参数:
  //   - min   [Int] : 钳制目标下界
  //   - max   [Int] : 钳制目标上界
  // 返回:
  //   钳制后的值
  // 约定:
  //   - min <= max
  inline Int SetClamp(const Int &min, const Int &max) {
    if (x <= min.x) return x = min;
    if (x >= max.x) return x = max;
    return x;
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
    if (value.x <= 0) return 0;
    if (value.x >= max.x) return max;
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
    if (value.x <= min.x) return min;
    if (value.x >= max.x) return max;
    return value;
  }

 public:
  inline Int &operator=(const Int &other) {
    x = other.x;
    return *this;
  }
  inline Int &operator=(const int &other) {
    x = other;
    return *this;
  }
};

inline Int operator-(const Int &r) { return -r.x; }
inline Int operator+(const Int &l, const int &r) { return l.x + r; }
inline Int operator+(const int &l, const Int &r) { return l + r.x; }
inline Int operator+(const Int &l, const Int &r) { return l.x + r.x; }
inline Int operator-(const Int &l, const int &r) { return l.x - r; }
inline Int operator-(const int &l, const Int &r) { return l - r.x; }
inline Int operator-(const Int &l, const Int &r) { return l.x - r.x; }
inline Int operator*(const Int &l, const int &r) { return l.x * r; }
inline Int operator*(const int &l, const Int &r) { return l * r.x; }
inline Int operator*(const Int &l, const Int &r) { return l.x * r.x; }
inline Int operator/(const Int &l, const int &r) { return l.x / r; }
inline Int operator/(const int &l, const Int &r) { return l / r.x; }
inline Int operator/(const Int &l, const Int &r) { return l.x / r.x; }
inline Int operator%(const Int &l, const int &r) { return l.x % r; }
inline Int operator%(const int &l, const Int &r) { return l % r.x; }
inline Int operator%(const Int &l, const Int &r) { return l.x % r.x; }
inline Int &operator+=(Int &l, const int &r) { return l.Add(r); }
inline Int &operator+=(Int &l, const Int &r) { return l.Add(r); }
inline Int &operator-=(Int &l, const int &r) { return l.Sub(r); }
inline Int &operator-=(Int &l, const Int &r) { return l.Sub(r); }
inline Int &operator*=(Int &l, const int &r) { return l.Mul(r); }
inline Int &operator*=(Int &l, const Int &r) { return l.Mul(r); }
inline Int &operator/=(Int &l, const int &r) { return l.Div(r); }
inline Int &operator/=(Int &l, const Int &r) { return l.Div(r); }
inline bool operator>(const Int &l, const int &r) { return l.x > r; }
inline bool operator>(const int &l, const Int &r) { return l > r.x; }
inline bool operator>(const Int &l, const Int &r) { return l.x > r.x; }
inline bool operator<(const Int &l, const int &r) { return l.x < r; }
inline bool operator<(const int &l, const Int &r) { return l < r.x; }
inline bool operator<(const Int &l, const Int &r) { return l.x < r.x; }
inline bool operator>=(const Int &l, const int &r) { return l.x >= r; }
inline bool operator>=(const int &l, const Int &r) { return l >= r.x; }
inline bool operator>=(const Int &l, const Int &r) { return l.x >= r.x; }
inline bool operator<=(const Int &l, const int &r) { return l.x <= r; }
inline bool operator<=(const int &l, const Int &r) { return l <= r.x; }
inline bool operator<=(const Int &l, const Int &r) { return l.x <= r.x; }
inline bool operator==(const Int &l, const int &r) { return l.x == r; }
inline bool operator==(const int &l, const Int &r) { return l == r.x; }
inline bool operator==(const Int &l, const Int &r) { return l.x == r.x; }

}  // namespace Xn
