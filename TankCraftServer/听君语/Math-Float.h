#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <random>

namespace Xn {

struct Float {
  Float() : x(0) {}
  Float(float value) : x(value) {}

  float x;

 public:
  static const Float ZERO;
  static const Float ONE;
  static const Float PI;

 public:
  // ȡ��[0, 1)�����������Float
  // ����:
  //   [0, 1)�����������Float
  static inline Float Random() {
    Float random_float = static_cast<float>(rand()) / RAND_MAX;
    return random_float;
  }
  // ȡ��[min, max)�����������Float
  // ����:
  //   - min [Float] : ��������½�
  //   - max [Float] : ��������Ͻ�
  // ����:
  //   [min, max)�����������Float
  static inline Float Random(const Float &min, const Float &max) {
    Float scale = static_cast<float>(rand()) / RAND_MAX;
    Float range = max - min;
    return min + scale * range;
  }

  // ǯ�ƺ������᷵��value��[0, 1]֮���ֵ
  // ����:
  //   - value [Float] : Ҫǯ�Ƶ�ֵ
  // ����:
  //   ǯ�ƺ��ֵ
  static inline Float Clamp(const Float &value) {
    if (value <= ZERO) return 0;
    if (value >= ONE) return 1;
    return value;
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
    if (value <= min) return min;
    if (value >= max) return max;
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
    return start + (end - start) * t;
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
    Float from_range = from_min - from_max;
    Float to_range = to_min - to_max;
    return to_min + (*this - from_min) * (to_range / from_range);
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
  inline operator bool() const { return x != 0; }
};

}  // namespace Xn
