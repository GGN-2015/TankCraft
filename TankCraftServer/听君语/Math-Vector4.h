#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math-Float.h"

namespace Xn {

struct Vector4 {
  Vector4() : x(0), y(0), z(0), w(0) {}
  Vector4(const Float &x, const Float &y, const Float &z, const Float &w)
      : x(x), y(y), z(z), w(w) {}
  Vector4(const float (&floats)[4]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }

  union {
    struct {
      Float x, y, z, w;
    };
    struct Pos {
      Float x, y, z, w;
    } asPos;
    struct Color {
      Float R, G, B, A;
    } asColor;
    struct Rect {
      Float left, top, right, bottom;
    } asRect;
    struct Floats {
      Float floats[4];
    } asFloats;
  };

 public:
  static const Vector4 ZERO;  // 0, 0, 0, 0
  static const Vector4 X;     // 1, 0, 0, 0
  static const Vector4 Y;     // 0, 1, 0, 0
  static const Vector4 Z;     // 0, 0, 1, 0
  static const Vector4 W;     // 0, 0, 0, 1
  static const Vector4 ONE;   // 1, 1, 1, 1

 public:
  // 取得[min, max)矩形区间上随机的Vector4
  // 参数:
  //   - min [Vector4] : 随机区间下界
  //   - max [Vector4] : 随机区间上界
  // 返回:
  //   [min, max)矩形区间上随机的Vector4
  static inline Vector4 Random(const Vector4 &min, const Vector4 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    auto z = Float::Random(min.z, max.z);
    auto w = Float::Random(min.w, max.w);
    return Vector4(x, y, z, w);
  }

  // 插值，同时进行钳制
  // 参数:
  //   - start [Vector4] : 插值起始值
  //   - end   [Vector4] : 插值目标值
  //   - t     [Vector4] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Vector4 Lerp(const Vector4 &start, const Vector4 &end,
                             const Float &t) {
    auto clamped_t = Float::Clamp(t);
    auto x = Float::LerpWithoutClamp(start.x, end.x, clamped_t);
    auto y = Float::LerpWithoutClamp(start.y, end.y, clamped_t);
    auto z = Float::LerpWithoutClamp(start.z, end.z, clamped_t);
    auto w = Float::LerpWithoutClamp(start.w, end.w, clamped_t);
    return Vector4(x, y, z, w);
  }

 public:
  // 取得向量的模
  // 返回:
  //   - 向量的模
  inline Float Norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }

 public:
  inline Vector4 &operator=(const Vector4 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.x;
    return *this;
  }
  inline Vector4 operator+(const Vector4 &other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
  }
  inline Vector4 operator-(const Vector4 &other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
  }
  inline Vector4 &operator+=(const Vector4 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }
  inline Vector4 &operator-=(const Vector4 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }
  inline explicit operator Float() { return this->Norm(); }
};

struct Vector4Int {
  Vector4Int(const int &x, const int &y, const int &z, const int &w)
      : x(x), y(y), z(z), w(w) {}
  Vector4Int(const int (&ints)[4])
      : x(ints[0]), y(ints[1]), z(ints[2]), w(ints[3]) {}

  union {
    struct {
      int x, y, z, w;
    };
    struct {
      int R, G, B, A;
    };
    int ints[4];
  };
};

struct Vector4Byte {
  Vector4Byte(const short &x, const short &y, const short &z, const short &w)
      : x(x), y(y), z(z), w(w) {}
  Vector4Byte(const short (&bytes)[4])
      : x(bytes[0]), y(bytes[1]), z(bytes[2]), w(bytes[3]) {}

  union {
    struct {
      short x, y, z, w;
    };
    struct {
      short R, G, B, A;
    };
    short bytes[4];
  };
};

}  // namespace Xn
