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
  Float(int value) : x(static_cast<float>(value)) {}
  Float(unsigned int value) : x(static_cast<float>(value)) {}

  float x;

 public:
  static const Float ZERO;  // 0
  static const Float ONE;   // 1
  static const Float PI;    // pi ~ 3.14����

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
    if (value <= 0.f) return 0;
    if (value >= 1.f) return 1;
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
  static inline Float LerpWithoutClampe(const Float &start, const Float &end,
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
    return LerpWithoutClampe(start, end, clamped_t);
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
  inline Float &operator=(const float &other) {
    x = other;
    return *this;
  }
  inline Float operator+(const float &other) const { return x + other; }
  inline Float operator-(const float &other) const { return x - other; }
  inline Float operator*(const float &other) const { return x * other; }
  inline Float operator/(const float &other) const { return x / other; }
  inline Float &operator+=(const float &other) {
    x += other;
    return *this;
  }
  inline Float &operator-=(const float &other) {
    x -= other;
    return *this;
  }
  inline Float &operator*=(const float &other) {
    x *= other;
    return *this;
  }
  inline Float &operator/=(const float &other) {
    x /= other;
    return *this;
  }
  inline bool operator>(const float &other) const { return x > other; }
  inline bool operator<(const float &other) const { return x < other; }
  inline bool operator>=(const float &other) const { return x >= other; }
  inline bool operator<=(const float &other) const { return x <= other; }
  inline bool operator==(const float &other) const { return x == other; }
  inline operator bool() const { return x != 0; }
  inline operator int() const { return static_cast<int>(x); }
};

}  // namespace Xn
