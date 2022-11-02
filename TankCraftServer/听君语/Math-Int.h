#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
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
  // ȡ��[0, max)�����������Int
  // ����:
  //   - max [Int] : ��������Ͻ�
  // ����:
  //   [0, max)�����������Int
  static inline Int Random(const Int &max) {
    const Int random_value = rand();
    return random_value % max;
  }
  // ȡ��[min, max)�����������Int
  // ����:
  //   - min [Int] : ��������½�
  //   - max [Int] : ��������Ͻ�
  // ����:
  //   [min, max)�����������Int
  static inline Int Random(const Int &min, const Int &max) {
    const Int random_value = rand();
    const Int range = max - min;
    return min + random_value % range;
  }

  // ǯ�ƺ������᷵��value��[0, max]֮���ֵ
  // ����:
  //   - value [Int] : Ҫǯ�Ƶ�ֵ
  //   - max [Int]   : ��������Ͻ�
  // ����:
  //   ǯ�ƺ��ֵ
  // Լ��:
  //   - 0 <= max
  static inline Int Clamp(const Int &value, const Int &max) {
    if (value <= 0) return 0;
    if (value >= max) return max;
    return value;
  }
  // ǯ�ƺ������᷵��value��[min, max]֮���ֵ
  // ����:
  //   - value [Int] : Ҫǯ�Ƶ�ֵ
  //   - min   [Int] : ǯ��Ŀ���½�
  //   - max   [Int] : ǯ��Ŀ���Ͻ�
  // ����:
  //   ǯ�ƺ��ֵ
  // Լ��:
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
