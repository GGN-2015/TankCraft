#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Math-Float.h"

namespace Xn {

struct Vector3 {
  Vector3() : x(0), y(0), z(0) {}
  Vector3(const Float &x, const Float &y, const Float &z) : x(x), y(y), z(z) {}
  Vector3(const Float (&floats)[3]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }

  union {
    struct {
      Float x, y, z;
    };
    struct Pos {
      Float x, y, z;
    } asPos;
    struct Color {
      Float R, G, B;
    } asColor;
    struct Floats {
      Float floats[3];
    } asFloats;
  };

 public:
  static const Vector3 ZERO;  // 0, 0, 0
  static const Vector3 X;     // 1, 0, 0
  static const Vector3 Y;     // 0, 1, 0
  static const Vector3 Z;     // 0, 0, 1
  static const Vector3 ONE;   // 1, 1, 1

 public:
  // ȡ��[min, max)���������������Vector3
  // ����:
  //   - min [Vector3] : ��������½�
  //   - max [Vector3] : ��������Ͻ�
  // ����:
  //   [min, max)���������������Vector3
  static inline Vector3 Random(const Vector3 &min, const Vector3 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    auto z = Float::Random(min.z, max.z);
    return Vector3(x, y, z);
  }

  // ��ֵ��ͬʱ����ǯ��
  // ����:
  //   - start [Vector3] : ��ֵ��ʼֵ
  //   - end   [Vector3] : ��ֵĿ��ֵ
  //   - t     [Vector3] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
  static inline Vector3 Lerp(const Vector3 &start, const Vector3 &end,
                             const Float &t) {
    auto clamped_t = Float::Clamp(t);
    auto x = Float::LerpWithoutClampe(start.x, end.x, clamped_t);
    auto y = Float::LerpWithoutClampe(start.y, end.y, clamped_t);
    auto z = Float::LerpWithoutClampe(start.z, end.z, clamped_t);
    return Vector3(x, y, z);
  }

 public:
  // ȡ��������ģ
  // ����:
  //   - ������ģ
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
