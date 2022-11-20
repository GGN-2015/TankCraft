#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Math-Float.h"
#include "Math-Vector2.h"
#include "Math-Vector3.h"

namespace Xn {

struct Vector4 {
  Vector4() : x(0), y(0), z(0), w(0) {}
  Vector4(const Vector4 &other)
      : x(other.x), y(other.y), z(other.z), w(other.w) {}
  Vector4(const Float &x, const Float &y, const Float &z, const Float &w)
      : x(x), y(y), z(z), w(w) {}
  Vector4(const float (&floats)[4]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }
  Vector4(const Vector2 &vector, const Float &z, const Float &w)
      : x(vector.x), y(vector.y), z(z), w(w) {}
  Vector4(const Vector3 &vector, const Float &w)
      : x(vector.x), y(vector.y), z(vector.z), w(w) {}

  struct Pos {
    Pos(const Float &x, const Float &y, const Float &z, const Float &w)
        : x(x), y(y), z(z), w(w) {}

    Float x, y, z, w;
  };
  struct Color {
    Color(const Float &R, const Float &G, const Float &B, const Float &A)
        : R(R), G(G), B(B), A(A) {}
    Color(const Vector3::Color &color, const Float &A = 1)
        : R(color.R), G(color.G), B(color.B), A(A) {}

    Float R, G, B, A;

   public:
    static inline Color RGB255(const uint8 &R, const uint8 &G, const uint8 &B,
                               const uint8 &A) {
      return Color(R / 255.f, G / 255.f, B / 255.f, A / 255.f);
    }
  };
  struct Rect {
    Rect(const Float &left, const Float &top, const Float &right,
         const Float &bottom)
        : left(left), top(top), right(right), bottom(bottom) {}

    Float left, top, right, bottom;

   public:
    Vector2::WH GetWH() { return Vector2::WH(right - left, bottom - top); }
  };
  struct Floats {
    Floats(const float (&floats)[4]) {
      std::memcpy(this->floats, floats, sizeof floats);
    }

    Float floats[4];
  };

  union {
    struct {
      Float x, y, z, w;
    };
    Pos asPos;
    Color asColor;
    Rect asRect;
    Floats asFloats;
  };

 public:
  static const Vector4 ZERO;  // 0, 0, 0, 0
  static const Vector4 X;     // 1, 0, 0, 0
  static const Vector4 Y;     // 0, 1, 0, 0
  static const Vector4 Z;     // 0, 0, 1, 0
  static const Vector4 W;     // 0, 0, 0, 1
  static const Vector4 ONE;   // 1, 1, 1, 1

 public:
  inline operator Vector3() const { return Vector3(x, y, z); }

 public:
  // ȡ��[min, max)���������������Vector4
  // ����:
  //   - min [Vector4] : ��������½�
  //   - max [Vector4] : ��������Ͻ�
  // ����:
  //   [min, max)���������������Vector4
  static inline Vector4 Random(const Vector4 &min, const Vector4 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    auto z = Float::Random(min.z, max.z);
    auto w = Float::Random(min.w, max.w);
    return Vector4(x, y, z, w);
  }

  // ��ֵ��ͬʱ����ǯ��
  // ����:
  //   - start [Vector4] : ��ֵ��ʼֵ
  //   - end   [Vector4] : ��ֵĿ��ֵ
  //   - t     [Vector4] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
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
  // ȡ��������ģ
  // ����:
  //   - ������ģ
  inline Float Norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }

 public:
  inline Vector4 &operator=(const Vector4 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
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
