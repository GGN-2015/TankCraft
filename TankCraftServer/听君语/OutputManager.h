#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "OutputManager-AudioManager.h"
#include "OutputManagerInterface.h"
#include "SingletonBaseClass.h"

namespace Xn {

class OutputManager : public OutputManagerInterface {
 public:
  OutputManager(const HWND& window_handle);

  AudioManager* GetOutputManager() { return &audio_manager_; }

 private:
  AudioManager audio_manager_;

  CAN_NOT_COPY_THIS_CLASS(OutputManager);
};

}  // namespace Xn
