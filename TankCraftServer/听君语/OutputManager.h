#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "OutputManager-AudioManager.h"
#include "OutputManagerInterface.h"
#include "SingletonBaseClass.h"

namespace Xn {

class OutputManager : public OutputManagerInterface {
 public:
  OutputManager();

  // 通过 OutputManagerInterface 继承
  virtual void PlayAudio(const byte* const& audio_bytes) override;
  virtual void PlayAudioWithLoop(const byte* const& audio_bytes) override;
  virtual void StopAllAudio() override;

 public:
  AudioManager audio_manager_;

  CAN_NOT_COPY_THIS_CLASS(OutputManager);
};

}  // namespace Xn
