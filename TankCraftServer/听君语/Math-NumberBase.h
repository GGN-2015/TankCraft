#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <random>
#include <string>

#include "typedef.h"

namespace Xn {

namespace XnMath {

struct NumberBase {
 protected:
  NumberBase() = default;

 public:
  inline virtual std::wstring ToString() const = 0;

 public:
  inline virtual operator bool() const = 0;
};

}  // namespace XnMath

}  // namespace Xn
