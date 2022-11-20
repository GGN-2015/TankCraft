#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
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
  // ��ѧ��ֵ
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Set(const Int &other) {
    x = other.x;
    return *this;
  }
  // ��ѧȡֵ
  inline const int &Get() { return x; }
  // ��ѧ�ӷ�
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Add(const Int &other) {
    x += other.x;
    return *this;
  }
  // ��ѧ�ӷ�
  inline Int AddJust(const Int &other) const { return x + other.x; }
  // ��ѧ����
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Sub(const Int &other) {
    x -= other.x;
    return *this;
  }
  // ��ѧ������
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &SubBy(const Int &other) {
    x = other.x - x;
    return *this;
  }
  // ��ѧ����
  inline Int SubJust(const Int &other) const { return x - other.x; }
  // ��ѧ������
  inline Int SubJustBy(const Int &other) const { return other.x - x; }
  // ��ѧ�˷�
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Mul(const Int &other) {
    x *= other.x;
    return *this;
  }
  // ��ѧ�˷�
  inline Int MulJust(const Int &other) const { return x * other.x; }
  // ��ѧ����
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Div(const Int &other) {
    x /= other.x;
    return *this;
  }
  // ��ѧ����
  inline Int DivJust(const Int &other) const { return x / other.x; }
  // ��ѧȡģ
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Int &Mod(const Int &other) {
    x %= other.x;
    return *this;
  }
  // ��ѧȡģ
  inline Int ModJust(const Int &other) const { return x % other.x; }

 public:
  // ��ֵ��Ϊ[0, max)�����������Int
  // ����:
  //   - max [Int] : ��������Ͻ�
  // ����:
  //   [0, max)�����������Int
  inline Int &SetRandom(const Int &max) {
    x = Int(rand()).ModJust(max).x;
    return *this;
  }
  // ȡ��[0, max)�����������Int
  // ����:
  //   - max [Int] : ��������Ͻ�
  // ����:
  //   [0, max)�����������Int
  static inline Int Random(const Int &max) { return Int(rand()).ModJust(max); }
  // ȡ��[min, max)�����������Int
  // ����:
  //   - min [Int] : ��������½�
  //   - max [Int] : ��������Ͻ�
  // ����:
  //   [min, max)�����������Int
  static inline Int Random(const Int &min, const Int &max) {
    const auto range = max.x - min.x;
    return min.x + Random(range).x;
  }

  // ��ֵǯ����[min, max]֮��
  // ����:
  //   - min   [Int] : ǯ��Ŀ���½�
  //   - max   [Int] : ǯ��Ŀ���Ͻ�
  // ����:
  //   ǯ�ƺ��ֵ
  // Լ��:
  //   - min <= max
  inline Int SetClamp(const Int &min, const Int &max) {
    if (x <= min.x) return x = min;
    if (x >= max.x) return x = max;
    return x;
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
    if (value.x <= 0) return 0;
    if (value.x >= max.x) return max;
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
