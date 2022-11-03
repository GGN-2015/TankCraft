#include "Math.h"

using namespace Xn;

// Int.h
const Int Int::ZERO = Int();
const Int Int::ONE = Int(1.0f);
const Int Int::MAX = Int(0x7FFF);

// Float.h
const Float Float::ZERO = Float();
const Float Float::ONE = Float(1.0f);
const Float Float::PI = Float(3.14159265358979323846f);

// Vector2.h
const Vector2 Vector2::ZERO = Vector2();
const Vector2 Vector2::X = Vector2(1, 0);
const Vector2 Vector2::Y = Vector2(0, 1);
const Vector2 Vector2::ONE = Vector2(1, 1);

// Vector3.h
const Vector3 Vector3::ZERO = Vector3();
const Vector3 Vector3::X = Vector3(1, 0, 0);
const Vector3 Vector3::Y = Vector3(0, 1, 0);
const Vector3 Vector3::Z = Vector3(0, 0, 1);
const Vector3 Vector3::ONE = Vector3(1, 1, 1);

// Vector4.h
const Vector4 Vector4::ZERO = Vector4();
const Vector4 Vector4::X = Vector4(1, 0, 0, 0);
const Vector4 Vector4::Y = Vector4(0, 1, 0, 0);
const Vector4 Vector4::Z = Vector4(0, 0, 1, 0);
const Vector4 Vector4::W = Vector4(0, 0, 0, 1);
const Vector4 Vector4::ONE = Vector4(1, 1, 1, 1);
