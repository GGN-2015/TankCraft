#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math-Float.h"
#include "Math-Vector2.h"

namespace Xn {

struct Vector3 {
  Vector3() : x(0), y(0), z(0) {}
  Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z) {}
  Vector3(const Float &x, const Float &y, const Float &z) : x(x), y(y), z(z) {}
  Vector3(const Float (&floats)[3]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }
  Vector3(const Vector2 &vector, const Float &z)
      : x(vector.x), y(vector.y), z(z) {}

  struct Pos {
    Pos(const Float &x, const Float &y, const Float &z) : x(x), y(y), z(z) {}

    Float x, y, z;

    inline operator Vector3() { return Vector3(x, y, z); }
  };
  struct Color {
    Color(const Float &R, const Float &G, const Float &B) : R(R), G(G), B(B) {}

    Float R, G, B;

    inline operator Vector3() { return Vector3(R, G, B); }

   public:
    static inline Color RGB255(const uint8 &R, const uint8 &G, const uint8 &B) {
      return Color(R / 255.f, G / 255.f, B / 255.f);
    }
  };
  struct Floats {
    Floats(const Float (&floats)[3]) {
      std::memcpy(this->floats, floats, sizeof floats);
    }

    Float floats[3];

    inline operator Vector3() { return Vector3(floats); }
  };

  union {
    struct {
      Float x, y, z;
    };
    Pos asPos;
    Color asColor;
    Floats asFloats;
  };

 public:
  static const Vector3 ZERO;  // 0, 0, 0
  static const Vector3 X;     // 1, 0, 0
  static const Vector3 Y;     // 0, 1, 0
  static const Vector3 Z;     // 0, 0, 1
  static const Vector3 ONE;   // 1, 1, 1

 public:
  // 取得[min, max)矩形区间上随机的Vector3
  // 参数:
  //   - min [Vector3] : 随机区间下界
  //   - max [Vector3] : 随机区间上界
  // 返回:
  //   [min, max)矩形区间上随机的Vector3
  static inline Vector3 Random(const Vector3 &min, const Vector3 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    auto z = Float::Random(min.z, max.z);
    return Vector3(x, y, z);
  }

  // 插值，同时进行钳制
  // 参数:
  //   - start [Vector3] : 插值起始值
  //   - end   [Vector3] : 插值目标值
  //   - t     [Vector3] : 插值参数：0时，返回值==start；1时，返回值==end
  // 返回:
  //   插值
  static inline Vector3 Lerp(const Vector3 &start, const Vector3 &end,
                             const Float &t) {
    auto clamped_t = Float::Clamp(t);
    auto x = Float::LerpWithoutClamp(start.x, end.x, clamped_t);
    auto y = Float::LerpWithoutClamp(start.y, end.y, clamped_t);
    auto z = Float::LerpWithoutClamp(start.z, end.z, clamped_t);
    return Vector3(x, y, z);
  }

 public:
  // 取得向量的模
  // 返回:
  //   - 向量的模
  inline Float Norm() const { return std::sqrt(x * x + y * y + z * z); }

 public:
  inline Vector3 &operator=(const Vector3 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  inline Vector3 operator+(const Vector3 &other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }
  inline Vector3 operator-(const Vector3 &other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }
  inline Vector3 &operator+=(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  inline Vector3 &operator-=(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  inline explicit operator Float() { return this->Norm(); }
  inline explicit operator Vector2() { return Vector2(x, y); }
};

struct Vector3Int {
  Vector3Int(const int &x, const int &y, const int &z) : x(x), y(y), z(z) {}
  Vector3Int(const int (&ints)[3]) : x(ints[0]), y(ints[1]), z(ints[2]) {}

  union {
    struct {
      int x, y, z;
    };
    struct {
      int R, G, B;
    };
    int floats[3];
  };
};

}  // namespace Xn
