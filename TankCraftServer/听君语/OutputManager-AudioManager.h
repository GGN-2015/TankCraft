#pragma once

#include <dsound.h>

#include <string>

#include "stdafx.h"
#include "typedef.h"

#pragma comment(lib, "dsound")

namespace Xn {

class OutputManager;

class AudioManager {
  friend OutputManager;

 public:
  AudioManager(const HWND& window_handle);

  IDirectSoundBuffer* LoadWaveAudio(const wchar* const& wave_path);

 private:
  void InitDSound(const HWND& window_handle);

  Microsoft::WRL::ComPtr<IDirectSound> d_sound_;
};

}  // namespace Xn
