#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <random>

namespace Xn {

struct Float {
  Float() : x(0) {}
  Float(float value) : x(value) {}
  Float(int value) : x(static_cast<float>(value)) {}
  Float(unsigned int value) : x(static_cast<float>(value)) {}

  float x;

 public:
  static const Float ZERO;  // 0
  static const Float ONE;   // 1
  static const Float PI;    // pi ~ 3.14……

 public:
  // 取得[0, 1)区间上随机的Float
  // 返回:
  //   [0, 1)区间上随机的Float
  static inline Float Random() {
    Float random_float = static_cast<float>(rand()) / RAND_MAX;
    return random_float;
  }
  // 取得[min, max)区间上随机的Float
  // 参数:
  //   - min [Float] : 随机区间下界
  //   - max [Float] : 随机区间上界
  // 返回:
  //   [min, max)区间上随机的Float
  static inline Float Random(const Float &min, const Float &max) {
    Float scale = static_cast<float>(rand()) / RAND_MAX;
    Float range = max - min;
    return min + scale * range;
  }

  // 钳制函数，会返回value在[0, 1]之间的值
  // 参数:
  //   - value [Float] : 要钳制的值
  // 返回:
  //   钳制后的值
  static inline Float Clamp(const Float &value) {
    if (value <= 0.f) return 0;
    if (value >= 1.f) return 1;
    return value;
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
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
  }

  // 插值，但是不进行钳制
  // 参数:
  //   - start [Float] : 插值起始值
  //   - end   [Float] : 插值目标值
  //   - t     [Float] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Float LerpWithoutClampe(const Float &start, const Float &end,
                                        const Float &t) {
    return start + (end - start) * t;
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
    return LerpWithoutClampe(start, end, clamped_t);
  }

 public:
  inline operator float() const { return x; }
  inline Float &operator=(const Float &other) {
    x = other.x;
    return *this;
  }
  inline Float operator+(const Float &other) const { return x + other.x; }
  inline Float operator-(const Float &other) const { return x - other.x; }
  inline Float operator*(const Float &other) const { return x * other.x; }
  inline Float operator/(const Float &other) const { return x / other.x; }
  inline Float &operator+=(const Float &other) {
    x += other.x;
    return *this;
  }
  inline Float &operator-=(const Float &other) {
    x -= other.x;
    return *this;
  }
  inline Float &operator*=(const Float &other) {
    x *= other.x;
    return *this;
  }
  inline Float &operator/=(const Float &other) {
    x /= other.x;
    return *this;
  }
  inline bool operator>(const Float &other) const { return x > other.x; }
  inline bool operator<(const Float &other) const { return x < other.x; }
  inline bool operator>=(const Float &other) const { return x >= other.x; }
  inline bool operator<=(const Float &other) const { return x <= other.x; }
  inline bool operator==(const Float &other) const { return x == other.x; }
  inline Float &operator=(const float &other) {
    x = other;
    return *this;
  }
  inline Float operator+(const float &other) const { return x + other; }
  inline Float operator-(const float &other) const { return x - other; }
  inline Float operator*(const float &other) const { return x * other; }
  inline Float operator/(const float &other) const { return x / other; }
  inline Float &operator+=(const float &other) {
    x += other;
    return *this;
  }
  inline Float &operator-=(const float &other) {
    x -= other;
    return *this;
  }
  inline Float &operator*=(const float &other) {
    x *= other;
    return *this;
  }
  inline Float &operator/=(const float &other) {
    x /= other;
    return *this;
  }
  inline bool operator>(const float &other) const { return x > other; }
  inline bool operator<(const float &other) const { return x < other; }
  inline bool operator>=(const float &other) const { return x >= other; }
  inline bool operator<=(const float &other) const { return x <= other; }
  inline bool operator==(const float &other) const { return x == other; }
  inline operator bool() const { return x != 0; }
  inline operator int() const { return static_cast<int>(x); }
};

}  // namespace Xn
