#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "InputManagerInterface.h"
#include "InputManager-KeyboardManager.h"
#include "InputManager-MouseManager.h"
#include "SingletonBaseClass.h"

namespace Xn {

class InputManager : public InputManagerInterface {
 public:
  InputManager();

  // 通过 InputManagerInterface 继承
  virtual void OnKeyDown(const byte& key) override;
  virtual void OnKeyUp(const byte& key) override;
  virtual void OnChar(const char& character) override;
  virtual void OnMouseMove(int x, int y) noexcept override;
  virtual void OnMouseLeftPressed(int x, int y) noexcept override;
  virtual void OnMouseLeftReleased(int x, int y) noexcept override;
  virtual void OnMouseRightPressed(int x, int y) noexcept override;
  virtual void OnMouseRightReleased(int x, int y) noexcept override;
  virtual void OnMouseWheel(int x, int y, int delta) noexcept override;
  virtual void OnMouseEnter() noexcept override;
  virtual void OnMouseLeave() noexcept override;
  virtual bool IsMouseInWindow() noexcept override;
  virtual void ClearState() override;

 private:
  KeyboardManager keyboard_manager_;
  MouseManager mouse_manager_;

  CAN_NOT_COPY_THIS_CLASS(InputManager);
};

}  // namespace Xn
