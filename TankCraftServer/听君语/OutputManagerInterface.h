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
  virtual void PlayAudio(const byte* const& audio_bytes) = 0;
  virtual void PlayAudioWithLoop(const byte* const& audio_bytes) = 0;

  virtual void StopAllAudio() = 0;
};

}  // namespace Xn
