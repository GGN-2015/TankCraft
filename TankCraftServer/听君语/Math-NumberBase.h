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
  inline std::wstring ToString() const;
};

}  // namespace XnMath

}  // namespace Xn
