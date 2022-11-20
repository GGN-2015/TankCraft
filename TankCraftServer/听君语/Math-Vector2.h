#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Math-Float.h"

namespace Xn {

struct Vector2 {
  Vector2() : x(0), y(0) {}
  Vector2(const Vector2 &other) : x(other.x), y(other.y) {}
  Vector2(const Float &x, const Float &y) : x(x), y(y) {}
  Vector2(const Float (&floats)[2]) {
    std::memcpy(this->asFloats.floats, floats, sizeof floats);
  }

  struct Pos {
    Pos(const Float &x, const Float &y) : x(x), y(y) {}

    Float x, y;

    operator Vector2() { return Vector2(x, y); }
  };
  struct Pair {
    Pair(const Float &first, const Float &second)
        : first(first), second(second) {}

    Float first, second;

    inline operator Vector2() { return Vector2(first, second); }
  };
  struct WH {
    WH(const Float &width, const Float &height)
        : width(width), height(height) {}

    Float width, height;

    inline operator Vector2() { return Vector2(width, height); }

   public:
    inline Float GetArea() const { return width * height; }
  };
  struct Floats {
    Floats(const Float (&floats)[2]) {
      std::memcpy(this->floats, floats, sizeof floats);
    }

    Float floats[2];

    inline operator Vector2() { return Vector2(floats); }
  };

  union {
    struct {
      Float x, y;
    };
    Pos asPos;
    Pair asPair;
    WH asWH;
    Floats asFloats;
  };

 public:
  static const Vector2 ZERO;  // 0, 0
  static const Vector2 X;     // 1, 0
  static const Vector2 Y;     // 0, 1
  static const Vector2 ONE;   // 1, 1

 public:
  // ȡ��[min, max)���������������Vector2
  // ����:
  //   - min [Vector2] : ��������½�
  //   - max [Vector2] : ��������Ͻ�
  // ����:
  //   [min, max)���������������Vector2
  static inline Vector2 Random(const Vector2 &min, const Vector2 &max) {
    auto x = Float::Random(min.x, max.x);
    auto y = Float::Random(min.y, max.y);
    return Vector2(x, y);
  }

  // ��ֵ��ͬʱ����ǯ��
  // ����:
  //   - start [Vector2] : ��ֵ��ʼֵ
  //   - end   [Vector2] : ��ֵĿ��ֵ
  //   - t     [Vector2] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
  static inline Vector2 Lerp(const Vector2 &start, const Vector2 &end,
                             const Float &t) {
    auto clamped_t = Float::Clamp(t);
    return LerpWithoutClamp(start, end, clamped_t);
  }
  // ��ֵ��������ǯ��
  // ����:
  //   - start [Vector2] : ��ֵ��ʼֵ
  //   - end   [Vector2] : ��ֵĿ��ֵ
  //   - t     [Vector2] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
  static inline Vector2 LerpWithoutClamp(const Vector2 &start,
                                         const Vector2 &end, const Float &t) {
    auto x = Float::LerpWithoutClamp(start.x, end.x, t);
    auto y = Float::LerpWithoutClamp(start.y, end.y, t);
    return Vector2(x, y);
  }

 public:
  // ȡ��������ģ
  // ����:
  //   - ������ģ
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
