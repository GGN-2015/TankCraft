#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math-NumberBase.h"

namespace Xn {

struct Float : public XnMath::NumberBase {
  Float() : x(0) {}
  Float(const float &value) : x(value) {}
  Float(const int &value) : x(static_cast<float>(value)) {}
  Float(const unsigned int &value) : x(static_cast<float>(value)) {}

  float x;

 public:
  static const Float ZERO;  // 0
  static const Float ONE;   // 1
  static const Float PI;    // pi ~ 3.14……

 public:
  inline std::wstring ToString() const { return std::to_wstring(x); }

 public:
  inline operator float() const { return x; }
  inline operator int() const { return static_cast<int>(x); }
  inline int ToInt() const { return static_cast<int>(x); }

 public:
  // 数学赋值
  // 注：
  //   会改变内部存储值
  inline Float &Set(const Float &other) {
    x = other.x;
    return *this;
  }
  // 数学取值
  inline const Float &Get() { return x; }
  // 数学加法
  // 注：
  //   会改变内部存储值
  inline Float &Add(const Float &other) {
    x += other.x;
    return *this;
  }
  // 数学加法
  inline Float AddJust(const Float &other) const { return x + other.x; }
  // 数学减法
  // 注：
  //   会改变内部存储值
  inline Float &Sub(const Float &other) {
    x -= other.x;
    return *this;
  }
  // 数学被减法
  // 注：
  //   会改变内部存储值
  inline Float &SubBy(const Float &other) {
    x = other.x - x;
    return *this;
  }
  // 数学减法
  inline Float SubJust(const Float &other) const { return x - other.x; }
  // 数学被减法
  inline Float SubJustBy(const Float &other) const { return other.x - x; }
  // 数学乘法
  // 注：
  //   会改变内部存储值
  inline Float &Mul(const Float &other) {
    x *= other.x;
    return *this;
  }
  // 数学乘法
  inline Float MulJust(const Float &other) const { return x * other.x; }
  // 数学除法
  // 注：
  //   会改变内部存储值
  inline Float &Div(const Float &other) {
    x /= other.x;
    return *this;
  }
  // 数学除法
  inline Float DivJust(const Float &other) const { return x / other.x; }

 public:
  // 将值改为[0, 1)区间上随机的Float
  // 返回:
  //   [0, 1)区间上随机的Float
  inline Float &SetRandom() {
    x = Random().x;
    return *this;
  }
  // 将值改为[min, max)区间上随机的Float
  // 参数:
  //   - max [Float] : 随机区间上界
  // 返回:
  //   [min, max)区间上随机的Float
  inline Float &SetRandom(const Float &min, const Float &max) {
    x = Random(min, max).x;
    return *this;
  }
  // 取得[0, 1)区间上随机的Float
  // 返回:
  //   [0, 1)区间上随机的Float
  static inline Float Random() {
    const auto random_float = static_cast<float>(rand()) / RAND_MAX;
    return random_float;
  }
  // 取得[min, max)区间上随机的Float
  // 参数:
  //   - min [Float] : 随机区间下界
  //   - max [Float] : 随机区间上界
  // 返回:
  //   [min, max)区间上随机的Float
  static inline Float Random(const Float &min, const Float &max) {
    const auto scale = static_cast<float>(rand()) / RAND_MAX;
    const auto range = max.x - min.x;
    return min.x + scale * range;
  }

  // 将值钳制在[min, max]之间
  // 参数:
  //   - min [Float] : 钳制目标下界
  //   - max [Float] : 钳制目标上界
  // 返回:
  //   钳制后的值
  // 约定:
  //   - min <= max
  inline Float SetClamp(const Float &min, const Float &max) {
    if (x <= min.x) return x = min;
    if (x >= max.x) return x = max;
    return x;
  }
  // 钳制函数，会返回value在[0, 1]之间的值
  // 参数:
  //   - value [Float] : 要钳制的值
  // 返回:
  //   钳制后的值
  static inline Float Clamp(const Float &value) {
    if (value.x <= 0.f) return 0;
    if (value.x >= 1.f) return 1;
    return value.x;
  }
  // 钳制函数，会返回value在[min, max]之间的值
  // 参数:
  //   - value [Float] : 要钳制的值
  //   - min   [Float] : 钳制目标下界
  //   - max   [Float] : 钳制目标上界
  // 返回:
  //   钳制后的值
  // 约定:
  //   - min<=max
  static inline Float Clamp(const Float &value, const Float &min,
                            const Float &max) {
    if (value.x <= min.x) return min;
    if (value.x >= max.x) return max;
    return value;
  }

  // 插值，但是不进行钳制
  // 参数:
  //   - start [Float] : 插值起始值
  //   - end   [Float] : 插值目标值
  //   - t     [Float] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Float LerpWithoutClamp(const Float &start, const Float &end,
                                       const Float &t) {
    return start.x + (end.x - start.x) * t.x;
  }
  // 插值，同时进行钳制
  // 参数:
  //   - start [Float] : 插值起始值
  //   - end   [Float] : 插值目标值
  //   - t     [Float] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Float Lerp(const Float &start, const Float &end,
                           const Float &t) {
    auto clamped_t = Clamp(t);
    return LerpWithoutClamp(start, end, clamped_t);
  }
  // TODO 编写注释
  inline Float ScaleFromTo(const Float &from_min, const Float &from_max,
                           const Float &to_min, const Float &to_max) {
    auto from_range = from_min.x - from_max.x;
    auto to_range = to_min.x - to_max.x;
    return to_min.x + (x - from_min.x) * (to_range / from_range);
  }
};

inline Float operator-(const Float &r) { return -r.x; }
inline Float operator+(const Float &l, const float &r) { return l.x + r; }
inline Float operator+(const float &l, const Float &r) { return l + r.x; }
inline Float operator+(const Float &l, const Float &r) { return l.x + r.x; }
inline Float operator-(const Float &l, const float &r) { return l.x - r; }
inline Float operator-(const float &l, const Float &r) { return l - r.x; }
inline Float operator-(const Float &l, const Float &r) { return l.x - r.x; }
inline Float operator*(const Float &l, const float &r) { return l.x * r; }
inline Float operator*(const float &l, const Float &r) { return l * r.x; }
inline Float operator*(const Float &l, const Float &r) { return l.x * r.x; }
inline Float operator/(const Float &l, const float &r) { return l.x / r; }
inline Float operator/(const float &l, const Float &r) { return l / r.x; }
inline Float operator/(const Float &l, const Float &r) { return l.x / r.x; }
inline Float &operator+=(Float &l, const float &r) { return l.Add(r); }
inline Float &operator+=(Float &l, const Float &r) { return l.Add(r); }
inline Float &operator-=(Float &l, const float &r) { return l.Sub(r); }
inline Float &operator-=(Float &l, const Float &r) { return l.Sub(r); }
inline Float &operator*=(Float &l, const float &r) { return l.Mul(r); }
inline Float &operator*=(Float &l, const Float &r) { return l.Mul(r); }
inline Float &operator/=(Float &l, const float &r) { return l.Div(r); }
inline Float &operator/=(Float &l, const Float &r) { return l.Div(r); }
inline bool operator>(const Float &l, const float &r) { return l.x > r; }
inline bool operator>(const float &l, const Float &r) { return l > r.x; }
inline bool operator>(const Float &l, const Float &r) { return l.x > r.x; }
inline bool operator<(const Float &l, const float &r) { return l.x < r; }
inline bool operator<(const float &l, const Float &r) { return l < r.x; }
inline bool operator<(const Float &l, const Float &r) { return l.x < r.x; }
inline bool operator>=(const Float &l, const float &r) { return l.x >= r; }
inline bool operator>=(const float &l, const Float &r) { return l >= r.x; }
inline bool operator>=(const Float &l, const Float &r) { return l.x >= r.x; }
inline bool operator<=(const Float &l, const float &r) { return l.x <= r; }
inline bool operator<=(const float &l, const Float &r) { return l <= r.x; }
inline bool operator<=(const Float &l, const Float &r) { return l.x <= r.x; }
inline bool operator==(const Float &l, const float &r) { return l.x == r; }
inline bool operator==(const float &l, const Float &r) { return l == r.x; }
inline bool operator==(const Float &l, const Float &r) { return l.x == r.x; }

}  // namespace Xn
