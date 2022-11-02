#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math-Float.h"

namespace Xn {

struct Vector2 {
  Vector2() : x(0), y(0) {}
  Vector2(const Float &x, const Float &y) : x(x), y(y) {}
  Vector2(const Float (&floats)[2]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }

  union {
    struct {
      Float x, y;
    };
    struct Pos {
      Float x, y;
    } asPos;
    struct Pair {
      Float first, second;
    } asPair;
    struct WH {
      Float width, height;

     public:
      inline Float GetArea() const { return width * height; }
    } asWH;
    struct Floats {
      Float floats[2];
    } asFloats;
  };

 public:
  static const Vector2 ZERO;  // 0, 0
  static const Vector2 X;     // 1, 0
  static const Vector2 Y;     // 0, 1
  static const Vector2 ONE;   // 1, 1

 public:
  // 取得[min, max)矩形区间上随机的Vector2
  // 参数:
  //   - min [Vector2] : 随机区间下界
  //   - max [Vector2] : 随机区间上界
  // 返回:
  //   [min, max)矩形区间上随机的Vector2
  static inline Vector2 Random(const Vector2 &min, const Vector2 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    return Vector2(x, y);
  }

  // 插值，同时进行钳制
  // 参数:
  //   - start [Vector2] : 插值起始值
  //   - end   [Vector2] : 插值目标值
  //   - t     [Vector2] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Vector2 Lerp(const Vector2 &start, const Vector2 &end,
                             const Float &t) {
    auto clamped_t = Float::Clamp(t);
    auto x = Float::LerpWithoutClampe(start.x, end.x, clamped_t);
    auto y = Float::LerpWithoutClampe(start.y, end.y, clamped_t);
    return Vector2(x, y);
  }

 public:
  // 取得向量的模
  // 返回:
  //   - 向量的模
  inline Float Norm() const { return std::sqrt(x * x + y * y); }

 public:
  inline Vector2 &operator=(const Vector2 &other) {
    x = other.x;
    y = other.y;
    return *this;
  }
  inline Vector2 operator+(const Vector2 &other) const {
    return Vector2(x + other.x, y + other.y);
  }
  inline Vector2 operator-(const Vector2 &other) const {
    return Vector2(x - other.x, y - other.y);
  }
  inline Vector2 &operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vector2 &operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  inline explicit operator Float() { return this->Norm(); }
};

struct Vector2Int {
  Vector2Int() : x(0), y(0) {}
  Vector2Int(const int &x, const int &y) : x(x), y(y) {}
  Vector2Int(const int (&ints)[2]) : x(ints[0]), y(ints[1]) {}

  union {
    struct {
      int x, y;
    };
    struct {
      int first, second;
    };
    int floats[2];
  };
};

}  // namespace Xn
