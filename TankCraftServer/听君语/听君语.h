#pragma once
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àĞ´
//
//*********************************************************

#include "InputManagerInterface.h"
#include "ObjectManagerInterface.h"
#include "OutputManagerInterface.h"
#include "PlatformManagerInterface.h"
#include "RenderManagerInterface.h"
#include "SingletonBaseClass.h"
#include "XnTime.h"
#include "stdafx.h"
#include "typedef.h"

namespace Xn {

class Ìı¾ıÓï : public Singleton<Ìı¾ıÓï> {
 public:
  static const constexpr wchar *core_name_ = L"ÏÉµÀ";

 public:
  void Init(const HINSTANCE &instance_handle_, const uint &width,
            const uint &height);

  int Run();

  void Start();
  void Update();
  void Render();

  void OnSizeChanged(uint width, uint height, bool minimized);
  void OnWindowMoved(int, int);
  void OnDestroy();

  float GetDeltaTime();
  float GetTimeFromStart();

  class PlatformManager *GetPlatformManager() {
    return (PlatformManager *)platform_manager_.get();
  }
  class RenderManager *GetRenderManager() {
    return (RenderManager *)render_manager_.get();
  }
  class ObjectManager *GetObjectManager() {
    return (ObjectManager *)object_manager_.get();
  }
  class InputManager *GetInputManager() {
    return (InputManager *)input_manager_.get();
  }
  class OutputManager *GetOutputManager() {
    return (OutputManager *)output_manager_.get();
  }

  void SetFullscreen();

 public:
  Time time_;

 private:
  std::unique_ptr<PlatformManagerInterface> platform_manager_;
  std::unique_ptr<RenderManagerInterface> render_manager_;
  std::unique_ptr<ObjectManagerInterface> object_manager_;
  std::unique_ptr<InputManagerInterface> input_manager_;
  std::unique_ptr<OutputManagerInterface> output_manager_;

  SINGLETON_HELPER(Ìı¾ıÓï);
};

}  // namespace Xn
