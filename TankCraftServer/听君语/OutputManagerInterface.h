#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <string>

#include "typedef.h"

namespace Xn {

class OutputManagerInterface {
 public:
  virtual class AudioManager* GetOutputManager() = 0;
};

}  // namespace Xn
