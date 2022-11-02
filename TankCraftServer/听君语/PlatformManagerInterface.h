#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Math.h"

namespace Xn {

enum class WindowZorder {
  Bottom,     // 将窗口置于Z序的底部
  Top,        // 将窗口置于Z序的顶部
  NoTopmost,  // 将窗口置于所有非顶层窗口之上，已经使非顶层窗口不起作用
  Topmost,    // 将窗口置于所有非顶层窗口之上，即使未被激活
  Count
};

// 平台无关化窗口类
// 约定：
//   - 在调用内部的其它函数前，应至少先执行Run函数一次，
//     并且通常Run函数不会在程序结束前返回
class PlatformManagerInterface {
 public:
  // 开始运行
  virtual int Run() = 0;

  virtual std::wstring GetWindowTitle() = 0;
  virtual Vector2Int GetPos() = 0;
  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;
  // 取得当前窗口是否处于全屏状态
  virtual const bool IsFullscreen() = 0;

  // 设置当前窗口是否处于全屏状态
  virtual void SetFullscreen(bool is_fullscreen) = 0;
  // 设置窗口在屏幕空间的Z序
  virtual void SetWindowZorder(const WindowZorder &window_zorder) = 0;
};

}  // namespace Xn
