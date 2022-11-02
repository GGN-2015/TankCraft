#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "ObjectManager-XnObject.h"
#include "PlatformManagerInterface.h"
#include "SingletonBaseClass.h"
#include "stdafx.h"
#include "typedef.h"

#ifdef _WIN64

class PlatformWindowCppGetInterface;

namespace Xn {

class PlatformManager : public PlatformManagerInterface {
  friend class PlatformWindowCppGetInterface;

 public:
  PlatformManager(const HINSTANCE &instance_handle, std::wstring window_title,
                  const int &width, const int &height);

  // 通过 PlatformManagerInterface 继承
  virtual int Run() override;
  virtual std::wstring GetWindowTitle() override;
  virtual Vector2Int GetPos() override;
  virtual int GetWidth() override;
  virtual int GetHeight() override;
  virtual const bool IsFullscreen() override;
  virtual void SetFullscreen(bool is_fullscreen) override;
  virtual void SetWindowZorder(const WindowZorder &window_zorder) override;

  // 取得窗口句柄
  // 样例：
  //   HWND wnd_handle = Application.Get().GetWindowHandle();
  const HWND GetWindowHandle();

 private:
  std::wstring window_title_;
  int x_;
  int y_;
  int width_;
  int height_;
  bool is_fullscreen_;

  HWND window_handle_;
  uint window_style_;
  RECT window_rect_before_fullscreen_;

  CAN_NOT_COPY_THIS_CLASS(PlatformManager);
};

}  // namespace Xn

#endif  // _WIN64
