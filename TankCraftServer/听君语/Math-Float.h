#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
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
  static const Float PI;    // pi ~ 3.14����

 public:
  inline std::wstring ToString() const { return std::to_wstring(x); }

 public:
  inline operator float() const { return x; }
  inline operator int() const { return static_cast<int>(x); }
  inline int ToInt() const { return static_cast<int>(x); }

 public:
  // ��ѧ��ֵ
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &Set(const Float &other) {
    x = other.x;
    return *this;
  }
  // ��ѧȡֵ
  inline const Float &Get() { return x; }
  // ��ѧ�ӷ�
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &Add(const Float &other) {
    x += other.x;
    return *this;
  }
  // ��ѧ�ӷ�
  inline Float AddJust(const Float &other) const { return x + other.x; }
  // ��ѧ����
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &Sub(const Float &other) {
    x -= other.x;
    return *this;
  }
  // ��ѧ������
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &SubBy(const Float &other) {
    x = other.x - x;
    return *this;
  }
  // ��ѧ����
  inline Float SubJust(const Float &other) const { return x - other.x; }
  // ��ѧ������
  inline Float SubJustBy(const Float &other) const { return other.x - x; }
  // ��ѧ�˷�
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &Mul(const Float &other) {
    x *= other.x;
    return *this;
  }
  // ��ѧ�˷�
  inline Float MulJust(const Float &other) const { return x * other.x; }
  // ��ѧ����
  // ע��
  //   ��ı��ڲ��洢ֵ
  inline Float &Div(const Float &other) {
    x /= other.x;
    return *this;
  }
  // ��ѧ����
  inline Float DivJust(const Float &other) const { return x / other.x; }

 public:
  // ��ֵ��Ϊ[0, 1)�����������Float
  // ����:
  //   [0, 1)�����������Float
  inline Float &SetRandom() {
    x = Random().x;
    return *this;
  }
  // ��ֵ��Ϊ[min, max)�����������Float
  // ����:
  //   - max [Float] : ��������Ͻ�
  // ����:
  //   [min, max)�����������Float
  inline Float &SetRandom(const Float &min, const Float &max) {
    x = Random(min, max).x;
    return *this;
  }
  // ȡ��[0, 1)�����������Float
  // ����:
  //   [0, 1)�����������Float
  static inline Float Random() {
    const auto random_float = static_cast<float>(rand()) / RAND_MAX;
    return random_float;
  }
  // ȡ��[min, max)�����������Float
  // ����:
  //   - min [Float] : ��������½�
  //   - max [Float] : ��������Ͻ�
  // ����:
  //   [min, max)�����������Float
  static inline Float Random(const Float &min, const Float &max) {
    const auto scale = static_cast<float>(rand()) / RAND_MAX;
    const auto range = max.x - min.x;
    return min.x + scale * range;
  }

  // ��ֵǯ����[min, max]֮��
  // ����:
  //   - min [Float] : ǯ��Ŀ���½�
  //   - max [Float] : ǯ��Ŀ���Ͻ�
  // ����:
  //   ǯ�ƺ��ֵ
  // Լ��:
  //   - min <= max
  inline Float SetClamp(const Float &min, const Float &max) {
    if (x <= min.x) return x = min;
    if (x >= max.x) return x = max;
    return x;
  }
  // ǯ�ƺ������᷵��value��[0, 1]֮���ֵ
  // ����:
  //   - value [Float] : Ҫǯ�Ƶ�ֵ
  // ����:
  //   ǯ�ƺ��ֵ
  static inline Float Clamp(const Float &value) {
    if (value.x <= 0.f) return 0;
    if (value.x >= 1.f) return 1;
    return value.x;
  }
  // ǯ�ƺ������᷵��value��[min, max]֮���ֵ
  // ����:
  //   - value [Float] : Ҫǯ�Ƶ�ֵ
  //   - min   [Float] : ǯ��Ŀ���½�
  //   - max   [Float] : ǯ��Ŀ���Ͻ�
  // ����:
  //   ǯ�ƺ��ֵ
  // Լ��:
  //   - min<=max
  static inline Float Clamp(const Float &value, const Float &min,
                            const Float &max) {
    if (value.x <= min.x) return min;
    if (value.x >= max.x) return max;
    return value;
  }

  // ��ֵ�����ǲ�����ǯ��
  // ����:
  //   - start [Float] : ��ֵ��ʼֵ
  //   - end   [Float] : ��ֵĿ��ֵ
  //   - t     [Float] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
  static inline Float LerpWithoutClamp(const Float &start, const Float &end,
                                       const Float &t) {
    return start.x + (end.x - start.x) * t.x;
  }
  // ��ֵ��ͬʱ����ǯ��
  // ����:
  //   - start [Float] : ��ֵ��ʼֵ
  //   - end   [Float] : ��ֵĿ��ֵ
  //   - t     [Float] : ��ֵ������0ʱ������ֵ==start��1ʱ������ֵ==end
  // ����:
  //   ��ֵ
  static inline Float Lerp(const Float &start, const Float &end,
                           const Float &t) {
    auto clamped_t = Clamp(t);
    return LerpWithoutClamp(start, end, clamped_t);
  }
  // TODO ��дע��
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
