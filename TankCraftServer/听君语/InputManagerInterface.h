#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "typedef.h"

namespace Xn {

class InputManagerInterface {
 public:
  virtual void OnKeyDown(const byte &key) = 0;
  virtual void OnKeyUp(const byte &key) = 0;
  virtual void OnChar(const char &character) = 0;

  virtual void OnMouseMove(int x, int y) noexcept = 0;
  virtual void OnMouseLeftPressed(int x, int y) noexcept = 0;
  virtual void OnMouseLeftReleased(int x, int y) noexcept = 0;
  virtual void OnMouseRightPressed(int x, int y) noexcept = 0;
  virtual void OnMouseRightReleased(int x, int y) noexcept = 0;
  virtual void OnMouseWheel(int x, int y, int delta) noexcept = 0;
  virtual void OnMouseEnter() noexcept = 0;
  virtual void OnMouseLeave() noexcept = 0;

  virtual bool IsMouseInWindow() noexcept = 0;

  virtual void ClearState() = 0;
};

}  // namespace Xn
