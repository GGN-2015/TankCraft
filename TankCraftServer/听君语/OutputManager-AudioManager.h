#pragma once

#include <string>

#include "typedef.h"

namespace Xn {

class OutputManager;

class AudioManager {
  friend OutputManager;

 public:
  AudioManager();

  void PlayAudio(const byte* const& audio_bytes);
  void PlayMusic(const byte* const& audio_bytes);
  void StopAll();
};

}  // namespace Xn
